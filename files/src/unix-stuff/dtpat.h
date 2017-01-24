/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* dtpat.h
**
** Time and date patterns.  Each pattern consists of a pattern string
** and an array of argument pointers.  The pattern is sort of
** a printf-style string, with percent-signs marking special pattern
** elements.  Spaces in the pattern are ignored, unless preceded by
** a percent sign.  The argument pointers point to objects
** referenced by the special pattern elements, or to locations where
** values returned by the pattern elements are to be placed.  Normal
** characters in the pattern must be matched exactly, except that case
** of letters does not matter.  Special pattern elements are as follows:
**
**   %= - Stores the last value returned by a pattern element
**     into the variable pointed to by the next argument pointer.
**   %% - Matches a percent sign.  Zero is always returned.
**   %  - (percent space) Matches a space.  Zero is always returned.
**     (cf. %w below).
**   %c - Matches the next pattern character, exempting it from
**     any special processing.  Returns 0.
**   %k - Matches a keyword from the table pointed to by the next
**     argument pointer.  The table is a NULL-terminated array
**     of string pointers.  A keyword may contain a vertical bar,
**     which marks the minimum match allowed for the keyword.  For
**     example, "JAN|UARY" will match "JAN" but not "JA".  If no bar
**     is present, nothing less than the complete keyword will match.
**     No check is made for an ambiguous match -- the bars should be
**     placed so as to take care of this.  Case of letters is ignored.
**     Returns the index of the matching keyword.
**   %#m - Matches a month number (range 1-12).  Returns the number
**     minus 1.
**   %#d - Matches a day number (range 1-31).  Returns the number
**     minus 1.  No check is made as to month-specific value constraints.
**   %#y - Matches a two-digit year number.  Returns the number plus 1900.
**   %#Y - Matches a four-digit year number.
**   %#h - Matches an hour number in the range 1-12.  Returns the number.
**   %#H - Matches an hour number in the range 0-24.  Returns the number.
**   %#M - Matches a four-digit hour-minute number (hhmm).  Returns two
**     numbers - the first %= gets the hour part (number / 100), the second
**     gets the minutes part (number % 100).  For normal, 2-digit minute
**     use %#s.
**   %#s - Matches a second number, in the range 0-59.  Returns the number
**   %p - Matches a punctuation character (not letter or digit).  Returns
**     the ASCII code of the character, but does not consume it in the
**     parse.
**   %w - Matches a run of whitespace.  Returns 0.
**   %d - Matches a delimiter character - next argument pointer gives a
**     string containing the allowed delimiters.  Returns the index of
**     the matching delimiter in the given string.
**   %r - Recursively attempts to match against an array of
**     patterns pointed to by the next argument pointer.  Returns the index
**     in the pattern array of the pattern that succeeded.
**   %f - Function pointed to by next argument pointer is invoked with
**     a single argument - an int pointer which is used to return a value
**     for the pattern element.  The function returns TRUE or FALSE
**     according to whether or not it succeeded.
**   %z - Assigns 0 to the next argument (like a %= after an element
**     that returned 0).
**   %n - Assigns -1 to the next argumnet (like a %= after an element
**     that returned -1).
**   %+ - Skips the next arg without other side-effects.
**
** Any pattern element can be specified with a question mark after the
** percent sign, to indicate that the element is optional.  An optional
** pattern element that fails always returns -1 but does not cause overall
** failure of the pattern.
**
** Along with the pattern string and argument pointers, each pattern is
** declared with a base rank, a qualifying flag word, and a list of
** rank adjusters.  The base rank is the rank normally assigned to the
** pattern, and can be used to arbitrate among several patterns that
** might match a given input.  The qualifying flag word contains parse
** flags whose presence in the parse cause the pattern to be disallowed.
** So, for example, any time pattern that includes a seconds field is
** disallowed by the DTP_NIS flag.  Each item in a rank adjustment list
** contains a flag word and an adjustment value.  If any of the given
** parse flags are present in the parse, the adjustment is added to the
** pattern's rank for the duration of the parse.  The adjustment can
** be negative, but if a pattern rank becomes negative as a result of
** adjustments, the pattern is disqualified.
**/



/* Here are the structure types to be used to hold pattern information */

