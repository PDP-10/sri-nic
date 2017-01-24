/*
** Various typedef names expected by some calls.
** These are system dependent; the values here are for a PDP-10 system.
*/

#ifndef _TYPES_INCLUDED
#define _TYPES_INCLUDED

/* Convenient type abbreviations, apparently some code uses these */
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;		/* ??? sys III compat ??? */

typedef	char *	caddr_t;	/* Core address */
typedef	int	daddr_t;	/* Disk address */
typedef int	dev_t;		/* Device type (stat(2)) */
typedef int	ino_t;		/* Inode # (T20: disk addr in stat(2)) */
typedef	int	off_t;		/* Disk file size (offset?) */
typedef	int	size_t;		/* ?? */
#ifndef _TIME_T		/* Avoid clash with ANSI <time.h> */
#define _TIME_T
typedef	long	time_t;		/* Time value (# seconds since 1/1/70) */
#endif

#endif /* _TYPES_INCLUDED */
