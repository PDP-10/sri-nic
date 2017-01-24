/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

/* Macros to calculate julian dates from normal dates.  0 = Jan 1,
** February assumed to have 29 days */

#define JAN(x)	((x)-1)
#define	FEB(x)	((x)+30)
#define	MAR(x)	((x)+59)
#define APR(x)	((x)+90)
#define	MAY(x)	((x)+120)
#define	JUN(x)	((x)+151)
#define	JUL(x)	((x)+181)
#define	AUG(x)	((x)+212)
#define	SEP(x)	((x)+243)
#define OCT(x)	((x)+273)
#define	NOV(x)	((x)+304)
#define	DEC(x)	((x)+334)


/* Time zone information... First, all the rules we know about -
** This was mostly taken from BSD 4.2 sources and TOP-20 V5 sources.
** The rules are not always accurate, and are frequently inaccurate in
** isolated areas (e.g. states which have exempted themselves from
** daylight savings time).  Modify these tables as required.
*/

static dstrule usarule[] = {		/* USA rules (by 1967 legislation) */
  { 1974, 1974, JAN(6), NOV(30) },	/* 74: Jan 6 -> end Nov */
  { 1975, 1975, FEB(28), OCT(31) },	/* 75: end Feb -> end Oct */
  { 0,    3000,	APR(30), OCT(31) }	/* others: end Apr -> end Oct */
};

static dstrule ausrule[] = {		/* Australia (reverse table) */
  { 1970, 1970, JAN(1), DEC(50) },	/* 70: no dst at all */
  { 1971, 1971, JAN(1), OCT(31) },	/* 71: end Oct -> */
  { 1972, 1972, FEB(29), OCT(31) },	/* 72: -> end Feb, end Oct -> */
  { 0,	  3000, MAR(7), OCT(31) }	/* others: -> beg Mar, end Oct -> */
};

/* European tables, believed correct for:
**   Western: Great Britain, Ireland, Portugal
**   Middle:  Belgium, Luxembourg, Netherlands, Denmark, Norway, Austria,
**	      Poland, Czechoslovakia, Sweden, Switzerland, DDR, DBR, France,
**	      Spain, Hungary, Italy, Jugoslavia
**   Eastern: Bulgaria, Finland, Greece, Rumania, Turkey, Western Russia
** Eastern Europe gets Middle European dst rules, since our source (BSD 4.2)
** claimed ignorance and followed this strategy.
**/

static dstrule werule[] = {
  { 1983, 3000, MAR(31), OCT(30) },	/* After 83: end Mar -> end Oct */
  { 0,    1982, DEC(50), DEC(50) }	/* Before 83: No dst */
};

static dstrule merule[] = {
  { 1983, 3000, MAR(31), SEP(30) },	/* After 83: end Mar -> end Sep */
  { 0,    1982, DEC(50), DEC(50) }	/* Before 83: No dst */
};


/* Timezone information table */

static tzinf tzinfo[] = {
  { TZ_GMT,  "GMT", "GMT",       0*60, werule,  60, FALSE },
  { TZ_UAST, "AST", "ADT",       4*60, usarule, 60, FALSE },
  { TZ_UEST, "EST", "EDT",       5*60, usarule, 60, FALSE },
  { TZ_UCST, "CST", "CDT",       6*60, usarule, 60, FALSE },
  { TZ_UMST, "MST", "MDT",       7*60, usarule, 60, FALSE },
  { TZ_UPST, "PST", "PDT",       8*60, usarule, 60, FALSE },
  { TZ_UYST, "YST", "YDT",       9*60, usarule, 60, FALSE },
  { TZ_UHST, "HST", "HDT",      10*60, usarule, 60, FALSE },
  { TZ_UBST, "BST", "BDT",      11*60, usarule, 60, FALSE },
  { TZ_EWET, "WET", "WET DST",   0*60, werule,  60, FALSE },
  { TZ_EMET, "MET", "MET DST",  -1*60, merule,  60, FALSE },
  { TZ_EEET, "EET", "EET DST",  -2*60, merule,  60, FALSE },
  { TZ_AEST, "EST", "EST",     -10*60, ausrule, 60, TRUE },
  { TZ_ACST, "CST", "CST", -(9*60+30), ausrule, 60, TRUE },
  { TZ_AWST, "WST", "WST",      -8*60, ausrule, 60, TRUE }
};

/* macro giving number of timezones represented in the table */
#define TZCOUNT	(sizeof(tzinfo) / sizeof(tzinf))

