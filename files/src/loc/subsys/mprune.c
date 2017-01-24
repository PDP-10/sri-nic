/**************************************************************************
  MPRUNE.C

    Todd Koumrian
    SRI International - DDN Information Center
    January 29, 1989

    Usage:
        mprune [-a] [-d dest_directory] [-s size] file
            -a requests archival of pruned branches on TOPS-20 systems
            -d allows specifying a destination directory for the pruned
               branches to be written to (orig. file stays where it is).
            -s allows specifying target pruned branch file size in pages.
               The default value is 240 pages.

    Description:
        MCHUNK is a program to split MM-format mail files which have
        grown uncomfortably large into smaller, more manageable pieces.
        MCHUNK echoes the process by which Operators at the NIC
        "prune" mail files when they grow in size to near TOPS-20 MM's
        file size limit of 296 disk-pages.

    Linking:
        For TOPS-20 systems only, link with C:LIBCKX (see MCHUNK.CMD).
        Otherwise no special actions are necessary.
 ***************************************************************************/

#include <c-env.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strung.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/param.h>

#if SYS_T20
#include <jsys.h>
#include <monsym.h>

#define FB_INV monsym("FB%INV")
#define OF_RTD monsym("OF%RTD")
#define OF_NWT monsym("OF%NWT")
#define _ARRAR monsym(".ARRAR")
#define _ARSET monsym(".ARSET")
#endif

#define DEF_FILE_PAGE_MAX 240
#define NUL '\0'

#define FALSE 0
#define TRUE  1

typedef int bool;
#if SYS_T20
typedef _KCCtype_char7 f_char;      /* TOPS-20 block I/O in 7 bit, not char */
#else
typedef char           f_char;      /* Others, it's the same */
#endif

void    base_name();
void    close_file();
void    fdata();
void    get_source();
char   *monstr();
f_char *msg_scan();
int     open_file();
void    cut_branch();
void    rewrite_source();
void    prune();

bool  dflag = FALSE;
char *dest_dir;
int   dest_pages = DEF_FILE_PAGE_MAX;
long  dest_size;

char *file_name;
off_t file_size;
long  file_pages;
int   file_handle;  /* TOPS-20: int JFN, UNIX: int fd */

f_char *buffer;     /* Pointer to mail file buffer */

long  blocksize;
char  bname[MAXPATHLEN];

struct {
    char *str;
    char *code;
} months[] = {
    "Jan", "01",
    "Feb", "02",
    "Mar", "03",
    "Apr", "04",
    "May", "05",
    "Jun", "06",
    "Jul", "07",
    "Aug", "08",
    "Sep", "09",
    "Oct", "10",
    "Nov", "11",
    "Dec", "12"
};

#if SYS_T20
bool aflag = FALSE;
#endif

main(argc, argv)
int argc;
char *argv[];
{
    int arg;

    if (argc < 2) {
#if SYS_T20
        fprintf(stderr, "Usage: %s [-a] [-d dest_dir] [-s size] file\n", argv[0]);
#else
        fprintf(stderr, "Usage: %s [-d dest_dir] [-s size] file\n", argv[0]);
#endif
        exit(0);
    }

    for (arg = 1; arg < argc && argv[arg][0] == '-'; arg++) {
        if (argv[arg][2] != NUL) {
            fprintf(stderr, "Garbled switch: %s\n", argv[arg]);
            exit(1);
        }

        switch (argv[arg][1]) {
#if SYS_T20
            case 'a':
                aflag = TRUE;
                break;
#endif
            case 'd':
                dest_dir = argv[++arg];
                dflag = TRUE;
                break;

            case 's':
                if ((dest_pages = atoi(argv[++arg])) <= 0) {
                    fprintf(stderr, "?Bad size parameter\n");
                    exit(1);
                }
                break;

        default:
            fprintf(stderr, "?Unknown switch: \"%s\"\n", argv[arg]);
            exit(1);
            break;
        }
    }

    if (arg >= argc) {
        fprintf(stderr, "?No mail file specified\n");
        exit(1);
    }
    file_name = argv[arg];

    fdata(file_name);
    dest_size = dest_pages * blocksize;
    printf("Target size (pages/chars) %d/%ld\n", dest_pages, dest_size);

    if (file_pages > dest_pages) {
        printf("Processing file \"%s\"\n", file_name);
        if (dflag)
            printf("Writing to directory \"%s\"\n", dest_dir);

        get_source(file_name);
        base_name();
        prune();
        printf("Prune complete\n");
        exit(0);
    }
    else {
        printf("File is already small enough\n");
        exit(0);
    }
}

