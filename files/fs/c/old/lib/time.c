/*
** time_t time((time_t *) 0)
** time_t time(tloc)
** time_t tloc;
**
** Greg Satz / Stanford University / 12-Sep-84
** Bill Palmer / Stanford University / 28-Feb-85
** TENEX additions by Ken Harrenstien, SRI  1-Jun-85
*/

#include "c-env.h"

entry time, ctime, localtime, gmtime, asctime;

#if SYS_T20+SYS_10X		/* Moby conditional */
entry _t2utim, _u2ttim;

#define	GTAD	0227		/* get time and date */

#include <sys/types.h>

#define	LH	0777777000000
#define	RH	0777777
#define DAYSECS (24*60*60)	/* # seconds in a day */
#define EPOCHDIFF (40587)	/* Days diff between UNIX and 10X/20X day 0 */

time_t time(tloc)
time_t *tloc;
{
	unsigned i, ablock[5];

	jsys(GTAD, ablock);
	i = _t2utim(ablock[1]);		/* convert to unix time */
	if (tloc)			/* fill in non-null arg. */
		*tloc = i;
	return i;			/* and return value */
}

/*
** char *ctime(clock)
** long *clock
*/

#define	ODTIM	0220
#define	OT_DAY	0200000000000
#define	OT_4YR	0010000000000
#define	OT_DAM	0004000000000
#define	OT_SPA	0002000000000

static char cbuf[26];

char *ctime(t)
time_t *t;
{
	int ablock[5];
	char temp[100];

	ablock[1] = (int) temp;
	ablock[2] = _u2ttim(*t);
	ablock[3] = OT_DAY|OT_4YR|OT_DAM|OT_SPA;
	if (jsys(ODTIM, ablock) == 0)
		return (char *) -1;
	strncpy(cbuf, temp+1, 10);	/* copy day, month, day */
	strncpy(cbuf+10, temp+16+1, 9);	/* copy time */
	strncpy(cbuf+19, temp+11+1, 5);	/* copy year */
	strncpy(cbuf+24, "\n", 2);
	return cbuf;
}
	
unsigned _t2utim(tad)
unsigned tad;
{
#if SYS_10X
	return((((tad>>18) - EPOCHDIFF)*DAYSECS) + (tad&RH));
#endif
#if SYS_T20
	unsigned i, j, k;

	i = tad - (EPOCHDIFF << 18);	/* # days between t20 and unix day 0 */
	k = ((i & LH) >> 18) & RH;	/* day count */
	k *= DAYSECS;			/* multiply by seconds in a day */
	i &= RH;			/* isolate fraction of a day */
	i *= DAYSECS;			/* make seconds */
	j = ((i & LH) >> 18) & RH;	/* get full seconds */
	if (i & 0400000)		/* check for round up */
		j++;			/* yes */
	k += j;				/* get total */
	return k;			/* pass back number */
#endif /*SYS_T20*/
}

unsigned _u2ttim(sec)
long sec;
{
#if SYS_10X
	return((((sec/DAYSECS)-EPOCHDIFF)<<18) + (sec%DAYSECS));
#endif
#if SYS_T20
	unsigned i,j;

	i = sec/DAYSECS;
	i += EPOCHDIFF;		/* days between t20 and unix epoch */
	j = (sec%DAYSECS) << 18;
	j /= DAYSECS;
	return ((i << 18)&LH) | (j&RH);
#endif /*SYS_T20*/
}


#include <sys/time.h>

#define ODCNV 0222			/* odcnv% jsys */
#define ICADS 0200000000000		/* ic%ads bit (DST used) */
#define ICJUD 0040000000000		/* ic%jud bit (use julian format) */

static struct tm ctmbuf;

struct tm *localtime(clock)
long *clock;
{
    unsigned i,j, acs[5];
    struct tm *t;
    t = &ctmbuf;
    acs[1] = acs[3] = acs[4] = 0;
    acs[2] = i = _u2ttim(*clock);	/* internal TAD to ac2 */
    if (!jsys(ODCNV,acs)) return(0);	/* something went wrong */
    t->tm_year = ((acs[2] & LH) >> 18) - 1900; /* set up year */
    t->tm_mon = (acs[2] & RH);		/* set up month */
    t->tm_mday = ((acs[3] & LH) >> 18) + 1; /* set up month day */
    t->tm_wday = ((acs[3] & RH) + 1) % 7; /* set up day of week */
    j = (acs[4] & RH);
    t->tm_hour = j / 3600;		/* hours */
    t->tm_min = (j - (t->tm_hour * 3600)) / 60;	/* minutes */
    t->tm_sec = (j - ((t->tm_hour * 60) + t->tm_min) * 60); /* seconds */
    t->tm_isdst = acs[4] & ICADS;	/* daylight-savings-p */
    acs[1] = acs[3] = 0;	
    acs[2] = i;				/* get time back */
    acs[4] = ICJUD;
    if (!jsys(ODCNV,acs)) return(0);	/* something went wrong */
    t->tm_yday = (acs[2] & RH) - 1;	/* day of year */
    return t;
}

#include <stdio.h>

struct tm *gmtime(clock)
long *clock;
{
    fprintf(stderr,"gmtime not yet implemented\n");
    return(0);
}

char *asctime(tm)
struct tm *tm;
{
    fprintf(stderr,"asctime not yet implemented\n");
    return(0);
}

#endif /* T20+10X */
