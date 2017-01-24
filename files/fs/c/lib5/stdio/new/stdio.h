/*
 *	STDIO - Include Header file.
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#ifndef STDIO_INCLUDED

#define STDIO_INCLUDED

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
    int siosinc;		/* increment for string buf auto expansion */
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
 */

#define _SIOF_READ	(01)		/* stream is open for reading */
#define _SIOF_WRITE	(02)		/* ...writing */
#define _SIOF_APPEND	(04)		/* ...appending */
#define _SIOF_UPDATE	(010)		/* ...updating ('+' fopen flag) */
#define _SIOF_MODIFIED	(020)		/* buffer has been modified */
#define _SIOF_BINARY	(040)		/* binary data stream (not text) */
#define _SIOF_PBC	(0100)		/* Using push-back count and pointer */
#define _SIOF_EOF	(0200)		/* Stream has hit EOF */
#define _SIOF_STR	(0400)		/* Stream is a string, siofd unused */
#define _SIOF_BUF	(01000)		/* Stream is buffered */
#define _SIOF_LASTBUF	(02000)		/* Current buffer is last one */
#define _SIOF_LINEBUF	(04000)		/* Buffer is filled line by line */
#define _SIOF_AUTOBUF	(010000)	/* Dynamically allocate buffer, pls */
#define _SIOF_GROWBUF	(020000)	/* String-buf automatically expands */
#define _SIOF_DYNAMFILE	(040000)	/* FILE block was stdio-allocated */
#define _SIOF_DYNAMBUF	(0100000)	/* I/O buffer was stdio-allocated */
#define _SIOF_CONVERTED	(0200000)	/* CRLF-newline conversion forced */
#define _SIOF_UNCONVERTED \
			(0400000)	/* forced NO newline-CRLF conversion */

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

#define BUFSIZ		(512)
#define NULL		(0)
#define EOF		(-1)

#define _IOFBF		0	/* full buffering */
#define _IOLBF		1	/* line buffering */
#define _IONBF		2	/* no buffering */

#define SEEK_SET	0	/* starting point is start-of-file */
#define SEEK_CUR	1	/* ...current position */
#define SEEK_END	2	/* ...or end-of-file */

/*
 *	implementation-dependent constants here
 */

#define SYS_OPEN	32	/* we guarantee this many files open at once */
#define TMP_MAX		0777777	/* we guarantee this many distinct filenames */
#define L_tmpnam	(40*5)	/* maximum size of a filename in chars */

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

extern FILE _sios[], *_FILE_head;

#define STDIN_CH	0
#define STDOUT_CH	1
#define STDERR_CH	2

#define stdin  (&_sios[STDIN_CH])
#define stdout (&_sios[STDOUT_CH])
#define stderr (&_sios[STDERR_CH])

/*
 *	Declarations
 */

int fclose(), fflush(), fgetc(), fprintf(), fputc(), fputs();
int fread(), fscanf(), fseek(), fwrite(), printf(), puts();
int scanf(), sprintf(), sscanf(), ungetc();
long ftell();
char *fgets(), *gets();
FILE *fopen(), *fdopen(), *freopen(), *sopen();
void _cleanup(), rewind(), setbuf(), setbuffer(), setlinebuf();

/*
 *	Gut declarations, only used by STDIO internals.
 *	Probably move these out to a "sio.h" file someday.
 */

int _filbuf(), _readable(), _writeable(), _prime(), _sioflags(), _expand();
FILE *_makeFILE();
void _freeFILE(), _setFILE();

/*
 *	ANSI declarations
 */
/* Not there yet
int setvbuf();
FILE *tmpfile();
char *tmpnam();
*/
#endif