/*---------------------------------------------------------------------
  GET_SOURCE()

    Arguments: name - pathname of desired mail file to load

        Get_source() attempt to open the desired mail file, with what is
        hopefully mutual exclusion, and then allocates memory to hold
        the contents of the entire file and loads the file into it.
  ---------------------------------------------------------------------*/

void get_source(name)
char *name;
{
#if SYS_T20
    int ablock[5];
#endif

    if ((file_handle = open_file(name, FALSE)) < 0) {
        fprintf(stderr, "%s - Unable to access\n", name);
        exit(1);
    }

    fdata(name);    /* Kludge - get size again in case it changed before
                                getting exclusive access to file */

    if ((buffer = (f_char *) malloc(file_size)) == NULL) {
        fprintf(stderr, "?buffer[] malloc() failed\n");
        exit(1);
    }

#if SYS_T20
    ablock[1] = file_handle;
    ablock[2] = (int) (buffer - 1);
    ablock[3] = (int) -file_size;
    if (!jsys(SIN, ablock)) {
        fprintf(stderr, "%s - ?SIN failed\n", name);
        exit(1);
    }

#else
/* UNIX type section, even though I don't know how to synch */
/* Hopefully by now we have exclusive access to the mail file */

    if (read(file_handle, buffer, (int) file_size) < file_size) {
        perror(name);
        exit(1);
    }
#endif
}

/*-------------------------------------------------------------------------
  PRUNE()

    Prune() implements the algorithm which scans through the loaded mail
    file, and prunes off dest_size chunks via cut_branch().  When
    all the pruning is finished, the original mail file is rewritten.
  -------------------------------------------------------------------------*/

void prune()
{
    f_char *base, *bufend;
    f_char *current, *last, *next;
    int     msgs = 1;

    bufend = buffer + file_size;  /* Ptr to after last char of file */
    base = buffer;

    while (bufend - base >= dest_size) {    /* want small leftovers */

        current = base;             /* Initialize loop for first time */
        last = base;
        next = msg_scan(current);

        while (next < bufend && next - base < dest_size) {  /* msg loop */
            last = current;
            current = next;
            next = msg_scan(current);
        }

        if (next - base >= dest_size) { /* Got a regulation size chunk */
            if (current == base) {      /* Is it just a single message? */
                cut_branch(base, next - base, base);    /* Yes - force it */
                base = next;
            }
            else {                                      /* No - normal */
                cut_branch(base, current - base, last); /* So prune it off */
                base = current;                        /* Let's try it again */
            }
        }
        else break;                         /* Ran off the end of world */
    }
    rewrite_source(base, bufend - base);    /* Write leftovers to orig. file */
}

/*---------------------------------------------------------------------------
  CUT_BRANCH()

    Arguments: beg - ptr to start of the branch to prune in buffer[].
               len - length of branch.
               header - ptr to start of last message of branch (the date of
                        which is used to derive the branches filename).

    Cut_branch() first constructs the filename for this pruned branch's
    file.  The convention is that the date of the last message in the
    branch becomes the extension for the filename (BASE_NAME.YYMMDD),
    where BASE_NAME is the name of the mail file without extension (as
    found by base_name() in bname[]).  An example is (MAIL.TXT -> MAIL.881231).

    The filename is opened, contents written, file closed and released.
    On TOPS-20 systems, if the -a flag is set, the branch will be marked
    for archival.
  ---------------------------------------------------------------------------*/

