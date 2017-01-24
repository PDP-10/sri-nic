/*	CCTOKS.H - Token and Node-Op bindings for KCC
**
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/

/* This file contains one "tokdef" macro call for every token and node op
 * used by KCC.  The program which #includes this file is responsible for
 * defining "tokdef" in an appropriate way (see CCDATA.C and CCNODE.H).
 * The first entry should NOT correspond to any valid token or node op,
 * so that zero indices and values can be trapped as errors.
 * If no "tokdef" macro is defined, it is assumed that the file is being
 * included merely to get the various symbol definitions.
 */

/* The following naming conventions exist:
 *	T_	Token code.  Used only as a token, never as a node op.
 *	Q_	Token and Node-op.  Used as both.
 *	N_	Node-op code.  Used only as a node op, never as a token.
 *	X_	Code no longer used. Placeholder, should eventually be flushed.
 */
/*
**	A note on tokens: all tokens returned from the lexer (T_ and Q_ codes)
** are distinct, except for -, &, and *.  Q_MINUS, Q_ANDT, and Q_MPLY are
** returned for these.  The parser must determine whether these are actually
** N_NEG, N_ADDR, or N_PTR.
*/

#undef _TOKDEF	/* Assume includer hasn't defined tokdef, nothing special */
#ifndef tokdef
#define tokdef0(name,type,prec) name
#define tokdef(name,type,prec) ,name
#else
#define _TOKDEF	/* User is doing special stuff */
#endif


#ifndef _TOKDEF

/* Define structure of entries in the "tok" table, indexed by token
 * number.  Table is built in CCDATA.
 */
#define TOKEN struct _token
struct _token {		/* Entry indexed by token number (T_,Q_,N_) */
	int tktype;	/*  token type (set to a TKTY_ value) */
        int tkprec;	/*  operator precedence, if meaningful */
};

/* Token type codes (TOKEN.tktype)
** There are only two things set with these values:
**	(1) The tok[] array in CCDATA, with one entry for every possible token.
**	(2) The "Skey" component of a SC_RW (reserved word) symbol table entry.
** Currently the following type values are never referenced:
**	TKTY_NULL, TKTY_RWCOMP.
**	(that is, their exact value doesn't matter as long as it is not one
**	of the other types)
** Also, the .tkprec value is only used (by binary() in CCSTMT) if the type
**	is one of TKTY_BINOP or TKTY_BOOLOP.  The value for all other
**	types is ignored.  Precedence is a number from 0 (ignored) to
**	16 (highest precedence) as per H&S table 7-1.
*/

enum toktype {
	TKTY_NULL,	/* None of the following */
	TKTY_PRIMARY,	/* Primary expression */
	TKTY_UNOP,	/* Unary operator */
	TKTY_BOOLUN,	/* Unary boolean operator (only '!') */
	TKTY_BINOP,	/* Binary operator */
	TKTY_BOOLOP,	/* Binary boolean operator */
	TKTY_ASOP,	/* Binary assignment operator */
	TKTY_TERNARY,	/* Ternary (conditional) operator (only '?') */
	TKTY_SEQ,	/* Sequential evaluation operator (only ',') */
	TKTY_RWTYPE,	/* Reserved-Word: Type-specifier */
	TKTY_RWSC,	/* Reserved-Word: Storage Class */
	TKTY_RWCOMP	/* Reserved-Word: Compound statement */
};

