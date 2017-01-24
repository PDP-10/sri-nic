#if 0
19-Jan-90 10:13:30-PST,1070;000000000005
Return-Path: <msnyder@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Fri, 19 Jan 90 10:13:17 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891221)
	id AA03090; Fri, 19 Jan 90 13:09:52 -0500
Date: 19 Jan 90 10:27:21 EST
From: <MSNYDER@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: bug in CCTYPE
Message-Id: <"CSI 5693-8093"@CompuServe.COM>
#endif

int airline[20][20] = {
	{1, 2, 3, 4, 5},
	{6, 7, 8, 9, 0}
    };

main()
{   int y;

    y = airline[0][0];
}

#if 0
Ken,
     This program compiles OK with the compiler version prior to all the
validation suite changes (maybe we need some revision numbering scheme!),
but yields the error msg "Array ref must be lvalue" when compiled with 
the new compiler.  I've tried the "generic tops-10" flavor compiler too,
and it gives the same error message.
     This message comes from convarrfn() of CCTYPE.  I know this is not
one of your top priorities any more, but it's a little deep for me; 
could you help find what's wrong?
					Thanks,
					Michael

#endif
