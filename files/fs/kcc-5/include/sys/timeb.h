/*
** Structure filled out by ftime() system call.
**	Changed from BSD description, which used time_t the wrong way
**	and would otherwise be inefficient.  Use gettimeofday() instead.
*/
struct timeb
{
	long time;		/* Time since epoch (1/1/70) in seconds */
	int millitm;		/* ms since above time (up to 1000) */
	int timezone;		/* local timezone in minutes west of GMT */
	int dstflag;		/* If set, DST applies locally */
};
