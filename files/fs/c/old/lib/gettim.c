/*
** gettimeofday(2) - get/set date and time
** Bill Palmer / Stanford University / 28 June 1985
*/

entry gettim;

#include "c-env.h"
#include <sys/time.h>

#define EPOCH 0117213252525		/* Jan 1, 1970 0:00 in T20 TAD form */
#define SECPERDAY 24*60*60		/* this many seconds in a day */
#define GTAD 0227
#define ODCNV 0222
#define IC_TMZ 0000077000000		/* time zone used (ODCNV) */
#define IC_ADS 0200000000000		/* daylight savings time used */
#define RH 0000000777777
#define LH 0777777000000

gettimeofday(tp,tzp)
struct timeval *tp;
struct timezone *tzp;
{
#if SYS_T20 
    unsigned acs[5];
    unsigned i, j;

    if (!jsys(GTAD,acs)) return -1;	/* jsys error */
    if (acs[1] == -1) return -1;	/* or time not set */
    i = ((acs[1] - EPOCH) >> 18);	/* get number of days into i */
    j = ((acs[1] - EPOCH) & RH);	/* fractions of a second */
    j *= SECPERDAY;
    j /= RH;
    i *= SECPERDAY;
    tp->tv_sec = (long)(i + j);		/* seconds since EPOCH */
    tp->tv_usec = 0L;			/* can't be believed */

    acs[2] = acs[1];			/* current TAD to ac1 */
    if (!jsys(ODCNV,acs)) return -1;	/* jsys error */
        
    tzp->tz_dsttime = (int)(acs[4] & IC_ADS);
    tzp->tz_minuteswest = (int)((acs[4] & IC_TMZ ) >> 18) * 60;
    return 0;				/* success return */
#else
    return -1;				/* other systems always fail */
#endif
}

