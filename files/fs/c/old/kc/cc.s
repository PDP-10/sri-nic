/* ------------------------------------------- */
/*     C Compiler structures & define file     */
/* ------------------------------------------- */

#define	TOPS20

#ifdef 	TOPS20
#define	USASCII		/* US ascii character set */
sc char *libprefix = "C:";
sc char *libpstfix = "";
sc char *sprefix = "C:";
sc char *spstfix = "";
#endif

#ifdef	WAITS
define	SUASCII		/* SU-AI ascii character set */
define	USEIBP		/* use IBP instead of ADJBP */
sc char *libprefix = "";
sc char *libpstfix = "[cc,kc]";
sc char *sprefix = "";
sc char *spstfix = "[cc,kc]";
#endif

#ifdef	CCRMA
define	SUASCII		/* SU-AI ascii character set */
define	NOADJBP		/* hardware does not support ADJBP at all */
sc char *libprefix = "";
sc char *libpstfix = "[c,sys]";
sc char *sprefix = "";
sc char *spstfix = "[c,sys]";
#endif

#ifdef	XCCRMA		/* TOPS-20/CCRMA cross compile */
define	USASCII		/* SU-AI ascii character set */
define	NOADJBP		/* hardware does not support ADJBP at all */
sc char *sprefix = "C:";
sc char *spstfix = "";
sc char *libprefix = "";
sc char *libpstfix = "[c,sys]";
#endif

#ifdef 	FOONLY
#define	USASCII		/* US ascii character set */
#define	NOADJBP		/* hardware does not support ADJBP at all */
#define NOADJSP		/* Doesn't support ADJSP either, barf! */
sc char *sprefix = "<KC>";
sc char *spstfix = "";
sc char *libprefix = "<KC>";
sc char *libpstfix = "";
#endif

#ifdef 	XFOONLY		/* TOPS-20/FOONLY cross compile */
define	USASCII		/* US ascii character set */
define	NOADJBP		/* hardware does not support ADJBP at all */
#define NOADJSP		/* Doesn't support ADJSP either, barf! */
sc char *sprefix = "C:";
sc char *spstfix = "";
sc char *libprefix = "<KC>";
sc char *libpstfix = "";
#endif

#define	_IDSIZE	10	/* identifier size 	*/
#define _FILSIZ 16	/* filename size   	*/
#define	 MAXHSH	8192	/* hashtable size (2^N) */
#define	 MAXSYM	2048	/* symboltable size	*/
#define MAXNODE 4096    /* number of nodes      */
#define	 TYPES	512	/* typetable size (2^N)	*/

/* target machine identifier size */

#define	INTSIZE     1
#define CHARSIZE    1
#define LONGSIZE    1
#define SHORTSIZE   1
#define UNSIZE      1
#define FLOATSIZE   1
#define DBLESIZE    1
#define PTRSIZE	    1
#define NBYTES      4		/* number of bytes per word */

typedef	char	id[_IDSIZE];
typedef char	file[_FILSIZ];
typedef	char	*string;
typedef struct  NODE *node;

struct	RW {		/* reserved words */
	string	n;	    /*    name    */
	int	t;	    /*    token   */
	int	k;	    /*    key	  */
};

struct CONTEXT {	/* input file context */
	FILE	*cptr;      /*	file pointer  */
	file	cname;      /*	filename      */
	int	cline;      /*	line number   */
	int	cpage;	    /*  page number   */
};

struct MAC {		/* macro definition */
	id	mname;      /*	macro name  */
	char	*mptr;	    /*	body	    */
	char	*marg[10];  /*  arguments   */
};

struct TOKEN {		/* token definition   */
	int	ttype;	    /*  token type    */
        int	tprec;	    /*  op precedence */
};

struct TY {		/* type table entry   */
	int	ttype;
	int	tsize;
	struct TY *tptr;
};

struct  TSTACK {	/* token stack */
	int	        ttoken;
	struct SYMBOL	*tsym;
};

struct NODE {		/* syntax tree node   */
	int	nop;	     /* node operator */
	int     *nt;         /* node type     */
	int     nflag;	     /* node flags    */
	struct  NODE *left;
	struct  NODE *right;
};

struct NOD0 {
	int 	nop;
	struct  TY  *ntype;
};

struct NOD1 {		/* identifier node    */
	int	nop;	     /* node operator = IDENT */
	int     *nt;         /* ident type    */
	int	nflag;	     /* node flags    */
	struct  SYMBOL *nid;
	int     nsize;	     /* size of ++,-- */
};

struct NOD2 {		/* identifier node    */
	int	nop;	     /* node operator = ICONST */
	int     *nt;          /* constant type */
	int     nflag;	     /* node flags    */
	int	niconst;     /* int const     */
	int     n1fill;
};

struct NOD3 {		/* identifier node    */
	int	nop;	     /* node operator = SCONST */
	int     *nt;          /* constant type */
	int     nflag;	     /* node flags    */
	char	*nsconst;    /* string const  */
	int     n1fill;
};

struct NOD4 {		/* coerce type node   */
	int	nop;	     /* node operator */
	int     *nt;         /* node type     */
	int     nflag;	     /* node flags    */
	struct  NODE *left;
	int	nc;	     /* coerce type   */
};

struct SYMBOL {		/* symbol table entry */
	id	sname;	    /* symbol name    */
	int	sclass;	    /* storage class  */
	int	svalue;	    /* symbol value   */
	struct  TY  *sptr;  /* type pointer   */
};

