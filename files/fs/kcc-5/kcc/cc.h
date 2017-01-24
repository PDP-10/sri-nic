/*	CC.H - Declarations for KCC data structures
**
**	All changes after version 84 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/*
** Merged with cc.s / David Eppstein / 13 Mar 85
*/
/* Note: EXT must be used to declare anything which is not explicitly declared
 * in CCDATA.  All others are "extern".
 */

#ifndef EXT
#define EXT extern
#endif

#include <stdio.h>
#include "ccparm.h"	/* First come the various KCC parameters */
#include "ccerr.h"	/* Error code definitions */
#include "ccsym.h"	/* SYMBOL and TYPE definitions */
#include "ccnode.h"	/* NODE definitions */

typedef char filename[_FILSIZ];

/* KCC switch flags.  These are set when KCC is first invoked and remain
**	constant over all files processed.
*/

EXT int
				/* -O or -O=all	All optimizer flags */
    optpar,			/* -O=parse	Parse tree optimizations */
    optgen,			/* -O=gen	Code generator " */
    optobj,			/* -O=object	Obj code (peephole) " */
				/* -n = 0 Don't optimize anything */
				/* -d or -d=all All debug flags */
    debpar,			/* -d=parse	Parse tree debug output */
    debgen,			/* -d=gen	Code generator "   "    */
    debpho,			/* -d=pho	Peephole optim "   "    */
    debsym,			/* -d=sym	Symbol table   "   "    */
				/* -v or -v=all	  All verboseness flags */
    vrbfun,			/* -v=fundef	Print function names */
    vrbsta,			/* -v=stats	Print stats at end */
    vrbld;			/* -v=load	Print linking loader cmds */
EXT int
    ldextf,			/* -i or -i=extend */
    ldpsectf;			/* -i=psect */
extern struct psect {		/*	Structure for psect specifications */
    int ps_beg, ps_len, ps_lim;	/*	Psect start, max length, max addr */
}	ldpsdata,		/* -i=data:<beg>:<len>	Data segment */
	ldpscode,		/* -i=code:<beg>:<len>	Code segment */
	ldpsstack;		/* -i=stack:<beg>:<len>	Stack segment */

EXT int
    assemble,			/*      1 Assemble after compile */
				/*	  (set/cleared by other switches) */
    delete,			/*      1 Delete assembler file when done */
				/* -S = 0 Don't (don't run anything either) */
    link,			/*      1 run link after assembling */
				/* -c = 0 compile only, don't run link */
    prepf,			/* -E = 1 run through preprocessor only */
    nowarn,			/* -w = 1 Don't type out warnings */
    condccf,			/* -q = 1 Compile files conditionally */
    keepcmts;			/* -C = 1 don't preprocess away comments */
EXT int npredef;		/* -D:    # of -D macro predefinitions */
extern char *predefs[];		/* -D:    pointers to them all */
EXT int npreundef;		/* -U:    # of -U macro pre-undefinitions */
extern char *preundefs[];	/* -U:    pointers to them all */
EXT int nincdirs;		/* -I:    # of -I include-file directories */
extern char *incdirs[],		/* -Ipath  pointers to them all (prefix) */
	*postincdirs[];		/* 	   (and postfix) */
extern char *asmhfile;		/* -Afile = name of assembler preamble file */
extern char *sprefix, *spstfix;	/* -Hpath  for < > type include files */
extern char *libprefix,		/* -Lpath  for -l library files */
	*libpostfix;

extern int clevkcc;		/* -P=KCC Asks for KCC extensions */
extern int clevel;		/* -P=    Specifies C implem level, one of: */
#define CLEV_BASE 0		/* Base (default) should always be 0 */
#define CLEV_CARM 1
#define CLEV_ANSI 2
#define CLEV_STDC 3

extern int cvercode, cverlib;	/* $$CVER version numbers (no switches) */
extern int cverdist, cverkcc;	/* Info-only version numbers (no switches) */

extern int tgsys;		/* Target System type */
				/*      0 (default) same as source system */
				/*	n different, some TGSYS_ value */
extern int tgcpu[];		/* Target CPU type array, indexed by TGCPU_ */
				/*	0 means don't code for this CPU */
				/*	1 means make code work for this CPU */
				/*	2 means same as 1, user-specified */
extern int tgasm;		/* Target Assembler type */
				/*      default is site dependent */
				/* -m = Use MACRO assembler */
