#include <stdio.h>
#include "cc.s"

extern  struct  TOKEN  tok[];		/* token attributes     */
extern	struct  RW     rw[];		/* reserve word list	*/

extern	int	    map[];		/* ascii/ebcdic mapping	*/
extern	char	    *errstr[];		/* error messages	*/

sc	file	    module,		/* module name		*/
		    input,		/* input file name	*/
		    output,		/* output file name	*/
		    symfname;		/* symtable file name   */

sc      FILE	    *in,		/* file pointer to current input  */
		    *out,		/* file pointer to current output */
		    *fsym;		/* symbol table dump file         */

sc struct CONTEXT   inc[10];		/* allow include depth of 10 	*/
sc struct MAC	    mac[10];		/* allow macro nesting 10 deep  */
sc struct CONSTANT  constant;		/* place to hold current const  */

sc struct NODE	    nodes[MAXNODE];	/* intermediate code nodes */

sc struct SYMBOL    *csymbol,		/* current symbol */
		    *htable[MAXHSH],	/* hash table     */
		    symbol[MAXSYM],	/* symbol table   */
		    *isym[64];		/* initialzed locals */

sc struct TY	    *deftype,		/* default -int- type */
		    *chartype,		/* -char- type        */
		    *deflabel,		/* default label      */
		    *scalar,		/* scalar types	      */
		    *ttable[TYPES],	/* type hash table    */
		    types[TYPES];	/* type table	      */

sc struct TSTACK    tstack[16];		/* 16 deep token stack */

sc	int	    line,		/* current line number	*/
		    page,		/* current page */
		    tline,		/* total # of lines     */
		    scope,		/* symbol scope		*/
		    level,		/* include level	*/
		    maclevel,		/* macro level		*/
		    breaklevel,		/* break level		*/
		    contlevel,		/* continue level	*/
		    eflag,		/* error flag		*/
		    eof,		/* end of file flag	*/
		    minsym,		/* first global symbol	*/
		    maxsym,		/* maximum symbol used	*/
		    maxtype,		/* maximum types used	*/
		    maxnode,		/* maximum node used    */
		    maxlabel,		/* maximum label	*/
		    maxinit,		/* number of local initializers */
		    minloc,		/* minimum local symbol */
		    maxauto,		/* size of auto vars */
		    ncond,		/* conditional compilations */
		    nelse,		/* else part of conditional */
		    token,		/* current token */
		    tokstack,		/* token stack level */
		    time,		/* runtime save location */
		    optimize,		/* optimization option */
		    dump,		/* dump symbols at end */
		    assemble,		/* assemble after compile */
		    delete,		/* delete assembler file */
		    vsym[64],		/* local init sym location */
		    syntax;		/* syntax check only */

sc	char	    _cpool[8000],	/* character pool  	*/
		    *cpool,		/*   and its pointer	*/
		    *macptr,		/* pointer to macro	*/
		    ch,			/* lookahead character  */
		    errlin[256],	/* error line */
		    *erptr;
