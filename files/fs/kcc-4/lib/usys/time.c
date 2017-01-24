/* TIME		- Get date/time value, of type "time_t", as per ANSI spec.
** GETTIMEOFDAY - Get date and time.  Simulation of 4.2BSD system call.
** FTIME	- obsolete form of above system call.
**
**	Copyright (c) 1987 by Ken Harrenstien, SRI International
**
** Also the following non-standard functions:
**	tadl_get()	Routine to return current date/time in local format.
**	tadl_to_utime	Routine to convert local TAD into Unix time_t.
**	tadl_from_utime	Routine to convert Unix time_t into local format.
** These are currently defined by <sys/time.h> as
**	_tadl_t, _tadlg(), _tadl2u(), _tadu2l().
**
** Note that none of these calls use the USYS_ macros;
** nothing they do is vulnerable to interrupts, and they do not set errno.
*/

#include "c-env.h"
#include "time.h"		/* For time_t and struct tm definition */
#include "sys/sitdep.h"		/* For local timezone/DST default values */
#include "sys/time.h"		/* For timeval and timezone structures */
#include "sys/timeb.h"		/* For timeb structure */

#if SYS_T20+SYS_10X
#include "jsys.h"		/* For ODCNV */
#include "sys/t20tim.h"		/* For _t20_tm structure for ODCNV */
#endif

time_t
time(tloc)
time_t *tloc;
{
#if 1				/* Fast method */
    if (tloc) return (*tloc = tadl_to_utime(tadl_get()));
    else return tadl_to_utime(tadl_get());
#else				/* General method */
    struct timeval tv;
    struct timezone tz;

    if (gettimeofday(&tv, &tz))		/* Get date/time and timezone */
	return -1;			/* Failed */
    if (tloc) *tloc = tv.tv_sec;	/* Return the date/time */
    return tv.tv_sec;
#endif
}

int
ftime(tp)
struct timeb *tp;
{
    struct timeval tv;
    struct timezone tz;

    if (gettimeofday(&tv, &tz))
	return -1;			/* Non-zero return means failed */
    tp->time = tv.tv_sec;
    tp->millitm = tv.tv_usec / 1000;	/* Sigh */
    tp->timezone = tz.tz_minuteswest;
    tp->dstflag = tz.tz_dsttime;
    return 0;				/* Won */
}

/* Default values for local timezone and DST type */
static int _ltzmins = _SITE_TIMEZONE;		/* Local timezone in minutes */
static int _ltzsecs = _SITE_TIMEZONE * 60;	/* Local timezone in seconds */
static int _ltdstt = _SITE_DSTTIME;	/* Local DST type (a _DST_xxx value) */
static int _ltzknown =
#if SYS_T20+SYS_10X
	0;			/* Systems where OS knows timezone */
#else
	1;			/* Doesn't, so assume defaults are OK */
#endif

int
gettimeofday(tp,tzp)
struct timeval *tp;
struct timezone *tzp;
{
    if (!_ltzknown) {
#if SYS_T20+SYS_10X
	/* This code gets the local timezone information from the monitor
	** and attempts to find out what kind of DST should be applied.
	** It tests whether July 4,1976 and Jan 4,1976 have DST applied;
	** normally the first will and the second won't, implying we are
	** using U.S. type DST (_DST_USA).  If the first doesn't, we assume
	** DST is never applied (_DST_OFF); if both have DST, then we assume
	** DST is always applied (_DST_ON).
	*/
	union {
	    unsigned ac[5];
	    struct {
		int junk[2];	/* Ignore acs 0 and 1 */
		struct _t20_tm ttm;	/* TNX broken-down time in acs 2-4 */
	    } odval;
	} acblk;
	static struct _t20_tm chkdst = {
			1976, 7-1, 4-1, 0 };	/* July 4, 1976 had DST */

	acblk.odval.ttm = chkdst;	/* Set time to check for */
	jsys(IDCNV, acblk.ac);		/* Convert that time */
	acblk.ac[4] = acblk.ac[3];	/* Get stupid result in right place */
	_ltzmins = acblk.odval.ttm.dt_ictmz * 60;	/* signed TZ in min */
	_ltzsecs = _ltzmins * 60;			/* and secs */
	if (acblk.odval.ttm.dt_icads) {		/* Was DST applied? */
	    acblk.odval.ttm = chkdst;		/* Yes, check further... */
	    acblk.odval.ttm.dt_mon = 0;		/* Jan 4, 1976 had NO DST. */
	    jsys(IDCNV, acblk.ac);
	    acblk.ac[4] = acblk.ac[3];	/* Get stupid result in right place */
	    _ltdstt = acblk.odval.ttm.dt_icads	/* If DST was also applied, */
			? _DST_ON : _DST_USA;	/* then always on, else USA. */
	} else _ltdstt = _DST_OFF;		/* No DST, never apply it. */
#endif

	_ltzknown++;			/* Don't try again */
    }