struct SYM1 {		/* reserved word entry     */  /* must have same  */
	id	sname;	    /* reserved word name  */  /*  size as SYMBOL */
	int	sclass;	    /* storage class = SRW */
	int	stoken;	    /* reserved word token */
	int	skey;	    /* reserved word key   */
};

struct SYM2 {		/* macro table entry  */  /* must have same  */
	id	sname;	    /* symbol name    */  /*  size as SYMBOL */
	int	sclass;	    /* storage class  */
	int	svalue;	    /* symbol value   */
	char    *smptr;     /* macro pointer  */
};

struct CONSTANT {	/* constant attribute */
	struct TY *ctype;   /* constant type  */
	int	cvalue;	    /* integer value  */
	char	*csptr;	    /* string pointer */
	int	cfill[2];   /* filler         */
};



/* character types, also tokens, in most cases */

#define	UNDEF	1
#define	SPACE	2
#define	NOT	3	/* token */
#define	DQUOTE	4
#define	POUND	5	/* token */
#define	MOD	6	/* token */
#define	AND	7	/* token */
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
#define	XOR	26	/* token */
#define	LBRACE	27	/* token */
#define	OR	28	/* token */
#define	RBRACE	29	/* token */
#define	COMPL	30	/* token */
#define	COLON	31	/* token */

/* tokens */

#define	INT	  32	/* don't change order of the next 10 defines */
#define CHAR	  33
#define FLOAT	  34
#define DOUBLE	  35
#define STRUCT	  36
#define UNION	  37
#define LONG	  38
#define SHORT	  39
#define UNSIGNED  40
#define	LABEL	  UNION

#define AUTO	  41
#define EXTERN	  42
#define REGISTER  43
#define TYPEDEF	  44
#define STATIC	  45
#define	GOTO	  46
#define	RETURN	  47
#define	SIZEOF	  48
#define	BREAK	  49
#define CONTINUE  50
#define	IF	  51
#define	ELSE	  52
#define	FOR	  53
#define DO	  54
#define WHILE	  55
#define	SWITCH	  56
#define	CASE	  57
#define DEFAULT	  58
#define ENTRY	  59

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
#define	XTERN	 103	/* temporary extern flag */



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

/* storage classes */

#define	SUNDEF    0	/* undefined */
#define	SRW	  1	/* reserved word */
#define	SMACRO	  2	/* macro */
#define	STAG	  3	/* structure tag */
#define	STYPEDEF  4	/* typedef */
#define	SEXTERN   5	/* extern */
#define	SSTATIC	  6	/* static */
#define	SARG	  7	/* formal argument */
#define	SREGISTER 8	/* register */
#define SMEMBER   9	/* struct member */
#define SAUTO	  10	/* auto variables 10 to ... */

/* reserved words key */

#define	TYPESPEC  100		/* type specifier 	   */
#define SCSPEC	  101		/* storage class specifier */
#define	COMPOUND  102		/* compound statement      */
#define	SUSPEC	  103		/* struct-union specifier  */

/* defnode flags */

#define	LVALUE  1

/* defnode keys */

#define	N1	1	/* operator argument only */
#define N2	2	/* unary operator */
#define N3	3	/* binary operator */

/* error codes */

#define	EIDENT	1	/* identifier needed		*/
#define	ECONST	2	/* constant needed		*/
#define	EMACRO	3	/* unsupported macro, arg=name  */
#define	EFILE   4	/* problem with file, arg=name  */
#define	EEOF	5	/* Unexpected end-of-file	*/
#define	ECHAR	6	/* Unclosed character constant  */
#define EXFILE	7	/* Filename expected		*/
#define EDSYMB	8	/* Symbol already defined	*/
#define	EARG	9	/* Ident needed as argument	*/
#define EARGEND 10	/* End of formal argument needed*/
#define	ENARGS	11	/* Number of argument mismatch  */
#define EUNDEF	12	/* Undefined symbol		*/
#define	ENOTMAC	13	/* Symbol not a macro		*/
#define EXPECT  14	/* Expected token not found     */
#define	ETYPE	15	/* No space for type		*/
#define SYNTAX  16	/* syntax error			*/
#define ENOTARG 17	/* not in argument list		*/
#define	ELABEL	18	/* identifier not a label	*/
#define	EPRIM	19	/* primary expected		*/
#define EMEMBER 20	/* struct member expected       */
#define EBREAK	21	/* nothing to break out of	*/
#define EPTR    22	/* pointer type needed		*/
#define EDSTRUC 23	/* struct tag previously defined*/
#define EDUNION 24	/* union tag previously defined */
#define	EDMEM	25	/* member of different offset   */
#define ESIZE	26	/* initializer too large	*/
#define ELVALUE 27	/* lvalue required		*/
#define	EARRAY  28	/* not an array			*/
#define EINT	29	/* integer needed		*/
#define ESTRUCT 30	/* struct name required on LHS  */
#define EPSTRUCT 31	/* pointer to struct needed     */
#define EPARITH  32	/* bad pointer arithmetic	*/
#define ECOER	 33	/* unsupported coercion */
#define ECONTROL 34	/* control line error */
#define	ECONTINUE 35	/* nowwhere to continue */

/* coercions */

#define	PC_PC	1	/* char pointer to monotonic value */
#define NOCOER  2	/* no actual coercion needed */
#define PI_PC   3	/* integer pointer to char pointer */
#define PC_PI   4	/* char pointer to integer pointer */
#define	PI_PI	5	/* integer pointer to monotonic value */