typedef struct RANKADJ {		/* rank adjustments */
	int _raflg;			/* flags triggering this adjustment */
					/*  (-1 signals end of adjustments) */
	int _raadj;			/* amount of adjustment */
} rankadj;


typedef struct DTPAT {			/* time/date parsing pattern */
	char *_dppat;			/* pattern string */
					/*  (NULL for end of pattern array) */
	int **_dparg;			/* arg pointer list */
	int  _dpqal;			/* (dis)qualifier flag word */
	int  _dprnk;			/* base rank for this pattern */
	rankadj *_dpraj;		/* array of rank adjustments */
} dtpat;



/* Keyword tables for use in parsing patterns */

/* Timezone keywords array, must be kept parallel with translation
** array matching keys with TZ_xxx values (possibly with TZ_DST flag
** or TZ_STD included).  EST and CST are taken to mean their USA versions
** in this table.  AEST and ACST are provided for australian versions.
** AWST is also allowed for australian WST.
**/

static char *(tznames[]) = {
  "gmt|", "green|wich",
  "ast|", "adt|", "atl|antic",
  "est|", "edt|", "east|ern",
  "cst|", "cdt|", "cen|tral",
  "mst|", "mdt|", "moun|tain",
  "pst|", "pdt|", "pac|ific",
  "yst|", "ydt|", "yuk|on",
  "hst|", "hdt|", "ala|ska", "haw|aii",
  "bst|", "bdt|", "ber|ing",
  "wet|", 
  "met|", 
  "eet|", 
  "aest|", 
  "acst|", 
  "wst|", "awst|",
  NULL
};

/* timezone name translation table - Must be kept synchronized with name
** array declared above.
**/

static int tztrans[(sizeof(tznames)/sizeof(char*))-1] = {
  TZ_GMT,  TZ_GMT,
  TZ_STD+TZ_UAST, TZ_DST+TZ_UAST, TZ_UAST,
  TZ_STD+TZ_UEST, TZ_DST+TZ_UEST, TZ_UEST,
  TZ_STD+TZ_UCST, TZ_DST+TZ_UCST, TZ_UCST,
  TZ_STD+TZ_UMST, TZ_DST+TZ_UMST, TZ_UMST,
  TZ_STD+TZ_UPST, TZ_DST+TZ_UPST, TZ_UPST,
  TZ_STD+TZ_UYST, TZ_DST+TZ_UYST, TZ_UYST,
  TZ_STD+TZ_UHST, TZ_DST+TZ_UHST, TZ_UHST, TZ_UHST,
  TZ_STD+TZ_UBST, TZ_DST+TZ_UBST, TZ_UBST,
  TZ_EWET, 
  TZ_EMET,
  TZ_EEET,
  TZ_AEST,
  TZ_ACST,
  TZ_AWST, TZ_AWST
};

/* Keyword table for daylight-savings-time specifications */

static char *(dstnames[]) = {
  "day|light", "dst|",
  "stan|dard", "std|",
  NULL
};

/* Keyword table for AM/PM specifications */

static char *(ampmnames[]) = {
  "m|idnight", "a|m", "n|oon", "p|m",
  NULL
};

/* Keyword table for days of the week */

static char *(downames[]) = {
  "sun|day", "mon|day", "tue|sday", "wed|nesday", "thu|rsday", 
  "fri|day", "sat|urday",
  NULL
};

/* Keyword table for months */

static char *(monnames[]) = {
  "jan|uary", "feb|ruary", "mar|ch", "apr|il", "may|", "jun|e",
  "jul|y", "aug|ust", "sep|tember", "oct|ober", "nov|ember", "dec|ember",
  NULL
};




/* Global variables referenced by the patterns */

/* Make sure this global var does not get misinterpreted */
#ifdef min
#undef min
#endif

static int mon,day,yr,dow;		/* date and day-of-week */
static int dowspec;			/* -1 iff dow not specified */
static int hr,min,sec;			/* time fields */
static int ampm;			/* -1 if not specified, else */
					/*  0-3 for midnight, am, noon, pm */
