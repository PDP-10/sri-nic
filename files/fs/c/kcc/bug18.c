#if 0
29-Nov-89 07:31:04-PST,513;000000000001
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Wed, 29 Nov 89 07:30:58 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891102)
	id AA09304; Wed, 29 Nov 89 10:29:46 -0500
Date: 29 Nov 89 10:00:48 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: BUGD.C
Message-Id: <"CSI 5676-14676"@CompuServe.COM>
#endif

/* BUGD.C */

main()
{
    char arr[100];

    if (sizeof(0,arr) != sizeof(&arr[0]))
	printf("? Uh-oh\n");
    
}

