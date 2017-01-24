#if 0
28-Nov-89 13:56:32-PST,589;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Tue, 28 Nov 89 13:55:03 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891102)
	id AA26069; Tue, 28 Nov 89 16:53:59 -0500
Date: 28 Nov 89 16:05:21 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: BUGA.C
Message-Id: <"CSI 5676-11543"@CompuServe.COM>

/* BUGA.C
   Ken -- I cannot find what the L means.  Please advise.
*/
#endif

#include <stddef.h>

main()
{
	if (!(sizeof(L'x') == sizeof(wchar_t)))
	    {
	    printf("? Uh-oh\n");
	    }
}


