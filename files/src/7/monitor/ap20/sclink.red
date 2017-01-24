REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:29:13
File 1: SRC:<7.MONITOR>SCLINK.MAC.1
File 2: SRC:<7.MONITOR.AP20>SCLINK.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8948 to SCLINK.MAC on 25-Aug-88 by RASPUZZI, for SPR #22244
;Prevent CFCLDP BUGHLTs and DECnet freespace lossage by having the session
;control layer check a link number before it dishes it out to the job. If too
;large, then cleanup and take an error path back so SCJSYS does the right
;thing from there.
; Edit= 8934 to SCLINK.MAC on 23-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 3, LINE 27; PAGE 3, LINE 27
;    7. SCTSEC - Once-a-second service for SCLINK  . . . . . .  50
;    8. SCTRFJ - Request Second Service for a Link . . . . . .  51
;    9. SCTL's Interlock
;        9.1    Queuing Version  . . . . . . . . . . . . . . .  52
;        9.2    Waiting Version  . . . . . . . . . . . . . . .  53
;        9.3    Flag Version . . . . . . . . . . . . . . . . .  54
;   10. SCTULK - SCTL's Interlock
;       10.1    Unlock Checks  . . . . . . . . . . . . . . . .  55
;   11. NSP.
;       11.1    Function Dispatch Table  . . . . . . . . . . .  56
;       11.2    Wait Check Tables
;           11.2.1    Definitons . . . . . . . . . . . . . . .  57
;           11.2.2    "Before" NSP. table  . . . . . . . . . .  58
;           11.2.3    "After" NSP. table . . . . . . . . . . .  59
;           11.2.4    Routines . . . . . . . . . . . . . . . .  60
;       11.3    SCTNSF - The Entry to SCLINK from SCMUUO . . .  61
;       11.4    NSFRE - REset all links  . . . . . . . . . . .  67
;       11.5    NSFEA - Enter Active State . . . . . . . . . .  68
;       11.6    NSFEP - Enter Passive State  . . . . . . . . .  71
;       11.7    NSFRI - Read Connect Information . . . . . . .  72
;       11.8    NSFAC - Accept the Connect . . . . . . . . . .  73
;       11.9    NSFRJ - Reject the Connect . . . . . . . . . .  75
;       11.10   NSFRC - Read Connect Confirm Information . . .  76
;       11.11   NSFSD - Synchronous Disconnect . . . . . . . .  77
;       11.12   NSFAB - Abort and Release  . . . . . . . . . .  79
;       11.13   NSFRD - Read Disconnect Data . . . . . . . . .  80
;       11.14   NSFRL - Release the Channel  . . . . . . . . .  81
;       11.15   NSFRS - Read the Channel Status  . . . . . . .  82
;       11.16   NSFIS - Send Interrupt Data  . . . . . . . . .  83
;       11.17   NSFIR - Receive Interrupt Data . . . . . . . .  85
;       11.18   NSFDS - Send Normal Data . . . . . . . . . . .  87
;       11.19   NSFDR - Receive Normal Data  . . . . . . . . .  91
;       11.20   NSFSQ - Set Quotas and Goals . . . . . . . . .  93
 ---------------------------------
;    7. SCTSEC - Once-a-second service for SCLINK  . . . . . .  50
;    8. SCTRFJ - Request Second Service for a Link . . . . . .  51
;    9. SCTL's Interlock
;        9.1    Queuing Version  . . . . . . . . . . . . . . .  52
;        9.2    Waiting Version  . . . . . . . . . . . . . . .  53
;        9.3    Flag Version . . . . . . . . . . . . . . . . .  55
;   10. SCTULK - SCTL's Interlock
;       10.1    Unlock Checks  . . . . . . . . . . . . . . . .  56
;   11. NSP.
;       11.1    Function Dispatch Table  . . . . . . . . . . .  57
;       11.2    Wait Check Tables
;           11.2.1    Definitons . . . . . . . . . . . . . . .  58
;           11.2.2    "Before" NSP. table  . . . . . . . . . .  59
;           11.2.3    "After" NSP. table . . . . . . . . . . .  60
;           11.2.4    Routines . . . . . . . . . . . . . . . .  61
;       11.3    SCTNSF - The Entry to SCLINK from SCMUUO . . .  62
;       11.4    NSFRE - REset all links  . . . . . . . . . . .  69
;       11.5    NSFEA - Enter Active State . . . . . . . . . .  70
;       11.6    NSFEP - Enter Passive State  . . . . . . . . .  73
;       11.7    NSFRI - Read Connect Information . . . . . . .  74
;       11.8    NSFAC - Accept the Connect . . . . . . . . . .  75
;       11.9    NSFRJ - Reject the Connect . . . . . . . . . .  77
;       11.10   NSFRC - Read Connect Confirm Information . . .  78
;       11.11   NSFSD - Synchronous Disconnect . . . . . . . .  79
;       11.12   NSFAB - Abort and Release  . . . . . . . . . .  81
;       11.13   NSFRD - Read Disconnect Data . . . . . . . . .  82
;       11.14   NSFRL - Release the Channel  . . . . . . . . .  83
;       11.15   NSFRS - Read the Channel Status  . . . . . . .  84
;       11.16   NSFIS - Send Interrupt Data  . . . . . . . . .  85
;       11.17   NSFIR - Receive Interrupt Data . . . . . . . .  87
;       11.18   NSFDS - Send Normal Data . . . . . . . . . . .  89
;       11.19   NSFDR - Receive Normal Data  . . . . . . . . .  93
;       11.20   NSFSQ - Set Quotas and Goals . . . . . . . . .  95