static int tz,tzc;			/* specified tz - offset & code */
static int tzspec;			/* -1 iff timezone not given */
static int dst;				/* daylight savings time spec */
static int dstspec;			/* -1 iff dst not specified */
static int tzsign,tzhr,tzmin;		/* pieces of GMT+hh:mm style tz */



/* Patterns for matching a timezone specification */

/* aux functions invoked by timezone patterns */
int tzlkup();				/* translate keyword index to */
					/*  TZ_xxx value, and interpret */
					/*  dst specification */
int tzcalc();				/* combine hrs and mins in offset */
					/*  from GMT, and flag that no dst */
					/*  indication was given */

/* arg lists for timezone patterns */
static int *(tz1arg[]) = {		/* patterns with named timezones */
	(int *) tznames, &tz,		/* keyword lookup of timezone name */
	(int *) dstnames, &dst,		/* optional dst specification */
	(int *) tzlkup			/* fn to convert key idx to TZ_xxx */
};
static int *(tz2arg[]) = { 		/* Patterns with offset from GMT */
	(int *) "+-", &tzsign,		/* direction of offset (+ for east) */
	&tzhr, &tzmin,			/* hours and minutes in offset */
	(int *) dstnames, &dst,		/* optional dst specifier */	
	(int *) tzcalc			/* fn to combine the pieces */
};

/* the timezone patterns */
static dtpat tzpats[] = {		/* timezone patterns */
  { "%?w %?c- %?w %k%= %w %k%= %p %f",	/* name with dst indicator, like */
    tz1arg, DTP_NTZ, 1, NULL },		/*  "-EASTERN DAYLIGHT" */
  { "%?w %?c- %?w %k%= %p %+%n %f",	/* named zone, like "-EST" */
    tz1arg, DTP_NTZ, 0, NULL },
  { "%?w %?c- %?w GMT %?w %d%= %?w %#h%= : %#s%= %?w %?k%= %p %f",
    tz2arg, DTP_NTZ+DTP_NAC, 4, NULL },	/* offset "-GMT+4:30" */
  { "%?w %?c- %?w GMT %?w %d%= %?w %#h%= %z %?w %?k%= %p %f",
    tz2arg, DTP_NTZ+DTP_AMS, 3, NULL },	/* offset like "-GMT+4" */
  { "%?w %?c- %?w GMT %?w %d%= %?w %#M%=%= %?w %?k%= %p %f",
    tz2arg, DTP_NTZ+DTP_AAC+DTP_AHM , 2, NULL }, /* offset like "-GMT+430" */
  NULL					/* end of pattern array */
};




/* Patterns for matching a time specification */

/* aux functions invoked by time patterns */
int fixampm();				/* modify time according to am/pm */
					/*  specification to yield 24-hour */
					/*  time */

/* arg list for time specifications */
static int *(timarg[]) = {		/* one list suffices for all */
	&hr, &min, &sec,		/* individual time fields */
	(int *) ampmnames, &ampm,	/* optional am/pm keyword */
	(int *) fixampm,		/* adjust time according to am/pm */
	(int *) tzpats, &tzspec		/* optional call to timezone pats */
};


