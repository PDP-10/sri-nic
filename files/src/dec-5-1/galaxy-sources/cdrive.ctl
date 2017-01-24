!CDRIVE.CTL
!This Control File Assembles, Loads, and saves CDRIVE

!System Files

!	ACTSYM.UNV
!	MONSYM.UNV	T20 only

!Required Files in Your Area

!	GLXMAC.UNV	Library Macro and Symbol Definitions
!	GLXLIB.REL
!	QSRMAC.UNV
!	ORNMAC.UNV
!	D60JSY.UNV	IBMCOM file
!	D60JSY.REL	IBMCOM file
!	NURD.REL	T20 only

!Source Files

!	CDRIVE.MAC

!Output Files

!	CDRIVE.EXE

!  Set up logical names and such

.AS DSK REL
.SET WAT VER
@DEF REL: DSK:
@DEF UNV: DSK:,UNV:
@VDIR	CDRIVE.MAC

.DIRECT	CDRIVE.MAC

COMP::
@
.
COMPILE/COMPILE CDRIVE.MAC

@
.
R LINK
*/LOCAL/SYMSEG:LOW=CDRIVE,D60JSY/GO

@
.
SAVE CDRIVE

FINI::
@VDIRECT CDRIVE.EXE
.DIRECT/CHECK CDRIVE.EXE

@
.
PLEASE	CDRIVE Assembly Successful
NOERROR
@MODIFY BATCH GALAXY/DEP:-1
.SUB GALAXY=/MOD/DEP:-1

%CERR::
%ERR::
@
.
PLEASE	Error During CDRIVE Assembly
%FIN::
