REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:47:08
File 1: SRC:<7.MONITOR>LATSRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>LATSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8979 to LATSRV.MAC on 6-Oct-88 by WADDINGTON, for SPR #22245
;Eliminate unnecessary LATNSC type 12s on inactive circuits.
; Edit= 8974 to LATSRV.MAC on 30-Sep-88 by GSCOTT
;Prevent LAPRBF BUGCHKs on systems without an NIA20. Add routines SMFLTS and
;TMFLTS which implement SMON and TMON function 74. 
; Edit= 8937 to LATSRV.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8917 to LATSRV.MAC on 18-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 3, LINE 29; PAGE 3, LINE 29
;            2.3.4    Pending Request Block (PR) . . . . . . .  22
;            2.3.5    Miscellaneous  . . . . . . . . . . . . .  23
;        2.4    External References  . . . . . . . . . . . . .  24
;        2.5    MIN.,MAX.,MAXENT Macros  . . . . . . . . . . .  25
;        2.6    Local Variables  . . . . . . . . . . . . . . .  26
;    3. LATOP% JSYS
;        3.1    Dispatch . . . . . . . . . . . . . . . . . . .  27
;        3.2    Set/Clear Functions
;            3.2.1    SSERVC . . . . . . . . . . . . . . . . .  33
;        3.3    SET/CLEAR SERVICE  . . . . . . . . . . . . . .  35
;        3.4    Set/Clear Functions
;            3.4.1    CSERVC . . . . . . . . . . . . . . . . .  36
;            3.4.2    SETRTX . . . . . . . . . . . . . . . . .  37
;            3.4.3    SETNNM . . . . . . . . . . . . . . . . .  38
;            3.4.4    SETNID/CLRNID  . . . . . . . . . . . . .  39
;            3.4.5    SETCOD . . . . . . . . . . . . . . . . .  40
;        3.5    SET MAXIMUM CIRCUITS . . . . . . . . . . . . .  42
;        3.6    Show Characteristics of Host . . . . . . . . .  45
;        3.7    Show Terminal Connects . . . . . . . . . . . .  47
;        3.8    Show Adjacent Servers  . . . . . . . . . . . .  50
;        3.9    Show Counters  . . . . . . . . . . . . . . . .  51
;        3.10   Zero counters  . . . . . . . . . . . . . . . .  52
;        3.11   Request Host-Initiated Connect . . . . . . . .  54
;        3.12   Terminate Host-Initiated Connects  . . . . . .  72
;        3.13   Show Host-Initiated Connects . . . . . . . . .  75
;        3.14   Utility Routines
;            3.14.1   FNDSRV . . . . . . . . . . . . . . . . .  78
;            3.14.2   GTSRVR . . . . . . . . . . . . . . . . .  79
;            3.14.3   SCMPAR String Compare  . . . . . . . . .  80
 ---------------------------------
;            2.3.4    Pending Request Block (PR) . . . . . . .  22
;            2.3.5    Miscellaneous  . . . . . . . . . . . . .  23
;        2.4    External References  . . . . . . . . . . . . .  24
;        2.5    MIN.,MAX.,MAXENT Macros  . . . . . . . . . . .  25
;        2.6    Local Variables  . . . . . . . . . . . . . . .  26
;    3. SMON%/TMON% JSYS . . . . . . . . . . . . . . . . . . .  27
;    4. LATOP% JSYS
;        4.1    Dispatch . . . . . . . . . . . . . . . . . . .  28
;        4.2    Set/Clear Functions
;            4.2.1    SSERVC . . . . . . . . . . . . . . . . .  34
;        4.3    SET/CLEAR SERVICE  . . . . . . . . . . . . . .  36
;        4.4    Set/Clear Functions
;            4.4.1    CSERVC . . . . . . . . . . . . . . . . .  37
;        4.5    Valid Character checker - VCRCHK . . . . . . .  38
;        4.6    Set/Clear Functions
;            4.6.1    SETRTX . . . . . . . . . . . . . . . . .  39
;            4.6.2    SETNNM . . . . . . . . . . . . . . . . .  40
;            4.6.3    SETNID/CLRNID  . . . . . . . . . . . . .  41
;            4.6.4    SETCOD . . . . . . . . . . . . . . . . .  42
;        4.7    SET MAXIMUM CIRCUITS . . . . . . . . . . . . .  44
;        4.8    Show Characteristics of Host . . . . . . . . .  47
;        4.9    Show Terminal Connects . . . . . . . . . . . .  49
;        4.10   Show Adjacent Servers  . . . . . . . . . . . .  52
;        4.11   Show Counters  . . . . . . . . . . . . . . . .  53
;        4.12   Zero counters  . . . . . . . . . . . . . . . .  54
;        4.13   Request Host-Initiated Connect . . . . . . . .  56
;        4.14   Terminate Host-Initiated Connects  . . . . . .  74
;        4.15   Show Host-Initiated Connects . . . . . . . . .  77