***** CHANGE #3; PAGE 5, LINE 8; PAGE 5, LINE 8
;		     Table of Contents for SCLINK
;
;				  Section		      Page
;
;
;       11.21   NSFRQ - Read Quotas and Goals  . . . . . . . .  94
;       11.22   NSFJS - Set Job Quotas and Goals . . . . . . .  95
;       11.23   NSFJR - Read Job Quotas and Goals  . . . . . .  96
;       11.24   NSFPI - Set PSI Reason Mask  . . . . . . . . .  97
;   12. RETFLW - Return current flow controls  . . . . . . . .  98
;   13. SNDDRQ - Send Data Requests  . . . . . . . . . . . . .  99
;   14. RLSLNK - Release a Link  . . . . . . . . . . . . . . . 100
;   15. LLINKS Calls
;       15.1    Entry Vector Table . . . . . . . . . . . . . . 101
;       15.2    SCTRIB - Reserve Input Buffer  . . . . . . . . 102
;       15.3    SCTUCG - Uncongestion Call . . . . . . . . . . 103
;       15.4    SCTLCI - Connect Initiate Call . . . . . . . . 104
;       15.5    The Vectored Call Entry Point SCTL . . . . . . 107
;       15.6    SCCCR - Connect Confirmed call from NSP  . . . 108
;       15.7    SCDIR - Disconnect Initiate received call  . . 109
;       15.8    SCDCR - Disconnect Confirm received call . . . 110
;       15.9    SCODN - Output done call . . . . . . . . . . . 111
;       15.10   SCSEG - Segment received call  . . . . . . . . 112
;       15.11   SCDRQ - Data request received call . . . . . . 113
;       15.12   SCNCF - No confidence in port call . . . . . . 114
;       15.13   SCNRS - No resources call  . . . . . . . . . . 115
;       15.14   SCCLS - Close Completed call . . . . . . . . . 116
;       15.15   SCNLK - No link call . . . . . . . . . . . . . 117
;       15.16   SCNCM - No communication call  . . . . . . . . 118
;       15.17   SCNRN - Not in Run State call  . . . . . . . . 119
;       15.18   SCCAK - Got a Connect ACK  . . . . . . . . . . 120
;   16. Subroutines
;       16.1    CHKABO - Check SLABO flag  . . . . . . . . . . 121
;       16.2    CHKPPN - Check self PPN for validity . . . . . 122
;       16.3    FREMSG - Free a message block  . . . . . . . . 123
;       16.4    CONBUF - Invoke Conservative Buffering . . . . 124
;       16.5    FNDSLB - Find SLB given a channel number . . . 125
;       16.6    FNDSBI - Find SLB from SLBid . . . . . . . . . 126
;       16.7    FRESLB - Deallocate a SLB  . . . . . . . . . . 127
;       16.8    FRECBP - Free Connect Block  . . . . . . . . . 128
;       16.9    CDMCBP - Copy User Data from Message Block . . 129
;       16.10   CPMSCB - Copy User Data from Message Block . . 130
;       16.11   FRESJB - Deallocate a SJB  . . . . . . . . . . 131
;       16.12   MAKSLB - Create a SLB, filling in defaults . . 132
;       16.13   MAKSJB - Create a SJB, filling in defaults . . 134
;       16.14   SCSSTS/CHKSTS - Set Status & Inform PSISER . . 135
;       16.15   SCTWKQ - Queue a Link for later call to SCTPSQ 137
;       16.16   SLBMAT - Pattern match connect to passive SLBs 138
;       16.17   CDBMAT - Match Two Connect Blocks  . . . . . . 139
;       16.18   STRMAT - Pattern Matcher . . . . . . . . . . . 142
;       16.19   STPTMR - Stop the Connect Initiate Timer . . . 143
;       16.20   STRTIM - Start the Connect Initiate Timer  . . 144
 ---------------------------------