    tp->tv_sec = tadl_to_utime(tadl_get());	/* Get current time */
    tp->tv_usec = 0;			/* Can't believe usec granularity */
    tzp->tz_minuteswest = _ltzmins;	/* Get signed TZ in min */
    tzp->tz_dsttime = _ltdstt;		/* and DST correction type */
    return 0;				/* Return success */
}

#if 0		/* Documentation on system-specific time representations */
/* UNIX 32-bit time word format:
**	<# of seconds since epoch>
**		where the epoch is 1-Jan-1970 00:00-GMT
**		This is good up until about 2037 when the sign bit gets hit,
**		and until 2105 if handled properly as unsigned value.
** Relevant system calls:
**	time()		V6 upwards
**	ftime()		V7 upwards
**	gettimeofday()	4.2BSD upwards
*/

/* TOPS-20 time word format:
**	<# days since epoch>,,<fraction of day>
**		where the epoch is 17-Nov-1858 00:00-GMT
**		This is good up until about 2216 when the sign bit gets hit.
**	RH is fraction of one day (1,,0 = 1 day), thus:
**		262144/86400 = 3 rem 2944 = 3.0340740 = val of 1 second
** Relevant monitor calls:
**	GTAD%
**	IDTIM%, IDTNC%, IDCNV%
**	ODTIM%, ODTNC%, ODCNV%
*/

/* TENEX time word format:
**	<# days since epoch>,,<# seconds in day>
**		where the epoch is 17-Nov-1858 00:00-GMT
**		This is good up until about 2216 when the sign bit gets hit.
** Relevant monitor calls are same as for TOPS-20.
*/

/* TOPS-10 and WAITS time word format:
**	These systems have no single-word value combining the date and time,
** so the following invented format is used:
**		<date in T10 fmt>,,<# secs in day>
**
** The date format for TOPS-10 and WAITS consists of the value:
**	< <year-1964>*12 + <month-1> >*31 + <day-1>
**		Note that only 15 bits are allocated in the system for this
**		value, limiting its range to about 88 years (until 2052).
** Relevant monitor calls:
**	DATE, MSTIME	(TOPS-10 and WAITS)
**	ACCTIM, DAYCNT	(WAITS only)
*/

/* ITS time word format:
** Mask    Field     Bits	Range	Var.    Variable range
** TM%SEC== 777776   2.9-1.2	0-131K	seconds	0-86399.
**		   ; 6 		0-63	secs 0-59
**		   ; 6		0-63	mins 0-59
**		   ; 5		0-31	hrs  0-23
** TM%DAY==   37,,0  3.5-3.1	0-31	days	1-31
** TM%MON==  740,,0  3.9-3.6	0-15	months	1-12
** TM%YR==177000,,0  4.7-4.1	0-127	years	0-127 rel to 1900 (1900-2027)
**		This is good up until 2156 with another bit added to year.
**
** Relevant system calls:
*/
	Documentation for ITS time stuff
----------------------------
.RYEAR ac,                              read year

        Returns in accumulator <ac> a word as follows:

        4.9     This year has 365. days, and it is after February 28.
        4.8     This year is a leap year (366. days).
        4.7     Daylight savings time is in effect.
        4.6     The time of year is known.
        4.2-3.9 If bit 4.6 set, the current day of the week.
                Sunday=0, Monday=1, etc.
        3.8-3.6 The day of the week of January 1 of this year.
        3.5-3.1 Zero.
        2.9-1.1 The year, not modulo 100., but as a full quantity,
                e.g. 1969. or 1975.

        If the system does not know the time, zero is returned.
----------------------
.RLPDTM ac,                             .PDTIME and .RYEAR

        This returns the time and date in ac and ac+1, in
        a binary form.  ac+1 contains the result of .RYEAR;
        see the description of that uuo for details.  ac
        contains the "localized" number of seconds since the
        beginning of the year.  If this is divided by the
        number of seconds in a day  (86400.), the remainder
        will be the number of seconds since midnight local
        time, and the quotient will be the number of days
        since the beginning of the year, with the (mis)feature
        that all years are considered to include February 29.
        This is done so that date-printing routines do not
        have to check for leap-year, but it does mean that
        during the last 10 months of non-leap years the day
        number is 1 greater than the correct Julian day.
        Bit 4.9 of ac+1 is set in this case.

        Note that .RLPDTM should be used instead of .PDTIME
        followed by .RYEAR, since it guarantees consistency
        of the two values returned.
