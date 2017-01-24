/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* TODO: Finish dtdst, figure out why always GMT */

/* datime
**
** Utilities to deal with times and dates.  Two routines are machine-
** specific and must be written for each machine individually.  These
** are dtnow, which fills in a datime structure with the current
** date and time, and dttzone, which returns the local timezone in
** minutes west of Greenwich and a timezone id, if available.
**
** Required structure and constant declarations are to be found in datime.h.
** tzone.h contains declarations of timezone information, separated from
** this source for ease in modifying the rules.
**/

#include "datime.h"			/* get symbols */
#include "tzone.h"			/* declare timezone info structures */

#include <ctype.h>			/* our ispunct macro needs this */
#undef ispunct				/* get rid of theirs, define ours */
#define ispunct(c) (!(isdigit(c) || isupper(c) || islower(c)))

/* A few useful character constants */

#define NULCHAR	'\000'
#define TAB	'\011'
#define SPACE	'\040'



/* dtnow
**
** Purpose:
**   Return, via a datime structure supplied by the caller, the current
**   date and time, using local timezone information.
**
** Input arguments:
**   dtblk - Pointer to a datime block to be filled in.
**
** Output arguments:
**   dtblk - Fields filled in with current info.
**
** Returns: Nothing.
**/

#ifdef MSDOS
#include "dos.h"		/* get symbols to do a bios call */

/* macro to convert from 2-digit BCD to integer */

#define bcd2int(x) (((x)&0xf)+(10*(((x)>>4)&0xf)))

dtnow(dtblk)
datime *dtblk;
{
  union REGS inregs, outregs;	/* structures for communicating with BIOS */
  int junk;

  inregs.h.ah = 2;		/* read the time */
  int86(0x1a,&inregs,&outregs);
  dtblk->_dthr = bcd2int(outregs.h.ch); /* transfer values */
  dtblk->_dtmin = bcd2int(outregs.h.cl);
  dtblk->_dtsec = bcd2int(outregs.h.dh);

  inregs.h.ah = 4;		/* read the date */
  int86(0x1a,&inregs,&outregs);
  dtblk->_dtmon = bcd2int(outregs.h.dh)-1; /* transfer values */
  dtblk->_dtday = bcd2int(outregs.h.dl)-1;
  dtblk->_dtyr = bcd2int(outregs.h.cl)+100*bcd2int(outregs.h.ch);
				/* compute day-of-week */
  dtblk->_dtdow = dtdow(dtblk->_dtmon,dtblk->_dtday,dtblk->_dtyr);
  dttzone(&dtblk->_dttz,&dtblk->_dttzc); /* get local timezone info */
  dtblk->_dtdst = dtdst(dtblk); /* compute daylight savings time */
}
#endif
#if (BSD|SYSV)

#if BSD
#include <sys/time.h>
#else
#include <time.h>
#endif

dtnow(dtblk)
datime *dtblk;
{
  long t;
  struct tm *localtime(), *x;
  t = time(NULL);			/* get time */
  x = localtime(&t);			/* convert it */
  dtblk->_dthr = x->tm_hour;		/* hour */
  dtblk->_dtmin = x->tm_min;		/* minute */
  dtblk->_dtsec = x->tm_sec;		/* second */
  dtblk->_dtmon = x->tm_mon;		/* month */
  dtblk->_dtday = x->tm_mday -1;	/* day of month */
  dtblk->_dtyr = x->tm_year+1900;	/* year */
  dtblk->_dtdow = x->tm_wday;		/* week day */
  dttzone(&dtblk->_dttz,&dtblk->_dttzc);/* time zone */
  dtblk->_dtdst = dtdst(dtblk);		/* daylight savings time */
}
#endif /*  (BSD|SYSV) */


/* dttzone
**
** Purpose:
**   Returns the local timezone, as minutes west of Greenwich.  Daylight
**   savings time is not considered.  If a timezone ID is known, it is
**   returned as well.
**
** Input arguments: None.
** Output arguments:
**   offset - Number of minutes west of Greenwich, in range -7199 to 7200
**     (that is, the international dateline is considered 12 hours WEST,
**     not 12 hours EAST of Greenwich)
**   code - Timezone code number if known, or -1
**
** Returns: Nothing
**/

#if MSDOS
dttzone(offset,code)
int *offset,*code;
{
  *offset = LCL_TZOFF;		/* get offset defined in datime.h */
#ifdef LCL_TZCODE
  *code = LCL_TZCODE;		/* and code from datime.h if there is one */
#else
  *code = -1;			/* otherwise signal no code */
#endif
}
#endif

#if (BSD)
dttzone(offset, code)
int *offset, *code;
{
  struct timeval tp;
  struct timezone tz;
  gettimeofday(&tp,&tz);		/* get time of day */
  *offset = tz.tz_minuteswest;
#ifdef LCL_TZCODE
  *code = LCL_TZCODE;		   /* and code from datime.h if there is one */
#else
  *code = -1;				/* otherwise signal no code */
#endif
}  
#endif /*  BSD */

#ifdef SYSV
dttzone(offset, code)
int *offset, *code;
{
  *offset = timezone / 60;		/* get the correct timezone */
#ifdef LCL_TZCODE
  *code = LCL_TZCODE;		   /* and code from datime.h if there is one */
#else
  *code = -1;				/* otherwise signal no code */
#endif
}  
#endif /*  SYSV */




