/*	CCDATA.C - KCC data allocations for vars and tables
**
**	All changes after version 76 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCDATA.C.89, 17-Dec-85 08:04:23, Edit by KLH */
/*  Rationalized names of constants and structures */
/* <KCC.CC>CCDATA.C.64, 17-Mar-85 08:26:04, Edit by KRONJ */
/*  Multiple defaults error shouldn't print a number */
/* <KCC.CC>CCDATA.C.54, 11-Mar-85 10:13:27, Edit by KRONJ */
/*  EMEMBER gets a string argument */
/* <KCC.CC>CCDATA.C.48,  7-Mar-85 17:58:31, Edit by KRONJ */
/*  Separate built in types from reserved words */
/*  Type hash table now can have different size from type table */
/* <KCC.CC>CCDATA.C.47,  7-Mar-85 16:49:39, Edit by KRONJ */
/*  Add void type; make vertical tab act like space */
/* <KCC.CC>CCDATA.C.46,  3-Mar-85 17:04:26, Edit by SATZ */
/*  Add illegal init. error message */
/* <KCC.CC>CCDATA.C.45, 28-Feb-85 14:06:22, Edit by SATZ */
/*  Remove include for stdio since cc.h includes it */
/* <KCC.CC>CCDATA.C.44, 24-Feb-85 02:02:00, Edit by SATZ */
/*  Add an out of memory error */
/* <KCC.CC>CCDATA.C.43, 18-Feb-85 13:22:34, Edit by SATZ */
/*  Create an error for bad BLOCK pseudo op */
/* <KCC.CC>CCDATA.C.42, 18-Feb-85 11:40:51, Edit by SATZ */
/*  Add error message for unmatched #endif */
/* <KCC.CC>CCDATA.C.41, 17-Feb-85 11:43:17, Edit by SATZ */
/*  Change "Problem with file" to "Can't open file" */
/* <KCC.CC>CCDATA.C.40,  2-Feb-85 01:17:08, Edit by SATZ */
/*  Structure errors output the member causing the error */
/* <KCC.CC>CCDATA.C.37,  3-Jan-85 21:47:03, Edit by KRONJ */
/*  ENOTARG now types the losing symbol name */
/* <KCC.CC>CCDATA.C.33,  1-Jan-85 18:25:38, Edit by KRONJ */
/*  Forward struct error is now struct size unavailable error */
/* <KCC.CC>CCDATA.C.32, 30-Dec-84 21:48:21, Edit by KRONJ */
/*  EINIT error for initializer mismatch */
/* <KCC.CC>CCDATA.C.31, 29-Dec-84 17:14:04, Edit by KRONJ */
/*  EULABEL error message */
/* <KCC.CC>CCDATA.C.29, 29-Dec-84 14:11:21, Edit by KRONJ */
/*  storage for vars in cc.h is here not in cc.c */
/* <KCC.ATBAT>CCDATA.C.3, 28-Nov-84 21:42:42, Edit by DAGONE */
/*  add error string that forward referenced struct need to be ptr */
/* <KCC.CC>CCDATA.C.27,  4-Sep-84 20:25:19, Edit by SATZ */
/*  add in error string for too many case statements */

/*
** ccdata - variables and constant storage for kcc
**          (C) 1981  K. Chen
*/

#include "ccsite.h"	/* Get our site-dependent stuff */

/* Define various externals here */
#define EXT
#include "cc.h"		/* Define various externals.  Note that this */
			/* also includes ccparm.h for our size parameters! */

			/* Allocate storage for tables in cc.h */
NODE nodes[MAXNODE];		/* Allocate parse tree node table */
SYMBOL *htable[MAXHSH];		/* Symbol hash table */
TYPE *ttable[THASHSIZE];	/* hash table of types */
TYPE types[TYPES];		/* table of type records */

#include "cclex.h"		/* More externals */

#define GEXT
#include "ccgen.h"

#define ERRSTRINGS		/* Make errstr[] table */
#include "ccerr.h"

#define CHARTABLE		/* Make _ctab[] table */
#include "ccchar.h"

	/* Make output string table of PDP-10 pseudo-ops, indexed by P_ vals */
char *popostr[] = {
#define opcode(iname,oname,f,a,b,c,d) oname,
#include "cccode.h"
#undef opcode
};
	/* Make flag table for PDP-10 pseudo-ops, indexed by P_ vals */
