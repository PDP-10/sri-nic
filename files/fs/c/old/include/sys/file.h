/*
 *	Various file definitions
 */

/*
 *	open(2) flags
 */

#define	O_RDONLY	(0)		/* open for reading */
#define	O_WRONLY	(01)		/* open for writing */
#define	O_RDWR		(02)		/* open for read & write */
#define	O_NDELAY	(04)		/* non-blocking open */
#define	O_APPEND	(010)		/* append on each write */
#define	O_CREAT		(01000)		/* open with file create */
#define	O_TRUNC		(02000)		/* open with truncation */
#define	O_EXCL		(04000)		/* error on create if file exists */

#define O_WILD		(010000)	/* allow wildcards */
#define O_CONVERTED	(020000)	/* forced conversion requested */
#define O_UNCONVERTED	(040000)	/* forced NO conversion requested */

/*
 *	flock(2) flags
 */

#define	LOCK_SH		1	/* shared lock */
#define	LOCK_EX		2	/* exclusive lock */
#define	LOCK_NB		4	/* don't block when locking */
#define	LOCK_UN		8	/* unlock */

/*
 *	access(2) flags
 */

#define	F_OK		0	/* does file exist */
#define	X_OK		1	/* is it executable by caller */
#define	W_OK		2	/* writable by caller */
#define	R_OK		4	/* readable by caller */

/*
 *	lseek(2) flags
 */

#define	L_SET		0	/* absolute offset */
#define	L_INCR		1	/* relative to current offset */
#define	L_XTND		2	/* relative to end of file */
