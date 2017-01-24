!			Build the MM mailsystem
!			 Mark Crispin 8-Mar-88
!
!  This control file builds the MM mailsystem.  It should be possible to
! submit this control file and run the resulting EXE files without
! modification on any site.  Sites with additional or modified terminal
! type may wish to install a custom version of BLANKT.MAC for their
! environment on the <MM.LOCAL> directory prior to submitting this
! control file.
!
!  Required directories:
!	<MM> for the sources
!	<MM.LOCAL> for locally-modified sources
!	<MM.BINARIES> for the resulting binaries
!
!  This control file explicitly does NOT install the MM mailsystem; all
! the resulting binaries are left on <MM.BINARIES>
!
! Clean up
@CONNECT <MM.BINARIES>
@DELETE *.*
@EXPUNGE
!
! Define local search path
@DEFINE DSK: DSK:,<MM.LOCAL>,<MM>
!
! Log the environment this job ran under
@VDIRECTORY <MM>,<MM.LOCAL>,
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
! if your site is still running 4.1 or 5.1 you won't be able to build MM
! with your versions of MACSYM and MONSYM.
!  The MACSYM on this tape is DEC 6.1, and fixes a serious bug in DO. that
! earlier (e.g. 5.1) MACSYM's have.  The MONSYM on this tape is a modified
! version of DEC 6.1 with the .ERCOD macro purged so MM can assemble.
!
! MACSYM -- System macro library with required bugfix
! MONSYM -- TOPS-20 system symbols
@COMPILE /NOBINARY MACSYM
@COPY TTY: DSK:REL1.MAC
@REL==1
@^Z
@COMPILE /COMPILE REL1.MAC+MACSYM.MAC MACREL
@DELETE REL1.MAC
@EXPUNGE
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
! ARMAIL -- Replacement for DEC's ARMAIL module
! BLANKT -- site-dependent terminal blanking codes
! HSTNAM -- host name lookup and domain processing
! RELAY -- relay host name lookup and domain processing
! SNDMSG -- terminal sending routines
! WAKEUP -- MMAILR wakeup module for mail composers
@COMPILE ARMAIL,BLANKT,HSTNAM,RELAY,SNDMSG,WAKEUP
!
!**********************************************************************
!		Mailsystem
!**********************************************************************
! MM -- mail composition/retrieval subsystem
@COMPILE FSCOPY,MMHELP,MMUUO
@LOAD %"NOINITIAL" MM
@GET MM
@DEPOSIT $GTMOK -1
@SAVE MM
!
! GRIPE -- tool for sending bug reports, etc.
@LOAD %"NOINITIAL" GRIPE
!
! MSTAT - tool for looking at queued messages
@LOAD %"NOINITIAL" MSTAT
!
! MAILST - system server for MSTAT requests
@LOAD %"NOINITIAL" MAILST
!
! MMAILR -- mail delivery process
@LOAD %"NOINITIAL" MMAILR
@GET MMAILR
@DEPOSIT $GTMOK -1
@SAVE MMAILR
!
! MMAILBOX -- mailbox lookup tool
@LOAD %"NOINITIAL" MMLBX
@RESET		!work around LINK bug
@RENAME MMLBX.EXE MMAILBOX.EXE
@GET MMAILBOX
@DEPOSIT $GTMOK -1
@SAVE MMAILBOX
!
! MAISER -- SMTP protocol server
@LOAD %"NOINITIAL" MAISER
!
! SMTJFN -- Internet SMTP listener
@LOAD %"NOINITIAL" SMTJFN
!
! SMTDCN -- DECnet SMTP listener
@LOAD %"NOINITIAL" SMTDCN
!
! VMAIL - DECnet MAIL-11 (VAX/VMS) listener/server
@LOAD %"NOINITIAL" VMAIL
!
! CAFARD - TTY line mail transfer program
@COMPILE CAFPRO,CAFDTR
@LOAD %"NOINITIAL" CAFARD
!
! MAPSER - IMAP II mail access protocol server
@COMPILE MAPSER
@NOERROR	!Don't die on a model A CPU
@LOAD %"NOINITIAL" MAPSER
@ERROR		!No EXE file will be made if built on model A CPU
!
! IMAPSV - Internet IMAP II listener
@LOAD %"NOINITIAL" IMAPSV
!
!**********************************************************************
!		Sendsystem
!**********************************************************************
! SEND -- Send composition process
@LOAD %"NOINITIAL" SEND
!
! SNDSRV -- Send delivery process
@LOAD %"NOINITIAL" SNDSRV
!
! REPLY -- Reply to latest send
@LOAD %"NOINITIAL" REPLY
!
! WHAT -- Report previous sends
@LOAD %"NOINITIAL" WHAT
!
!**********************************************************************
!		Auxillary tools
!**********************************************************************
! HSTTST -- host name registry lookup tool
@LOAD %"NOINITIAL" HSTTST
!
! QDMAIL -- mail queue perusal tool
@LOAD %"NOINITIAL" QDMAIL
!
! SNDSTAT -- Get sending statistics from SNDSRV
@LOAD %"NOINITIAL" SNDSTA
!
@NOERROR	!In case KCC or EMACS not installed
!
! MMSTAT -- Statistics from log file
@CC MMSTAT.C
!
! MLIST - Mailing list support tool
@CC MLIST.C
!
! MMAIL -- Support routines for fancy MM/EMACS interface
! MM-MAIL -- Auxillary EMACS routines
@CONNECT <MM>
@EMACS
*GLASS
*XSet Terminal TypeGLASS
*XFind FileMMAIL.EMACS
*XCompile
*XFind FileMM-MAIL.EMACS
*XCompile
*
@RENAME *.%EJ <MM.BINARIES>
!
@ERROR
@CONNECT <MM.BINARIES>
@EXPUNGE
!
! [End of BUILD-MM.CTL]
