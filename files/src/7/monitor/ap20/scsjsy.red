REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:33:42
File 1: SRC:<7.MONITOR>SCSJSY.MAC.1
File 2: SRC:<7.MONITOR.AP20>SCSJSY.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8936 to SCSJSY.MAC on 23-Aug-88 by GSCOTT
;Update BUG. documentation. Change some IFNSK. to IFSKP. 

***** CHANGE #2; PAGE 3, LINE 14; PAGE 3, LINE 14
;    2. Entry to JSYS  . . . . . . . . . . . . . . . . . . . .   6
;    3. Function code handlers
;        3.1    Connect (SCSCON) . . . . . . . . . . . . . . .   7
;        3.2    Listen (SCSLIS)  . . . . . . . . . . . . . . .   8
;        3.3    Receive DG/MSG (SCSRDG/SCSRMG) . . . . . . . .   9
;        3.4    Read port counters (SCSRPC)  . . . . . . . . .  10
;        3.5    Reject (SCSREJ)  . . . . . . . . . . . . . . .  11
;        3.6    Disconnect (SCSDIS)  . . . . . . . . . . . . .  12
;        3.7    Send message/datagram (SCSSMG/SCSSDG)  . . . .  13
;        3.8    Queue mess buffers (SCSQRM)  . . . . . . . . .  14
;        3.9    Cancel DG/MSG buffers (SCSCRD/SCSCRM)  . . . .  15
;        3.10   Connect state poll (SCSCSP)  . . . . . . . . .  16
;        3.11   Return connect data (SCSRCD) . . . . . . . . .  17
;        3.12   Status of connect (SCSSTS) . . . . . . . . . .  18
;        3.13   Map a buffer (SCSMAP)  . . . . . . . . . . . .  19
;        3.14   Unmap a buffer (SCSUMP)  . . . . . . . . . . .  20
;        3.15   Send/Request a buffer (SCSSND/SCSREQ)  . . . .  21
;        3.16   Maint. data send/rec (SCSMDS/SCSMDR) . . . . .  22
;        3.17   Start a remote system (SCSSRS) . . . . . . . .  23
;        3.18   Reset a remote system (SCSRRS) . . . . . . . .  24
;        3.19   Add interrupt channel (SCSAIC) . . . . . . . .  25
;        3.20   Accept connection (SCSACC) . . . . . . . . . .  26
;        3.21   Get data queue entry (SCSGDE)  . . . . . . . .  27
;        3.22   Get event Q entry (SCSEVT) . . . . . . . . . .  28
;        3.23   Get local node number (SCSGLN) . . . . . . . .  29
;        3.24   Return buffer sizes (SCSRBS) . . . . . . . . .  30
;        3.25   Return path status info (SCSRPS) . . . . . . .  31
;    4. SCS% error handlers
;        4.1    A2MHLT (PSB mapping bug) . . . . . . . . . . .  32
;        4.2    SERIAA (Invalid argument block address)  . . .  33
;        4.3    SERIBP (Invalid byte pointer)  . . . . . . . .  34
;    5. SCA call handlers
;        5.1    Main entry point . . . . . . . . . . . . . . .  35
;        5.2    SCSONT (Handle online/offline interrupts)  . .  36
;        5.3    SINDGR . . . . . . . . . . . . . . . . . . . .  37
;        5.4    SINDMA . . . . . . . . . . . . . . . . . . . .  38
;        5.5    SINMSC . . . . . . . . . . . . . . . . . . . .  39
;        5.6    SINPBC . . . . . . . . . . . . . . . . . . . .  40
;        5.7    SINCTL . . . . . . . . . . . . . . . . . . . .  41
;        5.8    SINCRA . . . . . . . . . . . . . . . . . . . .  42
;        5.9    SINOSD . . . . . . . . . . . . . . . . . . . .  43
;        5.10   SINRID . . . . . . . . . . . . . . . . . . . .  44
;        5.11   SINCIA . . . . . . . . . . . . . . . . . . . .  45
;        5.12   SINPSC . . . . . . . . . . . . . . . . . . . .  46
;        5.13   SINLCL . . . . . . . . . . . . . . . . . . . .  47
;        5.14   SINNWO . . . . . . . . . . . . . . . . . . . .  48
 ---------------------------------
;    2. Entry to JSYS  . . . . . . . . . . . . . . . . . . . .   6
;    3. Function code handlers
;        3.1    Connect (SCSCON) . . . . . . . . . . . . . . .   7
;        3.2    Listen (SCSLIS)  . . . . . . . . . . . . . . .   8
;        3.3    Receive DG/MSG (SCSRDG/SCSRMG) . . . . . . . .   9
;        3.4    Read port counters (SCSRPC)  . . . . . . . . .  14
;        3.5    Reject (SCSREJ)  . . . . . . . . . . . . . . .  15
;        3.6    Disconnect (SCSDIS)  . . . . . . . . . . . . .  16
;        3.7    Send message/datagram (SCSSMG/SCSSDG)  . . . .  17
;        3.8    Queue mess buffers (SCSQRM)  . . . . . . . . .  20
;        3.9    Cancel DG/MSG buffers (SCSCRD/SCSCRM)  . . . .  21
;        3.10   Connect state poll (SCSCSP)  . . . . . . . . .  22
;        3.11   Return connect data (SCSRCD) . . . . . . . . .  23
;        3.12   Status of connect (SCSSTS) . . . . . . . . . .  24
;        3.13   Map a buffer (SCSMAP)  . . . . . . . . . . . .  25
;        3.14   Unmap a buffer (SCSUMP)  . . . . . . . . . . .  26
;        3.15   Send/Request a buffer (SCSSND/SCSREQ)  . . . .  27
;        3.16   Maint. data send/rec (SCSMDS/SCSMDR) . . . . .  28
;        3.17   Start a remote system (SCSSRS) . . . . . . . .  29
;        3.18   Reset a remote system (SCSRRS) . . . . . . . .  30
;        3.19   Add interrupt channel (SCSAIC) . . . . . . . .  31
;        3.20   Accept connection (SCSACC) . . . . . . . . . .  33
;        3.21   Get data queue entry (SCSGDE)  . . . . . . . .  34
;        3.22   Get event Q entry (SCSEVT) . . . . . . . . . .  35
;        3.23   Get local node number (SCSGLN) . . . . . . . .  36
;        3.24   Return buffer sizes (SCSRBS) . . . . . . . . .  37
;        3.25   Return path status info (SCSRPS) . . . . . . .  38
;    4. SCS% error handlers
;        4.1    A2MHLT (PSB mapping bug) . . . . . . . . . . .  39
;        4.2    SERIAA (Invalid argument block address)  . . .  40
;        4.3    SERIBP (Invalid byte pointer)  . . . . . . . .  41
;    5. SCA call handlers
;        5.1    Main entry point . . . . . . . . . . . . . . .  42
;        5.2    SCSONT (Handle online/offline interrupts)  . .  43
;        5.3    SINDGR . . . . . . . . . . . . . . . . . . . .  45
;        5.4    SINDMA . . . . . . . . . . . . . . . . . . . .  46
;        5.5    SINMSC . . . . . . . . . . . . . . . . . . . .  47
;        5.6    SINPBC . . . . . . . . . . . . . . . . . . . .  50
;        5.7    SINCTL . . . . . . . . . . . . . . . . . . . .  51
;        5.8    SINCRA . . . . . . . . . . . . . . . . . . . .  52
;        5.9    SINOSD . . . . . . . . . . . . . . . . . . . .  53
;        5.10   SINRID . . . . . . . . . . . . . . . . . . . .  54
;        5.11   SINCIA . . . . . . . . . . . . . . . . . . . .  55
;        5.12   SINPSC . . . . . . . . . . . . . . . . . . . .  56
;        5.13   SINLCL . . . . . . . . . . . . . . . . . . . .  57
;        5.14   SINNWO . . . . . . . . . . . . . . . . . . . .  58