/* dttzinf
**
** Purpose:
**   Return a pointer to a timezone information structure, given
**   a timezone code or an offset from Greenwich.  The former style
**   of call is identified by the input argument being a positive
**   value with flag TZ_CODE set.  Anything else is taken to be
**   a signed number of minutes west of Greenwich.  In the former
**   case, the tzinfo array is scanned for a timezone with the given
**   code, and a pointer to the corresponding tzinf structure is
**   returned.  When an offset is given, the first timezone with
**   the given offset is returned (so timezones with different
**   dst rules, or in different hemispheres, cannot be distinguished
**   by means of their offsets).  In either case, if no matching
**   timezone is located, NULL is returned.
**
** Input arguments:
**   key - Signed offset in minutes west of Greenwich, or positive
**     timezone code value with the TZ_CODE flag turned on.
**
** Output arguments: None.
** Returns: Pointer to first matching tzinf structure, or NULL if none found.
**/

tzinf *
dttzinf(key)
int key;
{
  int bycode;				/* TRUE if looking up by code */
  int i;

  if ((key > 0) && (key & TZ_CODE)) {	/* lookup by code? */
    bycode = TRUE;			/* set flag */
    key &= ~TZ_CODE;			/* and isolate the code value */
  }
  else
    bycode = FALSE;			/* otherwise clear flag */

  for (i = 0; i < TZCOUNT; i++)		/* loop through the table */
    if (key == (bycode ? tzinfo[i]._tzcode : tzinfo[i]._tzoff)) 
      return(&tzinfo[i]);		/* found indicated zone */
  return(NULL);				/* zone not in table */
}



/* dtdow
**
** Purpose:
**   Given a date, compute the day of week on which the date falls,
**   according to the Gregorian calendar system.  The calculation
**   assumes that the Gregorian system has been in effect since
**   1-Jan-0000, which is not true.  In general, correct results
**   will be given for dates after 1923, by which time all the
**   major cultures had adopted the calendar.  See a world almanac
**   or other source for details of who joined the system when.
**
** Input arguments:
**   mon - The month for the date in question, 0 = January
**   day - The day of the month, with 0 = the first day.
**   yr - The year in question, with no conversion for years
**     less than 100.
** 
** Output arguments: None.
** Returns: Day of week for given date, with 0 = Sunday, 6 = Saturday.
**/

/* Useful macro to decide if a year is a leap year */

#define LEAP(y)	(((y)%400 == 0) || (((y)%100 != 0) && ((y)%4 == 0)))