void cut_branch(beg, len, header)
f_char *beg;
int     len;
f_char *header;
{
    int     branch; /* fd */
    char    pname[MAXPATHLEN];
    f_char *fptr;
    char    headbuf[100];
    char   *hptr;
    char    day[3];
    char    month[4];
    char   *mon;
    char    year[5];
#if SYS_T20
    int     ablock[5];
#endif

    fptr = header;                  /* Copy msg header to headbuf[] */
    hptr = headbuf;                 /* ...regardless of system type */
    while ((char) *fptr != '\n')
        *hptr++ = (char) *fptr++;
    
    hptr = headbuf;         /* Reset hptr to beginning of msg header */
    strcpy(pname, bname);   /* Copy over base pruned filename */
    while (isspace(*hptr))
        hptr++;

    if (isdigit(*hptr)) {
        if (isdigit (*(hptr+1))) {
            day[0] = *hptr++;
            day[1] = *hptr++;
            day[2] = NUL;
        }
        else {
            if (*(hptr+1) == '-') {
                day[0] = '0';
                day[1] = *hptr++;
                day[2] = NUL;
            }
            else {
                fprintf(stderr, "%s - ?Message has bad format (day)\n", file_name);
                exit(1);
            }
        }
    }
    else {
        fprintf(stderr, "%s - ?Message has bad header (day)\n", file_name);
        exit(1);
    }

    if (*hptr++ != '-') {
        fprintf(stderr, "%s - ?Message has bad header\n", file_name);
        exit(1);
    }

    if (!isalpha(*hptr) || !isalpha(*(hptr+1)) || !isalpha(*(hptr+2))) {
        fprintf(stderr, "%s - Message has bad header\n", file_name);
        exit(1);
    }
    month[0] = *hptr++;
    month[1] = *hptr++;
    month[2] = *hptr++;
    month[3] = NUL;

    if (*hptr++ != '-') {
        fprintf(stderr, "%s - Message has bad header\n", file_name);
        exit(1);
    }

    if (isdigit(*hptr) && isdigit(*(hptr+1))) {
        if (!isspace(*(hptr+2))) {
            if (isdigit(*(hptr+2)) && isdigit(*(hptr+3))) {
                hptr += 2;
            }
            else {
                fprintf(stderr, "%s - Message has bad header (yr)\n", file_name);
                exit(1);
            }
        }
        year[0] = *hptr++;
        year[1] = *hptr++;
        year[2] = NUL;
    }
    else {
        fprintf(stderr, "%s - Message has bad header (yr)\n", file_name);
        exit(1);
    }

    strcat(pname, ".");
    strcat(pname, year);
    if ((mon = monstr(month)) == NULL) {
        fprintf(stderr, "%s - ?Message has bad header (mon)\n", file_name);
        exit(1);
    }
    strcat(pname, mon);
    strcat(pname, day);

    printf("Pruning to file \"%s\"", pname);
    if ((branch = open_file(pname, TRUE)) < 0) {
#if SYS_T20
        fprintf(stderr, "%s - ?Couldn't open prune file\n", pname);
#else
        perror(pname);
#endif
        exit(1);
    }

#if SYS_T20                     /* TOPS-20 write */
    ablock[1] = branch;
    ablock[2] = (int) (beg - 1);
    ablock[3] = (int) -len;
    if (!jsys(SOUT, ablock)) {
        fprintf(stderr, "%s - ?SOUT failed\n", pname);
        exit(1);
    }

    if (aflag) {            /* If TOPS-20 archival requested... */
        ablock[1] = branch;
        ablock[2] = _ARRAR;
        ablock[3] = _ARSET;
        if (jsys(ARCF, ablock)) {

            ablock[1] = (_FBCTL<<18) | branch;
            ablock[2] = FB_INV;
            ablock[3] = FB_INV;
            if (jsys(CHFDB, ablock))
                printf(" (archive requested)");
            else
                fprintf(stderr, "\n%s - ?Couldn't make invisible\n", pname);
        }
        else
            fprintf(stderr, "\n%s - ?ARCF failed\n", pname);
    }

#else
    if (write(branch, beg, len) < len) {
        fprintf(stderr, "\n?Prune file write incomplete\n");  /* UNIX write */
        exit(1);
    }

#endif
    printf("\n");
    close_file(branch);     /* Generic close() routine */
}

