/* TLEX.C - This file tests the lexical analyzer's handling of constants.
**	Compile with -DBADSYN to test syntactic error detection,
**		with -DBADVAL to test value error detection.
*/
#include <limits.h>
#include <float.h>


/* Floating constants  [dpANS 3.1.3.1] */

float farr[] =  {
	.0,	0.,	0.0,	.1,	1.,	1.1,
	.0f,	0.f,	0.0f,	.1F,	1.F,	1.1F,
	.0L,	0.L,	0.0L,
	.0e0,	0.e0,	0.0e0,	.1E0,	1.E0,	1.1E0,
	.0e+1,	0.e+1,	0.0e+1,	.1E-1,	1.E-1,	1.1E-1,
	0e0,	0e+10,	0e-10,	1e0,	1e10,	1e-10,
	123.123e+01
};

double darr[] = {
	1e0, 1e1, 1e2, 1e4, 1e8, 1e16, 1e32,
	1e-0, 1e-1, 1e-2, 1e-4, 1e-8, 1e-16, 1e-32,
};

float fmax = FLT_MAX;
float fmin = FLT_MIN;
float feps = FLT_EPSILON;
float fmaxexp = 1e+38;
float fminexp = 1e-38;
double dmax = DBL_MAX;
double dmin = DBL_MIN;
double deps = DBL_EPSILON;
double dmaxexp = 1e+38;
double dminexp = 1e-38;

#ifdef BADSYN
float f1 = 1234;	/* Will just cast */
float f2 = e10;
float f3 = 00e;
float f4 = 00e+;
float f5 = 00e0+;
#endif /* BADSYN */

#ifdef BADVAL
double dbadarr[] = {
	1e40,		/* Exponent out of range */
	012345678901234567890123456789012345678901234567890.,
	0.12345678901234567890123456789012345678901234567890,
012345678901234567890123456789012345678901234567890.12345678901234567890123456789012345678901234567890,
	1e1000		/* REALLY out of range! */
};
#endif

/* Integer constants  [dpANS 3.1.3.2] */

int iarr[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 01, 02, 03, 04, 05, 06, 07, 010, 011, 012, 013, 014, 015, 016, 017,
	0x0, 0x1, 0x2, 0x4, 0x8, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf,
	0X0, 0X1, 0X2, 0X4, 0X8, 0XA, 0XB, 0XC, 0XD, 0XE, 0XF,
	0u, 1l, 2ul, 3lu, 4U, 5L, 6UL, 7LU,
	0010, 0x00a, 999, 0Xac0fl, 0xFADE2UL, 0xDeafful,
	0777777777777,
	0xfffffffff,
};

#ifdef BADSYN
int i1 = 0x;
int i2 = 0xi;
int i3 = 08;
int i4 = 0a;
int i5 = 0LL;
int i6 = 0UU;
int i7 = 0ULL;
int i8 = 0UL2;
#endif /* BADSYN */

#ifdef BADVAL
long larr[] = {
	0x00ffffffffffff,
	12345678901234567890123456789,
	0177777777777777,
};
#endif

/* Character constants  [dpANS 3.1.3.3] */

char charr[] = {
	'a', 'b', 'c', '"', '?',
	'\'', '\"', '\?', '\\',
	'\a', '\b', '\f', '\n', '\r', '\t', '\v',
	'A', '\101', '\x41', '\x00041',
	L'A', L'B', L'C',
	'\777', '\x1ff'
};

#ifdef BADSYN
char synchs[] = {
	'12', '\1234', '\238',
	'\x', '\q', '\Xfee',
	'', '',		/* May or may not complain */
};
#endif /* BADSYN */

#ifdef BADVAL
long valchs[] = {
	'\x1234',
};
#endif

/* String literals  [dpANS 3.1.4] */

char *sarr[] = {
	"abc'?",
	"\"\'\?\\",
	"\a\b\f\n\r\t\v",
	L"A\101\x41\x00041",
	L"ABC",
	"\777\x1ff",
	"\01234\128\x000aCK",
	"\x12" "3" "4\567" "890",	/* Concatenation */
	L"one " L"two " L"three",
};

#ifdef BADSYN
char *synstrs[] = {
	"\x", "\q", "\Xfee",
	L"one " "two " L"three",	/* Should complain */
	"", "",			/* May or may not complain */
};
#endif /* BADSYN */

#ifdef BADVAL
char *valstrs[] = {
	"abc\x1234ghi",
};
#endif