/* the time patterns - each finishes with a recursive pattern call to
** the timezone patterns
**/
static dtpat timpats[] = {
  { "%#H%= : %#s%= : %#s%= %+%n %f %?r%= %p",	/* hh:mm:ss, no am/pm */
    timarg, DTP_N24+DTP_NIS+DTP_NAC, 4, NULL },
  { "%#H%= : %#s%= %z %+%n %f %?r%= %p",	/* hh:mm, no am/pm */
    timarg, DTP_N24+DTP_AIS+DTP_NAC+DTP_AMS, 3, NULL },
  { "%#M%=%= : %#s%= %+%n %f %?r%= %p",		/* hhmm:ss, no am/pm */
    timarg, DTP_N24+DTP_NIS+DTP_AAC+DTP_AHM, 2, NULL },
  { "%#H%= %z %z %+%n %f %?r%= %p",		/* hh, no am/pm */
    timarg, DTP_N24+DTP_AIS+DTP_AMS, 1, NULL },
  { "%#M%=%= %z %+%n %f %?r%= %p",	  	/* hhmm, no am/pm */
    timarg, DTP_N24+DTP_AIS+DTP_AAC+DTP_AHM, 0, NULL },
  { "%#h%= : %#s%= : %#s%= %?w %k%= %f %?r%= %p", /* hh:mm:ss, with am/pm */
    timarg, DTP_NTM+DTP_NIS+DTP_NAC, 9, NULL },
  { "%#h%= : %#s%= %z %?w %k%= %f %?r%= %p",	/* hh:mm, with am/pm */
    timarg, DTP_NTM+DTP_AIS+DTP_NAC+DTP_AMS, 8, NULL },
  { "%#M%=%= : %#s%= %?w %k%= %f %?r%= %p",	/* hhmm:ss, with am/pm */
    timarg, DTP_NTM+DTP_NIS+DTP_AAC+DTP_AHM, 7, NULL },
  { "%#h%= %z %z %?w %k%= %f %?r%= %p",		/* hh, with am/pm */
    timarg, DTP_NTM+DTP_AIS+DTP_AMS, 6, NULL },
  { "%#M%=%= %z %?w %k%= %f %?r%= %p",		/* hhmm, with am/pm */
    timarg, DTP_NTM+DTP_AIS+DTP_AAC+DTP_AHM, 5, NULL },
  NULL						/* end of pattern list */
};



/* Date patterns are split into a month-day section and a year section,
** to allow BSD ctime-style time strings to be parsed, with the time
** portion inserted into the date in front of the year.  This style
** of specification is disallowed by the DTP_NSP (No SPlit) flag.
**
** In addition, day-of-week is split off to simplify the month-day 
** patterns.
**/

/* Year patterns */

/* Aux function invoked by year patterns */
int fixyr();				/* Check for two-digit years, and */
					/* adjust them to the current */
					/* century */

/* arg list for year patterns */
static int *(yrarg[]) = {
	(int *) "'",			/* optional apostrophe for 2 digits */
	&yr,				/* year number */
	(int *) fixyr			/* fn to adjust 2-digit years */
};

/* the year patterns */
static dtpat yrpats[] = {
  { "%?d %#y%= %p %f",			/* two digits w/optional apostrophe */
    yrarg, 0, 1, NULL },
  { "%+ %#Y%= %p %+",			/* four digits, no apostrophe */
    yrarg, 0, 0, NULL },
  NULL					/* end of pattern list */
};



/* Day of week patterns */

/* arg list for day-of-week patterns */
static int *(dowarg[]) = {
	(int *) downames,&dow		/* day-of-week via keyword table */
};

/* the day-of-week patterns */
static dtpat dowpats[] = {
  { "%k%= %?w , %?w",			/* dow w/comma, opt whitespace */
    dowarg, DTP_NDW, 1, NULL },
  { "%k%= %w",				/* dow w/out comma, wspace required */
    dowarg, DTP_NDW, 0, NULL },
  NULL					/* end of pattern list */
};




/* Month-day patterns */

/* arg lists for month-day patterns */
static int *(md1arg[]) = { 		/* month name, then day number */
	(int *) monnames, &mon,		/* month by name */
	&day				/* day number */
};
static int *(md2arg[]) = {		/* day number then month name */
	&day, (int *) "-/.",		/* day number, then delimiter */
	(int *) monnames, &mon		/* then month by name */
};
static int *(md3arg[]) = {		/* two numbers, month first */
	&mon, (int *) "-/.",		/* month number, then delimiter */
	&day				/* then day number */
};
static int *(md4arg[]) = {		/* two numbers, day first */
	&day, (int *) "-/.",		/* day number, then delimiter */
	&mon				/* then month number */
};

/* rank adjustments for the two-number patterns.  Normally, month-first
** patterns will be ranked one and day-first patterns zero.  If DTP_SNM
** is on, the situation is reversed by adding one to dd-mm patterns, and
** subtracting one from mm-dd patterns.  If DTP_ERR is on, both types
** of pattern are dropped by 1, putting the lower ranked type into the
** red and thereby disqualifying it.
**/
static rankadj md1adj[] = { 		/* adjustments for mm-dd patterns */
  { DTP_SNM, -1 },			/* drop rank if dd-mm prefered */
  { DTP_ERR, -1 },			/* and to -1 if no swap allowed */
  -1
};
static rankadj md2adj[] = {		/* adjustments for dd-mm patterns */
  { DTP_SNM, 1 },			/* bump rank if dd-mm preferred */
  { DTP_ERR, -1 },			/* but drop if no swap allowed */
  -1
};

