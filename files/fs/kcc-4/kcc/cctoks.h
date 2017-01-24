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
tokdef(Q_ASM,	TKTY_PRIMARY,	16)	/* asm() "call" (KCC extension) */
tokdef(Q_DOT,	TKTY_PRIMARY,	16)	/* .	direct selection */
tokdef(Q_MEMBER,TKTY_PRIMARY,	16)	/* ->	indirect selection */

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
tokdef(N_NODE,		TKTY_NULL, 0)	/*   Random substructure node op */

#ifndef _TOKDEF
#undef tokdef
#undef tokdef0
};
#endif

#if 0		/* Gigantic documentation comment */

			PARSE TREE FORMAT

This is a description of the parse-tree format for each node op.  It is
kept here in order to help ensure that changes are reflected both in the
definitions and in the documentation.

<node-op name>
	<brief description>		<syntax>
	T: <C type pointer>
	F: <flags>
	X: <extra variable, if used.  Normally isn't.>
	L: <Nleft pointer>
	R: <Nright pointer>

NOTE: NULL is not allowed as a value for any pointer unless explicitly
mentioned as an option!

NOTE: Only a few types of nodes can ever be seen as "lvalues".  H&S 7.1
describes the kinds of expressions that can be lvalues.
	0. Names of scalar/struct/union type variables, but not functions,
	   arrays, or enum constants.
		Q_IDENT (sometimes)
	1. Subscript expression e[k], always.
		(Same thing as 5, Q_PTR).
	2. Parenthesized expression, if contents are lvalue.
		(Irrelevant)
	3. Direct component selection e.name, if e is lvalue.
		Q_DOT (sometimes).
	4. Indirect component selection e->name, always.
		Q_MEMBER (always).
	5. Indirection *e, always.
		Q_PTR (always).

------- Primary Expressions (type TKTY_PRIMARY) ---------

Q_IDENT: <identifier>				TERMINAL NODE
	T: <type>
	F: <flags?>
	X: (actually "Nid") pointer to symbol
	L,R: null

	Note: the node type (Ntype) will be identical to the symbol's type
	(Nid->Stype) in all cases EXCEPT when the symbol is an array or
	function, in which case the Ntype will be "pointer to <Stype>".
	This implements the automatic conversion of array/function names
	to array/function pointers during expression evaluation.  Routines
	that need the actual type (eg sizeof and data defs) can use the Stype.

	Note: This node can sometimes be an lvalue expression.

N_ICONST: <integer constant>			TERMINAL NODE
	T: <type> - normally "int" but may be any integral type.
	F: ?
	X: (actually "Niconst") - contains value of constant
	L,R: null

N_FCONST: <floating-point constant>		TERMINAL NODE
	T: <type> - either "double" or "float"
	F: ?
	X,L: (actually "Nfconst") - value of constant.
	R: null

N_SCONST: <string constant>			TERMINAL NODE
	T: <type> - always "char *"
	F: ?
	X,L: String constant value in "Nsconst" and "Nsclen".
	R: null
		Also uses "Nsclab" in code generation.

N_PCONST: <pointer constant>			TERMINAL NODE
	T: <type> - pointer to X
	F: ?
	X: (actually "Niconst") - contains value of constant
	L,R: null

	This node is used for casts of constant values to some pointer type.

N_VCONST: <void "constant">			TERMINAL NODE
	T: <type> - always "void"
	F: ?
	L,R: null

	This node is used for casts of constant values to "void".

Q_ASM:	asm(string-literal)			TERMINAL NODE
	T: <type "void">
	F: 0
	L: null
	R: null or <exprlist>	arguments.  Only N_SCONST allowed.

	Note: This op is considered to have side effects!
	Note: this is deliberately similar to N_FNCALL.  Currently the
	string literal argument is passed directly to the assembler output
	file.

N_FNCALL:
	<function-call>
	T: <type of function>
	F: <various flags>
	X: (actually Nretstruct) pointer to auto symbol for returning struct.
	L: <expr>		must evaluate to function address.
	R: null			if no args
		or <expr>	if one arg
		or <exprlist>	if multiple args

	Note: This op is considered to have side effects!
	Note: Nretstruct is only set if the function returns a structure 
	larger than 2 words.  It is a SYMBOL * pointer which identifies
	a temporary, unique internal auto struct declaration which the returned
	structure can be copied to, if the code generation wishes to do so.

Q_DOT:	<direct structure member selection>	<struct>.<member>
	T: <C type of member>
	F: <various flags>
	X: (actually Nxoff) (int) offset from start of struct (see note below)
	L: <expr>		Must evaluate to structure address
	R: null
	Note: This node can sometimes be an lvalue expression.

	Note: Nxoff is encoded just as for Ssmoff in structure member symbols.
	If positive, it is a word offset from start of struct.
	If negative, it is a bitfield.  After making it positive again,
		the low 12 bits are the P+S fields of a byte pointer, and
		the remaining high bits are the word offset.

