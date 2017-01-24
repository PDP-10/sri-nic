!GALGEN.CTL



!This Control File Assembles, Loads, and saves GALGEN

!System Files

!	MONSYM.UNV

!Required Files in Your Area

!	GLXMAC.UNV	Library Macro and Symbol Definitions
!	GLXLIB.REL	Library Load Module
!	OPRPAR.REL	Parser routines

!Source Files

!	GALGEN.MAC

!Output Files

!	GALGEN.EXE


@DEF REL: SYS:,REL:

@VDIR	GALGEN.MAC

.DIRECT	GALGEN.MAC
COMP::
@COMPILE/COMPILE GALGEN.MAC

@R LINK
*/LOCALS/SYMSEG:LOW =GLXLIB/EXCLUDE:GLXINI,GALGEN/GO
@SAVE GALGEN

@VDIRECT GALGEN.EXE,
@CHECK
@SE
@

@MODIFY BATCH GALAXY/DEP:-1