***** CHANGE #3; PAGE 5, LINE 8; PAGE 5, LINE 8
;		     Table of Contents for LATSRV
;
;				  Section		      Page
;
;
;    4. TOPS-10
;        4.1    SCNSER Interface
;            4.1.1    LDBISR Dispatch  . . . . . . . . . . . .  82
;        4.2    NETOP. Functions . . . . . . . . . . . . . . .  83
;    5. LATINI - LAT Initialization  . . . . . . . . . . . . .  85
;    6. Subroutine CWHLAT - Type a server spec for WHERE comma  90
;    7. Multicast Transmitter  . . . . . . . . . . . . . . . .  92
;    8. Routine to check/compute the Dynamic Rating  . . . . .  93
;    9. Routines to Build the Host Multicast Message
;        9.1    BMCFXD . . . . . . . . . . . . . . . . . . . .  94
;        9.2    BMCFXD . . . . . . . . . . . . . . . . . . . .  95
;        9.3    BMCNID . . . . . . . . . . . . . . . . . . . .  96
;        9.4    BMCSRV . . . . . . . . . . . . . . . . . . . .  97
;        9.5    BMCRAT . . . . . . . . . . . . . . . . . . . .  98
;   10. Multicast Transmit Check Routine . . . . . . . . . . .  99
;   11. Build the START Message Template . . . . . . . . . . . 100
;   12. LATCBR - LAT DLL Callback Routinea . . . . . . . . . . 101
;   13. LAINTR - Interrupt Level Virtual Circuit Message Recei 102
;   14. LAINTX - Interrupt Level Virtual Circuit Message Recei 103
;   15. LARSCH - Scheduler Level Virtual Circuit Message Recei 104
;   16. LATLSC - Ethernet Link State Change Callback . . . . . 106
;   17. Message Receiver - HMSTRT  . . . . . . . . . . . . . . 108
;   18. HSTRCB . . . . . . . . . . . . . . . . . . . . . . . . 109
;   19. CBINIT - Circuit Block Initialization  . . . . . . . . 110
;   20. CMNIAD - Find CB base on NI address  . . . . . . . . . 111
;   21. HMRUN  . . . . . . . . . . . . . . . . . . . . . . . . 113
;   22. HSTRCB . . . . . . . . . . . . . . . . . . . . . . . . 115
;   23. HMSTOP . . . . . . . . . . . . . . . . . . . . . . . . 116
;   24. HMSTAT - Receive Status Message  . . . . . . . . . . . 118
;   25. HMINFO - Receive Response Information Message  . . . . 122
;   26. Message Transmit Routines  . . . . . . . . . . . . . . 124
;       26.1    XMTMSG . . . . . . . . . . . . . . . . . . . . 131
;       26.2    XMTDON . . . . . . . . . . . . . . . . . . . . 132
;   27. Receive Message Handling Routines  . . . . . . . . . . 133
;   28. Transmit Message Handling Routines . . . . . . . . . . 134
;   29. Slot Demultiplexor - LSDMUX  . . . . . . . . . . . . . 136
;   30. HSSTRT - Process a START SLOT Message  . . . . . . . . 137
;   31. Slot Processing Routines - STOP Slots  . . . . . . . . 141
;   32. Slot Processing Routines - DATA_A,DATA_B Slots . . . . 142
;   33. Slot Block Allocation and Deallocation Routines  . . . 144
;   34. Subroutines
;       34.1    NGVLDB - Give an LDB back to SCNSER  . . . . . 150
;   35. Slot Multiplexor - Main Loop . . . . . . . . . . . . . 152
;   36. BLDMSL . . . . . . . . . . . . . . . . . . . . . . . . 155
 ---------------------------------
