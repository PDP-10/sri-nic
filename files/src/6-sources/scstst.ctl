!
! Name: SCSTST.CTL
! Date: 3-Sep-84
!
!This control file is provided for information purposes only.  The
!purpose of the file is to document the procedures used to build
!the distributed software.  It is unlikely that this control file
!will be able to be submitted without modification on consumer
!systems.  Particular attention should be given to ersatz devices
!and structure names, PPN's, and other such parameters.  Submit
!times may vary depending on system configuration and load.  The
!availability of sufficient disk space and core is mandatory.
!
! Function:	This control file builds SCSTST from its basic
!		source. 
!
!
! Submit with the switch "/TAG:CREF" to obtain
!   .CRF listings of the source files
!
!
@DEF FOO: NUL:
@GOTO A
!
CREF:: @DEF FOO: DSK:
!
!
A::
@TAKE BATCH.CMD
!
@NOERROR
!
@INFORMATION LOGICAL-NAMES ALL
!
! Take a checksummed directory of all the input files
!
@VDIRECT SYS:MACRO.EXE,SYS:CREF.EXE,SCSTST.MAC,
@CHECKSUM SEQ
@
@VDIRECT SYS:CMD.REL,SYS:CMD.UNV,SYS:MONSYM.UNV,SYS:MACSYM.UNV,SYS:MACREL.REL,SYS:PA1050.EXE,
@CHECKSUM SEQ
@
@
@RUN SYS:MACRO
@INFORMATION VERSION
@GET SYS:LINK
@INFORMATION VERSION
@GET SYS:CREF
@INFORMATION VERSION
!
@DEL SCSTST.REL
!
@COMPILE/COMPILE/CREF SCSTST.MAC
!
@R CREF
*FOO:SCSTST.LST=SCSTST.CRF
!
@R LINK
*SCSTST
*SCSTST/SAVE/GO
!
@GET SCSTST.EXE
@INFO VERSION
!
@VDIRECT SCSTST.EXE,
@CHECKSUM SEQ
@
![End SCSTST.CTL]
