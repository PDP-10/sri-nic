!
! NAME: WATCH.CTL
! DATE: 13-AUG-79
!
!
!THIS CONTROL FILE IS PROVIDED FOR INFORMATION PURPOSES ONLY.
!THE PURPOSE OF THE FILE  IS TO DOCUMENT THE PROCEDURES  USED
!TO BUILD THE DISTRIBUTED SOFTWARE.  IT IS UNLIKELY THAT THIS
!CONTROL  FILE  WILL   BE  ABLE  TO   BE  SUBMITTED   WITHOUT
!MODIFICATION  ON  CONSUMER  SYSTEMS.   PARTICULAR  ATTENTION
!SHOULD BE  GIVEN  TO  ERSATZ DEVICES  AND  STRUCTURE  NAMES,
!PPN'S, AND  OTHER SUCH  PARAMETERS.  SUBMIT  TIMES MAY  VARY
!DEPENDING   ON   SYSTEM   CONFIGURATION   AND   LOAD.    THE
!AVAILABILITY OF SUFFICIENT DISK SPACE AND CORE IS MANDATORY.
!
!
! FUNCTION:	THIS CONTROL FILE BUILDS WATCH FROM ITS BASIC 
!		SOURCES.   THE FILES CREATED BY THIS JOB ARE:
!				WATCH.EXE
!
! SUBMIT WITH THE SWITCH "/TAG:CREF" TO OBTAIN A CREF LISTING
! OF THE SOURCE FILE
!
@DEFINE OUT: NUL:
@GOTO SKPCRF
!
CREF::
@DEFINE OUT: DSK:
!
!
SKPCRF::
!
@TAK BATCH.CMD
!
@INFORMATION LOGICAL-NAMES ALL
!
! TAKE A CHECKSUMMED DIRECTORY OF ALL THE INPUT FILES
!
@VDIRECTORY SYS:MACRO.EXE,SYS:LINK.EXE,SYS:CREF.EXE,
@CHECKSUM SEQUENTIALLY
@
@
@VDIRECTORY SYS:MONSYM.UNV,SYS:MACSYM.UNV,SYS:MACREL.REL,SYS:PA1050.EXE,
@CHECKSUM SEQ
@
@
@VDIRECTORY SYMSUB.MAC,PROLOG.UNV,
@CHECKSUM SEQUENTIALLY
@
@
@VDIRECTORY WATCH.MAC,WATMAI.MAC,WATDAT.MAC,
@CHECKSUM SEQUENTIALLY
@
@
@RUN SYS:MACRO
@INFORMATION VERSION
@GET SYS:LINK
@INFORMATION VERSION
@GET SYS:CREF
@INFORMATION VERSION
!
@COMPILE /COMPILE SYMSUB.MAC
@COMPILE /COMPILE /CREF WATCH.MAC
@COMPILE /COMPILE /CREF WATMAI.MAC
@COMPILE /COMPILE /CREF WATDAT.MAC
!
@R CREF
*OUT:WATCH.LST=WATCH.CRF
*OUT:WATMAI.LST=WATMAI.CRF
*OUT:WATDAT.LST=WATDAT.CRF
!
@LOAD WATCH,WATMAI,WATDAT
!
@SAVE WATCH
@INFORMATION VERSION
!
@DIRECTORY WATCH.EXE,
@CHECKSUM SEQUENTIALLY
@
!
@DELETE WATCH.REL,WATMAI.REL,WATDAT.REL,SYMSUB.REL
@DEFINE OUT:
