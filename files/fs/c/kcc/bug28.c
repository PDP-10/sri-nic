#if 0
 5-Dec-89 14:52:37-PST,770;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Tue, 5 Dec 89 14:52:26 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891204)
	id AA13464; Tue, 5 Dec 89 17:52:07 -0500
Date: 05 Dec 89 16:33:40 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: EBUG34.C
Message-Id: <"CSI 5679-21773"@CompuServe.COM>

A Systems-wide gathering at Jill's house?  Okay by me.
#endif
/* EBUG34.C */

main()
{
    /* Should generate an error */ /* no comma in const expression */
    { static int d = (1,2); }

    /* Should generate an error */ /* each const expr shall fit its type */
    {
#include <limits.h>
    int i = 0;
    switch (i)
	{
	case LONG_MAX * 4 : i = 1;
	}    
    }

}