static dtpat mdpats[] = {		/* examples illustrate formats */
  { "%k%= %w %#d%= %?w ,",		/* January 10, */
    md1arg, 0, 4, NULL },
  { "%k%= %w %#d%=",			/* January 10 (no comma) */
    md1arg, 0, 3, NULL },
  { "%#d%= %?w %?d %?w %k%=",		/* 10-Jan */
    md2arg, 0, 2, NULL },
  { "%#m%= %?w %?d %?w %#d%=",		/* mm-dd */
    md3arg, DTP_NNM, 1, md1adj },
  { "%#d%= %?w %?d %?w %#m%=",		/* dd-mm */
    md4arg, DTP_NNM, 0, md2adj },
  NULL
};



/* Date patterns - optional dow pattern, then a month-day pattern
** followed by a year pattern, followed by a call to the date
** checking routine 
**/


/* aux function invoked by date patterns */
int datchk();				/* make sure the date makes sense */

/* arg list for date parses */
static int *(datarg[]) = {
	(int *) dowpats, &dowspec,	/* optional day of week */
	(int *) mdpats,			/* month and day first */	
	(int *) "-/.", 			/* then delimiter */
	(int *) yrpats,			/* then a year */
	(int *) datchk 			/* and a validity check */
};	
		   
static dtpat datpats[] = {
  { "%?r%= %r %?w %?d %?w %r %f",	/* dow, mm-dd, delimiter, year */
   datarg, 0, 0, NULL },
  NULL
};




/* Date/time patterns - Either a date followed by a time, or a split date
** with the time in front of the year.
**/


/* arg lists for date/time patterns */
static int *(dt1arg[]) = {		/* normal date/time */
	(int *) datpats,		/* first the date */
	(int *) timpats			/* then the time */
};
static int *(dt2arg[]) = {		/* split date format */
	(int *) dowpats, &dowspec,	/* optional day of week */
	(int *) mdpats,			/* month/date first */
	(int *) timpats, 		/* then a complete time spec */
	(int *) yrpats,			/* then a year */
	(int *) datchk			/* and finally check the date */
};

/* the date/time patterns */
static dtpat dtpats[] = {
  { "%r %w %r",			/* date, time , separated by whitespace */
    dt1arg, 0, 2, NULL },
  { "%?r%= %r %w %r %w %r %f",	/* dow, month/day, time, year */
    dt2arg, 0, 1, NULL },
  { "%?r%= %r %w %r %?w , %?w %r %f", /* dow, month/day, time, comma, year */
    dt2arg, 0, 0, NULL },
  NULL
};



/* Auxiliary functions invoked by the patterns.  All share common calling
** conventions:
**
** Input arguments: None.
** Output arguments
**   val - Pointer to integer where pattern element return value is to
**     be placed by a successful invocation.
** Returns: TRUE on success, FALSE otherwise.
**/

/* tzlkup
**
** Purpose:
**   Global variable tz has the index in the tznames array of the timezone
**   keyword typed by the user.  Global variable dst has the index in
**   array dstnames of the daylight-savings-time keyword typed by the user,
**   or -1 if none was given.  This function uses the tztrans translation
**   table to get one of the TZ_xxx timezone codes, possibly with TZ_DST
**   or TZ_STD set as well.  The timezone code is translated into a number
**   of minutes west of Greenwich, which is then stored into tz.  The
**   dst value is translated into a boolean flag indicating whether or not
**   daylight savings time was specified, and a check is made to ensure that
**   any such specification does not clash with TZ_DST or TZ_STD flags from
**   the timezone table.  Variable dst is left with a value TRUE or FALSE,
**   depending on whether or not daylight savings time is meant to be on.
**   If dst is unspecified both by keyword and by flags from the table
**   lookup, variable dstspec is set to FALSE, otherwise it is set to
**   TRUE.  In the former case, the appropriate timezone rules should
**   be used to decide dst or not according to the complete time and
**   date specification.
**
**   Pattern element return value is always zero.
**/

