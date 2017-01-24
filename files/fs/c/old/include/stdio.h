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

struct _siostream {
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
};

#define FILE struct _siostream

/*
 *	sio2cnt, sio2cp are used only for input streams (for push-back);
 *	we're reusing the sio2cnt word for output streams, to keep an
 *	alternate buffer-count.  for linebuffering, the primary count is
 *	kept <=0 so that fputc is always called; fputc knows about line-
 *	buffering, and uses the alternate count word, siolcnt.
 */

#define siolcnt	sio2cnt

/*
 *	Internal flags - contents of sioflgs
 */

#define _SIOF_READ	(01)		/* stream is open for reading */
#define _SIOF_WRITE	(02)		/* writing */
#define _SIOF_APPEND	(04)		/* appending */
#define _SIOF_UPDATE	(010)		/* updating ('+' fopen flag) */
#define _SIOF_MODIFIED	(020)		/* buffer has been modified */

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

/*
 *	A file that is open is open for either read or write (or append,
 *	which is a superset of write), so openness is checked for by
 *	read/write-ness, using this mask:
 */

#define _SIOF_OPEN	(_SIOF_READ | _SIOF_WRITE)

#define BUFSIZ	(512)
#define NULL	(0)
#define EOF	(-1)

/*
 *	STDIO calls which are implemented as macros go here
 */

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

int fclose(), fflush(), fgetc(), fprintf(), fputc(), fputs();
int fread(), fscanf(), fseek(), fwrite(), printf(), puts();
int scanf(), sprintf(), sscanf(), ungetc();
long ftell();
char *fgets(), *gets();
FILE *makeFILE(), *fopen(), *fdopen(), *freopen(), *sopen();
void clearerr();
void _sexit(), rewind(), setbuf(), _setbuf(), setlinebuf();

#endif
