#if 0
 5-Dec-89 13:29:34-PST,595;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from giza.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Tue, 5 Dec 89 13:29:21 PST
Received: by giza.cis.ohio-state.edu (5.61/4.891204)
	id AA13733; Tue, 5 Dec 89 16:29:06 -0500
Date: 05 Dec 89 15:53:30 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: EBUG33.C1
Message-Id: <"CSI 5679-21464"@CompuServe.COM>

#endif
/* EBUG33.C1 */
/* Generate an internal error */
/* Well, this should generate an error, but probably not an internal error */

main()
{
    struct x { char a[4]; } f();
    int i;
    i = f().a[0];
}

