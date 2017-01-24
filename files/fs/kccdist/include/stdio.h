/*
 *	STDIO - Include Header file.
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#ifndef _STDIO_INCLUDED
#define _STDIO_INCLUDED 1

#ifndef _SIO_NPBC	/* # of push-back characters that ungetc guarantees */
#define _SIO_NPBC 1
#endif

/*
 *	Standard I/O Stream structure
 */

#define FILE struct _siostream

struct _siostream {
    int siocheck;		/* 0 if inactive, else magic values */
    int siocnt;			/* # chars left in buffer */
    char *siocp;		/* Char pointer into buffer */
    char *siopbuf;		/* Pointer to start of buffer */
    int siolbuf;		/* Length of buffer */
    int sioflgs;		/* Various flags */
    int siofd;			/* FD for stream, if any */
    long siofdoff;		/* FD's offset into file b4 last r/w syscall */
    int sioocnt;		/* original buffer count after filbuf */
    int sioerr;			/* If nonzero, error # for stream */
    int sio2cnt;		/* Saved siocnt and */
    char *sio2cp;		/* siocp (during pushback) */
    char siopbc[_SIO_NPBC];	/* Pushed-back characters */
    FILE *sionFILE;		/* pointer to next and... */
    FILE *siopFILE;		/* ...previous FILE blocks in chain */
    int siospare[4];		/* spare words for hassleless expansion */
};

/*
 *	sio2cnt, sio2cp are used only for input streams (for push-back);
 *	we're reusing the sio2cnt word for output streams, to keep an
 *	alternate buffer-count.  for linebuffering, the primary count is
 *	kept <= 0 so that fputc is always called; fputc knows about line-
 *	buffering, and uses the alternate count word, siolcnt.
 */

#define siolcnt	sio2cnt

/*
 *	Internal flags - contents of sioflgs
 *	Some of these are used while parsing the flag chars for
 *	fopen/freopen/sopen.  These are indicated by '<char>' in the comment.
 */

#define _SIOF_READ	01		/* 'r' stream is open for reading */
#define _SIOF_WRITE	02		/* 'w' ...writing */
#define _SIOF_APPEND	04		/* 'a' ...appending */
#define _SIOF_UPDATE	010		/* '+' ...updating */
#define _SIOF_MODIFIED	020		/* buffer has been modified */
#define _SIOF_BINARY	040		/* 'b' binary data stream (not text) */
#define _SIOF_PBC	0100		/* Using push-back count and pointer */
#define _SIOF_EOF	0200		/* Stream has hit EOF */
#define _SIOF_STR	0400		/* Stream is a string, siofd unused */
#define _SIOF_BUF	01000		/* Stream is buffered */
#define _SIOF_LASTBUF	02000		/* Current buffer is last one */
#define _SIOF_LINEBUF	04000		/* Flush output buffer on EOL */
#define _SIOF_AUTOBUF	010000		/* Dynamically allocate buffer, pls */
#define _SIOF_GROWBUF	020000		/* String-buf automatically expands */
#define _SIOF_DYNAMFILE	040000		/* FILE block was stdio-allocated */
#define _SIOF_DYNAMBUF	0100000		/* I/O buffer was stdio-allocated */
#define _SIOF_CONVERTED	0200000		/* 'C' CRLF-EOL conversion forced */
#define _SIOF_UNCONVERTED 0400000	/* '-C' forced NO CRLF conversion */
#define _SIOF_THAWED	01000000	/* 'T' Thawed access */
/*
 *	a valid FILE block returned by _makeFILE() has its siocheck word
 *	set to _SIOF_FILE.  after the stream has been opened, the check
 *	word is changed to _SIOF_OPEN.
 */

#define _SIOF_OPEN	(((-'O' * 'P' * 'E' * 'N') << 16) + 'S' + 'I' + 'O')
#define _SIOF_FILE	(((-'F' * 'I' * 'L' * 'E') << 16) + 'S' + 'I' + 'O')

/*
 *	Constants
 */

#define BUFSIZ		512
#define NULL		0
#define EOF		(-1)

/* setvbuf type argument defs */
#define _IOFBF		0	/* full buffering */
#define _IOLBF		1	/* line buffering */
#define _IONBF		2	/* no buffering */

/* fseek type argument defs */
#define SEEK_SET	0	/* starting point is start-of-file */
#define SEEK_CUR	1	/* ...current position */
#define SEEK_END	2	/* ...or end-of-file */

/*
 *	implementation-dependent constants here
 */

#define SYS_OPEN	32	/* we guarantee this many files open at once */
#define TMP_MAX		0777777	/* we guarantee this many distinct filenames */
#define L_tmpnam	512		/* Max size of temp filename strings */
#define L_ctermid	L_tmpnam	/* terminal id could be big filespec */
#define L_cuserid	40		/* max size of username */

/*
 *	STDIO calls which are implemented as macros go here
 */

#define clearerr(f)	((f)->sioerr = 0)
#define feof(f)		((f)->sioflgs & _SIOF_EOF)
#define ferror(f)	((f)->sioerr)
#define fileno(f)	((f)->siofd)
#define getc(f)		((--(f)->siocnt >= 0) ? (*++(f)->siocp) : fgetc(f))
#define getchar()	getc(stdin)
#define putc(c, f) \
		(--(f)->siocnt >= 0 ? (*++(f)->siocp = (c)) : fputc((c), (f)))
#define putchar(c)	putc((c), stdout)

/*
 *	for name disambiguation
 */

#define setbuffer	_setbuf

/*
 *	Standard streams, always predefined
 */

extern FILE _sios[];

#define stdin  (&_sios[0])
#define stdout (&_sios[1])
#define stderr (&_sios[2])

/*
 *	Declarations
 */

extern int fclose(), fflush(), fgetc(), fprintf(), fputc(), fputs();
extern int fread(), fscanf(), fseek(), fwrite(), printf(), puts();
extern int scanf(), sprintf(), sscanf(), ungetc();
extern long ftell();
extern char *fgets(), *gets();
extern FILE *fopen(), *fdopen(), *freopen(), *sopen();
extern void _cleanup(), rewind(), setbuf(), setbuffer(), setlinebuf();

/* New ANSI functions */
extern int setvbuf(), vfprintf(), vprintf(), vsprintf();
extern int remove(), rename();
extern FILE *tmpfile();
extern char *tmpnam(), *mktemp();

/*
 *	Gut declarations, only used by STDIO internals.
 *	Probably move these out to a "sio.h" file someday.
 */

extern FILE *_FILE_head;
extern int _filbuf(), _readable(), _writeable(), _prime(), _sioflags();
extern FILE *_makeFILE();
extern void _freeFILE(), _setFILE();

/* Stuff for new ftell/fseek scheme, only compiled if _SIOP_BITS defined. */
#define _SIOP_MAXBITS 10		/* Use constant size for now */

#ifdef _SIOP_MAXBITS
#define _SIOP_BITS(f)	_SIOP_MAXBITS		/* # bits for buffer size */
#define _SIOP_MASK(f) ((1<<_SIOP_MAXBITS)-1)	/* Mask for buffer size */
#else
#define _SIOP_BITS(f)	(f)->siopbits		/* # bits for buffer size */
#define _SIOP_MASK(f)	(f)->siopmask		/* Mask for buffer size */
#endif

#endif /* ifndef _STDIO_INCLUDED */
