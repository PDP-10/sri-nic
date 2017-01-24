#if 0
29-Nov-89 12:03:57-PST,2045;000000000001
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Wed, 29 Nov 89 12:01:55 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891102)
	id AA18905; Wed, 29 Nov 89 14:18:53 -0500
Date: 29 Nov 89 13:06:18 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: BUGF.C --- This is probably the last bug submitted today.
Message-Id: <"CSI 5676-16327"@CompuServe.COM>
#endif

/* BUGF.C */

#include <stdio.h>
#include <float.h>
#define FDIGITS_MAX 	(FLT_DIG - 1)	/* number of significant digits for single precision comparison. */
#define DIGITS_MAX 		(DBL_DIG - 1)	/* number of significant digits for double precision comparison. */
#define LDIGITS_MAX 	(LDBL_DIG - 1)	/* number of significant digits for long double precision comparison. */

main()
{
    static float c36var = 5.;
	{
	extern int c36var;
        if (c36var != 12)
	    printf ("? Uh-oh, c36var was %d instead of 12\n",c36var);
	}
    dequals(__LINE__,c36var,5.);
}
int c36var = 12;

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
static char details[BUFSIZ] = {0};
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
		sprintf(details, "ERROR: (%.*G) != (%.*G)",
			DIGITS_MAX+2, val1, DIGITS_MAX+2, val2);
		printf("%s",details);
		return (0);
		}
	return (1);
	}