;		     Table of Contents for SCLINK
;
;				  Section		      Page
;
;
;       11.21   NSFRQ - Read Quotas and Goals  . . . . . . . .  96
;       11.22   NSFJS - Set Job Quotas and Goals . . . . . . .  97
;       11.23   NSFJR - Read Job Quotas and Goals  . . . . . .  98
;       11.24   NSFPI - Set PSI Reason Mask  . . . . . . . . .  99
;   12. RETFLW - Return current flow controls  . . . . . . . . 100
;   13. SNDDRQ - Send Data Requests  . . . . . . . . . . . . . 101
;   14. RLSLNK - Release a Link  . . . . . . . . . . . . . . . 102
;   15. LLINKS Calls
;       15.1    Entry Vector Table . . . . . . . . . . . . . . 103
;       15.2    SCTRIB - Reserve Input Buffer  . . . . . . . . 104
;       15.3    SCTUCG - Uncongestion Call . . . . . . . . . . 106
;       15.4    SCTLCI - Connect Initiate Call . . . . . . . . 107
;       15.5    The Vectored Call Entry Point SCTL . . . . . . 110
;       15.6    SCCCR - Connect Confirmed call from NSP  . . . 111
;       15.7    SCDIR - Disconnect Initiate received call  . . 112
;       15.8    SCDCR - Disconnect Confirm received call . . . 113
;       15.9    SCODN - Output done call . . . . . . . . . . . 114
;       15.10   SCSEG - Segment received call  . . . . . . . . 115
;       15.11   SCDRQ - Data request received call . . . . . . 116
;       15.12   SCNCF - No confidence in port call . . . . . . 117
;       15.13   SCNRS - No resources call  . . . . . . . . . . 118
;       15.14   SCCLS - Close Completed call . . . . . . . . . 119
;       15.15   SCNLK - No link call . . . . . . . . . . . . . 120
;       15.16   SCNCM - No communication call  . . . . . . . . 121
;       15.17   SCNRN - Not in Run State call  . . . . . . . . 122
;       15.18   SCCAK - Got a Connect ACK  . . . . . . . . . . 123
;   16. Subroutines
;       16.1    CHKABO - Check SLABO flag  . . . . . . . . . . 124
;       16.2    CHKPPN - Check self PPN for validity . . . . . 125
;       16.3    FREMSG - Free a message block  . . . . . . . . 126
;       16.4    CONBUF - Invoke Conservative Buffering . . . . 127
;       16.5    FNDSLB - Find SLB given a channel number . . . 128
;       16.6    FNDSBI - Find SLB from SLBid . . . . . . . . . 129
;       16.7    FRESLB - Deallocate a SLB  . . . . . . . . . . 130
;       16.8    FRECBP - Free Connect Block  . . . . . . . . . 133
;       16.9    CDMCBP - Copy User Data from Message Block . . 134
;       16.10   CPMSCB - Copy User Data from Message Block . . 135
;       16.11   FRESJB - Deallocate a SJB  . . . . . . . . . . 136
;       16.12   MAKSLB - Create a SLB, filling in defaults . . 137
;       16.13   MAKSJB - Create a SJB, filling in defaults . . 139
;       16.14   SCSSTS/CHKSTS - Set Status & Inform PSISER . . 140
;       16.15   SCTWKQ - Queue a Link for later call to SCTPSQ 142
;       16.16   SLBMAT - Pattern match connect to passive SLBs 143
;       16.17   CDBMAT - Match Two Connect Blocks  . . . . . . 144
;       16.18   STRMAT - Pattern Matcher . . . . . . . . . . . 147
;       16.19   STPTMR - Stop the Connect Initiate Timer . . . 150
;       16.20   STRTIM - Start the Connect Initiate Timer  . . 151

***** CHANGE #4; PAGE 7, LINE 8; PAGE 7, LINE 8
;		     Table of Contents for SCLINK
;
;				  Section		      Page
;
;
;       16.21   TMRREJ - Send a Reject with Reason RSNNRO  . . 145
;       16.22   BLDCTX - Build connect message . . . . . . . . 146
;       16.23   PRSCTX - Parse a connect message . . . . . . . 150
;       16.24   CPYS2M - Copy string block to message segment  154
;       16.25   CPYM2S - Copy message data to string block . . 155
;       16.26   SCTGSS - Get segment size for a destination  . 156
;       16.27   SCTCSS - Check segment size for a destination  157
;   17. Node name/number database
;       17.1    Initialize . . . . . . . . . . . . . . . . . . 158
;       17.2    Lock/unlock database . . . . . . . . . . . . . 159
;       17.3    Add a node . . . . . . . . . . . . . . . . . . 160
;       17.4    Address to name  . . . . . . . . . . . . . . . 161
;       17.5    Name to address  . . . . . . . . . . . . . . . 162
;       17.6    Name to NO block . . . . . . . . . . . . . . . 163
;       17.7    Default area # . . . . . . . . . . . . . . . . 164
;       17.8    Hash routine . . . . . . . . . . . . . . . . . 165
;       17.9    Get a bucket . . . . . . . . . . . . . . . . . 166
;       17.10   SCTANL - Add a loopback node name  . . . . . . 167
;       17.11   SCTN2L - Convert name to loop circ . . . . . . 168
;       17.12   SCTL2N - Convert loopback circuit  . . . . . . 169
;       17.13   SCTCKL - Check for loopback node . . . . . . . 170
;   18. Network management
;       18.1    Dispatch . . . . . . . . . . . . . . . . . . . 171
;       18.2    SET/CLEAR/READ parameter . . . . . . . . . . . 172
;       18.3    Return list of entity ids  . . . . . . . . . . 173
;       18.4    Map node address to node name  . . . . . . . . 174
;       18.5    Map node name to node address  . . . . . . . . 175
;       18.6    Check loopback node name . . . . . . . . . . . 176
;       18.7    Event
;           18.7.1    Invalid message  . . . . . . . . . . . . 177
;           18.7.2    CSSE event . . . . . . . . . . . . . . . 178
;   19. Local SCTL Storage . . . . . . . . . . . . . . . . . . 179
;   20. End of SCLINK  . . . . . . . . . . . . . . . . . . . . 180
 ---------------------------------
