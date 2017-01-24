/*	CCLEX.H - Declarations for KCC input parsing (CCLEX and CCINP)
**
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
**	Split from cc.h and other places, 15 Dec 85
*/

#ifndef EXT		/* Actual definition happens in CCDATA */
#define EXT extern
#endif

EXT int ch;		/* Current character */
EXT int inifarg;	/* TRUE if parsing an #if/#elif argument */
EXT int level;		/* #include file level (CCERR, CCINP, CCLEX) */

EXT char errlin[ERRLSIZE];	/* error context - circular buffer */
EXT char *erptr;		/* pointer into it */
EXT int erpleft;		/* # chars left until wraparound */
EXT int ercsiz;			/* # chars to show in error msg */

EXT char _cpool[CPOOLSIZE];	/* character pool */
EXT char *cpool;		/* and its pointer */
EXT int cpleft;			/* Countdown of # free chars left */

/* Constant value if current token == CONST.
 *	Set by CCLEX as part of token parsing.
 *	Copied by CCSTMT into parse tree node values, and glanced at
 *	by CCINP macro processing.  Nothing else uses this.
 */

EXT struct {
    TYPE *ctype;		/* constant type */
    union {
	int cint;		/* integer value */
	float cflt;		/* float value */
	double cdbl;		/* double value */
/*	long double cldbl;	*/
	struct {		/* string constant */
	    char *csp;		/*   char pointer */
	    int csint;		/*   string length */
	} cstr;
    } cvar;
} constant;			/* place to hold current constant */

/* The value of ctype is set to only one of the following, with
 * the associated definitions used.  The cvar/cstr unions are never to be
 * used directly.
 */
/* ctype == deftype	(INT) integer constant */
#define cvalue cvar.cint		/* integer value */

/* ctype == strtype	(CHAR *) string constant */
#define csptr cvar.cstr.csp		/* Pointer for string const */
#define cslen cvar.cstr.csint		/* String length (includes null) */

/* ctype == dbltype	(DOUBLE) double (float) constant */
/*	or flttype */
/*	or lngdbltype */
#define Cfloat cvar.cflt	/* floating-point value */
#define Cdouble cvar.cdbl
#define Clngdbl cvar.cdbl	/* Not used yet for anything different. */
