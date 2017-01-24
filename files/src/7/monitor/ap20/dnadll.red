REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:07:06
File 1: SRC:<7.MONITOR>DNADLL.MAC.1
File 2: SRC:<7.MONITOR.AP20>DNADLL.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8887 to DNADLL.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 15, LINE 71; PAGE 15, LINE 71
	each circuit and one to guarantee some level of route-through ability.
	DNADLL was asked to open a data link, but the memory manager could
	not guarantee the buffers.

Action:	Allocate more memory or settle for fewer circuits.

>)
 ---------------------------------
	each circuit and one to guarantee some level of route-through ability.
	DNADLL was asked to open a data link, but the memory manager could
	not guarantee the buffers.

Action:	Allocate more memory or settle for fewer circuits.
>)

***** CHANGE #3; PAGE 21, LINE 28; PAGE 21, LINE 28
	CAXL T1,DF.OPN		;Range check the
	CAXLE T1,DF.MAX		; function code
	BUG.(CHK,DNDIUF,DNADLL,SOFT,<Illegal function code from DNADLL user>,,<

Cause:	DNADLL was called with a bad function by ROUTER

>,RTN)
 ---------------------------------
	CAXL T1,DF.OPN		;Range check the
	CAXLE T1,DF.MAX		; function code
	BUG.(CHK,DNDIUF,DNADLL,SOFT,<Illegal function code from DNADLL user>,,<

Cause:	DNADLL was called with a bad function by ROUTER
>,RTN)

***** CHANGE #4; PAGE 21, LINE 47; PAGE 21, LINE 46
	CAXL T1,NF.SET		;Range check the
	CAXLE T1,NF.CET		; function code
	BUG.(CHK,DNDINF,DNADLL,SOFT,<Illegal function code from NTMAN>,,<

Cause:	DNADLL was called with a bad function by NTMAN.

>,RTN)
 ---------------------------------
	CAXL T1,NF.SET		;Range check the
	CAXLE T1,NF.CET		; function code
	BUG.(CHK,DNDINF,DNADLL,SOFT,<Illegal function code from NTMAN>,,<

Cause:	DNADLL was called with a bad function by NTMAN.
>,RTN)

***** CHANGE #5; PAGE 33, LINE 14; PAGE 33, LINE 14

IFN FTPARANOIA,<
	CAXL T1,DI.ODN
	 CAXLE T1,DI.ICB
	BUG. (CHK,DTIIFK,DNADLL,SOFT,<Illegal function code from DTE kontroller>,,<
	This BUG is not documented yet.
>,RTN)
 ---------------------------------

IFN FTPARANOIA,<
	CAXL T1,DI.ODN
	 CAXLE T1,DI.ICB
	BUG. (CHK,DTIIFK,DNADLL,SOFT,<Illegal function code from DTE kontroller>,,<
	This BUG. is only assembled in DEBUG code and does not appear
	in the field.
>,RTN)

***** CHANGE #6; PAGE 33, LINE 27; PAGE 33, LINE 28

Cause:	DTESRV has lost the callback ID for this line or never had one.

Action:	See if protocol was started when Router thought the circuit state was
	off.  Or, check DCNCID in DTESRV to see what it has for a callback ID.

>,RTN)
 ---------------------------------

Cause:	DTESRV has lost the callback ID for this line or never had one.

Action:	See if protocol was started when Router thought the circuit state was
	off.  Or, check DCNCID in DTESRV to see what it has for a callback ID.
>,RTN)

***** CHANGE #7; PAGE 38, LINE 22; PAGE 38, LINE 22
Cause:	NISRV returned an error when asked to return the channel list.

Action:	Check error code returned in T1 to see what NISRV thinks is wrong.

Data:	ERRCOD - Error code returned by NISRV

>,NIDIN1)
	LOAD T1,UNBSZ,(UN)	; Get number of channels found
	SKIPE T1		; Any found?
	 AOS (P)		; Yes, so for now, indicate success
NIDIN1:	LOAD T1,UNBFA,(UN)	; Free the buffer
 ---------------------------------
Cause:	NISRV returned an error when asked to return the channel list.

Action:	Check error code returned in T1 to see what NISRV thinks is wrong.

Data:	ERRCOD - Error code returned by NISRV
>,NIDIN1)
	LOAD T1,UNBSZ,(UN)	; Get number of channels found
	SKIPE T1		; Any found?
	AOS (P)			; Yes, so for now, indicate success
NIDIN1:	LOAD T1,UNBFA,(UN)	; Free the buffer

***** CHANGE #8; PAGE 41, LINE 30; PAGE 41, LINE 30
Cause:	NISRV returned an error when trying to open a portal.

Action:	Check error code returned in T1 to see what NISRV thinks is wrong.

Data:	ERRCOD - Error code returned by NISRV

>,NIFOF)
 ---------------------------------
Cause:	NISRV returned an error when trying to open a portal.

Action:	Check error code returned in T1 to see what NISRV thinks is wrong.

Data:	ERRCOD - Error code returned by NISRV
>,NIFOF)

***** CHANGE #9; PAGE 41, LINE 69; PAGE 41, LINE 68
				;  or as many as it can
	SETONE DLLIU,(DL)	; Router is using the line
	LOAD T1,LTSTA,(LT)	; Return line state to Router
	RETSKP

NIFEMF:
	 BUG.(CHK,DNDEMF,DNADLL,SOFT,<Enable Ethernet multicast address failed>,<<T1,ERRCOD>>,<

 ---------------------------------
				;  or as many as it can
	SETONE DLLIU,(DL)	; Router is using the line
	LOAD T1,LTSTA,(LT)	; Return line state to Router
	RETSKP

NIFEMF:	BUG.(CHK,DNDEMF,DNADLL,SOFT,<Enable Ethernet multicast address failed>,<<T1,ERRCOD>>,<


***** CHANGE #10; PAGE 43, LINE 47; PAGE 43, LINE 47
Cause:	NISRV returned an error when trying to queue a message for transmit.

Action:	Check error code returned in T1 and investigate problem in NISRV.

Data:	ERRCOD - Error code returned by NISRV

>,FREUNB)
 ---------------------------------
Cause:	NISRV returned an error when trying to queue a message for transmit.

Action:	Check error code returned in T1 and investigate problem in NISRV.

Data:	ERRCOD - Error code returned by NISRV
>,FREUNB)

***** CHANGE #11; PAGE 68, LINE 13; PAGE 68, LINE 13

Cause:	DECnet has used all of its available memory and could not give us any.

Action:	Try to determine who is using all the memory and why.  Setting FTDEBUG
	to non-zero will give more information about who is using each block
	of memory.

>,RTN)
 ---------------------------------

Cause:	DECnet has used all of its available memory and could not give us any.

Action:	Try to determine who is using all the memory and why.  Setting FTDEBUG
	to non-zero will give more information about who is using each block
	of memory. You may also consider building a monitor with more DECnet
	freespace incorporated.
>,RTN)

