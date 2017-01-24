/*								-*-C-*-
 *	FCNTL.H - command definitions for fcntl() file information
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 *	Edits for ITS:  Copyright (C) 1988 Alan Bawden
 */

#ifndef _FCNTL_INCLUDED
#define _FCNTL_INCLUDED 1

#include "c-env.h"

#define	F_DUPFD		0	/* Duplicate FD */
#define	F_GETFD		1	/* Get the close-on-exec flag */
#define	F_SETFD		2	/* Set the close-on-exec flag */
#define	F_GETFL		3	/* Get descriptor status flags */
#define	F_SETFL		4	/* Set descriptor status flags */
#define	F_GETOWN 	5	/* Get process ID or group */
#define F_SETOWN 	6	/* Set process ID or group */
#if SYS_T20+SYS_10X+SYS_ITS
#define F_GETSYSFD	7	/* Get actual system FD (T20: JFN) for FD */
#define F_GETBYTESIZE	8	/* Get byte-size file was opened in */

/* NOT IMPLEMENTED YET (may never be; see O_SYSFD in <sys/file.h>) */
#define F_SETSYSFD	9	/* Set/make a FD given system FD (T20: JFN) */
#endif

/* File descriptor status flags.
**	Keep BSD etc flags in the low 9 bits to avoid conflicts with internal
**	or KCC-specific flags.
*/
#define FAPPEND	01	/* Force each write to append at EOF */
#define FASYNC	02	/* Enable SIGIO signal when I/O possible (TTY only) */
#define FNDELAY	04	/* Non-blocking I/O (TTY only) */

/* KCC-specific flags.  Keep these out of the low 9 bits. */
#define FDF_CVTEOL	01000	/* Force CRLF<->LF conversion */
#define FDF_OLDFILE	02000	/* This is an old file */
#define FDF_READ	04000	/* Open for reading */
#define FDF_WRITE	010000	/* Open for writing */
#define FDF_HANDPACK	020000	/* Packing/unpacking bytes by hand (only */
				/* seen under ITS) */ 

#endif /* not _FCNTL_INCLUDED */
