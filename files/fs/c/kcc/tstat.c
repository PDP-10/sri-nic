#if 0
11-Mar-88 12:15:07-PST,3279;000000000000
Received: from SCIENCE.UTAH.EDU by SRI-NIC.ARPA with TCP; Fri 11 Mar 88 12:05:01-PST
Date: Fri 11 Mar 88 11:42:37-MST
From: "Nelson H.F. Beebe" <Beebe@SCIENCE.UTAH.EDU>
Subject: Problem in fstat()/stat()
To: bug-kcc@SRI-NIC.ARPA
cc: BEEBE@SCIENCE.UTAH.EDU
X-US-Mail: "Center for Scientific Computing, South Physics, University of Utah, Salt Lake City, UT 84112"
X-Telephone: (801) 581-5254
Message-ID: <12381527631.21.BEEBE@SCIENCE.UTAH.EDU>

Under Unix, fstat() on a new open file correctly returns the
file size after an fflush(); I ran the following test
program on VAX 4.3BSD, Sun OS 3.3 (4.2BSD), Gould 4.2BSD,
Iris (System V), and HPUX (System V), and on all got the
output

fstat: statbuf.st_size = 1029 (expected 1029)
stat: statbuf.st_size = 1029 (expected 1029)

Here is a PHOTO log for our current version of KCC.  As you
can see from the output, PCC-20 and VAX VMS are also
incorrect.  I'm not worried about the different non-zero
byte counts, because that reflects end-of-line translation.
However, the fact that fstat() returns a 0 file size when
the file is not empty broke a program I tried to install
this morning.


[PHOTO:  Recording initiated  Fri 11-Mar-88 11:29AM]

 TOPS-20 Command processor 5(712)
@type bug.c
------------------
APS:<BEEBE>BUG.C.5
------------------
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXSIZE 1029			/* this number is ARBITRARY */

struct stat statbuf;

main()
{
    int k;
    static char fname[] = "bug.tmp";
    FILE* fp;
    
    fp = fopen(fname,"w");
    if (fp == (FILE*)NULL)
    {
        fprintf(stderr,"?Cannot open %s\n",fname);
	exit(1);
    }

    for (k = 0; k < MAXSIZE; ++k)	/* write the file */
       putc(k,fp);

    fflush(fp);				/* empty buffers */

    if (fstat(fileno(fp),&statbuf))	/* get its size */
	perror(fname);
    else
        fprintf(stderr,"fstat: statbuf.st_size = %d (expected %d)\n",
	    statbuf.st_size,MAXSIZE);

    fclose(fp);
    if (stat(fname,&statbuf))		/* get its size again */
	perror(fname);
    else
        fprintf(stderr,"stat: statbuf.st_size = %d (expected %d)\n",
	    statbuf.st_size,MAXSIZE);
}
#if 0
@
@def c: cnew:
@v cnew:kcc.exe.0,cnew:libc.rel.0

   ND20:<SUBSYS.KCC.NEW>
 KCC.EXE.583;P777752      156 79872(36)  13-Jan-88 05:44:47 KLH       
 LIBC.REL.206;P777752      84 42858(36)  16-Dec-87 23:27:22 KLH       

 Total of 240 pages in 2 files
@kcc -o bug bug.c
KCC:	bug
<BEEBE>BUG.PRE.1
<BEEBE>BUG.FAI.1
FAIL:  bug
LINK:	Loading
@bug
fstat: statbuf.st_size = 0 (expected 1029)
stat: statbuf.st_size = 1031 (expected 1029)
@bug
fstat: statbuf.st_size = 0 (expected 1029)
stat: statbuf.st_size = 1031 (expected 1029)
@del bug.rel,bug.exe,bug.tmp
 BUG.REL.4 [OK]
 BUG.EXE.4 [OK]
 BUG.TMP.6 [OK]
@cc -o bug bug.c
[PCC: bug.c]
MACRO:	bug.c
LINK:	Loading
@bug
fstat: statbuf.st_size = 0 (expected 1029)
stat: statbuf.st_size = 1030 (expected 1029)
@
@; Here is what VAX VMS (VMS C 2.3 + 2.2 libraries) says:
@remark
Type remark.  End with CTRL/Z.
NHFB-8600>run bug
fstat: statbuf.st_size = 0 (expected 1029)
stat: statbuf.st_size = 1029 (expected 1029)
^Z
@pop

[PHOTO:  Recording terminated  Fri 11-Mar-88 11:37AM]
-------
#endif
