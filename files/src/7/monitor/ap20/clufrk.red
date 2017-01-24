REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:05:16
File 1: SRC:<7.MONITOR>CLUFRK.MAC.1
File 2: SRC:<7.MONITOR.AP20>CLUFRK.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8877 to CLUFRK.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation.
; Edit= 8866 to CLUFRK.MAC on 21-Jul-88 by RASPUZZI
;Make the CLUDGR SYSAP correctly handle CLUBUF (SYSAP buffer count) and also
;make CLNEED handle little credit left situations.
;Remove references to CLUBUF from here.

***** CHANGE #2; PAGE 42, LINE 25; PAGE 42, LINE 25
	SKIPN T1		;If nothing passed in,
	MOVE T1,LSTERR		;then get our last error
	MOVEM T1,ERROR		;Save error code whilest we get SCA buffer
	MOVEI T1,1		;Tell SCA we only want a single buffer
	CALL <XENT SC.ABF>	;(T1/T1,T2,T3) get an SCA buffer
CLFNOB:	 BUG. (HLT,CLUOSB,CLUFRK,SOFT,<CLUDGR fork could not get an SCA buffer>,,<

 ---------------------------------
	SKIPN T1		;If nothing passed in,
	MOVE T1,LSTERR		;then get our last error
	MOVEM T1,ERROR		;Save error code whilest we get SCA buffer
	MOVEI T1,1		;Tell SCA we only want a single buffer
	CALL <XENT SC.ABF>	;(T1/T1,T2,T3) get an SCA buffer
CLFNOB:	 BUG.(HLT,CLUOSB,CLUFRK,SOFT,<CLUDGR fork could not get an SCA buffer>,,<


***** CHANGE #3; PAGE 42, LINE 36; PAGE 42, LINE 36
	get an SCA buffer to reply. This should not happen as SCA uses
	an entire section for buffers.
>)				;What do you mean no buffers?
	MOVEM T3,RETADR		;This is where return buffer
	MOVEM T1,SCABUF		;Save SCA buffer address
	AOS CLUBUF		;Say another buffer in use by CLUDGR
	MOVX T2,CL%REQ		;Say this is a response
 ---------------------------------
	get an SCA buffer to reply. This should not happen as SCA uses
	an entire section for buffers.
>)				;What do you mean no buffers?
	MOVEM T3,RETADR		;This is where return buffer
	MOVEM T1,SCABUF		;Save SCA buffer address
	MOVX T2,CL%REQ		;Say this is a response

***** CHANGE #4; PAGE 44, LINE 14; PAGE 44, LINE 14
; Returns:
;	NEVER

	XSWAPCD			;Called by fork

CLFFAL:	BUG. (HLT,CLUSCM,CLUFRK,SOFT,<CLUDGR fork send unconditionally failed>,,<

 ---------------------------------
; Returns:
;	NEVER

	XSWAPCD			;Called by fork

CLFFAL:	BUG.(HLT,CLUSCM,CLUFRK,SOFT,<CLUDGR fork send unconditionally failed>,,<


***** CHANGE #5; PAGE 44, LINE 23; PAGE 44, LINE 23
	MAXTRY times. This BUGHLT indicates that the send failed each
	time due to lack of receive credit on the remote system. The
	system must be sucking all of the credits somehow. However, simply
	not performing the send will hang a fork on the remote system.
	At least the fork will wake up.
>)				;Crash
 ---------------------------------
	MAXTRY times. This BUGHLT indicates that the send failed each
	time due to lack of receive credit on the remote system. The
	system must be sucking all of the credits somehow. However, simply
	not performing the send will hang a fork on the remote system.
	At least the fork will wake up.

Action:	It is possible that CI problems will cause this BUGHLT.
>)				;Crash

***** CHANGE #6; PAGE 45, LINE 17; PAGE 45, LINE 17
	XSWAPCD			;Process level code

CLFRSP:	SAVEAC <Q1>		;Save scratch spot
	XMOVEI T3,CLFREA	;Start putting stuff at this location
	SKIPN Q1,REQSCA(T1)	;Get first SCA buffer
	BUG. (HLT,CLFNSB,CLUFRK,SOFT,<No SCA buffers for request>,,<

 ---------------------------------
	XSWAPCD			;Process level code

CLFRSP:	SAVEAC <Q1>		;Save scratch spot
	XMOVEI T3,CLFREA	;Start putting stuff at this location
	SKIPN Q1,REQSCA(T1)	;Get first SCA buffer
	BUG.(HLT,CLFNSB,CLUFRK,SOFT,<No SCA buffers for request>,,<


***** CHANGE #7; PAGE 48, LINE 43; PAGE 48, LINE 43
	XMOVEI T1,CLFSCR	;Here's the data
	NOINT			;To satisfy SC.BRK
	CALL SC.BRK		;(T1,T2/T1,T2,T3) Put stuff in SCA message buffers
	IFNSK.			;If we failed,
	  SOSGE TRIES		;See if we failed too much
	  BUG. (HLT,CLOUTB,CLUFRK,SOFT,<System out of SCA buffers>,,<

 ---------------------------------
	XMOVEI T1,CLFSCR	;Here's the data
	NOINT			;To satisfy SC.BRK
	CALL SC.BRK		;(T1,T2/T1,T2,T3) Put stuff in SCA message buffers
	IFNSK.			;If we failed,
	  SOSGE TRIES		;See if we failed too much
	  BUG.(HLT,CLOUTB,CLUFRK,SOFT,<System out of SCA buffers>,,<


***** CHANGE #8; PAGE 49, LINE 1; PAGE 49, LINE 1
	  DISMS%		;Maybe buffers will come back
	  JRST CLFSN1		;Try it again
	ENDIF.
	OKINT			;All set now
	ADDM T2,CLUBUF		;Add the buffers in use by CLUDGR
	MOVEM T1,BUFCHN		;Save SCA buffer chain
 ---------------------------------
	  DISMS%		;Maybe buffers will come back
	  JRST CLFSN1		;Try it again
	ENDIF.
	OKINT			;All set now
	MOVEM T1,BUFCHN		;Save SCA buffer chain

***** CHANGE #9; PAGE 50, LINE 5; PAGE 50, LINE 5

;Here when send failed somehow. We are now going to give back
;the rest of the SCA buffers.
PNTCHN:	MOVE T1,BUFCHN		;Get buffer back
	SOS CLUBUF		;Discount this buffer
	CALLX (MSEC1,SC.RBF)	;(T1/) Now return it
 ---------------------------------

;Here when send failed somehow. We are now going to give back
;the rest of the SCA buffers.
PNTCHN:	MOVE T1,BUFCHN		;Get buffer back
	CALLX (MSEC1,SC.RBF)	;(T1/) Now return it

