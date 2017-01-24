/* <SYS/TIMES.H> - definitions for V7 times(2).
**
**	(c) Copyright Ken Harrenstien 1989
**
**	Times are in units of 1/HZ seconds where HZ is 60.
**	BSD claims this is obsoleted by getrusage(2).
**
**	Note that the elements of the tms structure in BSD are declared
** as "time_t", but that would conflict with the new ANSI definition
** (a date/time value, not a process time value), so we use something
** different here, namely "long" -- that is what V7 used anyway.
*/

#ifndef _SYS_TIMES_INCLUDED
#define _SYS_TIMES_INCLUDED

#ifndef HZ
#define HZ 60
#endif

struct tms {
	/* time_t */ long	tms_utime;	/* user time */
	/* time_t */ long	tms_stime;	/* system time */
	/* time_t */ long	tms_cutime;	/* user time, children */
	/* time_t */ long	tms_cstime;	/* system time, children */
};

#endif /* ifndef _SYS_TIMES_INCLUDED */