/*-------------------------------------------------------------------------
  REWRITE_SOURCE()

    Arguments: beg - ptr to what will be the first messge in the rewritten file
               len - length of the leftovers to be re-written.

    Rewrite_source() does just that, it rewrites the original mail file
    with the block starting at beg with length len.

    The file is first closed, and then reopened for write, with a special
    hack for TOPS-20 systems to preserve restricted access.  After being
    closed, the file is released.
  --------------------------------------------------------------------------*/

void rewrite_source(beg, len)
f_char *beg;
int     len;
{
#if SYS_T20
    int ablock[5];
#endif

    printf("Rewriting \"%s\"\n", file_name);

#if SYS_T20
    ablock[1] = file_handle | CO_NRJ;   /* Close retaining JFN */
    if (!jsys(CLOSF, ablock)) {
        fprintf(stderr, "%s - ?Reading CLOSF failed\n", file_name);
        exit(1);
    }

    ablock[1] = file_handle;            /* Reopen for writing (truncs file) */
    ablock[2] = (7<<30) | OF_WR | OF_RTD | OF_NWT;
    if (!jsys(OPENF, ablock)) {
        fprintf(stderr, "%s - ?writing OPENF failed\n", file_name);
        exit(1);
    }

    ablock[1] = file_handle;            /* Output the leftovers */
    ablock[2] = (int) (beg - 1);
    ablock[3] = (int) -len;
    if (!jsys(SOUT, ablock)) {
        fprintf(stderr, "%s - ?SOUT failed\n", file_name);
        exit(1);
    }

#else
/* UNIX source mail file rewrite code goes here (should be synced but isn't) */

    if (write(file_handle, beg, len) < len) {
        fprintf(stderr, "%s - ?write() failed\n", file_name);
        exit(1);
    }
#endif

    close_file(file_handle);            /* Close file, both TOPS-20 & UNIX */
}

/*--------------------------------------------------------------------------
  MSG_SCAN() - scan ahead one message from _start

    Arguments: start - pointer to the start of the message to scan ahead of.
      Returns: pointer to the start of the following message.

    Msg_scan() allows scanning ahead one message in a message buffer
    in a system independent way.  A generic f_char pointer is returned
    pointing to the start of the next message.
  --------------------------------------------------------------------------*/

f_char *msg_scan(start)
f_char *start;
{
    f_char *c = start;
    char nbuf[10];
    char *n;
    int len;

    while ((char) *c != ',')    /* skip to comma */
        c++;
    c++;                        /* ...and skip over it */

    n = nbuf;
    while (isdigit((char) *c))
        *n++ = (char) *c++;
    *n = NUL;
    if ((char) *c != ';') {
        fprintf(stderr, "%s - message has bad message length\n", nbuf);
        exit(1);
    }

    len = atoi(nbuf);           /* convert the message length */
    while ((char) *c != '\n')   /* Now skip past header to first char of msg */
        c++;
    c++;                        /* skip the newline */

    c += len;               /* Skip past end of message and ... */
    return (c);             /* Return ptr to first char of "next" msg hdr */
}

