#if 0
30-Nov-89 14:52:50-PST,4617;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Thu, 30 Nov 89 14:52:21 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891102)
	id AA01476; Thu, 30 Nov 89 17:25:55 -0500
Date: 30 Nov 89 15:08:23 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: XBUG2.C
Message-Id: <"CSI 5678-4785"@CompuServe.COM>
#endif

/* XBUG2.C

	Ken --- This seems sort of petty, but it showed up in the validation
        suite.  The calls to dequals failed as well, but in the course of
        carving up the sources those errors disappeared.
*/
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#define DIGITS_MAX 		(DBL_DIG - 1)	/* number of significant digits for double precision comparison. */
#define LDIGITS_MAX 	(LDBL_DIG - 1)	/* number of significant digits for long double precision comparison. */
#define UCHAR unsigned char
#define USHORT unsigned short
#define ULONG unsigned long
typedef char CHAR;
typedef short SHORT;
typedef int INT;
typedef long LONG;
typedef float FLOAT;

typedef unsigned int UINT;

typedef double DOUBLE;
typedef signed char SCHAR;
typedef long double LDOUBLE;
static char details[BUFSIZ] = {0};

main()
	{
	auto struct {
		CHAR c;
		SHORT s;
		INT i;
		UCHAR uc;
		USHORT us;
		UINT ui;
		LONG l;
		ULONG ul;
		FLOAT f;
		DOUBLE d;
		SCHAR sc;
		LDOUBLE ld;
		} G, *pG = &G;

	pG->c = 7;
	pG->sc = 8;
	pG->s = 9;
	pG->i = 10;
	pG->uc = 11;
	pG->us = 12;
	pG->ui = 13;
	pG->l = 14;
	pG->ul = 15;
	pG->f = 16;
	pG->d = 17;
	pG->ld = 18;
	{
	auto CHAR Jc[12][12];
	int x_0 = 0;
	int y_0 = 0;
	auto SCHAR Jsc[12][12];
	int x_1 = 1;
	int y_1 = 1;
	auto SHORT Js[12][12];
	int x_2 = 2;
	int y_2 = 2;
	auto INT Ji[12][12];
	int x_3 = 3;
	int y_3 = 3;
	auto UCHAR Juc[12][12];
	int x_4 = 4;
	int y_4 = 4;
	auto USHORT Jus[12][12];
	int x_5 = 5;
	int y_5 = 5;
	auto UINT Jui[12][12];
	int x_6 = 6;
	int y_6 = 6;
	auto LONG Jl[12][12];
	int x_7 = 7;
	int y_7 = 7;
	auto ULONG Jul[12][12];
	int x_8 = 8;
	int y_8 = 8;
	auto FLOAT Jf[12][12];
	int x_9 = 9;
	int y_9 = 9;
	auto DOUBLE Jd[12][12];
	int x_10 = 10;
	int y_10 = 10;
	auto LDOUBLE Jld[12][12];
	int x_11 = 11;
	int y_11 = 11;
	Jc[x_0][y_0] = 7;
	Jsc[x_1][y_1] = 8;
	Js[x_2][y_2] = 9;
	Ji[x_3][y_3] = 10;
	Juc[x_4][y_4] = 11;
	Jus[x_5][y_5] = 12;
	Jui[x_6][y_6] = 13;
	Jl[x_7][y_7] = 14;
	Jul[x_8][y_8] = 15;
	Jf[x_9][y_9] = 16;
	Jd[x_10][y_10] = 17;
	Jld[x_11][y_11] = 18;
	{
	dequals(__LINE__, pG->f + Jd[x_10][y_10], 33.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->f, 33.);
	ldequals(__LINE__, pG->f + Jld[x_11][y_11], 34.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->f, 34.L);
	}}
	}

static int Zero = 0;	/* See  setzero()  below */

/*
 * DEQUALS - 'double' equality check.  If val1 != val2, then
 * report an error. This is computed using an equality approximation
 * that verifies that the two numbers are equal to R digits whenever
 *
 *    |x - y|    1    1-R
 *    ------- <= - 10
 *      |x|      2
 *
 * DIGITS_MAX  is defined in defs.h
 */
double Delta = 0.0;
dequals(line, val1, val2)
	double val1, val2;
	int line;
	{
	double *pd;

	if (Delta == 0.0)
		Delta = 0.5 / pow(10.0, DIGITS_MAX-1.0); 
	if (val1 == val2)
		{
		return (1);
		}
	pd = &val1;
	if (val1 == 0.0)
		pd = &val2;

	/* special cases to handle zero against very small numbers */
	if (fabs(val1) == 0.0 && fabs(val2) < Delta)
		;
	else if (fabs(val2) == 0.0 && fabs(val1) < Delta)
		;
	else if ((fabs(val1 - val2) / fabs(*pd)) > Delta)
		{
		sprintf(details, "DEQUALS ERROR: (%.*G) != (%.*G)",
			DIGITS_MAX+2, val1, DIGITS_MAX+2, val2);
		printf("%s\n",details);
		return (0);
		}
	return (1);
	}






/*
 * LDEQUALS - Long double equality ... more of the same.
 */
#define ldabs(ld) ((ld) < 0.0 ? -(ld) : (ld))
long double LDelta = 0.0;
ldequals(line, val1, val2)
	long double val1, val2;
	int line;
	{
	long double *pd;

	if (LDelta == 0.0)
		{
		LDelta = 0.5L / pow(10.0, LDIGITS_MAX-1.0); 
		}
	if (val1 == val2)
		{
		return (1);
		}
	pd = &val1;
	if (val1 == 0.0)
		pd = &val2;

	/* special cases to handle zero against very small numbers */
	if (ldabs(val1) == 0.0 && ldabs(val2) < LDelta)
		;
	else if (ldabs(val2) == 0.0 && ldabs(val1) < LDelta)
		;
	else if ((ldabs(val1 - val2) / ldabs(*pd)) > LDelta)
		{
		sprintf(details, "LDEQUALS ERROR: (%.*LE) != (%.*LE)",
			LDIGITS_MAX+2, val1, LDIGITS_MAX+2, val2);
		printf("%s\n",details);
		return (0);
		}
	return (1);
	}
