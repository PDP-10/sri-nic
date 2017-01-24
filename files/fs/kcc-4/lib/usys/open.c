/*
 *	OPEN - URT lowest-level file-opening
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 *
 *	Also provides _gtjfn() and _rljfn().
 */

#include "c-env.h"
#include "errno.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "sys/usytty.h"
#include "sys/file.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif
#include "string.h"
#include "strung.h"

#define MAX_LINE	(512)		/* size of a GP string buffer */

struct filename {			/* for C:sys/xxx.h */
    char path[FILENAME_SIZE];		/* "sys" */
    char device[FILENAME_SIZE];		/* "C" - could be a logical! */
    char directory[FILENAME_SIZE];	/* NULL */
    char filename[FILENAME_SIZE];	/* "xxx.h" */
};

extern int _gtfdb();		/* used by various routines, defined in stat */

static int _get_tricky();	/* tricky logical name expander for _get_jfn */
static int _get_jfn();		/* work-horse for getting a file */
static int __gtjfn();		/* lowest level gtjfn, which does the GTJFN% */

static void parse_filename();	/* parse up a TOPS-20 filespec */
static void make_filename();	/* construct a TOPS-20 filespec from pieces */
static int log_expand();	/* expand a logical device */

#if SYS_10X+SYS_T20
extern int _dvtype();
static int _openf();
#endif

/* See the UIODAT.C module for definitions of the internal I/O tables
** and comments on their use.
*/

int
creat(path, mode)
char *path;
int mode;
{
    return open(path, (O_WRONLY | O_CREAT | O_TRUNC), mode);
}

int
open(path, flags, mode)
char *path;
int flags, mode;
{
    int fd, ufx;
    int byte_size;
#if SYS_T20+SYS_10X
    int jfn;
#endif

    USYS_BEG();
    if ((fd = _uiofd()) == -1 || !(ufx = _uiofx()))
	USYS_RETERR(EMFILE);	/* Failed to get FD or UFX slot */

#if SYS_T20+SYS_10X
/*
 *	This would be the point to translate the protection modes
 *	and pass to the GTJFN% as a ";Pnnnnnn" suffix for use if
 *	creating a new file (or version).  Or just "nnnnnn" for long
 *	form GTJFN call.
 *
 *  tnxprot(mode, &protstr);
 */
    if (!(jfn = _gtjfn(path, flags)))
	USYS_RETERR(ENOENT);		/* Failed to get JFN for file */
/*
 *	check to see if this is an old file, or a new one.  if FB_NXF
 *	is set then the file is new and doesn't exist (until closed)
 */
    _uioch[ufx] = jfn;		/* Store the JFN */

    if (!(_gtfdb(jfn, _FBCTL) & FB_NXF))
	_uioflgs[ufx] |= _UIO_OLD;
/*
 *	now open the file
 */
    if (!(_openf(ufx, flags))) {
	_rljfn(jfn);		/* oops, failed!  release the old JFN */
	USYS_RETERR(ENOENT);	/* and lose-return */
    }
#else
#error open() not supported for this system.
#endif

    _openufx(ufx, flags);	/* Set up UFX with given flags */
    _uioufx[fd] = ufx;		/* Set up UFX mapping for the FD */
    USYS_RET(fd);
}

