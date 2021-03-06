;<6.MONITOR>BOOT.CTL.9, 20-Jun-84 17:07:30, EDIT BY LEACHE
;Check boot version number
;<6.MONITOR>BOOT.CTL.8, 15-Mar-84 13:50:38, Edit by PURRETTA
! UPD ID= 2418, SNARK:<6.MONITOR>BOOT.CTL.7,   4-May-83 10:05:13 by HAUDEL
;Delete the commands for making SMBOOT.EXE and SMMTBT.EXE
;KLIPA and KLINI Microcode had been deleted before this edit.
! UPD ID= 2007, SNARK:<6.MONITOR>BOOT.CTL.5,  16-Mar-83 05:47:24 by WACHS
;Add KLIPA, KLNI Microcode
;<6.MONITOR>BOOT.CTL.4, 17-Nov-82 21:06:44, Edit by PAETZOLD
;Take BATCH.CMD
! UPD ID= 1354, SNARK:<6.MONITOR>BOOT.CTL.3,  19-Oct-82 11:10:49 by HAUDEL
;Change KLPRE.MAC to SYSFLG.MAC
! UPD ID= 1966, SNARK:<5.MONITOR>BOOT.CTL.2,   8-May-81 10:40:10 by LEACHE
;Add some labels
! UPD ID= 1493, SNARK:<5.MONITOR>BOOT.CTL.3,  26-Jan-81 12:10:16 by LEACHE
;Allow for selective inclusion of RP20 microcode
;<4.MONITOR>BOOT.CTL.12,  6-Jul-79 10:04:25, EDIT BY ENGEL
;MOVE BOOT TO 40000
;<4.MONITOR>BOOT.CTL.11, 22-Jun-79 14:38:19, EDIT BY ENGEL
;MOVE BOOT FROM 3000 TO 20000
;<4.MONITOR>BOOT.CTL.10, 15-Jan-79 10:33:00, EDIT BY GILBERT
;TCO 4.2155: Delete KBBOOT and KBMTBT.
;<3A.MONITOR>BOOT.CTL.11, 26-Jul-78 10:50:39, Edit by ENGEL
;<3A.MONITOR>BOOT.CTL.10, 23-Jun-78 15:29:14, Edit by ENGEL
;<3A.MONITOR>BOOT.CTL.9, 23-Jun-78 15:23:48, Edit by ENGEL
;<3A.MONITOR>BOOT.CTL.8, 22-Jun-78 09:59:05, Edit by FORTMILLER
;<3A-NEW>BOOT.CTL.7, 25-May-78 13:33:15, Edit by FORTMILLER
;<3.SM10-RELEASE-3>BOOT.CTL.6,  9-Apr-78 16:19:44, Edit by MCLEAN
;<3.SM10-RELEASE-3>BOOT.CTL.5,  9-Apr-78 16:18:46, Edit by MCLEAN
;<3.SM10-RELEASE-3>BOOT.CTL.4,  9-Apr-78 16:16:33, Edit by MCLEAN
! NAME: BOOT.CTL
! DATE: 11-OCT-77
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
! FUNCTION:	THIS CONTROL FILE BUILDS BOOT FROM ITS BASIC
!		SOURCES.  THE FILES CREATED BY THIS JOB ARE:
!
!			  BOOT.EXE	;KL DISK BOOT
!			      .EXB
!			MTBOOT.EXE	;KL MAG TAPE BOOT
!			      .EXB
!		        RP2DBT.EXE	;KL DISK BOOT WITH RP20 MCODE
!			      .EXB
!		        RP2MBT.EXE	;KL MAG TAPE BOOT WITH RP20 MCODE
!			      .EXB
!
!	NOTE THAT THE FOLLOWING RENAMES MUST BE PERFORMED BEFORE THE
!	RP20 BOOTS CAN BE MOVED TO THE FRONT END:
!
!	RENAME RP2DBT.EXB (to) BOOT.EXB
!	RENAME RP2MBT.EXB (to) MTBOOT.EXB
!
!	THE FOLLOWING ARE THE PARAMETER FILES USED FOR EACH BOOT:
!
!			  BOOT.EXE	;KL DISK BOOT
!					;FILES:
!					  SYSFLG.MAC
!			MTBOOT.EXE	;KL MAG TAPE BOOT
!					;FILES:
!					  SYSFLG.MAC+PMT.MAC
!		        RP2DBT.EXE	;KL DISK BOOT WITH RP20 MCODE
!					;FILES:
!					  SYSFLG.MAC+RP2.MAC
!		        RP2MBT.EXE	;KL MAG TAPE BOOT WITH RP20 MCODE
!					;FILES:
!					  SYSFLG.MAC+PMT.MAC+RP2.MAC
!
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
!
@ENABLE
@TAKE BATCH
@NOERROR
!TAKE A CHECKSUMMED DIRECTORY
!
@VDIR SYS:MACRO,SYS:LINK,SYS:RSXFMT,DSK:BOOT,DSK:DXMCA,DSK:DXMCE,DSK:MTBOOT,DSK:RP2DBT,DSK:RP2MBT,
@CHECK SEQ
@
!
!SEE WHAT VERSION OF MACRO AND WHERE IT CAME FROM
@GET SYS:MACRO
@I VER
@I FILE
@
!SEE WHAT VERSION OF LINK AND WHERE IT CAME FROM
@GET SYS:LINK
@I VER
@I FILE
@
!SEE WHAT VERSION OF RSXFMT AND WHERE IT CAME FROM
@GET SYS:RSXFMT
@I VER
@I FILE
@
!IDENTIFY THE LOCATION OF OUR SOURCE FILES
@SET TRAP FILE-OPENING
@
!FIRST BUILD STANDARD KL BOOT AND MTBOOT
!
BOOT::
@COMPILE /COMP R:SYSFLG.MAC+R:BOOT.MAC BOOT
@LINK
*/NOSYM
*/SET:.LOW.:40000
*BOOT,DXMCA.RMC/G
!BOOT NOW IN CORE
!NOW MAKE AN EXE FILE
@CSAVE BOOT 40000
@R RSXFMT
*CONVERT BOOT.EXE BOOT.EXB
!BOOT.EXB NOW MADE. MAKE MTBOOT
!
MTBOOT::
@COMPILE /COMP R:SYSFLG.MAC+R:PMT.MAC+R:BOOT MTBOOT
@LINK
*/NOSYM
*/SET:.LOW.:40000
*MTBOOT,DXMCA.RMC/G
@CSA MTBOOT 40000
@R RSXFMT
*CONVERT MTBOOT.EXE MTBOOT.EXB
!
!NOW BUILD KL BOOTS WITH RP20 MICROCODE
!
RP2DBT::
@COMPILE /COMP R:SYSFLG.MAC+R:RP2.MAC+R:BOOT.MAC RP2DBT
@LINK
*/NOSYM
*/SET:.LOW.:40000
*RP2DBT,DXMCA.RMC,DXMCE.RMC/G
!BOOT NOW IN CORE
!NOW MAKE AN EXE FILE
@CSAVE RP2DBT 40000
@R RSXFMT
*CONVERT RP2DBT.EXE  RP2DBT.EXB
!NOW MAKE MAG TAPE VERSION
!
RP2MBT::
@COMPILE /COMP R:SYSFLG.MAC+R:PMT.MAC+R:RP2.MAC+R:BOOT RP2MBT
@LINK
*/NOSYM
*/SET:.LOW.:40000
*RP2MBT,DXMCA.RMC,DXMCE.RMC/G
@CSA RP2MBT 40000 60777
@R RSXFMT
*CONVERT RP2MBT.EXE RP2MBT.EXB

!TAKE CHECKSUM OF FILES
@VDIRECT BOOT.EXB,MTBOOT.EXB,RP2DBT.EXB,RP2MBT.EXB,
@CHECK SEQ
@
!Identify the BOOT version #
@GET BOOT.EXE
@SET ENTRY-VECTOR 40000 3
@I VER
@
