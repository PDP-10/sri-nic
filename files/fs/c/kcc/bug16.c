#if 0
28-Nov-89 14:27:13-PST,1031;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Tue, 28 Nov 89 14:27:08 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891102)
	id AA26224; Tue, 28 Nov 89 16:55:28 -0500
Date: 28 Nov 89 16:26:00 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: BUGB.C
Message-Id: <"CSI 5676-11719"@CompuServe.COM>
#endif

/*  BUGB.C  */
struct {
    unsigned ubf2 : 2;
    signed sbf2 : 2;
    int ibf2: 2;
    } bf;

main()
{
    bf.ubf2 = ivalue(-1);
    if (bf.ubf2 != 3)
	{
	printf ("? Uh-oh\n");
        }
}

/*
 * VALUE - the value routines are used to defeat value propagation in optimizing compilers.
 * We want to make sure that we are testing what we think we are testing, not what the compiler transformed it to.
 * 1988: Some compilers "open-code" all small functions.  Now we have to hide the constants still further.
 */
static int Zero = 0;	/* See  setzero()  below */
int ivalue(i)
	int i;
	{
	return i + Zero;
	}

   