static int
tzlkup(val)
int *val;
{
  int dstk,dstf;			/* dst by keyword and by flag: */
					/*  1 for on, 0 for off, -1 for */
					/*  unspecified */
  tzinf *tzi;				/* timezone information structure */
  int dstadj;				/* adjustment for dst in this zone */

#ifdef TRACE
  fprintf(stderr,"LOOKUP TZONE ");
#endif
  tzc = tztrans[tz];			/* get TZ code and dst flags */
  if (tzc & TZ_DST)			/* check for implicit dst spec */
    dstf = 1;
  else if (tzc & TZ_STD)
    dstf = 0;
  else					/* unspecified in flags */
    dstf = -1;
  tzc &= ~(TZ_DST | TZ_STD);		/* remove flags from tz code */
  tzi = dttzinf(tzc | TZ_CODE);		/* lookup timezone info by code */
  if (tzi == NULL)
    return(FALSE);			/* internal error - no such zone */
  tz = tzi->_tzoff;			/* get offset from Greenwich */
  
  dstadj = tzi->_tzdadj;		/* get dst adjustment for zone */
  if (dst == -1)			/* dst keyword given? */
    dstk = -1;				/* nope */
  else if (dst <= 1)			/* decode keyword for on or off? */
    dstk = 1;				/* DAYLIGHT or DST -- on */
  else
    dstk = 0;				/* STANDARD or STD -- off */

  dstspec = 0;				/* assume dst specified somewhere */
  if (dstf == -1)			/* unspecified in flags? */
    if (dstk == -1)			/* and unspecified by keyword? */
      dstspec = -1;			/* yup, totally unspecified */
    else
      dst = (dstk == 1 ? dstadj : 0);	/* specified by keyword - set dst */
  else if (dstk == -1)			/* given by flags, not by keyword? */
    dst = (dstf == 1 ? dstadj : 0);	/* then set according to flags */
  else if (dstk == dstf)		/* given by both - do they agree? */
    dst = (dstf == 1 ? dstadj : 0);	/* yup, set global flag */
  else
    return(FALSE);			/* disagreement means failure */
  *val = 0;				/* set return value */
  return(TRUE);				/* and succeed */
}



/* tzcalc
**
** Purpose:
**   Invoked by patterns where user specifies timezone by means of
**   an explicit offset from Greenwich.  The data is available in
**   three pieces - sign (negative for east of Greenwich), hours, and 
**   minutes.  Also, dst contains the index of a selected keyword
**   specifying daylight-savings-time or standard time, or -1 if
**   no such keyword was given.  The offset is combined into a
**   signed number of minutes west of Greenwich after checking that
**   the magnitude is no greater than 12:00, and the result is stored
**   in global variable tz.  The dst keyword is translated into an
**   adjustment value, which is stored back into variable dst.  If
**   a dst keyword was given, dstspec is set to 0, else to -1.
**
**   Pattern element return value is always zero.
**/

static int
tzcalc(val)
int *val;
{
  tzinf *tzi;				/* timezone information */
  
#ifdef TRACE
  fprintf(stderr,"CALC TZONE ");
#endif
  if ((tzhr == 12) && (tzmin > 0))	/* offset magnitude too large? */
    return(FALSE);
  tz = (tzhr * 60) + tzmin;		/* calculate offset magnitude */
  if (tzsign == 0)			/* sign is '+' (east of Greenwich)? */
    tz = -tz;				/* negate the offset */
  if (tz == -12*60)			/* 12 hours east? */
    tz = 12*60;				/* switch to 12 hours west */
  
  tzc = -1;				/* no timezone code available */

  if (dst == -1)			/* no dst keyword given? */
    dstspec = -1;			/* then dst not specified */
  else if (dst >= 2) {			/* STANDARD or STD given? */
    dstspec = 0;			/* then dst specified */
    dst = 0;				/* and adjustment is zero */
  }
  else {				/* DAYLIGHT or DST given */
    dstspec = 0;			/* signal that dst was specified */
    tzi = dttzinf(tz);			/* look up timezone info */
    if (tzi == NULL)			/* no such timezone? */
      dst = -60;			/* then assume we subtract 60 mins */
					/*  to get standard time */
    else
      dst = tzi->_tzdadj;		/* known zone - get dst adjustment */
  }
  return(TRUE);				/* successful operation */
}



