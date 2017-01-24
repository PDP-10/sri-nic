#if 0
29-Nov-89 07:28:10-PST,779;000000000001
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Wed, 29 Nov 89 07:28:00 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891102)
	id AA07426; Wed, 29 Nov 89 09:58:05 -0500
Date: 29 Nov 89 09:34:16 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: BUGC.C
Message-Id: <"CSI 5676-14446"@CompuServe.COM>
#endif

/* BUGC.C */

#include <limits.h>

main()
{
    signed long int sl;
    unsigned char uc = ivalue(4);

    /* integer demoted to smaller unsigned: modulo "largest plus one" */

    sl = ivalue(-3);
    uc = sl;
    if (uc != UCHAR_MAX-2)
	printf ("? Uh-oh\n");
    
}

static int Zero = 0;	/* See  setzero()  below */
int ivalue(i)
	int i;
	{
	return i + Zero;
	}


