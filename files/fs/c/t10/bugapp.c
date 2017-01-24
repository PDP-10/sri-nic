#ifdef HELLO_KEN
24-Jul-90 10:42:28-GMT,1507;000000000005
Received: from NIC.DDN.MIL by NISC.SRI.COM (5.61/SRI-NISC1.0)
	id AA15528; Tue, 24 Jul 90 03:42:25 -0700
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Tue, 24 Jul 90 03:42:01 PDT
Received: by saqqara.cis.ohio-state.edu (5.61-kk/5.900605)
	id AA22392; Tue, 24 Jul 90 06:41:55 -0400
Date: 23 Jul 90 15:28:48 EDT
From: <MSNYDER@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: write()
Message-Id: <"CSI 5746-5946"@CompuServe.COM>

Thanks, your patch to _blkfin() appears to have worked.  We still
had a problem if the buffer length was a multiple of 4, and the
output file already existed before being opened.  We think it'll
be solved by subtracting one in the line before the one you changed;
will let you know when we have tested it more.

Interesting: it looks as if the file size in bytes is only tracked
in the case of 8 bit files; is that correct?  At any rate,
when we run the programs we sent you, with (7- or) 9-bit files, they
always read back a multiple of (5 or) 4 bytes.  Is that behaviour the
same under your simulator?

Here is a short program that will generate the other (iodump) error
message we mentioned.  If you type the output file after execution, 
it appears that both writes succeeded; the panic occurred during 
fclose.

#endif

#include <stdio.h>

FILE *out;

main()
{
    out = fopen("a.tmp", "w");
    fprintf(out, "Hollow world\n");
    fclose(out);
    out = fopen("a.tmp", "a");
    fprintf(out, "Yellow world\n");
    fclose(out);
}
