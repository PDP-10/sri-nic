/*	CCNODE.H - Declarations for Parse Tree Nodes
**
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
**	Split off from cc.h 13-Jan-86.
**	Most of the comments are by Ken Harrenstien, SRI International
*/

/* Parse tree node structure
 *	Input is parsed into a linked-list tree of these nodes, which is
 *	then given to the code generation routines.
 * NOTE: None of the "n_" members should ever be referenced directly;
 *	instead the "N" defines below should be used.  This allows
 *	distinguishing among all the various uses of a node's storage, and
 *	makes changes much easier.
 */
#define NODE struct _node

/* First, an auxiliary union definition for certain variables */
union n_v {		/* Variable can be one of: */
	NODE *n_node;		/*   subtree or chain pointer */
	SYMBOL *n_sym;		/*   symbol pointer */
	int n_int;		/*   integer value */
};

struct _node {
    int Nop;			/* Node opcode - a N_ or Q_ value */
    TYPE *Ntype;		/* C type of node */
    int Nflag;			/* Flags - NF_ bits */

    /* Now for random variables that can have different usages. */
    /* var 0 and var 1 are sometimes combined. */
    union {
      struct {
	union n_v n_v0;		/* Ordinary var 0 */
	union n_v n_v1;		/* Ordinary var 1 */
      } n_v01;
      long n_long;		/* or integer/pointer constant value */
      double n_double;		/* or double (float) constant value (2 wds) */
      struct {			/* or string constant (2 wds) */
	char *n_sptr;		/*	(pointer to string in char pool) */
	int n_slen;		/*	(length of string including null) */
      } n_vs;
    } n_vu;
    union n_v n_var2;		/* Var 2 */
    union n_v n_var3;		/* Var 3 - basically only used for Nendlab */
};
#define n_var0 n_vu.n_v01.n_v0	/* Shorthand to get at var 0 */
#define n_var1 n_vu.n_v01.n_v1	/* Shorthand to get at var 1 */

/* The most commonly used members.  Nleft, Nright, and Nendlab are the
** "normal" uses for var1/2/3.  Var0 is unused except for unusual nodes.
** Most node ops can use all of these; an effort has been made to
** document all exceptions on the rest of this page.
*/
#define Nleft   n_var1.n_node	/* Left subtree */
#define Nright  n_var2.n_node	/* Right subtree */
#define Nendlab n_var3.n_sym	/* Label to be generated after node */
	/* Note: Nendlab is never set in a parse-tree node returned
	** from the parser.  It is only used during the code generation
	** phase (CCGEN etc).
	*/

/* Alternate node uses */

/* N_FNCALL - Nretstruct
**	Nretstruct is only used for N_FNCALL nodes, to remember an
**	"auto" struct declaration which can be used to store the return
**	value of a function returning a structure larger than 2 words.
*/
#define Nretstruct n_var0.n_sym	/* Place to store returned structure */

/* Q_SWITCH - Nxswlist
**	Nxswlist is only used for Q_SWITCH nodes.  It is a pointer that
**	points to a list of Q_CASE nodes for this switch (chained together
**	through NRight).  If a Q_DEFAULT node exists it will be the first
**	thing on the list.
**	Set by CCSTMT's switchstmt() during parsing, and read by CCGSWI.
*/
#define Nxswlist n_var0.n_node	/* List of Q_CASE nodes */

/* Q_CASE - Nxfint, Nxfsym
** Q_GOTO, N_LABEL, Q_DEFAULT - Nxfsym
**	Nxfint is only used for Q_CASE nodes, to store the constant
**	value of the case statement.  Set by CCSTMT's casestmt()
**	during parsing, and read by CCGSWI's countcases().
** Nxfsym is used for Q_CASE, Q_DEFAULT, Q_GOTO, and N_LABEL nodes, by
**	CCGEN genstmt(), CCGEN1 gif(), CCGSWI countcases(), CCSTMT statement().
**	The tree as it comes from the parser only uses this for N_LABEL and
**	Q_GOTO; Q_CASE does not use it until code generation, after the
**	need for Nxfint is gone (since these are the same location).
*/
#define Nxfint n_var0.n_int	/* Case constant value in var0 */
#define Nxfsym n_var0.n_sym	/* Same loc now used as sym pointer */


