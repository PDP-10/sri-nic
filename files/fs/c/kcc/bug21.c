#if 0
30-Nov-89 12:33:50-PST,1326;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from giza.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Thu, 30 Nov 89 12:31:33 PST
Received: by giza.cis.ohio-state.edu (5.61/4.891102)
	id AA10442; Thu, 30 Nov 89 15:31:13 -0500
Date: 30 Nov 89 13:27:16 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: XBUG1.C
Message-Id: <"CSI 5678-3855"@CompuServe.COM>
#endif

/* XBUG1.C */

double dvalue(double);

main()
	{
	static long stlong2;
	static double d1;
	auto double * pd1;
	static double d2;
	static double * pd2;
	static long stlong6;
	static long stlong7;
	static double d4;

	stlong2 = 2L;
	d1 = dvalue(1.100000000000000E+000);
	pd1 = &d1;
	d2 = dvalue(2.200000000000000E+000);
	pd2 = &d2;
	stlong6 = 6L;
	stlong7 = 7L;
	d4 = dvalue(4.400000000000000E+000);

	/*
    	   Ken --
	   The following comparison somehow brings about the problem.
	   If the two expressions being compared with == are encountered
	   by themselves, then there is no error.

          Also, if the calls to dvalue() are removed, the problem goes
          away.
	*/

	if ((*pd1 += (*pd2 < d4)) == (stlong2 %= (stlong7 += stlong6)));

	if (stlong2 != 2L)
		printf ("? Uh-oh");
	}

static int Zero = 0;	/* See  setzero()  below */
double dvalue(i)
	double i;
	{
	return i + Zero;
	}

