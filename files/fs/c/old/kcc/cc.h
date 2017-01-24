/*
** cc.h - declarations for KCC data structures
** Merged with cc.s / David Eppstein / 13 Mar 85
*/

#ifndef EXT
#define EXT extern
#endif

#include <stdio.h>
#include "ccerr.h"
#include "ccsym.h"

#define _FILSIZ 64			/* filename size */
#define	_STRSIZ	128			/* Size of input strings */
#define MAXNODE 10000			/* number of nodes      */
#define THASHSIZE 1021			/* size of type hash table */
#define TYPES 1021			/* number of types possible */
#define PRGBLEN 0200			/* size of PRARG blocks */

#define MAXMAC 10000			/* size of immense macros */
#define MAXMARG 7			/* max args (can't conflict with \n) */

/* target machine identifier size */

#define	INTSIZE     1
#define CHARSIZE    1
#define FLOATSIZE   1
#define DBLESIZE    2
#define VOIDSIZE    0
#define PTRSIZE	    1

#define NBYTES      4		/* number of bytes per word */

typedef char	file[_FILSIZ];
typedef	char	*string;

#define MAXPREDEF 20			/* how many -D predefs allowed  */
EXT int npredef;			/* how many there actually were */
EXT char *predefs[MAXPREDEF];		/* pointers to them all */

struct RW {				/* reserved words */
    string n;				/* name */
    int	t;				/* token */
    int k;				/* key */
};

struct TNAME {				/* built in types */
    string tnname;			/* name e.g. "int" */
    int tntype;				/* what type this is e.g. INT */
    int tnsize;				/* how big it is e.g. INTSIZE */
};

struct CONTEXT {	/* input file context */
	FILE	*cptr;      /*	file pointer  */
	file	cname;      /*	filename      */
	int	cline;      /*	line number   */
	int	cpage;	    /*  page number   */
};

struct MAC {		/* macro definition */
	char	*mptr;	    /*	body	    */
	char	*marg[10];  /*  arguments   */
};

struct TOKEN {		/* token definition   */
	int	ttype;	    /*  token type    */
        int	tprec;	    /*  op precedence */
};

struct  TSTACK {	/* token stack */
	int	        ttoken;
	struct SYMBOL	*tsym;
};

typedef struct NODE {			/* parse tree node */
    union {
	int nint;			/*   node operator */
	struct SYMBOL *nsym;		/*   label */
    } nvar0;
    struct TY *ntype;			/*   node type */
    int nflag;				/*   flags or integer value */
    union {
	struct NODE *nnode;		/*   subtree or chain pointer */
	struct SYMBOL *nsym;		/*   symbol pointer */
	int nint;			/*   integer value */
	char *nstr;			/*   string const pointer */
    } nvar1, nvar2, nvar3;		/* three of each of above */
} *node;

#define nop nvar0.nint
#define nsclab nvar0.nsym

#define left nvar1.nnode		/* left */
#define right nvar2.nnode		/* and right subtrees */

#define nid nvar1.nsym			/* symbol for identifier node */
#define nsize nvar2.nint		/* size for ++ etc */

#define nc nvar2.nint			/* coercion to apply to left node */

#define niconst nvar1.nint		/* integer constant */
#define nsconst nvar1.nstr		/* string constant pointer */
#define nsclen nvar2.nint		/* string constant length count */
#define nmantissa nvar1.nint		/* float constant mantissa */
#define nexponent nvar2.nint		/* float constant decimal exponent */

#define endlab nvar3.nsym		/* label to be generated after node */
#define nscnext nvar3.nnode		/* string const emission delay chain */

/* Constant */

EXT struct {				/* constant attribute */
    struct TY *ctype;			/* constant type */
    int cvalue;				/* integer value */
    union {
	char *csp;			/* string pointer */
	int cint;			/* or another integer */
    } cvar;
} constant;				/* place to hold current constant */

#define csptr cvar.csp			/* pointer for string const */
#define cmantissa cvalue		/* mantissa for float const */
#define cexponent cvar.cint		/* exponent for float const */

/* character types / tokens / parse tree opcodes */

