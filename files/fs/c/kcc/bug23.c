#if 0
 1-Dec-89 13:32:53-PST,1442;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Fri, 1 Dec 89 13:32:29 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891115)
	id AA03401; Fri, 1 Dec 89 16:32:01 -0500
Date: 01 Dec 89 14:43:44 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: Updated version of BUG15.C
Message-Id: <"CSI 5679-4236"@CompuServe.COM>

Ken -- I believe I suffering burnout here.  What follows is a corrected
BUG15.C.  I neglected to remove the dvalue() calls in the previous
submission.  The file below should compile standalone.

Also, I referred to BUG16.C in a recent note.  I haven't submitted
BUG16 yet.  The reference should have been to BUG15.C.

And, lastly, I don't believe that you necessarily receive these
messages in the order that I send them.  This fact might explain
some forward references.

This will probably be all for today.  Have a good weekend.

-----------------------------------------------------------------------
#endif
/* BUG15.C */

#include <float.h>
main()
{
	double d1, d2, d3;

	d1 = 1.;
	d2 = (2 * DBL_EPSILON / 3);
	d3 = d1 + d2;	/* should cause rounding into lowest-order digit */
	if (FLT_ROUNDS == 1 && d3 <= d1)
		printf("? Pos round failed: %g %g %g\n", d1, d2, d3);
	d1 = -1.;
	d2 = -(2 * DBL_EPSILON / 3);
	d3 = d1 + d2;	/* should cause rounding into lowest-order digit */
	if (FLT_ROUNDS == 1 && d3 >= d1)
		printf("? Neg round failed: %g %g %g\n", d1, d2, d3);
}
