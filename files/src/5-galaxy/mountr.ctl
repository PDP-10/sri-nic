! [SRI-NIC]SRC:<5-GALAXY>MOUNTR.CTL.3, 24-Jun-85 23:05:04, Edit by HSS!
! Don't CREF because of PA1050 limitations.
! [SRI-NIC]SRC:<5-GALAXY>MOUNTR.CTL.2, 24-Jun-85 22:14:51, Edit by HSS!
! Change definition of SYS:

!MOUNTR.CTL
!This Control File Assembles, Loads, and saves MOUNTR

!             COPYRIGHT (C) 1979,1980,1981,1982,1983,1984
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

!Required UNV and REL files

!	ACTSYM.UNV
!	GLXMAC.UNV
!	MACSYM.UNV
!	MONSYM.UNV
!	ORNMAC.UNV
!	QSRMAC.UNV
!	SERCOD.UNV
!	MACREL.REL

!Source Files

!	MOUNTR.MAC
!	mtrcfs.mac
!	mtrmac.mac
!	mtrusr.mac
!	MTRDDB.MAC

!Output File

!	MOUNTR.EXE
!	mtrmac.unv
!
@DEFINE CRF: NUL:
@GOTO DEF
!
CREF:: 
@DEF CRF: DSK:
!
!
DEF::
@DEFINE SYS: DSK:,PS:<LOC-SUBSYS>,PS:<SUBSYS>
@DEFINE REL: DSK:,SYS:
@DEFINE UNV: DSK:,SYS:
!
VER::
@SET TRAP FILE
@VDIR MOUNTR.MAC,mtrcfs.mac,mtrmac.mac,mtrusr.mac,
@CHECKSUM SEQ
@

!
COMP::

@ena
@compile/compile mtrmac
@compile/compile mountr
@compile/compile mtrcfs
@compile/compile mtrusr
@compile/compile mtrddb
!@get sys:macro
!@i ver
!@st
!*mtrmac,mtrmac/c=mtrmac
!*mountr,mountr/c=mountr
!*mtrcfs,mtrcfs/c=mtrcfs
!*mtrusr,mtrusr/c=mtrusr
!*MTRDDB,MTRDDB/C=MTRDDB
!*^c
!
!DOCREF::
!@RUN SYS:CREF
!*CRF:MOUNTR.LST=MOUNTR.CRF
!
END::
@get sys:link.exe
@st
*mountr,mtrcfs,mtrusr,MTRDDB
*mountr/save
*/go
*^c
!
@get mountr.exe
@INFORMATION VERSION
!
@VDIR MOUNTR.EXE,
@CHECKSUM SEQ
@
!
@DELETE MOUNTR.REL,mtrcfs.rel,mtrusr.rel,MTRDDB.REL

@PLEASE	MOUNTR Assembly Successful

NOERROR

@MODIFY BATCH GALAXY/DEP:-1
!
%CERR::
%ERR::
@PLEASE	Error During MOUNTR Assembly


%FIN::
