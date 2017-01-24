REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:04:38
File 1: SRC:<7.MONITOR>CLUDGR.MAC.1
File 2: SRC:<7.MONITOR.AP20>CLUDGR.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8972 to CLUDGR.MAC on 27-Sep-88 by RASPUZZI
;Place CLUEIN BUGINFs under CIBUGX. Heavy sigh over this SCA bug.
; Edit= 8964 to CLUDGR.MAC on 8-Sep-88 by RASPUZZI
;Add CLUDGR joining sequence to occur after CFSJYN has finished.
;Add CLUJYN routine to be called during system initialization. This routine
;will make sure that we have connected to all nodes lower than us.
; Edit= 8885 to CLUDGR.MAC on 12-Aug-88 by RASPUZZI
;Prevent CLUHST from filling up with stale entries by handling the -1 .SSPBC
;callback from SCA. Also, be CIOFF in CLUVAC.
; Edit= 8876 to CLUDGR.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation.
; Edit= 8867 to CLUDGR.MAC on 22-Jul-88 by RASPUZZI
;Make a mnemonic for 2*NFKS called MAXBUF.
; Edit= 8866 to CLUDGR.MAC on 21-Jul-88 by RASPUZZI
;Make the CLUDGR SYSAP correctly handle CLUBUF (SYSAP buffer count) and also
;make CLNEED handle little credit left situations.

***** CHANGE #2; PAGE 2, LINE 7; PAGE 2, LINE 7
	SEARCH CLUPAR,PROLOG,SCAPAR,TTYDEF

	TTITLE (CLUDGR,,< - CLuster User Data GatheRing Service>)
;	M. Raspuzzi October 87

	EXTERN HSTSIZ
	SALL
 ---------------------------------
	SEARCH CLUPAR,PROLOG,SCAPAR,TTYDEF

	TTITLE (CLUDGR,,< - CLuster User Data GatheRing Service>)
;	M. Raspuzzi October 87

	EXTERN HSTSIZ,NFKS		;[8867]
	SALL

