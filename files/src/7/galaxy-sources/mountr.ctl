!MOUNTR.CTL
!This Control File Assembles, Loads, and saves MOUNTR

!             COPYRIGHT (C) 1979,1980,1981,1982,1983,1984,1987,1988
!                    DIGITAL EQUIPMENT CORPORATION
!
!     THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY  BE  USED
!     AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE
!     AND WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.   THIS
!     SOFTWARE  OR ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR
!     OTHERWISE MADE AVAILABLE TO ANY OTHER PERSON.  NO  TITLE  TO
!     AND OWNERSHIP OF THE SOFTWARE IS HEREBY TRANSFERRED.
!
!     THE INFORMATION  IN  THIS  SOFTWARE  IS  SUBJECT  TO  CHANGE
!     WITHOUT  NOTICE  AND SHOULD NOT BE CONSTRUED AS A COMMITMENT
!     BY DIGITAL EQUIPMENT CORPORATION.
!
!     DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR RELIABILITY
!     OF  ITS  SOFTWARE  ON  EQUIPMENT  WHICH  IS  NOT SUPPLIED BY
!     DIGITAL.

!System Files

!	ACTSYM.UNV
!	MACSYM.UNV
!	MONSYM.UNV
!	SERCOD.UNV
!	MACREL.REL

!Required Files in Your Area

!	GLXMAC.UNV
!	ORNMAC.UNV
!	QSRMAC.UNV

!Source Files

!	MOUNTR.MAC
!	MTRCFS.MAC
!	MTRMAC.MAC
!	MTRUSR.MAC
!	MTRDDB.MAC

!Output File

!	MOUNTR.EXE
!	MTRMAC.UNV

DEF::
@TAKE BATCH.CMD

@DEFINE REL: DSK:,SYS:
@DEFINE UNV: DSK:,SYS:
!
VER::
@SET TRAP FILE
@VDIR MOUNTR.MAC,MTRCFS.MAC,MTRMAC.MAC,MTRUSR.MAC,MTRDDB.MAC,
@CHECKSUM SEQ
@

!
COMP::

@ENA
@GET SYS:MACRO
@I VER
@ST
*MTRMAC=MTRMAC
*MOUNTR=MOUNTR
*MTRCFS=MTRCFS
*MTRUSR=MTRUSR
*MTRDDB=MTRDDB
*^C
!
!
END::
@GET SYS:LINK.EXE
@ST
*MOUNTR,MTRCFS,MTRUSR,MTRDDB
*MOUNTR/SAVE
*/GO
*^C
!
@GET MOUNTR.EXE
@INFORMATION VERSION
!
@VDIR MOUNTR.EXE,
@CHECKSUM SEQ
@
!

@PLEASE	MOUNTR Assembly Successful

NOERROR

@MODIFY BATCH GALAXY/DEP:-1
!
%CERR::
%ERR::
@PLEASE	Error During MOUNTR Assembly


%FIN::