----------------------
RQDATE: read disk format date

        val 1   Current date and time in disk format:
                4.7-4.1 Year (mod 100.).
                3.9-3.6 Month (January = 1).
                3.5-3.1 Day of month.
                2.9-1.1 Time in half-seconds after midnight.
                If date and time are unknown, -1 is returned.
        val 2   Date and time the system came up in disk format.
                If date and time are unknown, -1 is returned.
----------------------
#endif

#define DAYSECS (24*60*60)	/* # seconds in a day */

#if SYS_T20+SYS_10X
#define EPOCHDIFF (40587)	/* Days diff from UNIX epoch */
#endif

#if SYS_T10+SYS_WAITS
#include "muuo.h"
#define EPOCHDIFF (2192)	/* Days diff from UNIX epoch */
	/* There are (6 yrs * 365) + (2 leap days) = 2192. = 04220 days
	** separating this epoch from the UNIX one of 1-Jan-1970.
	*/
#endif

#if SYS_ITS
struct _its_tm {
	unsigned	 : 2;	/* 4.9-4.8 unused */
	unsigned dt_year : 7;	/* 4.7-4.1 Year (1900 based) */
	unsigned dt_mon  : 4;	/* 3.9-3.6 Month (1 = January) */
	unsigned dt_day  : 5;	/* 3.5-3.1 Day of month (1-31.) */
	unsigned dt_tim2 : 18;	/* 2.9-1.2 Time in half-secs after midnight */
};
#endif

/* Local-to-Unix and Unix-to-Local date/time val conversions.
*/

#if SYS_T10+SYS_WAITS+SYS_ITS
/* _TMONTB - Table of days thus far in year, assuming non-leap year.
** Indexed by month, with 0 = January.
*/
static int _tmontb[] = {
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};

/* Declare internal functions needed from CTIME.C */
extern struct tm *_lbrktime();	/* Internal version of localtime() */
extern int _tmisdst();		/* See if DST applies to a broken-down time */

#endif


/* TADL_GET - Get Local Time-And-Date value
*/
tadl_t
tadl_get()
{
#if SYS_T20+SYS_10X
    asm("\tSEARCH MONSYM\n");
    asm("\tGTAD\n");		/* GTAD% leaves result in AC 1 !! */

#elif SYS_WAITS
    asm("\CALLI 1,400101\n");	/* ACCTIM 1, */
				/*   Puts <date>,,<time> in return AC */
#elif SYS_T10
    asm("\tCALLI 2,23\n");	/* MSTIME 2, */
    asm("\tCALLI 3,14\n");	/* DATE   3, */
    asm("\tCALLI 1,23\n");	/* MSTIME 1,  to see if went past midnight */
    asm("\tCAMGE 1,2\n");	/* If so, ugh!  Hit timing glitch */
    asm("\t CALLI 3,14\n");	/*   and must do another DATE 3, */
    asm("\tIDIVI 1,1750\n");	/* Divide millisec by 1000 to get secs */
    asm("\tHRL 1,3\n");		/* Get <date>,,<time> in return AC */

#elif SYS_ITS			/* Gets disk format date in return AC */
    asm("\t.CALL [SETZ\n\t\tSIXBIT/RQDATE/\n\t\tSETZM 1]\n");
    asm("\t SETO 1,\n");

#else
#error tadl_get() not supported for this system.
#endif
}

/* TADL_FROM_UTIME - Convert UNIX Time-And-Date value to Local TAD value
*/
tadl_t
tadl_from_utime(utad)
time_t utad;
{
#if SYS_10X
	return (((utad/DAYSECS)+EPOCHDIFF) << 18) + (utad%DAYSECS);

#elif SYS_T20
	unsigned i,j;

	i = (utad/DAYSECS + EPOCHDIFF) << 18;	/* Get days */
	j = (utad%DAYSECS) << 18;
	if ((j % DAYSECS) >= (DAYSECS/2))	/* Round up day fraction? */
	    i++;				/* Yep, do it indirectly */
	return i + (j/DAYSECS);

#elif SYS_T10+SYS_WAITS+SYS_ITS
	struct tm u;

	_lbrktime(&u, utad);	/* Break up Unix time into a "tm" structure */

#if SYS_T10+SYS_WAITS
	return	(((((u.tm_year+(1970-1964))*12)
			+ u.tm_mon)*31 + (u.tm_mday-1)) << 18)
		+ ((u.tm_hour*60) + u.tm_min)*60
		+ u.tm_sec;
#endif
#if SYS_ITS
	    {
		union {
		    time_t xtad;
		    struct _its_tm itm;
		} i;
		i.xtad = 0;		/* Clear all fields */
		i.itm.dt_year = u.tm_year + 70;
		i.itm.dt_mon = u.tm_mon + 1;
		i.itm.dt_day = u.tm_mday;
		i.itm.dt_tim2 = (((u.tm_hour*60)+u.tm_min)*60+u.tm_sec) << 1;
		return i.xtad;
	    }
#endif

#else
#error tadl_from_utime() not supported for this system.
#endif
}