/* fixampm
**
** Purpose:
**   Invoked after a time has been parsed, to check and adjust the time
**   according to the am/pm keyword or lack thereof.  If NOON or MIDNIGHT
**   was specified, the time must have been 12:00:00, and anything else
**   results in an error.  MIDNIGHT is changed to 00:00:00.  If AM or
**   PM is specified, the time must be from 1:00:00 to 12:59:59 inclusive.
**   Hour 12 is changed to 0, and then 12 is added if PM was specified.
**   In particular, this means that 12am is interpreted as midnight, and
**   12pm as noon.  If no keyword was given, the time must be in the range
**   00:00:00 to 24:00:00, and 24:00:00 is converted to 00:00:00.
**
**   Pattern element return value is always 0.
**/

static int
fixampm(val)
int *val;
{
#ifdef TRACE
  fprintf(stderr,"FIX AMPM ");
#endif
  if (ampm == -1) {			/* no keyword given? */
    if (hr == 24)			/* check special case of 24:mm:ss */
      if ((min != 0) || (sec != 0))	/* anything but 24:00:00? */
	return(FALSE);
      else
	hr = 0;				/* 24:00:00 becomes 0:00:00 */
  }
  else if ((ampm == 0) || (ampm == 2)) { /* noon or midnight? */
    if ((hr != 12) || (min != 0) || (sec != 0)) /* only 12:00:00 allowed */
      return(FALSE);
    if (ampm == 0)			/* midnight? */
      hr = 0;				/* change to 00:00:00 */
  }
  else {				/* AM or PM specified */
    if ((hr < 1) || (hr > 12))		/* hrs must be 1-12 */
      return(FALSE);
    else if (hr == 12)			/* change 12:mm:ss to 00:mm:ss */
      hr = 0;
    if (ampm == 3)			/* adjust PM up by 12 hours */
      hr += 12;
  }
  
  *val = 0;				/* all ok - set return value */
  return(TRUE);				/* and succeed */
}



/* fixyr
**
** Purpose:
**   Adjust a two-digit year by adding the current century.
**   Pattern element return value always zero
**/

static int
fixyr(val)
int *val;
{
  datime curdt;			/* holds current date/time */

#ifdef TRACE
  fprintf(stderr,"FIX YEAR ");
#endif
  if (yr < 100) {			/* two-digit year given? */
    dtnow(&curdt);			/* get current date */
    yr += (curdt._dtyr/100)*100;	/* add in current century */
  }
  *val = 0;				/* set return value */
  return(TRUE);				/* and succeed */
}



/* datchk
**
** Purpose:
**   Check the date that has been parsed, to make sure it is legitimate.
**   If no day-of-week was specified in the parse, compute the correct
**   day-of-week and store it in global variable dow.  Otherwise make
**   sure that the given day-of-week is correct.
**
**   Computations work under the incorrect assumption that the present
**   Gregorian calendar system has been in effect since the year 0000.
**   This is an ok assumption for all major cultures since 1923, when
**   Greece adopted the system, but before then, local calendar systems
**   varied.
**
**   Pattern element return value is always zero.
**/

static int
datchk(val)
int *val;
{
  /* lengths of the months, assuming a leap year */
  static int mthlens[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  int cdow;				/* calculated day-of-week */
  int i;
  
#ifdef TRACE
  fprintf(stderr,"CHECK DATE ");
#endif
  if (day >= mthlens[mon])
    return(FALSE);			/* day out of range for given month */
  if ((yr % 4 != 0) || ((yr % 100 == 0) && (yr % 400 != 0))) /* non-leap? */
    if ((mon == 1) && (day == 28))	/* then disallow February 29 */
     return(FALSE);
  cdow = dtdow(mon,day,yr);		/* compute day of week for date */

  if (dowspec == -1)			/* day-of-week not specified? */
    dow = cdow;				/* then install calculated value */
  else if (dow != cdow)			/* specified wrong? */
    return(FALSE);			/* then fail */
  *val = 0;				/* set return value */
  return(TRUE);				/* and succeed */
}