int popflg[] = {
#define opcode(iname,oname,f,a,b,c,d) f,
#include "cccode.h"
#undef opcode
};
	/* Make reg change table for PDP-10 pseudo-ops, indexed by P_ vals */
int popprc[] = {
#define opcode(iname,oname,f,prcf,b,c,d) prcf,
#include "cccode.h"
#undef opcode
};

/*
** Initial switch values, table allocations, and file locations.
*/

/* Version numbers (not switches -- cannot change at runtime).
**    cverdist is incremented whenever cvercode or cverlib is, or when
**	a new major KCC distribution is made.
**    cverkcc ideally should be updated every time a new .EXE is made and
**	should match the file version number.  This may have to be patched in
**	after compilation.
*/
int cvercode = 2;		/* Current C code version (for $$CVER) */
int cverlib = 2;		/* Current C library version (for $$CVER) */
int cverdist = 5;		/* Major KCC distrib version (info only) */
int cverkcc = 602;		/* KCC.EXE file version (rev #) (info only) */

int clevel = SWI_CLEV;		/* Default C implementation level */
int clevkcc = 1;		/* Always default to ask for KCC extensions */

int tgcpu[TGCPU_N] = { 		/* Target CPU types (default to source) */
	CPU_KA, CPU_KI,		/*	Set to values from c-env.h */
	CPU_KL,			/*      KS ok if any KL ok */
	CPU_KL,			/*	KL0 ok if ditto */
	CPU_KL };		/*	KLX ok if ditto */
int tgsys = 			/* Target System type (default to source) */
#if SYS_T20
	TGSYS_TOPS20;
#elif SYS_T10
	TGSYS_TOPS10;
#elif SYS_TENEX
	TGSYS_TENEX;
#elif SYS_WAITS
	TGSYS_WAITS;
#elif SYS_ITS
	TGSYS_ITS;
#else
	TGSYS_NULL;
#endif
int tgasm = SWI_TGASM;		/* Target Assembler type (site-dep default) */
int tgcsize = TGSIZ_CHAR;		/* Target Char size in bits */
int tgcpw = TGSIZ_WORD/TGSIZ_CHAR;	/* Target # Chars Per Word */
int tgcmask = (1<<TGSIZ_CHAR)-1;	/* Target Char Mask */

char *predefs[MAXPREDEF];	/* Pointers to -D macro predefs */
char *preundefs[MAXPREDEF];	/* Pointers to -U macro pre-undefs */
char *incdirs[MAXINCDIR];	/* Pointers to -I search paths (prefix) */
char *postincdirs[MAXINCDIR];	/* Pointers to -I search paths (postfix) */

char *sprefix = HF_PREFIX;		/* Where to find <> include files */
char *spstfix = HF_POSTFIX;		/* Other end of <> files (SAIL, T10) */
char *libprefix = LIB_PREFIX;		/* Where to find -l library files */
char *libpostfix = LIB_POSTFIX;		/* other end of library file spec */
char *asmhfile = NULL;			/* User-specified asm hdr file loc */

/* Loader psect default specifications */
#define PSDATA_BEG 01000
#define PSCODE_BEG 0400000

#if SYS_T10+SYS_WAITS
#define PSCODE_END 0700000	/* VMDDT starts at page 700 */
#elif SYS_T20+SYS_10X
#define	PSCODE_END 0765000	/* UDDT vars start at page 770-2 */
#elif SYS_ITS
#define	PSCODE_END 01000000	/* ITS has HACTRN, needs no crufty DDT */
#endif
struct psect ldpsdata  = { PSDATA_BEG, PSCODE_BEG-PSDATA_BEG, PSCODE_BEG-1 };
struct psect ldpscode  = { PSCODE_BEG, PSCODE_END-PSCODE_BEG, PSCODE_END-1 };
struct psect ldpsstack = {       0, 0, 0 };	/* For now */

/*
** Reserved words.  [K&R p.180], [H&S 2.6], ANSI
*/