;		     Table of Contents for SCLINK
;
;				  Section		      Page
;
;
;       16.21   TMRREJ - Send a Reject with Reason RSNNRO  . . 152
;       16.22   BLDCTX - Build connect message . . . . . . . . 153
;       16.23   PRSCTX - Parse a connect message . . . . . . . 157
;       16.24   CPYS2M - Copy string block to message segment  161
;       16.25   CPYM2S - Copy message data to string block . . 162
;       16.26   SCTGSS - Get segment size for a destination  . 163
;       16.27   SCTCSS - Check segment size for a destination  164
;   17. Node name/number database
;       17.1    Initialize . . . . . . . . . . . . . . . . . . 165
;       17.2    Lock/unlock database . . . . . . . . . . . . . 167
;       17.3    Add a node . . . . . . . . . . . . . . . . . . 168
;       17.4    Address to name  . . . . . . . . . . . . . . . 169
;       17.5    Name to address  . . . . . . . . . . . . . . . 171
;       17.6    Name to NO block . . . . . . . . . . . . . . . 172
;       17.7    Default area # . . . . . . . . . . . . . . . . 173
;       17.8    Hash routine . . . . . . . . . . . . . . . . . 174
;       17.9    Get a bucket . . . . . . . . . . . . . . . . . 175
;       17.10   SCTANL - Add a loopback node name  . . . . . . 176
;       17.11   SCTN2L - Convert name to loop circ . . . . . . 177
;       17.12   SCTL2N - Convert loopback circuit  . . . . . . 178
;       17.13   SCTCKL - Check for loopback node . . . . . . . 179
;   18. Network management
;       18.1    Dispatch . . . . . . . . . . . . . . . . . . . 180
;       18.2    SET/CLEAR/READ parameter . . . . . . . . . . . 181
;       18.3    Return list of entity ids  . . . . . . . . . . 182
;       18.4    Map node address to node name  . . . . . . . . 183
;       18.5    Map node name to node address  . . . . . . . . 184
;       18.6    Check loopback node name . . . . . . . . . . . 185
;       18.7    Event
;           18.7.1    Invalid message  . . . . . . . . . . . . 186
;           18.7.2    CSSE event . . . . . . . . . . . . . . . 187
;   19. Local SCTL Storage . . . . . . . . . . . . . . . . . . 188
;   20. End of SCLINK  . . . . . . . . . . . . . . . . . . . . 189

***** CHANGE #5; PAGE 9, LINE 9; PAGE 9, LINE 9

.chapter Conventions

.hl1 Register Definitions

Since DECnet-36 is designed to be used in both TOPS-10 and TOPS-20, it
must fail to use the standard register definitions of at least one
host operating system.  Therefore, DECnet-36 uses its own register
definitions.  The register names have been chosen to fit in as best as
is reasonable with the two target operating systems.

 ---------------------------------

.chapter Conventions

.hl1 Register Definitions

Since DECnet-36 is designed to be used in both TOPS-10 and TOPS-20, it must
fail to use the standard register definitions of at least one host operating
system.  Therefore, DECnet-36 uses its own register definitions.  The register
names have been chosen to fit in as best as is reasonable with the two target
operating systems.


***** CHANGE #6; PAGE 9, LINE 44; PAGE 9, LINE 44

These assignments are subject to change.

The DECnet-36 registers FREE1, FREE2 and FREE3 are renamed to different names
by the different DECnet-36 modules.  All DECnet-36 registers other than T1
through T6 are treated as preserved registers.  DECnet-36 does not use
register zero (FREE0).  DECnet-36 defines six temporary registers in order to
use them for EXTEND instructions such as MOVSLJ.
 ---------------------------------

These assignments are subject to change.

The DECnet-36 registers FREE1, FREE2 and FREE3 are renamed to different names
by the different DECnet-36 modules.  All DECnet-36 registers other than T1
through T6 are treated as preserved registers.  DECnet-36 does not use register
zero (FREE0).  DECnet-36 defines six temporary registers in order to use them
for EXTEND instructions such as MOVSLJ.