;		     Table of Contents for LATSRV
;
;				  Section		      Page
;
;
;        4.16   Utility Routines
;            4.16.1   FNDSRV . . . . . . . . . . . . . . . . .  80
;            4.16.2   GTSRVR . . . . . . . . . . . . . . . . .  81
;            4.16.3   SCMPAR String Compare  . . . . . . . . .  82
;    5. TOPS-10
;        5.1    SCNSER Interface
;            5.1.1    LDBISR Dispatch  . . . . . . . . . . . .  84
;        5.2    NETOP. Functions . . . . . . . . . . . . . . .  85
;    6. LATINI - LAT Initialization  . . . . . . . . . . . . .  87
;    7. Subroutine CWHLAT - Type a server spec for WHERE comma  92
;    8. Multicast Transmitter  . . . . . . . . . . . . . . . .  94
;    9. Routine to check/compute the Dynamic Rating  . . . . .  96
;   10. Routines to Build the Host Multicast Message
;       10.1    BMCFXD . . . . . . . . . . . . . . . . . . . .  97
;       10.2    BMCFXD . . . . . . . . . . . . . . . . . . . .  98
;       10.3    BMCNID . . . . . . . . . . . . . . . . . . . .  99
;       10.4    BMCSRV . . . . . . . . . . . . . . . . . . . . 100
;       10.5    BMCRAT . . . . . . . . . . . . . . . . . . . . 101
;   11. Multicast Transmit Check Routine . . . . . . . . . . . 102
;   12. Build the START Message Template . . . . . . . . . . . 103
;   13. LATCBR - LAT DLL Callback Routinea . . . . . . . . . . 104
;   14. LAINTR - Interrupt Level Virtual Circuit Message Recei 105
;   15. LAINTX - Interrupt Level Virtual Circuit Message Recei 106
;   16. LARSCH - Scheduler Level Virtual Circuit Message Recei 107
;   17. LATLSC - Ethernet Link State Change Callback . . . . . 109
;   18. Message Receiver - HMSTRT  . . . . . . . . . . . . . . 111
;   19. HSTRCB . . . . . . . . . . . . . . . . . . . . . . . . 112
;   20. CBINIT - Circuit Block Initialization  . . . . . . . . 113
;   21. CMNIAD - Find CB base on NI address  . . . . . . . . . 114
;   22. HMRUN  . . . . . . . . . . . . . . . . . . . . . . . . 116
;   23. HSTRCB . . . . . . . . . . . . . . . . . . . . . . . . 118
;   24. HMSTOP . . . . . . . . . . . . . . . . . . . . . . . . 119
;   25. HMSTAT - Receive Status Message  . . . . . . . . . . . 121
;   26. HMINFO - Receive Response Information Message  . . . . 125
;   27. Message Transmit Routines  . . . . . . . . . . . . . . 127
;       27.1    XMTMSG . . . . . . . . . . . . . . . . . . . . 134
;       27.2    XMTDON . . . . . . . . . . . . . . . . . . . . 135
;   28. Receive Message Handling Routines  . . . . . . . . . . 136
;   29. Transmit Message Handling Routines . . . . . . . . . . 137
;   30. Slot Demultiplexor - LSDMUX  . . . . . . . . . . . . . 139
;   31. HSSTRT - Process a START SLOT Message  . . . . . . . . 140
;   32. Slot Processing Routines - STOP Slots  . . . . . . . . 144
;   33. Slot Processing Routines - DATA_A,DATA_B Slots . . . . 145
;   34. Slot Block Allocation and Deallocation Routines  . . . 147
;   35. Subroutines
;       35.1    NGVLDB - Give an LDB back to SCNSER  . . . . . 153
;   36. Slot Multiplexor - Main Loop . . . . . . . . . . . . . 155

