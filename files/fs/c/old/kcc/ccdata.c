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
/*  Change "Problem with file" to "Can't open file"
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

#define EXT
#include "cc.h"

#define ERRSTRINGS
#include "ccerr.h"


/*
** File locations and such.
**
*/

char *sprefix = HF_PREFIX;		/* Where to find <> include files */
char *spstfix = HF_POSTFIX;		/* Other end of <> files (SAIL, T10) */
char *hdrfile = ASM_HDR_FILE;		/* where to get preamble from */
char *usrincludes = "";			/* -I search path */

/*
** Initial switch values
 */
int iskl = SWI_ISKL;		/* 1 if assembling for KL */
int asmtyp = SWI_ASMTYP;	/* Assembler output type */

/* Assembler stuff indexed by asmtyp */
char *asmfext[] = { ".fai", ".mac", ".mid" };

/*
** Character set.
**
** We compile differently for SAIL, which uses its own variant
** of ASCII with special control chars and braces in the wrong place.
*/

#ifndef WAITS

/* Normal ASCII */
int map [] = {
  EOF,    UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  UNDEF,  SPACE,  SPACE,  SPACE,  SPACE,  SPACE,  UNDEF,  UNDEF,
  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  SPACE,  NOT,    DQUOTE, POUND,  DOLLAR, MOD,    ANDT,   SQUOTE,
  LPAREN, RPAREN, MPLY,   PLUS,   COMMA,  MINUS,  DOT,    DIV,
  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,
  CONST,  CONST,  COLON,  SCOLON, LESS,   ASGN,   GREAT,  QUERY,
  UNDEF,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  LBRACK, UNDEF,  RBRACK, XORT,   IDENT,
  UNDEF,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  LBRACE, OR,     RBRACE, COMPL,  UNDEF};

#else

/* WAITS ASCII */
int map [] = {
  EOF,    UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  UNDEF,  SPACE,  SPACE,  UNDEF,  SPACE,  SPACE,  UNDEF,  UNDEF,
  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  IDENT,  UNDEF,  COMPL,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  SPACE,  NOT,    DQUOTE, POUND,  DOLLAR, MOD,    ANDT,   SQUOTE,
  LPAREN, RPAREN, MPLY,   PLUS,   COMMA,  MINUS,  DOT,    DIV,
  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,
  CONST,  CONST,  COLON,  SCOLON, LESS,   ASGN,   GREAT,  QUERY,
  UNDEF,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  LBRACK, UNDEF,  RBRACK, XORT,    IDENT,
  UNDEF,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  LBRACE, OR,     UNDEF,  RBRACE, UNDEF};

#endif

/*
** Reserved words.  ref.[1], p. 180
**
** Note that predefined types are kept separate from other reserved words,
** and in fact are not kept in the symbol table as reserved words.
*/

struct TNAME tnames[] = {
	"int",		INT,		INTSIZE,
	"char",		CHAR,		CHARSIZE,
	"float",	FLOAT,		FLOATSIZE,
	"double",	DOUBLE,		DBLESIZE,
	"void",		VOID,		VOIDSIZE,
	NULL,		NULL,		NULL
};

struct RW rw[] = {
	"long",		LONG,		TYPESPEC,
	"short",	SHORT,		TYPESPEC,
	"unsigned",	UNSIGNED,	TYPESPEC,
	"struct",	STRUCT,		SUSPEC,
	"union",	UNION,		SUSPEC,
	"enum",		ENUM,		SUSPEC,
	"auto",		AUTO,		SCSPEC,
	"extern",	EXTERN,		SCSPEC,
	"register",	REGISTER,	SCSPEC,
	"typedef",	TYPEDEF,	SCSPEC,
	"static",	STATIC,		SCSPEC,
	"goto",		GOTO,		COMPOUND,
	"return",	RETURN,		COMPOUND,
	"sizeof",	SIZEOF,		PRIMARY,
	"break",	BREAK,		COMPOUND,
	"continue",	CONTINUE,	COMPOUND,
	"if",		IF,		COMPOUND,
	"else",		ELSE,		COMPOUND,
	"for",		FOR,		COMPOUND,
	"do",		DO,		COMPOUND,
	"while",	WHILE,		COMPOUND,
	"switch",	SWITCH,		COMPOUND,
	"case",		CASE,		COMPOUND,
	"default",	DEFAULT,	COMPOUND,
	"entry",	ENTRY,		COMPOUND,
	NULL,		NULL,		NULL };

