!			Build the MM mailsystem
!			 Mark Crispin 27-Aug-85
!
!  This control file builds the MM mailsystem.  It should be possible to
! submit this control file and run the resulting EXE files without
! modification on any site.  Sites with additional or modified terminal
! types may wish to customize the BLANKT.MAC file for their environment
! prior to submitting this control file.
!
!  Required directories: <MM> for the sources, <MM.BINARIES> for the
! resulting binaries.
!
!  This control file explicitly does NOT install the MM mailsystem; all
! the resulting binaries are left on <MM.BINARIES>
!
!			*** EXEC BUG ***
!
!  A bug introduced in the 5.1 EXEC AP6 tape (and as of this writing in
! the 6 EXEC) will prevent this control file from running.  The problem
! is with the %"NOINITIAL" specification used in the LOAD commands herein.
! To patch around this problem, patch location CMIND+16 from JRST CMPER
! to NOP in SYSTEM:EXEC.EXE.
!
!
@ENABLE
@CONNECT SRC:<MM-NEW.BINARIES>
@DEFINE DSK: DSK:,SRC:<MM-NEW>
!
! Clean up
@DELETE *.*
@EXPUNGE
!
! Log the environment this job ran under
@VDIRECTORY SRC:<MM-NEW>,
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
! is for TOPS-20 release 5.3 (a TCP/IP version of 5.1).  Note that MM may
! not assemble with the 5.4 or 6.x versions of MACSYM and MONSYM due to a
! MACRO bug which gets a MCRNEC error.  This is being worked on.
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
! WAKEUP -- MMailr wakeup module for mail composers
@COMPILE ARMAIL,BLANKT,HSTNAM,RELAY,SNDMSG,WAKEUP
!
!**********************************************************************
!		Mailsystem
!**********************************************************************
! MM -- mail composition/retrieval subsystem
@COMPILE FSCOPY,MMHELP,MMUUO
@LOAD %"NOINITIAL" MM
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
!
! MMAILBOX -- mailbox lookup tool
@LOAD %"NOINITIAL" MMLBX
@RESET		!work around release 6 LINK bug
@RENAME MMLBX.EXE MMAILBOX.EXE
!
! MAISER -- SMTP protocol server
@LOAD %"NOINITIAL" MAISER
!
! SMTPSV -- Internet SMTP listener
@LOAD %"NOINITIAL" SMTPSV
!
! SMTJFN -- JFN-based Internet SMTP listener
@LOAD %"NOINITIAL" SMTJFN
!
! DMASER -- DECnet SMTP protocol listener/server
@LOAD %"NOINITIAL" DMASER
!
! CAFARD - TTY line mail transfer program
@COMPILE CAFPRO,CAFDTR
@LOAD %"NOINITIAL" CAFARD
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
!  The following parts of the MM built procedure may not work on a site
! which does not have EMACS and/or C properly installed.
@NOERROR
!
! MMSTAT -- Statistics from log file
@CC MMSTAT.C
!
@CONNECT SRC:<MM-NEW>
!
! MMAIL -- Support routines for fancy MM/EMACS interface
@EMACS
*
*XLOAD LIBRARYPURIFY
*XGENERATE LIBRARYSRC:<MM-NEW.BINARIES>MMAIL<MM-NEW>MMAIL
*
@DELETE MMAIL.COMPRS
@EXPUNGE
!
@ERROR
!
! [End of BUILD-MM.CTL]
