!
! Name: MMAILR.CTL
! Date:  4-Mar-88
!
!This  control  file  is  provided  for  information purposes only. The
!purpose of the file is to document the procedures used  to  build  the
!distributed  software.  It  is unlikely that this control file will be
!able  to  be  submitted  without  modification  on  consumer  systems.
!Particular attention should be given to ersatz devices  and  structure
!names,  ppn's,  and  other  such  parameters.  Submit  times  may vary
!depending on system configuration and load.
!
!Function:  This control file builds MMAILR from its basic sources. The
!files created by this job are: MMAILR.EXE, MAISER.EXE, SMTPSV.EXE, and
!MMAILBOX.EXE


! TAKE A CHECKSUMMED DIRECTORY OF ALL THE INPUT FILES

@TAK BATCH.CMD

@VDIRECT SYS:MACRO.EXE,SYS:LINK.EXE,
@CHECKSUM SEQ
@
@
@VDIRECT ARMAIL.MAC,SNDDEF.MAC,HSTNAM.MAC,RELAY.MAC,SNDMSG.MAC,MMAILR.MAC,MAISER.MAC,BLANKT.MAC,WAKEUP.MAC,
@CHECKSUM SEQ
@
@
@VDIRECT SMTPSV.MAC,MMLBX.MAC,SNDDEF.MAC,
@CHECKSUM SEQ
@
@
@VDIRECT SYS:MONSYM.UNV,SYS:MACSYM.UNV,SYS:MACREL.REL,SYS:PA1050.EXE,
@CHECKSUM SEQ
@
@
@GET SYS:MACRO
@INFORMATION VERSION
@GET SYS:LINK
@INFORMATION VERSION
@GET SYS:CREF
@INFORMATION VERSION

@NOERROR

@COMPILE /NOBINARY SNDDEF

!
!**********************************************************************
!		Libraries
!**********************************************************************
! ARMAIL -- Specially modified version of DEC's ARMAIL module
! BLANKT -- Site-dependent terminal blanking codes
! HSTNAM -- Host name lookup and domain processing
! RELAY -- Relay host name lookup and domain processing
! SNDMSG -- Terminal sending routines
! WAKEUP -- MMAILR wakeup module for mail composers
@COMPILE BLANKT,HSTNAM,RELAY,SNDMSG,WAKEUP,ARMAIL

! MMAILBOX -- mailbox lookup tool
@LOAD %"NOINITIAL" MMLBX
@RESET
@RENAME MMLBX.EXE MMAILBOX.EXE

! MAISER -- SMTP protocol server
@LOAD %"NOINITIAL" MAISER

! SMTJFN -- Internet SMTP listener
@LOAD %"NOINITIAL" SMTJFN

! MMAILR -- mail delivery process
@LOAD %"NOINITIAL" MMAILR
