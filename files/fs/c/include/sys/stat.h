/* <SYS/STAT.H> - definitions for stat(2), fstat(2)
**
**	(c) Copyright Ken Harrenstien 1989
**
**	As described by 4.3BSD UPM.  Note that not all of the stat
**	structure is meaningful for a particular system.
**	Some extended components were added for the KCC-only
**	xstat and xfstat calls.
*/

#ifndef _SYS_STAT_INCLUDED
#define _SYS_STAT_INCLUDED

struct	stat
{
    dev_t st_dev;	/* Device inode is on */
			/*   T20: device designator */
			/*   T10: .RBDEV - SIXBIT device name */
    ino_t st_ino;	/* Inode number */
			/*   T20: .FBADR - Disk addr of file index blk*/
			/*   T10: .RBPOS - not sure if this is right */
    unsigned st_mode;	/* File type and protection */
			/*   T20: protection translated to Un*x format */
			/*   T10: protection translated to Un*x format */
    int	st_nlink;	/* # of "links" to file */
			/*   T20: 1 always */
			/*   T10: 1 always */
    int	st_uid;		/* User ID of owner */
			/*   T20: User #, 10X: directory # */
			/*   T10: .RBAUT - PPN of creator */
    int	st_gid;		/* Group ID of owner */
			/*   T20: 0 always */
			/*   T10: (LH of st_uid) */
    dev_t st_rdev;	/* Device type (if inode is device) */
			/*   T20: Device info (DVCHR% val, includes .DVxxx) */
			/*   T10: Device type (DEVTYP val, includes TY.xxx) */
    off_t st_size;	/* Size of file, in bytes */
			/*   T20: .FBSIZ - (note bytesize can vary!) */
			/*   T10: .RBSIZ (# wds) converted to # bytes */
    time_t st_atime;	/* Last Access time, in Un*x format */
			/*   T20: .FBREF - last ref */
			/*   T10: max(.RBEXT,st_ctime) access date (midnite) */
    time_t st_mtime;	/* Last Modify time, in Un*x format */ 
			/*   T20: .FBWRT - last write open */
			/*   T10: .RBPRV creation date/time (mins) */
    time_t st_ctime;	/* Last status change time, in Un*x format */
			/*   T20: .FBCRE - last write close */
			/*   T10: max(.RBTIM, st_mtime) */
    long st_blksize;	/* Best file system I/O blocksize (in bytes) */
			/*   T20: # bytes in a page (derived from FB%BSZ) */
			/*   T10: 0200 wds, converted to # bytes */
    long st_blocks;	/* Actual # blocks allocated for file */
			/*   T20: # pages in file (FB%PGC of .FBBYV) */
			/*   T10: .RBEST - # blocks in file */
    long st_spares[4];	/* Spare wds for possible future additions */
};

/* Flags in st_mode.
**	Only S_IFMT and the permission flags are implemented.
*/
#define	S_IFMT	0170000		/* type of file */
#define    S_IFDIR	0040000	/* directory */
#define    S_IFCHR	0020000	/* device: character special */
#define    S_IFBLK	0060000	/* device: block special */
#define    S_IFREG	0100000	/* regular file */
#define    S_IFLNK	0120000	/* symbolic link */
#define    S_IFSOCK	0140000	/* socket */
#define	S_ISUID	0004000		/* set user id on execution */
#define	S_ISGID	0002000		/* set group id on execution */
#define	S_ISVTX	0001000		/* save swapped text even after use */
#define	S_IREAD	0000400		/* read permission, owner */
#define	S_IWRITE 0000200	/* write permission, owner */
#define	S_IEXEC	0000100		/* execute/search permission, owner */

/* KCC-dependent stuff */

/* Support for "extended stat" -  xstat() and xfstat().
** These are pretty much TOPS-20 specific at the moment.
*/
struct xstat {
    struct stat st;			/* include original structure */
    union {				/* device-dependent portion */
	struct {
	    int state;			/* connection state */
	    int fhost;			/* foreign host# */
	    int fport;			/* foreign port# */
	} tcp;
	struct {
	    int version;		/* version# of file */
	    int pagcnt;			/* count of # pages in file */
	    int bytsiz;			/* byte size of file */
	} disk;
    } dev_dep;
};

#define xst_fhost	dev_dep.tcp.fhost
#define xst_fport	dev_dep.tcp.fport
#define xst_state	dev_dep.tcp.state
#define xst_version	dev_dep.disk.version
#define xst_pagcnt	dev_dep.disk.pagcnt
#define xst_bytsiz	dev_dep.disk.bytsiz

#endif /* ifndef _SYS_STAT_INCLUDED */