***** CHANGE #7; PAGE 46, LINE 5; PAGE 46, LINE 5
	SUBTTL Definitions -- Accumulators

;These are some local AC defintions for Session Control.

	SL=FREE1		;SL POINTS TO THE CURRENT SC LINK BLOCK
 ---------------------------------
	SUBTTL Definitions -- Accumulators

;These are some local AC defintions for Session Control.

	SJB=P3			;[8948] SJB points to session control job block
	SL=FREE1		;SL POINTS TO THE CURRENT SC LINK BLOCK

***** CHANGE #8; PAGE 57, LINE 35; PAGE 57, LINE 35
IFN FTOPS10,<
SCTLW1:	AOSE SCTLOK		;TEST AND SET THE INTERLOCK
	JRST SCTLW1		;ITS LOCKED, SPIN UNTIL ITS FREED
	APRID SCTLKO		;SET THE OWNER OF THE INTERLOCK
>;END IFN FTOPS10		; CAN ONLY BE COMPETING WITH ANOTHER
				; PROCESSOR ON TOPS10
 ---------------------------------
IFN FTOPS10,<
SCTLW1:	AOSE SCTLOK		;TEST AND SET THE INTERLOCK
	JRST SCTLW1		;ITS LOCKED, SPIN UNTIL ITS FREED
	APRID SCTLKO		;SET THE OWNER OF THE INTERLOCK
>;END IFN FTOPS10		; CAN ONLY BE COMPETING WITH ANOTHER
				; PROCESSOR ON TOPS10

***** CHANGE #9; PAGE 57, LINE 58; PAGE 58, LINE 23
	JRST 1(T4)		;YES, WAKE UP FORK
	XRESCD

SCTLW2:	BUG.(CHK,SCTBWK,SCLINK,SOFT,<SCTNSF call from sched without lock>,,<

Cause:	The DECnet entry point SCTNSF has been called from scheduler
	level when the Session Control interlock was locked.

Action:	All scheduler level routines which call SCTNSF should first
	check SCTLOK.  If SCTLOK is not -1, then the caller should
	wait for the next scheduler cycle before calling SCTNSF.
	Inspect the stack to find out who the offender is.

>)
 ---------------------------------
	JRST 1(T4)		;YES, WAKE UP FORK
	XRESCD

SCTLW2:	BUG.(CHK,SCTBWK,SCLINK,SOFT,<SCTNSF call from sched without lock>,,<

Cause:	The DECnet entry point SCTNSF has been called from scheduler level when
	the Session Control interlock was locked.  All scheduler level routines
	which call SCTNSF should first check SCTLOK.  If SCTLOK is not -1, then
	the caller should wait for the next scheduler cycle before calling
	SCTNSF.  Inspect the stack to find out who the offender is.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.
>)

***** CHANGE #10; PAGE 69, LINE 36; PAGE 70, LINE 36
SCTNR1:	IFSTATE T2,RJ
	  SCERR %NERBO,SCTNIE,<Rejected by Object>
	IFSTATE T2,DR
	  SCERR %NEDBO,SCTNIE,<Disconnected by Object>
SCTNR2:	SCERR %NEREJ,SCTNIE,<Unspecified Reject Reason>


 ---------------------------------
SCTNR1:	IFSTATE T2,RJ
	  SCERR %NERBO,SCTNIE,<Rejected by Object>
	IFSTATE T2,DR
	  SCERR %NEDBO,SCTNIE,<Disconnected by Object>
SCTNR2:	SCERR %NEREJ,SCTNIE,<Unspecified Reject Reason>


***** CHANGE #11; PAGE 69, LINE 44; PAGE 71, LINE 7
;Here with an error code in T1.

SCTNIE:	SKIPN T1		;IS THERE AN ERROR CODE?
	BUG.(CHK,SCLNZE,SCLINK,SOFT,<Passing zero error code to SCMUUO>,,<

Cause:	The routine that is supposed to store an error code
	for the user is zero.  This is an illegal value.

Action:	Find who called SCTNIE with T1/ 0 and correct the caller's behavior.

>)
 ---------------------------------
;Here with an error code in T1.

SCTNIE:	SKIPN T1		;IS THERE AN ERROR CODE?
	BUG.(CHK,SCLNZE,SCLINK,SOFT,<Passing zero error code to SCMUUO>,,<

Cause:	The routine that is supposed to store an error code for the user is
	zero.  This is an illegal value.  To solve this, find who called SCTNIE
	with T1/ 0 and correct the caller's behavior.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.
>)

***** CHANGE #12; PAGE 75, LINE 21; PAGE 77, LINE 21
;	.NSAA1/ Ptr to connect-block

NSFEP:	TRACE SC,<Performing Enter Passive (.NSFEP)>
	CALL MAKSLB		;MAKE A SLB
	 SCERR %NEALF,RTN,<Allocation failure>
	STOR T1,SAACH,(SA)	;STORE THE CHANNEL FOR USER
 ---------------------------------
;	.NSAA1/ Ptr to connect-block

