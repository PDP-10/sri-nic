/* TPPMSC.C - Misc Preprocessor tests
**	Compile with -E so only run through preprocessor.
**
** This file is a collection of oddball ad-hoc preprocessor checks.
** Most all of them are left commented out, because many are intended
** to generate error messages.  Just uncomment
** when a particular test is desired.
*/

	#	if 1
		"Should include this"
	#	if 0
	"Should NOT include this!"
#endif	/* To test nesting */
	"Should include this"
#pragma blunder blunder toil and trouble
#else
	"Should NOT include this!"
#pragma worse is yet to come...
#endif	/* OK, end of test */

#define foo() null arg list
foo	/* Should complain */
foo()	/* Should be OK */
foo(a)	/* Should complain */
(foo)(a) /* Should be OK */

#if 0	/* Test whether expansion rescan can trigger macro cmd */
#define INCL #include <nosuchfile.h>
INCL
#define EXP(a,b)  a :::: b
EXP(testing 1st arg value
#include <morebadfile.h>, (Second arg
goes here))
#endif

#if 0	/* Test CARM example from 3.3.6 p.39 */
	conc/* comment */test
#define INC CARM test
#define TAB is ok
#define INCTAB CARM_test_is_NOT_ok
#define CONC(x,y) x/**/y
CONC(INC,TAB)
#endif

#if 0	/* Test of #if without anything else */
#if
1
	This stuff should not appear.
#endif
#endif

#if 0		/* Check out #line */
#line 100
#barfo		/* Trigger error to see what KCC thinks context is */
#line 123 "foofile"
#barfo		/* Trigger error to see what KCC thinks context is */
#line
  456 "barfile"
*#barfo		/* Trigger error to see what KCC thinks context is */
#endif

#if 0 /* include-file error test */
#include this-is-a-garbage-name
#include <this is an unterminated name
#include "this is a very long filename xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx barf"
#include <this>is>a>multics>filename>	/* See what happens */
#include "this"is"a"filename"with"garbage"afterwards"/* comment */ garbage
#define FOOFILE "macro.h"
#include FOOFILE	/* Trying a macro */
#endif

#if 0	/* Macro expansion depth error test */
#define recurse(a) recurse(a)+1
recurse(foo);
#endif

#if 0 /* Macro redefinition error checks */
#define FOO 10
#define FOO 100	/* complain */
#define FOO() 100	/* complain */
#define FOO(a) 100	/* complain */
#define FOO(a) 100	/* don't complain */
#define defined This should complain.
#endif

/* elif testing */
#define SYS_ITS 0
#define SYS_T20 1
#if SYS_ITS
	int its;
#elif SYS_T20
	int t20;
#endif
#else
	ignore this;
#endif
#elif 1
	int usethis;
#endif

/* Test of "defined" */
#if SYS_T20 && defined(SYS_ITS)
	int yestoo;
#endif
#if SYS_T20 && defined FOO
	int nope;
#endif

/*
#include <stdio.h>
*/
main(argc,argv)
char *argv[];
{	register int i;
	register char *cp;

	for(i = 1; i < argc; ++i)
		printf("%s ", argv[i]);

}