***** CHANGE #4; PAGE 7, LINE 8; PAGE 7, LINE 8
;		     Table of Contents for LATSRV
;
;				  Section		      Page
;
;
;   37. Slot Formatting Routines
;       37.1    XSLSTA . . . . . . . . . . . . . . . . . . . . 159
;       37.2    XSLSTP . . . . . . . . . . . . . . . . . . . . 160
;       37.3    XSLDTA . . . . . . . . . . . . . . . . . . . . 161
;       37.4    XSLDTB . . . . . . . . . . . . . . . . . . . . 164
;   38. Queue Handling Routines  . . . . . . . . . . . . . . . 167
;   39. Buffer Handling Routines
;       39.1    Transmit Buffers . . . . . . . . . . . . . . . 169
;       39.2    Receive Buffers  . . . . . . . . . . . . . . . 173
;   40. SCNSER DEVICE DEPENDANT ROUTINES . . . . . . . . . . . 174
;   41. TTYSRV DEVICE DEPENDENT ROUTINES . . . . . . . . . . . 176
;   42. More SCNSER device dependant routines  . . . . . . . . 177
;   43. General Utility Routines . . . . . . . . . . . . . . . 178
;   44. LAGTCR - LAT memory management routines  . . . . . . . 179
;   45. End of LATSRV  . . . . . . . . . . . . . . . . . . . . 182
 ---------------------------------
;		     Table of Contents for LATSRV
;
;				  Section		      Page
;
;
;   37. BLDMSL . . . . . . . . . . . . . . . . . . . . . . . . 158
;   38. Slot Formatting Routines
;       38.1    XSLSTA . . . . . . . . . . . . . . . . . . . . 162
;       38.2    XSLSTP . . . . . . . . . . . . . . . . . . . . 163
;       38.3    XSLDTA . . . . . . . . . . . . . . . . . . . . 164
;       38.4    XSLDTB . . . . . . . . . . . . . . . . . . . . 167
;   39. Queue Handling Routines  . . . . . . . . . . . . . . . 170
;   40. Buffer Handling Routines
;       40.1    Transmit Buffers . . . . . . . . . . . . . . . 172
;       40.2    Receive Buffers  . . . . . . . . . . . . . . . 176
;   41. SCNSER DEVICE DEPENDANT ROUTINES . . . . . . . . . . . 177
;   42. TTYSRV DEVICE DEPENDENT ROUTINES . . . . . . . . . . . 179
;   43. More SCNSER device dependant routines  . . . . . . . . 180
;   44. General Utility Routines . . . . . . . . . . . . . . . 181
;   45. LAGTCR - LAT memory management routines  . . . . . . . 182
;   46. End of LATSRV  . . . . . . . . . . . . . . . . . . . . 185

***** CHANGE #5; PAGE 29, LINE 51; PAGE 29, LINE 51
	MAX.	(TMPLNG,<<GB.LEN+1>,ML.NNM,ML.DSC>)
TMPBLK:	BLOCK TMPLNG		;Temporary block for LATOP.
	TMPGB==TMPBLK+1

>;end IFN FTOPS10

 ---------------------------------
	MAX.	(TMPLNG,<<GB.LEN+1>,ML.NNM,ML.DSC>)
TMPBLK:	BLOCK TMPLNG		;Temporary block for LATOP.
	TMPGB==TMPBLK+1

>;end IFN FTOPS10
	SUBTTL SMON%/TMON% JSYS

IFN FTOPS20,<
	SWAPCD			;[8974] JSYS code

;[8974] SMFLTS - Routine to set LAT access state for TMON.
;Called from SMON code with T2/ LS.OFF or LS.ON
;Returns +1 always, ITERR on errors

SMFLTS::SKIPN T1,LAHNDB		;If there is no data base,
	ITERR (LATX03)		; then LAT is not operational
	CAIE T2,LS.ON		;Is it
	CAIN T2,LS.OFF		; on or off?
	CAIA			;Yes
	ITERR (ARGX02)		;No, argument out of range
	STOR T2,HNLAS,(T1)	;Store new LAT state
	RET			;Return

;[8974] TMFLTS - Get LAT access state for TMON.
;Returns +1 always, lat access state in T1
;ITERRs on errors

TMFLTS::SKIPN T2,LAHNDB		;If there is no data base,
	ITERR (LATX03)		; then LAT is not operational
	LOAD T1,HNLAS,(T2)	;Load LAT state
	RET			;Return with state in T1
>;end of IFN FTOPS20


***** CHANGE #6; PAGE 64, LINE 38; PAGE 65, LINE 38
	  BUG. (CHK,LATIPR,LATSRV,SOFT,<LAT Invalid PR block>,<<P1,PRBLOK>,<T1,SBBLOK>>,<

Cause:	A Pending Request block was about to be deleted that still had a
	Slot block attached to it.

>)
 ---------------------------------
	  BUG. (CHK,LATIPR,LATSRV,SOFT,<LAT Invalid PR block>,<<P1,PRBLOK>,<T1,SBBLOK>>,<

Cause:	A Pending Request block was about to be deleted that still had a
	Slot block attached to it.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	PRBLOK - Address of pending request block
	SBBLOK - Address of slot block
>)