NSFEP:	TRACE SC,<Performing Enter Passive (.NSFEP)>
	CALL MAKSLB		;MAKE A SLB
	 SCERR %NEALF,RTN,<Allocation failure>
	SKIPGE FORKX		;[8948] Must be fork context
	IFSKP.			;[8948] If so,
	  OPSTR <SKIPN >,SJMXP,(SJB) ;[8948] Does SJB have a maximum?
	  IFSKP.		;[8948] If so, check channel limit
	    OPSTR <CAMLE T1,>,SJMXP,(SJB) ;[8948] Too many channels already?
	    SCERR %NERES,NSFEPC,<Resource failure, all channels in use> ;[8948]
	  ENDIF.		;[8948]
	ENDIF.			;[8948]
	STOR T1,SAACH,(SA)	;STORE THE CHANNEL FOR USER

***** CHANGE #13; PAGE 106, LINE 36; PAGE 108, LINE 36
;This routine is called from LLINKS when it receives a message and
;there are Session Control data requests for that message.  If this
;routine returns failure, LLINKS will keep the message, unACKed, and
;will try to send it to Session Control again when the memory manager
;tells LLINKS that congestion has been relieved.

 ---------------------------------
;This routine is called from LLINKS when it receives a message and
;there are Session Control data requests for that message.  If this
;routine returns failure, LLINKS will keep the message, unACKed, and
;will try to send it to Session Control again when the memory manager
;tells LLINKS that congestion has been relieved.


***** CHANGE #14; PAGE 106, LINE 42; PAGE 109, LINE 6
	XRESCD
