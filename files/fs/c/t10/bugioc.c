#if 0
20-Jul-90 11:54:43-GMT,1479;000000000005
Received: from NIC.DDN.MIL by NISC.SRI.COM (5.61/SRI-NISC1.0)
	id AA13073; Fri, 20 Jul 90 04:54:42 -0700
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Fri, 20 Jul 90 04:52:25 PDT
Received: by saqqara.cis.ohio-state.edu (5.61-kk/5.900605)
	id AA17910; Fri, 20 Jul 90 07:45:39 -0400
Date: 20 Jul 90 07:16:47 EDT
From: <BEAR@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: Mine and Michael's variation on the first --Kevin
Message-Id: <"CSI 5745-5897"@CompuServe.COM>
#endif

/*  am I nuts? the last character of wBuf gets hosed.
 */

#include <stdio.h>
#include <sys/file.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char   ubyte;

int File= 0;

ubyte  wBuf[] = "Hi, whats ";
/*    {    'a', 'b', 'c', 'd', 'e'    }; */

ubyte  rBuf[ 100 ];
int    Len;

/*  Send a file sequence (complete/w startup) */
main()
{	int i;
    if ((File = open( "a.tmp", O_WRONLY | O_BSIZE_9 | O_BINARY | O_CREAT)) < 0)
        return -2;
    write( File, wBuf, sizeof(wBuf)-1);
    close( File );
    if ((File = open( "a.tmp", O_RDONLY | O_BSIZE_9 | O_BINARY)) < 0)
        return -2;
    Len = read(File, rBuf, 100 );
    printf("sizeof( wBuf ) = %d, length read %d.\n", sizeof( wBuf ), Len );
    for (i = 0; i < Len; i++ )
        printf("read %2X should be %2x\n", rBuf[ i ], wBuf[ i ] );
    close(File);
    return 0;
}
