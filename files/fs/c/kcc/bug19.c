#if 0
29-Nov-89 07:31:25-PST,568;000000000001
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Wed, 29 Nov 89 07:30:39 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891102)
	id AA09274; Wed, 29 Nov 89 10:29:31 -0500
Date: 29 Nov 89 10:01:13 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: BUGE.C
Message-Id: <"CSI 5676-14683"@CompuServe.COM>
#endif

/* BUGE.C */

main()
{
    char carray[] = "variable string literal";
    char *const constp = carray;
    ++*constp;
    if (*constp != 'v'+1)
	printf ("? Uh-oh\n");
}

