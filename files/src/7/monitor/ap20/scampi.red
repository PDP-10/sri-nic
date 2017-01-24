REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:19:55
File 1: SRC:<7.MONITOR>SCAMPI.MAC.1
File 2: SRC:<7.MONITOR.AP20>SCAMPI.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8914 to SCAMPI.MAC on 17-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 3, LINE 3; PAGE 3, LINE 3
	EXTERN MAPBUF,UMAP,SNDDAT,REQDAT,OPENVC
	EXTERN LOCPRT
	Subttl	Table of Contents

;		     Table of Contents for SCAMPI
;
 ---------------------------------
	EXTERN MAPBUF,UMAP,SNDDAT,REQDAT,OPENVC
	EXTERN LOCPRT
	Subttl	Table of Contents

;		     Table of Contents for AM70-SCAMPI
;

***** CHANGE #3; PAGE 3, LINE 16; PAGE 3, LINE 16
;        1.2    Ring Buffer Usage  . . . . . . . . . . . . . .   8
;    2. Local variable storage for SCS . . . . . . . . . . . .   9
;    3. State Tables . . . . . . . . . . . . . . . . . . . . .  10
;    4. Initialization code  . . . . . . . . . . . . . . . . .  19
;    5. Calls from sysap
;        5.1    SC.SOA (Set ONLINE address)  . . . . . . . . .  20
;        5.2    Connection management  . . . . . . . . . . . .  21
;            5.2.1    SC.CON (Connect) . . . . . . . . . . . .  23
;            5.2.2    SC.LIS (Listen)  . . . . . . . . . . . .  26
;            5.2.3    SC.ACC (Accept)  . . . . . . . . . . . .  29
;            5.2.4    SC.REJ (Reject)  . . . . . . . . . . . .  31
;            5.2.5    SC.DIS (Disconnect)  . . . . . . . . . .  32
;        5.3    Messages and datagrams
;            5.3.1    SC.SDG (Send a datagram) . . . . . . . .  35
;            5.3.2    SC.SMG (Send a message)  . . . . . . . .  38
;            5.3.3    SC.RDG (Receive a datagram)  . . . . . .  42
;            5.3.4    SC.RMG (Receive a message) . . . . . . .  43
;            5.3.5    SC.CRD (Cancel Receive datagram) . . . .  45
;            5.3.6    SC.CMS (Cancel receive message)  . . . .  46
;        5.4    Named buffers
;            5.4.1    SC.MAP (Map a buffer)  . . . . . . . . .  47
;            5.4.2    SC.UMP (Unmap a mapped buffer) . . . . .  48
;            5.4.3    SC.SND (Send DMA data) . . . . . . . . .  49
;        5.5    State polling  . . . . . . . . . . . . . . . .  51
;            5.5.1    SC.DCI (Return destination connect ID) .  52
;            5.5.2    SC.RCD (Return configuration data) . . .  53
;            5.5.3    SC.NOD (Return node number given CID)  .  54
;            5.5.4    SC.PRT (Return local port number)  . . .  55
;        5.6    Maintenance
;            5.6.1    SC.RST (Reset remote system) . . . . . .  56
;            5.6.2    SC.STA (Start remote node) . . . . . . .  57
	Subttl	Table of Contents (page 2)

;		     Table of Contents for SCAMPI
;
 ---------------------------------
;        1.2    Ring Buffer Usage  . . . . . . . . . . . . . .   8
;    2. Local variable storage for SCS . . . . . . . . . . . .   9
;    3. State Tables . . . . . . . . . . . . . . . . . . . . .  10
;    4. Initialization code  . . . . . . . . . . . . . . . . .  19
;    5. Calls from sysap
;        5.1    SC.SOA (Set ONLINE address)  . . . . . . . . .  22
;        5.2    Connection management  . . . . . . . . . . . .  24
;            5.2.1    SC.CON (Connect) . . . . . . . . . . . .  26
;            5.2.2    SC.LIS (Listen)  . . . . . . . . . . . .  30
;            5.2.3    SC.ACC (Accept)  . . . . . . . . . . . .  33
;            5.2.4    SC.REJ (Reject)  . . . . . . . . . . . .  35
;            5.2.5    SC.DIS (Disconnect)  . . . . . . . . . .  36
;        5.3    Messages and datagrams
;            5.3.1    SC.SDG (Send a datagram) . . . . . . . .  40
;            5.3.2    SC.SMG (Send a message)  . . . . . . . .  43
;            5.3.3    SC.RDG (Receive a datagram)  . . . . . .  47
;            5.3.4    SC.RMG (Receive a message) . . . . . . .  48
;            5.3.5    SC.CRD (Cancel Receive datagram) . . . .  50
;            5.3.6    SC.CMS (Cancel receive message)  . . . .  52
;        5.4    Named buffers
;            5.4.1    SC.MAP (Map a buffer)  . . . . . . . . .  53
;            5.4.2    SC.UMP (Unmap a mapped buffer) . . . . .  54
;            5.4.3    SC.SND (Send DMA data) . . . . . . . . .  55
;        5.5    State polling  . . . . . . . . . . . . . . . .  57
;            5.5.1    SC.DCI (Return destination connect ID) .  58
;            5.5.2    SC.RCD (Return configuration data) . . .  59
;            5.5.3    SC.NOD (Return node number given CID)  .  60
;            5.5.4    SC.PRT (Return local port number)  . . .  61
;        5.6    Maintenance
;            5.6.1    SC.RST (Reset remote system) . . . . . .  62
;            5.6.2    SC.STA (Start remote node) . . . . . . .  63
	Subttl	Table of Contents (page 2)

;		     Table of Contents for AM70-SCAMPI
;

