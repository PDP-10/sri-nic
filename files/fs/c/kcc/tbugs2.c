#if 0	/* TEST BUGS FILE #2 */

	This file consists of various small pieces of code which
in the past would tickle some kind of KCC bug or another.  By including
them all in one source file, and seeing if new KCC versions produce any
differences in the resulting .FAI file, it is possible to quickly verify
that no old bugs have been re-introduced.

	The fancy name for this is "regression testing".

	To repeat: as far as is known, all the code here DOES now compile
correctly.  Code sequences which do NOT compile correctly are left as
separate BUG*.C files until their respective bugs are fixed, whereupon the
code is then moved into here.
#endif

/* 2-Nov-89 Hit internal error while comparing pointer constant operands.
** Reported 25-Oct-89 by Ben @ CSI (Found by PH suite)
*/
#define	SIG_DFL	(void (*)(int))0 /* Arg to "signal" to resume default action */
#define	SIG_IGN	(void (*)(int))1 /* Arg to "signal" to ignore this sig */
#define	SIG_ERR	(void (*)(int))(-1) /* Return val from "signal" if error */
bug14()
{
    extern int checkthat();
    checkthat(__LINE__, SIG_DFL != SIG_ERR && SIG_DFL != SIG_IGN && SIG_ERR != SIG_IGN);
}

/* 2-Nov-89 Register spill code never did handle overflow case correctly.
** Re-did most of CCREG to fix this one.
** Reported 25-Oct-89 by Ben @ CSI (Found by PH suite)
*/
bug10()
{
/* #5: 32 expressions nested by parentheses (ANSI8709 reduced this from 127) */
          /* 1  2  3  4  5  6  7  8  9  10 */
#define L(a) (1-(1-(1-(1-(1-(1-(1-(1-(1-(1-a
#define R    )  )  )  )  )  )  )  )  )  )  
    /* L(x) generates 10 left parens, 30 chars (plus length of x) */
    int i = 2;
    int j;
    extern int printf();

    j = L(L(L((1-(i-1))))) R R R;
    if (j != 0) printf("? Error: j should be equal to 0.  j=%d\n",j);
}

/* 25-Oct-89 Constant folding overlooked unsigned->float case.
** Reported 25-Oct-89 by Ben @ CSI (Found by PH suite)
*/
double bug9()
{
    return (double)(~(unsigned)2);
}

/* 23-Oct-89 Mistakenly complained about bitfields in unions.
** Reported 23-Oct-89 by Ben @ CSI (Found by PH suite)
*/
union ufields { unsigned x1 : 1; } uf1;

/* 23-Oct-89 Mistakenly complained about call to non-function.
** Reported 23-Oct-89 by Ben @ CSI (Found by PH suite)
*/
bug2(foo)
int foo();
{
    foo(1);
}

/* 25-Oct-89 Type equivalence checking bug (found by PH suite)
** Reported 23-Oct-89 by Ben @ CSI
*/
typedef void VF();
typedef void VFVF(VF *);
static void bug1 (VF *);
static void bug1 (VFVF *);
static void bug1(VFVF *pf)
{
    extern int checkthat();
    checkthat(__LINE__, pf == bug1);
}

typedef int typi;
int bugpfi(int (*)(typi));
int bugpfx(int pfi(int))
    { return pfi(0);}

/* 23-Oct-89 Preprocessor bug uncovered by Plum Hall validation suite.
*/
#define pp1 x ## y
#define xy b
#define cat3() x ## x ## y
bug7() {
    static int xxy, b, res;
	res = pp1;		/* -> xy -> b */
	res = cat3();		/* -> xxy */
}
#undef pp1
#undef xy
#undef cat3

/* 14-Oct-89 Fixed peepholer bug in CCOPT's makepop() which was mistakenly
** adjusting stack refs the wrong way.
** Reported 13-Oct-89 by Peter Samson of SC (S.SNELSON@Macbeth.Stanford.EDU)
*/
struct bugpds {	int a;	int b;	char *c;} ;
struct bugpds
bugpds(p)
char *p;
{	char *p1 = p;
	struct bugpds val;
	int b = 0;
	int a = 0;
	extern int dummy();

	dummy();	/* this must be here to lose */
	val.a = a;
	val.b = b;
	val.c = p1;
	return val;
}

/* 14-Sep-89 Logical testing trickery fixup.  The code in these routines
** was being produced suboptimally (doing a TRNE R,777 after a LDB) and
** possibly wrongly (only testing first word of a double for zero).  Latter
** case is left alone for speed since unnormalized doubles shouldn't exist
** anyway.
*/
bugtz1(s1, s2)
char *s1, *s2;
{
    extern int barf();
    static double dbl;
	if (*s1 = *s2) return barf();
	if (dbl) return barf();
	return 0;
}

bugtz2(s, t)
char *s, *t;
{
    extern int barf();
	if ((*s = *t) != 0) return barf();
	return 0;
}

/* 12-Sep-89 This uncovered a flaw in the algorithm to generate the source
** for automatic array initialization (string literals were flushed before
** code generator got to them).
** Reported 12-Sep-89 by MSnyder @ CSI.CompuServe.COM
*/
char *
bugaag(i)
{
    char *name[] = {"alloca", "misc", 0 };
    return name[i];
}

/* 1-Jun-88 I think this used to trigger an internal error when attempting
** to evaluate the right-hand constant.  This now compiles without error
** into a doomed run-time evaluation.
*/
bugevl()
{
    static struct { int cnt; char *sbp; } bar;
    bar.sbp = ((char *)0)-1;
}

/* 13-Jun-88 Syntax problem finally fixed by ANSI KCC.
** Reported 11-Jun-88 by Bill Palmer at cisco.
*/
bugsyi()
{
    static struct foo { int bar, baz; } *glork;
    glork++ -> baz = 0;
}
