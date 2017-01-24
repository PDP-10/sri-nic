/*								-*-C-*-
 *	UIO.H - Unix runtime definitions for I/O calls
 */

#ifndef _UIO_INCLUDED
#define _UIO_INCLUDED 1

#include <stdio.h>

#define UIO_FIRST_CH	3		/* first free chan (0-2 predefined) */
#define OPEN_MAX 	64		/* Max FDs we can have open */
#if SYS_ITS
#define OPEN_UFX_MAX	16	/* Max UFXs (channels) we can support */
#else
#define OPEN_UFX_MAX	64		/* Max UFXs we can support */
#endif
#define UIO_UNASS	0		/* Value of unassigned FD slot */

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
 *	Storage for per-Unix-file stuff.  All initially zero.
 */

extern int _uionopen[];		/* # of FDs which have this UFX open */
extern int _uioch[];		/* Storage for sys I/O chans or JFNs */
extern int _uiotype[];		/* OS device type     (== major device #) */
extern int _uiodnum[];		/* USYS device number (== minor device #) */
extern int _uiobsize[];		/* byte size of open */
extern int _uiocnt[];		/* count of chars left in buffer */
extern char *_uiocp[];		/* current pointer into buffer */
extern char *_uiopbuf[];	/* pointer to start of buffer */
extern int _uioflgs[];		/* flag bits */
extern long _uiopos[];		/* current position within file */
extern int _uioeof[];		/* EOF on channel */
#if SYS_ITS
#define UIO_ZBUFSIZ	4	/* Kludge for byte packing on ITS */
extern int _uiozcnt[];
extern int _uiozbuf[][UIO_ZBUFSIZ];
extern int _uiobword[];		/* _uiobword[ufx] = (36 / _uiobsize[ufx]) */
#endif /* SYS_ITS */

/* _UIOFLGS flag bits are defined in <fcntl.h> so that users can access
** them via the fcntl() F_GETFL function.
*/
#include <fcntl.h>		/* Get defs for flags */
#define _UIO_CONVERTED	FDF_CVTEOL		/* force CRLF-NL convert */
#define _UIO_OLD	FDF_OLDFILE		/* this is an old file */
#define _UIO_READ	FDF_READ		/* open for read */
#define _UIO_WRITE	FDF_WRITE		/* open for write */
#define _UIO_HANDPACK	FDF_HANDPACK		/* handpacking bytes on ITS */

/*
 *	function declarations
 */

int open(), close(), read(), write();
long lseek();
long _bufpos();
void exit(), _exit();

/* Internal declarations */
char *_getbuf();	/* Get an I/O buffer */
void _frebuf();		/* Free it */

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

#endif /* not _UIO_INCLUDED */
