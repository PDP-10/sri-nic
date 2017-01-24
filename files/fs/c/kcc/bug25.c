#if 0
 5-Dec-89 13:26:20-PST,1620;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from giza.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Tue, 5 Dec 89 13:25:58 PST
Received: by giza.cis.ohio-state.edu (5.61/4.891204)
	id AA13404; Tue, 5 Dec 89 16:25:24 -0500
Date: 05 Dec 89 15:37:51 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: EBUG31.C
Message-Id: <"CSI 5679-21368"@CompuServe.COM>

#endif
/* EBUG31.C */

/* Ken -- Each of the following should generate an error. */
/* Rather than make several tiny files, this is the first */
/* of a dozen or so small files concerning errors that    */
/* ANSI seems to think need to be generated, but that KCC */
/* doesn't detect or doesn't detect as an error.   Benny  */

main()
{
    
    /* M31003.C */
    /* short and int are different, even if same size */
    { short *p; int *q = 0; p = q; } 	/* Should generate error */
    
    /* M31004.C */
    /* long and int are different, even if same size */
    { long  *a; int *b = 0; a = b; } 	/* Should generate error */
    
    /* M31008.C */
    { /* enum constant must fit the (int) storage */    
    #include <limits.h>
    enum { A = INT_MAX, B };		/* Should generate error */
    }

    /* M31015.C */
    /* violates hexadecimal-constant syntax */
    { int i = 0x ; }

    /* M31019.C */
    /* value of hex escape sequence shall fit its storage */
    { int i = sizeof('\x1234567890123456789012345678901234567890'); }

    /* M31030.C */
    /* matching of {} punctuator is constraint */
    { { }

    /* M31032.C */
    { /* header-name syntax */
    #include <stdio.h
    }

}