/* TADL_TO_UTIME - Convert Local Time-And-Date value to UNIX TAD value
**	Note this does not take DST into account, so UNIX TAD value may
** be wrong (except for T20 and T10X).
*/
time_t
tadl_to_utime(ltad)
tadl_t ltad;
{
#if SYS_10X
    return ((((ltad>>18) - EPOCHDIFF)*DAYSECS) + (ltad&RH));
#elif SYS_T20
	register i, k;

	i = ltad - (EPOCHDIFF << 18);	/* Convert to Unix epoch */
	k = (int)((unsigned)i >> 18) * DAYSECS;	/* Convert days to seconds */
	i = (i & RH) * DAYSECS;		/* Convert day-fraction to secs */
	if (i & 0400000)		/* Check for round up */
	    i += 0400000;		/* Yes, do roundup (carries into LH) */
	return k + ((unsigned)i >> 18);	/* Now return sum of seconds! */

#elif SYS_WAITS+SYS_T10+SYS_ITS
	register int tim, days;
	struct tm dtm;			/* So we can use _tmisdst */
#if SYS_ITS
	union {
		tadl_t xtad;
		struct _its_tm itm;
	} utad;
#endif

	/* Ensure that local variables are set up for timezone/DST values */
	if (!_ltzknown) {		/* Ensure local static vars are set.
	    struct timeb tmb;
	    ftime(&tmb);
	}
	/* Now extract various fields from local-format TAD */
#if SYS_WAITS+SYS_T10
	tim = (unsigned)ltad >> 18;	/* Isolate date (re-using "tim") */
	dtm.tm_mday =  (tim % 31) + 1;		/* Fix 0-origin day of month */
	dtm.tm_mon  =  (tim / 31) % 12;		/* Get 0-origin month */
	dtm.tm_year = ((tim / 31) / 12) - 64;	/* Fix 1964-origin year */
	tim = ltad & RH;			/* Get time of day in secs */
#endif
#if SYS_ITS
	utad.xtad = ltad;		/* Make fields accessible via union */
	dtm.tm_year = utad.itm.dt_year;	/* Get 1900-origin year */
	dtm.tm_mon = utad.itm.dt_mon-1;	/* Fix 1-origin month */
	dtm.tm_mday = utad.itm.dt_day;	/* Get 1-origin day of month */
	tim = utad.itm.dt_tim2 >> 1;	/* Fix 0.5-sec time of day */
#endif
	/* All variables have been extracted, now compute the Unix time.
	** This is very similar to the mktime() routine in CTIME.C but
	** must be independent to avoid loops (and overhead!)
	*/
	dtm.tm_yday = _tmontb[mon]	/* Find # days this yr (non-leap yr) */
		+ dtm.tm_mday - 1;
	if ((dtm.tm_year&03)==0		/* But if this is leap year, */
	  && dtm.tm_mon > 1)		/* and after February, */
	    dtm.tm_yday++;		/* add a leap day. */

	/* Find total days since 1/1/70 */
	days = dtm.tm_yday		/* # days this year */
	    + (dtm.tm_year-70)*365	/* plus # days in years so far */
	    + (dtm.tm_year-70+1)>>2;	/* plus # of leap days since 1970 */

	if ((tim += _ltzsecs) < 0) {	/* Make timezone adjustment */
	    --days;			/* Oops, back up 1 day */
	    tim += 24*60*60;		/* so time always positive */
	}
	if (_ltdstt != _DST_OFF) {	/* If we have to check for DST, */
	    dtm.tm_wday = (days+4) % 7;	/* set up day-of-week (4 = Thurs) */
	    dtm.tm_hour = tim / 3600;	/* and hour (can ignore min/sec) */
	    if (_tmisdst(&dtm, _ltdstt))	/* Does DST apply? */
		tim += 60*60;			/* Yes, bump up 1 hour */
	}

	return (days * DAYSECS) + tim;	/* Return total # of secs */

#else
#error tadl_to_utime() not supported for this system.
#endif
}