#define	UNDEF	1
#define	SPACE	2
#define	NOT	3	/* token */
#define	DQUOTE	4
#define	POUND	5	/* token */
#define	MOD	6	/* token */
#define	ANDT	7	/* token */
#define	SQUOTE	8
#define	LPAREN	9	/* token */
#define	RPAREN	10	/* token */
#define	MPLY	11	/* token */
#define	PLUS	12	/* token */
#define	COMMA	13	/* token */
#define	MINUS	14	/* token */
#define	DOT	15	/* token */
#define	DIV	16	/* token */
#define	CONST	17	/* token */
#define	SCOLON	18	/* token */
#define	LESS	19	/* token */
#define	ASGN	20	/* token */
#define	GREAT	21	/* token */
#define	QUERY	22	/* token */
#define	IDENT	23	/* token */
#define	LBRACK	24	/* token */
#define	RBRACK	25	/* token */
#define	XORT	26	/* token */
#define	LBRACE	27	/* token */
#define	OR	28	/* token */
#define	RBRACE	29	/* token */
#define	COMPL	30	/* token */
#define	COLON	31	/* token */
#define DOLLAR	32

#define LABEL	33

#define STRUCT	34
#define UNION	35
#define ENUM	36
#define LONG	37
#define SHORT	38
#define UNSIGNED 39
#define	XTERN	40	/* temporary extern flag */
#define ENTRY	41

#define AUTO	  42
#define EXTERN	  43
#define REGISTER  44
#define TYPEDEF	  45
#define STATIC	  46
#define	GOTO	  47
#define	RETURN	  48
#define	SIZEOF	  49
#define	BREAK	  50
#define CONTINUE  51
#define	IF	  52
#define	ELSE	  53
#define	FOR	  54
#define DO	  55
#define WHILE	  56
#define	SWITCH	  57
#define	CASE	  58
#define DEFAULT	  59

#define	ASPLUS	  60
#define	ASMINUS	  61
#define	ASMPLY	  62
#define	ASDIV	  63
#define	ASMOD	  64
#define	ASRSH	  65
#define	ASLSH	  66
#define	ASAND	  67
#define	ASXOR	  68
#define	ASOR	  69

#define PTR	  70
#define ADDR	  71
#define NEG	  72
#define INC	  73	/* () ++ */
#define DEC	  74	/* () -- */
#define PINC	  75	/* ++ () */
#define PDEC	  76    /* -- () */
#define MEMBER	  77

#define	LAND	  78
#define	LOR	  79
#define	EQUAL	  80
#define LEQ	  81
#define GEQ	  82
#define	NEQ	  83
#define	STRING	  84
#define FUNCTION  85	/* function type */
#define FNDEF	  86	/* function definition */
#define	ARRAY	  87
#define FNCALL	  88	/* function call */

#define STATEMENT 89
#define	COMPND    90
#define EXPRESS   91

#define ICONST	  92		/* integer constant  A.2.4.1 */
#define LCONST	  93		/* long constant     A.2.4.2 */
#define DCONST	  94		/* floating constant A.2.4.4 */
#define SCONST	  95		/* string constant   A.2.4.5 */

#define	COERCE	  96

#define	LSHFT	  97
#define	RSHFT	  98

#define	IZ	  99
#define	IZLIST	 100
#define	BLOCK	 101
#define DATA	 102

/* built in data type codes */
/* can't conflict with ARRAY, PTR, STRUCT, UNION (sigh) */

enum { INT, CHAR, FLOAT, DOUBLE, VOID };

/* token type codes */

#define	BINOP	1
#define	UNOP	2
#define	TERNARY 3
#define	PRIMARY 4
#define	TYPE	5
#define	SC	6
#define	COMP	7
#define ASOP	8
#define BOOLOP  9
#define BOOLUN  10	/* boolean unary */
#define	NONE	16

/* reserved words key */

#define	TYPESPEC  100		/* type specifier 	   */
#define SCSPEC	  101		/* storage class specifier */
#define	COMPOUND  102		/* compound statement      */
#define	SUSPEC	  103		/* struct-union specifier  */

/* defnode flags */

#define	LVALUE  001	/* expr can be an lvalue */
#define RETEXPR 002	/* want result in RETVAL so can be returned */
#define GLOBAL	004	/* unable to cause a stackref */
#define STKREF	010	/* counted already as a stackref */
#define INPARENS 020	/* op has been parenthesized */
#define WASCOMP 040 	/* op was comparison */

/* defnode keys */

#define	N1	1	/* operator argument only */
#define N2	2	/* unary operator */
#define N3	3	/* binary operator */

/* coercions */

