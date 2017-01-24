! <6-1-EXEC>MKSUEX.CMD.6, 12-May-85 01:14:26, Edit by WHP4!
!  this file used in building Stanford command-editor EXEC !
MICPRM.MAC MICPRM		;MIC symbols
SUDEF.MAC+EXECDE.MAC+EXECGL.MAC EXECGL	;Universal symbols
EXECPR.MAC EXECPR		;Global storage, *NOT* for individual local
				;  command storage; use TRVAR and STKVAR
SUDEF.MAC+EXECVR.MAC EXECVR	;Version information
EXEC0.MAC EXEC0			;Top-level command input, SYSTAT
EXECCA.MAC EXECCA		;Configuration varients
EXEC1.MAC EXEC1			;Miscellaneous commands
EXECSE.MAC EXECSE		;SET commands and ^ESET commands
EXECMT.MAC EXECMT		;Tape and disk mounting
EXECP.MAC EXECP			;Program commands (like RUN, DDT, CONTINUE)
EXECIN.MAC EXECIN		;INFORMATION commands
EXEC2.MAC EXEC2			;COPY, APPEND, TYPE
EXEC3.MAC EXEC3			;DIRECTORY-class commands
EXEC4.MAC EXEC4			;BUILD, ^ECREATE, INFO DIRECTORY
EXECED.MAC EXECED		;EDIT, CREATE commands
EXECCS.MAC EXECCS		;COMPIL-class commands
EXECQU.MAC EXECQU		;QUEUE-class commands
EXECSU.MAC EXECSU		;Subroutines
EXECHS.MAC EXECHS		;History/command editor
EXECTT.MAC EXECTT		;Terminal type info for EXECHS
EXECMI.MAC EXECMI		;MIC-class commands
EXECPM.MAC EXECPM		;PCL
EXECPS.MAC EXECPS
EXECPI.B36 EXECPI
EXECPC.B36 EXECPC
EXECPX.B36 EXECPX
EXECPU.B36 EXECPU