Q_MEMBER: <indirect structure member selection>	<struct> -> <member>
	See Q_DOT; same structure.
	Note: This node is always an lvalue expression.

------- Unary Expressions (type TKTY_UNOP, TKTY_BOOLUN) ---------

T_SIZEOF:		/* sizeof	Size */
	This is only seen as an input token, not a node.  The
	result value of the sizeof is evaluated immediately and
	converted into a N_ICONST (integer constant) node.

N_CAST:	<cast-expression>		(<type>) <expr>
	T: <desired type of result>
	F: <various flags>
	X: (actually Ncast) holds CAST_ type to apply.
	L: <expression>
	R: null

N_POSTINC:		/* ()++	Postfix increment */
N_POSTDEC:		/* ()--	Postfix decrement */
N_PREINC:		/* ++()	Prefix increment */
N_PREDEC:		/* --()	Prefix decrement */
Q_COMPL:		/* ~	Bitwise not */
Q_NOT:			/* !	Logical not (the sole TKTY_BOOLUN op) */
N_NEG:			/* -()	Arithmetic negation */
N_ADDR:			/* &()	Address of */
N_PTR:			/* *()	Indirection */
	T: <type of expr>
	F: <flags>
	L: <expr>
	R: <null - unused>

	Note: The 4 inc/dec ops all require that their operand be an lvalue.
		They are also all considered side-effect ops!
	Note: N_ADDR requires that its operand be an lvalue.
	Note: N_PTR is always an lvalue expression.

------- Binary Expressions (type TKTY_BINOP, TKTY_BOOLOP) ---------
	All of these operators have the same basic structure.
All are left-associative during parsing.  It does
not matter which operand (left or right) is evaluated first, except for
|| and && which are required to evaluate the left operand first.

Q_MPLY: Q_DIV: Q_MOD:	13 * / %  Multiply, Divide, Remainder
Q_PLUS: Q_MINUS:	12 + -	 Add, Subtract
Q_LSHFT: Q_RSHFT:	11 << >>  Left shift, Right shift
Q_LESS: Q_GREAT: Q_LEQ: Q_GEQ:	10 < > <= >=  Comparisons
Q_EQUAL: Q_NEQ:		9 == !=  Equal, Unequal
Q_ANDT:			8 &	 Bitwise AND
Q_XORT:			7 ^	 Bitwise XOR
Q_OR:			6 |	 Bitwise OR
Q_LAND:			5 &&	 Logical AND
Q_LOR:			4 ||	 Logical OR
	T: <C type of result>
	F: <flags>
	L: <left operand expr>
	R: <right operand expr>

------- Binary Assignment Expressions (type TKTY_ASOP) ---------
	All operators have the same structure as the basic Q_ASGN
assignment op, and all require that the left operand be an lvalue.
They are also all considered side-effect ops.

Q_ASPLUS: Q_ASMINUS: Q_ASMPLY: Q_ASDIV: Q_ASMOD:
Q_ASRSH:  Q_ASLSH:   Q_ASAND:  Q_ASXOR: Q_ASOR:
Q_ASGN:	<assignment operation (any TKTY_ASOP type)>	<lvalue> = <expr>
	T: <resulting C type>
	F: <flags>
	X: (actually Nascast) Applicable assignment conversion (CAST_ value)
	L: <lvalue expression>
	R: <value expression>

Note: if a type conversion of the <lvalue expression> is required, L will
point to a N_CAST node which points to the <lvalue>.  However, that expression
is evaluated only once; the cast is applied only for the operation.  If another
conversion is needed before storing the result value back into the <lvalue>
then this is specified by the contents of Nascast; anything but CAST_NONE
indicates a conversion.

------- Ternary Expression (type TKTY_TERNARY) ---------

Q_QUERY:	TKTY_TERNARY
	<conditional-expression>	  <EC> ? <E1> : <E2>
	T: <type of E1>
	F: <flags>
	L: <expr>			EC (condition)
	R: <node>
		T: <type of E1>
		F: 0
		L: null or <exprlist>	E1 ("then")
		R: null or <expression>	E2 ("else")		

	Note: Normally the types of E1 and E2 are the same as the overall
type of the expression.  However, if the value of the ternary expression
is not used for anything (NF_DISCARD will be set) then it is possible for
the types to be mismatched or for either or both expressions to be NULL.

------- Comma Expression (type TKTY_SEQ) ---------