#define NOCOER  0	/* no actual coercion needed */
#define	PC_PC	1	/* char pointer to monotonic value */
#define PI_PC   2	/* integer pointer to char pointer */
#define PC_PI   3	/* char pointer to integer pointer */
#define INT_FL	4	/* integer to floating point */
#define INT_DF	5	/* integer to double precision */
#define FL_INT	6	/* floating point to integer */
#define FL_DF	7	/* floating point to double precision */
#define DF_INT	8	/* double precision to integer */
#define	DF_FL	9	/* double precision to floating point */

/* Assembler output types (values of asmtyp) */

#define AST_FAIL 0	/* SU-AI FAIL code */
#define AST_MACRO 1	/* DEC MACRO-10 code */
#define AST_MIDAS 2	/* MIT MIDAS code */

extern struct TOKEN tok[];		/* token attributes */
extern struct RW rw[];			/* reserve word list */
extern struct TNAME tnames[];		/* built in type list */

extern int map[];			/* ascii/ebcdic mapping	*/
extern char *errstr[];			/* error messages */

EXT struct SYMBOL *curfn;		/* name of current function */
EXT int curfnloc, curfnnew;		/* where in file it started */

EXT int stackrefs;			/* whether this fn addresses locals */

EXT file module, input, output, prefname, debfname, symfname;
EXT FILE *in, *out, *fdeb, *fsym;

EXT struct CONTEXT inc[10];		/* allow include depth of 10 	*/
EXT struct MAC mac[10];			/* allow macro nesting 10 deep  */

EXT struct NODE nodes[MAXNODE];

EXT node
    litstrings,				/* str consts saved for later emit */
    decls;				/* SAUTO initializations */

EXT struct SYMBOL
    *csymbol,				/* current symbol */
    *htable[MAXHSH],			/* hash table */
    *symbol,				/* symbol table head ptr */
    *symtail,				/* ptr to tail of symbol */
    *minsym,				/* minimum variable ptr */
    *locsymbol,				/* symbol table for auto variables */
    *loctail;				/* ptr to tail of locsymbol */

EXT struct TY
    *deftype,				/* integer type, default */
    *chartype,				/* character type */
    *strtype,				/* pointer to char, string type */
    *flttype,				/* floating point const type */
    *dbltype,				/* double precision floating point */
    *ttable[THASHSIZE],			/* hash table of types */
    types[TYPES];			/* table of type records */

EXT struct TSTACK tstack[16];		/* 16 deep token stack */

EXT int
    line, page,				/* position in input file */
    tline,				/* total # of lines */
    scope,				/* symbol scope */
    flushing,				/* inside conditional */
    level,				/* include level */
    maclevel,				/* macro level */
    breaklevel,				/* break level */
    contlevel,				/* continue level */
    caselevel,				/* switch stmt level */
    eflag,				/* error flag */
    eof,				/* end of file flag */
    maxsmem,				/* num struct members */
    maxtype,				/* maximum types used */
    maxnode,				/* maximum node used */
    maxauto,				/* size of auto vars */
    iflevel,				/* nesting of ifdefs */
    ifflag,				/* whether reading const expr to eol */
    inasm,				/* inside assembly passthrough */
    token,				/* current token */
    tokstack,				/* token stack level */
    time,				/* runtime save location */
    firstime;				/* first time through extdef() */

EXT node swdefault, swcases;		/* chained nodes for duplicate check */
EXT int swrange;			/* AND const for case range check */

EXT int
    verbose,				/* be noisy about functions compiled */
    optimize,				/* optimization option */
    dump,				/* dump symbols at end */
    assemble,				/* assemble after compile */
    delete,				/* delete assembler file */
    link,				/* run link after assembling */
    prepf,				/* preprocessor-only flag */
    debug,				/* debugging flag */
    nowarn,				/* don't type out warnings */
    keepcmts;				/* don't preprocess away comments */

EXT char
    _cpool[16000],			/* character pool */
    *cpool,				/* and its pointer */
    *macptr,				/* pointer to macro */
    ch,					/* lookahead character */
    ident[_STRSIZ],			/* global identifier */
    errlin[256],			/* input line for error context */
    *erptr;				/* and pointer to it */

extern char *sprefix, *spstfix;		/* where to find include files */
extern char *hdrfile;			/* name of preamble file */
extern char *usrincludes;		/* -I search directory */
extern char *asmfext[];			/* Assembler filename extensions */

extern int asmtyp;			/* assembler output type (AST_*) */
extern int iskl;			/* Set if KL - can use DMOVE etc */