/* Q_DOT, Q_MEMBER - Nxoff
**	Set by CCSTMT primary().
**	Read by CCGEN1 gident(), gaddress(), gassign().
*/
#define Nxoff n_var0.n_int	/* Offset for Q_DOT or Q_MEMBER */

/* N_CAST - Ncast
**	Contains a CAST_xxx value.
**	Set in CCSTMT primary(), CCTYPE typecheck(), leftconv(), rightconv().
**	Read by CCGEN2's gcast().
*/
#define Ncast n_var0.n_int	/* cast to apply to left node */

/* Nodes of type TKTY_ASOP - Nascast
**	Contains a CAST_xxx value which specifies the assignment conversion,
**	if any, to be applied to the resulting expression value.
**	Set in CCSTMT's expression(), read by CCGEN2's gassign().
**	Other vars are as for a normal expression operator.
*/
#define Nascast n_var0.n_int	/* Assignment conversion to apply */

/* Q_IDENT - Nid
**	Set by CCSTMT primary(), CCDECL funcdef(), izer().
**	Read by CCEVAL value(), lookup(), CCFOLD optexpr(),
**	CCGEN gendata() (without checking nop!), genbody() (ditto)
**	CCGEN1 gaddress(), gcall().
*/
#define Nid n_var0.n_sym	/* symbol for identifier node */

/* N_ICONST - Niconst
**	Refed by CCDECL pconst(), CCEVAL value(), CCFOLD i/f_expr_fold(),
**	CCGEN gizword(), gizarray(), gizchar(),
**	CCGEN1 gconst(), gif(), gdo(),
**	CCGEN2 gboolean(), gbinop(),
**	CCSTMT switchstmt(), casestmt(), primary(), unary()
*/
#define Niconst n_vu.n_long		/* integer constant */

/* N_FCONST - Nfconst
**	Set by CCSTMT's primary() and used by CCGEN1's gconst(),
**	CCGEN's gizword(), and CCFOLD's f_expr_fold() (which may also set it).
*/
#define Nfconst n_vu.n_double		/* floating-point constant */

/* N_SCONST - Nsconst, Nsclen, Nsclab, Nscnext
**	Nsconst, Nsclen - set by CCSTMT's primary() from lexer's parsed
**	constant.
**	Used by CCGEN's gizchar() to initialize a char array,
**	and CCGEN2's gend() to emit a string literal.
** Nsclab and Nscnext are only set during code generation by
**	CCGEN1's gconst() for N_SCONST nodes, and read by
**	CCGEN2's gend() at end of function.  Would be nice if there was
**	some way to flush this usage...
*/
#define Nsconst n_vu.n_vs.n_sptr	/* String constant pointer */
#define Nsclen  n_vu.n_vs.n_slen	/* String constant length */
#define Nsclab  n_var2.n_sym	/* String constant label */
#define Nscnext n_var3.n_node	/* String constant emission delay chain */

/* character types / tokens / parse tree opcodes (NODE.Nop) */

#include "cctoks.h"

/* Flag bits for NODE.Nflag */

#define	NF_LVALUE	001	/* expr can be an lvalue */
#define NF_RETEXPR	002	/* want result in RETVAL so can be returned */
#define NF_GLOBAL	004	/* unable to cause a stackref */
#define NF_STKREF	010	/* counted already as a stackref */
#define NF_INPARENS	020	/* op has been parenthesized */
#define NF_WASCOMP	040 	/* op was comparison */
#define NF_ARRFCONV	0100	/* This N_ADDR is an automatic conversion of
				** an array or function name.  Used only
				** so sizeof and fncalls can reverse it.
				*/
#define NF_DISCARD	0200	/* This expr's value will be discarded */
#define NF_USERCAST	0400	/* This N_CAST was explicitly given by user */
/* #define NF_SIDEFF	0 */	/* This expr has some side effect(s) */

/* defnode() key - 1st arg to defnode() must be one of the following,
 *	which specifies how many args are being given to defnode().
 */
#define	N1	1	/* op only: no other args */
#define N2	2	/* unary op:  type, flag, left-link (no right-link) */
#define N3	3	/* binary op: type, flag, left-link, right-link */

extern void initnodes();	/* Initialize node stuff */
extern NODE *defnode();		/* Get a node */
extern int nodeidx();		/* Get index for a node pointer (debugging) */
