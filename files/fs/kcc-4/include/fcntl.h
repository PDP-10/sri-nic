/*
 *	FCNTL.H - command definitions for fcntl() file information
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
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
#if SYS_T20+SYS_10X
#define F_GETSYSFD	7	/* Get internal system handle for FD, which
				   is a JFN on TOPS-20/TENEX */
#define F_GETBYTESIZE	8	/* Get byte-size file was opened in */
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

#endif /* not _FCNTL_INCLUDED */