/*-------------------------------------------------------------------------
  OPEN_FILE()

    Arguments: fname - pathname of file to open
               wrt   - boolean value indicating whether the open is for
                       reading of writing (TRUE == write, FALSE == read).
      Returns: handle for this file.

    Open_file() provides a convenient generic way to open files independent
    of what system is being used.  The idea is to open with mutual exclusion,
    though as yet the UNIX open's don't guarantee it.
  -------------------------------------------------------------------------*/

int open_file(fname, wrt)
char *fname;
bool wrt;
{
    int hdl;
#if SYS_T20
    int ablock[5];

    ablock[1] = GJ_SHT | (wrt ? GJ_FOU : GJ_OLD);
    ablock[2] = (int) (fname - 1);
    if (!jsys(GTJFN, ablock))
        return (-1);

    hdl = ablock[1] & RH;
    ablock[1] = hdl;
    ablock[2] = (7<<30) | (wrt ? OF_WR : OF_RD) | OF_RTD | OF_NWT;
    if (!jsys(OPENF, ablock))
        return (-1);
    else return (hdl);

#else
    if ((hdl = open(name, (wrt ? (O_CREAT | O_WRONLY) : O_RDWR))) < 0)
        return (-1);
    else return (hdl);
#endif
}

/*------------------------------------------------------------------------
  CLOSE_FILE()

    Arguments: hdl - handle of file to close.

    Close_file() generically closes an open file.  With TOPS-20 systems,
    it releases the JFN as well.
  ------------------------------------------------------------------------*/

void close_file(hdl)
int hdl;
{
#if SYS_T20
    int ablock[5];

    ablock[1] = hdl;
    if (!jsys(CLOSF, ablock)) {
        fprintf(stderr, "?Unable to close file for JFN %d\n", hdl);
        exit(1);
    }

#else
    close(hdl);
#endif
}

/*------------------------------------------------------------------------
  FDATA()

    Arguments: name - filename to obtain data about.

    Fdata() performs a stat() call on name, and sets appropriate
    file_ global variables with relevant info.  Also, blocksize is
    set with the contents of st_blksize in the hopes of finding out
    how large a disk ("page" | "block") is.
  ------------------------------------------------------------------------*/
void fdata(name)
char *name;
{
    struct stat statbuf;

    if (stat(name, &statbuf) < 0) {
        perror(name);
        exit(1);
    }

    file_size  = statbuf.st_size;
    file_pages = statbuf.st_blocks;
    blocksize  = statbuf.st_blksize;
}

/*------------------------------------------------------------------------
  BASE_NAME()

    Base_name() takes the filename of the source mail file (file_name),
    and strips off directory and extension info to just get the filename.
    This is stored in global bname[].
  ------------------------------------------------------------------------*/

void base_name()
{
    char *p, *f;
    int off;

    if (dflag)                      /* Was dest directory specified? */
        strcpy(bname, dest_dir);    /* Yes - copy it over */
    else bname[0] = NUL;

    p = file_name;                  /* Get mail file's name */

#if SYS_T20
    if ((off = strpos(p, ':')) >= 0)    /*Look for TOPS20 Device */
        p += (off + 1);                 /* Skip past it */

    if ((off = strpos(p, '<')) >= 0) {  /* Look for TOPS20 directory */
        p += (off + 1);
        if ((off = strpos(p, '>')) < 0) {   /* Rgt angle-bracket missing */
            fprintf(stderr, "%s - ?Bad dir format\n");
            exit(1);
        }
        else p += (off + 1);            /* Found it, skip it */
    }
#else
    if ((off = strrpos(p, '/')) >= 0)   /* Check for UNIX directory */
        p += (off + 1);                 /* Want only filename */
#endif

    f = bname + strlen(bname);          /* Copy filename without extension */
    while (*p != NUL && *p != '.')
        *f++ = *p++;
    *f = NUL;
}

char *monstr(month)
char *month;
{
    int index = 0;

    while (index < 12 && strCMP(months[index].str, month))
        index++;

    if (index < 12)
        return (months[index].code);
    else return (NULL);
}
