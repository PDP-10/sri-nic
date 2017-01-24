/* Test preprocessor stuff using dpANS examples */

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

 f(f(z))	/* Should become */ "f(2 * (f(2 * (z[0]))))"
/* f(a)    => f(2 * (a)) */
/* f(f(z)) => f(2 * (f(z))) (straightforward scheme) */
/* f(f(z)) => f(f(2 * (z))) (ANSI: expand macro args separately first!) */

 f(y+1) + f(f(z)) % t(t(g)(0) + t)(1);
/* Should be */ "f(2 * (y+1)) + f(2 * (f(2 * (z[0])))) % f(2 * (0)) + t(1);"

 g(x+(3,4)-w) | h 5) & m
	(f)^m(m);
/* Should be */ "f(2 * (2+(3,4)-0,1)) | f(2 * (~ 5)) & f(2 * (0,1))^m(0,1);"

#if 0
#define L insidestuff
	0L

#define same(a) a
#define rec(a) a rec(foo) bar
#define hard hard 

#if 0
rec(try)
/* hard */
#endif

#endif /* left out for now */
