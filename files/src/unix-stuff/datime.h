/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* datime.h
**
** Structures and symbols for manipulating times and timezones
** using the datime utilities.
**/

#ifdef MSDOS
/* Fill in local timezone offset and code (if known) here */
#define LCL_TZCODE TZ_UEST	/* USA Eastern time */
#define LCL_TZOFF (5*60)	/* 5 hrs west of Greenwich */
#endif

/*
** DATIME structures hold date and time information.  00:00:00 is considered
** the beginning of the day, not the end.
**/

typedef struct DATIME {
	short	_dtmon;		/* 0 = January, 11 = December */
	short	_dtday;		/* 0 = first day of month */
	int	_dtyr;
	short	_dtdow;		/* day of week: 0 = Sunday, 6 = Saturday */
	short	_dthr;		/* 0 to 23 */
	short	_dtmin;
	short	_dtsec;
	int	_dttz;		/* time zone, # of mins west of Greenwich */
				/*  (this is the number of minutes to add */
				/*  in order to get GMT) */
	int	_dttzc;		/* time zone code, one of TZ_XXX codes */
				/*  defined below, or -1 */
	int	_dtdst;		/* additional minutes to shift due to */
				/*  daylight savings time (generally -60 */
				/*  when dst is in effect and 0 otherwise) */
} datime;

/* Flags to control the time/date parse */

#define DTP_NTI 0x0001		/* do not parse time */
#define DTP_N24 0x0002		/* no 24-hour time format allowed */
#define DTP_NTM 0x0004		/* no AM/PM specifier allowed */
#define DTP_NIS 0x0008		/* seconds may not be specified */
#define DTP_AIS 0x0010		/* seconds must be specified after colon */
#define DTP_NAC 0x0020		/* colon cannot appear between hrs and mins */
#define DTP_AAC 0x0040		/* colon must appear between hrs and mins */
#define DTP_AHM 0x0080		/* first # in time cannot be hrs & mins */
#define DTP_AMS 0x0100		/* first # in time is both hrs and mins */
				/*  unless too any colons for this */
#define DTP_NTZ 0x0200		/* no time zone allowed in input */
#define DTP_NDA 0x0400		/* do not parse date */
#define DTP_NNM 0x0800		/* month may not be numeric */
#define DTP_SNM 0x1000		/* second of two date numbers is month */
#define DTP_ERR 0x2000		/* do not swap month/day numbers if TAD_SNM */
				/*  setting implies range error */
#define DTP_NDW 0x4000		/* do not allow day-of-week in input */
#define DTP_NSP 0x8000		/* do not allow time to split date before */
				/*  year, as BSD's ctime call does (e.g. */
				/*  Saturday, 9 March 9:52am 1986 */


/* DSTRULE
**
** Describes rules for determining daylight savings times.  Each rule
** structure describes a time change that takes place at a given time
** on a given day of the year.  In most cases, the rules will be given
** in pairs, one turning on dst, and another turning it off.  When DST
** must be determined for a given date/time, the last change preceding
** that moment determines the dst adjustment that applies.
**
** Each change rule can span several years indicating that the given
** change occurs in each of the spanned years.  The time at which
** a change occurs is specified in terms of the wall clock time (ie,
** including the preceding adjustment, if any).  The date of the change
** is given in terms of a day of the week and a julian date, meaning
** the last date on the given day-of-week that does NOT follow the
** given julian date (for example, last Sunday in April is the last
** Sunday not following April 30).  If no change occurs
** before a given date/time, a zero adjustment is to be used.
**/

typedef struct DSTRULE {
    	int _dsyr1; 	    	    	/* first year in which change occurs */
	int _dsyrn;			/* last year in which change occurs */
	int _dsdow;			/* day of week of change (0-7 for */
					/*  Sun-Sat, -1 for date in _dsday) */
	int _dsday;			/* julian day (assuming leap year) */
					/*  less than one week after change */
	    	    	    	    	/*  Use 0 for Jan 1.  If >= 59 on a */
					/*  non-leap year, it is decremented */
					/*  by one before applying the rule */
	int _dstim;			/* time of change (mins since 12M) */
} dstrule;

typedef struct TZINF {			/* timezone information */
	int _tzcode;			/* one of the TZ_XXX values */
					/*  defined below */
	char *_tznam,*_tzdnm;		/* name, and name with dst */
	int _tzoff;			/* minutes west of greenwich */
	dstrule *_tzdst;		/* array of dst rules, terminated */
					/* by one for year zero */
	int _tzdadj;			/* minutes to subtract during dst */
	int _tzsth;			/* TRUE for southern hemisphere, */
					/* so the dst rules are reversed */
} tzinf;


/* Time zones that we know about */

#define	TZ_GMT	0			/* Greenwich Mean Time */
#define	TZ_UAST	1			/* USA Atlantic */
#define TZ_UEST	2			/* USA Eastern */
#define TZ_UCST	3			/* USA Central */
#define	TZ_UMST	4			/* USA Mountain */
#define	TZ_UPST	5			/* USA PAcific */
#define	TZ_UYST	6			/* USA Yukon */
#define TZ_UHST	7			/* USA Alaska/Hawaii */
#define	TZ_UBST	8			/* USA Bering */
#define	TZ_EWET	9			/* EUROPE Western */
#define	TZ_EMET	10			/* EUROPE Middle */
#define	TZ_EEET	11			/* EUROPE Eastern */
#define	TZ_AEST	12			/* AUSTRALIA Eastern */
#define	TZ_ACST	13			/* AUSTRALIA Central */
#define TZ_AWST	14			/* AUSTRALIA Western */

/* Flags to be added to timezone ID's to indicate daylight savings time
** handling
**/

#define TZ_DST	0x4000			/* dst was indicated */
#define	TZ_STD	0x2000			/* standard time was indicated */


/* Include the following flag in a timezone code passed to dttzinf to
** distinguish the code from a positive Greenwich offset.  (The flag is
** not examined if the passed value is negative, which can only be
** an offset */

#define TZ_CODE	0x1000

/* Routines that do not return ints */

tzinf *dttzinf();

/* Miscellaneous definitions */

#ifndef TRUE
#define TRUE -1
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif
