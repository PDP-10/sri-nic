REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:05:21
File 1: SRC:<7.MONITOR>CLUPAR.MAC.1
File 2: SRC:<7.MONITOR.AP20>CLUPAR.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8866 to CLUPAR.MAC on 21-Jul-88 by RASPUZZI
;Make the CLUDGR SYSAP correctly handle CLUBUF (SYSAP buffer count) and also
;make CLNEED handle little credit left situations.
;Remove BUFMAX it shouldn't be a constant. It should be a data cell.

***** CHANGE #2; PAGE 8, LINE 6; PAGE 8, LINE 6

CLSND==0,,-1			;Special function for cluster send
CLCHNG==0,,-2			;Special function for setting our CLUDGR/Cluster send all status
BUFINI==^D10			;Number of buffers to queue up initially
BUFLOT==^D128			;Buffer threshold for returning SCA buffers
BUFMAX==2*BUFLOT		;Max number of buffers can be used by CLUDGR
BUFQUE==^D5			;Number of buffers to queue when CLUDGR gets low
 ---------------------------------

CLSND==0,,-1			;Special function for cluster send
CLCHNG==0,,-2			;Special function for setting our CLUDGR/Cluster send all status
BUFINI==^D10			;Number of buffers to queue up initially
BUFLOT==^D128			;Buffer threshold for returning SCA buffers
BUFQUE==^D5			;Number of buffers to queue when CLUDGR gets low

