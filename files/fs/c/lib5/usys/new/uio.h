/*
 *	UIO.H - Unix runtime definitions for I/O calls
 */

#ifndef UIO_INCLUDED

#define UIO_INCLUDED

#include "stdio.h"

#define UIO_FIRST_CH	3		/* first free chan (0-2 predefined) */
#define OPEN_MAX 	64		/* Max FDs we can have open */
#define UIO_UNASS	0		/* Value of unassigned FD slot */

#define BLOCK_SIZE	2		/* # of pages in a file-map-block */
#define N_BUFFERS	2		/* # of buffers in pre-read chain */

#if (CPU_PDP10)
#define INT_SIZE_BITS	36				/* bits/int */
#define PAGE_SIZE_INT	512				/* ints/page */
#define PAGE_SIZE_CHAR	(PAGE_SIZE_W * sizeof(int))	/* chars/page */
#endif

#define STDIN_CH	0
#define STDOUT_CH	1
#define STDERR_CH	2

/*
 *	The UIO buffer must be at LEAST twice the size of the higher-level
 *	STDIO buffer, for the ftell() crockery to work.  See bufsiz() and
 *	ftell() for the gory details.
 */

#define UIO_BUFSIZ	(BUFSIZ*2)	/* lowest-level buffer size */


/*
 *	Storage for per-FD stuff.  Just a map into true UFX index.
 */

extern int _uioufx[];			/* Indexed by FD, gets UFX! */

/*
 *	Storage for per-Unix-file (ufx) stuff.  All initially zero.
 */

extern int _uionopen[];			/* # of FDs which have this UFX open */
extern int _uioch[];			/* Storage for sys I/O chans or JFNs */
extern int _uiotype[];			/* device type for channel */
extern int _uiocnt[];			/* count of chars left in buffer */
extern char *_uiocp[];			/* current pointer into buffer */
extern char *_uiopbuf[];		/* pointer to start of buffer */
extern int _uioflgs[];			/* flag bits */
extern long _uiopos[];			/* current position within file */
extern int _uioeof[];			/* EOF on file */

/*
 *	the following are only used for disk files, for page mapping
 */

extern int _uioszb[];			/* size in bytes of file */
extern int _uioszp[];			/* size in pages of file */
extern int _uiolfp[];			/* Last File Page read, -1 if none */
extern int _uiobi[];			/* Buffer Index or current map buf */
extern int _uioiu[];			/* # of buffers in use */
extern int _uiopm[][N_BUFFERS];		/* Page Map, page in mem of fil buf */
extern int _uiofp[][N_BUFFERS];		/* correspond File Page of map */

/*
 *	these are flag bits kept by uio routines in _uioflgs[]
 */

#define _UIO_CONVERTED		(01)		/* force CRLF-NL convert */
#define _UIO_OLD		(02)		/* this is an old file */
#define _UIO_READ		(04)		/* open for read */
#define _UIO_WRITE		(010)		/* open for write */
#define _UIO_PRE_READ		(020)		/* pre-read disk files */
#define _UIO_NEW_POS		(040)		/* at a new pos in file */

/*
 *	function declarations
 */

int open(), close(), read(), write();
long lseek();
long _bufpos();
void exit(), _exit();

#if SYS_T10+SYS_WAITS
struct	bufhead {			/* structure for buffer headers */
	int	system;			/*       used by stdio ?? */
	char	*user;			/*       deposit pointer */
	int	count;			/*       space remaining in buffer */
};

struct	filespec {			/* structure for opening files */
	int	name;			/*       filename */
	int	extension;		/*       extension */
	int	date;			/*       date ?? */
	int	PPN;			/*       project-programmer num */
};

extern struct bufhead _buffers[UIO_MAXFILE]; /* file I/O buffers */
extern int _uioeof;			/* end of file reached on terminal */
#endif /* SYS_T10+SYS_WAITS */

#endif