***** CHANGE #3; PAGE 3, LINE 11; PAGE 3, LINE 11
;
;
;    1. CLUDGR Storage . . . . . . . . . . . . . . . . . . . .   5
;    2. CLUDGR Initialization
;        2.1    CLUINI . . . . . . . . . . . . . . . . . . . .   6
;    3. Connection Management
;        3.1    CLULSN (Make CLUDGR listener)  . . . . . . . .   7
;        3.2    BADSCA (Bad SCA callback)  . . . . . . . . . .   9
;        3.3    NEWCLU (New node in cluster) . . . . . . . . .  10
;        3.4    CLUVAC (Remote system left cluster)  . . . . .  12
;        3.5    GOTCLU (Message arrived for CLUDGR)  . . . . .  13
;        3.6    CONLSN (Connect to listener) . . . . . . . . .  18
;        3.7    CLWHAT (Connect response here) . . . . . . . .  20
;        3.8    OKCLU (OK to send) . . . . . . . . . . . . . .  22
;    4. Buffer Management
;        4.1    GIVBCK (Return an SCA buffer)  . . . . . . . .  23
;        4.2    CLUCRD (Credit is available) . . . . . . . . .  24
;        4.3    CLNEED (CLUDGR needs buffers)  . . . . . . . .  25
;    5. SYSAP Entry Point
;        5.1    CL.SCA . . . . . . . . . . . . . . . . . . . .  26
;    6. CLUDGR SYSAP Work Routines
;        6.1    CLWAKE (Wake up sleeping forks)  . . . . . . .  27
;        6.2    FILLIN (To fill in a chain of SCA buffers) . .  28
;    7. INFO%
;        7.1    The JSYS that does it all  . . . . . . . . . .  29
;        7.2    RESACS (Restore ACs) . . . . . . . . . . . . .  32
;        7.3    INFCIN (Get all CI nodes doing INFO%)  . . . .  35
;        7.4    LCLDVT (Local DEVST%)  . . . . . . . . . . . .  37
;        7.5    LCLSTV (Local STDEV%)  . . . . . . . . . . . .  38
;        7.6    LCLNTF (Local NTINF%)  . . . . . . . . . . . .  39
;        7.7    LCLDVC (Local DVCHR%)  . . . . . . . . . . . .  40
;        7.8    LCLXPK (Local XPEEK%)  . . . . . . . . . . . .  41
;        7.9    LCLTMN (Local TMON%) . . . . . . . . . . . . .  42
;        7.10   LCLSGT (Local SYSGT%)  . . . . . . . . . . . .  43
;        7.11   LCLSNP (Local SNOOP%)  . . . . . . . . . . . .  44
;        7.12   LCLSKD (Local SKED%) . . . . . . . . . . . . .  45
;        7.13   LCLRCR or LCLRCD (Local RCUSR% or RCDIR%)  . .  46
;        7.14   LCLMUT (Local MUTIL%)  . . . . . . . . . . . .  47
;        7.15   LCLMTO (Local MTOPR%)  . . . . . . . . . . . .  48
;        7.16   LCLLNM (Local LNMST%)  . . . . . . . . . . . .  49
;        7.17   LCLINL (Local INLNM%)  . . . . . . . . . . . .  50
;        7.18   LCLGTY (Local GTTYP%)  . . . . . . . . . . . .  51
;        7.19   LCLGTB (Local GETAB%)  . . . . . . . . . . . .  52
;        7.20   LCLDST (Local DIRST%)  . . . . . . . . . . . .  53
;        7.21   LCLCFG (Local CNFIG%)  . . . . . . . . . . . .  54
;        7.22   LCLSYS (INFO%'s SYSTAT function) . . . . . . .  55
;        7.23   LCLJOB (Get username's job numbers)  . . . . .  59
;        7.24   LCLGJI (Local GETJI%)  . . . . . . . . . . . .  61
;        7.25   LCLMSR (Local MSTR%) . . . . . . . . . . . . .  62
 ---------------------------------
;
;
;    1. CLUDGR Storage . . . . . . . . . . . . . . . . . . . .   5
;    2. CLUDGR Initialization
;        2.1    CLUINI . . . . . . . . . . . . . . . . . . . .   6
;        2.2    CLUJYN (Check all nodes for connections) . . .   7
;    3. Connection Management
;        3.1    CLULSN (Make CLUDGR listener)  . . . . . . . .   9
;        3.2    BADSCA (Bad SCA callback)  . . . . . . . . . .  11
;        3.3    NEWCLU (New node in cluster) . . . . . . . . .  12
;        3.4    CLUVAC (Remote system left cluster)  . . . . .  14
;        3.5    GOTCLU (Message arrived for CLUDGR)  . . . . .  15
;        3.6    CONLSN (Connect to listener) . . . . . . . . .  20
;        3.7    CLWHAT (Connect response here) . . . . . . . .  22
;        3.8    OKCLU (OK to send) . . . . . . . . . . . . . .  24
;    4. Buffer Management
;        4.1    GIVBCK (Return an SCA buffer)  . . . . . . . .  25
;        4.2    CLUCRD (Credit is available) . . . . . . . . .  26
;        4.3    CLNEED (CLUDGR needs buffers)  . . . . . . . .  27
;    5. SYSAP Entry Point
;        5.1    CL.SCA . . . . . . . . . . . . . . . . . . . .  28
;    6. CLUDGR SYSAP Work Routines
;        6.1    CLWAKE (Wake up sleeping forks)  . . . . . . .  29
;        6.2    FILLIN (To fill in a chain of SCA buffers) . .  30
;    7. INFO%
;        7.1    The JSYS that does it all  . . . . . . . . . .  31
;        7.2    RESACS (Restore ACs) . . . . . . . . . . . . .  34
;        7.3    INFCIN (Get all CI nodes doing INFO%)  . . . .  37
;        7.4    LCLDVT (Local DEVST%)  . . . . . . . . . . . .  39
;        7.5    LCLSTV (Local STDEV%)  . . . . . . . . . . . .  40
;        7.6    LCLNTF (Local NTINF%)  . . . . . . . . . . . .  41
;        7.7    LCLDVC (Local DVCHR%)  . . . . . . . . . . . .  42
;        7.8    LCLXPK (Local XPEEK%)  . . . . . . . . . . . .  43
;        7.9    LCLTMN (Local TMON%) . . . . . . . . . . . . .  44
;        7.10   LCLSGT (Local SYSGT%)  . . . . . . . . . . . .  45
;        7.11   LCLSNP (Local SNOOP%)  . . . . . . . . . . . .  46
;        7.12   LCLSKD (Local SKED%) . . . . . . . . . . . . .  47
;        7.13   LCLRCR or LCLRCD (Local RCUSR% or RCDIR%)  . .  48
;        7.14   LCLMUT (Local MUTIL%)  . . . . . . . . . . . .  49
;        7.15   LCLMTO (Local MTOPR%)  . . . . . . . . . . . .  50
;        7.16   LCLLNM (Local LNMST%)  . . . . . . . . . . . .  51
;        7.17   LCLINL (Local INLNM%)  . . . . . . . . . . . .  52
;        7.18   LCLGTY (Local GTTYP%)  . . . . . . . . . . . .  53
;        7.19   LCLGTB (Local GETAB%)  . . . . . . . . . . . .  54
;        7.20   LCLDST (Local DIRST%)  . . . . . . . . . . . .  55
;        7.21   LCLCFG (Local CNFIG%)  . . . . . . . . . . . .  56
;        7.22   LCLSYS (INFO%'s SYSTAT function) . . . . . . .  57
;        7.23   LCLJOB (Get username's job numbers)  . . . . .  61
;        7.24   LCLGJI (Local GETJI%)  . . . . . . . . . . . .  63

***** CHANGE #4; PAGE 5, LINE 8; PAGE 5, LINE 8
;		     Table of Contents for CLUDGR
;
;				  Section		      Page
;
;
;        7.26   LCLTIM (Local TIME% JSYS)  . . . . . . . . . .  63
;        7.27   INFSYS (Remote SYSTAT information) . . . . . .  64
;        7.28   INFCFG (Remote CNFIG%) . . . . . . . . . . . .  67
;        7.29   INFDST (Remote DIRST%) . . . . . . . . . . . .  70
;        7.30   INFGTB (Remote GETAB%) . . . . . . . . . . . .  71
;        7.31   INFGTY (Remote GTTYP%) . . . . . . . . . . . .  72
;        7.32   INFINL (Remote INLNM%) . . . . . . . . . . . .  73
;        7.33   INFLNS (Remote LNMST%) . . . . . . . . . . . .  74
;        7.34   INFJOB (Get jobs of user on remote system) . .  76
;        7.35   INFMTO (Remote MTOPR%) . . . . . . . . . . . .  77
;        7.36   INFMUT (Remote MUTIL%) . . . . . . . . . . . .  78
;        7.37   INFRCR/INFRCD (Remote RCUSR%/RCDIR%) . . . . .  80
;        7.38   INFSKD (Remote SKED%)  . . . . . . . . . . . .  82
;        7.39   INFSNP (Remote SNOOP%) . . . . . . . . . . . .  84
;        7.40   INFSGT (Remote SYSGT%) . . . . . . . . . . . .  85
;        7.41   INFTMN (Remote TMON%)  . . . . . . . . . . . .  86
;        7.42   INFXPK (Remote XPEEK%) . . . . . . . . . . . .  87
;        7.43   INFDVC (Remote DVCHR%) . . . . . . . . . . . .  89
;        7.44   INFSTV (Remote STDEV%) . . . . . . . . . . . .  90
;        7.45   INFDVT (Remote DEVST%) . . . . . . . . . . . .  92
;        7.46   INFNTF (Remote NTINF%) . . . . . . . . . . . .  93
;        7.47   INFGJI (Remote GETJI%) . . . . . . . . . . . .  95
;        7.48   INFMSR (Remote MSTR%)  . . . . . . . . . . . .  97
;        7.49   INFGTB (Remote GETAB%) . . . . . . . . . . . . 102
;        7.50   CL.ENT (Entry point to CLUDGR from user level) 103
;        7.51   CLURSM (Reassemble SCA buffers into free space 111
;        7.52   REMQ (Remove from receive queue) . . . . . . . 113
;        7.53   CLUWAT (Wait routine)  . . . . . . . . . . . . 114
;        7.54   LCLWAT (Little credit, so we wait) . . . . . . 115
;        7.55   INFALO (Remote node allowing?) . . . . . . . . 116
;        7.56   CLGOOD (All is well) . . . . . . . . . . . . . 117
;        7.57   CLFAIL (CL.ENT failed) . . . . . . . . . . . . 118
;        7.58   CLREMR (Remote error)  . . . . . . . . . . . . 119
;        7.59   DOACS (Fill in fake ACs) . . . . . . . . . . . 120
;        7.60   CHKGAL (Check to see if we are GALAXY) . . . . 121
;        7.61   CHKPID (Check for GALAXY PID)  . . . . . . . . 123
;    8. Cluster Sendall
;        8.1    CLTMSG . . . . . . . . . . . . . . . . . . . . 124
;        8.2    CHKSND (Check to see if OK to send to remote)  128
;    9. SMON% Functions
;        9.1    SETCLU (Disable CLUDGR)  . . . . . . . . . . . 129
;        9.2    CLRCLU (Enable CLUDGR) . . . . . . . . . . . . 130
;        9.3    SETTMG (Disable Cluster sendalls)  . . . . . . 131
;        9.4    CLRTMG (Enable Cluster sendalls) . . . . . . . 132
;        9.5    CLNTFY (Notify all nodes)  . . . . . . . . . . 133
;   10. End of CLUDGR  . . . . . . . . . . . . . . . . . . . . 134
 ---------------------------------
;		     Table of Contents for CLUDGR
;
;				  Section		      Page
;
;
;        7.25   LCLMSR (Local MSTR%) . . . . . . . . . . . . .  64
;        7.26   LCLTIM (Local TIME% JSYS)  . . . . . . . . . .  65
;        7.27   INFSYS (Remote SYSTAT information) . . . . . .  66
;        7.28   INFCFG (Remote CNFIG%) . . . . . . . . . . . .  69
;        7.29   INFDST (Remote DIRST%) . . . . . . . . . . . .  72
;        7.30   INFGTB (Remote GETAB%) . . . . . . . . . . . .  73
;        7.31   INFGTY (Remote GTTYP%) . . . . . . . . . . . .  74
;        7.32   INFINL (Remote INLNM%) . . . . . . . . . . . .  75
;        7.33   INFLNS (Remote LNMST%) . . . . . . . . . . . .  76
;        7.34   INFJOB (Get jobs of user on remote system) . .  78
;        7.35   INFMTO (Remote MTOPR%) . . . . . . . . . . . .  79
;        7.36   INFMUT (Remote MUTIL%) . . . . . . . . . . . .  80
;        7.37   INFRCR/INFRCD (Remote RCUSR%/RCDIR%) . . . . .  82
;        7.38   INFSKD (Remote SKED%)  . . . . . . . . . . . .  84
;        7.39   INFSNP (Remote SNOOP%) . . . . . . . . . . . .  86
;        7.40   INFSGT (Remote SYSGT%) . . . . . . . . . . . .  87
;        7.41   INFTMN (Remote TMON%)  . . . . . . . . . . . .  88
;        7.42   INFXPK (Remote XPEEK%) . . . . . . . . . . . .  89
;        7.43   INFDVC (Remote DVCHR%) . . . . . . . . . . . .  91
;        7.44   INFSTV (Remote STDEV%) . . . . . . . . . . . .  92
;        7.45   INFDVT (Remote DEVST%) . . . . . . . . . . . .  94
;        7.46   INFNTF (Remote NTINF%) . . . . . . . . . . . .  95
;        7.47   INFGJI (Remote GETJI%) . . . . . . . . . . . .  97
;        7.48   INFMSR (Remote MSTR%)  . . . . . . . . . . . .  99
;        7.49   INFGTB (Remote GETAB%) . . . . . . . . . . . . 104
;        7.50   CL.ENT (Entry point to CLUDGR from user level) 105
;        7.51   CLURSM (Reassemble SCA buffers into free space 113
;        7.52   REMQ (Remove from receive queue) . . . . . . . 115
;        7.53   CLUWAT (Wait routine)  . . . . . . . . . . . . 116
;        7.54   LCLWAT (Little credit, so we wait) . . . . . . 117
;        7.55   INFALO (Remote node allowing?) . . . . . . . . 118
;        7.56   CLGOOD (All is well) . . . . . . . . . . . . . 119
;        7.57   CLFAIL (CL.ENT failed) . . . . . . . . . . . . 120
;        7.58   CLREMR (Remote error)  . . . . . . . . . . . . 121
;        7.59   DOACS (Fill in fake ACs) . . . . . . . . . . . 122
;        7.60   CHKGAL (Check to see if we are GALAXY) . . . . 123
;        7.61   CHKPID (Check for GALAXY PID)  . . . . . . . . 125
;    8. Cluster Sendall
;        8.1    CLTMSG . . . . . . . . . . . . . . . . . . . . 126
;        8.2    CHKSND (Check to see if OK to send to remote)  130
;    9. SMON% Functions
;        9.1    SETCLU (Disable CLUDGR)  . . . . . . . . . . . 131
;        9.2    CLRCLU (Enable CLUDGR) . . . . . . . . . . . . 132
;        9.3    SETTMG (Disable Cluster sendalls)  . . . . . . 133
;        9.4    CLRTMG (Enable Cluster sendalls) . . . . . . . 134
;        9.5    CLNTFY (Notify all nodes)  . . . . . . . . . . 135
;   10. End of CLUDGR  . . . . . . . . . . . . . . . . . . . . 136

***** CHANGE #5; PAGE 7, LINE 18; PAGE 7, LINE 18
RS (CLUID,1)			;Running CLUDGR ID counter
IFN DEBUG,<			;[7.1247] Only in DEBUG
RS (PREVID)			;[7.1247] Last used CLUID number
	>			;[7.1247] End IFN DEBUG
RS (CLUBUF,1)			;Number of SCA buffers in use by CLUDGR

 ---------------------------------
RS (CLUID,1)			;Running CLUDGR ID counter
IFN DEBUG,<			;[7.1247] Only in DEBUG
RS (PREVID)			;[7.1247] Last used CLUID number
	>			;[7.1247] End IFN DEBUG
RS (CLUBUF,1)			;Number of SCA buffers in use by CLUDGR
RS (BUFMAX,1)			;[8866] Maximum buffers for SYSAP to use
MAXBUF==2*NFKS			;[8867] Maximum buffers per connect ID


***** CHANGE #6; PAGE 9, LINE 1; PAGE 9, LINE 1
Data:	ERR - Error code as returned by SCA.
>)				;Let everyone know we failed..
	RET

	SUBTTL Connection Management -- CLULSN (Make CLUDGR listener)
 ---------------------------------
Data:	ERR - Error code as returned by SCA.
>)				;Let everyone know we failed..
	RET

	SUBTTL CLUDGR Initialization -- CLUJYN (Check all nodes for connections)
;[8964]
;CLUJYN - Routine called by CHKR at system startup after CFS joining. This
;routine is called to make sure that CLUDGR has connected to all TOPS-20,
;version 7.0 nodes (or higher) in the cluster). If not, then establish
;any missing connections.
;
; Called with:
;	No arguments
;	CALL CLUJYN
;
; Returns:
;	+1 - Always, with full cluster connectivity

	XSWAPCD			;Called from fork level

CLUJYN::TRVAR <<CLUNOD,C%SBLL>> ;Temp storage table
	SAVEQ			;These are used for storage
	SETZM CLUNOD		;Zero the first location
	MOVSI T1,CLUNOD		;Get source of BLT
	HRRI T1,1+CLUNOD	;And beginning of destination
	BLT T1,C%SBLL-1+CLUNOD	;Zero out this table
	CIOFF			;Don't bother me while I check the cluster
	MOVSI Q1,-HSTSIZ	;Initialize that funky loop counter
	DO.			;Get all CLUDGR connect IDs
	  SKIPG T1,CLUHST(Q1)	;Do we have a connect ID here?
	  IFSKP.		;If so,
	    CALL <XENT SC.NOD>	;(T1/T1,T2) Get the node number for this
	    IFGE. T2		;Do we have a good node number?
	      ADDI T2,CLUNOD	;Find a spot in this table to...
	      MOVEM T1,(T2)	;...store the connect ID
	    ENDIF.
	  ENDIF.
	  AOBJN Q1,TOP.		;Do next table entry
	OD.
	MOVE Q1,MYPOR4		;Initialize counter

;Note - We only need to check all CI nodes that are lower than us. Therefore,
;our loop counter starts at our node number and counts down to 0. This will
;save some looping time.

	DO.			;Loop over all nodes
	  SOJL Q1,ENDLP.	;Step to next lowest CI node
	  SKIPN SBLIST(Q1)	;Is there a CI node here?
	  IFSKP.		;If so,
	    MOVE T2,Q1		;Put node number in a good AC
	    CALLX (MSEC1,IS7020) ;(T2/) Is this TOPS-20, 7.0?
	    IFSKP.		;If so,
	      MOVEI Q2,CLUNOD	;Use this table to check on CID
	      ADD Q2,T2		;Find node's place in the sun
	      SKIPE (Q2)	;Do we have a connect ID?
	      JRST TOP.		;Yes, do next node
	      CALL NEWCLU	;(T2/) No CID, establish a connection
	       NOP		;Ignore this. Error has been reported
	      SKIPE CIBUGX	;[8972] Wanna see this?
	      BUG.(INF,CLUEIN,CLUDGR,SOFT,<CLUDGR - Establishing initial connection>,<<Q1,NODE>>,<

Cause:	This BUG. appears when the CLUDGR joining code stumbles on a CI node
	that a CLUDGR connection has not been established for. CLUJYN then
	attempts to make a CLUDGR connection to this node. Theoretically,
	SCA should have notified CLUDGR of all nodes that are online. This
	BUG. is simply telling you that one of them has been missed by SCA
	and CLUDGR noticed it.

Action:	CLUJYN will attempt to establish a connection to the missing node.
	There is no manual intervention required.

Data:	NODE - CI node that SCA didn't tell us about
>)				;Let someone know what we are doing
	    ENDIF.
	  ENDIF.
	  JRST TOP.		;Loop over all nodes
	OD.
	CION			;Turn this back on
	RET			;And done

	SUBTTL Connection Management -- CLULSN (Make CLUDGR listener)

***** CHANGE #7; PAGE 10, LINE 2; PAGE 12, LINE 2
	  ENDIF.
	  AOBJN T1,TOP.		;Do next entry in table
	  CION			;Turn CION before going back
	ENDDO.			;End of loop
	BUG. (CHK,CLUNFE,CLUDGR,SOFT,<CLUDGR - No free entry in table>,,<

 ---------------------------------
	  ENDIF.
	  AOBJN T1,TOP.		;Do next entry in table
	  CION			;Turn CION before going back
	ENDDO.			;End of loop
	BUG.(CHK,CLUNFE,CLUDGR,SOFT,<CLUDGR - No free entry in table>,,<


***** CHANGE #8; PAGE 12, LINE 14; PAGE 14, LINE 14
; Returns:
;	+1 - Always

	XRESCD			;Called from PI level

NEWCLU:	SAVEQ			;Don't trash good things
	MOVE Q2,T2		;Put node here.
	CALLX (MSEC1,SC.PRT)	;(/T1) Get our port number
	 RETBAD()		;Failed
	CAMG T1,Q2		;Should we try to talk to this guy?
	RET			;No, he has a higher CI node than us
	CALLX (MSEC1,IS7020)	;(T2/) Do we want to talk to this node?
 ---------------------------------
; Returns:
;	+1 - Always

	XRESCD			;Called from PI level

NEWCLU:	CAML T2,MYPOR4		;[8964] Should we try to talk to this guy?
	RET			;No, he has a higher CI node than us
	SAVEQ			;Don't trash good things
	MOVE Q2,T2		;Put node here.
	CALLX (MSEC1,IS7020)	;(T2/) Do we want to talk to this node?

***** CHANGE #9; PAGE 13, LINE 5; PAGE 15, LINE 5
	CION			;Turn CI back on
	MOVE T2,Q1		;Get table index back
	BLCAL. (<XENT SC.CON>,<<.,CLUNAM>,<.,CLUNAM>,Q2,[SCRDIT],[RCRDIT],<.,CL.SCA>,T2,[CLUSET],[BUFINI],[0]>) ;Do connect
	IFNSK.
NEWCL1:	  BUG. (CHK,CLULES,CLUDGR,SOFT,<No CLUDGR connection established with remote node>,<<T1,WHY>>,<

 ---------------------------------
	CION			;Turn CI back on
	MOVE T2,Q1		;Get table index back
	BLCAL. (<XENT SC.CON>,<<.,CLUNAM>,<.,CLUNAM>,Q2,[SCRDIT],[RCRDIT],<.,CL.SCA>,T2,[CLUSET],[BUFINI],[0]>) ;Do connect
	IFNSK.
NEWCL1:	  BUG.(CHK,CLULES,CLUDGR,SOFT,<No CLUDGR connection established with remote node>,<<T1,WHY>>,<


***** CHANGE #10; PAGE 13, LINE 31; PAGE 15, LINE 31
	MOVX T1,CL%OPN		;Indicate connection becoming open...
	IORM T1,CLUSTS(Q1)	;...for this system
	MOVEI T1,BUFINI		;Note that CLUDGR just queued up buffers
	ADDM T1,CLUBUF		;By adding the count to the buffer count word
	ADDM T1,CLUSTS(Q1)	;Say how many buffers in use by this connection
	RETSKP			;Done
 ---------------------------------
	MOVX T1,CL%OPN		;Indicate connection becoming open...
	IORM T1,CLUSTS(Q1)	;...for this system
	MOVEI T1,BUFINI		;Note that CLUDGR just queued up buffers
	ADDM T1,CLUBUF		;By adding the count to the buffer count word
	ADDM T1,CLUSTS(Q1)	;Say how many buffers in use by this connection
	MOVEI T1,MAXBUF		;[8867] Get max buffers for this CID
	ADDM T1,BUFMAX		;[8866] And add to overall max count
	RETSKP			;Done

***** CHANGE #11; PAGE 14, LINE 15; PAGE 16, LINE 15
; Returns:
;	+1 - Always

	XRESCD			;Here from PI level

CLUVAC:	JUMPL T2,R		;Is this an extraneous callback? If so, return
	LOAD T3,SID,T2		;Get index into CLUHST table
	SKIPN T1,CLUHST(T3)	;Is this node active?
	IFSKP.			;If so,
	  CALL <XENT SC.NOD>	;(T1/T1,T2) Get node number
 ---------------------------------
; Returns:
;	+1 - Always

	XRESCD			;Here from PI level

CLUVAC:	CIOFF			;[8885] Turn this darn thing off
	SAVEQ			;[8885] Used here
	JUMPL T2,CLUVC2		;[8885] Smiles and giggles callback?
	LOAD T3,SID,T2		;Get index into CLUHST table
	SKIPN T1,CLUHST(T3)	;Is this node active?
	IFSKP.			;If so,
	  CAME T1,T2		;[8885] Is this the CID we want?
	  JRST CLUVC3		;[8885] No, return now
	  CALL <XENT SC.NOD>	;(T1/T1,T2) Get node number

***** CHANGE #12; PAGE 14, LINE 30; PAGE 16, LINE 34
	node on the CI-20.

Data:	NODE - Remote CI node number
	CID - Connect ID.
>,,<DB%NND>)			;[7.1210] Let someone know, not normally dumped
	  MOVE T1,CLUSTS(T3)	;[7.1273] Get status
	  TXNE T1,CL%RDY	;[7.1273] If it was ready to receive stuff
	  CALL CLWAKE		;[7.1273] (T2/) Wake up outstanding forks
	  LOAD T1,CIDBUF,(T3)	;Get number of buffers in use by this CID
	  SUBM T1,CLUBUF	;Discount them here
	ENDIF.
	SETZM CLUHST(T3)	;Indicate this entry is now gone
	SETZM CLUSTS(T3)	;No status on null entry
	RET			;And back to caller

 ---------------------------------
	node on the CI-20.

Data:	NODE - Remote CI node number
	CID - Connect ID.
>,,<DB%NND>)			;[7.1210] Let someone know, not normally dumped
CLUVC1:	  MOVE T1,CLUSTS(T3)	;[8885] Get status
	  TXNE T1,CL%RDY	;[7.1273] If it was ready to receive stuff
	  CALL CLWAKE		;[7.1273] (T2/) Wake up outstanding forks
	  LOAD T1,CIDBUF,(T3)	;Get number of buffers in use by this CID
	  MOVNS T1		;[8866] Make it negative
	  ADDM T1,CLUBUF	;[8866] Discount them from SYSAP
	  MOVNI T1,MAXBUF	;[8867] Overall maximum will now be less
	  ADDM T1,BUFMAX	;[8866] Subtract out this vanquished CID
	  SETZM CLUHST(T3)	;[8885] Indicate this entry is now gone
	  SETZM CLUSTS(T3)	;[8885] No status on null entry
	ENDIF.
CLUVC3:	CION			;[8885] Turn this back on
	RET			;And back to caller

;[8885] Here when SCA gives us the -1 callback. We must loop through
;the CLUHST table and see if the entry is still there. If it is, get
;rid of it.
CLUVC2:	SKIPL Q1,T3		;Get node number
	CAIL Q1,C%SBLL		;Is it in range?
	JRST CLUVC3		;No, ignore this callback
	MOVSI Q2,-HSTSIZ	;Get table length
	DO.			;Loop over all table entries
	  SKIPG T1,CLUHST(Q2)	;Is there something in this spot?
	  IFSKP.		;If so,
	    CALL <XENT SC.NOD>	;(T1/T1,T2) Get the node number
	    CAME T2,Q1		;Is this the one?
	    IFSKP.		;If so,
	      LOAD T3,SID,T1	;Get index into data tables
	      JRST CLUVC1	;And get rid of this entry
	    ENDIF.
	  ENDIF.
	  AOBJN Q2,TOP.		;Loop over all entries
	OD.
	JRST CLUVC3		;Couldn't find one, that's OK, return


***** CHANGE #13; PAGE 15, LINE 21; PAGE 17, LINE 21
GOTCLU:	SAVEQ			;Save some work registers
	SETZM .CLFLI(T3)	;Ensure no FLINK at all, this packet is in
				;limbo until it gets placed somewhere
	LOAD Q1,SID,T2		;Get table index
	SOS CLUSTS(Q1)		;Discount this buffer from receive credit
	MOVE Q3,T2		;Save connect ID
 ---------------------------------
GOTCLU:	SAVEQ			;Save some work registers
	SETZM .CLFLI(T3)	;Ensure no FLINK at all, this packet is in
				;limbo until it gets placed somewhere
	LOAD Q1,SID,T2		;Get table index
	SOS CLUSTS(Q1)		;Discount this buffer from receive credit
	SOS CLUBUF		;[8866] And overall SYSAP buffer useage
	MOVE Q3,T2		;Save connect ID

***** CHANGE #14; PAGE 16, LINE 49; PAGE 18, LINE 49

Cause:	The CLUDGR SYSAP received an SCA buffer from a remote system
	but it could not find a request to associate it with. Somehow,
	the request block dissappeared. This could be due to a cluster
	state transition.

Cure:	If this becomes a problem, then take a dump and try to determine
	what happened to the orphaned request.
>)				;[7.1121] Could not find request
 ---------------------------------

Cause:	The CLUDGR SYSAP received an SCA buffer from a remote system
	but it could not find a request to associate it with. Somehow,
	the request block dissappeared. This could be due to a cluster
	state transition.
>)				;[7.1121] Could not find request

***** CHANGE #15; PAGE 21, LINE 9; PAGE 23, LINE 9
	MOVX T2,CL%OPN		;Connection is open but not ready to receive
	IORM T2,CLUSTS(T1)	;Show we are open for business
	MOVEI T2,BUFINI		;We just this many buffers
	ADDM T2,CLUBUF		;Count them for CLUDGR SYSAP
	ADDM T2,CLUSTS(T1)	;Say how many buffers in use by this connection
	MOVX T2,CL%DNY		;Find out remote node's CLUDGR setting
 ---------------------------------
	MOVX T2,CL%OPN		;Connection is open but not ready to receive
	IORM T2,CLUSTS(T1)	;Show we are open for business
	MOVEI T2,BUFINI		;We just this many buffers
	ADDM T2,CLUBUF		;Count them for CLUDGR SYSAP
	ADDM T2,CLUSTS(T1)	;Say how many buffers in use by this connection
	MOVEI T2,MAXBUF		;[8867] Get maximum for this CID
	ADDM T2,BUFMAX		;[8866] And count this for overall SYSAP
	MOVX T2,CL%DNY		;Find out remote node's CLUDGR setting

***** CHANGE #16; PAGE 22, LINE 24; PAGE 24, LINE 24
	MOVE Q2,T2		;Save CID for BUGINF
	LOAD T1,SID,T2		;Get index into table
	IFE. T3			;Did remote node reject?
	  SETZM CLUHST(T1)	;Yes, must not be running CLUDGR
	  SETZM CLUSTS(T1)	;Clear this too
	  MOVEI T1,BUFINI	;Get count of buffers we initally queued
	  SUBM T1,CLUBUF	;And discount them
	  RET			;No more to do
 ---------------------------------
	MOVE Q2,T2		;Save CID for BUGINF
	LOAD T1,SID,T2		;Get index into table
	IFE. T3			;Did remote node reject?
	  SETZM CLUHST(T1)	;Yes, must not be running CLUDGR
	  SETZM CLUSTS(T1)	;Clear this too
	  MOVNI T1,BUFINI	;[8866] Get count of buffers we initally queued
	  ADDM T1,CLUBUF	;[8866] And discount them
	  MOVNI T1,MAXBUF	;[8867] Remove this much from max count
	  ADDM T1,BUFMAX	;[8866] Take it out
	  RET			;No more to do

***** CHANGE #17; PAGE 25, LINE 28; PAGE 27, LINE 28
	JRST GIVSCA		;No, node must have gone away, give buffer
				;just to SCA
	LOAD T4,CIDBUF,(T1)	;Find out how many buffers in use by this CID
	CAIG T4,BUFLOT		;If too many,
	IFSKP.			;then return this one to SCA
GIVSCA:	  SOS CLUBUF		;And now discount from overall picture
	  MOVE T1,T3		;Get buffer address
	  CALLX (MSEC1,SC.RBF)	;(T1/) Return buffer to SCA
	ELSE.			;Else, requeue this buffer
	  AOS CLUSTS(T1)	;Increment the receive credit here
	  BLCAL. (<XENT SC.RMG>,<T2,[1],T3>) ;Requeue this buffer
 ---------------------------------
	JRST GIVSCA		;No, node must have gone away, give buffer
				;just to SCA
	LOAD T4,CIDBUF,(T1)	;Find out how many buffers in use by this CID
	CAIG T4,BUFLOT		;If too many,
	IFSKP.			;then return this one to SCA
GIVSCA:	  MOVE T1,T3		;Get buffer address
	  CALLX (MSEC1,SC.RBF)	;(T1/) Return buffer to SCA
	ELSE.			;Else, requeue this buffer
	  AOS CLUSTS(T1)	;Increment the receive credit here
	  AOS CLUBUF		;[8866] Increment overall SYSAP count
	  BLCAL. (<XENT SC.RMG>,<T2,[1],T3>) ;Requeue this buffer

***** CHANGE #18; PAGE 27, LINE 15; PAGE 29, LINE 15
; Returns:
;	+1 Always, with (hopefully) BUFQUE more buffers queued up

	XRESCD			;Called by PI code

CLNEED: LOAD T1,SID,T2		;Get index
	LOAD T3,CIDBUF,(T1)	;Get buffer count for this CID
	CAIG T3,BUFMAX		;Is it less than the max?
	IFSKP.
 ---------------------------------
; Returns:
;	+1 Always, with (hopefully) BUFQUE more buffers queued up

	XRESCD			;Called by PI code

CLNEED: MOVE T3,CLUBUF		;[8866] Get SYSAP buffer count
	CAMG T3,BUFMAX		;[8866] Is it less than the max?
	IFSKP.

***** CHANGE #19; PAGE 29, LINE 14; PAGE 31, LINE 14
;	T2/ CI node that disconnected
;	CALL CLWAKE
;
; Returns:
;	+1 - Always, with each sleeping fork now ready to wake

	XRESCD			;Called at PI level

CLWAKE:	CIOFF			;Shut things off when touching the queues
	STKVAR <DISNOD>		;[7.1273] Save these
 ---------------------------------
;	T2/ CI node that disconnected
;	CALL CLWAKE
;
; Returns:
;	+1 - Always, with each sleeping fork now ready to wake
;	     Preserves the Temps

	XRESCD			;Called at PI level

CLWAKE:	CIOFF			;Shut things off when touching the queues
	SAVET			;[8866] Save these
	STKVAR <DISNOD>		;[7.1273] Save these

***** CHANGE #20; PAGE 108, LINE 16; PAGE 110, LINE 16
	ELSE.			;If stuff in user space
	  CALL SC.BR1		;(T1,T2/T1,T2,T3) Move into SCA buffers
	   RETBAD (INFX10)	;Say not enough SCA space
	ENDIF.
	MOVE Q2,T1		;Save SCA buffer chain
	ADDM T2,CLUBUF		;Count these buffers in the overall picture
	MOVEM T3,RETADR		;Return the SCA buffers by calling here
 ---------------------------------
	ELSE.			;If stuff in user space
	  CALL SC.BR1		;(T1,T2/T1,T2,T3) Move into SCA buffers
	   RETBAD (INFX10)	;Say not enough SCA space
	ENDIF.
	MOVE Q2,T1		;Save SCA buffer chain
	MOVEM T3,RETADR		;Return the SCA buffers by calling here

***** CHANGE #21; PAGE 110, LINE 28; PAGE 112, LINE 28
	  MOVE T1,Q1		;Get receive block address
	  CALL REMQ		;(T1/) Remove entry from CLRCVQ
	  CION			;Entry gone
	  DO.			;Return all SCA buffers
	    MOVE T1,Q2		;Get SCA buffer in chain
	    SOS CLUBUF		;Discount it from overall SYSAP
	    CALL @RETADR	;(T1/) And give it back to SCA
 ---------------------------------
	  MOVE T1,Q1		;Get receive block address
	  CALL REMQ		;(T1/) Remove entry from CLRCVQ
	  CION			;Entry gone
	  DO.			;Return all SCA buffers
	    MOVE T1,Q2		;Get SCA buffer in chain
	    CALL @RETADR	;(T1/) And give it back to SCA

***** CHANGE #22; PAGE 112, LINE 11; PAGE 114, LINE 11
				;Send it to remote system
	IFSKP.			;If success,
	  RETSKP		;Then done
	ENDIF.
	MOVE T1,Q2		;Get SCA buffer
	SOS CLUBUF		;Discount it from overall SYSAP
	CALL @RETADR		;(T1/) And give it back to SCA
 ---------------------------------
				;Send it to remote system
	IFSKP.			;If success,
	  RETSKP		;Then done
	ENDIF.
	MOVE T1,Q2		;Get SCA buffer
	CALL @RETADR		;(T1/) And give it back to SCA

***** CHANGE #23; PAGE 113, LINE 43; PAGE 115, LINE 43
	CALLX (MSEC1,ASGSWP)	;(T1/T1) Assign the free space
	 JRST RSMRBF		;Couldn't get free space, return SCA buffers
	MOVEM T1,FSPADR		;Save free space address
	MOVE T3,FSPADR		;Start putting stuff at this location
	SKIPN Q1,CHAIN		;Get chain back, must be at least one buffer
	BUG. (HLT,CLUNSB,CLUFRK,SOFT,<No SCA buffers for response>,,<

 ---------------------------------
	CALLX (MSEC1,ASGSWP)	;(T1/T1) Assign the free space
	 JRST RSMRBF		;Couldn't get free space, return SCA buffers
	MOVEM T1,FSPADR		;Save free space address
	MOVE T3,FSPADR		;Start putting stuff at this location
	SKIPN Q1,CHAIN		;Get chain back, must be at least one buffer
	BUG.(HLT,CLUNSB,CLUFRK,SOFT,<No SCA buffers for response>,,<


***** CHANGE #24; PAGE 124, LINE 13; PAGE 126, LINE 13

Cause:	The call to GALCHK failed because GALCHK could not get
	enough free space from the system pool to do an MUTIL%
	JSYS or the call to MUTIL% failed. Therefore, it could
	not verify whether or not this job was part of GALAXY.
	If this BUGCHK appears, software engineering should be
	notified.

 ---------------------------------

Cause:	The call to GALCHK failed because GALCHK could not get
	enough free space from the system pool to do an MUTIL%
	JSYS or the call to MUTIL% failed. Therefore, it could
	not verify whether or not this job was part of GALAXY.

Action:	If this BUGCHK appears, use the DOB% facility to take a
	dump and submit an SPR about this problem.