/*
** Tokens.
**
** One day these should be defined together with the token constants,
** in a file called cclex.h, similar to ccerr.h and cccode.h.
** For now the constant definitions are too entangled with ASCII,
** so that will have to wait.
*/

struct TOKEN tok[] = {
	NULL,	NONE,
	NULL,	NONE,
	NULL,	NONE,		/* space */
	BOOLUN,	15,		/* not */
	NULL,	NONE,		/* dquote */
	NULL,	NONE,		/* pound */
	BINOP,	13,		/* mod */
	BINOP,	8,		/* and */
	NULL,	NONE,		/* squote */
	NULL,	NONE,		/* lparen */
	NULL,	NONE,		/* rparen */
	BINOP,	13,		/* mply */
	BINOP,	12,		/* plus */
	NULL,	1,		/* comma */
	BINOP,	12,		/* minus */
	PRIMARY,15,		/* dot */
	BINOP,	13,		/* div */
	NULL,	15,		/* const */
	NULL,	NONE,		/* scolon */
	BOOLOP,	10,		/* less */
	ASOP,	2,		/* assign */
	BOOLOP,	10,		/* great */
	TERNARY,3,		/* query */
	PRIMARY,15,		/* ident */
	PRIMARY,15,		/* lbrack */
	NULL,	NONE,		/* rbrack */
	BINOP,	7,		/* xor */
	NULL,	NONE,		/* lbrace */
	BINOP,	6,		/* or */
	NULL,	NONE,		/* rbrace */
	UNOP,	15,		/* complement */
	NULL,	NONE,		/* colon */
	TYPE,	NONE,		/* int */
	TYPE,	NONE,		/* char */
	TYPE,	NONE,		/* float */
	TYPE,	NONE,		/* double */
	TYPE,	NONE,		/* struct */
	TYPE,	NONE,		/* union */
	TYPE,	NONE,		/* long */
	TYPE,	NONE,		/* short */
	TYPE,	NONE,		/* unsigned */
	SC,	NONE,		/* auto */
	SC,	NONE,		/* extern */
	SC,	NONE,		/* register */
	SC,	NONE,		/* typedef */
	SC,	NONE,		/* static */
	COMP,	NONE,		/* goto */
	COMP,	NONE,		/* return */
	NULL,	15,		/* sizeof */
	COMP,	NONE,		/* break */
	COMP,	NONE,		/* continue */
	COMP,	NONE,		/* if */
	COMP,	NONE,		/* else */
	COMP,	NONE,		/* for */
	COMP,	NONE,		/* do */
	COMP,	NONE,		/* while */
	COMP,	NONE,		/* switch */
	COMP,	NONE,		/* case */
	COMP,	NONE,		/* default */
	NULL,	NONE,		/* entry */
	ASOP,	2,		/* += */
	ASOP,	2,		/* -= */
	ASOP,	2,		/* *= */
	ASOP,	2,		/* /= */
	ASOP,	2,		/* %= */
	ASOP,	2,		/* >>= */
	ASOP,	2,		/* <<= */
	ASOP,	2,		/* &= */
	ASOP,	2,		/* ^= */
	ASOP,	2,		/* |= */
	UNOP,	15,		/* *() */
	UNOP,	15,		/* &() */
	UNOP,	15,		/* -() */
	UNOP,	15,		/* ++() */
	UNOP,	15,		/* --() */
	UNOP,	15,		/* ()++ */
	UNOP,	15,		/* ()-- */
	PRIMARY,15,		/* -> */
	BOOLOP,	5,		/* && */
	BOOLOP,	4,		/* || */
	BOOLOP,	9,		/* == */
	BOOLOP,	10,		/* <= */
	BOOLOP,	10,		/* >= */
	BOOLOP,	9,		/* != */
	NULL,	NONE,		/* string */
	NULL,	NONE,		/* function */
	NULL,	NONE,		/* function definition */
	NULL,	NONE,		/* array */
	NULL,	NONE,		/* label */
	NULL,	NONE,		/* statement */	
	NULL,	NONE,		/* compound statement */
	NULL,	NONE,		/* expression */
	NULL,	NONE,		/* integer constant */
	NULL,	NONE,		/* long constant */
	NULL,	NONE,		/* double constant */
	NULL,	NONE,		/* string constant */
	NULL,	NONE,		/* coercion */
	BINOP,	11,		/* << */
	BINOP,	11 };		/* >> */