SCTRIB:	OPSTR <CAME T1,>,SLSLB,(T1) ;IS THIS A LEGIT SLB POINTER?
	BUG.(CHK,SCLRIB,SCLINK,SOFT,<Bad SCTRIB call from LLINKS>,<<T1,ADDR>>,<

Cause:	LLINKS has called SCTRIB for permission to send a message to SCLINK
	and has passed an invalid SLB address in T1.  The data structures
	for this logical link are inconsistent.

Action:	Find out what is in LLINK's ELSCB and why its not an SLB pointer.

 ---------------------------------
	XRESCD
SCTRIB:	OPSTR <CAME T1,>,SLSLB,(T1) ;IS THIS A LEGIT SLB POINTER?
	BUG.(CHK,SCLRIB,SCLINK,SOFT,<Bad SCTRIB call from LLINKS>,<<T1,ADDR>>,<

Cause:	LLINKS has called SCTRIB for permission to send a message to SCLINK and
	has passed an invalid SLB address in T1.  The data structures for this
	logical link are inconsistent.  Find out what is in LLINK's ELSCB and
	why its not an SLB pointer.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.


***** CHANGE #15; PAGE 128, LINE 21; PAGE 131, LINE 21
;Phase II and promise never to release this link's reservations, and
;always to use the existing reservation when reallocating.

CONBUF:	BUG.(INF,SCLCBN,SCLINK,SOFT,<Phase-II buffering not implemented>,,<

Cause:	Conservative buffering is not yet implemented.
	We should never have a logical link open to a phase II node.

>)
 ---------------------------------
;Phase II and promise never to release this link's reservations, and
;always to use the existing reservation when reallocating.

CONBUF:	BUG.(INF,SCLCBN,SCLINK,SOFT,<Phase-II buffering not implemented>,,<

Cause:	Conservative buffering is not yet implemented.  We should never have a
	logical link open to a phase II node.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.
>)

***** CHANGE #16; PAGE 129, LINE 26; PAGE 132, LINE 26
	SKIPN SL,(T4)		;GET THE POINTER TO IT
	RET			;NO THERE, GIVE BAD RETURN
	OPSTR <CAME SL,>,SLSLB,(SL) ;MAKE SURE IT POINTS TO AN SLB
	BUG.(CHK,SCLSPF,SCLINK,SOFT,<SLB self pointers messed up in FNDSLB>,<<T1,CHAN>,<T2,SJBPTR>>,<

Cause:	The DECnet data structures for this link are inconsistent.
	If this happens more than once, submit a SPR.

Data:	CHAN - The DECnet channel number
	SJBPTR - Pointer to the SJB

>,RTN)
 ---------------------------------
	SKIPN SL,(T4)		;GET THE POINTER TO IT
	RET			;NO THERE, GIVE BAD RETURN
	OPSTR <CAME SL,>,SLSLB,(SL) ;MAKE SURE IT POINTS TO AN SLB
	BUG.(CHK,SCLSPF,SCLINK,SOFT,<SLB self pointers messed up in FNDSLB>,<<T1,CHAN>,<T2,SJBPTR>>,<

Cause:	The DECnet data structures for this link are inconsistent. 

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.

Data:	CHAN - The DECnet channel number
	SJBPTR - Pointer to the SJB
>,RTN)

***** CHANGE #17; PAGE 131, LINE 17; PAGE 134, LINE 17

FRESLB:	SAVEAC <P1,P2>
	OPSTR <SKIPN P2,>,SLSJB,(SL) ;LOAD UP THE JOB BLK POINTER
	BUG.(CHK,SCLSLB,SCLINK,SOFT,<SLB bad at FRESLB>,<<SL,SLBPTR>>,<

Cause:	There is no Session Control Job Block (SJB) for this Session
	Control Link Block (SLB).  This error could have happened at
	any time during the life of the link after it was actively
	transferring data.

 ---------------------------------

FRESLB:	SAVEAC <P1,P2>
	OPSTR <SKIPN P2,>,SLSJB,(SL) ;LOAD UP THE JOB BLK POINTER
	BUG.(CHK,SCLSLB,SCLINK,SOFT,<SLB bad at FRESLB>,<<SL,SLBPTR>>,<

Cause:	There is no Session Control Job Block (SJB) for this Session Control
	Link Block (SLB).  This error could have happened at any time during
	the life of the link after it was actively transferring data.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.


***** CHANGE #18; PAGE 131, LINE 32; PAGE 134, LINE 34
	LOAD T4,SJCHT,(P2)	;GET POINTER TO SLB TABLE IN THE SJB
	ADDI T4,-1(T1)		;FIND THE ENTRY FOR THIS CHANNEL
	CAME SL,(T4)		;MAKE SURE THERE'S ONE THERE
	BUG.(CHK,SCLTFS,SCLINK,SOFT,<Tried to free wrong SLB>,<<SL,SLBPTR>>,<

Cause:	The channel table entry didn't point to the correct SLB.
	There is an internal inconsistency in the DECnet data
	structures for this link.  

Action:	Submit a SPR if this happens more than once.

 ---------------------------------
	LOAD T4,SJCHT,(P2)	;GET POINTER TO SLB TABLE IN THE SJB
	ADDI T4,-1(T1)		;FIND THE ENTRY FOR THIS CHANNEL
	CAME SL,(T4)		;MAKE SURE THERE'S ONE THERE
	BUG.(CHK,SCLTFS,SCLINK,SOFT,<Tried to free wrong SLB>,<<SL,SLBPTR>>,<

Cause:	The channel table entry didn't point to the correct SLB.  There is an
	internal inconsistency in the DECnet data structures for this link.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.


***** CHANGE #19; PAGE 131, LINE 43; PAGE 134, LINE 45
Data:	SLBPTR - Pointer to the bad SLB

>,RTN)
	SETZM (T4)		    ;ZERO CHANNEL TABLE ENTRY

 ---------------------------------
Data:	SLBPTR - Pointer to the bad SLB

>,RTN)
	SETZM (T4)		    ;ZERO CHANNEL TABLE ENTRY


***** CHANGE #20; PAGE 131, LINE 70; PAGE 135, LINE 27
	SKIPN T2		;#YES, ANY OUTPUTS?
	AOS T3			;#NO OR NO, ONE MORE WAS RESERVED FOR
	MOVNS T3		;# OTHER DIRECTION
	ADDM T3,DCNRSB		;#UPDATE RESERVED BUFFER COUNT
FRESL1:	D36ON			;#END CRITICAL SECTION

 ---------------------------------
	SKIPN T2		;#YES, ANY OUTPUTS?
	AOS T3			;#NO OR NO, ONE MORE WAS RESERVED FOR
	MOVNS T3		;# OTHER DIRECTION
	ADDM T3,DCNRSB		;#UPDATE RESERVED BUFFER COUNT
FRESL1:	D36ON			;#END CRITICAL SECTION


***** CHANGE #21; PAGE 135, LINE 25; PAGE 140, LINE 25
	JUMPLE T2,FRESJ2	;DON'T CHECK IF COUNT IS ZERO
FRESJ1:	SKIPE (T1)		;IS THERE AN ENTRY THERE?
	BUG.(CHK,SCLTFJ,SCLINK,SOFT,<Freeing SJB with SLB entries existing>,<<P1,SJBPTR>>,<

Cause:	FRESJB was called to free up a SJB.  However, there are still active
	links in use for this SJB.  This should never happen, and there is
	an internal inconsistency in the DECnet data structures.  Submit a
	SPR if this happens more than once.

 ---------------------------------
	JUMPLE T2,FRESJ2	;DON'T CHECK IF COUNT IS ZERO
FRESJ1:	SKIPE (T1)		;IS THERE AN ENTRY THERE?
	BUG.(CHK,SCLTFJ,SCLINK,SOFT,<Freeing SJB with SLB entries existing>,<<P1,SJBPTR>>,<

Cause:	FRESJB was called to free up a SJB.  However, there are still active
	links in use for this SJB.  This should never happen, and there is an
	internal inconsistency in the DECnet data structures.  Submit a SPR if
	this happens more than once.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.


***** CHANGE #22; PAGE 146, LINE 41; PAGE 151, LINE 41
	SETZ P3,		;USE P3 AS A RECURSION COUNTER
	MOVX T2,<POINT 8,(P1)>	;SET UP INDEXED BYTE POINTER TO PATTERN
	MOVX T4,<POINT 8,(P2)>	; TO SOURCE STRING ALSO
	MOVE P0,P		;SAVE THE STACK POINTER
	ADJSP P0,4		;THIS IS ZERO LEVEL

 ---------------------------------
	SETZ P3,		;USE P3 AS A RECURSION COUNTER
	MOVX T2,<POINT 8,(P1)>	;SET UP INDEXED BYTE POINTER TO PATTERN
	MOVX T4,<POINT 8,(P2)>	; TO SOURCE STRING ALSO
	MOVE P0,P		;SAVE THE STACK POINTER
	ADJSP P0,4		;THIS IS ZERO LEVEL


***** CHANGE #23; PAGE 146, LINE 76; PAGE 152, LINE 35
	CAIA			;NO
	SUBI T5,40		;YES, MAKE UPPER CASE
	XORI T5,(T6)		;COMPARE THE CHARACTERS
	TRNN T5,177		; BUT ONLY LOOK AT LOW 7 BITS
	JRST STRMT2		;IF THEY MATCH, COMPARE NEXT CHARACTERS

 ---------------------------------
	CAIA			;NO
	SUBI T5,40		;YES, MAKE UPPER CASE
	XORI T5,(T6)		;COMPARE THE CHARACTERS
	TRNN T5,177		; BUT ONLY LOOK AT LOW 7 BITS
	JRST STRMT2		;IF THEY MATCH, COMPARE NEXT CHARACTERS


***** CHANGE #24; PAGE 146, LINE 102; PAGE 153, LINE 26
	ADJSP P0,-4		;BACK TO TOP LEVEL FROM ZERO LEVEL
  IFN FTDEBUG <
	CAME P,P0		;I GET PARANOID ABOUT STACKS
	BUG.(CHK,SCLSMS,SCLINK,SOFT,<STRMAT messed up the stack pointer>,,<

Cause:	This BUG is for debugging purposes only and will not be present in
	a production monitor.

>,RTN)
 ---------------------------------
	ADJSP P0,-4		;BACK TO TOP LEVEL FROM ZERO LEVEL
  IFN FTDEBUG <
	CAME P,P0		;I GET PARANOID ABOUT STACKS
	BUG.(CHK,SCLSMS,SCLINK,SOFT,<STRMAT messed up the stack pointer>,,<

Cause:	At the end of routine STRMAP, the stack pointer should have reset to
	the value in P0.   It did not.  This BUGCHK will not be present in
	a production monitor.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.
>,RTN)

***** CHANGE #25; PAGE 162, LINE 39; PAGE 169, LINE 39
	  BUG.(CHK,SCLVAS,SCLINK,SOFT,<SCLINK - Couldn't get memory>,,<

Cause:	SCLINK called ASGVAS to assign virtual address space for the node
	name/address database. Since the requested memory is non-resident,
	this should always succeed. However, ASGVAS gave a fail return.
>,RTN,<DB%NND>)			;[7.1210] and return error

;Success return from ASGVAS, T1 has address of first location
	MOVEM T1,HSHTAB		;Save hash table address
 ---------------------------------
	  BUG.(CHK,SCLVAS,SCLINK,SOFT,<SCLINK - Couldn't get memory>,,<

Cause:	SCLINK called ASGVAS to assign virtual address space for the node
	name/address database. Since the requested memory is non-resident,
	this should always succeed. However, ASGVAS gave a fail return.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.
>,RTN,<DB%NND>)			;[7.1210] and return error

;Success return from ASGVAS, T1 has address of first location

	MOVEM T1,HSHTAB		;Save hash table address

***** CHANGE #26; PAGE 165, LINE 42; PAGE 173, LINE 42
	OPSTR <CAME P2,>,NOADR,(T2) ;Node addresses match?
	BUG.(CHK,SCLA2N,SCLINK,SOFT,<Node database inconsistent>,<<T1,NOD>>,<

Cause:	The node database SCLA2N failed an internal consistency check.

Data:	NOD - NO pointer in ADRTAB
 ---------------------------------
	OPSTR <CAME P2,>,NOADR,(T2) ;Node addresses match?
	BUG.(CHK,SCLA2N,SCLINK,SOFT,<Node database inconsistent>,<<T1,NOD>>,<

Cause:	The node database SCLA2N failed an internal consistency check.

Action:	If this bug is reproducible, set it dumpable and send in an SPR along
	with how to reproduce the problem.

Data:	NOD - NO pointer in ADRTAB

***** CHANGE #27; PAGE 165, LINE 49; PAGE 173, LINE 52

>,RTN)
  >
	LOAD T1,NONAM,(T2)	;Get node name
	RETSKP			; and return with T1 and T2

 ---------------------------------

>,RTN)
  >
	LOAD T1,NONAM,(T2)	;Get node name
	RETSKP			; and return with T1 and T2


