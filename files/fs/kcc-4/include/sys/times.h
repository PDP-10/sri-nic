/* SYS/TIMES.H - definitions for the times() system call simulation.
**	Times are in units of 1/HZ seconds where HZ is 60.
**	BSD claims this is obsoleted by getrusage(2).
**
**	Note that the elements of the tms structure in BSD are declared
** as "time_t", but that would conflict with the new ANSI definition
** (a date/time value, not a process time value), so we use something
** different here, namely "long" -- that is what V7 used anyway.
*/
#ifndef HZ
#define HZ 60
#endif

struct tms {
	/* time_t */ long	tms_utime;	/* user time */
	/* time_t */ long	tms_stime;	/* system time */
	/* time_t */ long	tms_cutime;	/* user time, children */
	/* time_t */ long	tms_cstime;	/* system time, children */
};
