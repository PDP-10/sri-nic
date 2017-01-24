!
! NAME: ENVIRONMENT.CTL
! DATE: 1-APR-82
!
!THIS CONTROL FILE IS PROVIDED FOR INFORMATION PURPOSES ONLY.  THE
!PURPOSE OF THE FILE IS TO DOCUMENT THE PROCEDURES USED TO BUILD
!THE DISTRIBUTED SOFTWARE.  IT IS UNLIKELY THAT THIS CONTROL FILE
!WILL BE ABLE TO BE SUBMITTED WITHOUT MODIFICATION ON CUSTOMER
!SYSTEMS.
!
! FUNCTION:	THIS CONTROL FILE BUILDS ENVIRONMENT FROM ITS BLISS-36
!		SOURCE.  NOTE THAT BLISS-36 MUST BE INSTALLED, AND MONSYM.L36
!		MUST BE PRESENT ON BLI:, FOR THE BUILD TO SUCCEED.
!		THE FILES CREATED BY THIS JOB ARE:
!				ENVIRONMENT.EXE
!
@INFORMATION LOGICAL-NAMES ALL
!
! TAKE A CHECKSUMMED DIRECTORY OF ALL THE INPUT FILES
!
@VDIRECT SYS:BLISS.EXE,SYS:LINK.EXE,ENVIRONMENT.BLI,BLI:MONSYM.L36,
@CHECKSUM SEQ
@
@GET SYS:LINK
@INFORMATION VERSION
@GET SYS:BLISS
@INFORMATION VERSION
!
@ST
*ENVIRONMENT.BLI/OBJECT:ENVIRO.REL/EXIT
!
@LOAD ENVIRO
!
@SAVE ENVIRONMENT.EXE
!
@DIRECT ENVIRONMENT.EXE,
@CHECKSUM SEQ
@
!
@DELETE ENVIRO.REL
