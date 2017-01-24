/* <SYS/TYPES.H> - common typedefs for USYS calls.
**
**	(c) Copyright Ken Harrenstien 1989
**
**	Names are those used by V7 and BSD; the types, however, are
**	system dependent and the definitions here assume a PDP-10 system.
*/

#ifndef _SYS_TYPES_INCLUDED
#define _SYS_TYPES_INCLUDED

/* Convenient type abbreviations, apparently some code uses these */
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
#if 0
typedef	unsigned short	ushort;		/* ???ugh??? sys III/V compat ??? */
#endif

typedef	char *	caddr_t;	/* Core address */
typedef	int	daddr_t;	/* Disk address */
typedef int	dev_t;		/* Device type (stat(2)) */
typedef int	ino_t;		/* Inode # (T20: disk addr in stat(2)) */
typedef	int	off_t;		/* Disk file size (offset?) */

#ifndef _SIZE_T_DEFINED		/* Avoid clash with ANSI <stddef.h> */
#define _SIZE_T_DEFINED
typedef	int	size_t;		/* Type big enough to hold any "sizeof" val */
#endif
#ifndef _TIME_T_DEFINED		/* Avoid clash with ANSI <time.h> */
#define _TIME_T_DEFINED
typedef	long	time_t;		/* Time value (# seconds since 1/1/70) */
#endif

#endif /* _TYPES_INCLUDED */