/* _OPENUFX - Auxiliary used by open() and pipe() to share common code.
**	_uioch and _uioflags must already be set.
**	_uiopbuf is either NULL or points to an allocated buffer for re-use.
**	Sets:
**		_uioflags (OR'd in)
**		_uiotype
**		_uiocp
**		_uiocnt
**		_uiopos
**		_uioeof
**		_uionopen
*/
_openufx(ufx, flags)
int ufx, flags;
{
    /* Set R/W flags */
    if (flags & O_RDWR) _uioflgs[ufx] |= (_UIO_READ | _UIO_WRITE);
    else _uioflgs[ufx] |= ((flags & O_WRONLY) ? _UIO_WRITE : _UIO_READ);

    /* Set buffer variables */
#if 0
    if (_uiopbuf[ufx]) {
	_uiocp[ufx] = _uiopbuf[ufx] - 1;
	_uiocnt[ufx] = (flags & O_WRONLY) ? UIO_BUFSIZ : 0;
    } else {
#elif 0
    if (_uiopbuf[ufx])
	_panic("opening unreleased IOB");
#endif
    _uiocp[ufx] = NULL, _uiocnt[ufx] = 0;

    _uioeof[ufx] = 0;		/* Clear EOF flag */
    if (flags & O_APPEND) {	/* for append file, read starting position */
	int ablock[5];
	ablock[1] = _uioch[ufx];
	_uiopos[ufx] = (jsys(RFPTR, ablock)) ? ablock[2] : 0;
    } else _uiopos[ufx] = 0;	/* else starts at 0 */

    _uiotype[ufx] = _dvtype(_uioch[ufx]);	/* get dev type for chan/JFN */
    _uiodnum[ufx] = 0;				/* Assume 0 for minor dev # */
    if (_uiotype[ufx] == _DVTTY) {	/* If TTY, fix up device number */
	int i;
	if (_uioch[ufx] == _CTTRM)	/* Always use 0 for control TTY */
	    i = 0;
	else {				/* If a random TTY, pick another */
	    for (i = 1; _ttys[i].tt_ufx != 0;)
		if (++i >= _NTTYS) {
#define TOOMANYTTYS "\npanic: trying to open too many terminal devices.\n"
		    write(2, TOOMANYTTYS, sizeof(TOOMANYTTYS)-1);
		    abort();
		}
	}

	/* i now has index into _ttys[] */
	if (_ttys[i].tt_ufx == 0) {
	    _ttys[i] = _deftty;		/* Initialize whole struct! */
	    _ttys[i].tt_ufx = ufx;	/* Point back to UFX */
	}
	_uiodnum[ufx] = i;		/* Point to tty struct # */
    }

    /* OK, all ready to go!  Set confirmation that UFX is open, and map FD */
    _uionopen[ufx] = 1;		/* UFX is open, with only one FD */
}

/*
 *	int _uiofd();
 *
 *	find a spare FD slot and return the FD, else -1 if none.  this
 *	routine is exported, so don't make it static.
 */

int _uiofd()
{
    int fd;

    for (fd = 0; fd < OPEN_MAX; fd++)
	if (!_uioufx[fd])
	    return fd;
    errno = EMFILE;
    return -1;
}

/*
 *	int _uiofx();
 *
 *	find a spare UFX slot and return the UFX, else 0 if none.  this
 *	routine is exported, so don't make it static.
 *
 *	Note a ufx index of 0 is NEVER seen!  The tables this index addresses
 *	have length OPEN_MAX+1, and a valid return value can be from 1 to
 *	OPEN_MAX inclusive.
 */

int _uiofx()
{
    int ufx;

    for (ufx = 0; ++ufx <= OPEN_MAX;)
	if (_uionopen[ufx] <= 0) {	/* If found a free slot */
	    _uionopen[ufx] = 0;		/* Make sure marked not active */
	    _uioflgs[ufx] = 0;		/* Also ensure nothing open */
	    return ufx;
	}
    errno = EMFILE;		/* No free slots, fail */
    return 0;
}

#if SYS_T20+SYS_10X
/*
 *	get a jfn on a pathname.
 */

int _gtjfn(path, flags)
char *path;
int flags;
{
    int jfn;
    struct filename file;
    char tried[MAX_LINE];			/* devices already tried */

    parse_filename(path, &file);		/* parse up the filename */
    if (!*file.path)				/* if no unix-style path */
	return _get_jfn(path, flags);		/* spec, then this is easy */
    *tried = '\0';				/* nothing tried yet */
    return _get_tricky(&file, tried, 1, flags);	/* alas, do tricky things */
}

/*
 *	tricky logical-device follower.  this traces out logical devices
 *	and generates full paths from the different end-directories,
 *	keeping at it until it finds the file.  this routine is not needed
 *	unless foo/ style filespecs are being used.
 *
 *	tried[] is a buf that _get_tricky stores devices it has tried into,
 *	so that it won't re-try anything and potentially get into a loop.
 */

static int _get_tricky(file, tried, job_wide_flag, flags)
struct filename *file;
char *tried;
int job_wide_flag;
{
    char expansion[MAX_LINE], o_device[FILENAME_SIZE], token[FILENAME_SIZE];
    char new_path[MAX_LINE], *cp, *p, *next;
    int jfn;

    if (!log_expand(file->device, expansion, job_wide_flag)) {
	make_filename(new_path, file);		/* not a logical device */
	return _get_jfn(new_path, flags);	/* so just look for it. */
    }
    strcpy(o_device, file->device);		/* save original device */
    for (cp = expansion; *cp; cp = ++next) {	/* loop over expansion. */
	next = strchr(cp, ',');			/* find separator */
	if (next) *next = '\0';			/* tie this one off */
	if (p = strchr(cp, '<')) {		/* if got a directory */
	    *strchr(cp, ':') = '\0';		/* kill the colon */
	    strcpy(file->device, cp);		/* device from expansion */
	    *strchr(p, '>') = '\0';		/* kill trailing > */
	    strcpy(file->directory, ++p);	/* directory from expansion */
	    make_filename(new_path, file);	/* make up the new path */
	    if (jfn = _get_jfn(new_path, flags))
		return jfn;			/* got it! */
	} else {
	    *strchr(cp, ':') = '\0';		/* kill the colon */
	    job_wide_flag = (strCMP(o_device, cp) != 0); /* same device on */
	    *token = (job_wide_flag) ? '*' : '$'; /* right side means sys */
	    strcpy(token + 1, cp);		/* construct device token */
	    if (!strSTR(tried, token)) {	/* if haven't tried this */
		strcat(tried, token);		/* we're trying it now */
		strcpy(file->device, cp);	/* try this device now */
		if (jfn = _get_tricky(file, tried, job_wide_flag, flags))
		    return jfn;
	    }
	}
    }
    return 0;					/* not found... */
}

/*
 *	do the real work of finding a file
 */

static int _get_jfn(path, flags)
char *path;
int flags;
{
    int jfn, bits;

    bits = (flags & O_T20_WILD) ? GJ_IFG : 0;
    if (flags & (O_RDWR | O_WRONLY | O_APPEND)) {
/*
 *	Normally we always create a new version for writing.
 *	The O_T20_WROLD flag exists in order to override this and
 *	act like UNIX if such behavior is ever desirable (ugh).
 *	Note that if O_APPEND or O_RDWR is set, we always look
 *	for an existing file first.
 */
	if (!(flags & (O_APPEND | O_RDWR | O_T20_WROLD)))
	    return __gtjfn(path, GJ_FOU);
	else if (jfn = __gtjfn(path, GJ_OLD)) {
	    if ((flags & O_CREAT) && (flags & O_EXCL)) {
		_rljfn(jfn);
		errno = EEXIST;
		return 0;
	   }
	} else if (!(flags & O_CREAT) || !(jfn = __gtjfn(path, GJ_FOU))) {
	    errno = ENOENT;
	    return 0;
	} else return jfn;
    } else return __gtjfn(path, ((flags & O_T20_WILD) ? GJ_IFG : 0) | GJ_OLD);
}

/*
 *	lowest level actual GTJFN% call
 */

static int __gtjfn(path, flags)
char *path;
int flags;
{
    int arg_block[5];

    arg_block[1] = GJ_SHT | flags;
    arg_block[2] = (int) (path - 1);
    return (jsys(GTJFN, arg_block)) ? arg_block[1] : 0;
}

/*
 *	take apart a TOPS-20 filespec and put the pieces in the given
 *	(struct filename) structure.
 */

static void parse_filename(path, file)
char *path;
struct filename *file;
{
    char *start, *part;
    int c, length;

    *file->path = *file->device = *file->directory = *file->filename = '\0';

    do {					/* loop over pathname */
	start = path;				/* cp to start of part */
	length = 0;				/* length of part */
	part = NULL;				/* what this part is */
	while (!part) switch (c = *path++) {
	    case '\26':	path++; length++; break;	/* ^V quote */
	    case '\0':	part = file->filename; break;
	    case ':':	part = file->device; break;
	    case '<':	start = path; continue;
	    case '>':	part = file->directory; break;
	    case '/':	part = file->path; break;
	    default:	length++;
	}
	strncpy(part, start, length);
	part[length] = '\0';
    } while (c);
}

/*
 *	given a (struct filename) structure, construct a TOPS-20 filename
 *	from the pieces
 */

static void make_filename(buf, file)
char *buf;
struct filename *file;
{
    *buf = '\0';			/* initialize buffer always */
    if (*file->device) {
	strcat(buf, file->device);
	strcat(buf, ":");
    }
    if (*file->directory || *file->path) {
	strcat(buf, "<");
	if (*file->directory)
	    strcat(buf, file->directory);
	strcat(buf, ".");
	if (*file->path)
	    strcat(buf, file->path);
	strcat(buf, ">");
    }
    if (*file->filename) strcat(buf, file->filename);
}

/*
 *	expand the logical device *from (which should not include the
 *	trailing colon) to *to.  if (job_wide) then a local expansion is
 *	tried first, else just system-wide.
 */

static int log_expand(from, to, job_wide)
char *from, *to;
int job_wide;
{
    int ablock[5];
    char *p;

    ablock[2] = (int) (from - 1);
    ablock[3] = (int) (to - 1);
    if (job_wide) {				/* not system-logical only? */
	ablock[1] = _LNSJB;			/* job-wide */
	if (jsys(LNMST, ablock)) return 1;	/* local expansion worked */
    }
    ablock[1] = _LNSSY;				/* system-wide */
    return jsys(LNMST, ablock);
}

#define old_file(ufx)	(_uioflgs[ufx] & _UIO_OLD)
#define new_file(ufx)	(!(_uioflgs[ufx] & _UIO_OLD))

static int
_openf(ufx, flags)
int ufx, flags;
{
    int byte_size;
    int open_mode = 0;
    int arg_block[5];

/*
 *	convert file.h flags into OPENF% bit flags
 */
    switch (flags & (O_RDONLY | O_WRONLY | O_RDWR | O_APPEND)) {
	case O_WRONLY:	open_mode = OF_WR; break;
	case (O_WRONLY | O_APPEND):
			open_mode = OF_WR | OF_APP; break;
	case O_RDWR:	open_mode = OF_RD | OF_WR | OF_PLN; break;
	case (O_RDWR | O_APPEND):
			open_mode = OF_RD | OF_WR | OF_APP | OF_PLN; break;
	case O_APPEND:	open_mode = OF_APP; break;
	default:	open_mode = OF_RD | OF_PLN;
    }
/*
 *	you can't open an existing file for write-only (not read-write),
 *	and not have it be truncated.  sorry, kids.
 */
    if (old_file(ufx) && (open_mode == OF_WR) && !(flags & O_TRUNC)) {
	errno = ETRUNC;			/* new T20 error code! */
	return 0;
    }
/*
 *	to avoid truncating an existing file, you need to open it in
 *	r/w mode, even if you just want write.
 */
    if (old_file(ufx) && !(flags & (O_TRUNC | O_APPEND)))
	open_mode |= OF_RD;
/*
 *	If a byte size is explicitly requested, use that.
 *	Otherwise:
 *		for a new file, use 9 if O_BINARY, else 7.
 *		for an old file, use the file bytesize.
 *			A size of 0 or 36 is handled as for a new file.
 *			Any other size is simply used - if this is not
 *			one of 7, 8, or 9 then the results are unpredictable.
 */
    switch (flags & O_BSIZE_MASK) {
	case O_BSIZE_7: byte_size = 7; break;
	case O_BSIZE_8: byte_size = 8; break;
	case O_BSIZE_9: byte_size = 9; break;
	default:	/* Error, but nothing we can do about it now. */
	case 0:
	    if (new_file(ufx))
		byte_size = (flags & O_BINARY) ? 9 : 7 ;
	    else {
		byte_size = (_gtfdb(_uioch[ufx],_FBBYV) << FBBSZ_S) & FBBSZ_M;
		if (byte_size == 0 || byte_size == 36)
		    byte_size = (flags & O_BINARY) ? 9 : 7;
	    }
    }
    _uiobsize[ufx] = byte_size;			/* save for posterity */
    if (flags & O_T20_THAWED) open_mode |= OF_THW;	/* thawed access? */
/*
 *	if they explicitly request converted i/o, then propagate the flag,
 *	else if they DIDN'T explicitly request UNconverted (e.g. it's up to
 *	us to decide), and the file is being opened for text in 7-bit bytes,
 *	then go for conversion.
 */
    if ((flags & O_CONVERTED) ||
	(byte_size == 7 && !(flags & (O_UNCONVERTED | O_BINARY))))
	_uioflgs[ufx] |= _UIO_CONVERTED;

    arg_block[1] = _uioch[ufx];
    arg_block[2] = (byte_size << 30) + open_mode;
    return jsys(OPENF, arg_block);
}

int _rljfn(jfn)
int jfn;
{
    int arg_block[5];

    arg_block[1] = jfn;
    return jsys(RLJFN, arg_block);
}
#endif

#if 0	/* OLD STUFF - Kept pending integration */

#if SYS_T20+SYS_10X

static char *
tnxprot(uprot, str)
int uprot;
char *str;
{
    char *s = str;
    *s++ = ';';
    *s++ = 'P';
    f = modprot(mode >> 6);		/* user prots */
    *s++ = (f >> 3) + '0';
    *s++ = (f & 7) + '0';
    f = modprot(mode >> 3);		/* group prots */
    *s++ = (f >> 3) + '0';
    *s++ = (f & 7) + '0';
    f = modprot(mode);		/* other prots */
    *s++ = (f >> 3) + '0';
    *s++ = (f & 7) + '0';
    return str;
}

/* Unix -> T20/10X file protection conversion
*/
static int
modprot(mod)
int mod;
{
    int prot = 0;
    if (mod & 1) prot |= 012;		/* execute (and list) access */
    if (mod & 2) prot |= 027;		/* write and append access */
    if (mod & 4) prot |= 042;		/* read (and list) access */
    return prot;
}
#endif

#if 0
creat(name,prot) == _cfile(name,prot,7,0);	/* ASCII */
bcreat(name,prot) == _cfile(name,prot,8,0);
icreat(name,prot) == _cfile(name,prot,36,0);

open(name,mode) == _ofile(name,mode,7);		/* ASCII */
bopen(name,mode) == _ofile(name,mode,8);
iopen(name,mode) == _ofile(name,mode,36);

/* Open for writing (create) */
_cfile(filename, unixprot, bytesize, appendflg)	- Create file, write.
  (for ITS this is just _ofile(filename, O_WRONLY, bytesize) - no append)
  (for WAITS, ditto ITS)
  (T20/10X basically did _ofile too, but noticed protection and bytesize)
/* Open for reading */
_ofile(filename, unixmode, bytesize) - read file	; unixmode is O_ flags.
	for T20, fileparse then GTJFN/OPENF.
#endif

#if SYS_T20+SYS_10X

/* --------------------------------------------------------------------- */
/*      jacket routine for GTJFN% to mung filenames into submission      */
/* --------------------------------------------------------------------- */

int _gtjfn(name, flags)
char *name;
{
    char dirpart[80], filpart[80], *dirptr, *filptr, *_dirst();
    int anydir, indir;			/* marker for dir part changed */

    anydir = 0;				/* dir part remains default */
    indir = 0;				/* assume not in directory part */
    dirpart[0] = 'D';			/* start off with directory part */
    dirpart[1] = 'S';			/* pointing to "DSK:" */
    dirpart[2] = 'K';
    dirpart[3] = ':';
    dirptr = &dirpart[3];		/* point to just before the end */
    filptr = &filpart[-1];		/* of dir and file parts */

    while (1) {
	switch (*name) {
	case '\0':				/* run out of chars? */
	    *++filptr = 0;			/* yes, null terminate */
	    filptr = &filpart[-1];		/* start at top again */
	    while (*++dirptr = *++filptr) ;	/* append to dir part */
	    return gtjfn_(dirpart, flags);	/* do low level lookup */

	case ':':
	    if (anydir) return -1;	/* already have dir, lose */
	    anydir = 1;			/* now we have one */
	    *++filptr = '\0';		/* terminate file part */
	    dirptr = &dirpart[-1];	/* start at top */
	    filptr = &filpart[-1];	/* top of filename part */
	    while (*++dirptr = *++filptr); /* copy across */
	    *dirptr = ':';		/* put the colon on */
	    filptr = &filpart[-1];	/* reinitialize file pointer */
	    break;

	case '[':
	case '<':
	    *++filptr = '<';		/* open bracket becomes angle */
	    indir = 1;			/* remember we're in dir part */
	    break;

	case '.':
	    if (indir < 0) *++filptr = '\026'; /* quote extra dots */
	    else if (indir == 0) indir = -1; /* only allow one in file */
	    *++filptr = '.';		/* add the dot */
	    break;

	case '>':
	case ']':
	    if (*dirptr != ':') return -1; /* already have dir, lose */
	    anydir = 1;			/* remember we have a dir and dev */
	    indir = 0;			/* no longer in dir part */
	    *++filptr = '\0';		/* terminate file part */
	    filptr = &filpart[-1];	/* top of filename part */
	    while (*++dirptr = *++filptr); /* copy across */
	    *dirptr = '>';		/* add the close bracket */
	    filptr = &filpart[-1];	/* reinitialize file pointer */
	    break;

	case '\026':			/* control-V? */
	case '\\':			/* or quoteing backslash? */
	    *++filptr = '\026';		/* yes, add it */
	    *++filptr = *++name;	/* and the next char */
	    break;

	case '/':			/* slash UNIX dir delimiter? */
	    indir = 0;			/* yes, no longer in dir part */
	    *(dirptr+1) = '\0';		/* yes, terminate directory part */
	    *++filptr = '\0';		/* and filename part */
	    switch (filpart[0]) {	/* check out first part of name */
	    case '.':			/* period? */
		switch (filpart[1]) {	/* yes, look at what follows */
		case '\0':		/* ./x ? */
		    name++;		/* yes, skip slash */
		    filptr = &filpart[-1]; /* start again in file part */
		    continue;		/* and ignore this dir part */

		default:
		    return -1;		/* .x/y loses */

		case '\026':		/* maybe ..? */
		    break;		/* yes, handle outside switch */
		}

		/* dir starts with .. - hack superdirectory */
		if (filpart[2] != '.' || filpart[3] != '\0') return -1;
		if (*dirptr != '>') {	/* do we have a real dir part? */
		    dirptr = _dirst(dirpart); /* no, fix it up */
		    if (dirptr == NULL) return -1; /* lost */
		}
		while (*--dirptr != '.' && *dirptr != ':') ; /* find delim */
		if (*dirptr == '.') {	/* found subdir start? */
		    *dirptr = '>';	/* turn into dir end */
		    name++;		/* skip over slash */
		    filptr = &filpart[-1]; /* forget .. */
		    continue;		/* on with the show */
		}
		/* no subdirs, move back to dev:<root-dir> */

	    case '\0':			/* no name, want root dir */
		if (*dirptr != ':') return -1; /* must have only dev */
		filptr = "<ROOT-DIRECTORY" - 1; /* point to dir part */
		break;

	    default:
		if (*dirptr != '>') {
		    dirptr = _dirst(dirpart); /* fix up filename */
		    if (dirptr == NULL) return -1; /* lost */
		}
		*dirptr = '.';		/* drop period over close bracket */
		filptr = &filpart[-1];	/* normal name, just use it */
	    }
	    while (*++dirptr = *++filptr) ; /* append name */
	    *dirptr = '>';		/* and close bracket */
	    filptr = &filpart[-1];	/* restart file part */
	    break;

	default:
	    *++filptr = *name;		/* normal char, add to file part */
	    break;
	}
	name++;				/* move on to next char */
    }
}

static char *
_dirst()
{
#asm
; *************************************************************
;	Get channel (JFN), return in AC1
; *************************************************************

; Here to canonicalize dir name
; Not sure if this is really needed for anything on TENEX.

#if SYS_T20
	MOVE	1,[RC%EMO]	;Forcing exact match
	%CHRBP	2,-1(17)	;From string pointer given as arg
	RCDIR%			;Get directory number in AC3
	 ERJMP	$RETF
	TDNE	1,[RC%NOM]	;Matched?
	 JRST	$RETF		;No, fail
	%CHRBP	1,-1(17)	;From string
	MOVE	2,3		;With directory number
	DIRST%			;Make string for dir number
	 ERJMP	$RETF
#endif /* T20 */
#if SYS_10X
	SETZ 1,			; Force exact match
	%CHRBP	2,-1(17)	; for string pointer given as arg
	STDIR%			; Get directory number in AC1(RH)
	 ERJMPA	$RETF		; No match
	 ERJMPA $RETF		; Ambiguous
	HRRZ 2,1		; Set up dir number
	%CHRBP	1,-1(17)	; Output to this string
	DIRST%			; Make string for dir number
	 ERJMPA	$RETF
#endif /* 10X */
	POPJ	17,		;Did it, return with updated pointer
#endasm
}
#endif /* T20+10X */

#if SYS_WAITS

/* ------------------------------- */
/*      handler for file open      */
/* ------------------------------- */

static int
_ofile(ufx, name, flags, uprot)
char *name;
int flags, uprot;
{
    int channel,f,device,disk;
    int out, byte_size;
    struct filespec fs;
	struct openspec {
	    int status;
	    int device;
	    int obuf : 18;	/* LH */
	    int ibuf : 18;	/* RH */
	} ospec;

    if (flags & O_RDWR) return -1;
    else if (flags & O_WRONLY) out = 1;
    else out = 0;

    if (*name == ':') {
	name++;
	device = sixbit(name);
	disk = 0;
    }    
    else
	disk = device = sixbit("DSK");

    if (!convert(name,&fs)) return -1;

    switch (flags & O_BSIZE_MASK) {
	case O_BSIZE_7: byte_size = 7; break;
	case O_BSIZE_8: byte_size = 8; break;
	case O_BSIZE_9: byte_size = 9; break;
	default:	/* Error, but nothing we can do about it now. */
	case 0:
	    if (out)
		byte_size = (flags & O_BINARY) ? 9 : 7 ;
	    else /* Try to set from existing mode -- give up at moment */
		byte_size = (flags & O_BINARY) ? 9 : 7 ;
    }

    if (byte_size == 7)
	ospec.status = 0;	/* data mode 0 */
    else
	ospec.status = 010;	/* data mode 010 */
    ospec.device = device;

    if (out) {
        ospec.ibuf = 0;
        ospec.obuf = (int)&buffers[ufx];
	if (disk) {
	    if (!muuo(MUUO_ENTER, f, &fs)) return -1;
	}
	_out(f);
    } else {
        ospec.ibuf = (int)&buffers[ufx];
	ospec.obuf = 0;
	if(!muuo(MUUO_OPEN, f, &ospec)) return -1;
	if (disk) {
	    if (!muuo(MUUO_LOOKUP,f,&fs)) return -1;
	    _in(f);
	}
    }
    return 0;
}

/* --------------------------------------------------- */
/*	obtain a sixbit representation for string      */
/* --------------------------------------------------- */

static sixbit(s)
char *s;
{
    int n,w,t;
    w = 0;
    n = 30;
    while (*s && n >= 0) {
	t = *s++;
	if (t & 0100) t = t | 040; else t = t & 0137;
	w += (t&077) << n;
	n -= 6;
    }
    return w;
}

/* ----------------------------------------------------------------------- */
/*	convert a filename string to internal filespec representation      */
/* ----------------------------------------------------------------------- */

static convert(s,t)
char *s; struct filespec *t;
{
    char u[16],*v;
    int  state;

    state = 1;
    t->name = t->extension = t->date = t->PPN = 0;
    while (state) {
	v = u;
	while (!fend(*s)) *v++ = *s++;
	*v = 0;
	switch (state) {
	case 1:
	    t->name = sixbit(u);
	    switch (*s) {
	    case 0:
	        state = 0;
		break;
	    case '.':
	        state = 2;
		break;
	    case '[':
	        state = 3;
		break;
	    default:
	        return 0;
	    }
	    s++;
	    break;
	case 2:
	    t->extension = sixbit(u) & 0777777000000;
	    switch (*s) {
	    case 0:
	        state = 0;
		break;
	    case '[':
	        state = 3;
		break;
	    default:
	        return 0;
	    }
	    s++;
	    break;
	case 3:
	    t->PPN = rightjust(sixbit(u));
	    switch (*s) {
	    case ',':
	        state = 4;
		break;
	    default:
		return 0;
	    }
	    s++;
	    break;
	case 4:
	    t->PPN += (rightjust(sixbit(u)) >> 18);
	    state = 0;
	    break;
	default:
	    return 0;
	}
    }
    return 1;
}

static fend(c)
{
    if (c == 0 || c == '.' || c == '[' || c == ',' || c == ']') return 1;
    return 0;
}

static rightjust(n)
{
    if (n = n & 0777777000000) while (!(n & 077000000)) n = n >> 6;
    return n;
}
#endif /* WAITS */

#if SYS_ITS
#include "sysits.h"

_ofile(ufx, name, umode, uprot)
char *name;
int umode,bsize;
{	int fd, res, modchn;
	char *bp;

	if((modchn = getchn()) < 0)
		return(-1);		/* No ITS chans available */
	switch(umode)
	  {	case 0:			/* Read */
			break;		/* Leave LH zero (.UAI) */
		case 1:			/* Write */
			modchn |= (1<<18);	/* Set .UAO */
			break;
		case 2:			/* Reading and Writing */
			return(-1);	/* Sorry, don't support this */
	  }

	--name;				/* Back up ptr to make proper BP */
	res = SYSCAL2(SC_NAM("sopen"),SC_ARG(&modchn),SC_ARG(&name));
	if(res) return(-1);		/* Error of some kind */
	_uioch[fd] = modchn&017;
	return(fd);
}
#endif /* ITS */

#endif	/* Commented-out stuff */
