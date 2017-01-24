!			Build the MM mailsystem
!			 Mark Crispin 26-Apr-84
!
!  This control file builds the MM mailsystem.  It should be possible to
! submit this control file and run the resulting EXE files without
! modification on any site.  Sites with additional or modified terminal
! types may wish to customize the BLANKT.MAC file for their environment
! prior to submitting this control file.
!
!  This control file explicitly does NOT install the MM mailsystem; all
! the resulting EXE's are left on the connected directory.
!
!			*** EXEC BUG ***
!
!  A bug introduced in the 5.1 EXEC AP6 tape (and as of this writing in
! the 6 EXEC) will prevent this control file from running.  The problem
! is with the %"/NOINITIAL" specification used in the LOAD commands herein.
! To patch around this problem, patch location CMIND+16 from JRST CMPER
! to NOP in SYSTEM:EXEC.EXE.
!
!
! Clean up first
@DELETE *.REL,*.UNV
@EXPUNGE
!
! Log the environment this job ran under
@VDIRECTORY,
@CHECKSUM SEQUENTIALLY
@
@GET SYS:MACRO
@INFORMATION VERSION
@GET SYS:LINK
@INFORMATION VERSION
!
! Universals
!
! *** Begin use of private system universals ***
!
!  The reason we have private copies of the system universals is that
! experience has demonstrated that we cannot count on the proper versions
! of the various system universals existing on the local system.  In fact,
! as of this writing (7/7/83) it is extremely unlikely that the correct
! versions of MACSYM and MONSYM will be installed.  MACSYM has a critical
! bugfix in the DO./ENDDO. construct which many sites do not have.  MONSYM
! is for TOPS-20 release 5.3 (the TCP/IP version of 5.1).
!
! JOBDAT -- Low core symbol definitions
! MACSYM -- System macro library with required bugfix
! MONSYM -- System symbols for TOPS-20 release 5.3
@COPY TTY: DSK:UNV.MAC
@%..UNV==1
@^Z
@COMPILE /NOBINARY UNV.MAC+JOBDAT.MAC
@DELETE UNV.MAC
@EXPUNGE
@COMPILE /NOBINARY MACSYM
@COMPILE /NOBINARY MONSYM
!
! Use our copies of the universals instead of the system ones
@DEFINE SYS: DSK:,SYS:
! *** End use of private system universals ***
!
! SNDDEF -- Definitions for the sendsystem
@COMPILE /NOBINARY SNDDEF
!
!**********************************************************************
!		Libraries
!**********************************************************************
! BLANKT -- site-dependent terminal blanking codes
! HSTNAM -- host name lookup and domain processing
! RELAY -- relay host name lookup and domain processing
! SNDMSG -- terminal sending routines
@COMPILE BLANKT,HSTNAM,RELAY,SNDMSG
!
!**********************************************************************
!		Mailsystem
!**********************************************************************
! MM -- mail composition/retrieval subsystem
!  MMDEC is a version of MM which uses DEC's IPCF mailer for local
! delivery.  Its usage is not encouraged, and it is not supported
@COMPILE MMUUO
@LOAD %"/NOINITIAL" @MMDEC
@RENAME MM.EXE MMDEC.EXE
@LOAD %"/NOINITIAL" @MM
!
! GRIPE -- tool for sending bug reports, etc.
@LOAD %"/NOINITIAL" GRIPE
!
! MSTAT - tool for looking at queued messages
@LOAD %"/NOINITIAL" MSTAT
!
! MAILST - system server for MSTAT requests
@LOAD %"/NOINITIAL" MAILST
!
! MMAILR -- mail delivery process
@LOAD %"/NOINITIAL" MMAILR
!
! MMAILBOX -- mailbox lookup tool
@LOAD %"/NOINITIAL" MMLBX
@RENAME MMLBX.EXE MMAILBOX.EXE
!
! MAISER -- SMTP protocol server
@LOAD %"/NOINITIAL" MAISER
!
! SMTPSV -- Internet SMTP listener
@LOAD %"/NOINITIAL" SMTPSV
!
! DMASER -- DECnet SMTP protocol listener/server
@LOAD %"/NOINITIAL" DMASER
!
!**********************************************************************
!		Sendsystem
!**********************************************************************
! SEND -- Send composition process
@LOAD %"/NOINITIAL" SEND
!
! SNDSRV -- Send delivery process
@LOAD %"/NOINITIAL" SNDSRV
!
! REPLY -- Reply to latest send
@LOAD %"/NOINITIAL" REPLY
!
! WHAT -- Report previous sends
@LOAD %"/NOINITIAL" WHAT
!
!**********************************************************************
!		Auxillary tools
!**********************************************************************
! HSTTST -- host name registry lookup tool
@LOAD %"/NOINITIAL" HSTTST
!
! QDMAIL -- mail queue perusal tool
@LOAD %"/NOINITIAL" QDMAIL
!
! SNDSTAT -- Get sending statistics from SNDSRV
@LOAD %"/NOINITIAL" SNDSTA
!
! [End of BUILD-MM.CTL]
