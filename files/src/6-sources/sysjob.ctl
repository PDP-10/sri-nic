!
! NAME: SYSJOB.CTL
! DATE: 5-APR-76
!
!THIS CONTROL FILE IS PROVIDED FOR INFORMATION PURPOSES ONLY.  THE
!PURPOSE OF THE FILE IS TO DOCUMENT THE PROCEDURES USED TO BUILD
!THE DISTRIBUTED SOFTWARE.  IT IS UNLIKELY THAT THIS CONTROL FILE
!WILL BE ABLE TO BE SUBMITTED WITHOUT MODIFICATION ON CONSUMER
!SYSTEMS.  PARTICULAR ATTENTION SHOULD BE GIVEN TO ERSATZ DEVICES
!AND STRUCTURE NAMES, PPN'S, AND OTHER SUCH PARAMETERS.  SUBMIT
!TIMES MAY VARY DEPENDING ON SYSTEM CONFIGURATION AND LOAD. THE
!AVAILABILITY OF SUFFICIENT DISK SPACE AND CORE IS MANDATORY.
!
! FUNCTION:	THIS CONTROL FILE BUILDS SYSJOB FROM ITS BASIC
!		SOURCES.  THE FILES CREATED WILL BE:
!				SYSJOB.EXE
!
! SUBMIT WITH THE SWITCH "/TAG:CREF" TO OBTAIN
!   A .CRF LISTING OF THE SOURCE FILE
!
!
@DEF FOO: NUL:
@GOTO A
!
CREF:: @DEF FOO: DSK:
!
!
A::
!
@INFORMATION LOGICAL-NAMES ALL
!
! TAKE A CHECKSUMMED DIRECTORY OF ALL THE INPUT FILES
!
@VDIRECT SYS:MACRO.EXE,SYS:LINK.EXE,SYSJOB.MAC,
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
@COMPILE /CREF SYSJOB.MAC
!
@R CREF
*FOO:SYSJOB.LST=SYSJOB.CRF
!
!
@LOAD SYSJOB
!
@SAVE 6-SYSJOB
@INFORMATION VERSION
!
@VDIRECT 6-SYSJOB.EXE,
@CHECKSUM SEQ
@
!
@DELETE SYSJOB.REL
!
!
