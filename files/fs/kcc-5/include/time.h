/* ANSI <time.h> - Date and Time definitions and declarations
**
*/
#ifndef _TIME_INCLUDED		/* Include only once */
#define _TIME_INCLUDED

#define CLK_TCK		1000	/* Processor time is in msec */
typedef long clock_t;		/* A processor time value */

#ifndef _TIME_T			/* Avoid clash with BSD <sys/types.h> */
#define _TIME_T
typedef long time_t;		/* A calendar date/time value */
#endif

/* TM broken-down calendar time structure */
struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
	int tm_dummy[3];	/* Spares for extra features or expansion */
};

/* Function Declarations */

extern clock_t clock();		/* Get processor runtime */
extern time_t time();		/* Get calendar date/time */
extern char *asctime(),		/* Make static d/t string given struct tm */
	*ctime();		/* Make static d/t string given time_t */
extern struct tm *gmtime(),	/* Break down time_t into static struct tm */
	*localtime();		/* Same but local time instead of GMT */
extern double difftime();	/* Get difference of two time_t vals */
#endif