N_EXPRLIST:	<expression-list>  expr1, expr2, ... lastexpr
	T: <type of lastexpr>
	F: 0
	L: N_EXPRLIST
	  ...
		T: <type of expr2>
		F: 0
		L: N_EXPRLIST
			T: <type of expr1>
			F: 0
			L: null
			R: <expr1>
		R: <expr2>
	R: <lastexpr>


------- Compound statements (type TKTY_RWCOMP) ---------

Q_GOTO:	<goto-statement>
	T: null, F: 0
	X: (actually Nxfsym) (SYMBOL *) set to plabel(sym,0)
	L, R: null

Q_RETURN:	<return-statement>
	T: null or <type of current function> if a return-value exists.
	F: 0
	L: null
	R: null or <exprlist>	Return value (T also set if one exists)

Q_BREAK:	<break-statement>
	No T, F, L, R.

Q_CONTINUE:	<continue statement>
	No T, F, L, R.

Q_IF:	<conditional-statement> =  if ( <expr> ) <stmt> [else <stmt>]
	T: null, F: 0
	L: <exprlist>		Used as condition
	R: <node>
		T: null, F: 0
		L: <then-statement>
		R: <else-statement>	NULL if no "else"

Q_FOR:	<for-statement>		for ( <e1> ; <e2> ; <e3> ) <stmt>
	T: null, F: 0
	L: <node>
		T: null, F: 0
		L: <node>
			T: null, F: 0
			L: null or <exprlist>	E1 init
			R: null or <exprlist>	E2 condition
		R: <node>
			T: null, F: 0
			L: null or <exprlist>	E3 increment
			R: null
	R: <statement>		Body

Q_DO:	<do-statement>
	T: null, F: 0
	L: <exprlist>		Used as condition
	R: <statement>		Body

Q_WHILE: <while-statement>
	T: null, F: 0
	L: <exprlist>		Used as condition
	R: <statement>		Body

Q_SWITCH:	<switch-statement>
	T: null, F: 0
	L: <exprlist>		Condition
	R: <statement>		Body (usually a compound stmt)

Q_CASE:	<case-label> : <stmt>
	T: <deftype>		Always type integer for now
	F: 0
	X: (actually Nxfint or Nxfsym) <case value>
	L: <statement>		Statement labelled by this case-label
	R: <null, or ptr to previous Q_CASE node within this switch body>

Q_DEFAULT:	<default-label> : <stmt>
	T: <deftype>	set to type INT
	F: 0
	L: <statement>	Statement this is a label for
	R: null



------- Miscellaneous node ops (type TKTY_NULL) ---------

N_FUNCTION: <function-definition>
	T: null, F: 0
	L: <node>
		T: null, F: 0
		L: Q_IDENT for header (contains name, type)
		R: <local-scope static decls> or null if none.
			Note the decl list is made of all static decls within
			the entire function, regardless of actual scope.
	R: N_STATEMENT
		L: <statement>	Function body (will be compound stmt)
		R: N_STATEMENT
			T: null, F: 0
			L: Q_RETURN
			R: null

N_STATEMENT: <compound-statement>	{ decls stmt1 stmt2 ... last-stmt }
	T: null, F: 0
	L: <decls>		; If no decls, is stmt1.
	R: N_STATEMENT
		T: null, F: 0
		L: <stmt1>
		R: N_STATEMENT
		     ...
			T: null, F: 0
			L: <last-stmt>
			R: null

N_LABEL:	<named-label : statement>
	T: null, F: 0
	X: (actually Nxfsym) (SYMBOL *) to result of plabel(sym,1)
	L: <statement>
	R: null

N_DATA:	<data declaration or definition>	Any declaration not a funct def
	T: null, F: 0				e.g. int A,B=2,C;
	L: N_IZ for A
	R: N_DATA
		T: null, F: 0
		L: N_IZ for B
		R: N_DATA
			... until R: null

; Note that the structure for N_IZ is the same as that for Q_ASGN.
; This makes it easy to generate initialization code by replacing the
; N_IZ op with a Q_ASGN op when it is encountered.

N_IZ:	<storage alloc for variable>
	T: null, F: 0
	L: Q_IDENT
		T: <C type of data>
		F: 0
		X: (actually Nid) (SYMBOL *) for this identifier
		L,R: unused
	R: null or <expr> or N_IZLIST		Optional initializer

	Note: the Ntype of the Q_IDENT here is never examined when
	generating code for static-extent data (gendata()); the symbol's
	Stype is used instead.  See the note for Q_IDENT.

N_IZLIST:	<initializer-list>	= { e1, e2, ... elast }
	T: null, F: 0
	L: <expr> or N_IZLIST		E1
	R: N_IZLIST
		T: null, F: 0
		L: <expr> or N_IZLIST		E2
		R: N_IZLIST
			T: null, F: 0
			L: <lastexpr> or N_IZLIST	Elast
			R: null

