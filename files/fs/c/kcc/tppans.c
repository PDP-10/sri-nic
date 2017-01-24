/* TPPANS.C - Test preprocessor using dpANS examples.
**	This file should be compiled with -E so it is only run through
**	the preprocessor.
*/

#define x	3
#define f(a)	f(x * (a))
#undef x
#define x	2
#define g	f
#define z	z[0]
#define h	g(~
#define m(a)	a(w)
#define w	0,1
#define t(a)	a

----------- START TEST OF EXPAND AND RESCAN ---------------

/* f(f(z)) => f(2 * (f(2 * (z[0])))) */
/* f(a)    => f(2 * (a)) */
/* f(f(z)) => f(2 * (f(z))) (straightforward scheme) */
/* f(f(z)) => f(f(2 * (z))) (ANSI: expand macro args separately first!) */

 f(y+1) + f(f(z)) % t(t(g)(0) + t)(1);
"f(2 * (y+1)) + f(2 * (f(2 * (z[0])))) % f(2 * (0)) + t(1);" /* Shd be this */

 g(x+(3,4)-w) | h 5) & m
	(f)^m(m);
"f(2 * (2+(3,4)-0,1)) | f(2 * (~ 5)) & f(2 * (0,1))^m(0,1);" /* Shd be this */

----------- START TEST OF STRINGIZE AND CONCAT ---------------

#define str(s)	# s
#define xstr(s)	str(s)
#define debug(s, t) printf("x" # s "= %d, x" # t "= %s", \
		x ## s, x ## t)
#define INCFILE(n)	vers ## n
#define glue(a, b)	a ## b
#define xglue(a, b)	glue(a, b)
#define HIGHLOW		"hello"
#define LOW		LOW ", world"

This_stuff debug(1, 2);
Should_be: printf("x" "1" "= %d, x" "2" "= %s", x1, x2);

This_stuff fputs(str(strncmp("abc\0d", "abc", '\4')	/* this goes away */
	== 0) str(: @\n), s);
Should_be: fputs("strncmp(\"abc\\0d\", \"abc\", '\\4') == 0" ": @\n", s);

#undef h	/* Flush def from previous test */
/*#include*/ xstr(INCFILE(2).h)
 "vers2.h"

This_stuff glue(HIGH, LOW);
Should_be: "hello";

This_stuff xglue(HIGH, LOW)
Should_be: "hello" ", world"

----------- END TEST OF STRINGIZE AND CONCAT ---------------

----------- START TEST OF REDEFS ---------------

------- Should NOT complain ---------
#define OBJ_LIKE	(1-1)
#define OBJ_LIKE	/* whitespace */ (1-1) /* more wsp */
#define FTN_LIKE(a)	( a )
#define FTN_LIKE( a )(	/* Note the whitespace */\
			a	/* More stuff commented out
			  */ )

---------- SHOULD complain! ------------
#define OBJ_LIKE	(0)	/* Diff tokens */
#define OBJ_LIKE	( 0 )	/* Diff whitespace */
#define FTN_LIKE(b)	( a )	/* Diff param usage */
#define FTN_LIKE(c)	( a )	/* Diff param spelling (BARF BLETCH) */

----------- END TEST OF REDEFS ---------------

------------------ TEST OF RESCAN AMBIGUITY ------------------

#undef f
#undef g
#define f(a) a*g
#define g    f

 f(2)(9)
"2*f(9)"

#undef f
#undef g
#define f(a)  a*g
#define g(a)  f(a)

 f(2)(9)
"2*f(9)" --or--
"2*9*g"

------------------ END OF TESTS ------------------------------
