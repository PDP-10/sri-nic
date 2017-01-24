/* cc2.c   (C) 1981  K. Chen  */

#include <stdio.h>
#define  sc  extern
#include "cc.s"

/* ------------------------------------ */
/*	Compiler initializations	*/
/* ------------------------------------ */

#ifdef USASCII	/* US ASCII code */

int map [] = {
  EOF,    UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  UNDEF,  SPACE,  SPACE,  UNDEF,  SPACE,  SPACE,  UNDEF,  UNDEF,
  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  SPACE,  NOT,    DQUOTE, POUND,  UNDEF,  MOD,    AND,    SQUOTE,
  LPAREN, RPAREN, MPLY,   PLUS,   COMMA,  MINUS,  DOT,    DIV,
  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,
  CONST,  CONST,  COLON,  SCOLON, LESS,   ASGN,   GREAT,  QUERY,
  UNDEF,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  LBRACK, UNDEF,  RBRACK, XOR,    IDENT,
  UNDEF,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  LBRACE, OR,     RBRACE, COMPL,  UNDEF};

#endif
#ifdef SUASCII	/* Stanford ASCII */

int map [] = {
  EOF,    UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  UNDEF,  SPACE,  SPACE,  UNDEF,  SPACE,  SPACE,  UNDEF,  UNDEF,
  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  IDENT,  UNDEF,  COMPL,  UNDEF,  UNDEF,  UNDEF,  UNDEF,  UNDEF,
  SPACE,  NOT,    DQUOTE, POUND,  UNDEF,  MOD,    AND,    SQUOTE,
  LPAREN, RPAREN, MPLY,   PLUS,   COMMA,  MINUS,  DOT,    DIV,
  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,  CONST,
  CONST,  CONST,  COLON,  SCOLON, LESS,   ASGN,   GREAT,  QUERY,
  UNDEF,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  LBRACK, UNDEF,  RBRACK, XOR,    IDENT,
  UNDEF,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  IDENT,  
  IDENT,  IDENT,  IDENT,  LBRACE, OR,     UNDEF,  RBRACE, UNDEF};

#endif

/*	reserved words.  ref.[1], p. 180      */

struct RW rw[] = {
	"int",		INT,		TYPESPEC,
	"char",		CHAR,		TYPESPEC,
	"float",	FLOAT,		TYPESPEC,
	"double",	DOUBLE,		TYPESPEC,
	"struct",	STRUCT,		SUSPEC,
	"union",	UNION,		SUSPEC,
	"long",		LONG,		TYPESPEC,
	"short",	SHORT,		TYPESPEC,
	"unsigned",	UNSIGNED,	TYPESPEC,
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

char	*errstr[] = {
	0,
	"Identifier expected",
	"Constant expected",
	"Unsupported macro",
	"Problem with file",
	"Unexpected end of file",
	"Unclosed character constant",
	"Filename expected",
	"Symbol previously defined",
	"Identifier needed as formal argument",
	"End of formal argument list expected",
	"Number of arguments mismatches",
	"Undefined symbol",
	"Symbol is not a macro",
	"Expected token not found",
	"Type table overflow",
	"Syntax",
	"Identifier not in parameter list",
	"Identifier not a label",
	"Primary expected",
	"Name of a structure member expected",
	"Break not allowed here",
	"Pointer type needed as operand of *()",
	"Structure previously defined",
	"Union previously defined",
	"Member previously defined with different offset",
	"Initializer too large",
	"Lvalue required",
	"Array or pointer type required",
	"Int type needed",
	"structure name required on the left of the (.) operator",
	"pointer to a struct needed on the left of the (->) operator",
	"Illegal pointer arithmetic",
	"Unsupported type coercion",
	"Compiler control lines begin-end mismatch",
	"Continue not allowed here" };


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
