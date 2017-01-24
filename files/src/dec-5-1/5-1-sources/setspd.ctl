!
! NAME: SETSPD.CTL
! DATE: 5-APR-76
!
!THIS CONTROL FILE IS PROVIDED FOR INFORMATION PURPOSES ONLY.  THE
!PURPOSE OF THE FILE IS TO DOCUMENT THE PROCEDURES USED TO BUILD
!THE DISTRIBUTED SOFTWARE.  IT IS UNLIKELY THAT THIS CONTROL FILE
!WILL BE ABLE TO BE SUBMITTED WITHOUT MODIFICATION ON CONSUMER
!SYSTEMS.  PARTICULAR ATTENTION SHOULD BE GIVEN TO ERSATZ DEVICES
!AND STRUCTURE NAMES, PPN'S, AND OTHER SUCH PARAMETERS.  SUBMIT
!TIMES MAY VARY DEPENDING ON SYSTEM CONFIGURATION AND LOAD.  THE
!AVAILABILITY OF SUFFICIENT DISK SPACE AND CORE IS MANDATORY.
!
! FUNCTION:	THIS CONTROL FILE BUILDS SETSPD FROM ITS BASIC 
!		SOURCES.  THE FILES CREATED BY THIS JOB ARE:
!				SETSPD.EXE
!
! SUBMIT WITH THE SWITCH "/TAG:CREF" TO OBTAIN
!   A .CRF LISTING OF THE SOURCE FILE
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
@INFORMATION LOGICAL-NAMES ALL
!
! TAKE A CHECKSUMMED DIRECTORY OF ALL THE INPUT FILES
!
@VDIRECT SYS:MACRO.EXE,SYS:LINK.EXE,SYS:CREF.EXE,SETSPD.MAC,
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
@COMPILE /COMPILE/CREF SETSPD.MAC
!
@R CREF
*FOO:SETSPD.LST=SETSPD.CRF
!
@LOAD SETSPD
!
@SAVE 5-1-SETSPD
@INFORMATION VERSION
!
@VDIRECT 5-1-SETSPD.EXE,
@CHECKSUM SEQ
@
!
@DELETE SETSPD.REL