/* lengths of the months, assuming a leap year */
static int mthlens[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int
dtdow(mon,day,yr)
int mon,day,yr;
{
  int dow;				/* calculated day of week */
  int i;
  
  /* First calculate day-of-week relative to 1-Jan-0000, that is,
  ** the number of days since that date, modulo 7.  A first approximation
  ** accounts for years prior to the given year, by calculating (365*yr) % 7.
  ** But since 365%7 is 1, this is the same as (1*yr) % 7, or yr % 7.
  ** Adjustments are then made for leap years, and then for preceding months
  ** in the given year, and preceding days in the given month.
  **/
  dow = yr % 7;				/* start with days in prior years */
  dow += (yr+3) / 4;			/* adjust for preceding leap years */
  dow -= (yr+99) / 100;			/* most centuries are non-leap */
  dow += (yr+399) / 400;		/* but every fourth one is */
  for (i = 0; i < mon; i++)
    dow += mthlens[i];			/* add preceding months this year */
  if ((mon > 1) && !LEAP(yr))		/* after non-leap Feb? */
    dow -= 1;				/* yup, we added to much */
  dow += day;				/* now adjust for date within month */
  dow += 6;				/* Greg 1-Jan-0000 was a Saturday */
  dow %= 7;				/* get dow in range 0-7 */
  return(dow);				/* give result to caller */
}



/* dtdst
**
** Purpose:
**   Given a datime structure filled in with date, time, and timezone
**   information, compute the daylight-savings-time adjustment that
**   should be in effect according to whatever rules we have for the
**   given timezone.  If no appropriate rule exists, assume no adjustment
**   (that is, an adjustment of zero).  Generally, when dst is in effect,
**   the adjustment will be -60, meaning that to convert to standard time,
**   60 minutes should be subtracted from the given time.  If a timezone
**   code is present in the datime structure, it is used to find the dst
**   rules.  Otherwise, the timezone offset is used.  In the latter case,
**   the wrong rules may be applied since timezones with identical offsets
**   but different dst rules will not be distinguished.
**
** Input arguments:
**   dtblk - A pointer to the datime block holding the date, time, and zone
**     information to use.
**
** Output arguments: None.  (In particular, the _dtdst field of the given
**   datime block is NOT modified.)
**
** Returns: Number of minutes to add to given time in order to convert
**   to standard time in its timezone (so adding this value as well
**   as the _dttz value will result in Greenwich Mean Time).
**
**/

int
dtdst(dtblk)
datime *dtblk;
{
  tzinf *tzi;			/* time zone info block */
  dstrule *rule;		/* applicable dst rule */
  int julian, dston, dstoff, i;

  if (dtblk->_dttzc != -1)
    tzi = dttzinf(TZ_CODE | dtblk->_dttzc); /* look up by code if set */
  else
    tzi = dttzinf(dtblk->_dttz); /* otherwise look up by offset */

  if (tzi == NULL) 
    return(0);			/* no adjustment if tz not known */

  /* calculate julian date of current date */
  for (julian = dtblk->_dtday+1, i = 0; i < dtblk->_dtmon; i++)
    julian += mthlens[i];
  if ((dtblk->_dtmon > 1) && !LEAP(dtblk->_dtyr))
    julian--;
  
  for (rule = tzi->_tzdst; ; rule++) {
    if (dtblk->_dtyr < rule->_dsyr1 || dtblk->_dtyr > rule->_dsyrn) {
      if (rule->_dsyr1 > 0)
	continue;
      else return 0;
    }
    
    /* get appropximate dates of dst on/off */
    dston = rule->_dsdow;	/* XXX struct member is mislabeled */
    dstoff = rule->_dsday;	/* XXX struct member is mislabeled */
    
    /* account for feb 29th */
    if (!LEAP(dtblk->_dtyr)) {
      if (dston >= 59)
	dston--, dstoff--;
      else
	if (dstoff >= 59)
	  dstoff--;
    }
    
    /*
     * get actual julian dates for dst on/off, assumed to be Sunday
     * previous to the approximate date.
     */
    
    /* get days from Jan 1st to first Sunday */
    i = (julian - 1 + 7 - dtblk->_dtdow) % 7;
    /* subtract number of days between dston and previous Sunday */
    dston -= ((((dston - 1) % 7) - i) + 7) % 7;
    dstoff -= ((((dstoff - 1) % 7) - i) + 7) % 7;
    

    if ((julian < dston) || (julian > dstoff) ||
	(julian == dston && (dtblk->_dthr < 2)) ||
	(julian == dstoff && (dtblk->_dthr >= (tzi->_tzsth ? 2 : 1)))) {
	  if (rule->_dsyr1)
	    continue;
	  else return (0);
	}
    return (- tzi->_tzdadj);
  }
}



/* dtparse
**
** Purpose:
**   Attempt to interpret a given input string as a date/time specification.
**   If successful, fill the results into a given datime block.  A successful
**   parse need not consume all the available input.
**   
**   The input must match one of the patterns declared in dtpat.h, which
**   cover a wide range of formats.  Flags may be passed to constrain
**   which patterns may be used, or to indicate that either the time or
**   the date portion is not to be parsed.  The patterns are ranked so
**   that a pattern of a higher rank will be chosen when there is
**   ambiguity in the input.  The ranking can be supplemented by a set of
**   adjustments that depend on the flags that are specified in the parse,
**   so that the ranking can depend on the flags.  For details of how
**   the patterns are constructed, read the comments in dtpat.h.
**
**   Parsing succeeds if, of all the patterns matching the current input,
**   there is a single matching pattern of highest rank.  The input is
**   decoded according to that pattern, checked for acceptability, and
**   then returned via the  datime structure supplied by the caller. 
**
**   Completion text is set by some pattern elements when they encounter
**   the end of the input.
**
** Input arguments:
**   flags - Flags to control the parse, as defined in datime.h
**   text - A pointer to the text to be parsed.
**   textlen - The number of characters in the text to be parsed.
**
** Output arguments:
**   dtblk - Filled in with date/time info from parse if successful.
**     If time is not given, 00:00:01 is used, with the local timezone
**     and daylight savings computed by the rules.  If date is not
**     given, the current date is filled in.  Day of week is computed
**     from the date, and if the input specifies a day of week, it
**     must match the computed day in order to succeed.
**   parselen - The number of characters consumed by a successful parse.
**   completion - A pointer to text that can be used to complete
**     the current input (generally only partial completion results)
**     if the date/time specification is incomplete.
**   incomplete - TRUE iff the parse failed due to lack of sufficient
**     input.
**
** Returns: TRUE iff the parse succeeds.
**/

/* Define TRACE here if a pattern match trace is desired */

/* #define TRACE */

/* Global variables used by the pattern matching routines */

static char *dttext;		/* pointer to input text */
static int dtlen;		/* # of chars left to parse */
static int dtinc;		/* parse failed for lack of text */
static char *dtcmp;		/* pointer to completion text after */
				/*  incomplete parse */

#include "dtpat.h"		/* load in the patterns */

int
dtparse(flags,text,textlen,dtblk,parselen,completion,incomplete)
int flags,textlen,*parselen,*incomplete;
char *text,**completion;
datime *dtblk;
{
  datime curdt;			/* current time/date go here */
  dtpat *pats;			/* pattern array selected by flags */

  dttext = text;		/* set up global variables */
  dtlen = textlen;
  dtinc = *incomplete = FALSE;	/* assume not incomplete */

  if (flags & DTP_NDA)		/* check exclusions from parse */
    if (flags & DTP_NTI)
      return(FALSE);		/* cannot exlude both time and date */
    else
      pats = timpats;		/* exclude date but not time */
  else
    if (flags & DTP_NTI)
      pats = datpats;		/* exclude time but not date */
    else
      pats = dtpats;		/* exclude neither */
  if (!match(pats,flags)) {	/* attempt a parse */
    if (dtinc) {		/* reached end of input? */
      *incomplete = TRUE;	/* yup, let them know */	
      *completion = dtcmp;	/* and pass on completion text */
    }
    return(FALSE);		/* give a failure return */
  }
  if (flags & DTP_NDA) {	/* need current date info? */
    dtnow(&curdt);		/* get it */
    dtblk->_dtmon = curdt._dtmon; /* and transfer date info */
    dtblk->_dtday = curdt._dtday;
    dtblk->_dtyr = curdt._dtyr;
    dtblk->_dtdow = curdt._dtdow;
  }
  else {
    dtblk->_dtmon = mon;	/* otherwise transfer parsed values */
    dtblk->_dtday = day;
    dtblk->_dtyr = yr;
    dtblk->_dtdow = dow;
  }
  
  if (flags & DTP_NTI) {	/* time not parsed? */
    dtblk->_dthr = 0;		/* fill in one second past midnight */
    dtblk->_dtmin = 0;
    dtblk->_dtsec = 1;
    dttzone(&dtblk->_dttz,&dtblk->_dttzc); /* get local timezone info */
    dstspec = -1;		/* need dst computation */
  }
  else {			/* parsed time - fill in values */
    dtblk->_dthr = hr;
    dtblk->_dtmin = min;
    dtblk->_dtsec = sec;
    if (tzspec != -1) {		/* timezone given? */
      dtblk->_dttz = tz;	/* then transfer it */
      dtblk->_dttzc = tzc;
    }
    else {
      dttzone(&dtblk->_dttz,&dtblk->_dttzc); /* otherwise get local zone */
      dstspec = -1;		/* and no dst indicator given */
    }
  }
  if (dstspec == -1)		/* dst indicator not given? */
    dtblk->_dtdst = dtdst(dtblk); /* then compute it by the rules */
  else
    dtblk->_dtdst = dst;	/* else set value from parse */

  *parselen = dttext - text;	/* set # of chars consumed */
  
  return(TRUE);			/* and succeed */
}


/* The pattern matcher routines */

#define MAXVAL	2
static int patval[MAXVAL];		/* values set by pattern elements */

/* match
** 
** Purpose:
**   Apply a given set of patterns to the text currently pointed to by
**   global variable 'dttext'.  If there is a single succeeding pattern
**   of highest rank, consume the characters and return TRUE.  Otherwise
**   return FALSE.  As soon as a highest-rank failing pattern sets the 
**   dtinc flag, a FALSE return is made immediately without attempting 
**   other patterns.  On success, the index of the succeeding pattern in 
**   the pattern array is returned via the patval array.
**
** Input arguments:
**   pats - Pointer to the first pattern in an array of patterns to attempt,
**     terminated by a pattern whose pattern string is NULL.
**   flags - The flags specified in the FDB for the parse.
**
** Output arguments: None
** Returns: TRUE if matching succeeds, FALSE otherwise.
**
** Global variables:
**   dttext - Pointer to the text to be matched, updated after a successful 
**     match
**   dtlen - Number of characters in input text, updated after success
**   dtinc - TRUE after a failing match that failed because of insufficient
**     input text.
**   dtcmp - After an incomplete match, points to appropriate completion 
**     text, or NULL if there is none.
**/


#ifdef TRACE
static int tracelev = 0;    	    	/* indentation level for trace */
#endif

static int
match(pats,flags)
dtpat *pats;
int flags;
{
#ifdef TRACE
  int ret;				/* results of match operation */
  tracelev +=2;
  trindent();fprintf(stderr,"Matching: ");trshow();trnl();
  tracelev +=2;
  ret = match1(pats,flags);
  tracelev -= 2;
  trindent();fprintf(stderr,"Match result: %s\n",(ret ? "SUCCESS":"FAILURE"));
  tracelev -= 2;
  return(ret);
}
static int
match1(pats,flags)
dtpat *pats;
int flags;
{  
#endif

  char *origtext = dttext;		/* save input parameters */
  int origlen = dtlen;			/*  to be restored on failure */
  char *savtext;			/* input params after a good match */
  int savlen;
  int rank;				/* ranks of individual patterns */
  int hirank;				/* highest pattern rank */
  dtpat *p;				/* for stepping through patterns */
  int matched = FALSE;			/* TRUE if a pattern succeeds */
  
  hirank = -1;				/* assume no patterns eligible */
  for (p = pats; p->_dppat != NULL; p++) { /* step through the patterns */
    rank = patrank(p,flags);		/* compute ranks */
    if (rank > hirank)
      hirank = rank;			/* and remember highest rank */
  }
  
  while (hirank >= 0) {			/* step down through eligible ranks */
    for (p = pats; p->_dppat != NULL; p++) /* loop through pat at that rank */
      if (patrank(p,flags) == hirank) {
	if (matchpat(p,flags))		/* good match? */
#ifdef TRACE
        { tracelev -= 2; trindent(); fprintf(stderr,"PATTERN SUCCEEDED\n");
#endif
	  if (!matched) {		/* first one? */
	    matched = TRUE;		/* then say we matched */
	    savtext = dttext;		/* save the input parameters */
	    savlen = dtlen;
	    set1val(p - pats);		/* set index of good pattern for */
					/*  return value for %r call */
          }
	  else
	    return(FALSE);		/* multiple highest-rank matches */
#ifdef TRACE
    	}
#endif
        else if (dtinc)			/* incomplete match? */
#ifdef TRACE
          { tracelev -= 2; trindent(); fprintf(stderr,"PATTERN FAILED (INC)\n");
#endif
	  return(FALSE);		/* then quit now */
#ifdef TRACE
    	  }
	else {
	  tracelev -= 2; trindent(); fprintf(stderr,"PATTERN FAILED\n");
	}
#endif

        dttext = origtext;		/* restore input for next pattern */
	dtlen = origlen;
      }
    if (matched) {			/* single match at this rank? */
      dttext = savtext;			/* fix global vars to consume input */
      dtlen = savlen;
      return(TRUE);			/* and succeed */
    }
    else
      hirank--;				/* no match, try next lower rank */
  }

  return(FALSE);			/* no success at any rank */
}



/* patrank
**
** Purpose:
**   Compute the rank for a parse pattern with a given set of flags.
**   If any of the flags disqualifies the pattern (via its _dpqal field),
**   -1 is returned.  Otherwise, rank adjustments are applied, to the
**   base rank, and the adjusted rank is returned.
**
** Input args:
**   pat - A pointer to the pattern to be ranked.
**   flags - Parse flags active for the parse.
**
** Output args: None
** Returns: Adjusted rank.
**/

static int
patrank(pat,flags)
dtpat *pat;
int flags;
{
  int rank;				/* base rank for the pattern */
  rankadj *adj;				/* pointer to adjustment array */

  if (flags & pat->_dpqal)		/* any disqualifying flags active? */
    return(-1);				/* yup, throw it out */

  rank = pat->_dprnk;			/* get the base rank */
  adj = pat->_dpraj;			/* and point to adjustments array */
  if (adj != NULL)
    while (adj->_raflg != -1) {		/* loop through adjustments */
      if (adj->_raflg & flags)		/* adjustment indicated by flags? */
	rank += adj->_raadj;		/* yup, adjust the rank */
      adj++;				/* and move to next adjustment */
    }
  return(rank);				/* give back adjusted rank */
}



/* matchpat
**
** Purpose:
**   Apply a given pattern to the current input.  Return TRUE if all the
**   required special pattern elements succeed and if intervening pattern 
**   characters match the input.  Otherwise return FALSE.  After a success,
**   the global input parameters (dttext and dtlen) will be left so as to
**   consume the input characters that participated in the match.  On
**   failure caused by insufficient input text, global variable dtinc
**   will be set TRUE, and dtcmp may point to completion text to be
**   filled in if completion is requested.
**
** Input args:
**   pat - A pointer to the pattern to be matched.
**   flags - The flags in effect for the parse.
**
** Output args: None
** Returns: TRUE for a successful match, FALSE for a failure.
**/

static int
matchpat(pat,flags)
dtpat *pat;
int flags;
{
  int **patarg = pat->_dparg;		/* point to pattern args */
  char *pats = pat->_dppat;		/* and to pattern string */
  char pc,tc;				/* chars from pat & dttext input */
  char *opttext;			/* vars to save input state */
  int optlen;				/*  before an optional pat element */
  int opt;				/* TRUE iff during optional pat elt */
  int nogood;				/* TRUE after a failing pattern elt */
  char *delims;				/* pointer to a %d arg string */
  int i;
  
#ifdef TRACE
  trindent();fprintf(stderr,"Pattern [%s] Input [",pats);
  trshow();fprintf(stderr,"]\n");
  tracelev += 2;
#endif

  clearval();				/* clear the pattern values */

  while ((pc = *pats++) != NULCHAR) {	/* scan through pattern string */
    if (pc == SPACE)
      continue;				/* skip spaces in pattern */
    if (pc != '%') {			/* normal character */
#ifdef TRACE
      trindent();fprintf(stderr,"Char [%c] ",pc);
#endif
      if (dtlen == 0) {		/* reached end of input */
	dtinc = TRUE;			/* signal incomplete match */
	dtcmp = NULL;			/* no completion on normal chars */
#ifdef TRACE
    	fprintf(stderr,"INC\n");
#endif
	return(FALSE);			/* and fail */
      }
      tc = *dttext++;			/* get next input char */
      dtlen--;				/* and consume it */
      if (islower(tc))			/* convert to upper case */
	tc = toupper(tc);
      if (islower(pc))
	pc = toupper(pc);
      if (pc != tc)
#ifdef TRACE
    	{ fprintf(stderr,"NO\n");
#endif
	return(FALSE);			/* fail if chars do not match */
#ifdef TRACE
    	}
      else
        fprintf(stderr,"OK\n");
#endif
      continue;				/* otherwise continue scanning */
    }
    pc = *pats++;			/* get special element char */
#ifdef TRACE
    trindent();
#endif
    if (pc == '?') {			/* this element optional? */
#ifdef TRACE
      fprintf(stderr,"Optional ");
#endif
      opt = TRUE;			/* flag it */
      opttext = dttext;			/* and save input state */
      optlen = dtlen;
      pc = *pats++;			/* and get next pattern char */
    }
    else
      opt = FALSE;			/* else clear optional flag */
#ifdef TRACE
    fprintf(stderr,"Special [%c] Args [",pc); trargs();
    fprintf(stderr,"] Input [");trshow();
    fprintf(stderr,"] ");
#endif
    nogood = FALSE;			/* assume pat element will succeed */

    switch(pc) {			/* now dispatch to element handler */
      case '=':				/* assign value from prior element */
	**(patarg++) = patval[0];	/* assign value, move to next arg */
	for (i = 1; i < MAXVAL; i++)	/* cycle the pattern value array */
	  patval[i-1] = patval[i];
	patval[MAXVAL-1] = -1;		/* and clear the last value */
	break;

      case '%':				/* match a percent sign */
	if (dtlen-- == 0)		/* insufficient input? */
	  dtinc = nogood = TRUE;	/* yes, flag incomplete failure */
        else if ((*dttext++) != '%')
	  nogood = TRUE;		/* check for next char match */
	else
	  set1val(0);			/* returns 0 on success */
	break;
      
      case SPACE:			/* match a space */
	if (dtlen-- == 0)		/* insufficient input? */
	   dtinc = nogood = TRUE;	/* yes, flag incomplete failure */
        else if ((*dttext++) != SPACE)
	  nogood = TRUE;		/* next char not a space */
        else
	  set1val(0);			/* good match */
	break;
	
      case 'c':				/* match next pattern char */
      case 'C':
#ifdef TRACE
        fprintf(stderr,"Char [%c] ",*(pats+1));
#endif
	if (dtlen-- == 0)		/* insufficient input? */
	   dtinc = nogood = TRUE;	/* yes, flag incomplete failure */
        else if ((*dttext++) != *pats++)
	  nogood = TRUE;		/* next char does not match */
        else
	  set1val(0);			/* good match */
	break;	

      case 'k':				/* keyword match */
      case 'K':
	nogood = !matchkey(*patarg++);  /* attempt the match */
	break;

      case '#':				/* one of the numeric patterns */
	nogood = !matchnum(*pats++);	/* attempt numeric parse */
        break;

      case 'p':				/* next char punctuation */
      case 'P':
	if (dtlen <= 0) {		/* insufficient input? */
	  dtinc = nogood = TRUE;	/* then set incomplete failure */
	  dtcmp = " ";		/* complete with a space */
        }
	else {
	  tc = *dttext;			/* get next char, do not consume it */
	  if (!ispunct(tc))		/* not a punctuation char? */
	    nogood = TRUE;		/* then signal failure */
          else
	    set1val((int) tc);		/* return ASCII code of char */
        }
	break;
	
      case 'w':				/* match a run of whitespace */
      case 'W':
	if (dtlen <= 0) {		/* insufficient chars? */
	  dtinc = nogood = TRUE;	/* signal incomplete failure */
	  dtcmp = " ";			/* complete with a space */
	  break;
        }
	tc = *dttext;			/* get next input char, unquoted */
	if ((tc != SPACE) && (tc != TAB)) /* first char not whitespace? */
	  nogood = TRUE;
        else {
	  while ((--dtlen) > 0) {	/* loop through nonempty run */
	    tc = *(++dttext);
	    if ((tc != SPACE) && (tc != TAB))
	      break;			/* until non-whitespace encountered */
          }
	  if (dtlen == 0)		/* if exited loop for lack of input */
	    dttext++;			/*  pointer was not bumped enough */
          set1val(0);			/* always return zero */
        }
        break;

      case 'd':				/* match a delimiter char */
      case 'D':
	delims = (char *) *patarg++;	/* point to delimiter set */
#ifdef TRACE
    	fprintf(stderr,"Delims [%s] ",delims);
#endif
	if (dtlen <= 0) {		/* no input left? */
	  nogood = dtinc = TRUE;	/* signal incomplete failure */
	  dtcmp = NULL;		/* no completion */
        }
	else {
	  nogood = TRUE;		/* assume first char not delimiter */
	  tc = *dttext++;			/* get next input char */
	  dtlen--;			/* and count it */
	  for (i = 0; delims[i] != NULCHAR; i++) /* loop through delimiters */
	    if (delims[i] == tc) {	/* found a match? */
	      nogood = FALSE;		/* signal success */	
	      set1val((int) i);		/* return index of matching delim */
	      break;
            }
        }
	break;
	
      case 'r':				/* recursively invoke a pattern set */
      case 'R':
#ifdef TRACE
        trnl();
#endif
	nogood = !match(*patarg++,flags); /* invoke the pattern matcher */
#ifdef TRACE
        trindent(); fprintf(stderr,"Result for %r: ");
#endif
	break;

      case 'f':				/* invoke a function */
      case 'F':
	nogood = !(*((int (*)()) *patarg++))(&i); /* get return value in i */
	if (!nogood)
	  set1val(i);			/* set return value on success */
	break;
	
      case 'z':				/* store a zero value */
      case 'Z':
	*(*patarg++) = 0;		/* set the value, move to next arg */
	break;
	
      case 'n':				/* store a -1 value */
      case 'N':
	*(*patarg++) = -1;		/* set the value, move to next arg */
	break;
	
      case '+':				/* skip next argument */
	patarg++;			/* bump the pointer */
	break;
	
      default:				/* anything else is an error */
	nogood = TRUE;
        break;
    }
    
    if (dtinc)				/* incomplete parse? */
#ifdef TRACE
      { fprintf(stderr,"INC\n");
#endif
      return(FALSE);			/* yes, fail immediately */
#ifdef TRACE
      }
#endif

    if (nogood)				/* pattern element failed? */
      if (opt) {			/* optional element? */
#ifdef TRACE
        fprintf(stderr,"NO (OK)\n");
#endif
	clearval();			/* yes, all values to -1 */
	dttext = opttext;		/* restore input pointers */
	dtlen = optlen;
      }
      else
#ifdef TRACE
        { fprintf(stderr,"NO\n");
#endif
	return(FALSE);			/* fail if failing elt not optional */
#ifdef TRACE
        }
    else
       fprintf(stderr,"OK\n");
#endif

  }
  return(TRUE);				/* got through entire pattern */
}



/* matchkey
**
** Purpose:
**   Implement the %k special pattern element.  Input characters are matched
**   against the keywords in the given array until a matching key is located,
**   or until they have all failed to match.  In the former case, the index
**   of the matching keyword is set into the patval array as the value
**   returned by this pattern element, and TRUE is returned.  In the latter
**   case, FALSE is returned.  If matching fails on any keyword as a result
**   of insufficient input, dtinc is set and an immediate failure is given.
**   A keyword matches the input only if the input contains all the characters
**   contained in the keyword up to a vertical bar in the keyword.  After 
**   that, additional matching characters are consumed as well, up to the
**   first nonmatching character or the end of the keyword.  Case is ignored
**   when matching letters.
**
**   In the case that the end of the input is reached (causing dtinc to be
**   set), if the current input uniquely matches exactly one of the keywords,
**   the remainder of that keyword is set as the completion text, even if
**   the input has not reached the vertical bar marker for that keyword.
** Input arguments:
**   keys - Pointer to an array of keyword strings, terminated by NULL.
**
** Output arguments: None.
** Returns: TRUE for success, FALSE for failure or incomplete.
**/


static int
matchkey(keys)
char **keys;
{
  int len;				/* length of individual key match */
  char *key;				/* pointer to individual key */
  int sawbar;				/* TRUE if bar seen in matching key */
  int i,j,k;
#define KEYCMPLEN 15			/* size of following buffer */
  static char keycmp[KEYCMPLEN];	/* keyword completion text buffer */

#ifdef TRACE
  fprintf(stderr,"Keys [%s...] ",keys[0]);
#endif
  if (dtlen == 0) {			/* no input to match? */
    dtinc = TRUE;			/* set incomplete flag */
    dtcmp = NULL;			/* and no completion */
    return(FALSE);			/* and fail immediately */
  }
	  
  for (i = 0; (key = keys[i]) != NULL; i++) { /* loop through the keys */
    len = matchk1(dttext,dtlen,key,&sawbar); /* try to match current key */
    if (dtinc || sawbar)
      break;				/* exit on success or end of input */
  }
  
  if (dtinc) {				/* ran out of input? */
    key += len;				/* point to remainder of keyword */
    if (sawbar) key++;			/* account for bar if seen */
    for (j = k = 0; k < KEYCMPLEN-1; j++) /* copy it to our buffer */
      if (key[j] == '|')
	continue;			/* skip the bar */
      else if (key[j] == NULCHAR)
	break;				/* exit at end of key */
      else
	keycmp[k++] = key[j];		/* copy other chars */
    keycmp[k] = NULCHAR;		/* tie off completion buffer */
    dtcmp = keycmp;			/* assume this is our completion */
    if (!sawbar)			/* if less than minimum match */
      while ((key = keys[++i]) != NULL)	/*  then look for a 2nd match */
        if (matchk1(dttext,dtlen,key,&sawbar) == len) { 
	  dtcmp = NULL;			/* if found, then just beep */
	  break;
        }
    return(FALSE);			/* fail the match in any case */
  }
  else if (sawbar) {			/* successful match? */
    dttext += len;			/* update input pointers */
    dtlen -= len;
    set1val(i);				/* set pattern return value */
    return(TRUE);			/* and succeed */
  }
  else					/* all keys failed outright */
    return(FALSE);			/* so just fail */
}



/* matchk1 - Aux routine for matchkey
**
** Purpose:
**   Determine how much of the input text matches a given keyword, and
**   return the number of matching chars.   Set a return flag indicating
**   whether or not a vertical bar was passed during the match (meaning
**   that the text should be accepted as matching the keyword).
**
** Input arguments:
**   text - A pointer to the text to be matched.
**   textlen - The length of the text to be matched.  Guaranteed not
**     to be zero.
**   key - A pointer to the null-terminated keyword text.
**
** Output arguments:
**   sawbar - Set TRUE if a vertical bar is passed while matching
**     the key.  Otherwise, FALSE is set.
**/

static int
matchk1(text,textlen,key,sawbar)
char *text,*key;
int textlen,*sawbar;
{
  char tc,kc;				/* individual chars to compare */
  int len = 0;				/* number of chars matched */

  *sawbar = FALSE;			/* bar not seen yet */
  while (textlen != 0) {		/* loop through text */
    tc = *text++;			/* get next char */
    textlen--;				/* and count it */
    if ((kc = *key++) == NULCHAR) 
      break;				/* exit at end of key */
    if (kc == '|') {
      *sawbar = TRUE;			/* there's the bar! */
      if ((kc = *key++) == NULCHAR)	/* move to next char */
	break;
    }
    if (islower(tc))			/* convert to upper case */
      tc = toupper(tc);
    if (islower(kc))
      kc = toupper(kc);
    if (tc != kc)			/* chars don't match? */
      return(len);			/* return length of match */
    else
      len++;				/* count matching chars */
  }

  if (kc == '|')			/* input just barely met minimum? */
    *sawbar = TRUE;			/* yup, set flag */
  else if (kc != NULCHAR)		/* reached end of input? */
    dtinc = TRUE;			/* set incomplete flag */
  return(len);				/* give number of matching chars */
}



/* matchnum
**
** Purpose:
**   Parse a decimal number in the current input, and check the value
**   according to the number type indicated by the passed character
**   (one of the characters that can follow "%#" in a pattern).
**   Set return value(s) according to the type, as well.
**
** Input arguments:
**   type - Character selecting the treatment required for the parsed number.
**
** Output args: None.
** Returns: TRUE if successful parse, FALSE otherwise.
**/

static int
matchnum(type)
char type;
{
  int val = 0;				/* value parsed from data */
  char tc;				/* individual chars from input */
  int digcnt = 0;			/* number of digits in number */

#ifdef TRACE
  fprintf(stderr,"Type [%c] ",type);
#endif
  if (dtlen <= 0) {			/* no input left? */
    dtinc = TRUE;			/* set incomplete flag */
    dtcmp = NULL;			/* no completion text */
    return(FALSE);			/* and fail */
  }
  tc = *dttext;				/* get first char */
  if (!isdigit(tc))
    return(FALSE);			/* no digits to parse */
  while (isdigit(tc)) {			/* loop through digits */
    val = (val*10) + tc - '0';		/* add digits into value */
    digcnt++;				/* and count them */
    tc = *(++dttext);			/* grab the next character */
    if ((--dtlen) <= 0)			/* and count the last one */
      break;				/* exit loop if no more chars */
  }

  switch(type) {			/* now validate the number */
    case 'm':				/* month number, range 1-12 */
      if ((digcnt > 2) || (val < 1) || (val > 12))
	return(FALSE);			/* out of range */
      val--;				/* drop to zero origin */
      break;
    case 'd':				/* day number, range 1-31 */
      if ((digcnt > 2) || (val < 1) || (val > 31))
	return(FALSE);			/* out of range */
      val--;				/* drop to zero origin */
      break;
    case 'y':				/* 2-digit year number */
      if ((digcnt != 2) || (val > 99))	/* value out of range? */
	return(FALSE);
      break;				/* leave good value as is */
    case 'Y':				/* 4-digit year number */
      if ((digcnt < 3) || (digcnt > 4) || (val > 9999))
	return(FALSE);			/* out of range */
      break;				/* leave good value as is */
    case 'h':				/* hour in range 1-12 */
      if ((digcnt > 2) || (val < 1) || (val > 12))
	return(FALSE);			/* out of range */
      break;				/* no modificaton to value */
    case 'H':				/* hour in range 0-24 */
      if ((digcnt > 2) || (val > 24))
	return(FALSE);			/* out of range */
      break;				/* no modification to value */
    case 'M':				/* 4-digit hours-and-minutes */
      if ((digcnt < 3) || (digcnt > 4) || (val > 2400))
	return(FALSE);			/* out of range */
      if ((val % 100) > 59)
	return(FALSE);			/* minutes out of range */
      set2val(val/100,val%100);		/* break up the input */
      return(TRUE);			/* and succeed */
    case 's':				/* seconds or minutes, range 0-59 */
      if ((digcnt != 2) || (val > 59))
	return(FALSE);			/* out of range */
      break;				/* good value is left as is */
    default:				/* unknown type */
      return(FALSE);			/* just fail */
  }
  
  set1val(val);				/* set return value */
  return(TRUE);				/* and succeed */
}



/* Routines for manipulating return value array:
**   clearval - Set all values to -1.
**   set1val - Set first value, clear the rest to -1.
**   set2val - Set first two values, clear the rest to -1.
**
** Input arguments:
**   val1 - First value to set (set1val and set2val only)
**   val2 - Second value to set (set2val only)
**
** Output arguments: None.
** Returns: Nothing.
**/

static
clearval()
{
  int i;
  for (i = 0; i < MAXVAL; i++)		/* loop through value array */
    patval[i] = -1;			/* and clear the entries */
}

static
set1val(val1)
int val1;
{
  int i;
  for (i = 1; i < MAXVAL; i++)		/* clear all but first entry */
    patval[i] = -1;
  patval[0] = val1;			/* and set given value */
}

static
set2val(val1,val2)
int val1,val2;
{
  int i;
  for (i = 2; i < MAXVAL; i++)		/* clear all but first two entries */
    patval[i] = -1;
  patval[0] = val1;			/* and set given values */
  patval[1] = val2;
}




#ifdef TRACE

/* Routines to print out trace information during the pattern match */

static
trindent()				/* indent to current trace level */
{
  int i;
  for (i = 0; i < tracelev; i++)
    fprintf(stderr," ");
}

static
trshow()				/* show remaining unparsed input */
{
  int i;
  char c;
  for (i = 0; i < dtlen; i++) {
    c = dt[i];
    if ((c == '\177') || (c < SPACE))
      fprintf(stderr,"^%c",c^'\100');
    else
      fprintf(stderr,"%c",c);
  }
}

static
trargs()				/* print contents of patval array */
{
  int i;
  for (i = 0; i < MAXVAL; i++) {
    if (i != 0)
      fprintf(stderr," ");
    fprintf(stderr,"%d",patval[i]);
  }
}

static
trnl()					/* print a newline */
{
  fprintf(stderr,"\n");
}

#endif
