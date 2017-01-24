/* TIZERS.C - Test constants and initializers.
**	Compile with -S, check output.
**		use -DBADSYN to test error checks.
**	This file tries all possible types of permitted data initializations
**	in order to exercise the KCC code which (a) checks for legality in
**	CCDECL, and (b) generates the output in CCGEN.
**
*/

/* Various constants */
int coni1 = 123456789;		/* Decimal constant, type int */
int coni2 = 01234567;		/* Octal constant, type int */
int coni3 = 0x1aBcDeF;		/* Hex constant, type int */
long con1 = 0L;			/* Long decimal */
long con2 = 0777777777777;	/* 36 bits of ones */
float conf = 0. + .0 + 1.0 + 3e1	/* Various types of float constants */
	+ 1.0e-3 + .00034 + 3.14159 + 1e-3 + 2e+9;
char conch = 'A' + '\040';	/* Char constants */
char *constr = "String constant: \n\t\b\r\f\v\\\'\" and \1 \0111 \080";


/* Arithmetic data types */
char ca = 1, cb = -1;
signed char	sca = 1, scb = -1;
signed short	ssa = 1, ssb = -1;
signed int	sia = 1, sib = -1;
signed long	sla = 1, slb = -1;
unsigned char	uca = 1, ucb = -1;
unsigned short	usa = 1, usb = -1;
unsigned int	uia = 1, uib = -1;
unsigned long	ula = 1, ulb = -1;

int	sic = 123, sid = {100 * 2 };
float	fl1 = 12.3, fl2 = 12;
double	dbl = {12.3};
long double lda = 12.3456789;

/* KCC extended char types */
signed _KCCtype_char6	sc6a = 1, sc6b = -1;
signed _KCCtype_char7	sc7a = 1, sc7b = -1;
signed _KCCtype_char8	sc8a = 1, sc8b = -1;
signed _KCCtype_char9	sc9a = 1, sc9b = -1;
signed _KCCtype_char18	sc18a = 1, sc18b = -1;
unsigned _KCCtype_char6	uc6a = 1, uc6b = -1;
unsigned _KCCtype_char7	uc7a = 1, uc7b = -1;
unsigned _KCCtype_char8	uc8a = 1, uc8b = -1;
unsigned _KCCtype_char9	uc9a = 1, uc9b = -1;
unsigned _KCCtype_char18 uc18a = 1, uc18b = -1;

_KCCtype_char6 str6a[]	= "sixbit";
_KCCtype_char6 *str6p	= "sixbit";
_KCCtype_char7 str7a[]	= "asciz";
_KCCtype_char7 *str7p	= "asciz";
_KCCtype_char8 str8a[]	= "bytez";
_KCCtype_char8 *str8p	= "bytez";

/* Pointers */
/* 1 */ char *cp = 0;
/* 2 */ extern int foo(); static int (*fp)() = foo;
/* 3 */ char ary[100]; char *cp2 = ary;
/* 4 */ static short sh; short *sp = &sh;
/* 5 */ float popi[10]; float *psq = &popi[2];
/* 6 */ long *PSW = (long *) 0xFFFFFFF0;
/* 7 */ char *greet = "This is a test";
/* 8 */ int bar; int *ip = &bar + 3, *ip2 = &bar - 3;
/* 9 */ int bara[3][4]; int *ip3 = &bara[1][2];
/* 10 */ char *lcp = &"string"[2];
rtn()
{
	return bara[1][2];
}

/* Arrays */

int aryone[4] = {0,1,2,3,};
int arytwo[4][2][3] =
	{{{ 0, 1, 2},	{ 3, 4, 5,}},		/* Note trailing comma */
	 {{ 6, 7, 8},	{ 9,10,11,},},		/* Note trailing commas */
	 {{12,13,14},	{15,16,17}},
	 {{18,19,20},	{21,22,23}}};

struct ast {int a; float b;}
	arythree[3] = {{1, 2.5},{2,3.9},{0,-4.0}};

int aryfour[5] = {1,2,3};
int aryfive[3][3] = {{1,2,}, {3}};

int arysix[] = {0,1,2,3,4,5};

char arysev[5] = "ABCD";
char aryate[] = "ABCDEF";

char *arycps[] = {"one", "two", "three"};

/* Eliding braces */
int arymat[2][3] = {1,2,3,4,5,6};

/* Structures and unions */

struct S1 {int a;
	struct S2 {double b; char c;} b;
	int c[4]; };
struct S1 strx = {1, {4.5}};

#if __STDC__
union U1 { int i; double d; char c; };
union U1 uni = { 123 };
union U2 {double d; int i; char c; } und = { 123.456 };
union U3 { union U2 u; int i; } un3d = { {456.789} };
#endif

/* Special hackery case -- inner char array initialized by string */
struct cstruc {
	char a;	int b;	char ca[3]; int d;	/* 4 wds */
	char cam[3][4][5];			/* 017 wds */
} outca[3] = { {1, 2, "ab", 4} };		/* 023 wds per struct */ 

/* Complicated addresses */
char *cptr = &outca[2].ca[1];
char *cptr2 = &outca[2].cam[1][2][3];
char *cptr3 = outca[2].cam[1][2];
char *cptr4 = &((&outca[0])->ca[1]);


/* Handle casts of addrs */
char *captr = (char *)outca;
long *lcaptr = (long *)outca;

main(){}

/* Type-qualified data */

#if __STDC__
int tqun = 0;
const tqconi = 123;
volatile tqvoli = 456;
const volatile tqconv = 789;

struct svc { int a; const b; volatile c; const volatile d;
	const int *ip; char *volatile cp; const char *str;
	int *const cip;
};
struct svc svc1 = {
	123, 456, 'V', 'C'+'V',
	&tqconi, "volatile ptr", "const chars",
	&tqun,
};
#endif

#if __STDC__
/* Automatic initializations.
*/
autoin()
{
	char c = 'a';
	short si = 0123;
	int i = 456;
	long L = 0xbedaf;
	float f = 1e5;
	double d = 1e6;
	long double ld = 1e7;
	union U1 { float f; char c; } au = { 1e5};
	struct S1 {int a,b,c;} as = { 1, 2, 3};
	char carr[] = "char array";
	int iarr[10] = { 10, };

	return c + si + i + L + (int)(f + d + ld + au.f) + as.b + carr[6]
		+ iarr[5];
}
#endif /* __STDC__ */
