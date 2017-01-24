!
! Name: TELNET.CTL
! Date: 11-Feb-83
!
!This  control  file  is  provided  for  information purposes only. The
!purpose of the file is to document the procedures used  to  build  the
!distributed  software.  It  is unlikely that this control file will be
!able  to  be  submitted  without  modification  on  consumer  systems.
!Particular attention should be given to ersatz devices  and  structure
!names,  ppn's,  and  other  such  parameters.  Submit  times  may vary
!depending on system configuration and load.
!
!Function:  This control file builds telnet from its basic sources. The
!file created by this job is: TELNET.EXE
!
!submit  with  the  switch  "/TAG:CREF" to obtain a .CRF listing of the
!source file.
!
@DEF FOO: NUL:
@GOTO A
!
CREF:: @DEF FOO: DSK:
!
A::
@TAK BATCH.CMD
@I LOG SYS:
!
! TAKE A CHECKSUMMED DIRECTORY OF ALL THE INPUT FILES
!
@VDIRECT SYS:MACRO.EXE,SYS:LINK.EXE,SYS:CREF.EXE,TELNET.MAC,HSTNAM.MAC,
@CHECKSUM SEQ
@
@VDIRECT SYS:MONSYM.UNV,SYS:MACSYM.UNV,SYS:MACREL.REL,SYS:PA1050.EXE,
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
@NOERROR
@COMPILE /COMPILE /CREF TELNET.MAC,HSTNAM.MAC
!
@R CREF
*FOO:TELNET.LST=TELNET.CRF
*FOO:HSTNAM.LST=HSTNAM.CRF
!
@LOAD TELNET.MAC
!
@INFORMATION VERSION
!
@DIRECT TELNET.EXE,
@CHECKSUM SEQ
@
!
@DELETE TELNET.REL,HSTNAM.REL