***** CHANGE #7; PAGE 71, LINE 50; PAGE 72, LINE 50
	CAXN T1,.LAQUE		; or queued?
	TRNA			;Yes, continue waiting
	RET			;No, return
	MOVX T1,EV.LAT		;LAT event
	LOAD J,PRJOB,(P1)	;Get the job number
	SNCALL (ESLEEP##,MCSEC0) ;[7.yyyy](T1,J)Go to sleep
	JRST PRWAI1		;Awake for some reason: check connection
 ---------------------------------
	CAXN T1,.LAQUE		; or queued?
	TRNA			;Yes, continue waiting
	RET			;No, return
	MOVX T1,EV.LAT		;LAT event
	LOAD J,PRJOB,(P1)	;Get the job number
	SNCALL (ESLEEP##,MCSEC0) ;[7.1024](T1,J)Go to sleep
	JRST PRWAI1		;Awake for some reason: check connection

***** CHANGE #8; PAGE 89, LINE 14; PAGE 90, LINE 14
;	Normal Return

	ENTRY LATINI		;Kepp LINK happy in library search mode

XRENT	(LATINI,G)		;[7.1024]LATINI::, XLATIN::

	ACVAR <HN,XB,W1,W2>	;Reserve AC for Host Node data base address
 ---------------------------------
;	Normal Return

	ENTRY LATINI		;Kepp LINK happy in library search mode

XRENT	(LATINI,G)		;[7.1024]LATINI::, XLATIN::
	SKIPN NIHERE		;[8974] Do we have an NIA20 today?
	RET			;[8974] Nope
	ACVAR <HN,XB,W1,W2>	;Reserve AC for Host Node data base address

***** CHANGE #9; PAGE 92, LINE 47; PAGE 93, LINE 47
INIRES:
	BUG. (CHK,LATINE,LATSRV,SOFT,<LATINI failed to initialize>,,<

Cause:	Could not obtain sufficient memory for the LAT host databases.

Action:	Determine why there is so little resident memory available at system
	startup.

Data:	HN.LST words for the host node database, CBMAXI words for CBVECT,
	and NTTLAH words for SBVECT.

>)
 ---------------------------------
INIRES:
	BUG. (CHK,LATINE,LATSRV,SOFT,<LATINI failed to initialize>,,<

Cause:	Could not obtain sufficient memory for the LAT host databases.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump in order to
        determine why there is so little resident memory available at system
	startup.  The values to consider are: HC.LST words for the host node 
	database, CBMAXI words for CBVECT, NTTLAH words for SBVECT, and
	PRMAXI words for PRVECT and PRRAND.
>)

***** CHANGE #10; PAGE 96, LINE 11; PAGE 97, LINE 11
;	Normal Return

XRENT	(LATXMC,G)		;[7.1024]LATXMC::, XLATXM::

	ACVAR <HN,XB,W1,W2>
	MOVE HN,LAHNDB		;Get the HN data base address
	SETZM LATMCT		;[7.1281]Clear the timer
	CALL GETRBF		;[7.1281](HN)Post more receive buffers if needed
	 TRN			;[7.1281]Not necessary.
	XMOVEI XB,HN.MCM(HN)	;[7.1281]Get the address of the multicast msg
	LOAD T1,UNUID,+UNB.OF(XB) ;[7.1281]Get the UID field
	TXNE T1,DLL.FL		;[7.1281]If DLL.FL is lit then we don't want
	RET			;[7.1281] to step on the buffer.  Back soon.
	LOAD T1,HNMTI,(HN)	;Reset the multicast timer
IFN FTOPS20,<
	IMULI T1,^D1000		;Convert from seconds to milliseconds
>;END IFN TOPS20
	MOVEM T1,LATMCT		;
	CALL CHKXMC		;(HN)Transmit the multicast this time?
	 RET			;No.
	CALL DYNRAT		;(HN)Check if dynamic rating needs update.
 ---------------------------------
;	Normal Return

XRENT	(LATXMC,G)		;[7.1024]LATXMC::, XLATXM::

	ACVAR <HN,XB,W1,W2>
	SKIPN HN,LAHNDB		;[8974] Get the HN data base address
	RET			;[8974] No NI on this system
	LOAD T1,HNMTI,(HN)	;Reset the multicast timer
IFN FTOPS20,<
	IMULI T1,^D1000		;Convert from seconds to milliseconds
>;END IFN TOPS20
	MOVEM T1,LATMCT		;Store new multicast timer
	CALL CHKXMC		;(HN)Transmit the multicast this time?
	 RET			;No.
	CALL GETRBF		;[8974](HN) Post more receive buffers if needed
	 TRN			;[8974] Not necessary.
	XMOVEI XB,HN.MCM(HN)	;[8974] Get the address of the multicast msg
	LOAD T1,UNUID,+UNB.OF(XB) ;[8974] Get the UID field
	TXNN T1,DLL.FL		;[8974] Is DLL.FL lit?
	IFSKP.			;[8974] Yes, wait till it is off
	  SETZM LATMCT		;[8974] Clear the timer
	  RET			;[8974] Come back real soon to check again
	ENDIF.			;[8974] If DLL.FL is off, continue

	CALL DYNRAT		;(HN)Check if dynamic rating needs update.

***** CHANGE #11; PAGE 105, LINE 11; PAGE 107, LINE 11
;	T2/UN block address
;	CALL LATCBR
;	Normal return

LATCBR:	ACVAR <HN,XB,RB>	;Reserve some registers
	MOVE HN,LAHNDB		;Get the host node data base address
	MOVE T4,T3		;Save NISRV's error register
 ---------------------------------
;	T2/UN block address
;	CALL LATCBR
;	Normal return

LATCBR:	ACVAR <HN,XB,RB>	;Reserve some registers
	SKIPN HN,LAHNDB		;[8974] Get the host node data base address
	RET			;[8974] No NIA20 on this system
	MOVE T4,T3		;Save NISRV's error register

***** CHANGE #12; PAGE 105, LINE 20; PAGE 107, LINE 21
	CAIG T1,MAXCBF
	CALLRET @CBRDSP-MINCBF(T1) ;Valid, call the proper routine
	BUG. (CHK,LATICB,LATSRV,SOFT,<LATCBR called from NISRV with illegal callback function code>,<<T1,CODE>>,<

Cause:	NISRV has called the LATSRV callback routine with an invalid function
	code.

Data:	CODE - Function code

>)
 ---------------------------------
	CAIG T1,MAXCBF
	CALLRET @CBRDSP-MINCBF(T1) ;Valid, call the proper routine
	BUG. (CHK,LATICB,LATSRV,SOFT,<LATCBR called from NISRV with illegal callback function code>,<<T1,CODE>>,<

Cause:	NISRV has called the LATSRV callback routine with an invalid function
	code.  This indicates a problem in NISRV.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	CODE - Function code
>)

***** CHANGE #13; PAGE 108, LINE 5; PAGE 110, LINE 5
	SUBTTL LARSCH - Scheduler Level Virtual Circuit Message Receiver

;LARSCH - Complete circuit message processing at scheduler level

IFN FTOPS10,<			;[7.yyyy]
	RESCD			;[7.yyyy]
LATSTO::SKPCPU	(0)		;[7.yyyy]On policy CPU?
	RET			;[7.yyyy]No, do nothing
;	CALLRET LARSCH		;[7.yyyy]Yes, fall into LARSCH
> ;End IFN FTOPS10
 ---------------------------------
	SUBTTL LARSCH - Scheduler Level Virtual Circuit Message Receiver

;LARSCH - Complete circuit message processing at scheduler level

IFN FTOPS10,<			;[7.1024]
	RESCD			;[7.1024]
LATSTO::SKPCPU	(0)		;[7.1024]On policy CPU?
	RET			;[7.1024]No, do nothing
;	CALLRET LARSCH		;[7.1024]Yes, fall into LARSCH
> ;End IFN FTOPS10

***** CHANGE #14; PAGE 108, LINE 50; PAGE 110, LINE 50
	CAILE T1,MAXMTY		;...
	IFNSK.
	  BUG. (CHK,LATIMT,LATSRV,SOFT,<LAT Illegal Message Type>,<<T1,MSGTYP>>,<

Cause:	The LAT virtual circuit message was received with a message type out
	of range.

Data:	MSGID - Message type

>)
 ---------------------------------
	CAILE T1,MAXMTY		;...
	IFNSK.
	  BUG. (CHK,LATIMT,LATSRV,SOFT,<LAT Illegal Message Type>,<<T1,MSGTYP>>,<

Cause:	The LAT virtual circuit message was received with a message type out
	of range.  This indicates a protocol problem.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	MSGTYP - Message type
>)

***** CHANGE #15; PAGE 118, LINE 30; PAGE 120, LINE 30
	IFNSK.			;Invalid slot type returned.
	  BUG. (INF,LATIST,LATSRV,SOFT,<LAT Illegal Slot Type>,<<T2,SLTID>>,<

Cause:	LAT Slot received with Slot type out of range.

Data:	SLTID - Slot ID

>)
 ---------------------------------
	IFNSK.			;Invalid slot type returned.
	  BUG. (INF,LATIST,LATSRV,SOFT,<LAT Illegal Slot Type>,<<T2,SLTID>>,<

Cause:	LAT Slot received with Slot type out of range.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	SLTID - Slot ID
>)

***** CHANGE #16; PAGE 121, LINE 34; PAGE 123, LINE 34
NIHALT:	MOVEI T1,CE.NIH		;NI halted
	SETZ T2,0		;PC meaningless
	JRST INFSTP
>;END IFN FTOPS20

INVRUN:	MOVEI T1,CE.INV		;Invalid message
	JRST INFSTP
INFSTP:	OPSTR <DMOVE T3,>,UNSAD,+UNB.OF(RB) ;[7.1227]GET ADDRESS IN T3 & T4
 ---------------------------------
NIHALT:	MOVEI T1,CE.NIH		;NI halted
	SETZ T2,0		;PC meaningless
	JRST INFSTP
>;END IFN FTOPS20

;[8979]There are two paths into INVRUN: One if there is no CB.  The other
;if there is an id mismatch between the circuit block and the run message.
;In that case, T3 contains CBRID.  If CBRID is zero, this implies that the
;CB has been moved to the inactive queue.  If CBRID is nonzero then we have
;a real live protocol violation and we fall thru to the LATNSC.  We should
;only get this LATNSC on active circuits.
INVRUN: MOVEI T1,CE.INV		;Invalid message
	JUMPE CB,MTTSTP		;[8979]No circuit block, go quietly
	JUMPE T3,MTTSTP		;[8979]Inactive circuit block, go quietly
;	JRST INFSTP		;[8979]Real LATNSC - invalid run message

INFSTP:	OPSTR <DMOVE T3,>,UNSAD,+UNB.OF(RB) ;[7.1227]GET ADDRESS IN T3 & T4

***** CHANGE #17; PAGE 121, LINE 60; PAGE 123, LINE 69
	CE.RES==7		;Insufficient resources
	CE.STO==10		;Server circuit timer out of range
	CE.SKW==11		;Protocol version skew
	CE.INV==12		;Invalid Message


Data:	CODE - Reason code
	PC - PC
	HIADDR - High order 32 bits of Ethernet address
 ---------------------------------
	CE.RES==7		;Insufficient resources
	CE.STO==10		;Server circuit timer out of range
	CE.SKW==11		;Protocol version skew
	CE.INV==12		;Invalid Message

        If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	CODE   - Reason code
	PC     - PC
	HIADDR - High order 32 bits of Ethernet address

***** CHANGE #18; PAGE 139, LINE 32; PAGE 141, LINE 32
	IFNSK.
IFN FTOPS20,<
	  BUG. (CHK,LAPRBF,LATSRV,SOFT,<Specify Receive Buffer Failure>,<<T1,DLLERC>>,<

Cause:	LATSRV received an error from NISRV while attempting to post a
	receive buffer.

Data:	DLLERC - Error code returned by NISRV

>)
 ---------------------------------
	IFNSK.
IFN FTOPS20,<
	  BUG. (CHK,LAPRBF,LATSRV,SOFT,<Specify Receive Buffer Failure>,<<T1,DLLERC>>,<

Cause:	LATSRV received an error from NISRV while attempting to post a
	receive buffer.  This indicates a problem in NISRV.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC - Error code returned by NISRV
>)

