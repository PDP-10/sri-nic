/* <SYS/FILE.H> - definitions for access(2), lseek(2), open(2)
**
**	(c) Copyright Ken Harrenstien 1989
**
**	These definitions emulate 4.3BSD, with some KCC additions.
*/

#ifndef _SYS_FILE_INCLUDED
#define _SYS_FILE_INCLUDED

/*
**	access(2) mode flags
*/

#define F_OK	0	/* File exists? */
#define X_OK	1	/* Execute (or dir search) access? */
#define W_OK	2	/* Write access? */
#define R_OK	4	/* Read access? */

/*
**	lseek(2) "whence" values
*/

#define L_SET	0	/* Set pointer (seek from 0) */
#define L_INCR	1	/* Increment pointer (seek from current loc) */
#define L_XTND	2	/* Extend file size (seek from EOF) */

/*
**	open(2) mode flags
*/
	/* BSD4.x flags (07077) */
#define O_RDONLY	(0)		/* Open for reading only */
#define O_WRONLY	(01)		/* Open for writing only */
#define O_RDWR		(02)		/* Open for reading and writing */
#define O_NDELAY	(04)		/* Don't block on open */
#define O_APPEND	(010)		/* Append on each write */
/*			(060)*/		/* 2 bits reserved */
#define O_CREAT		(01000)		/* Create file if it does not exist */
#define O_TRUNC		(02000)		/* Truncate size to 0 */
#define O_EXCL		(04000)		/* Error if create and file exists */
	/* KCC specific flags (0170700) */
#define O_BINARY	(0100)		/* Open in binary mode (sys-dep) */
#define O_CONVERTED	(0200)		/* Forced conversion requested */
#define O_UNCONVERTED	(0400)		/* Forced NO conversion requested */
#define O_BSIZE_MASK	(070000)	/* Mask: Force specified byte size */
#define  O_BSIZE_7	(010000)	/*   Bytesize value: 7-bit */
#define  O_BSIZE_8	(020000)	/*   Bytesize value: 8-bit */
#define  O_BSIZE_9	(030000)	/*   Bytesize value: 9-bit */
#define O_SYSFD		(0100000)	/* Open using "system FD" (T20: JFN) */

	/* OS specific flags (077,,0) */
	/* Distinct systems could overlay each other's flags. */
#define O_T20_WILD	(1<<18)		/* Allow wildcards on GTJFN% */
#define O_T20_WROLD	(1<<19)		/* For writes, do NOT use GJ%FOU */
#define O_T20_SYS_LOG	(1<<20)		/* Logical device is system-wide! */
#define O_T20_THAWED	(1<<21)		/* Open file for thawed access */

#define O_ITS_IMAGE	(1<<22)		/* Force image mode */
#define O_ITS_NO_IMAGE	(1<<23)		/* Force no image mode */

#endif /* ifndef _SYS_FILE_INCLUDED */
