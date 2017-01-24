!OPERATOR.CTL	15-Apr-80
!
!
!                  COPYRIGHT (c) 1979,1980,1981,1982
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

!This Control File Assembles, Loads, and saves ORION and OPR

!System Files

!	MONSYM.UNV

!Required Files in Your Area

!	GLXMAC.UNV	Library Macro and Symbol Definitions
!	GLXLIB.REL	Library Load Module
!	QSRMAC.UNV
!	ACTSYM.UNV
!	ARMAIL.REL
!	NCPTAB.REL

!Source Files

!  For ORION:
!	ORNMAC.MAC
!	ORION.MAC
!	OPRQSR.MAC
!	OPRPAR.MAC
!	OPRLOG.MAC
!	OPRNET.MAC
!	OPRERR.MAC
!	OPRCMD.MAC

!  For OPR:
!	OPR.MAC
!	OPRPAR.MAC
!	OPRCMD.MAC

!Output Files

!	ORION.EXE
!	OPR.EXE
!	ORNMAC.UNV
!	OPRPAR.REL

!Documetation Files

!	OPR.HLP

.ASS DSK REL
.SET WATCH VERSION
@DEFINE REL: DSK:
@DEF UNV: DSK:,PS:<UNIVERSALS>
@DEF DSK: DSK:,SYS:
@VDIR	OPR.MAC
@VDIR	OPRPAR.MAC
@VDIR	OPRCMD.MAC

.DIRECT	OPR.MAC
.DIRECT	OPRPAR.MAC
.DIRECT	OPRCMD.MAC


@VDIR	ORNMAC.MAC
@VDIR	ORION.MAC
@VDIR	OPRQSR.MAC
@VDIR	OPRPAR.MAC
@VDIR	OPRLOG.MAC
@VDIR	OPRNET.MAC
@VDIR	OPRERR.MAC
@VDIR	OPRCMD.MAC


.DIRECT	ORNMAC.MAC
.DIRECT	ORION.MAC
.DIRECT	OPRQSR.MAC
.DIRECT	OPRPAR.MAC
.DIRECT	OPRLOG.MAC
.DIRECT	OPRNET.MAC
.DIRECT	OPRERR.MAC
.DIRECT	OPRCMD.MAC

COMP::
@
.
COMPILE/COMPILE ORNMAC.MAC
COMPILE/COMPILE OPRPAR.MAC
COMPILE/COMPILE ORION.MAC
COMPILE/COMPILE OPRCMD.MAC
COMPILE/COMPILE OPRQSR.MAC
COMPILE/COMPILE OPRLOG.MAC
COMPILE/COMPILE OPRNET.MAC
COMPILE/COMPILE OPRERR.MAC
COMPILE/COMPILE OPR.MAC

LOAD::
@
.
R LINK
*/SYMSEG:LOW/SEGMENT:LOW =ORION,OPRQSR,OPRPAR,OPRLOG,OPRNET,OPRERR,ARMAIL/GO
SAVE ORION

@
.
R LINK
*/SYMSEG:LOW/SEGMENT:LOW =OPR,OPRPAR,OPRCMD/GO
SAVE OPR

@VDIRECT ORION.EXE,OPR.EXE,ORNMAC.UNV,OPRPAR.REL
.DIRECT ORION.EXE,OPR.EXE,ORNMAC.UNV,OPRPAR.REL

@
.
PLEASE	ORION and OPR Assembly Successful
NOERROR
@MODIFY BATCH GALAXY/DEP:-1
.SUBMIT GALAXY=/MOD/DEP:-1

%CERR::
%ERR::
@
.
PLEASE	Error During OPR Assembly
%FIN::