***** CHANGE #3; PAGE 5, LINE 9; PAGE 5, LINE 9
;
;				  Section		      Page
;
;
;    6. Support routines
;        6.1    SCSDMU (Data monitor to user)  . . . . . . . .  49
;        6.2    SCSRUB (Remove user buffer)  . . . . . . . . .  50
;        6.3    SCSSUM (Move string from user to monitor)  . .  51
;        6.4    SCSDUM (Move data from user to monitor)  . . .  52
;        6.5    SCSCUB (Count user buffers)  . . . . . . . . .  53
;        6.6    SCSXUB (Link buffer chain into database) . . .  54
;        6.7    SCSUNM (User name move)  . . . . . . . . . . .  55
;        6.8    SCSUDM (User connection data move) . . . . . .  56
;        6.9    SCSCBI (Connect block data init) . . . . . . .  57
;        6.10   SCSABT/SCSRBT (Add/remove bit from table)  . .  58
;        6.11   SCSGNB (Get next bit)  . . . . . . . . . . . .  59
;        6.12   SCSAFT (Add fork to table) . . . . . . . . . .  60
;        6.13   SCSRFT (Remove fork from table)  . . . . . . .  61
;        6.14   SCSGNF (Get next fork) . . . . . . . . . . . .  62
;        6.15   SCSQMD (Queue entry on maint. data Q)  . . . .  63
;        6.16   SCSDMD (Dequeue maint. data queue entry) . . .  64
;        6.17   SCSLFQ (Link packet to front of queue's) . . .  65
;        6.18   SCSAER (ASGRES error handler)  . . . . . . . .  66
;        6.19   SCSLCB (Link CB onto fork CB queue)  . . . . .  67
;        6.20   SCSKIL (Clean up SCS% data)  . . . . . . . . .  68
;        6.21   SCSCBD (JSYS CB data delete) . . . . . . . . .  69
;        6.22   SCSGPR (General packet return) . . . . . . . .  70
;        6.23   SCSCBS (Clean a BSD queue) . . . . . . . . . .  71
;        6.24   SCSRCB (Remove CB from owning fork CB queue) .  72
;        6.25   SCSAXN (Add DMA buffer name) . . . . . . . . .  73
;        6.26   SCSDXN (Delete DMA buffer name)  . . . . . . .  74
;        6.27   SCSFXN (Find a buffer name)  . . . . . . . . .  75
;        6.28   SCSCXN (Clean a buffer name list)  . . . . . .  76
;        6.29   SCSLXN (Link entry onto fork DMA name queue) .  77
;        6.30   SCSRXN (Remove entry from fork DMA nam list) .  78
;        6.31   SCSULK (Unlock pages in a DMA buffer)  . . . .  79
;        6.32   SCSCLK (Clock service routine) . . . . . . . .  80
;        6.33   SCSPSI (Add entry to fork queue) . . . . . . .  81
;        6.34   SCSQUE (Queue a block for list queueing) . . .  82
;        6.35   SCSMBS (Make another BSD)  . . . . . . . . . .  83
;        6.36   SCSDEQ (Dequeue a buffer from fork+CB Q's) . .  84
;        6.37   SCSMPS (Map a forks PSB) . . . . . . . . . . .  85
;        6.38   SCSUPS (Unmap a forks PSB) . . . . . . . . . .  86
;        6.39   SCSLUB (Link on user buffer addr)  . . . . . .  87
;        6.40   SCSGUB (Get user buffer address) . . . . . . .  88
;        6.41   SCSRET (Return buffer to correct pool) . . . .  89
;    7. Storage  . . . . . . . . . . . . . . . . . . . . . . .  90
;    8. End of SCSJSY  . . . . . . . . . . . . . . . . . . . .  91
 ---------------------------------
;
;				  Section		      Page
;
;
;    6. Support routines
;        6.1    SCSDMU (Data monitor to user)  . . . . . . . .  59
;        6.2    SCSRUB (Remove user buffer)  . . . . . . . . .  60
;        6.3    SCSSUM (Move string from user to monitor)  . .  61
;        6.4    SCSDUM (Move data from user to monitor)  . . .  62
;        6.5    SCSCUB (Count user buffers)  . . . . . . . . .  63
;        6.6    SCSXUB (Link buffer chain into database) . . .  64
;        6.7    SCSUNM (User name move)  . . . . . . . . . . .  65
;        6.8    SCSUDM (User connection data move) . . . . . .  66
;        6.9    SCSCBI (Connect block data init) . . . . . . .  67
;        6.10   SCSABT/SCSRBT (Add/remove bit from table)  . .  68
;        6.11   SCSGNB (Get next bit)  . . . . . . . . . . . .  69
;        6.12   SCSAFT (Add fork to table) . . . . . . . . . .  70
;        6.13   SCSRFT (Remove fork from table)  . . . . . . .  71
;        6.14   SCSGNF (Get next fork) . . . . . . . . . . . .  72
;        6.15   SCSQMD (Queue entry on maint. data Q)  . . . .  73
;        6.16   SCSDMD (Dequeue maint. data queue entry) . . .  74
;        6.17   SCSLFQ (Link packet to front of queue's) . . .  75
;        6.18   SCSAER (ASGRES error handler)  . . . . . . . .  76
;        6.19   SCSLCB (Link CB onto fork CB queue)  . . . . .  77
;        6.20   SCSKIL (Clean up SCS% data)  . . . . . . . . .  78
;        6.21   SCSCBD (JSYS CB data delete) . . . . . . . . .  80
;        6.22   SCSGPR (General packet return) . . . . . . . .  82
;        6.23   SCSCBS (Clean a BSD queue) . . . . . . . . . .  83
;        6.24   SCSRCB (Remove CB from owning fork CB queue) .  84
;        6.25   SCSAXN (Add DMA buffer name) . . . . . . . . .  85
;        6.26   SCSDXN (Delete DMA buffer name)  . . . . . . .  86
;        6.27   SCSFXN (Find a buffer name)  . . . . . . . . .  87
;        6.28   SCSCXN (Clean a buffer name list)  . . . . . .  88
;        6.29   SCSLXN (Link entry onto fork DMA name queue) .  89
;        6.30   SCSRXN (Remove entry from fork DMA nam list) .  90
;        6.31   SCSULK (Unlock pages in a DMA buffer)  . . . .  91
;        6.32   SCSCLK (Clock service routine) . . . . . . . .  92
;        6.33   SCSPSI (Add entry to fork queue) . . . . . . .  93
;        6.34   SCSQUE (Queue a block for list queueing) . . .  98
;        6.35   SCSMBS (Make another BSD)  . . . . . . . . . .  99
;        6.36   SCSDEQ (Dequeue a buffer from fork+CB Q's) . . 100
;        6.37   SCSMPS (Map a forks PSB) . . . . . . . . . . . 102
;        6.38   SCSUPS (Unmap a forks PSB) . . . . . . . . . . 104
;        6.39   SCSLUB (Link on user buffer addr)  . . . . . . 105
;        6.40   SCSGUB (Get user buffer address) . . . . . . . 106
;        6.41   SCSRET (Return buffer to correct pool) . . . . 107
;    7. Storage  . . . . . . . . . . . . . . . . . . . . . . . 109
;    8. End of SCSJSY  . . . . . . . . . . . . . . . . . . . . 110

***** CHANGE #4; PAGE 7, LINE 11; PAGE 7, LINE 11
;
SCSINI::BLCAL. (<XENT SC.SOA>,<<.,SCSONT>>) ;[7.1043]Tell SCA about this address
	 BUG. (CHK,SCSNOI,SCSJSY,SOFT,<SCSJSY - SCS% cannot receive node online/offline interrupts>,,<

Cause:	SCA has told the JSYS SYSAP that there are too many SYSAPs and the JSYS
	is not allowed to see online/offline interrupts. The system can run
	but many diagnostics will get upset as will anything that uses the 
	JSYS.
>)
 ---------------------------------
;
SCSINI::BLCAL. (<XENT SC.SOA>,<<.,SCSONT>>) ;[7.1043]Tell SCA about this address
	 BUG. (CHK,SCSNOI,SCSJSY,SOFT,<SCSJSY - SCS% cannot receive node online/offline interrupts>,,<

Cause:	SCA has told the JSYS SYSAP that there are too many SYSAPs and the JSYS
	is not allowed to see online/offline interrupts. The system can run but
	many diagnostics will get upset as will anything that uses the JSYS.

Action:	If this BUGCHK is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.
>)

***** CHANGE #5; PAGE 11, LINE 30; PAGE 11, LINE 30
	CAIGE T1,.LBRDG		;Is the block long enough to return all data in
	ITERR (SCSBTS)		;No, fail with block to short
	UMOVE T1,.SQCID(Q1)	;Get the connect ID from arg block
	ERJMP SERIAA		;Handle bad addresses
	NOINT			;Interlock the fork datagram queue
	CAMN T1,[-1]		;Do we want first for fork or connection???
	IFNSK.
	 SKIPN T1,@.TOPFQ(Q2)	;Is there a packet waiting for this fork???
	 ITERR (SCSQIE,<OKINT>);No, fail with queue is empty
	 LOAD P1,MECID,(T1)	;Get the destination connect ID
 ---------------------------------
	CAIGE T1,.LBRDG		;Is the block long enough to return all data in
	ITERR (SCSBTS)		;No, fail with block to short
	UMOVE T1,.SQCID(Q1)	;Get the connect ID from arg block
	ERJMP SERIAA		;Handle bad addresses
	NOINT			;Interlock the fork datagram queue
	CAME T1,[-1]		;[8936] Do we want first for fork or connection
	IFSKP.			;[8936] Yes
	 SKIPN T1,@.TOPFQ(Q2)	;Is there a packet waiting for this fork???
	 ITERR (SCSQIE,<OKINT>)	;No, fail with queue is empty
	 LOAD P1,MECID,(T1)	;Get the destination connect ID

***** CHANGE #6; PAGE 11, LINE 46; PAGE 11, LINE 46
	 CALL <XENT SC.CSC>	;[7.1043](T1/T1,P1)Sanity check the connect ID
	  ITERR (SCSIID,<OKINT>) ;Nope, not a good one, fail now pls...
	 SKIPN T1,@.TOPCQ(Q2)	;Are there datagrams queued for this connect???
	 ITERR (SCSQIE,<OKINT>) ;Nothing on the queue, fail with queue empty
	ENDIF.

 ---------------------------------
	 CALL <XENT SC.CSC>	;[7.1043](T1/T1,P1)Sanity check the connect ID
	  ITERR (SCSIID,<OKINT>) ;Nope, not a good one, fail now pls...
	 SKIPN T1,@.TOPCQ(Q2)	;Are there datagrams queued for this connect???
	 ITERR (SCSQIE,<OKINT>) ;Nothing on the queue, fail with queue empty
	ENDIF.


***** CHANGE #7; PAGE 11, LINE 54; PAGE 12, LINE 8
;	T1/	Address of desired packet buffer
;	Q1/	Address of user arguments
;	Q2/	Address of 4 word header block pointing to list headers
;	P1/	Address of connect block
;
	SKIPG @.JBUFF(Q2)	;Are there datagram buffers queued???
	IFNSK.
	 MOVX T1,SCSNMQ		;Assume we are doing messages
 ---------------------------------
;	T1/	Address of desired packet buffer
;	Q1/	Address of user arguments
;	Q2/	Address of 4 word header block pointing to list headers
;	P1/	Address of connect block
;
	SKIPLE @.JBUFF(Q2)	;[8936] Are there datagram buffers queued???
	IFSKP.			;[8936]
	 MOVX T1,SCSNMQ		;Assume we are doing messages

***** CHANGE #8; PAGE 11, LINE 86; PAGE 12, LINE 40
	TDNN T2,.METYP(T4)	;Is this an industry compatable packet???
	SUBI T1,.MHLNB		;Yes, account for the packet header
	MOVEM T1,LEN		;Save the corrected length of the packet
	UMOVEM T1,.SQLRP(Q1)	;Tell user how long the datagram is
	ERJMP RDGER1		;Handle write locked user pages and such
	TDNN T2,.METYP(T4)	;Is this an industry compatable mode packet???
 ---------------------------------
	TDNN T2,.METYP(T4)	;Is this an industry compatable packet???
	SUBI T1,.MHLNB		;Yes, account for the packet header
	MOVEM T1,LEN		;Save the corrected length of the packet
	UMOVEM T1,.SQLRP(Q1)	;Tell user how long the datagram is
	ERJMP RDGER1		;Handle write locked user pages and such

	TDNN T2,.METYP(T4)	;Is this an industry compatable mode packet???

***** CHANGE #9; PAGE 11, LINE 133; PAGE 13, LINE 48
	LOAD T2,CBDNOD,(P1)	;Now get the remote node number for this 
	XCTU [HRRM T2,.SQDFL(Q1)] ;Store node number for the user
	ERJMP SERIA0		;Handle a bad address
	OKINT
	RETSKP			; and return all happy

 ---------------------------------
	LOAD T2,CBDNOD,(P1)	;Now get the remote node number for this 
	XCTU [HRRM T2,.SQDFL(Q1)] ;Store node number for the user
	ERJMP SERIA0		;Handle a bad address
	OKINT
	RETSKP			; and return all happy


***** CHANGE #10; PAGE 11, LINE 160; PAGE 14, LINE 27
	MOVX T2,.BDFDG		;No, get BSD offset for datagrams
	CALL SCSLUB		;(T1,T2,P1) Link this user address back into database
	IFNSK.
	 MOVE T2,FORKX		;Say who we are
	 MOVE T3,USRBUF	;  and what buffer we tried for
	 BUG. (INF,SCSUBL,SCSJSY,SOFT,<SCSJSY - User buffer lost during error recovery>,<<T1,ERRCOD>,<T2,CURFRK>,<T3,BUFADR>>,<

Cause:	Bad access to user memory or a failing routine caused SCS to try to 
	place the currently owned user buffer back on the buffer list. The
	attempt failed and the buffer address has been lost. Note that there
	is no memory loss, the monitor has just forgotten one user buffer
	address.

DATA:	ERRCOD - Erro code
	CURFRK - Current fork
	BUFADR - Buffer address
>);End of BUG. SCSUBL
	ENDIF.			;Fall into packet return code please...

 ---------------------------------
	MOVX T2,.BDFDG		;No, get BSD offset for datagrams
	CALL SCSLUB		;(T1,T2,P1) Link this user address back into database
	IFNSK.
	 MOVE T2,FORKX		;Say who we are
	 MOVE T3,USRBUF	;  and what buffer we tried for
	 BUG.(INF,SCSUBL,SCSJSY,SOFT,<SCSJSY - User buffer lost during error recovery>,<<T1,ERRCOD>,<T2,CURFRK>,<T3,BUFADR>>,<

Cause:	Bad access to user memory or a failing routine caused SCS to try to
	place the currently owned user buffer back on the buffer list.  The
	attempt failed and the buffer address has been lost.  Note that there
	is no memory loss, the monitor has just forgotten one user buffer
	address.

Action:	If this BUGINF is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.

Data:	ERRCOD - Erro code
	CURFRK - Current fork
	BUFADR - Buffer address
>);End of BUG. SCSUBL
	ENDIF.			;Fall into packet return code please...


***** CHANGE #11; PAGE 15, LINE 33; PAGE 19, LINE 33
	CALL <XENT SC.CSC>	;[7.1043](T1/T1,P1)Is the ID ok???
	 ITERR (<>,<OKINT>)	;No, fail with invalid ID
	MOVEM T1,CID		;Stash the connect ID for later

	MOVX T1,1		;We wish one packet buffer please
	SKIPN MSGFLG		;Are we doing messages???
	IFNSK.
	 CALL <XENT SC.ALD>	;[7.1043]Get a monitor buffer for the data
 ---------------------------------
	CALL <XENT SC.CSC>	;[7.1043](T1/T1,P1)Is the ID ok???
	 ITERR (<>,<OKINT>)	;No, fail with invalid ID
	MOVEM T1,CID		;Stash the connect ID for later

	MOVX T1,1		;We wish one packet buffer please
	SKIPE MSGFLG		;[8936] Are we doing messages???
	IFSKP.			;[8936]
	 CALL <XENT SC.ALD>	;[7.1043]Get a monitor buffer for the data

***** CHANGE #12; PAGE 15, LINE 47; PAGE 19, LINE 47
	  ITERR (<>,<OKINT>)	;None available, fail please
	 MOVX T2,JH%DGB		;Get the datagram buffer flag
	 ANDCAM T2,.JHFLG(T1)	;Zero the datagram flag in the JSYS header
	ENDIF.
	MOVEM T1,BUFADR		;Save the buffer address

 ---------------------------------
	  ITERR (<>,<OKINT>)	;None available, fail please
	 MOVX T2,JH%DGB		;Get the datagram buffer flag
	 ANDCAM T2,.JHFLG(T1)	;Zero the datagram flag in the JSYS header
	ENDIF.
	MOVEM T1,BUFADR		;Save the buffer address


***** CHANGE #13; PAGE 15, LINE 91; PAGE 20, LINE 44
	CAILE T3,.SSHGH		;  or too high???
	IFNSK.
	 MOVX T1,SCSIPS		;Yes, fail with invalid path spec.
	 JRST SDGERR		;.	.	.
	ENDIF.

	UMOVE T4,.SQLPT(Q1)	;Get the length of the packet text from user
	ERJMP SDGER1		;Handle a bad address

	SKIPN MSGFLG		;Are we doing messages???
	IFNSK.
	 BLCAL. (<XENT SC.SDG>,<CID,T2,T4,BUFADR,[SCSPRI],T3>) ;[7.1043]Have SCA send the datagram
	  JRST SDGERR		;Fail with code from SCA
;**;[7211] Add 1 line at SCSSMG+86	CEG 18-Dec-85
	 AOS SCSJDS		;[7211] COUNT THE DATAGRAM SEND
 ---------------------------------
	CAILE T3,.SSHGH		;  or too high???
	IFNSK.
	 MOVX T1,SCSIPS		;Yes, fail with invalid path spec.
	 JRST SDGERR		;.	.	.
	ENDIF.

	UMOVE T4,.SQLPT(Q1)	;Get the length of the packet text from user
	ERJMP SDGER1		;Handle a bad address

	SKIPE MSGFLG		;[8936] Are we doing messages???
	IFSKP.			;[8936]
	 BLCAL. (<XENT SC.SDG>,<CID,T2,T4,BUFADR,[SCSPRI],T3>) ;[7.1043]Have SCA send the datagram
	  JRST SDGERR		;Fail with code from SCA
	 AOS SCSJDS		;[7211] COUNT THE DATAGRAM SEND

***** CHANGE #14; PAGE 21, LINE 98; PAGE 27, LINE 98
	  IDIVI T3,11
	  SKIPE T4		;REMAINDER?
	  AOS T3		;YES. ADD ONE TO THE WORD COUNT
	ENDIF.
	MOVEM T3,WRDLEN		;SAVE THE NUMBER OF WORDS NEEDED.
	CAILE T3,PGSIZ		;Is the block not longer than one page???
	IFNSK.
	 MOVEI T1,SCSSTL	;Yes, error code for segment to long
 ---------------------------------
	  IDIVI T3,11
	  SKIPE T4		;REMAINDER?
	  AOS T3		;YES. ADD ONE TO THE WORD COUNT
	ENDIF.
	MOVEM T3,WRDLEN		;SAVE THE NUMBER OF WORDS NEEDED.
	CAIG T3,PGSIZ		;[8936] Is the block not longer than one page?
	IFSKP.			;[8936]
	 MOVEI T1,SCSSTL	;Yes, error code for segment to long

***** CHANGE #15; PAGE 27, LINE 38; PAGE 33, LINE 38
	CAILE T1,.SIHGH		;It this within range of reasonable codes???
	ITERR (SCSIST)		;No, fail
	MOVEM T1,CODE		;Save the interrupt code
	MOVEM T2,CHAN		;Save the current channel number as well
	XCT PSBPSI(T1)		;Store the current channel in the PSB

 ---------------------------------
	CAILE T1,.SIHGH		;It this within range of reasonable codes???
	ITERR (SCSIST)		;No, fail
	MOVEM T1,CODE		;Save the interrupt code
	MOVEM T2,CHAN		;Save the current channel number as well
	XCT PSBPSI(T1)		;Store the current channel in the PSB


***** CHANGE #16; PAGE 27, LINE 47; PAGE 34, LINE 9
;interrupt codes ignore FRKTAB.

	CAIE T1,.SIPAN		;Are we doing events???
	JRST AICCON		;No, nothing needs to be done with FRKTAB
	MOVE T1,FORKX		;Get number of fork doing this SCS% JSYS
	CAIE T2,-1		;Are we disabling event interrupts???
	IFNSK.
	 CALL SCSAFT		;(T1)No, add this fork to FRKTAB
 ---------------------------------
;interrupt codes ignore FRKTAB.

	CAIE T1,.SIPAN		;Are we doing events???
	JRST AICCON		;No, nothing needs to be done with FRKTAB
	MOVE T1,FORKX		;Get number of fork doing this SCS% JSYS
	CAIN T2,-1		;[8936] Are we disabling event interrupts???
	IFSKP.			;[8936]
	 CALL SCSAFT		;(T1)No, add this fork to FRKTAB

***** CHANGE #17; PAGE 28, LINE 37; PAGE 35, LINE 37
	MOVEM T2,DATRET		;  and thre address of the space return routine

	UMOVE T1,.SQCID(Q1)	;Get the connect ID from the user
	ERJMP ACCER1		;Handle bad addresses pls...
	CALL <XENT SC.CSC>	;[7.1043](T1/T1,P1)Is this a valid CID???
	IFNSK.
	 JRST ACCERR		;Handle the error please
	ENDIF.
	BLCAL. (<XENT SC.ACC>,<T1,DATADR,[0],[0]>) ;[7.1043]Do the accept call
 ---------------------------------
	MOVEM T2,DATRET		;  and thre address of the space return routine

	UMOVE T1,.SQCID(Q1)	;Get the connect ID from the user
	ERJMP ACCER1		;Handle bad addresses pls...
	CALL <XENT SC.CSC>	;[7.1043](T1/T1,P1)Is this a valid CID???
	 JRST ACCERR		;[8936] Handle the error please
	BLCAL. (<XENT SC.ACC>,<T1,DATADR,[0],[0]>) ;[7.1043]Do the accept call

***** CHANGE #18; PAGE 34, LINE 3; PAGE 41, LINE 3
	ERJMP SERIAA		;Handle bad addresses
	RETSKP			;All done, return
	SUBTTL SCS% error handlers -- A2MHLT (PSB mapping bug)

A2MHLT:	 BUG. (HLT,SCSA2M,SCSJSY,SOFT,<SCSJSY - Attempt to map second PSB>,<<MPSFRK,OWNFRK>,<T1,CURFRK>>,<

Cause:	Some routine mapped a PSB but did not release it, or did not use the 
	correct interlock. The net result was that we are trying to map another
 ---------------------------------
	ERJMP SERIAA		;Handle bad addresses
	RETSKP			;All done, return
	SUBTTL SCS% error handlers -- A2MHLT (PSB mapping bug)

A2MHLT:	 BUG.(HLT,SCSA2M,SCSJSY,SOFT,<SCSJSY - Attempt to map second PSB>,<<MPSFRK,OWNFRK>,<T1,CURFRK>>,<

Cause:	Some routine mapped a PSB but did not release it, or did not use the
	correct interlock. The net result was that we are trying to map another

***** CHANGE #19; PAGE 38, LINE 20; PAGE 45, LINE 20
;	No data returned
;
SCSONT:	TRVAR <NOD,CODE,FORK>
	CAIN T1,.SSPBC		;Did the node go offline???
	MOVX T4,.SEPBC		;Yes, get the event code for node offline
	CAIN T1,.SSNCO		;Did the node come online???
	IFNSK.
	  MOVX T4,.SENCO	;Yes, get the event code for node online
 ---------------------------------
;	No data returned
;
SCSONT:	TRVAR <NOD,CODE,FORK>
	CAIN T1,.SSPBC		;Did the node go offline???
	MOVX T4,.SEPBC		;Yes, get the event code for node offline
	CAIE T1,.SSNCO		;[8936] Did the node come online???
	IFSKP.			;[8936]
	  MOVX T4,.SENCO	;Yes, get the event code for node online

***** CHANGE #20; PAGE 38, LINE 39; PAGE 45, LINE 39
	 RET			;No more forks enabled, all done
	CALL ONTBLD		;Build and queue a block for this fork
	 $SKIP			;Failed, BUGCHK and loop again
	JRST ONTLOP		;All went OK, loop for more forks

	BUG. (CHK,SCSPBF,SCSJSY,SOFT,<SCSJSY - PSI block build failure>,<<T1,ERRCOD>>,<

 ---------------------------------
	 RET			;No more forks enabled, all done
	CALL ONTBLD		;Build and queue a block for this fork
	 $SKIP			;Failed, BUGCHK and loop again
	JRST ONTLOP		;All went OK, loop for more forks

	BUG.(CHK,SCSPBF,SCSJSY,SOFT,<SCSJSY - PSI block build failure>,<<T1,ERRCOD>>,<


***** CHANGE #21; PAGE 38, LINE 47; PAGE 45, LINE 47
Cause:	The routine to build an event block failed. It is very likely that
	ASGRES did not have the space available.

Data:	ERRCOD -  Error code returned by ONTBLD
>,ONTLOP)

 ---------------------------------
Cause:	The routine to build an event block failed. It is very likely that
	ASGRES did not have the space available.

Data:	ERRCOD -  Error code returned by ONTBLD
>,ONTLOP)


***** CHANGE #22; PAGE 41, LINE 20; PAGE 49, LINE 20
MSCLOP:	CAMN T2,.MQBUF(T1)	;Is this the entry we are looking for???
	JRST MSCOK		;Yes, go process it please
	SKIPE T1,.MQNXT(T1)	;Is there a next entry???
	JRST MSCLOP		;Can't find an entry for this name...
	JRST MSCERR		;Nope, die here please...

 ---------------------------------
MSCLOP:	CAMN T2,.MQBUF(T1)	;Is this the entry we are looking for???
	JRST MSCOK		;Yes, go process it please
	SKIPE T1,.MQNXT(T1)	;Is there a next entry???
	JRST MSCLOP		;Can't find an entry for this name...
	JRST MSCERR		;Nope, die here please...


***** CHANGE #23; PAGE 41, LINE 52; PAGE 50, LINE 32
	SETZRO MEFLG,(T1)	;Zero the flags
	CALLRET SCSQUE		;Place this entry on the appropriate q's

; Here when we cannot find the buffer name SCA has just given us...
;
MSCERR:	BUG. (CHK,SCSCFB,SCSJSY,SOFT,<SCSJSY - Can't find maintainance buffer name>,<<T2,BFRNAM>>,<

Cause:	SCA called us with a buffer name for a completed maintainance data 
	transfer. Upon trying to find this name in the database of outstanding 
	JSYS maint xfers, no match was found. The fork which performed the 
	maint data xfer request will never be notified of the completion of 
	the request.

Action: Try to find the buffer name in the port driver database of BSD/BHD's
	and see if this is a valid buffer name. If so, check the consistancy
	of the SCA maint Q. Other than this, not much can be done...

 ---------------------------------
	SETZRO MEFLG,(T1)	;Zero the flags
	CALLRET SCSQUE		;Place this entry on the appropriate q's

; Here when we cannot find the buffer name SCA has just given us...
;
MSCERR:	BUG.(CHK,SCSCFB,SCSJSY,SOFT,<SCSJSY - Can't find maintainance buffer name>,<<T2,BFRNAM>>,<

Cause:	SCA called us with a buffer name for a completed maintenance data
	transfer.  Upon trying to find this name in the database of outstanding
	JSYS maint transfers, no match was found.  The fork which performed the
	maint data transfer request will never be notified of the completion of
	the request.

Action:	If this BUGCHK is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.

	In the dump, try to find the buffer name in the port driver database of
	BSD/BHD's and see if this is a valid buffer name. If so, check the
	consistancy of the SCA maint queue.  Other than this, not much can be
	done.


***** CHANGE #24; PAGE 41, LINE 69; PAGE 50, LINE 53
Data:	BFRNAM - Buffer name

>)
	RETBAD ()


 ---------------------------------
Data:	BFRNAM - Buffer name

>)
	RETBAD ()


***** CHANGE #25; PAGE 41, LINE 77; PAGE 51, LINE 7
; Here on a bizzare failure.
;
; Expects:
;	T1/	Error code
;
MSCER1:	BUG. (CHK,SCSBIZ,SCSJSY,SOFT,<SCSJSY - Maintainance data transfer Q smashed>,<<T1,ERRCOD>>,<

Cause:	A routine which should not return (+1), did.

 ---------------------------------
; Here on a bizzare failure.
;
; Expects:
;	T1/	Error code
;
MSCER1:	BUG.(CHK,SCSBIZ,SCSJSY,SOFT,<SCSJSY - Maintenance data transfer queue smashed>,<<T1,ERRCOD>>,<

Cause:	A routine which should not return (+1), did.  This indicates that a
	maintenance data transfer queue is damaged.

Action:	If this BUGCHK is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.


***** CHANGE #26; PAGE 55, LINE 45; PAGE 65, LINE 45
	JRST CUBERR		;No, fail
	SOS Q2			;Yes.Set for the end of the buffer
	XCTU [MOVES (Q2)]	;Touch the end of the buffer
	ERJMP CUBERR		;Failed, report bad page access
	AOS T3			;Count the buffer
	CAIL T3,C%MXBF		;Have we exceeded a reasonable number of buf?
	 IFNSK.
	 MOVX T1,SCSENB		;Yes, get the error code for too many buffers
	 RETBAD ()		;Report failure
	 ENDIF.
	XCTU [MOVE Q1,(Q1)]	;Get the addr of the next buffer
 ---------------------------------
	JRST CUBERR		;No, fail
	SOS Q2			;Yes.Set for the end of the buffer
	XCTU [MOVES (Q2)]	;Touch the end of the buffer
	ERJMP CUBERR		;Failed, report bad page access
	AOS T3			;Count the buffer
	CAIGE T3,C%MXBF		;[8936] Have we exceeded a reasonable number of buf?
	IFSKP.			;[8936]
	 MOVX T1,SCSENB		;Yes, get the error code for too many buffers
	 RETBAD ()		;Report failure
	ENDIF.
	XCTU [MOVE Q1,(Q1)]	;Get the addr of the next buffer

***** CHANGE #27; PAGE 60, LINE 39; PAGE 70, LINE 39
	IDIVI T3,^D36		;Turn bit number into word + bit offset
	ADD T3,T2		;Add table offset to table base addr
	MOVE T4,BITS(T4)	;Light the bit from the bit number
	MOVE T1,(T3)		;Save the origional contents of the table

	SKIPN ABTCOD		;Are we adding the bit???
	IFNSK.
	 ANDCAM T4,(T3)		;Turn off the bit please
 ---------------------------------
	IDIVI T3,^D36		;Turn bit number into word + bit offset
	ADD T3,T2		;Add table offset to table base addr
	MOVE T4,BITS(T4)	;Light the bit from the bit number
	MOVE T1,(T3)		;Save the origional contents of the table

	SKIPE ABTCOD		;[8936] Are we adding the bit???
	IFSKP.			;[8936]
	 ANDCAM T4,(T3)		;Turn off the bit please

***** CHANGE #28; PAGE 68, LINE 26; PAGE 78, LINE 26
;
SCSAER:	POP P,T2		;Get the callers PC back
	TXZ T2,<777740,,0>	;Keep just the PC part
	BUG. (INF,SCSACF,SCSJSY,SOFT,<SCSJSY - Can't get resident space from ASGRES>,<<T1,ERRCOD>,<T2,CALLPC>>,<

Cause:	A call to ASGRES (by the JSYS) has failed. With the error code and 
	caller's PC given by the BUGINF, figuring out why it failed should
	be easy enough.

 ---------------------------------
;
SCSAER:	POP P,T2		;Get the callers PC back
	TXZ T2,<777740,,0>	;Keep just the PC part
	BUG. (INF,SCSACF,SCSJSY,SOFT,<SCSJSY - Can't get resident space from ASGRES>,<<T1,ERRCOD>,<T2,CALLPC>>,<

Cause:	A call to ASGRES (by the JSYS) has failed. With the error code and
	caller's PC given by the BUGINF, figuring out why it failed should be
	easy enough.

Action:	If this BUGCHK is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.


***** CHANGE #29; PAGE 69, LINE 21; PAGE 79, LINE 21
;	P1/	Address routine was called with
;
SCSLCB:	
	MOVE T1,SCSBCQ		;Get addr of last entry on the queue
	XMOVEI T2,SCSTCQ	;  and the addr of the top of queue pointer
	CAME T1,T2		;Is BLINK a pointer to the FLINK???
	IFNSK.
	 MOVEM T1,.CBJPB(P1)	;No, update previous pointer of new CB
 ---------------------------------
;	P1/	Address routine was called with
;
SCSLCB:	
	MOVE T1,SCSBCQ		;Get addr of last entry on the queue
	XMOVEI T2,SCSTCQ	;  and the addr of the top of queue pointer
	CAMN T1,T2		;[8936] Is BLINK a pointer to the FLINK???
	IFSKP.			;[8936]
	 MOVEM T1,.CBJPB(P1)	;No, update previous pointer of new CB

***** CHANGE #30; PAGE 70, LINE 26; PAGE 80, LINE 26
				;[7.1043]Alternate entry name for FLOGO (MEXEC)
XNENT	(SCSKIL,G)		;[7.1043]SCSKIL::, XSCSKI::
	SAVEAC <T1,T2,T3,T4,P1>	;Save ALL ACs used here
	TRVAR <FRKN,PSBADR,FLNK,BLNK>
	MOVEM T1,FRKN		;Save the fork number for later
;
; NOTE: The folowing two lines are a temporary hack to prevent SCSFR1
; BUGCHKs from FRKCHK when SCSKIL is called from CLZFF%.  The correct
; action here is to allow SCSKIL to clean up CI connections owned by a
; fork inferior to the current fork.
;
	CAME T1,FORKX		;IS THE CURRENT FORK
 ---------------------------------
				;[7.1043]Alternate entry name for FLOGO (MEXEC)
XNENT	(SCSKIL,G)		;[7.1043]SCSKIL::, XSCSKI::
	SAVEAC <T1,T2,T3,T4,P1>	;Save ALL ACs used here
	TRVAR <FRKN,PSBADR,FLNK,BLNK>
	MOVEM T1,FRKN		;Save the fork number for later

; NOTE: The following two lines are a temporary hack to prevent SCSFR1 BUGCHKs
; from FRKCHK when SCSKIL is called from CLZFF%.  The correct action here is to
; allow SCSKIL to clean up CI connections owned by a fork inferior to the
; current fork.

	CAME T1,FORKX		;IS THE CURRENT FORK

***** CHANGE #31; PAGE 70, LINE 43; PAGE 80, LINE 43
	CALL SCSRFT		;(T1)Remove this fork from the PSI table
	 NOP			;I don't care if it wasn't in the table
	XMOVEI T1,SCSTCQ
	MOVEM T1,FLNK		;Save the address of the queue FLINK

	XMOVEI T3,SCSBCQ		;Get 18 bits of queue BLINK address
	MOVEM T3,BLNK		;Save the address of the queue BLINK
 ---------------------------------
	CALL SCSRFT		;(T1)Remove this fork from the PSI table
	 NOP			;I don't care if it wasn't in the table
	XMOVEI T1,SCSTCQ
	MOVEM T1,FLNK		;Save the address of the queue FLINK

	XMOVEI T3,SCSBCQ	;Get 18 bits of queue BLINK address
	MOVEM T3,BLNK		;Save the address of the queue BLINK

***** CHANGE #32; PAGE 70, LINE 54; PAGE 80, LINE 54

	CALL SCSCXN		;Delete any DMA resources owned by
				;this fork
	OKINT
	RET			;All done, return

 ---------------------------------

	CALL SCSCXN		;Delete any DMA resources owned by
				;this fork
	OKINT
	RET			;All done, return


***** CHANGE #33; PAGE 70, LINE 83; PAGE 81, LINE 29
	XMOVEI T1,SCSTCQ	;Get extended address of queue FLINK
	MOVEM T1,@BLNK		;Re-init headers for now empty queue
	SETZM @FLNK		;Zero the queue FLINK
	RET			;No more connect blocks, all done


; Here when SC.DIS fails.
 ---------------------------------
	XMOVEI T1,SCSTCQ	;Get extended address of queue FLINK
	MOVEM T1,@BLNK		;Re-init headers for now empty queue
	SETZM @FLNK		;Zero the queue FLINK
	RET			;No more connect blocks, all done

; Here when SC.DIS fails.

***** CHANGE #34; PAGE 70, LINE 90; PAGE 81, LINE 35
;
KILERR:	 OKINT			;SCA is done, allow interrupts again
	 CAIN T1,SCSIID		;Is the error, bad CID
	 JRST KILCHK		;Yes, then connect is already gone, no problem
	 BUG. (CHK,SCSABF,SCSJSY,SOFT,<SCSJSY - Connection abort failure on fork delete>,<<T1,ERRCOD>>,<

Cause:	During the deletion process for a fork we tried to abort the 
	connections it had open. We failed in the attempt. 

 ---------------------------------
;
KILERR:	 OKINT			;SCA is done, allow interrupts again
	 CAIN T1,SCSIID		;Is the error, bad CID
	 JRST KILCHK		;Yes, then connect is already gone, no problem
	 BUG.(CHK,SCSABF,SCSJSY,SOFT,<SCSJSY - Connection abort failure on fork delete>,<<T1,ERRCOD>>,<

Cause:	During the deletion process for a fork we tried to abort the
	connections it had open.  We failed in the attempt.

Action:	If this BUGCHK is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.


***** CHANGE #35; PAGE 71, LINE 46; PAGE 82, LINE 46
	XMOVEI T1,.CBTXQ(P1)	;Point to the top of the queue
	XMOVEI T2,.CBBXQ(P1)	;  and the bottom of the queue
	MOVE T3,[MSEC1,,RELRES]	;[7.1043]  and the routine to return the space
	XMOVEI T4,XFER		;  and the 4 word block for DMA
	CALL SCSGPR		;(T1,T2,T3,T4,P1)Return the blocks on the queue

 ---------------------------------
	XMOVEI T1,.CBTXQ(P1)	;Point to the top of the queue
	XMOVEI T2,.CBBXQ(P1)	;  and the bottom of the queue
	MOVE T3,[MSEC1,,RELRES]	;[7.1043]  and the routine to return the space
	XMOVEI T4,XFER		;  and the 4 word block for DMA
	CALL SCSGPR		;(T1,T2,T3,T4,P1)Return the blocks on the queue


***** CHANGE #36; PAGE 71, LINE 79; PAGE 83, LINE 33
	  SETONE CBFRAP,(P1)	;Yes, light reap as well
	  SETONE CIREP		;[7122] TELL CIFORK TO RUN
	ENDIF.
	RET			;All done

CBDCHK:	BUG. (CHK,SCSCDC,SCSJSY,SOFT,<SCSJSY - Cannot delete connect block from fork queue>,<<T1,ERRCOD>>,<

Cause:	We tried to remove a connect block from the owning fork's list of 
	connect blocks. The most likely failure is a +1 return from SCSMPS.
	This fails only when we map a PSB but do not unmap it.

 ---------------------------------
	  SETONE CBFRAP,(P1)	;Yes, light reap as well
	  SETONE CIREP		;[7122] TELL CIFORK TO RUN
	ENDIF.
	RET			;All done

CBDCHK:	BUG.(CHK,SCSCDC,SCSJSY,SOFT,<SCSJSY - Cannot delete connect block from fork queue>,<<T1,ERRCOD>>,<

Cause:	We tried to remove a connect block from the owning fork's list of
	connect blocks.  The most likely failure is a +1 return from SCSMPS.
	This fails only when we map a PSB but do not unmap it.

Action:	If this BUGCHK is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.


***** CHANGE #37; PAGE 83, LINE 18; PAGE 95, LINE 18
;	No arguments
;
;	Return (+1) Always to PIRQR
;	No data returned
;
;
;THE .MEANC OFFSET OF EACH QUEUE ENTRY IS USED AS A FORWARD POINTER
;TO THE NEXT ENTRY. THE .MEAPC OFFSET IS USED AS A BACKWARD POINTER.
;THIS IS TRUE ONLY BETWEEN THE TIMES WHEN THE ENTRY
;IS QUEUED (VIA QUEFRK) AND WHEN IT IS REMOVED (VIA SCSPSI). AFTER THE
;ENTRY IS REMOVED FROM THE SCSTPQ LIST, THE .MEANC AND .MEAPC OFFSETS
;ARE USED FOR ANOTHER PURPOSE.
;
XRENT	(SCSPSI,G)		;[7.1043]SCSPSI::, XSCSPS::
 ---------------------------------
;	No arguments
;
;	Return (+1) Always to PIRQR
;	No data returned
;
;The .MEANC offset of each queue entry is used as a forward pointer to the next
;entry. The .MEAPC offset is used as a backward pointer.  This is true only
;between the times when the entry is queued (via QUEFRK) and when it is removed
;(via SCSPSI).  After the entry is removed from the SCSTPQ list, the .MEANC and
;.MEAPC offsets are used for another purpose.

XRENT	(SCSPSI,G)		;[7.1043]SCSPSI::, XSCSPS::

***** CHANGE #38; PAGE 83, LINE 50; PAGE 95, LINE 48
	  JRST PSIEXI		;NO. FINISHED.
	ELSE.			;DID NOT KNOW OF ANY OTHER ENTRIES ON QUEUE.
	  SKIPN T1,SCSTPQ	;HAVE ANY ENTRIES BEEN ADDED?
	  JRST PSIEXI		;NO. FINISHED FOR THIS FORK.
	ENDIF.
				;T1 SHOULD NOW HAVE THE ADDRESS OF AN ENTRY
 ---------------------------------
	  JRST PSIEXI		;NO. FINISHED.
	ELSE.			;DID NOT KNOW OF ANY OTHER ENTRIES ON QUEUE.
	  SKIPN T1,SCSTPQ	;HAVE ANY ENTRIES BEEN ADDED?
	  JRST PSIEXI		;NO. FINISHED FOR THIS FORK.
	ENDIF.
				;T1 SHOULD NOW HAVE THE ADDRESS OF AN ENTRY

***** CHANGE #39; PAGE 83, LINE 92; PAGE 96, LINE 42
	ENDIF.			;ENTRY NOW DELETED FROM SCSTPQ. CONTINUE ON.
	SETZM SCSLCK		;UNDO THE SCSLCK LOCK.

	MOVEM T1,BLKADR		;Save the event block address
	LOAD T1,MECID,(T1)	;Get the CID from the event block
	CAMN T1,[-1]		;Is this a special block with no CID???
	IFNSK.
	 SETO P1,0		;Yes, special block, show address of CB as -1
	 JRST PSINDL		;Yes, no CB work to do
	ENDIF.

 ---------------------------------
	ENDIF.			;ENTRY NOW DELETED FROM SCSTPQ. CONTINUE ON.
	SETZM SCSLCK		;UNDO THE SCSLCK LOCK.

	MOVEM T1,BLKADR		;Save the event block address
	LOAD T1,MECID,(T1)	;Get the CID from the event block
	CAME T1,[-1]		;[8936] Is this a special block with no CID???
	IFSKP.			;[8936] yes
	 SETO P1,0		;Yes, special block, show address of CB as -1
	 JRST PSINDL		;Yes, no CB work to do
	ENDIF.


***** CHANGE #40; PAGE 83, LINE 129; PAGE 97, LINE 32
	MOVE T3,(Q2)		;Get the current last entry
	CAME T3,Q1		;Is this a pointer to the q head pointer???
	MOVEM T3,.MEAPC(T1)	;No, update the previous pointer of new entry
	MOVEM T1,@Q2		;Link this entry onto the end of the queue
	MOVEM T1,(Q2)		;  and update the tail pointer

 ---------------------------------
	MOVE T3,(Q2)		;Get the current last entry
	CAME T3,Q1		;Is this a pointer to the q head pointer???
	MOVEM T3,.MEAPC(T1)	;No, update the previous pointer of new entry
	MOVEM T1,@Q2		;Link this entry onto the end of the queue
	MOVEM T1,(Q2)		;  and update the tail pointer


***** CHANGE #41; PAGE 83, LINE 146; PAGE 98, LINE 17
	MOVE Q2,.BOTFQ(T2)	;Get addr of the fork queue BLINK
	SETZM .MEANF(T1)	;No next on fork queue since we are last
	SETZM .MEAPF(T1)	;  and for now, assume no previous
	MOVE T4,@Q1		;Save the current state of the queue
	MOVE T3,@Q2		;Get addr of current last entry
	CAMN T3,Q1		;Is this a pointer to the FLINK???
	IFNSK.
	 MOVEM T1,(Q1)		;Yes, update the fork Q FLINK
 ---------------------------------
	MOVE Q2,.BOTFQ(T2)	;Get addr of the fork queue BLINK
	SETZM .MEANF(T1)	;No next on fork queue since we are last
	SETZM .MEAPF(T1)	;  and for now, assume no previous
	MOVE T4,@Q1		;Save the current state of the queue
	MOVE T3,@Q2		;Get addr of current last entry
	CAME T3,Q1		;[8936] Is this a pointer to the FLINK???
	IFSKP.			;[8936]
	 MOVEM T1,(Q1)		;Yes, update the fork Q FLINK

***** CHANGE #42; PAGE 83, LINE 164; PAGE 98, LINE 35
	JRST PSINXT		;Yes, don't give a PSI
	MOVE T3,BLKADR		;Get the event block address again
	LOAD T2,MEFRK,(T3)	;Get the fork to interrupt
	CALLX (MSEC1,PSIRQ)	;[7.1043]Queue a PSI interrupt for the fork
	JRST PSINXT		;Loop for more entries

 ---------------------------------
	JRST PSINXT		;Yes, don't give a PSI
	MOVE T3,BLKADR		;Get the event block address again
	LOAD T2,MEFRK,(T3)	;Get the fork to interrupt
	CALLX (MSEC1,PSIRQ)	;[7.1043]Queue a PSI interrupt for the fork
	JRST PSINXT		;Loop for more entries


***** CHANGE #43; PAGE 83, LINE 172; PAGE 99, LINE 8
;entry.

PSIBER:	MOVE T1,BLKADR		;Get the free space addr back
	LOAD T2,METYP,(T1)	;Get the entry type
	CAILE T2,.ETDMA		;Check entry type
PSIBUG:	BUG. (CHK,SCSBDE,SCSJSY,SOFT,<SCSJSY - Bad entry type found>,<<T2,TYPE>,<T1,BLKADR>>,<

Cause: An illegal type of message buffer was attempted to be returned.
       It is now lost.

 ---------------------------------
;entry.

PSIBER:	MOVE T1,BLKADR		;Get the free space addr back
	LOAD T2,METYP,(T1)	;Get the entry type
	CAILE T2,.ETDMA		;Check entry type
PSIBUG:	BUG.(CHK,SCSBDE,SCSJSY,SOFT,<SCSJSY - Bad entry type found>,<<T2,TYPE>,<T1,BLKADR>>,<

Cause:	An illegal type of message buffer was attempted to be returned.
	It is now lost.

Action:	If this BUGCHK is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.


***** CHANGE #44; PAGE 86, LINE 45; PAGE 102, LINE 45
	MOVEM T4,@.BOTCQ(T2)	;No, update the tail pointer in the CB
	SKIPE T4		;Is there a previous entry???
	MOVEM T3,.MEANC(T4)	;Yes, update next pointer of previous
	SKIPN T4		;Is there a previous entry???
	MOVEM T3,@.TOPCQ(T2)	;No, update the head pointer in the CB

; Here to remove the entry from the fork list
;
DEQFRK:	SKIPL P1		;Is this fork only case???
	IFNSK.
	 MOVX T3,CBFKIL		;No, see if fork with connect is dead
 ---------------------------------
	MOVEM T4,@.BOTCQ(T2)	;No, update the tail pointer in the CB
	SKIPE T4		;Is there a previous entry???
	MOVEM T3,.MEANC(T4)	;Yes, update next pointer of previous
	SKIPN T4		;Is there a previous entry???
	MOVEM T3,@.TOPCQ(T2)	;No, update the head pointer in the CB

; Here to remove the entry from the fork list
;
DEQFRK:	IFGE. P1		;[8936] Is this fork only case?
	 MOVX T3,CBFKIL		;No, see if fork with connect is dead

***** CHANGE #45; PAGE 87, LINE 26; PAGE 104, LINE 26
;
; Note:
;	It is the responsibility of the caller to provide an interlock while
;we own the PSB.
;
SCSMPS:	SKIPL T2,MPSFRK		;Have we got a fork mapped already???
	IFNSK.
	 CAME T1,T2		;Yes, are we mapping the same fork again???
 ---------------------------------
;
; Note:
;	It is the responsibility of the caller to provide an interlock while
;we own the PSB.
;
SCSMPS:	SKIPGE T2,MPSFRK	;[8936] Have we got a fork mapped already???
	IFSKP.			;[8936]
	 CAME T1,T2		;Yes, are we mapping the same fork again???

***** CHANGE #46; PAGE 87, LINE 39; PAGE 104, LINE 39
	 RETSKP			;  and return as if we did the work again
	ENDIF.

	MOVEM T1,MPSFRK		;Save the fork number of last fork mapped

	CAMN T1,FORKX		;Are we the target fork???
	IFNSK.
	 AOS MPSCNT		;Increment the nesting level
 ---------------------------------
	 RETSKP			;  and return as if we did the work again
	ENDIF.

	MOVEM T1,MPSFRK		;Save the fork number of last fork mapped

	CAME T1,FORKX		;[8936] Are we the target fork???
	IFSKP.			;[8936]
	 AOS MPSCNT		;Increment the nesting level

***** CHANGE #47; PAGE 87, LINE 48; PAGE 104, LINE 48
	 AOS MPSFKX		;Count a mapped ourselves occurance
	 XMOVEI T2,PSBPGA	;Point to the base of our PSB
	 MOVEM T2,MPSADR	;Save the last reported address
	 RETSKP			;  and return
	ENDIF.

 ---------------------------------
	 AOS MPSFKX		;Count a mapped ourselves occurance
	 XMOVEI T2,PSBPGA	;Point to the base of our PSB
	 MOVEM T2,MPSADR	;Save the last reported address
	 RETSKP			;  and return
	ENDIF.


***** CHANGE #48; PAGE 91, LINE 26; PAGE 109, LINE 26
	IFSKP.			;[7.1043]If yes, then
	  CALLRET <XENT SC.RLD> ;[7.1043] Return this datagram buffer
	ELSE.			;[7.1043]Else, 
	  CALLRET <XENT SC.RBF> ;[7.1043] Return the message buffer please
	ENDIF.			;[7.1043]

 ---------------------------------
	IFSKP.			;[7.1043]If yes, then
	  CALLRET <XENT SC.RLD> ;[7.1043] Return this datagram buffer
	ELSE.			;[7.1043]Else, 
	  CALLRET <XENT SC.RBF> ;[7.1043] Return the message buffer please
	ENDIF.			;[7.1043]


***** CHANGE #49; PAGE 91, LINE 45; PAGE 110, LINE 19

	CAMN T1,FORKX		;FOR THIS FORK?
	IFSKP.			;NO.
	  MOVE T2,FORKX
	  MOVE T3,(P)		;CALLING ADDRESS
	  BUG. (CHK,SCSFR1,SCSJSY,SOFT,<SCSJSY - SCS% fork removing entries that do not belong to it>,<<T1,FRKNUM>,<T2,CURFRK>,<T3,ADDRESS>>,<

Cause:	It is assumed that only the owning fork can manipulate SCS% in
	a CB or in its own PSB.

 ---------------------------------

	CAMN T1,FORKX		;FOR THIS FORK?
	IFSKP.			;NO.
	  MOVE T2,FORKX
	  MOVE T3,(P)		;CALLING ADDRESS
	  BUG.(CHK,SCSFR1,SCSJSY,SOFT,<SCSJSY - SCS% fork removing entries that do not belong to it>,<<T1,FRKNUM>,<T2,CURFRK>,<T3,ADDRESS>>,<

Cause:	SCS% fork is remving entries that don't belong to it.  It is assumed
	that only the owning fork can manipulate SCS% in a CB or in its own
	PSB.

Action:	If this BUGCHK is reproducible, set it dumpable and send in an SPR
	along with how to reproduce the problem.