extern int tgcsize;		/* Target Char Size, in bits */
extern int tgcpw;		/* Target # Chars Per Word */
extern int tgcmask;		/* Target Char Mask */

EXT struct {			/* Target CPU/SYS use-feature flags */
	int dmovx;		/*	Has DMOVx instructions */
	int adjsp;		/*	Has ADJSP instruction */
	int adjbp;		/*	Has ADJBP instruction */
	int fixflt;		/*	Has FIX, FIXR, FLTR instrs */
	int dfl_s;		/*	Use Software double format */
	int dfl_h;		/*	Use Hardware double format */
	int dfl_g;		/*	Use "G" double format */
	int exadr;		/*	Has extended addressing */
	int mapch;		/*	Sys: Must map char set */
	int mapdbl;		/*	Mach: Must map double format */
} tgmachuse;


	/* Constant tables - not changed at any time */
extern TOKEN tok[];		/* Token/Node-Op attributes */
extern char *nopname[];		/* Token/Node-Op names */
extern int nopmax;		/* Highest valid token/node-op index */
extern char *errstr[];		/* Array of error messages */

	/* Pointers to basic data types supported */
#define deftype	inttype		/* Default type - set to (int) */
#define siztype inttype		/* Type for "sizeof" operator */
#define voidtype	typeptr[TS_VOID]	/* (void)	*/
#define flttype		typeptr[TS_FLOAT]	/* (float)	*/
#define dbltype		typeptr[TS_DOUBLE]	/* (double)	*/
#define lngdbltype	typeptr[TS_LNGDBL]	/* (long double)	*/
#define schartype	typeptr[TS_CHAR]	/* (signed char)	*/
#define shrttype	typeptr[TS_SHORT]	/* (short)	*/
#define inttype		typeptr[TS_INT]		/* (int)	*/
#define longtype	typeptr[TS_LONG]	/* (long)	*/
#define uinttype	typeptr[TS_UINT]	/* (unsigned int) */
#define ulongtype	typeptr[TS_ULONG]	/* (unsigned long) */
#define ushrttype	typeptr[TS_USHORT]	/* (unsigned short) */
#define uchartype	typeptr[TS_UCHAR]	/* (unsigned char) */
EXT TYPE *chartype;		/* (char) - set to schartype or uchartype */
EXT TYPE *strcontype;		/* (char *) - type for string constants */
extern TYPE *typeptr[];		/* Type pointers */
extern int typsiztab[];		/* Type sizes in words */
extern int typbsiztab[];	/* Type sizes in bits */
extern char *tsnames[];		/* Type names */


/* Per-file variables.  These are reset for each file being processed. */
EXT filename module,	/* Basic input file name (without extension) */
	inpfname,	/* Current input file, normally:	module.C */
	outfname,	/* Assembler main output file:	module.FAI/MAC/MID */
	prefname,	/* Assembler "entry" prefix file:	module.PRE */
	debfname,	/* Debugging output file (parse tree):	module.DEB */
	phofname,	/* Debugging output file (peephole):	module.PHO */
	symfname;	/* Debugging output file (symtab):	module.SYM */
EXT FILE *in, *out, *fdeb, *fpho, *fsym;	/* STDIO file I/O pointers */
EXT int maxtype;	/* maximum types used (CCSYM, CCDUMP) */

/* Variables used during input file processing */
EXT int page,		/* position in input file */
    line,		/* ditto - line # on current page */
    fline,		/* ditto - line # in current file */
    tline,		/* total # of lines (including includes) */
    eof,		/* end of file flag */
    token;		/* current input token from lexer */
EXT int nerrors;	/* # errors in current file being compiled */


/* Per-declaration variables.  These are reset for each top-level
** declaration or function.
*/
extern NODE nodes[];	/* Parse tree node table (CCSTMT, CCDATA) */
EXT int savelits;	/* 0 when OK to reset string literal pool */
EXT SYMBOL *curfn;	/* Name of current function */
EXT int curfnloc, curfnnew;	/* where in file it started (CCERR, CCDECL) */
EXT int maxauto;	/* Current fn: size of auto vars (CCGEN, CCDECL) */
EXT int stackrefs;	/* Current fn: Whether it addresses locals */
EXT int stkgoto;	/* Current fn: Whether it contains a non-local goto */