enum tokcodes {
#endif

/* Simple tokens.  Most correspond to single characters. */
tokdef0(T_NULL,	TKTY_NULL,	0)	/* 0 is an invalid token/node-op */
tokdef(T_EOF,	TKTY_NULL,	0)	/* EOF reached */
tokdef(T_LPAREN,TKTY_NULL,	0)	/* ( */
tokdef(T_RPAREN,TKTY_NULL,	0)	/* ) */
tokdef(T_SCOLON,TKTY_NULL,	0)	/* ; */
tokdef(T_LBRACK,TKTY_NULL,	0)	/* [ */
tokdef(T_RBRACK,TKTY_NULL,	0)	/* ] */
tokdef(T_LBRACE,TKTY_NULL,	0)	/* { */
tokdef(T_RBRACE,TKTY_NULL,	0)	/* } */
tokdef(T_COLON,	TKTY_NULL,	0)	/* : */
tokdef(T_COMMA,	TKTY_NULL,	0)	/* , */
tokdef(T_LCONST,TKTY_NULL,	0)	/* A literal constant (any kind) */
tokdef(T_INC,	TKTY_NULL,	0)	/* ++ */
tokdef(T_DEC,	TKTY_NULL,	0)	/* -- */

/* Tokens/Node-Ops organized by operator type
** 	This listing follows [H&S table 7-1, sec 7.2.2]
*/
/* Primary expressions */
tokdef(Q_IDENT,	TKTY_PRIMARY,	16)	/* identifier */
tokdef(N_UNDEF,	TKTY_PRIMARY,	16)	/* Undef ident (error placeholder) */
tokdef(N_ICONST,TKTY_PRIMARY,	16)	/* Integral type constant */
tokdef(N_FCONST,TKTY_PRIMARY,	16)	/* Floating-point type constant */
tokdef(N_SCONST,TKTY_PRIMARY,	16)	/* String literal constant */
tokdef(N_PCONST,TKTY_PRIMARY,	16)	/* Pointer-type constant */
tokdef(N_VCONST,TKTY_PRIMARY,	16)	/* Void-type "constant" expr */
tokdef(N_ECONST,TKTY_PRIMARY,	16)	/* Enum-type constant */
tokdef(N_ACONST,TKTY_PRIMARY,	16)	/* Special - address constant??? */
/* no special op for subscript */	/* [k]	subscripting */
tokdef(N_FNCALL,TKTY_PRIMARY,	16)	/* f()	function call */
tokdef(Q_DOT,	TKTY_PRIMARY,	16)	/* .	direct selection */
tokdef(Q_MEMBER,TKTY_PRIMARY,	16)	/* ->	indirect selection */
tokdef(Q_ASM,	TKTY_PRIMARY,	16)	/* asm() "call" (KCC extension) */
tokdef(T_SYMVAL,TKTY_PRIMARY,	16)	/* _KCCsymval() "call" (KCC ext) */
tokdef(T_SYMFND,TKTY_PRIMARY,	16)	/* _KCCsymfnd() "call" (KCC ext) */

/* Unary operators */
tokdef(N_POSTINC,TKTY_UNOP,	15)	/* ()++	Postfix increment */
tokdef(N_POSTDEC,TKTY_UNOP,	15)	/* ()--	Postfix decrement */
tokdef(N_PREINC,TKTY_UNOP,	14)	/* ++()	Prefix increment */
tokdef(N_PREDEC,TKTY_UNOP,	14)	/* --()	Prefix decrement */
tokdef(T_SIZEOF,TKTY_UNOP,	14)	/* sizeof	Size */
tokdef(N_CAST,	TKTY_UNOP,	14)	/* (type)	Cast */
tokdef(Q_COMPL,	TKTY_UNOP,	14)	/* ~	Bitwise not */
tokdef(Q_NOT,	TKTY_BOOLUN,	14)	/* !	Logical not */
tokdef(N_NEG,	TKTY_UNOP,	14)	/* -()	Arithmetic negation */
tokdef(N_ADDR,	TKTY_UNOP,	14)	/* &()	Address of */
tokdef(N_PTR,	TKTY_UNOP,	14)	/* *()	Indirection */

/* Binary operators (and one ternary) */
tokdef(Q_MPLY,	TKTY_BINOP,	13)	/* *	(L) Multiply */
tokdef(Q_DIV,	TKTY_BINOP,	13)	/* /	(L) Divide */
tokdef(Q_MOD,	TKTY_BINOP,	13)	/* %	(L) Remainder */
tokdef(Q_PLUS,	TKTY_BINOP,	12)	/* +	(L) Add */
tokdef(Q_MINUS,	TKTY_BINOP,	12)	/* -	(L) Subtract */
tokdef(Q_LSHFT,	TKTY_BINOP,	11)	/* <<	(L) Left shift */
tokdef(Q_RSHFT,	TKTY_BINOP,	11)	/* >>	(L) Right shift */
tokdef(Q_LESS,	TKTY_BOOLOP,	10)	/* <	(L) Less than */
tokdef(Q_GREAT,	TKTY_BOOLOP,	10)	/* >	(L) Greater than */
tokdef(Q_LEQ,	TKTY_BOOLOP,	10)	/* <=	(L) Less or equal */
tokdef(Q_GEQ,	TKTY_BOOLOP,	10)	/* >=	(L) Greater or equal */
tokdef(Q_EQUAL,	TKTY_BOOLOP,	9)	/* ==	(L) Equal */
tokdef(Q_NEQ,	TKTY_BOOLOP,	9)	/* !=	(L) Unequal */
tokdef(Q_ANDT,	TKTY_BINOP,	8)	/* &	(L) Bitwise AND */
tokdef(Q_XORT,	TKTY_BINOP,	7)	/* ^	(L) Bitwise XOR */
tokdef(Q_OR,	TKTY_BINOP,	6)	/* |	(L) Bitwise OR */
tokdef(Q_LAND,	TKTY_BOOLOP,	5)	/* &&	(L) Logical AND */
tokdef(Q_LOR,	TKTY_BOOLOP,	4)	/* ||	(L) Logical OR */
tokdef(Q_QUERY,	TKTY_TERNARY,	3)	/* ?	(R) Conditional */
tokdef(Q_ASGN,	TKTY_ASOP,	2)	/* =	(R) Assignment */
tokdef(Q_ASPLUS,TKTY_ASOP,	2)	/* +=	(R)	*/
tokdef(Q_ASMINUS,TKTY_ASOP,	2)	/* -=	(R)	*/
tokdef(Q_ASMPLY,TKTY_ASOP,	2)	/* *=	(R)	*/
tokdef(Q_ASDIV,	TKTY_ASOP,	2)	/* /=	(R)	*/
tokdef(Q_ASMOD,	TKTY_ASOP,	2)	/* %=	(R)	*/
tokdef(Q_ASRSH,	TKTY_ASOP,	2)	/* >>=	(R)	*/
tokdef(Q_ASLSH,	TKTY_ASOP,	2)	/* <<=	(R)	*/
tokdef(Q_ASAND,	TKTY_ASOP,	2)	/* &=	(R)	*/
tokdef(Q_ASXOR,	TKTY_ASOP,	2)	/* ^=	(R)	*/
tokdef(Q_ASOR,	TKTY_ASOP,	2)	/* |=	(R)	*/
tokdef(N_EXPRLIST,TKTY_SEQ,    1)	/* ,	(L) sequential eval */

/* Reserved-Word: Type-specifier */
/* See the tnames table in CCDATA for other type-spec reserved words */
tokdef(T_VOID,		TKTY_RWTYPE, 0)	/* void */
tokdef(T_STRUCT,	TKTY_RWTYPE, 0)	/* struct */
tokdef(T_UNION,		TKTY_RWTYPE, 0)	/* union */
tokdef(T_ENUM,		TKTY_RWTYPE, 0)	/* enum */
tokdef(T_FLOAT,		TKTY_RWTYPE, 0)	/* float */
tokdef(T_DOUBLE,	TKTY_RWTYPE, 0)	/* double */
tokdef(T_CHAR,		TKTY_RWTYPE, 0)	/* char */
tokdef(T_SHORT,		TKTY_RWTYPE, 0)	/* short */
tokdef(T_INT,		TKTY_RWTYPE, 0)	/* int */
tokdef(T_LONG,		TKTY_RWTYPE, 0)	/* long */
tokdef(T_UNSIGNED,	TKTY_RWTYPE, 0)	/* unsigned */
tokdef(T_SIGNED,	TKTY_RWTYPE, 0)	/* signed   (ANSI addition) */
tokdef(T_CONST,		TKTY_RWTYPE, 0)	/* const    (ANSI addition) */
tokdef(T_VOLATILE,	TKTY_RWTYPE, 0)	/* volatile (ANSI addition) */
tokdef(T_CHAR6,		TKTY_RWTYPE, 0)	/* _KCCtype_char6 (KCC extension) */
tokdef(T_CHAR7,		TKTY_RWTYPE, 0)	/* _KCCtype_char7 (KCC extension) */
tokdef(T_CHAR8,		TKTY_RWTYPE, 0)	/* _KCCtype_char8 (KCC extension) */
tokdef(T_CHAR9,		TKTY_RWTYPE, 0)	/* _KCCtype_char9 (KCC extension) */
tokdef(T_CHAR18,	TKTY_RWTYPE, 0)	/* _KCCtype_char18 (KCC extension) */

/* Reserved-Word: Storage Class */
tokdef(T_AUTO,		TKTY_RWSC, 0)	/* auto */
tokdef(T_EXTERN,	TKTY_RWSC, 0)	/* extern */
tokdef(T_REGISTER,	TKTY_RWSC, 0)	/* register */
tokdef(T_TYPEDEF,	TKTY_RWSC, 0)	/* typedef */
tokdef(T_STATIC,	TKTY_RWSC, 0)	/* static */

/* Reserved-Word: Compound statement */
tokdef(Q_GOTO,		TKTY_RWCOMP, 0)	/* goto */
tokdef(Q_RETURN,	TKTY_RWCOMP, 0)	/* return */
tokdef(Q_BREAK,		TKTY_RWCOMP, 0)	/* break */
tokdef(Q_CONTINUE,	TKTY_RWCOMP, 0)	/* continue */
tokdef(Q_IF,		TKTY_RWCOMP, 0)	/* if */
tokdef(T_ELSE,		TKTY_RWCOMP, 0)	/* else */
tokdef(Q_FOR,		TKTY_RWCOMP, 0)	/* for */
tokdef(Q_DO,		TKTY_RWCOMP, 0)	/* do */
tokdef(Q_WHILE,		TKTY_RWCOMP, 0)	/* while */
tokdef(Q_SWITCH,	TKTY_RWCOMP, 0)	/* switch */
tokdef(Q_CASE,		TKTY_RWCOMP, 0)	/* case */
tokdef(Q_DEFAULT,	TKTY_RWCOMP, 0)	/* default */

/* Miscellaneous node ops */
tokdef(N_FUNCTION,	TKTY_NULL, 0)	/* Function definition */
tokdef(N_STATEMENT,	TKTY_NULL, 0)	/* Statement list */
tokdef(N_LABEL,		TKTY_NULL, 0)	/* Labelled statement */
tokdef(N_DATA,		TKTY_NULL, 0)	/* Data decl list, contains N_IZs */
tokdef(N_IZ,		TKTY_NULL, 0)	/* Identifier decl (w/optional init) */
tokdef(N_IZLIST,	TKTY_NULL, 0)	/* Initializer list (under N_IZ) */
tokdef(N_NODE,		TKTY_NULL, 0)	/* Random substructure node op */
tokdef(N_ERROR,		TKTY_NULL, 0)	/* Error placeholder (stmt or expr) */

#ifndef _TOKDEF
#undef tokdef
#undef tokdef0
};
#endif