N_NODE:	<node>
	This node op is used for random nodes that are treated
	specially anyway by whatever points to them.  Everything that
	used to set a node op to 0 now sets it to N_NODE; the only
	purpose of this value is to ensure that all active nodes have
	non-zero op values.
---------------------------------------------------------------------

Note about null statements:
	<null-statement> has no representation other than by a null pointer.
	If CCSTMT's statement() parses a null statement, a null pointer is
	returned.

Definition of <statement>:
	The node pointer returned from CCSTMT's statement() will be one of the
following:
	<null> i.e. a null statement
	node op has type TKTY_COMP - some structured statement.
	<exprlist> - any kind of expression.

Definition of <exprlist> and <expression>:
	These correspond respectively to what H&S calls
<comma-expression> and <no-comma-expression>.  There is very little
difference between them in terms of parse-tree nodes since a true
<exprlist> (i.e. a list of expressions separated by commas) can be
converted into an <expression> just by enclosing it in parentheses,
which results in exactly the same parse-tree node structure except
that the top node has its NF_INPARENS flag set.
	ANY of the nodes described as "expressions" may be found
as an <expression> or <exprlist>.  The only restriction is that
if an <expression> is required, then N_EXPRLIST cannot be seen
unless NF_INPARENS is set.  Only four nodes are guaranteed to be
terminating nodes: Q_IDENT and the three types of constants.
All other expression nodes MUST point to further non-null expression nodes.
This is different from the situation with <statement> nodes, which can
always be null to represent null statements.

	The NF_ flags as far as I know are only used with expression
nodes and are meaningless in any other type of node.  That is, they are
not used for TKTY_COMP or TKTY_NULL nodes.  The flags themselves
are for the most part only used within the expression parser itself
(CCSTMT).
	NF_LVALUE	/* expr can be an lvalue */
Mainly used in CCSTMT.  One reference in CCGEN1 to see if trying to
extract a structure component from a "virtual" non-addressable
structure, ie one returned on stack from a function.  A couple of refs
in CCTYPE when doing type conversion checking.

NF_RETEXPR	/* want result in RETVAL so can be returned */
	Only used in the code generator (CCGEN,1,2).  The parser never
uses this.

NF_GLOBAL	/* unable to cause a stackref */
	Mainly used in CCSTMT; one ref in CCTYPE.
This is used to help with NF_STKREF but not clear on this one yet.

NF_STKREF	/* counted already as a stackref */
	Only in CCSTMT.  Its usage is documented in CCSTMT.

NF_INPARENS	/* Expr was parenthesized */
	Only in CCSTMT.  This flag is set only to indicate
that an expression node was enclosed in parentheses, and thus may occur
for any type of expression node.  Its only use is for printing nice
warning (not error) messages about possible operator precedence
problems (ternary and binary).

NF_WASCOMP 	/* Operator is a comparison or logical &&, || */
	Only in CCSTMT's binary().  This flag is set only for
nodes of type TKTY_BOOLOP, and is only used in order to print a nice
warning message if it looks like the user has gotten the precedence of
the | and & operators mixed up with the comparison/logical operators.

NF_ARRFCONV	/* This N_ADDR is an implicit array/function conversion */
	Only in CCSTMT.  This flag is set only for N_ADDR nodes which
were created as part of an "immediate conversion" of an array or function
name parsed as a primary expression.  The flag is for the benefit of
the "sizeof" operator and function call (.)() processing, so that the
conversion can be reversed if the flag is seen.  Perhaps someday this
can be flushed when the code generation knows how to handle array/function
objects properly.

NF_DISCARD	This expression's value will be discarded.
	Set by CCFOLD's evaldiscard(), used by CCSTMT and CCGEN2.  As
per H&S 7.12, there are three places where an expression may have this
flag set: (1) an expression statement, (2) the first operand of a
comma expression, and (3) the initialization and incrementation
expressions in a "for" statement.  As much as possible of the expression
is recursively discarded, and the remaining expression will have this
flag set for the topmost node.  CCGEN2 checks this in various places to
optimize code generation.

NF_USERCAST	This N_CAST operation was explicitly given by user.
	A N_CAST node can be generated either by an explicit user cast
in the code, or by an implied type coercion (the "usual unary/binary/etc
conversions").  This flag is set by CCSTMT and distinguishes the two;
it is only used by CCFOLD's evaldiscard() to avoid generating warning
messages when discarding implicit cast conversions.

NF_SIDEFF	This expression has side effects.
	Not yet implemented; may or may not be a good idea.  Idea is
that any operator that has a side effect will set this flag, which
should be propagated upwards (that is, a node will have the flag set
if any of its operands have it set).

For any particular expression node, at any time:
	T: <the C type of the expression this node represents>
	F: <NF_ flags for this expression result>

#endif /* end of gigantic comment */
