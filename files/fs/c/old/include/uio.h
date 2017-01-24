/*
 *	UIO.H - Unix runtime definitions for I/O calls
 */

#ifndef UIO_INCLUDED

#define UIO_INCLUDED

#include "stdio.h"

#define UIO_FIRST_CH	3		/* first free chan (0-2 predefined) */
#define UIO_MAXFILE 	32		/* Max FDs we can have open */
#define UIO_UNASS	-1		/* Value of unassigned FD slot */

/*
 *	The UIO buffer must be at LEAST twice the size of the higher-level
 *	STDIO buffer, for the ftell() crockery to work.  See bufsiz() and
 *	ftell() for the gory details.
 */

#define UIO_BUFSIZ	(BUFSIZ*2)	/* lowest-level buffer size */

/*
 *	pre-defined standard-I/O channels
 */

#define STDIN_CH	0
#define STDOUT_CH	1
#define STDERR_CH	2

/*
 *	initial flag assortment for standard-io streams
 */

#define STDIN_FLAGS	(_SIOF_READ|_SIOF_BUF|_SIOF_LINEBUF|_SIOF_AUTOBUF)
#define STDOUT_FLAGS	(_SIOF_WRITE|_SIOF_BUF|_SIOF_LINEBUF|_SIOF_AUTOBUF)
#define STDERR_FLAGS	(_SIOF_WRITE)

extern int _uioch[UIO_MAXFILE];		/* Storage for I/O chans or JFNs */
extern int _uiotype[UIO_MAXFILE];	/* device type for channel */
extern int _uiocnt[UIO_MAXFILE];	/* count of chars left in buffer */
extern char *_uiocp[UIO_MAXFILE];	/* current pointer into buffer */
extern char *_uiopbuf[UIO_MAXFILE];	/* pointer to start of buffer */
extern int _uioflgs[UIO_MAXFILE];	/* flag bits */
extern long _uiopos[UIO_MAXFILE];	/* current position within file */
extern int _uioeof[UIO_MAXFILE];	/* EOF hit while reading from TTY */

extern FILE _sios[UIO_FIRST_CH];	/* standard I/O streams: stdin, etc */

/*
 *	these are flag bits kept by uio routines in _uioflgs[]
 */

#define _UIO_RAW	01		/* raw mode, no newline-CRLF conv. */
#define _UIO_OLD	02		/* this is an old (existing) file */
#define _UIO_READ	04		/* open for read */
#define _UIO_WRITE	010		/* open for write */

/*
 *	function declarations
 */

int open(), close(), read(), readln(), write(), lseek();
long bufpos();

#endif