RESWD rwtab[] = {
	/* Type Specifiers */
	"void",		T_VOID,
	"struct",	T_STRUCT,
	"union",	T_UNION,
	"enum",		T_ENUM,
	"float",	T_FLOAT,
	"double",	T_DOUBLE,
	"char",		T_CHAR,
	"short",	T_SHORT,
	"int",		T_INT,
	"long",		T_LONG,
	"unsigned",	T_UNSIGNED,
	"signed",	T_SIGNED  +	RWF_ANSI,	/* ANSI addition */
	"const",	T_CONST   +	RWF_ANSI,	/* ANSI addition */
	"volatile",	T_VOLATILE+	RWF_ANSI,	/* ANSI addition */
	"_KCCtype_char6",T_CHAR6  + RWF_KCC,	/* KCC extension */
	"_KCCtype_char7",T_CHAR7  + RWF_KCC,	/* KCC extension */
	"_KCCtype_char8",T_CHAR8  + RWF_KCC,	/* KCC extension */
	"_KCCtype_char9",T_CHAR9  + RWF_KCC,	/* KCC extension */
	"_KCCtype_char18",T_CHAR18+ RWF_KCC,	/* KCC extension */

	/* Storage Classes */
	"auto",		T_AUTO,
	"extern",	T_EXTERN,
	"register",	T_REGISTER,
	"typedef",	T_TYPEDEF,
	"static",	T_STATIC,

	/* Compound Statement keywords */
	"goto",		Q_GOTO,
	"return",	Q_RETURN,
	"break",	Q_BREAK,
	"continue",	Q_CONTINUE,
	"if",		Q_IF,
	"else",		T_ELSE,
	"for",		Q_FOR,
	"do",		Q_DO,
	"while",	Q_WHILE,
	"switch",	Q_SWITCH,
	"case",		Q_CASE,
	"default",	Q_DEFAULT,

	/* Oddball keywords */
	"sizeof",	T_SIZEOF,	/* unary operator */
	"asm",		Q_ASM +RWF_KCC,		/* asm() (KCC extension) */
	"_KCCsymval",	T_SYMVAL +RWF_KCC,	/* _KCCsymval() (KCC ext) */
	"_KCCsymfnd",	T_SYMFND +RWF_KCC,	/* _KCCsymfnd() (KCC ext) */

	NULL,		NULL };


/* Various tables for dealing with C data types */

/* Make _tfltab[] table for parser/generator.
**	Note that this is where the TF_BYTE flag gets added in.
*/
int _tfltab[] = {
#define typespec(ts,str,bsiz,fl) \
    fl | ((bsiz && bsiz < TGSIZ_WORD) ? TF_BYTE : 0),
	alltypemacro		/* Expand list of types from CCSYM.H */
#undef typespec
};

char *tsnames[] = {
#define typespec(ts,str,bsiz,fl) str,
	alltypemacro		/* Expand list of types from CCSYM.H */
#undef typespec
};

/* TYPSIZTAB - Type size tables.
**	This is somewhat machine-dependent and so may someday be moved
** to a CCTARG machine-specific file.  There is one entry for each basic type.
** Observe that (somewhat unfortunately) sizes here are expressed in
** terms of words, not bytes such as sizeof would return.
** The TYPBSIZTAB table is in terms of bits, not words or bytes.
**	These tables should only be referenced by CCSYM during initialization.
*/
int typsiztab[TS_MAX] = {
#define typespec(ts,str,bsiz,fl) ((bsiz+TGSIZ_WORD-1) / TGSIZ_WORD),
	alltypemacro		/* Expand list of types from CCSYM.H */
#undef typespec
};
int typbsiztab[TS_MAX] = {
#define typespec(ts,str,bsiz,fl) bsiz,
	alltypemacro		/* Expand list of types from CCSYM.H */
#undef typespec
};


/* Table of pointers to oft-used basic types such as int, char, etc.
**	Initialized by CCSYM.  Some "variables" such as inttype are
** actually macros which point to entries in this table.  Not all types
** have their entries initialized.
*/
TYPE *typeptr[TS_MAX];

/*
** Tokens.
**
** This table is referenced by CCDECL, CCFOLD, CCGEN, CCSTMT.
** It is indexed by the token/node-op value (T_,Q_,N_).
** See the included file for details on the values.
*/

TOKEN tok[] = {
#define tokdef0(name,type,prec) type,prec
#define tokdef(name,type,prec) ,type,prec
#include "cctoks.h"
#undef tokdef
#undef tokdef0
};

/* Define a table of token/node op names. */
char *nopname[] = {
#define tokdef0(a,b,c) "a"
#define tokdef(a,b,c) ,"a"
#include "cctoks.h"
#undef tokdef
#undef tokdef0
};
int nopmax = (sizeof nopname)/sizeof(char *) - 1; /* Highest valid code */