***** CHANGE #4; PAGE 4, LINE 9; PAGE 4, LINE 9
;				  Section		      Page
;
;
;    6. Call from port driver
;        6.1    SC.ONL (Node has come online)  . . . . . . . .  58
;        6.2    SC.ERR (Node(s) have gone away)  . . . . . . .  62
;        6.3    SC.DMA (DMA operation complete)  . . . . . . .  66
;        6.4    Incoming buffer
;            6.4.1    SC.INT (Buffer arrival)  . . . . . . . .  67
;            6.4.2    SC.ORQ (Connect request) . . . . . . . .  70
;            6.4.3    SC.ORS (Connect response)  . . . . . . .  72
;            6.4.4    SC.ARQ (Accept request)  . . . . . . . .  73
;            6.4.5    SC.ARS (Accept response) . . . . . . . .  75
;            6.4.6    SC.RRQ (Reject request)  . . . . . . . .  76
;            6.4.7    SC.RRS (Reject response) . . . . . . . .  77
;            6.4.8    SC.DRQ (Disconnect request)  . . . . . .  78
;            6.4.9    SC.DRS (Disconnect response) . . . . . .  81
;            6.4.10   SC.CRQ (Credit request)  . . . . . . . .  82
;            6.4.11   SC.CRS (Credit response) . . . . . . . .  84
;            6.4.12   SC.INC (Connection management) . . . . .  86
;            6.4.13   SC.ADG/SC.AMG (application datagram/mess  90
;    7. Periodic functions
;        7.1    SC.CLK (Dispatch)  . . . . . . . . . . . . . .  93
;        7.2    SC.IDL (Idle chatter)  . . . . . . . . . . . .  94
;        7.3    SC.RAP (Reap old connections)  . . . . . . . .  96
;        7.4    SC.MAL (Allocate buffers)  . . . . . . . . . . 102
;        7.5    SC.ALC (Allocate buffers for pool) . . . . . . 103
;        7.6    SC.DEF (Allocate deferred buffers for sysap) . 105
;    8. Connection management utility routines
;        8.1    SC.SCA (Send connection management message)  . 108
;        8.2    SC.SNM (Send next connection management messag 109
;        8.3    SC.AWQ (Add entry to work queue) . . . . . . . 112
;        8.4    SC.RWQ (Remove entry from work queue)  . . . . 113
;        8.5    SC.AIB (Allocate and initialize a connection b 114
;        8.6    SC.LCB (Link a new CB) . . . . . . . . . . . . 117
;        8.7    SC.SCM (Search for connection match) . . . . . 120
;        8.8    SC.CSC (CID sanity check)  . . . . . . . . . . 124
;        8.9    SC.CDT (Send credit_request) . . . . . . . . . 125
;        8.10   SC.CVC (Close a virtual connection)  . . . . . 128
;        8.11   SC.SDM (String and data move to CB)  . . . . . 129
;        8.12   SC.OUT (Check sysap call)  . . . . . . . . . . 130
;        8.13   SC.RSP (Send a response) . . . . . . . . . . . 131
;        8.14   SC.RQS (Send a request)  . . . . . . . . . . . 132
;        8.15   SC.PAK (Send a packet) . . . . . . . . . . . . 135
	Subttl	Table of Contents (page 3)

;		     Table of Contents for SCAMPI
;
 ---------------------------------
;				  Section		      Page
;
;
;    6. Call from port driver
;        6.1    SC.ONL (Node has come online)  . . . . . . . .  64
;        6.2    SC.ERR (Node(s) have gone away)  . . . . . . .  69
;        6.3    SC.DMA (DMA operation complete)  . . . . . . .  77
;        6.4    Incoming buffer
;            6.4.1    SC.INT (Buffer arrival)  . . . . . . . .  78
;            6.4.2    SC.ORQ (Connect request) . . . . . . . .  82
;            6.4.3    SC.ORS (Connect response)  . . . . . . .  84
;            6.4.4    SC.ARQ (Accept request)  . . . . . . . .  85
;            6.4.5    SC.ARS (Accept response) . . . . . . . .  87
;            6.4.6    SC.RRQ (Reject request)  . . . . . . . .  88
;            6.4.7    SC.RRS (Reject response) . . . . . . . .  89
;            6.4.8    SC.DRQ (Disconnect request)  . . . . . .  90
;            6.4.9    SC.DRS (Disconnect response) . . . . . .  94
;            6.4.10   SC.CRQ (Credit request)  . . . . . . . .  95
;            6.4.11   SC.CRS (Credit response) . . . . . . . .  97
;            6.4.12   SC.INC (Connection management) . . . . . 100
;            6.4.13   SC.ADG/SC.AMG (application datagram/mess 105
;    7. Periodic functions
;        7.1    SC.CLK (Dispatch)  . . . . . . . . . . . . . . 108
;        7.2    SC.IDL (Idle chatter)  . . . . . . . . . . . . 109
;        7.3    SC.RAP (Reap old connections)  . . . . . . . . 112
;        7.4    SC.MAL (Allocate buffers)  . . . . . . . . . . 124
;        7.5    SC.ALC (Allocate buffers for pool) . . . . . . 125
;        7.6    SC.DEF (Allocate deferred buffers for sysap) . 127
;    8. Connection management utility routines
;        8.1    SC.SCA (Send connection management message)  . 131
;        8.2    SC.SNM (Send next connection management messag 132
;        8.3    SC.AWQ (Add entry to work queue) . . . . . . . 137
;        8.4    SC.RWQ (Remove entry from work queue)  . . . . 138
;        8.5    SC.AIB (Allocate and initialize a connection b 139
;        8.6    SC.LCB (Link a new CB) . . . . . . . . . . . . 142
;        8.7    SC.SCM (Search for connection match) . . . . . 146
;        8.8    SC.CSC (CID sanity check)  . . . . . . . . . . 150
;        8.9    SC.CDT (Send credit_request) . . . . . . . . . 151
;        8.10   SC.CVC (Close a virtual connection)  . . . . . 155
;        8.11   SC.SDM (String and data move to CB)  . . . . . 156
;        8.12   SC.OUT (Check sysap call)  . . . . . . . . . . 157
;        8.13   SC.RSP (Send a response) . . . . . . . . . . . 158
;        8.14   SC.RQS (Send a request)  . . . . . . . . . . . 159
;        8.15   SC.PAK (Send a packet) . . . . . . . . . . . . 162
	Subttl	Table of Contents (page 3)

;		     Table of Contents for AM70-SCAMPI
;

***** CHANGE #5; PAGE 5, LINE 9; PAGE 5, LINE 9
;				  Section		      Page
;
;
;    9. Buffer management routines
;        9.1    SC.SBT (Set buffer thresholds) . . . . . . . . 137
;        9.2    SC.BUF . . . . . . . . . . . . . . . . . . . . 142
;        9.3    SC.ALD (Allocate long datagram buffers)  . . . 144
;        9.4    SC.ABF (Allocate a buffer/buffers) . . . . . . 145
;        9.5    SC.CMG/SC.CDG (Create MSG/DG buffers)  . . . . 146
;        9.6    SC.BBF (Break memory into buffers) . . . . . . 147
;        9.7    SC.RBF (Return a message buffer) . . . . . . . 148
;        9.8    SC.RLD (Return a datagram buffer)  . . . . . . 149
;        9.9    SC.TMQ/SC.TDQ (Trace free queue) . . . . . . . 150
;   10. Byte-swapping routines
;       10.1    SC.RIN (Byte swap incoming packet) . . . . . . 151
;       10.2    SC.ISW (Swap bytes from 11 to 10 format) . . . 152
;       10.3    SC.ROU (Byte swap outgoing packet) . . . . . . 153
;       10.4    SC.OSW (Swap word from 10 to 11 format)  . . . 154
;   11. Locking/unlocking routines
;       11.1    SC.PFF (Turn KLIPA channel off)  . . . . . . . 155
;       11.2    SC.PON (Turn KLIPA channel on) . . . . . . . . 156
;       11.3    SC.LOK (Lock connection block) . . . . . . . . 157
;       11.4    SC.ULK (Unlock connection block) . . . . . . . 158
;   12. Ring Buffer routines
;       12.1    SC.RGI (Initialize ring buffer variables)  . . 159
;       12.2    SC.RHD (Write ring buffer entry header)  . . . 160
;       12.3    SC.RTE (Write ring buffer entry tail)  . . . . 161
;       12.4    SC.RWR (Write ring buffer entry) . . . . . . . 162
;       12.5    RG.SSC (SYSAP to SCA calls)  . . . . . . . . . 163
;       12.6    RG.SCS (SCA to SYSAP calls)  . . . . . . . . . 164
;       12.7    RG.BFM (Buffer manipulation) . . . . . . . . . 165
;       12.8    RG.PKT (Packet movement) . . . . . . . . . . . 166
;       12.9    RG.PIT (PI transitions)  . . . . . . . . . . . 167
;       12.10   RG.PQM (Port queue manipulation) . . . . . . . 168
;       12.11   RG.ITL (Interlocks)  . . . . . . . . . . . . . 169
;   13. General Routines
;       13.1    SC.BRK (Break data into SCA buffers) . . . . . 171
;       13.2    MXCIND (Return max CI nodes seen)  . . . . . . 172
;   14. Dummy routines until the port driver supports them . . 173
;   15. End of SCAMPI  . . . . . . . . . . . . . . . . . . . . 174
 ---------------------------------
;				  Section		      Page
;
;
;    9. Buffer management routines
;        9.1    SC.SBT (Set buffer thresholds) . . . . . . . . 164
;        9.2    SC.BUF . . . . . . . . . . . . . . . . . . . . 169
;        9.3    SC.ALD (Allocate long datagram buffers)  . . . 171
;        9.4    SC.ABF (Allocate a buffer/buffers) . . . . . . 172
;        9.5    SC.CMG/SC.CDG (Create MSG/DG buffers)  . . . . 177
;        9.6    SC.BBF (Break memory into buffers) . . . . . . 180
;        9.7    SC.RBF (Return a message buffer) . . . . . . . 181
;        9.8    SC.RLD (Return a datagram buffer)  . . . . . . 182
;        9.9    SC.TMQ/SC.TDQ (Trace free queue) . . . . . . . 183
;   10. Byte-swapping routines
;       10.1    SC.RIN (Byte swap incoming packet) . . . . . . 184
;       10.2    SC.ISW (Swap bytes from 11 to 10 format) . . . 185
;       10.3    SC.ROU (Byte swap outgoing packet) . . . . . . 186
;       10.4    SC.OSW (Swap word from 10 to 11 format)  . . . 187
;   11. Locking/unlocking routines
;       11.1    SC.PFF (Turn KLIPA channel off)  . . . . . . . 188
;       11.2    SC.PON (Turn KLIPA channel on) . . . . . . . . 189
;       11.3    SC.LOK (Lock connection block) . . . . . . . . 191
;       11.4    SC.ULK (Unlock connection block) . . . . . . . 192
;   12. Ring Buffer routines
;       12.1    SC.RGI (Initialize ring buffer variables)  . . 195
;       12.2    SC.RHD (Write ring buffer entry header)  . . . 196
;       12.3    SC.RTE (Write ring buffer entry tail)  . . . . 199
;       12.4    SC.RWR (Write ring buffer entry) . . . . . . . 200
;       12.5    RG.SSC (SYSAP to SCA calls)  . . . . . . . . . 201
;       12.6    RG.SCS (SCA to SYSAP calls)  . . . . . . . . . 202
;       12.7    RG.BFM (Buffer manipulation) . . . . . . . . . 203
;       12.8    RG.PKT (Packet movement) . . . . . . . . . . . 204
;       12.9    RG.PIT (PI transitions)  . . . . . . . . . . . 205
;       12.10   RG.PQM (Port queue manipulation) . . . . . . . 206
;       12.11   RG.ITL (Interlocks)  . . . . . . . . . . . . . 207
;   13. General Routines
;       13.1    SC.BRK (Break data into SCA buffers) . . . . . 209
;       13.2    MXCIND (Return max CI nodes seen)  . . . . . . 212
;   14. Dummy routines until the port driver supports them . . 213
;   15. End of SCAMPI  . . . . . . . . . . . . . . . . . . . . 214

***** CHANGE #6; PAGE 20, LINE 43; PAGE 20, LINE 43
	MOVEM T1,BOTDFQ		;Init the BLINK as pointer to FLINK

	SETZM TOPDC		;Zero the dont care queue FLINK
	XMOVEI T1,TOPDC		;Get the address of top of dont care queue
	MOVEM T1,BOTDC		;Save as the bottom of the queue

	MOVX T1,1		;We desire only one page
	CALLX (MSEC1,PGRSKD)	;[7.1037]Get a page of resident memory for us pls..
	 BUG. (HLT,SCANPT,SCAMPI,SOFT,<SCA - No page for CID table>,,<

Cause:	SCA called PGRSKD for a page to put its data tables in.
	The call failed. Nothing can be done without these tables.
>)
 ---------------------------------
	MOVEM T1,BOTDFQ		;Init the BLINK as pointer to FLINK

	SETZM TOPDC		;Zero the dont care queue FLINK
	XMOVEI T1,TOPDC		;Get the address of top of dont care queue
	MOVEM T1,BOTDC		;Save as the bottom of the queue

	MOVX T1,1		;We desire only one page
	CALLX (MSEC1,PGRSKD)	;[7.1037]Get a page of resident memory for us pls..
	 BUG.(HLT,SCANPT,SCAMPI,SOFT,<SCA - No page for CID table>,,<

Cause:	SCA called PGRSKD for a page to put its data tables in.  The call
	failed.  Nothing can be done without these tables.
>)

***** CHANGE #7; PAGE 20, LINE 97; PAGE 21, LINE 54
	MOVEM T1,TOPFQ		;Set up the pointer to the top of the free q
	MOVEM T2,BOTFQ		;And the pointer to the bottom of the free q
   IFN SCADBG,<
	CALL SC.TMQ		;Trace the message free queue
>				;End of IFN SCADBG

 ---------------------------------
	MOVEM T1,TOPFQ		;Set up the pointer to the top of the free q
	MOVEM T2,BOTFQ		;And the pointer to the bottom of the free q
   IFN SCADBG,<
	CALL SC.TMQ		;Trace the message free queue
>				;End of IFN SCADBG


***** CHANGE #8; PAGE 20, LINE 117; PAGE 22, LINE 20

; Allocate the notification table
;
SSC.NN:	MOVEI T1,1		;We only wish one buffer
	CALL <XENT SC.ABF>	;[7.1037]Get a single buffer
	 BUG. (HLT,SCANBL,SCAMPI,SOFT,<SCA - No buffer for online list>,,<

Cause:	SC.ABF was called to get a buffer for the address list to be used
	to call SYSAPs when a node comes online. Without this list no one will
	ever be told when a node comes online and hence we cannot run.
 ---------------------------------

; Allocate the notification table
;
SSC.NN:	MOVEI T1,1		;We only wish one buffer
	CALL <XENT SC.ABF>	;[7.1037]Get a single buffer
	 BUG.(HLT,SCANBL,SCAMPI,SOFT,<SCA - No buffer for online list>,,<

Cause:	SC.ABF was called to get a buffer for the address list to be used to
	call SYSAPs when a node comes online.  Without this list no one will
	ever be told when a node comes online and hence we cannot run.

***** CHANGE #9; PAGE 20, LINE 139; PAGE 22, LINE 42
	SETOM SCAINI		;Indicate completion of init code
	RET			;And return...

; Here when we find an unrecoverable error in SCA init.
;
SCADIE:	BUG. (HLT,SCACCI,SCAMPI,SOFT,<SCA - Cannot complete initialization>,,<

Cause:	During the init SCA detected an error it could not recover from. 
	SCADIE is called by the location and the stack points out the 
	faultly phase of init.
>)
	JRST SCADIE		;Just in case we try something silly, like
				;  continue...
 ---------------------------------
	SETOM SCAINI		;Indicate completion of init code
	RET			;And return...

; Here when we find an unrecoverable error in SCA init.
;
SCADIE:	BUG.(HLT,SCACCI,SCAMPI,SOFT,<SCA - Cannot complete initialization>,,<

Cause:	During the init SCA detected an error it could not recover from.
	SCADIE is called by the location and the stack points out the faultly
	phase of init.
>)

***** CHANGE #10; PAGE 22, LINE 29; PAGE 24, LINE 29
	MOVE T3,SS.ADR		;Get the user provided address
SSO.EL:	SKIPN (T1)		;Is there an entry here?
	JRST SSO.CO		;No.Go store this address
	AOS T1			;Yes. Try for the next entry.
	SOJGE T2,SSO.EL
	BUG. (CHK,SCANLF,SCAMPI,SOFT,<SCA - Notice table full>,,<

Cause:	So many SYSAPs have requested notification of nodes that come on
	and go off line that the table of notification addresses overflowed.
>)
	RETBAD (SCSNRT)		;Return error code

 ---------------------------------
	MOVE T3,SS.ADR		;Get the user provided address
SSO.EL:	SKIPN (T1)		;Is there an entry here?
	JRST SSO.CO		;No.Go store this address
	AOS T1			;Yes. Try for the next entry.
	SOJGE T2,SSO.EL
	BUG.(CHK,SCANLF,SCAMPI,SOFT,<SCA - Notice table full>,,<

Cause:	So many SYSAPs have requested notification of nodes that come on and go
	off line that the table of notification addresses overflowed.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.
>)
	RETBAD (SCSNRT)		;Return error code


***** CHANGE #11; PAGE 25, LINE 27; PAGE 28, LINE 27
;	T1/	Error code
;
;	Return (+2)
;	T1/	Connect ID
;

 ---------------------------------
;	T1/	Error code
;
;	Return (+2)
;	T1/	Connect ID
;


***** CHANGE #12; PAGE 25, LINE 50; PAGE 29, LINE 23
;See if the current state is correct (It should be in this case!), and
;get new connection and block states.

	MOVEI T1,EV.CON		;Event is connect call
	CALL SC.OUT		;(T1,P1/T1,T2)
	 BUG. (HLT,SCACFO,SCAMPI,SOFT,<SCA - SC.CON received failure from SC.OUT>,,<

Cause:	SC.CON created a new connection block and then called SC.OUT to
	check its state. The call should never fail.
>)
 ---------------------------------
;See if the current state is correct (It should be in this case!), and
;get new connection and block states.

	MOVEI T1,EV.CON		;Event is connect call
	CALL SC.OUT		;(T1,P1/T1,T2)
	 BUG.(HLT,SCACFO,SCAMPI,SOFT,<SCA - SC.CON received failure from SC.OUT>,,<

Cause:	SC.CON created a new connection block and then called SC.OUT to check
	its state. The call should never fail.
>)

***** CHANGE #13; PAGE 29, LINE 9; PAGE 33, LINE 9
	;..
	MOVEI T1,EV.LIS		;Event is listen call
	CALL SC.OUT		;(T1,P1/T1,T2)
	 BUG. (HLT,SCALFO,SCAMPI,SOFT,<SCA - SC.LIS received failure from SC.OUT>,,<

Cause:	SC.LIS created a new connection block and then called SC.OUT to
	check its state. The call should never fail.
>)
 ---------------------------------
	;..
	MOVEI T1,EV.LIS		;Event is listen call
	CALL SC.OUT		;(T1,P1/T1,T2)
	 BUG. (HLT,SCALFO,SCAMPI,SOFT,<SCA - SC.LIS received failure from SC.OUT>,,<

Cause:	SC.LIS created a new connection block and then called SC.OUT to check
	its state.  The call should never fail.
>)

***** CHANGE #14; PAGE 36, LINE 38; PAGE 40, LINE 38
;Connection state may have changed since SC.DIS was first called. Make
;sure a disconnect is still legal, and get new states.

	MOVEI T1,EV.DIS		;Event is disconnect call
	CALL SC.OUT		;(T1,P1/T1,T2) Get new states
	 BUG. (HLT,SCAFN2,SCAMPI,SOFT,<SCA - Can't complete deferred call to SC.DIS>,,<

Cause:	A sysap called SCAMPI at SC.DIS when the connection block was locked.
	The connection block is being unlocked, and the	request is being 
	processed. SC.OUT has returned failure, indicating that this
	function can't be performed for the current state. There is no way
	to return that failure to the sysap, which believes that the
	disconnect has proceeded normally. The system will crash to determine 
	the cause.

Action:	It may be impossible to analyze this from the current data.
	If the ring buffer was enabled, try to determine the sequence of
	events. Look at the current state of the connection block, and try
	to see why SC.OUT failed.

>)

 ---------------------------------
;Connection state may have changed since SC.DIS was first called. Make
;sure a disconnect is still legal, and get new states.

	MOVEI T1,EV.DIS		;Event is disconnect call
	CALL SC.OUT		;(T1,P1/T1,T2) Get new states
	 BUG.(HLT,SCAFN2,SCAMPI,SOFT,<SCA - Can't complete deferred call to SC.DIS>,,<

Cause:	A SYSAP called SCAMPI at SC.DIS when the connection block was locked.
	The connection block is being unlocked, and the	request is being
	processed.  SC.OUT has returned failure, indicating that this function
	can't be performed for the current state.  There is no way to return
	that failure to the SYSAP, which believes that the disconnect has
	proceeded normally.  The system will crash to determine the cause.

Action:	Submit an SPR with the dump and instructions for reproducing it.

	It may be impossible to analyze this from the current data.  If the SCA
	ring buffer was enabled, try to determine the sequence of events.  Look
	at the current state of the connection block, and try to see why SC.OUT
	failed.
>)


***** CHANGE #15; PAGE 39, LINE 22; PAGE 44, LINE 22
	TDNN T2,SS.FLG		;Did we bump the pending packet count?
	IFSKP.
   IFN SCADBG,<
	  LOAD T2,CBNPO,(P1)	;Get outstanding packet count
	  SKIPG T2		;Is it greater than zero?
	  BUG. (HLT,SCANP1,SCAMPI,SOFT,<SCA - CBNPO about to go negative>,,<

Cause:	The count is already zero or less when the count of queued buffers 
	for a connection block is to be decremented. 
>)
 ---------------------------------
	TDNN T2,SS.FLG		;Did we bump the pending packet count?
	IFSKP.
   IFN SCADBG,<
	  LOAD T2,CBNPO,(P1)	;Get outstanding packet count
	  SKIPG T2		;Is it greater than zero?
	  BUG.(HLT,SCANP1,SCAMPI,SOFT,<SCA - CBNPO about to go negative>,,<

Cause:	The count is already zero or less when the count of queued buffers for
	a connection block is to be decremented.
>)

***** CHANGE #16; PAGE 43, LINE 20; PAGE 48, LINE 20
	TDNN T2,SS.FLG		;Did we bump the pending packet count
	IFSKP.
   IFN SCADBG,<
	  LOAD T2,CBNPO,(P1)	;Get outstanding packet count
	  SKIPG T2		;Is it greater than zero?
	  BUG. (HLT,SCANP2,SCAMPI,SOFT,<SCA - CBNPO about to go negative>,,<

Cause:	We are about to decrement the count of queued buffers for a
	particular connection block, but the count is already zero or less.
>)
 ---------------------------------
	TDNN T2,SS.FLG		;Did we bump the pending packet count
	IFSKP.
   IFN SCADBG,<
	  LOAD T2,CBNPO,(P1)	;Get outstanding packet count
	  SKIPG T2		;Is it greater than zero?
	  BUG.(HLT,SCANP2,SCAMPI,SOFT,<SCA - CBNPO about to go negative>,,<

Cause:	We are about to decrement the count of queued buffers for a particular
	connection block, but the count is already zero or less.
>)

***** CHANGE #17; PAGE 47, LINE 36; PAGE 52, LINE 36
SCR.BL:	SOSL .CBDGR(P1)		;Reduce number of queued buffers
	IFSKP.
	  AOS .CBDGR(P1)	;Didn't have that many. Restore count
	  RETBAD (SCSNEB,<CALL SC.ULK>) ;Return failure
	ENDIF.
	BLCAL. (<@[MSEC1,,ULNKDG]>,<P4>) ;[7.1037](/T1) Unlink a datagram buffer
 ---------------------------------
SCR.BL:	SOSL .CBDGR(P1)		;Reduce number of queued buffers
	IFSKP.
	  AOS .CBDGR(P1)	;Didn't have that many. Restore count
	  RETBAD (SCSNEB,<CALL SC.ULK>) ;Return failure
	ENDIF.

	BLCAL. (<@[MSEC1,,ULNKDG]>,<P4>) ;[7.1037](/T1) Unlink a datagram buffer

***** CHANGE #18; PAGE 47, LINE 43; PAGE 53, LINE 8
	IFNSK.
	  EXCH T1,P4		;Save error code, get system block address
	  LOAD T1,SBDPA,(T1)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  MOVE T3,SS.NUM	;Number we still need
	  BUG. (CHK,SCACCD,SCAMPI,SOFT,<SCA - Can't cancel datagram buffer>,<<T1,NODE>,<T2,CID>,<T3,COUNT>>,<

Cause:	A sysap has done the "cancel receive datagram" function of SCA, and
	the port's queue did not contain as many buffers as the system believes
	it should contain.

 ---------------------------------
	IFNSK.
	  EXCH T1,P4		;Save error code, get system block address
	  LOAD T1,SBDPA,(T1)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  MOVE T3,SS.NUM	;Number we still need
	  BUG.(CHK,SCACCD,SCAMPI,SOFT,<SCA - Can't cancel datagram buffer>,<<T1,NODE>,<T2,CID>,<T3,COUNT>>,<

Cause:	A SYSAP has done the "cancel receive datagram" function of SCA, and the
	port's queue did not contain as many buffers as the system believes it
	should contain.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #19; PAGE 60, LINE 13; PAGE 66, LINE 13
	SKIPN SCAINI		;Has SCA been initialized???
	IFNSK.
	  BUG.(CHK,SCAOBI,SCAMPI,SOFT,<SCA - Online before initialization done>,,<

Cause:	A node came online before the initialization of SCA was completed.
>)
 ---------------------------------
	SKIPN SCAINI		;Has SCA been initialized???
	IFNSK.
	  BUG.(CHK,SCAOBI,SCAMPI,SOFT,<SCA - Online before initialization done>,,<

Cause:	A node came online before the initialization of SCA was completed.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.
>)

***** CHANGE #20; PAGE 60, LINE 29; PAGE 66, LINE 32
	MOVE T1,BITS(T2)
	TDNN T1,SBSTUK		;Is this system block stuck on buffers?
	IFSKP.
	  BUG.(CHK,SCAONS,SCAMPI,SOFT,<SCA - Online when stuck on buffers>,<<T3,NODE>>,<

Cause:	A node came online while stuck on buffers. This should have been cleared
	when the node went offline.

Action:	Determine whether PHYKLP is calling twice with the same node.
	If not, something has probably clobbered the flags word for this
	system block.

 ---------------------------------
	MOVE T1,BITS(T2)
	TDNN T1,SBSTUK		;Is this system block stuck on buffers?
	IFSKP.
	  BUG.(CHK,SCAONS,SCAMPI,SOFT,<SCA - Online when stuck on buffers>,<<T3,NODE>>,<

Cause:	A node came online while stuck on buffers.  This should have been
	cleared when the node went offline.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.

	Determine whether PHYKLP is calling twice with the same node.  If not,
	something has probably clobbered the flags word for this system block.


***** CHANGE #21; PAGE 60, LINE 41; PAGE 66, LINE 46
Data:	NODE - Node number
>)
	  ANDCAM T1,SBSTUK
	ENDIF.

 ---------------------------------
Data:	NODE - Node number
>)
	  ANDCAM T1,SBSTUK
	ENDIF.


***** CHANGE #22; PAGE 60, LINE 49; PAGE 67, LINE 8

	SKIPN .SBTWQ(P4)	;Does work queue still exist?
	IFSKP.
	  BUG.(CHK,SCAOWJ,SCAMPI,SOFT,<SCA - Online when work queue exists>,<<T3,NODE>>,<

Cause:	A node came online while its work queue still contained entries. 
	The work queue should have been cleaned up when the node went offline.

 ---------------------------------

	SKIPN .SBTWQ(P4)	;Does work queue still exist?
	IFSKP.
	  BUG.(CHK,SCAOWJ,SCAMPI,SOFT,<SCA - Online when work queue exists>,<<T3,NODE>>,<

Cause:	A node came online while its work queue still contained entries.  The
	work queue should have been cleaned up when the node went offline.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #23; PAGE 60, LINE 59; PAGE 67, LINE 21
>)
	  XMOVEI T1,.SBTWQ(P4)
	  MOVEM T1,.SBBWQ(P4)
	  SETZM .SBTWQ(P4)
	ENDIF.
	;..

 ---------------------------------
>)
	  XMOVEI T1,.SBTWQ(P4)
	  MOVEM T1,.SBBWQ(P4)
	  SETZM .SBTWQ(P4)
	ENDIF.


***** CHANGE #24; PAGE 61, LINE 4; PAGE 68, LINE 4
;No connection blocks should be locked now. We get here only after the
;v.c. has been closed, at which point the c.b.'s should have been unlocked.

	;..
	SKIPN T1,.SBCLC(P4)	;Is a connection block still locked?
	IFSKP.
	  BUG.(CHK,SCAOWL,SCAMPI,SOFT,<SCA - Online when connection block still locked>,<<T3,NODE>,<T1,COUNT>>,<

Cause:	A node came online while a connection block was still locked.  The 
	v.c. should not have been closed until this count was zero.

 ---------------------------------
;No connection blocks should be locked now. We get here only after the
;v.c. has been closed, at which point the c.b.'s should have been unlocked.

	SKIPN T1,.SBCLC(P4)	;Is a connection block still locked?
	IFSKP.
	  BUG.(CHK,SCAOWL,SCAMPI,SOFT,<SCA - Online when connection block still locked>,<<T3,NODE>,<T1,COUNT>>,<

Cause:	A node came online while a connection block was still locked.  The
	virtual circuit should not have been closed until this count was zero.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #25; PAGE 62, LINE 11; PAGE 69, LINE 11
	SKIPN T2,.SBOBB(P4)	;Already have a buffer?
	IFSKP.
	  LOAD T3,SS.NOD	;Get node number
	  BUG.(CHK,SCAOBB,SCAMPI,SOFT,<SCA - Buffer already exists>,<<T3,NODE>,<T2,BUFFER>>,<

Cause:	A node that already had an outbound buffer came online.  This field 
	should have been zeroed when the node went offline. SCAMPI may have 
	been called twice for node online. The buffer will be lost after
	this.

 ---------------------------------
	SKIPN T2,.SBOBB(P4)	;Already have a buffer?
	IFSKP.
	  LOAD T3,SS.NOD	;Get node number
	  BUG.(CHK,SCAOBB,SCAMPI,SOFT,<SCA - Buffer already exists>,<<T3,NODE>,<T2,BUFFER>>,<

Cause:	A node that already had an outbound buffer came online.  This field
	should have been zeroed when the node went offline.  SCAMPI may have
	been called twice for node online.  The buffer will be lost after this.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #26; PAGE 63, LINE 24; PAGE 70, LINE 24
;Here when we failed to allocate the two buffers required for connection
;management. In theory, we should be able to tolerate this failure, and
;wait until job 0 creates more buffers. If we start to see this problem,
;it seems reasonable to try to do that. For now, we'll crash.

SON.ME:	BUG. (HLT,SCASCQ,SCAMPI,SOFT,<SCA - Can't get connection management buffers>,<<T1,ERROR>>,<

Cause:	SCA has been notified of a new system coming online. It tried
	to allocate two buffers to be used for connection management, and
	failed. This indicates that a large number of buffers have been
	allocated at interrupt level, and the process that creates more 
	hasn't run recently.

Action:	It is possible to recover from this by deferring buffer allocation 
	to process context. Meanwhile, try to find out why buffers are being 
	used so rapidly, or why job 0 is not running.

Data:	ERROR - error code from allocation routine

>)
 ---------------------------------
;Here when we failed to allocate the two buffers required for connection
;management. In theory, we should be able to tolerate this failure, and
;wait until job 0 creates more buffers. If we start to see this problem,
;it seems reasonable to try to do that. For now, we'll crash.

SON.ME:	BUG.(HLT,SCASCQ,SCAMPI,SOFT,<SCA - Can't get connection management buffers>,<<T1,ERROR>>,<

Cause:	SCA has been notified of a new system coming online.  It tried to
	allocate two buffers to be used for connection management, and failed.
	This indicates that a large number of buffers have been allocated at
	interrupt level, and the process that creates more hasn't run recently.

Action:	It is possible to recover from this by deferring buffer allocation 
	to process context.  Meanwhile, try to find out why buffers are being 
	used so rapidly, or why job 0 is not running.

Data:	ERROR - error code from allocation routine
>)

***** CHANGE #27; PAGE 64, LINE 29; PAGE 71, LINE 29
	MOVE P4,SS.NOD		;Get the SBI of the system that went offline
	MOVE P4,SBLIST(P4)	;Get the address of the system block
	TMNN SBFOFL,(P4)	;Have we seen this as offline already?
	IFSKP.
	  LOAD T1,SBDPA,(P4)	;Yes. get node number
	  BUG. (CHK,SCAOF2,SCAMPI,SOFT,<SCA - Offline twice for a node>,<<T1,NODE>>,<

Cause:	SC.ERR was called when a system block was already flagged as offline.
	While this won't cause an immediate problem, it does indicate 
	internal confusion and should be investigated.

 ---------------------------------
	MOVE P4,SS.NOD		;Get the SBI of the system that went offline
	MOVE P4,SBLIST(P4)	;Get the address of the system block
	TMNN SBFOFL,(P4)	;Have we seen this as offline already?
	IFSKP.
	  LOAD T1,SBDPA,(P4)	;Yes. get node number
	  BUG.(CHK,SCAOF2,SCAMPI,SOFT,<SCA - Offline twice for a node>,<<T1,NODE>>,<

Cause:	SC.ERR was called when a system block was already flagged as offline.
	While this won't cause an immediate problem, it does indicate internal
	confusion and should be investigated.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #28; PAGE 64, LINE 41; PAGE 71, LINE 44
Data:	NODE - Node number
>)
	  CION
	  JRST SOF.NT
	ENDIF.
	SETONE SBFOFL,(P4)	;No. Mark it
	SOSL SBCNT		;Decrement the system count
	IFSKP.
	  BUG. (CHK,SCANSC,SCAMPI,SOFT,<SCA - Negative system count>,,<

Cause:	SCA was notified of a system going offline and decremented the count
	of systems currently online. In doing so, the count went negative. 
>)

	  SETZM SBCNT		;Make the count zero, rather than -1
	 ENDIF.			;  and continue normally
	;..

;Loop over all connections on the SB and mark them as closed. If they were
;already closed, the sysap isn't interested in them any more. If not, tell
;the sysap that the connection has been closed.
;If not CIOFF, need NOSKED to keep reaper from changing the list
;Sysaps expect this callback not to be interrupted

	;..
	SKIPN P1,.SBFCB(P4)	;Get the addr of the first connection block
 ---------------------------------
Data:	NODE - Node number
>)
	  CION
	  JRST SOF.NT
	ENDIF.

	SETONE SBFOFL,(P4)	;No. Mark it
	SOSL SBCNT		;Decrement the system count
	IFSKP.
	  BUG.(CHK,SCANSC,SCAMPI,SOFT,<SCA - Negative system count>,,<

Cause:	SCA was notified of a system going offline and decremented the count of
	systems currently online.  In doing so, the count went negative.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.
>)

	  SETZM SBCNT		;Make the count zero, rather than -1
	 ENDIF.			;  and continue normally


;Loop over all connections on the SB and mark them as closed. If they were
;already closed, the sysap isn't interested in them any more. If not, tell the
;sysap that the connection has been closed.  If not CIOFF, need NOSKED to keep
;reaper from changing the list Sysaps expect this callback not to be
;interrupted.

	SKIPN P1,.SBFCB(P4)	;Get the addr of the first connection block

***** CHANGE #29; PAGE 65, LINE 41; PAGE 73, LINE 41
				; so clear this so we can reap
	CALL SC.PTC		;(P1/) Indicate protocol is complete

SOF.NX:	SKIPE P1,.CBANB(P1)	;Is there a next connect block???
	JRST SOF.CL		;Yes, loop for its new state

 ---------------------------------
				; so clear this so we can reap
	CALL SC.PTC		;(P1/) Indicate protocol is complete

SOF.NX:	SKIPE P1,.CBANB(P1)	;Is there a next connect block???
	JRST SOF.CL		;Yes, loop for its new state


***** CHANGE #30; PAGE 66, LINE 44; PAGE 75, LINE 44
	  CALL RG.PQM		;(T1,T2,T3,T4,P4) Record port manipulation
	ENDBK.
>				;End of IFN SCARNG
	CALL <XENT SC.RBF>	;[7.1037](T1/) Give it to SCA
	JRST SOF.NZ		;Skip past the error code

 ---------------------------------
	  CALL RG.PQM		;(T1,T2,T3,T4,P4) Record port manipulation
	ENDBK.
>				;End of IFN SCARNG
	CALL <XENT SC.RBF>	;[7.1037](T1/) Give it to SCA
	JRST SOF.NZ		;Skip past the error code


***** CHANGE #31; PAGE 66, LINE 50; PAGE 76, LINE 6
;The port ran out of buffers. In theory, we put two buffers on the queue when
;this node came online, so we ought to be able to take two off.

SOF.NG:	LOAD T1,SBDPA,(P4)
	BUG. (CHK,SCANMB,SCAMPI,SOFT,<SCA - Can't return SCS control message buffer>,<<T1,NODE>>,<

Cause:	A node went offline, The local node tried to retrieve two message 
	buffers from the port's queue but found the queue empty.

Action:	Depending on timing this may happen legitimately. If it persists, 
	stock the port's message free queue more generously at system startup.
 ---------------------------------
;The port ran out of buffers. In theory, we put two buffers on the queue when
;this node came online, so we ought to be able to take two off.

SOF.NG:	LOAD T1,SBDPA,(P4)
	BUG.(CHK,SCANMB,SCAMPI,SOFT,<SCA - Can't return SCS control message buffer>,<<T1,NODE>>,<

Cause:	A node went offline, The local node tried to retrieve two message
	buffers from the port's queue but found the queue empty.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.

	Depending on timing this may happen legitimately.  If it persists, 
	stock the port's message free queue more generously at system startup.

***** CHANGE #32; PAGE 66, LINE 71; PAGE 76, LINE 30
	  TXZ T3,RPQFLK   	;Clear port link flag since unlinking
	  TXO T3,RPQFMG   	;Set message free queue flag
	  CALL RG.PQM		;(T1,T2,T3,T4,P4) Record port manipulation
>	         		;End of IFN SCARNG


 ---------------------------------
	  TXZ T3,RPQFLK   	;Clear port link flag since unlinking
	  TXO T3,RPQFMG   	;Set message free queue flag
	  CALL RG.PQM		;(T1,T2,T3,T4,P4) Record port manipulation
>	         		;End of IFN SCARNG



***** CHANGE #33; PAGE 70, LINE 12; PAGE 81, LINE 12
SIN.MS:	LOAD T3,MH$SCI,(P2)	;Get the source connect ID from the packet
	$LDCID P1,T3		;Get the CB address from the CID
   IFN SCADBG,<
	LOAD T1,CBNPO,(P1)	;Get outstanding packet count
	SKIPG T1		;Is it greater than zero?
	  BUG. (HLT,SCANP3,SCAMPI,SOFT,<SCA - CBNPO about to go negative>,,<

Cause:	When the count of queued buffers is about to be decremented, the
	count is found to be already zero or less.
>)
 ---------------------------------
SIN.MS:	LOAD T3,MH$SCI,(P2)	;Get the source connect ID from the packet
	$LDCID P1,T3		;Get the CB address from the CID
   IFN SCADBG,<
	LOAD T1,CBNPO,(P1)	;Get outstanding packet count
	SKIPG T1		;Is it greater than zero?
	  BUG.(HLT,SCANP3,SCAMPI,SOFT,<SCA - CBNPO about to go negative>,,<

Cause:	When the count of queued buffers is about to be decremented, the count
	is found to be already zero or less.
>)

***** CHANGE #34; PAGE 70, LINE 44; PAGE 81, LINE 44
	LOAD T3,MH$MSG,(P2)	;Get the message type
	CAILE T3,.STLST		;Is the packet type within range???
	JRST SIN.ER		;NO!! Handle remote SCA brain damage
	AOS RECTAB(T3)		;Count the message we just received
	JRST @DSPTAB(T3)	;Yes it is, goto the handling routine

; Here when we have a bad message type from the remote SCA. Since this could
;very well have been a message we must close the VC.
;Note that we don't know what kind of buffer we have, so we just throw
;it away.

SIN.ER:	LOAD T1,SBDPA,(P4)	;Get node number
	LOAD T2,CBSCID,(P1)	;Get connect ID
	BUG. (CHK,SCABMT,SCAMPI,SOFT,<SCA - Bad message type from remote node>,<<T1,NODE>,<T2,CID>,<T3,OPCODE>>,<

Cause:	A bad message type was found on range checking. This shouldn't
	happen if the port and port driver are working correctly. The 
	message is thrown away.

 ---------------------------------
	LOAD T3,MH$MSG,(P2)	;Get the message type
	CAILE T3,.STLST		;Is the packet type within range???
	JRST SIN.ER		;NO!! Handle remote SCA brain damage
	AOS RECTAB(T3)		;Count the message we just received
	JRST @DSPTAB(T3)	;Yes it is, goto the handling routine

;Here when we have a bad message type from the remote SCA. Since this could
;very well have been a message we must close the VC.  Note that we don't know
;what kind of buffer we have, so we just throw it away.

SIN.ER:	LOAD T1,SBDPA,(P4)	;Get node number
	LOAD T2,CBSCID,(P1)	;Get connect ID
	BUG.(CHK,SCABMT,SCAMPI,SOFT,<SCA - Bad message type from remote node>,<<T1,NODE>,<T2,CID>,<T3,OPCODE>>,<

Cause:	A bad message type was found on range checking.  This shouldn't happen
	if the port and port driver are working correctly.  The message is
	thrown away.


***** CHANGE #35; PAGE 82, LINE 15; PAGE 94, LINE 15

;CIOFF protects from incoming disconnect_response. Ordinarily, it can't
;interfere with incoming disc_req because both are at interrupt level.
;We must be CIOFF for the callback anyway.

SC.FN3:
	CIOFF
	SETZRO CBFDRQ,(P1)	;Clear indication that this was deferred
 ---------------------------------

;CIOFF protects from incoming disconnect_response. Ordinarily, it can't
;interfere with incoming disc_req because both are at interrupt level.
;We must be CIOFF for the callback anyway.

SC.FN3:	CIOFF
	SETZRO CBFDRQ,(P1)	;Clear indication that this was deferred

***** CHANGE #36; PAGE 82, LINE 24; PAGE 94, LINE 23
	MOVEI T1,.STDRQ		;Op code is disconnect_request
	IMULI T1,MXCNST		;Compute offset in the table based
	OPSTR <ADD T1,>,CBCNST,(P1) ; on op code and current state
	SOS T1			;States start at 1
	TMNE K.ERR,(T1)		;We can't handle protocol violation here
	BUG. (HLT,SCAFN3,SCAMPI,SOFT,<SCA - Can't complete deferred call to SC.DRQ>,,<

Cause:	PHYKLP called SCAMPI at SC.DRQ when the connection block was locked.
	At the time, the incoming packet was legal for the current state of
	the connection. Now it is not legal. This shouldn't happen, and it 
	is uncertain how to proceed. It is possible to close the v.c. and 
	continue, but there is a halt in order to analyze the protocol 
	confusion and fix the bug.
 ---------------------------------
	MOVEI T1,.STDRQ		;Op code is disconnect_request
	IMULI T1,MXCNST		;Compute offset in the table based
	OPSTR <ADD T1,>,CBCNST,(P1) ; on op code and current state
	SOS T1			;States start at 1
	TMNE K.ERR,(T1)		;We can't handle protocol violation here
	BUG.(HLT,SCAFN3,SCAMPI,SOFT,<SCA - Can't complete deferred call to SC.DRQ>,,<

Cause:	PHYKLP called SCAMPI at SC.DRQ when the connection block was locked.
	At the time, the incoming packet was legal for the current state of the
	connection.  Now it is not legal.  This shouldn't happen, and it is
	uncertain how to proceed.  It is possible to close the virtual circuit
	and continue, but there is a halt in order to analyze the protocol
	confusion and fix the bug.

***** CHANGE #37; PAGE 82, LINE 51; PAGE 94, LINE 50

	CAIN T1,.CSCLO		;Is the new state closed?
	CALL SC.PTC		;(P1/) Yes. Indicate protocol is complete
	CION
	RET

;Here when new state is disconnect_received.
;Corporate spec says the sysap should be required to do a disconnect.
;We fake it by setting the state and sending the message.
;Set the state before calling the sysap. If it does an SC.DIS before returning
;we won't send two disconnect_requests.

 ---------------------------------

	CAIN T1,.CSCLO		;Is the new state closed?
	CALL SC.PTC		;(P1/) Yes. Indicate protocol is complete
	CION
	RET

;Here when new state is disconnect_received.  Corporate spec says the sysap
;should be required to do a disconnect.  We fake it by setting the state and
;sending the message.  Set the state before calling the sysap. If it does an
;SC.DIS before returning we won't send two disconnect_requests.


***** CHANGE #38; PAGE 86, LINE 12; PAGE 99, LINE 12
;	P4/	Address of system block
;
;	Return +1:	Always
;

;NOTE:
;If credit field is negative, we can take buffers back from the port. If
;that fails, we have a problem. There will be disagreement among 1) our
;receive credit, 2) the other side's send credit, and 3) the number of
;buffers actually queued for this connection. This code attempts to
;keep 1 and 2 in agreement.

SC.CRS:	AOS SCAORC		;[7211] COUNT THE MESSAGE
	CALL SC.INC		;[7211] (P2/T1,T2,P1) Check validity, get state & op code
	 JRST SC.RIB		;Return the buffer and quit

 ---------------------------------
;	P4/	Address of system block
;
;	Return +1:	Always
;

;NOTE: If credit field is negative, we can take buffers back from the port. If
;that fails, we have a problem. There will be disagreement among 1) our receive
;credit, 2) the other side's send credit, and 3) the number of buffers actually
;queued for this connection. This code attempts to keep 1 and 2 in agreement.

SC.CRS:	AOS SCAORC		;[7211] COUNT THE MESSAGE
	CALL SC.INC		;[7211] (P2/T1,T2,P1) Check validity, get state & op code
	 JRST SC.RIB		;Return the buffer and quit


***** CHANGE #39; PAGE 86, LINE 37; PAGE 100, LINE 15
	ELSE.
	  CAMN P3,.CBRQC(P1)	;Do we agree with the other end?
	  JRST SCC.YY		;Yes. Proceed
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  BUG. (CHK,SCAUCF,SCAMPI,SOFT,<SCA - Unexpected credit field in credit_request>,<<T1,NODE>,<T2,CID>>,<

Cause:	The response to a positive credit_request contains a credit value that
	is different from what is believed to have been sent.

 ---------------------------------
	ELSE.
	  CAMN P3,.CBRQC(P1)	;Do we agree with the other end?
	  JRST SCC.YY		;Yes. Proceed
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  BUG.(CHK,SCAUCF,SCAMPI,SOFT,<SCA - Unexpected credit field in credit_request>,<<T1,NODE>,<T2,CID>>,<

Cause:	The response to a positive credit_request contains a credit value that
	is different from what is believed to have been sent.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #40; PAGE 86, LINE 56; PAGE 100, LINE 37
	SETZM .CBPND(P1)	;Indicate no credit_request pending
	CALL SC.CD1		;(P1,P4/) Queue credit_request if needed
	 NOP			;Don't care whether queued or not
	CALL SC.GCB		;(P1/) Get canceled buffers, if any
	JRST SC.SAR		;Go send the next message



 ---------------------------------
	SETZM .CBPND(P1)	;Indicate no credit_request pending
	CALL SC.CD1		;(P1,P4/) Queue credit_request if needed
	 NOP			;Don't care whether queued or not
	CALL SC.GCB		;(P1/) Get canceled buffers, if any
	JRST SC.SAR		;Go send the next message


***** CHANGE #41; PAGE 89, LINE 17; PAGE 103, LINE 17
	  OPSTR <CAMN T3,>,CBEXPR,(P1); Is this what we expected?
	  JRST SC.IN2		;Yes.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get out connect id
	  LOAD T4,CBEXPR,(P1)	;Get what we expected
	  BUG. (CHK,SCAUXR,SCAMPI,SOFT,<SCA - Unexpected response>,<<T1,NODE>,<T2,CID>,<T3,OPCODE>,<T4,EXPECT>>,<

Cause:	A connection management response arrived for a particular
	connection, but the op code is not the expected one.

Action:	The v.c. will be closed, on the assumption that the other node
	violated protocol. This may correct the confusion. If this error
	persists, try to determine the events that led to it. Use the
	ring buffer if necessary.

 ---------------------------------
	  OPSTR <CAMN T3,>,CBEXPR,(P1); Is this what we expected?
	  JRST SC.IN2		;Yes.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get out connect id
	  LOAD T4,CBEXPR,(P1)	;Get what we expected
	  BUG.(CHK,SCAUXR,SCAMPI,SOFT,<SCA - Unexpected response>,<<T1,NODE>,<T2,CID>,<T3,OPCODE>,<T4,EXPECT>>,<

Cause:	A connection management response arrived for a particular connection,
	but the op code is not the expected one.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.

	The virtual circuit will be closed, on the assumption that the other
	node violated protocol.  This may correct the confusion.  If this error
	persists, try to determine the events that led to it.  Use the SCA ring
	buffer if necessary.


***** CHANGE #42; PAGE 90, LINE 10; PAGE 104, LINE 10
	LOAD T2,CBSCID,(P1)	;get our connect ID
	LOAD T3,MH$MSG,(P2)	;get op code
	LOAD T4,CBCNST,(P1)	;get state of connection
	BUG. (CHK,SCAPER,SCAMPI,SOFT,<SCA - Protocol error>,<<T1,NODE>,<T2,CID>,<T3,OPCODE>,<T4,STATE>>,<

Cause:	An incoming message violated the SCS protocol. This message is 
	illegal. Closing the v.c. will eliminate any confusion.

Action:	If this persists, change it to a BUGHLT, and determine the sequence of
	events that led to it. Use the ring buffer, if necessary.

 ---------------------------------
	LOAD T2,CBSCID,(P1)	;get our connect ID
	LOAD T3,MH$MSG,(P2)	;get op code
	LOAD T4,CBCNST,(P1)	;get state of connection
	BUG. (CHK,SCAPER,SCAMPI,SOFT,<SCA - Protocol error>,<<T1,NODE>,<T2,CID>,<T3,OPCODE>,<T4,STATE>>,<

Cause:	An incoming message violated the SCS protocol.  This message is
	illegal.  The virtual circuit will be closed to eliminate any
	confusion.

Action:	If this persists, change it to a BUGHLT, and submit an SPR along with
	the dump and instruction on reproducing it.

	Look at the dump and determine the sequence of events that led to it.
	Use the SCA ring buffer, if necessary.


***** CHANGE #43; PAGE 90, LINE 23; PAGE 104, LINE 27
	CID    - Connect ID at this node
	OPCODE - Op code of incoming packet
	STATE  - state of connection
>)
	JRST SC.IN7		;Go close the v.c.

 ---------------------------------
	CID    - Connect ID at this node
	OPCODE - Op code of incoming packet
	STATE  - state of connection
>)
	JRST SC.IN7		;Go close the v.c.


***** CHANGE #44; PAGE 90, LINE 32; PAGE 105, LINE 9

SC.IN6:
	LOAD T1,SBDPA,(P4)	;Get node number
	LOAD T2,MH$DCI,(P2)	;Get destination CID
	LOAD T3,MH$MSG,(P2)	;Get op code
	BUG. (CHK,SCANOC,SCAMPI,SOFT,<SCA - Received packet and connection block doesn't exist>,<<T1,NODE>,<T2,CID>,<T3,OPCODE>>,<

Cause:	An incoming packet's destination CID doesn't match any connection block.
	This may reflect disagreement with another node about the state of
	a previously-existing connection.

Action:	The v.c. will be closed, which will correct the problem.
	If this bug persists, try to determine the events that led to it.
	Use the ring buffer if necessary.

 ---------------------------------

SC.IN6:
	LOAD T1,SBDPA,(P4)	;Get node number
	LOAD T2,MH$DCI,(P2)	;Get destination CID
	LOAD T3,MH$MSG,(P2)	;Get op code
	BUG.(CHK,SCANOC,SCAMPI,SOFT,<SCA - Received packet and connection block doesn't exist>,<<T1,NODE>,<T2,CID>,<T3,OPCODE>>,<

Cause:	An incoming packet's destination CID doesn't match any connection
	block.  This may reflect disagreement with another node about the state
	of a previously existing connection.  	The virtual circuit will be
	closed, which will correct the problem.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.

	From looking at the dump, try to determine the events that led to it.
	Use the SCA ring buffer if necessary.


***** CHANGE #45; PAGE 96, LINE 35; PAGE 111, LINE 35
	SKIPN T1,.SBTIM(P4)	;Has this node ever spoken to us???
	RET			;No, ignore it
	SUB T1,TODCLK		;Yes, build long ago that was
	CAMLE T1,TMGTIM		;Has it been too long???
	RET			;No, return

 ---------------------------------
	SKIPN T1,.SBTIM(P4)	;Has this node ever spoken to us???
	RET			;No, ignore it
	SUB T1,TODCLK		;Yes, build long ago that was
	CAMLE T1,TMGTIM		;Has it been too long???
	RET			;No, return


***** CHANGE #46; PAGE 96, LINE 45; PAGE 112, LINE 10

	TMNN SBFTMG,(P4)	;Has a timed message been sent on this SB???
	JRST SCL.LP		;No.
	AOS TMGCNT		;Yes, count another system biting the dust
	LOAD T2,SBDPA,(P4)	;Get remote node number
	BUG. (INF,SCATMO,SCAMPI,SOFT,<SCA - SCA timed out remote node>,<<T2,NODE>,<T1,TIME>>,<

Cause:	SCA sent a message to another node, and did not receive a response
	within a timeout period.

Action:	This happens legitimately if a node crashes. If this timeout
	is occurring for nodes that appear to be running, try to determine
	why they are not communicating.

 ---------------------------------

	TMNN SBFTMG,(P4)	;Has a timed message been sent on this SB???
	JRST SCL.LP		;No.
	AOS TMGCNT		;Yes, count another system biting the dust
	LOAD T2,SBDPA,(P4)	;Get remote node number
	BUG.(INF,SCATMO,SCAMPI,SOFT,<SCA - SCA timed out remote node>,<<T2,NODE>,<T1,TIME>>,<

Cause:	SCA sent a message to another node, and did not receive a response
	within a timeout period.

Action:	This happens legitimately if a node crashes.  If this timeout is
	occurring for nodes that appear to be running, try to determine why
	they are not communicating.  If this bug is reproducible when the node
	is running normally, change it to a BUGHLT and submit an SPR with the
	dump and instructions for reproducing it.


***** CHANGE #47; PAGE 99, LINE 25; PAGE 115, LINE 25

;We mean it. Do the job.

	CALL <XENT SC.DAB>	;[7.1037](P1/) Reap the connect data
	JRST SRA.CL		;  and go on

 ---------------------------------

;We mean it. Do the job.

	CALL <XENT SC.DAB>	;[7.1037](P1/) Reap the connect data
	JRST SRA.CL		;  and go on


***** CHANGE #48; PAGE 99, LINE 44; PAGE 116, LINE 19
	LOAD T1,CBSCID,(P1)	;Get connect ID
	MOVE T2,.CBSBA(P1)	;Get system block address
	LOAD T2,SBDPA,(T2)	;Get node number
	LOAD T3,CBBKST,(P1)	;BLock state
	LOAD T4,CBNPO,(P1)	;Number of queued commands
	BUG. (CHK,SCARTO,SCAMPI,SOFT,<SCA - Reap timed out>,<<T2,NODE>,<T1,CID>,<T3,STATE>,<T4,COUNT>>,<

Cause:	A block that is reapable cannot be reaped because either the count of
	outstanding packets is non-zero or a debugging check has failed. After
	several postponements, these were not corrected. The block is now being
	deleted.

Action:	If COUNT is non-zero, see if the CI-20 was reloaded recently.
	Buffers can be lost legitimately when this happens.

 ---------------------------------
	LOAD T1,CBSCID,(P1)	;Get connect ID
	MOVE T2,.CBSBA(P1)	;Get system block address
	LOAD T2,SBDPA,(T2)	;Get node number
	LOAD T3,CBBKST,(P1)	;BLock state
	LOAD T4,CBNPO,(P1)	;Number of queued commands
	BUG.(CHK,SCARTO,SCAMPI,SOFT,<SCA - Reap timed out>,<<T2,NODE>,<T1,CID>,<T3,STATE>,<T4,COUNT>>,<

Cause:	A block that is reapable cannot be reaped because either the count of
	outstanding packets is non-zero or a debugging check has failed.  After
	several postponements, these were not corrected.  The block is now
	being deleted.

Action:	If COUNT is non-zero, see if the CI-20 was reloaded recently.  Buffers
	can be lost legitimately when this happens.

	If this bug is reproducible with COUNT zero, change it to a BUGHLT and
	submit an SPR with the dump and instructions for reproducing it.


***** CHANGE #49; PAGE 101, LINE 22; PAGE 118, LINE 22
;something is wrong.

	TMNN CBBKST,(P1)	;Not ready if block state isn't zero
	IFSKP.
	  LOAD T3,CBBKST,(P1)	;Get block state
	  BUG. (CHK,SCABSN,SCAMPI,SOFT,<SCA - Block state non-zero when ready to reap>,<<T1,NODE>,<T2,CID>,<T3,STATE>>,<

Cause:	SCA is ready to delete a connection block, and its block state
	is non-zero. The block state should have been cleared when the reap
	bit was set.

 ---------------------------------
;something is wrong.

	TMNN CBBKST,(P1)	;Not ready if block state isn't zero
	IFSKP.
	  LOAD T3,CBBKST,(P1)	;Get block state
	  BUG.(CHK,SCABSN,SCAMPI,SOFT,<SCA - Block state non-zero when ready to reap>,<<T1,NODE>,<T2,CID>,<T3,STATE>>,<

Cause:	SCA is ready to delete a connection block, and its block state is
	non-zero.  The block state should have been cleared when the reap bit
	was set.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #50; PAGE 101, LINE 35; PAGE 118, LINE 38
	CID   - Connect ID
	STATE - block state
>)
	  RET
	ENDIF.

 ---------------------------------
	CID   - Connect ID
	STATE - block state
>)
	  RET
	ENDIF.


***** CHANGE #51; PAGE 101, LINE 43; PAGE 119, LINE 8
;to which we expect a reply.

	SKIPE T3,.CBRQC(P1)	;Non-zero implies credit_request outstanding
	TMNN CBFCVC,(P1)	; but it's bad only if v.c. is open
	IFSKP.
	  BUG. (CHK,SCANRC,SCAMPI,SOFT,<SCA - Non-zero requeue credit at SC.DAB>,<<T1,NODE>,<T2,CID>,<T3,CREDIT>>,<

Cause:	SCA is ready to delete a connection block, and its requeue credit is
	non-zero. This indicates that a credit_response is still expected.

Data:	NODE   - Node number
	CID    - Connect ID
	CREDIT - Requeue credit

>)				;[7.1037]
 ---------------------------------
;to which we expect a reply.

	SKIPE T3,.CBRQC(P1)	;Non-zero implies credit_request outstanding
	TMNN CBFCVC,(P1)	; but it's bad only if v.c. is open
	IFSKP.
	  BUG.(CHK,SCANRC,SCAMPI,SOFT,<SCA - Non-zero requeue credit at SC.DAB>,<<T1,NODE>,<T2,CID>,<T3,CREDIT>>,<

Cause:	SCA is ready to delete a connection block, and its requeue credit is
	non-zero.  This indicates that a credit_response is still expected.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.

Data:	NODE   - Node number
	CID    - Connect ID
	CREDIT - Requeue credit
>)				;[7.1037]

***** CHANGE #52; PAGE 101, LINE 62; PAGE 119, LINE 29
;If the lock count is non-zero, some code thinks it still owns the connection
;block.

	SKIPN T3,.CBLCK(P1)
	IFSKP.
	  BUG. (CHK,SCANLC,SCAMPI,SOFT,<SCA - Non-zero lock count at SC.DAB>,<<T1,NODE>,<T2,CID>,<T3,COUNT>>,<

Cause:	SCA is ready to delete a connection block, and its lock count is
	non-zero.

 ---------------------------------
;If the lock count is non-zero, some code thinks it still owns the connection
;block.

	SKIPN T3,.CBLCK(P1)
	IFSKP.
	  BUG.(CHK,SCANLC,SCAMPI,SOFT,<SCA - Non-zero lock count at SC.DAB>,<<T1,NODE>,<T2,CID>,<T3,COUNT>>,<

Cause:	SCA is ready to delete a connection block, and its lock count is
	non-zero.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #53; PAGE 102, LINE 38; PAGE 120, LINE 38
	MOVE Q1,.CBRCD(P1)	;Get the total current credits
	ADD Q1,.CBPRC(P1)	;add the number of pending credits
	ADD Q1,.CBRQC(P1)	; and number of credits in outstand CDT_REQ
	ADD Q1,.CBRTC(P1)	;Add canceled buffers not yet dequeued
	JUMPE Q1,SRC.DG		;If nothing to dequeue, try datagrams
SRC.B1:	BLCAL. (<@[MSEC1,,ULNKMG]>,<P4>) ;[7.1037](/T1) Get a buffer back from the port
	IFNSK.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  BUG. (CHK,SCACGM,SCAMPI,SOFT,<SCA - Can't get message buffer when reaping>,<<T1,NODE>,<T2,CID>,<Q1,COUNT>>,<

Cause:	While reaping a connection block, a receive credit indicates that
	message buffers are queued to the port. However, the port's queue has
	been emptied while these buffers were removed.

 ---------------------------------
	MOVE Q1,.CBRCD(P1)	;Get the total current credits
	ADD Q1,.CBPRC(P1)	;add the number of pending credits
	ADD Q1,.CBRQC(P1)	; and number of credits in outstand CDT_REQ
	ADD Q1,.CBRTC(P1)	;Add canceled buffers not yet dequeued
	JUMPE Q1,SRC.DG		;If nothing to dequeue, try datagrams

SRC.B1:	BLCAL. (<@[MSEC1,,ULNKMG]>,<P4>) ;[7.1037](/T1) Get a buffer back from the port
	IFNSK.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  BUG.(CHK,SCACGM,SCAMPI,SOFT,<SCA - Can't get message buffer when reaping>,<<T1,NODE>,<T2,CID>,<Q1,COUNT>>,<

Cause:	While reaping a connection block, a receive credit indicates that
	message buffers are queued to the port.  However, the port's queue has
	been emptied while these buffers were removed.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #54; PAGE 102, LINE 79; PAGE 121, LINE 45
	ENDBK.
>				;End of IFN SCARNG
	CALL <XENT SC.RBF>	;[7.1037](T1/) Return the buffer to SCA free pool 
	SOJG Q1,SRC.B1		;  and loop if there are more buffers left
	;..

 ---------------------------------
	ENDBK.
>				;End of IFN SCARNG
	CALL <XENT SC.RBF>	;[7.1037](T1/) Return the buffer to SCA free pool 
	SOJG Q1,SRC.B1		;  and loop if there are more buffers left
	;..


***** CHANGE #55; PAGE 102, LINE 90; PAGE 122, LINE 11
	JUMPE Q1,SRC.EX		;Is there are none queued, exit now
SRC.DL:	BLCAL. (<@[MSEC1,,ULNKDG]>,<P4>) ;[7.1037]Unlink a datagram
	IFNSK.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  BUG. (CHK,SCACGD,SCAMPI,SOFT,<SCA - Can't get datagram buffer when reaping>,<<T1,NODE>,<T2,CID>,<Q1,COUNT>>,<

Cause:	When reaping a connection block, a buffer count indicates that
	datagram buffers are queued to the port. However, the port's queue has
	been emptied while these buffers were removed.

 ---------------------------------
	JUMPE Q1,SRC.EX		;Is there are none queued, exit now
SRC.DL:	BLCAL. (<@[MSEC1,,ULNKDG]>,<P4>) ;[7.1037]Unlink a datagram
	IFNSK.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  BUG.(CHK,SCACGD,SCAMPI,SOFT,<SCA - Can't get datagram buffer when reaping>,<<T1,NODE>,<T2,CID>,<Q1,COUNT>>,<

Cause:	When reaping a connection block, a buffer count indicates that datagram
	buffers are queued to the port.  However, the port's queue has been
	emptied while these buffers were removed.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #56; PAGE 102, LINE 112; PAGE 122, LINE 36
	  TXZ T3,RPQFMG   	;Clear message free queue flag since datagram
	  CALL RG.PQM		;(T1,T2,T3,T4,P4) Record port manipulation
>	         		;End of IFN SCARNG
          JRST SRC.EX		;Can't get any more
	ENDIF.
   IFN SCARNG,<
 ---------------------------------
	  TXZ T3,RPQFMG   	;Clear message free queue flag since datagram
	  CALL RG.PQM		;(T1,T2,T3,T4,P4) Record port manipulation
>	         		;End of IFN SCARNG
          JRST SRC.EX		;Can't get any more
	ENDIF.

   IFN SCARNG,<

***** CHANGE #57; PAGE 102, LINE 131; PAGE 123, LINE 20
	SOJG Q1,SRC.DL		;Loop if there are more buffers to do

SRC.EX:
;	CALL SC.JSY		;(P1/) Clean up data for JSYS connection
	JRST SRC.KL		;Go kill the buffer

 ---------------------------------
	SOJG Q1,SRC.DL		;Loop if there are more buffers to do

SRC.EX:
;	CALL SC.JSY		;(P1/) Clean up data for JSYS connection
	JRST SRC.KL		;Go kill the buffer


***** CHANGE #58; PAGE 105, LINE 11; PAGE 127, LINE 11
	SUB T1,FQCNT		;How many do we need?
	JUMPLE T1,SAL.DG	;We have enough
	CALL SC.CMG		;(T1/T1,T2,T3) Create the message buffers
	IFNSK.
	  MOVE T2,FQCNT
	  BUG. (CHK,SCAMCF,SCAMPI,SOFT,<SCA - Message buffer creation failure>,<<T1,ERROR>,<T2,COUNT>>,<

Cause:	SCA detected that the level of buffers maintained was below minimum.
	The attempt to create more message buffers failed.

 ---------------------------------
	SUB T1,FQCNT		;How many do we need?
	JUMPLE T1,SAL.DG	;We have enough
	CALL SC.CMG		;(T1/T1,T2,T3) Create the message buffers
	IFNSK.
	  MOVE T2,FQCNT
	  BUG.(CHK,SCAMCF,SCAMPI,SOFT,<SCA - Message buffer creation failure>,<<T1,ERROR>,<T2,COUNT>>,<

Cause:	SCA detected that the level of buffers maintained was below minimum.
	The attempt to create more message buffers failed.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #59; PAGE 106, LINE 10; PAGE 128, LINE 10
	SUB T1,DFQCNT		;How many do we need?
	JUMPLE T1,R		;None
	CALL SC.CDG		;(T1/T1,T2,T3) Create the datagram buffers
	IFNSK.
	  MOVE T2,DFQCNT
	  BUG. (CHK,SCADCF,SCAMPI,SOFT,<SCA - Datagram buffer creation failure>,<<T1,ERROR>,<T2,COUNT>>,<

Cause:	SCA detected that the level of buffers maintained was below minimum.
	The attempt to create more datagram buffers failed. The error code is
	in T1. Output is given as additional data.

 ---------------------------------
	SUB T1,DFQCNT		;How many do we need?
	JUMPLE T1,R		;None
	CALL SC.CDG		;(T1/T1,T2,T3) Create the datagram buffers
	IFNSK.
	  MOVE T2,DFQCNT
	  BUG.(CHK,SCADCF,SCAMPI,SOFT,<SCA - Datagram buffer creation failure>,<<T1,ERROR>,<T2,COUNT>>,<

Cause:	SCA detected that the level of buffers maintained was below minimum.
	The attempt to create more datagram buffers failed. The error code is
	in T1.  Output is given as additional data.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #60; PAGE 107, LINE 18; PAGE 129, LINE 18
	RET	
	SAVEP
SBD.FB:	JFFO T1,SBD.ST		;Find first system block that's stuck
	RET			;Could happen if node went offline
SBD.ST:	SKIPN P4,SBLIST(T2)	;Get system block address
	BUG. (HLT,SCANSB,SCAMPI,SOFT,<SCA - System block has gone away>,<<T2,NODE>>,<

Cause:	SC.DEF found a system block marked as stuck for buffers, but the address
	of the system block is 0.

Data:	NODE - Node number
>)

 ---------------------------------
	RET	
	SAVEP
SBD.FB:	JFFO T1,SBD.ST		;Find first system block that's stuck
	RET			;Could happen if node went offline
SBD.ST:	SKIPN P4,SBLIST(T2)	;Get system block address
	BUG.(HLT,SCANSB,SCAMPI,SOFT,<SCA - System block has gone away>,<<T2,NODE>>,<

Cause:	SC.DEF found a system block marked as stuck for buffers, but the
	address of the system block is 0.

Data:	NODE - Node number
>)


***** CHANGE #61; PAGE 109, LINE 8; PAGE 132, LINE 8
;a page.

SBD.ER:
	LOAD T2,SBDPA,(P4)	;Get node number
	LOAD T3,CBSCID,(P1)	;Get CID
	BUG. (HLT,SCAEBD,SCAMPI,SOFT,<SCA - Error handling buffer deferral request>,<<T2,NODE>,<T3,CID>,<T1,ERROR>>,<

Cause:	SCA was unable to create buffers when running in job 0. This should 
	never happen since job 0 can create pages as needed.
 ---------------------------------
;a page.

SBD.ER:
	LOAD T2,SBDPA,(P4)	;Get node number
	LOAD T3,CBSCID,(P1)	;Get CID
	BUG.(HLT,SCAEBD,SCAMPI,SOFT,<SCA - Error handling buffer deferral request>,<<T2,NODE>,<T3,CID>,<T1,ERROR>>,<

Cause:	SCA was unable to create buffers when running in job 0.  This should
	never happen since job 0 can create pages as needed.

***** CHANGE #62; PAGE 110, LINE 16; PAGE 133, LINE 16

;Returns +1: always

;Caller must have locked connection block

;It is possible to find block state non-zero here. This can happen,
;for example, if we had the c.b. queued waiting to send a credit_request,
;and a disconnect_request came in. In that case, we will store the new
;state on top of the old one. The block will already be queued, so we won't
;try to queue it again. This should happen only rarely.

 ---------------------------------

;Returns +1: always

;Caller must have locked connection block

;It is possible to find block state non-zero here. This can happen, for
;example, if we had the c.b. queued waiting to send a credit_request, and a
;disconnect_request came in. In that case, we will store the new state on top
;of the old one. The block will already be queued, so we won't try to queue it
;again. This should happen only rarely.


***** CHANGE #63; PAGE 110, LINE 30; PAGE 133, LINE 30
	IFSKP.
	  MOVE T3,.CBSBA(P1)	;Get address of system block
	  LOAD T3,SBDPA,(T3)	;Get node number
	  LOAD T4,CBSCID,(P1)	;Get CID
	  LOAD T2,CBBKST,(P1)	;Get block state
	  BUG. (INF,SCASBN,SCAMPI,SOFT,<SCA - Block state already non-zero>,<<T3,NODE>,<T4,CID>,<T2,OLDSTA>,<T1,NEWSTA>>,<

Cause:	While trying to set a connection's block state, it is found to be 
	already non-zero. This can happen legitimately under some conditions.

Action:	If the old state is anything except CREDIT_PEND, something is wrong.
	Try to trace the events that led to this, using the ring buffer, if
	necessary.
 ---------------------------------
	IFSKP.
	  MOVE T3,.CBSBA(P1)	;Get address of system block
	  LOAD T3,SBDPA,(T3)	;Get node number
	  LOAD T4,CBSCID,(P1)	;Get CID
	  LOAD T2,CBBKST,(P1)	;Get block state
	  BUG.(INF,SCASBN,SCAMPI,SOFT,<SCA - Block state already non-zero>,<<T3,NODE>,<T4,CID>,<T2,OLDSTA>,<T1,NEWSTA>>,<

Cause:	While trying to set a connection's block state, it is found to be
	already non-zero.  This can happen legitimately under some conditions.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.

	If the old state is anything except CREDIT_PEND, something is wrong.
	Try to trace the events that led to this, using the SCA ring buffer if
	necessary.

***** CHANGE #64; PAGE 110, LINE 51; PAGE 133, LINE 54
	  RET			;But don't try to queue it again
	ENDIF.
	STOR T1,CBBKST,(P1)	;Set state as requested
	CALL SC.AWQ		;(P1,P4/) Add this c.b. to the work queue
	RET

 ---------------------------------
	  RET			;But don't try to queue it again
	ENDIF.
	STOR T1,CBBKST,(P1)	;Set state as requested
	CALL SC.AWQ		;(P1,P4/) Add this c.b. to the work queue
	RET

***** CHANGE #65; PAGE 112, LINE 25; PAGE 135, LINE 25
	SAVEP
	SETZM P2		;Prepare to clear the address of the buffer
	EXCH P2,.SBOBB(P4)	;Grab outbound buffer if it's there
	JUMPE P2,R		;It was in use. We'll come here later
	SETZM P3		;Initialize address of first c.b. moved

 ---------------------------------
	SAVEP
	SETZM P2		;Prepare to clear the address of the buffer
	EXCH P2,.SBOBB(P4)	;Grab outbound buffer if it's there
	JUMPE P2,R		;It was in use. We'll come here later
	SETZM P3		;Initialize address of first c.b. moved


***** CHANGE #66; PAGE 112, LINE 41; PAGE 136, LINE 16
	TMNE CBBKST,(P1)	;Is block state still non-zero?
	IFSKP.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  LOAD T3,CBCNST,(P1)	;Get connection state
	  BUG. (CHK,SCABSZ,SCAMI,SOFT,<SCA - Block state is zero when trying to send connection management request>,<<T1,NODE>,<T2,CID>,<T3,STATE>>,<

 ---------------------------------
	TMNE CBBKST,(P1)	;Is block state still non-zero?
	IFSKP.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  LOAD T3,CBCNST,(P1)	;Get connection state
	  BUG.(CHK,SCABSZ,SCAMI,SOFT,<SCA - Block state is zero when trying to send connection management request>,<<T1,NODE>,<T2,CID>,<T3,STATE>>,<


***** CHANGE #67; PAGE 112, LINE 48; PAGE 136, LINE 23
Cause:	A connection block is on the system block's work queue, but its block
	state is zero. This value should be non-zero to indicate the kind of
	request to be sent.

Data:	NODE  - Node number
 ---------------------------------
Cause:	A connection block is on the system block's work queue, but its block
	state is zero. This value should be non-zero to indicate the kind of
	request to be sent.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.

Data:	NODE  - Node number

***** CHANGE #68; PAGE 112, LINE 59; PAGE 136, LINE 37
	  CALL SC.PTC		;(P1/) Allow reaper to get this block
	  CALL SC.ULK		;(P1/) Unlock the connection block
	  JRST SSN.BA		;Don't try to send.
	ENDIF.
   >				;end of ifn scadbg

 ---------------------------------
	  CALL SC.PTC		;(P1/) Allow reaper to get this block
	  CALL SC.ULK		;(P1/) Unlock the connection block
	  JRST SSN.BA		;Don't try to send.
	ENDIF.
   >				;end of ifn scadbg


***** CHANGE #69; PAGE 112, LINE 70; PAGE 137, LINE 11
   IFN SCADBG,<
	TMNN CBFRAP,(P1)	;Are we ready to reap this block?
	IFSKP.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get CID
	  BUG. (CHK,SCARBS,SCAMPI,SOFT,<SCA - Reap bit is set when block state is non-zero>,<<T1,NODE>,<T2,CID>>,<

Cause:	A connection block is marked as both waiting to send a request 
	message as reapable. The reapable bit should be set only when
	there is no need to send further requests.

Action:	The message is not sent and the block is deleted. If the
	problem persists, use the ring buffer to trace the events leading
	to it.

 ---------------------------------
   IFN SCADBG,<
	TMNN CBFRAP,(P1)	;Are we ready to reap this block?
	IFSKP.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get CID
	  BUG.(CHK,SCARBS,SCAMPI,SOFT,<SCA - Reap bit is set when block state is non-zero>,<<T1,NODE>,<T2,CID>>,<

Cause:	A connection block is marked as both waiting to send a request message
	as reapable. The reapable bit should be set only when there is no need
	to send further requests.  The message is not sent and the block is
	deleted. 

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #70; PAGE 120, LINE 39; PAGE 145, LINE 39
	SKIPN T3,.CBANB(P1) 	;Is there a current forward link
	SKIPE T4,.CBAPB(P1) 	;  or a link to a previous buffer???
	IFNSK.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  BUG. (CHK,SCABAL,SCAMPI,SOFT,<SCA - Connection block already linked>,<<T1,NODE>,<T2,CID>,<T3,FLINK>,<T4,BLINK>>,<

Cause:	SCA is linking a connection block onto a system block. However, the
	connection block's pointers indicate that it is already linked to some
	other block

 ---------------------------------
	SKIPN T3,.CBANB(P1) 	;Is there a current forward link
	SKIPE T4,.CBAPB(P1) 	;  or a link to a previous buffer???
	IFNSK.
	  LOAD T1,SBDPA,(P4)	;Get node number
	  LOAD T2,CBSCID,(P1)	;Get connect ID
	  BUG.(CHK,SCABAL,SCAMPI,SOFT,<SCA - Connection block already linked>,<<T1,NODE>,<T2,CID>,<T3,FLINK>,<T4,BLINK>>,<

Cause:	SCA is linking a connection block onto a system block.  However, the
	connection block's pointers indicate that it is already linked to some
	other block.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #71; PAGE 120, LINE 53; PAGE 145, LINE 56
	FLINK - Address of next connection block
	BLINK - Address of previous connection block
>)
	  CALL SC.RSQ		;(P1/) Dequeue CB from current queue
	ENDIF.

 ---------------------------------
	FLINK - Address of next connection block
	BLINK - Address of previous connection block
>)
	  CALL SC.RSQ		;(P1/) Dequeue CB from current queue
	ENDIF.


***** CHANGE #72; PAGE 125, LINE 38; PAGE 151, LINE 38
	SETZM .CBANB(P1)	;We just dequeued, zero links so SCLCB
	SETZM .CBAPB(P1)	;   does not dequeue again
	CALL <XENT SC.LCB>	;[7.1037](P1,P4/) Link this CB onto the remote's SB
	IFNSK.
	  LOAD T2,SBDPA,(P4)	;Get node number
	  BUG. (HLT,SCACLB,SCAMPI,SOFT,<SCA - Incoming connect_request on closed v.c.>,<<T2,NODE>>,<

Cause:	SCAMPI received a connect_request and matched it to a listener.
	But when SCAMPI tried to queue the connection block to the system 
	block, it found that the v.c. was closed. Since SCAMPI had checked 
	for that state earlier, and this is happening at interrupt level, 
	something unexpected has happened.
 ---------------------------------
	SETZM .CBANB(P1)	;We just dequeued, zero links so SCLCB
	SETZM .CBAPB(P1)	;   does not dequeue again
	CALL <XENT SC.LCB>	;[7.1037](P1,P4/) Link this CB onto the remote's SB
	IFNSK.
	  LOAD T2,SBDPA,(P4)	;Get node number
	  BUG.(HLT,SCACLB,SCAMPI,SOFT,<SCA - Incoming connect_request on closed v.c.>,<<T2,NODE>>,<

Cause:	SCAMPI received a connect_request and matched it to a listener.  But
	when SCAMPI tried to queue the connection block to the system block, it
	found that the virtual circuit was closed.  Since SCAMPI had checked
	for that state earlier, and this is happening at interrupt level,
	something unexpected has happened.

***** CHANGE #73; PAGE 129, LINE 1; PAGE 155, LINE 1

SC.CD7:
	;..

;Here because credit values suggest sending a credit_request. See
;if the state of the connection will allow it. We don't send if the connection
;isn't open. If we're still opening it, there's a risk of confusing the
;other side. If we're closing it, there's no need to send a credit_request.
;Also, we want to avoid sending anything after the disconnect sequence
;has started, because we will declare protocol complete and reap the block
;when the final disconnect_response comes in. We don't want to receive
;a credit_response after that.

 ---------------------------------

SC.CD7:
	;..


;Here because credit values suggest sending a credit_request. See if the state
;of the connection will allow it. We don't send if the connection isn't open.
;If we're still opening it, there's a risk of confusing the other side. If
;we're closing it, there's no need to send a credit_request.  Also, we want to
;avoid sending anything after the disconnect sequence has started, because we
;will declare protocol complete and reap the block when the final
;disconnect_response comes in. We don't want to receive a credit_response after
;that.


***** CHANGE #74; PAGE 129, LINE 16; PAGE 155, LINE 17
	;..
SCC.SM:	LOAD T1,CBCNST,(P1)	;Do this only if we're open
	CAIE T1,.CSOPN
	JRST SCC.RT

;Racefree, indicate that we're setting up for a credit_request, and make
;sure no other context is already doing it. If someone got here ahead of
;us, we don't need to send a credit_request, because our change is already
;reflected in .CBPRC. And we don't want to send one, because we might try
;to queue the same c.b. twice. Also, the second message would contain 0,
;so why bother? If credit is still pending when the credit_response comes
;in, we'll send another credit_request.

 ---------------------------------
	;..
SCC.SM:	LOAD T1,CBCNST,(P1)	;Do this only if we're open
	CAIE T1,.CSOPN
	JRST SCC.RT

;Racefree, indicate that we're setting up for a credit_request, and make sure
;no other context is already doing it. If someone got here ahead of us, we
;don't need to send a credit_request, because our change is already reflected
;in .CBPRC. And we don't want to send one, because we might try to queue the
;same c.b. twice. Also, the second message would contain 0, so why bother? If
;credit is still pending when the credit_response comes in, we'll send another
;credit_request.


***** CHANGE #75; PAGE 129, LINE 29; PAGE 155, LINE 30
	SETOM T1		;Indicate credit_request is pending
	EXCH T1,.CBPND(P1)	;Get the old value
	JUMPL T1,SCC.RT		;Already pending. Don't want to do this
	LOAD T1,CBBKST,(P1)	;Get the block state
	SKIPN T1
	IFSKP.
	  MOVE T1,.CBSBA(P1)	;Get address of system block
	  LOAD T1,SBDPA,(T1)	;Get port number
	  LOAD T2,CBSCID,(P1)	;Get CID
	  LOAD T3,CBBKST,(P1)	;Get state
	  BUG. (CHK,SCACSC,SCAMPI,SOFT,<SCA - Can't send credit request>,<<T1,NODE>,<T2,CID>,<T3,STATE>>,<

Cause:	SCA wants to send a credit request, but the connection block already
	has some other message pending. This reflects some sort of
	inconsistency, since the state was "open", and the interlock word
	for credit requests was 0.

 ---------------------------------
	SETOM T1		;Indicate credit_request is pending
	EXCH T1,.CBPND(P1)	;Get the old value
	JUMPL T1,SCC.RT		;Already pending. Don't want to do this
	LOAD T1,CBBKST,(P1)	;Get the block state
	IFN. T1			;[8914] Message pending?
	  MOVE T1,.CBSBA(P1)	;Get address of system block
	  LOAD T1,SBDPA,(T1)	;Get port number
	  LOAD T2,CBSCID,(P1)	;Get CID
	  LOAD T3,CBBKST,(P1)	;Get state
	  BUG.(CHK,SCACSC,SCAMPI,SOFT,<SCA - Can't send credit request>,<<T1,NODE>,<T2,CID>,<T3,STATE>>,<

Cause:	SCA wants to send a credit request, but the connection block already
	has some other message pending.  This reflects some sort of
	inconsistency, since the state was "open", and the interlock word for
	credit requests was 0.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #76; PAGE 129, LINE 54; PAGE 155, LINE 57
	ENDIF.
	MOVEI T1,.BSCRP		;Set block state to credit_pending
	CALL SC.SCA		;(T1,P1,P4/) Set block state and queue block
	RETSKP			;Tell caller to get it sent

SCC.RT:	
	RET
 ---------------------------------
	ENDIF.
	MOVEI T1,.BSCRP		;Set block state to credit_pending
	CALL SC.SCA		;(T1,P1,P4/) Set block state and queue block
	RETSKP			;Tell caller to get it sent

SCC.RT:	RET

***** CHANGE #77; PAGE 131, LINE 31; PAGE 157, LINE 31
	  TXO T3,RPQFMG   	;Set message free queue flag
	  CALL RG.PQM		;(T1,T2,T3,T4,P4) Record port manipulation
>	         		;End of IFN SCARNG
	  LOAD T1,SBDPA,(P4)	;get node number
	  LOAD T2,CBSCID,(P1)	;get our connect ID
	  BUG. (CHK,SCACRB,SCAMPI,SOFT,<SCA - Can't reclaim buffers>,<<T1,NODE>,<T2,CID>,<P3,COUNT>>,<

Cause:	Based on the return_credit field for this connection, SCAMPI is trying
	to reclaim buffers from the port's queue. The queue is empty. This
	reflects confusion about credit, since these buffers should have 
	been queued at some time in the past.

 ---------------------------------
	  TXO T3,RPQFMG   	;Set message free queue flag
	  CALL RG.PQM		;(T1,T2,T3,T4,P4) Record port manipulation
>	         		;End of IFN SCARNG
	  LOAD T1,SBDPA,(P4)	;get node number
	  LOAD T2,CBSCID,(P1)	;get our connect ID
	  BUG.(CHK,SCACRB,SCAMPI,SOFT,<SCA - Can't reclaim buffers>,<<T1,NODE>,<T2,CID>,<P3,COUNT>>,<

Cause:	Based on the return_credit field for this connection, SCAMPI is trying
	to reclaim buffers from the port's queue.  The queue is empty.  This
	reflects confusion about credit, since these buffers should have been
	queued at some time in the past.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #78; PAGE 131, LINE 45; PAGE 157, LINE 48
	CID   - Connect ID at this node
	COUNT - Number of buffers we couldn't get
>)
	  JRST SCG.EX	  
	ENDIF.
   IFN SCARNG,<
 ---------------------------------
	CID   - Connect ID at this node
	COUNT - Number of buffers we couldn't get
>)
	  JRST SCG.EX	  
	ENDIF.

   IFN SCARNG,<

***** CHANGE #79; PAGE 136, LINE 35; PAGE 163, LINE 35
	SKIPE T1		;Shouldn't be zero
	CAILE T1,YSIZE		;Too big?
	IFNSK.
	  LOAD T2,SBDPA,(P4)	;Get node number
	  LOAD T3,CBSCID,(P1)	;Get connect ID
	  BUG. (HLT,SCAIBS,SCAMPI,SOFT,<SCA - Invalid block state>,<<T2,NODE>,<T3,CID>,<T1,BSTATE>>,<

Cause:	This caller to SC.RQS provided a block state larger than the table 
	(or zero).

 ---------------------------------
	SKIPE T1		;Shouldn't be zero
	CAILE T1,YSIZE		;Too big?
	IFNSK.
	  LOAD T2,SBDPA,(P4)	;Get node number
	  LOAD T3,CBSCID,(P1)	;Get connect ID
	  BUG.(HLT,SCAIBS,SCAMPI,SOFT,<SCA - Invalid block state>,<<T2,NODE>,<T3,CID>,<T1,BSTATE>>,<

Cause:	This caller to SC.RQS provided a block state larger than the table (or
	zero).  The caller has a problem.


***** CHANGE #80; PAGE 140, LINE 29; PAGE 167, LINE 29
	CAIN T3,.CSCLO		;Should be closed
	IFSKP.
	  MOVE T4,(P)		;Get address of caller
	  LOAD T1,CBSCID,(P1)	;Get connect ID
	  CALL <XENT SC.NOD>	;[7.1037](T1/T1,T2) Get node number
	  BUG. (CHK,SCAPCC,SCAMPI,SOFT,<SCA - Protocol complete set and connect state not closed>,<<T2,NODE>,<T1,CID>,<T3,CSTATE>,<T4,CALLER>>,<

Cause:	A call was made to SC.PTC indicating that the protocol was complete.
	When this happens, the connect state should be closed.  It was not.

 ---------------------------------
	CAIN T3,.CSCLO		;Should be closed
	IFSKP.
	  MOVE T4,(P)		;Get address of caller
	  LOAD T1,CBSCID,(P1)	;Get connect ID
	  CALL <XENT SC.NOD>	;[7.1037](T1/T1,T2) Get node number
	  BUG.(CHK,SCAPCC,SCAMPI,SOFT,<SCA - Protocol complete set and connect state not closed>,<<T2,NODE>,<T1,CID>,<T3,CSTATE>,<T4,CALLER>>,<

Cause:	A call was made to SC.PTC indicating that the protocol was complete.
	When this happens, the connect state should be closed.  It was not.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #81; PAGE 149, LINE 50; PAGE 176, LINE 50
	MOVE T2,T1		;Start buffer loop with addr of first buffer
SAB.BL:	SOJLE T4,SAB.CR		;If no more buffers,fix up count and Q pointers
	SKIPN T2,(T2)		;Now on to the next buffer
	JRST SAB.ER		;No entry!!! Recover from a bad FQCNT
	JRST SAB.BL		;Loop for more entries

 ---------------------------------
	MOVE T2,T1		;Start buffer loop with addr of first buffer
SAB.BL:	SOJLE T4,SAB.CR		;If no more buffers,fix up count and Q pointers
	SKIPN T2,(T2)		;Now on to the next buffer
	JRST SAB.ER		;No entry!!! Recover from a bad FQCNT
	JRST SAB.BL		;Loop for more entries


***** CHANGE #82; PAGE 149, LINE 90; PAGE 177, LINE 40
	ENDBK.
>				;End of IFN SCARNG
	MOVE T2,NUMBUF		;Return the number of buffers requested
	MOVE T3,[MSEC1,,SC.RBF]	;[7.1048] Tell caller where to return buffer
	RETSKP			;Return all OK

 ---------------------------------
	ENDBK.
>				;End of IFN SCARNG
	MOVE T2,NUMBUF		;Return the number of buffers requested
	MOVE T3,[MSEC1,,SC.RBF]	;[7.1048] Tell caller where to return buffer
	RETSKP			;Return all OK


***** CHANGE #83; PAGE 149, LINE 110; PAGE 178, LINE 20
	MOVE T3,RMRCNT		;Get count of refused requests
	SETOM T4		;Indicate buffer was not created
	CALL RG.BFM		;(T1,T2,T3,T4) Record buffer management
>				;End of IFN SCARNG
	RETBAD (SCSNBA)		;Return not enough buffers

 ---------------------------------
	MOVE T3,RMRCNT		;Get count of refused requests
	SETOM T4		;Indicate buffer was not created
	CALL RG.BFM		;(T1,T2,T3,T4) Record buffer management
>				;End of IFN SCARNG
	RETBAD (SCSNBA)		;Return not enough buffers


***** CHANGE #84; PAGE 149, LINE 120; PAGE 179, LINE 10

SAB.ER:	MOVE T1,FQCNT		;Get number of buffers we thought we had
	MOVE T2,TOPFQ		;Get pointer to top of free queue
	MOVE T3,BOTFQ		;Get pointer to bottom of free queue
	MOVE T3,NUMBUF		;Get number of buffers requested
	BUG. (CHK,SCAMCR,SCAMPI,SOFT,<SCA - Message buffer count was incorrect>,<<T1,COUNT>,<T2,TOPQ>,<T3,BOTQ>,<T4,BUFNUM>>,<

Cause:	There are no message buffers when the count indicated there are enough.

 ---------------------------------

SAB.ER:	MOVE T1,FQCNT		;Get number of buffers we thought we had
	MOVE T2,TOPFQ		;Get pointer to top of free queue
	MOVE T3,BOTFQ		;Get pointer to bottom of free queue
	MOVE T3,NUMBUF		;Get number of buffers requested
	BUG.(CHK,SCAMCR,SCAMPI,SOFT,<SCA - Message buffer count was incorrect>,<<T1,COUNT>,<T2,TOPQ>,<T3,BOTQ>,<T4,BUFNUM>>,<

Cause:	There are no message buffers when the count indicated there are enough.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.


***** CHANGE #85; PAGE 149, LINE 144; PAGE 179, LINE 37
	AOS RMRCNT		;Increment the number of refused requests
	MOVE T1,NUMBUF		;Get the request size again
	ADD T1,ASRMR		;Add current average size to  new request
	IDIVI T1,2		;Take the average of the two
	MOVEM T1,ASRMR		;Update the average size of refused request

 ---------------------------------
	AOS RMRCNT		;Increment the number of refused requests
	MOVE T1,NUMBUF		;Get the request size again
	ADD T1,ASRMR		;Add current average size to  new request
	IDIVI T1,2		;Take the average of the two
	MOVEM T1,ASRMR		;Update the average size of refused request


***** CHANGE #86; PAGE 150, LINE 46; PAGE 181, LINE 46
	MOVE T1,Q1		;Get the page count we will create
	IMUL T1,NUMPPG		;Find the total number of buffers created
	MOVEM T1,NUMBUF		;Save for later return
	SETZ P4,		;Zero the page chain foreward pointer
	MOVEI P5,P4		;Make the tail pointer point to the head

 ---------------------------------
	MOVE T1,Q1		;Get the page count we will create
	IMUL T1,NUMPPG		;Find the total number of buffers created
	MOVEM T1,NUMBUF		;Save for later return
	SETZ P4,		;Zero the page chain foreward pointer
	MOVEI P5,P4		;Make the tail pointer point to the head


***** CHANGE #87; PAGE 150, LINE 80; PAGE 182, LINE 34
	MOVE T1,P4		;Get the base address of the page chain
	MOVE T2,BUFSZ		;Get the size of the buffers desired
	CALL SC.BBF		;Create the buffer chain
	MOVE T3,NUMBUF		;Return the total buffer count as well
	RETSKP			;Return all OK

 ---------------------------------
	MOVE T1,P4		;Get the base address of the page chain
	MOVE T2,BUFSZ		;Get the size of the buffers desired
	CALL SC.BBF		;Create the buffer chain
	MOVE T3,NUMBUF		;Return the total buffer count as well
	RETSKP			;Return all OK


***** CHANGE #88; PAGE 150, LINE 89; PAGE 183, LINE 9
;for SCA buffers that we overrun the section. Hence die here... In general
;this means someone is not lighting F.RTB when it should be and buffers are 
;ending up on the hardware free queue by the dozen...
;
SCM.ER:	OKSKED
	BUG. (CHK,SCABSF,SCAMPI,SOFT,<SCA - Buffer section full>,<>,<

Cause:	SCA went to create more buffers and discovered that the section
	is full. This is an indication that buffers are not being returned.
>)
 ---------------------------------
;for SCA buffers that we overrun the section. Hence die here... In general
;this means someone is not lighting F.RTB when it should be and buffers are 
;ending up on the hardware free queue by the dozen...
;
SCM.ER:	OKSKED
	BUG.(CHK,SCABSF,SCAMPI,SOFT,<SCA - Buffer section full>,<>,<

Cause:	SCA went to create more buffers and discovered that the section is
	full.  This is an indication that buffers are not being returned.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.
>)

***** CHANGE #89; PAGE 154, LINE 50; PAGE 187, LINE 50
	SKIPE T2,(T2)		;Is there a next buffer?
	AOJA T3,.-1		;Yes, continue to count buffers
	CAMN T3,T1		;Are the counts the same?
	RET			;Yes, return ok

SCT.ER:	BUG. (HLT,SCATQF,SCAMPI,SOFT,<SCA - Trace of free queue detected inconsistency>,,<

 ---------------------------------
	SKIPE T2,(T2)		;Is there a next buffer?
	AOJA T3,.-1		;Yes, continue to count buffers
	CAMN T3,T1		;Are the counts the same?
	RET			;Yes, return ok

SCT.ER:	BUG.(HLT,SCATQF,SCAMPI,SOFT,<SCA - Trace of free queue detected inconsistency>,,<


***** CHANGE #90; PAGE 160, LINE 33; PAGE 193, LINE 33
>				;End of IFN SCARNG
   IFE SCARNG, SAVEAC <T1>
	CONI PI,T1		;Get current PI state
	TRNE T1,<1B25>		;Are we in KLIPA interrupt code???
	RET			;Yes, we have nothing to do here

 ---------------------------------
>				;End of IFN SCARNG
   IFE SCARNG, SAVEAC <T1>
	CONI PI,T1		;Get current PI state
	TRNE T1,<1B25>		;Are we in KLIPA interrupt code???
	RET			;Yes, we have nothing to do here


***** CHANGE #91; PAGE 160, LINE 43; PAGE 194, LINE 10
;nothing. The channel on done by routine A will actually turn on the channel.
;
	SOSLE CHNCTL		;Is this an outermost CION in a nest???
	 RET			;No, do nothing
	SKIPE CHNCTL
	BUG. (HLT,SCAODI,SCAMPI,SOFT,<SCA - Overly decremented CI interlock>,,<

Cause:	A CION was done when no previous CIOFF had occurred. This leads to
	an overly-decremented lock.

Action:	If you are unable to determine how this happened, turn on the ring
	buffer tracing of interlocks. (Do this by setting RPITRN in RINGSW.)
	It should be possible to pair each CION with a preceding CIOFF.
	Note that these calls are invoked from several CI-related modules.
>)
 ---------------------------------
;nothing. The channel on done by routine A will actually turn on the channel.
;
	SOSLE CHNCTL		;Is this an outermost CION in a nest???
	 RET			;No, do nothing
	SKIPE CHNCTL
	BUG.(HLT,SCAODI,SCAMPI,SOFT,<SCA - Overly decremented CI interlock>,,<

Cause:	A CION was done when no previous CIOFF had occurred. This leads to an
	overly-decremented lock.

Action:	Send in an SPR along with instruction on reproducing the problem.

	If you are unable to determine how this happened, turn on the ring
	buffer tracing of interlocks. (Do this by setting RPITRN in RINGSW.)
	It should be possible to pair each CION with a preceding CIOFF.  Note
	that these calls are invoked from several CI-related modules.
>)

***** CHANGE #92; PAGE 162, LINE 36; PAGE 196, LINE 36
	  OKSKED		;Lock routine went NOSKED
	  RET			;Last unlocker will do the rest
	ENDIF.
   IFN SCADBG,<
	SKIPG T1		;Should never be less than 1
	BUG. (HLT,SCAILC,SCAMPI,SOFT,<SCA - Illegal lock count in connection block>,,<

Cause:	The routine to unlock a connection block found that the count
	is less than 1. Since the caller must have locked the connection
	block earlier, something unexpected has happened.

>)
   >

 ---------------------------------
	  OKSKED		;Lock routine went NOSKED
	  RET			;Last unlocker will do the rest
	ENDIF.
   IFN SCADBG,<
	SKIPG T1		;Should never be less than 1
	BUG.(HLT,SCAILC,SCAMPI,SOFT,<SCA - Illegal lock count in connection block>,,<

Cause:	The routine to unlock a connection block found that the count is less
	than 1. Since the caller must have locked the connection block earlier,
	something unexpected has happened.
>)
   >


***** CHANGE #93; PAGE 162, LINE 82; PAGE 197, LINE 38
	IFSKP.
	  SETZRO CBFSNM,(P1)	;Yes. Indicate no longer deferred
	  SETOM P3		;Remember this
	ENDIF.
	JRST SC.UL2		;See if anything new has come up

 ---------------------------------
	IFSKP.
	  SETZRO CBFSNM,(P1)	;Yes. Indicate no longer deferred
	  SETOM P3		;Remember this
	ENDIF.
	JRST SC.UL2		;See if anything new has come up


***** CHANGE #94; PAGE 162, LINE 92; PAGE 198, LINE 10
	CION
   IFN SCADBG,<
	SKIPE .CBLCK(P1)	;Better be zero now!
	BUG. (HLT,SCALCC,SCAMPI,SOFT,<SCA - Connection block lock count has changed>,,<

Cause:	The routine to unlock a connection block has found an inconsistency
	in the lock count. Earlier in the routine, the current count was 1.
	Now, after decrementing it, the count is not zero. Some other context
	must have changed it. This should not happen since the locker is
	NOSKED and interrupt level code doesn't change the lock.
>)
 ---------------------------------
	CION
   IFN SCADBG,<
	SKIPE .CBLCK(P1)	;Better be zero now!
	BUG. (HLT,SCALCC,SCAMPI,SOFT,<SCA - Connection block lock count has changed>,,<

Cause:	The routine to unlock a connection block has found an inconsistency in
	the lock count. Earlier in the routine, the current count was 1.  Now,
	after decrementing it, the count is not zero. Some other context must
	have changed it. This should not happen since the locker is NOSKED and
	interrupt level code doesn't change the lock.
>)

***** CHANGE #95; PAGE 164, LINE 36; PAGE 200, LINE 36
	JRST SC.RHE		;No, return error
	MOVE T1,RNGADR		;Get current pointer position
	MOVEM T1,RNGCUR		;Save as the top of this new entry
	ADD T1,ENTLTB(T2)	;Determine end of entry
	SOS T1			;Adjust since already pointing at first word
	CAMG T1,RNGBOT		;Will it overflow?
 ---------------------------------
	JRST SC.RHE		;No, return error
	MOVE T1,RNGADR		;Get current pointer position
	MOVEM T1,RNGCUR		;Save as the top of this new entry
	ADD T1,ENTLTB(T2)	;Determine end of entry
	SOS T1			;Adjust since already pointing at first word

	CAMG T1,RNGBOT		;Will it overflow?

***** CHANGE #96; PAGE 164, LINE 50; PAGE 201, LINE 15
;of RNGSW will cause many SCARAPs to be issued.  However, if reporting of 
;this condition is desired, the above SKIPA instruction can be removed.

	  BUG.(INF,SCARAP,SCAMPI,SOFT,<SCA - Ring buffer has wrapped>,,<

Cause:	The ring buffer overflowed and wrapped upon itself.
	The entries at the top are being overwritten and will be lost.
>)				;Report wraping of ring buffer
 ---------------------------------
;of RNGSW will cause many SCARAPs to be issued.  However, if reporting of 
;this condition is desired, the above SKIPA instruction can be removed.

	  BUG.(INF,SCARAP,SCAMPI,SOFT,<SCA - Ring buffer has wrapped>,,<

Cause:	The ring buffer overflowed and wrapped upon itself.  The entries at the
	top are being overwritten and will be lost.

Action:	No action required, this bug is for information only.
>)				;Report wraping of ring buffer

***** CHANGE #97; PAGE 164, LINE 70; PAGE 201, LINE 37
	MOVE T1,PCC		;Get PC of caller to feature routine
	CALL SC.RWR		;(T1) Place in header
	MOVE T1,TODCLK		;Get time of day
	CALL SC.RWR		;(T1) Place in entry header
	RET			;Return success

 ---------------------------------
	MOVE T1,PCC		;Get PC of caller to feature routine
	CALL SC.RWR		;(T1) Place in header
	MOVE T1,TODCLK		;Get time of day
	CALL SC.RWR		;(T1) Place in entry header
	RET			;Return success


***** CHANGE #98; PAGE 164, LINE 82; PAGE 202, LINE 12

Cause:	A bad event code was passed to SC.RHD to be placed in the ring buffer
	header.  This could cause problems since the entry length table is
	indexed by event code.

Action:	Check the caller of SC.RHD to be sure that a valid event code is being  
	placed in T3 before the call. The caller is indicated by the jacket 
	routine address in T4.
 ---------------------------------

Cause:	A bad event code was passed to SC.RHD to be placed in the ring buffer
	header.  This could cause problems since the entry length table is
	indexed by event code.

Action:	Send in an SPR along with instruction on reproducing the problem.

	Check the caller of SC.RHD to be sure that a valid event code is being
	placed in T3 before the call. The caller is indicated by the jacket
	routine address in T4.

***** CHANGE #99; PAGE 175, LINE 28; PAGE 213, LINE 28
;
;Note that it is the caller's responsibility to return the SCA buffers
;after it has sent them.

	XRESCD			;Called from anywhere, anytime - always home

 ---------------------------------
;
;Note that it is the caller's responsibility to return the SCA buffers
;after it has sent them.

	XRESCD			;Called from anywhere, anytime - always home


***** CHANGE #100; PAGE 175, LINE 41; PAGE 214, LINE 13
SC.BR2:	STKVAR <DATA,NBUFS,WRDS,OFFSET,FIRST,BLTIT,FILWRD,GIVBAC> ;Temp storage
	MOVEM Q1,BLTIT		;Put it in this location
	SKIPGE INTDF		;Caller NOINT?
	BUG.(CHK,SCBROK,SCAMPI,SOFT,<SC.BRK called while OKINT>,<<T1,PCC>>,<

Cause:	This is a coding problem. SC.BRK will be attempting to assign
	SCA buffers to the caller and being OKINT leaves a possible window
	where SCA buffers could be lost.

Action:	Make the caller NOINT until it has returned the SCA buffers.

 ---------------------------------
SC.BR2:	STKVAR <DATA,NBUFS,WRDS,OFFSET,FIRST,BLTIT,FILWRD,GIVBAC> ;Temp storage
	MOVEM Q1,BLTIT		;Put it in this location
	SKIPGE INTDF		;Caller NOINT?
	BUG.(CHK,SCBROK,SCAMPI,SOFT,<SC.BRK called while OKINT>,<<T1,PCC>>,<

Cause:	This is a coding problem. SC.BRK will be attempting to assign SCA
	buffers to the caller and being OKINT leaves a possible window where
	SCA buffers could be lost.

Action:	If this bug is reproducible, change it to a BUGHLT and submit an SPR
	with the dump and instructions for reproducing it.

	Make the caller NOINT until it has returned the SCA buffers.


***** CHANGE #101; PAGE 175, LINE 67; PAGE 214, LINE 42
	CALL <XENT SC.ABF>	;(T1/T1,T2,T3) Get those SCA buffers
	 RETBAD(,OKINT)		;Failed, pass back error
	MOVEM T1,FIRST		;Save address of first buffer
	MOVEM T3,GIVBAC		;Save routine to return SCA buffers for caller
	MOVE T2,DATA		;Get data address back

 ---------------------------------
	CALL <XENT SC.ABF>	;(T1/T1,T2,T3) Get those SCA buffers
	 RETBAD(,OKINT)		;Failed, pass back error
	MOVEM T1,FIRST		;Save address of first buffer
	MOVEM T3,GIVBAC		;Save routine to return SCA buffers for caller
	MOVE T2,DATA		;Get data address back


