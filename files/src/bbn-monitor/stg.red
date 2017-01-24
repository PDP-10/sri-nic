REDIT 1(104) COMPARE by user CLYNN, 18-Feb-86 19:49:51
File 1: CWL:<DEC-6-1-BASE>STG.MAC.1
File 2: CWL:<CMU>STG.MAC.65
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
;Merge CMU and BBN code
; Fix MNTPTP; use lvl in NTITEM macros, place some CMU changes under IFDEF CMU
;<MONITOR.CMU>STG.MAC.10, 23-Aug-85 13:11:25, Edit by VAF
;CS*** Always allocate PI6STK, not just under PI6FLG. This is due to a bug
;      in the network code that should be fixed someday.
;CS36 Add dummy ATNVTP,.CVSKT for non-PUP systems.
;CS99 Add dummy IMPFPF for non-AN20 systems.
;<MONITOR.CMU>STG.MAC.9, 21-Aug-85 17:11:42, Edit by VAF
;CS150 Compute and export NLHOST length.
;<MONITOR.CMU>STG.MAC.8, 14-Aug-85 08:14:41, Edit by VAF
;CS147 Conditionalize IP monitoring code on IPMONF.
;CS147 Add dummy MNTPTP if MNTMN==0.
;<MONITOR.CMU>STG.MAC.7,  9-Aug-85 01:06:14, Edit by VAF
; Fix merge bug - allocate PI6STK of NPISK6 in length.
;<MONITOR.CMU>STG.MAC.6, 23-Jul-85 16:06:25, Edit by VAF
;<MONITOR.CMU>STG.MAC.1, 17-Jun-85 09:36:25, Edit by VAF
;CS147 Use PROLGX
;CS147 Don't need TCPT2P.
;Remerge CMU changes with BBN TCP (based on AP#8)
;Note: CMU-NFE is now conditional on CMUN, since BBN uses NFEN.
; CS36	Define dummy PUPDTB and PATNVT if no PUP.
; CM236	TCPPFN must be global.
; Move CONOPG and CASHF to RSDAT so symbol table isn't trashed at startup

***** CHANGE #2; PAGE 2, LINE 12; PAGE 2, LINE 12
THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY ONLY BE USED
  OR COPIED IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE.

COPYRIGHT (C) 1976,1977,1978,1979 BY DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
>>				;[2849]

	SEARCH PROLOG,PHYPAR,SERCOD
	TTITLE STG
 ---------------------------------
THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY ONLY BE USED
  OR COPIED IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE.

COPYRIGHT (C) 1976,1977,1978,1979 BY DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
>>				;[2849]

IFDEF  CMU,<	SEARCH PROLGX,PHYPAR,SERCOD ;CS147 >
IFNDEF CMU,<	SEARCH PROLOG,PHYPAR,SERCOD >
	TTITLE STG

***** CHANGE #3; PAGE 2, LINE 53; PAGE 2, LINE 54
NDG	NTTD10,0		;NUMBER OF DC10 LINES
   IFG DTFLG,<NDLL==:5>		;IF A FE PRESENT, MUST HAVE 5 DLS
   IFE DTFLG,<NDLL==:0>		;IF NO FE PRESENT, NDLL=0
   IFE DCN,<NTTMCB==:0>		;IF NO DECNET NO DECNET NVTS
	NTTFE==:NDHL+NDLL	;NUMBER OF FE LINES
NDG	NTTTVT,0		;NUMBER OF TCP TELNET LINES
;NUMBER OF LINES, INCLUDING LINE 0 FOR THE CTY
NDG	NLINES,NTTFE+NTTMCB+NTTPTY+NTTD10+NTTNVT+NTTTVT+NTTDZ+1
   IFG ACTLNS-NLINES,<ACTLNS==:NLINES> ;ADJUST VALUE AS APPROPRIATE
NDG	DLSN,NTTD10		;NUMBER OF DC LINES
NDG	NTTBL,1			;DEFAULT NUMBER BUFFERS PER LINE
NDG	NTTBL1,2		;# OF BUFFERS FOR "FAST" LINES
NDG	NTTBF,ACTLNS*NTTBL1	;NUMBER OF TTY BUFFERS
 ---------------------------------
NDG	NTTD10,0		;NUMBER OF DC10 LINES
   IFG DTFLG,<NDLL==:5>		;IF A FE PRESENT, MUST HAVE 5 DLS
   IFE DTFLG,<NDLL==:0>		;IF NO FE PRESENT, NDLL=0
   IFE DCN,<NTTMCB==:0>		;IF NO DECNET NO DECNET NVTS
	NTTFE==:NDHL+NDLL	;NUMBER OF FE LINES
NDG	NTTTVT,0		;NUMBER OF TCP TELNET LINES
NDG	NTTCVT,0		;NUMBER OF CHAOS LINES
;NUMBER OF LINES, INCLUDING LINE 0 FOR THE CTY
NDG	NLINES,NTTFE+NTTMCB+NTTPTY+NTTD10+NTTNVT+NTTTVT+NTTCVT+NTTDZ+1
   IFG ACTLNS-NLINES,<ACTLNS==:NLINES> ;ADJUST VALUE AS APPROPRIATE
NDG	DLSN,NTTD10		;NUMBER OF DC LINES
NDG	NTTBL,1			;DEFAULT NUMBER BUFFERS PER LINE
NDG	NTTBL1,2		;# OF BUFFERS FOR "FAST" LINES
NDG	NTTBL2,3		;CS143 # of buffers for network lines
NDG	NTTBF,ACTLNS*NTTBL1	;NUMBER OF TTY BUFFERS

***** CHANGE #4; PAGE 2, LINE 97; PAGE 2, LINE 100
NDG	DLXN,0			;NUMBER OF UNITS ON DL10
NDG	CMAN,0			;NUMBER OF AYDIN DISPLAYS
NDG	KNIN,0			;NUMBER OF KLNI'S
NDG	FTNITEST,0		;Control whether we want NITEST loaded
NDG	IPNIN,0			;NUMBER OF NI's
IFE KNIN,<IPNIN==:0>		;IF NO NI THEN NO IP ON IT
NDG	ANXN,0			;NUMBER OF AN20S
NDG	ANX0,520		;ADDRESS FOR FIRST AN20
%NETS==:ANXN+IPNIN		;TOTAL NUMBER OF IP INTERFACES
NINTIB==:20*%NETS		;NUMBR OF IP INPUT BUFFERS TO KEEP AROUND
IFLE <^D8-%NETS>,<
PRINTX %Too many network interfaces to work with current code
>

   IFG DLXN,<
 ---------------------------------
NDG	DLXN,0			;NUMBER OF UNITS ON DL10
NDG	CMAN,0			;NUMBER OF AYDIN DISPLAYS
NDG	KNIN,0			;NUMBER OF KLNI'S
NDG	FTNITEST,0		;Control whether we want NITEST loaded
NDG	IPNIN,0			;NUMBER OF NI's
IFE KNIN,<IPNIN==:0>		;IF NO NI THEN NO IP ON IT
NDG	NETN,0			; Network code not included
				; Following are only meaningful IFN NETN
NDG	CHAOS,0			; Default to no CHAOS
NDG	ETHER,0			; Default to no Ethernet
ND	INMNPR,^D70		; Internet monitoring protocol
ND	INMNPT,^D241		; Internet monitoring port
NDG	ANXN,0			; # of DEC AN10 Imp interfaces
NDG	BBNN,0			; # of BBN IMP10 Imp interfaces
NDG	NFEN,0			; # of Front-end DTE interfaces
NDG	ALTN,0			; # of ALTO interfaces
NDG	CMUN,0			;CS99 Number of CMU NFE's
NDG	TTYN,0			;CS99 Number of TTY lines
%NETS==:ANXN+BBNN+NFEN+ALTN+CMUN+TTYN	; Total number of interfaces
				; End of IFN NETN defaults
   IFG DLXN,<

***** CHANGE #5; PAGE 3, LINE 8; PAGE 3, LINE 8
;section number definitions

NDG MSEC1,1			;section 1- code
NDG DRSECN,2			;section 2- directories
NDG IDXSEC,3			;section 3- index table
NDG BTSEC,4			;section 4- bit table
NDG INTSEC,5			;INTERNET STORAGE
NDG LCSSEC,6			;SECTION 6 - LCS MEMORY DRIVER
NDG TABSEC,7			;SECTION FOR TABLES
NDG MAXSEC,7			;max section number
	MSEC1B==:<MSEC1>B17	;DEFINE SECTION 1 PC FORMAT
 ---------------------------------
;section number definitions

NDG MSEC1,1			;section 1- code
NDG DRSECN,2			;section 2- directories
NDG IDXSEC,3			;section 3- index table
NDG BTSEC,4			;section 4- bit table
NDG MNTSEC,5			;section 5- network storage
NDG LCSSEC,6			;SECTION 6 - LCS MEMORY DRIVER
NDG TABSEC,7			;SECTION FOR TABLES
NDG DOMSEC,20			;Section 20 - Domain section 1
NDG DM2SEC,21			;Section 21 - Domain section 2
NDG MAXSEC,21			;max section number
	MSEC1B==:<MSEC1>B17	;DEFINE SECTION 1 PC FORMAT

***** CHANGE #6; PAGE 4, LINE 55; PAGE 4, LINE 55
	IFG TM78F,<LOADMODULE PHYM78>
	IFG TM02F,<LOADMODULE PHYM2>>
IFN SMFLG,<	LOADMODULE APRSSM>
IFE SMFLG,<	LOADMODULE APRSRV>
IFG DTEN,<	LOADMODULE DTESRV>
IFG DCN,<	LOADMODULE NSPSRV>
IFN ATS,<	LOADMODULE <FILATS,ATSSRV,NRMSRV>>
 ---------------------------------
	IFG TM78F,<LOADMODULE PHYM78>
	IFG TM02F,<LOADMODULE PHYM2>>
IFN SMFLG,<	LOADMODULE APRSSM>
IFE SMFLG,<	LOADMODULE APRSRV>
IFG DTEN,<	LOADMODULE DTESRV>
IFG DCN,<	LOADMODULE NSPSRV>
IFN CHAOS,<	LOADMODULE CHAOS>
IFN ATS,<	LOADMODULE <FILATS,ATSSRV,NRMSRV>>

***** CHANGE #7; PAGE 4, LINE 81; PAGE 4, LINE 82
		LOADMODULE IPIPIP
		LOADMODULE IPFREE
		LOADMODULE TCPTCP
		LOADMODULE TCPCRC
		LOADMODULE TCPJFN
		LOADMODULE TCPBBN
IFN ANXN,<	LOADMODULE IMPDV
		LOADMODULE IMPANX>
IFN IPNIN&KNIN,<
 ---------------------------------
		LOADMODULE IPIPIP
		LOADMODULE IPFREE
		LOADMODULE TCPTCP
		LOADMODULE TCPCRC
		LOADMODULE TCPJFN
		LOADMODULE TCPBBN
 IFG ANXN+BBNN,<LOADMODULE 1822DV>
 IFG ANXN,<	LOADMODULE IMPANX>
 IFG BBNN,<	LOADMODULE IMPBBN>
 IFG NFEN,<	LOADMODULE NFEPHY>
 IFG CMUN,<	LOADMODULE NFESRV> ;CS99
 IFDEF CMU,<	LOADMODULE GTDNM ;CS148 >
IFN IPNIN&KNIN,<

***** CHANGE #8; PAGE 5, LINE 18; PAGE 5, LINE 18
   IFG DLXN,<
RS(DLBASE,PGSIZ*DLIBFP)	 ;RESIDENT PAGES FOR DL10
RS(DLOBUF,PGSIZ*DLOBFP)
   >;IFG DLXN
IFN SMFLG,<RS(LPBUF,LPTN*PGSIZ)>	;LINEPRINTER BUFFER SPACE
IFN KDPN,<RS(KMCPAG,KDPN*PGSIZ)>	;A PAGE FOR EACH KDP(KMC11/DUP11)
IFN NETN,<RS(INTIX,PGSIZ)>	;INTERNET BUFFER SECTION INDEX PAGE
IFE NETN,<INTIX==:0>
RS (BLKTRN,PGSIZ)		;LCS MEMORY DRIVER BLOCK TRANSFER PAGE
 ---------------------------------
   IFG DLXN,<
RS(DLBASE,PGSIZ*DLIBFP)	 ;RESIDENT PAGES FOR DL10
RS(DLOBUF,PGSIZ*DLOBFP)
   >;IFG DLXN
IFN SMFLG,<RS(LPBUF,LPTN*PGSIZ)>	;LINEPRINTER BUFFER SPACE
IFN KDPN,<RS(KMCPAG,KDPN*PGSIZ)>	;A PAGE FOR EACH KDP(KMC11/DUP11)
IFN NETN,<RS(MNTIX,PGSIZ)	;Multinet section (5) index page
	  RS(DOMIDX,PGSIZ)>	;Domain page for scheduler access
IFE NETN,<MNTIX==:0
	  DOMIDX==:0>
RS (BLKTRN,PGSIZ)		;LCS MEMORY DRIVER BLOCK TRANSFER PAGE

***** CHANGE #9; PAGE 12, LINE 35; PAGE 12, LINE 35
RSI DCHKSW,<0>			;STOP ON BUGCHKS
RSI EDDTF,<IEDDTF>		;EDDT PRESENT IF NON-0
RSI WEFLAG,<0>			;WRITE-ENABLE MON IF NON-0 AND DBUGSW=1
RSI DINFSW,<0>			;STOP ON BUGINFS
RSI DBUGIP,<0>			;IP DEBUGING

RSI BUGHLT,<0,JRST BUGH0>	;JSR BLOCK FOR BUGHLT'S
RS SVVEXM,1			;SAVE VALID EXAMINE IN BUGTYO
				;USED IN KS10 TO SAVE KEEPALIVE WORD
RSI BUGLCK,<-1>			;LOCK ON BUGXXX ROUTINES
RSI BUGCHK,<0,JRST BUGC0>	;JSR BLOCK FOR BUGCHK'S
RSI BUGINF,<0,JRST BUGI0>	;JSR BLOCK FOR BUGINF'S
 ---------------------------------
RSI DCHKSW,<0>			;STOP ON BUGCHKS
RSI EDDTF,<IEDDTF>		;EDDT PRESENT IF NON-0
RSI WEFLAG,<0>			;WRITE-ENABLE MON IF NON-0 AND DBUGSW=1
RSI DINFSW,<0>			;STOP ON BUGINFS
RSI DBUGIP,<0>			;IP DEBUGING

;RSI BUGHLT,<0,JRST BUGH0>	;JSR BLOCK FOR BUGHLT'S
IFE NETN,<RSI BUGHLT,<0,JRST BUGH0>>	;JSR BLOCK FOR BUGHLT'S
IFN NETN,<RSI BUGHLT,<0,MNTSTP,JRST BUGH0>> ;JSR BLOCK FOR BUGHLT'S
RS SVVEXM,1			;SAVE VALID EXAMINE IN BUGTYO
				;USED IN KS10 TO SAVE KEEPALIVE WORD
RSI BUGLCK,<-1>			;LOCK ON BUGXXX ROUTINES
;RSI BUGCHK,<0,JRST BUGC0>	;JSR BLOCK FOR BUGCHK'S
IFE NETN,<RSI BUGCHK,<0,JRST BUGC0>>	;JSR BLOCK FOR BUGCHK'S
IFN NETN,<RSI BUGCHK,<0,SKIPE DCHKSW,SKIPN DDTPRS,JRST BUGC0,MNTSTP,JRST BUGC0>>
RSI BUGINF,<0,JRST BUGI0>	;JSR BLOCK FOR BUGINF'S

***** CHANGE #10; PAGE 22, LINE 26; PAGE 22, LINE 26
NDG	DGOMTA,0!GT%DEF		;NO ACCESS FOR MT'S
NDG	DGOACC,0!GT%DEF		;NO ACCESS
NDG	DGOOAD,SF%DOK!GT%DEF	;OPEN ASSIGNABLE DEVICE
NDG	DGODNA,SF%DOK!GT%DEF	;DECNET ACCESS
NDG	DGOANA,SF%DOK!GT%DEF	;ARPANET ACCESS
NDG	DGOATJ,SF%DOK!GT%DEF	;ATTACH

 ---------------------------------
NDG	DGOMTA,0!GT%DEF		;NO ACCESS FOR MT'S
NDG	DGOACC,0!GT%DEF		;NO ACCESS
NDG	DGOOAD,SF%DOK!GT%DEF	;OPEN ASSIGNABLE DEVICE
NDG	DGODNA,SF%DOK!GT%DEF	;DECNET ACCESS
NDG	DGOANA,SF%DOK!GT%DEF	;ARPANET ACCESS
NDG	DGOATJ,SF%DOK!GT%DEF	;ATTACH
NDG	DGOAIQ,SF%DOK!GT%DEF	;ASSIGN INTERNET QUEUE


***** CHANGE #11; PAGE 22, LINE 57; PAGE 22, LINE 58
	DGOMTA		;MT ACCESS
	DGOACC		;ENTRY FOR ACESS JSYS
	DGOOAD		;ASSIGN DEVICE VIA OPENF
	DGODNA		;DECNET ACCESS
	DGOANA		;ARPANET ACCESS
	DGOATJ		;ATTACH
MXGOKF==:.-GTOKPR	;SIZE OF TABLE
 ---------------------------------
	DGOMTA		;MT ACCESS
	DGOACC		;ENTRY FOR ACESS JSYS
	DGOOAD		;ASSIGN DEVICE VIA OPENF
	DGODNA		;DECNET ACCESS
	DGOANA		;ARPANET ACCESS
	DGOATJ		;ATTACH
	DGOAIQ		;ASSIGN INTERNET QUEUE
MXGOKF==:.-GTOKPR	;SIZE OF TABLE

***** CHANGE #12; PAGE 29, LINE 127; PAGE 29, LINE 127
	MXSECN==:37		;HIGHEST POSSIBLE SECTION NUMBER

EPTMHI=:KIEPT+200		;MAP FOR HIGH EXEC (400-777)
EPTTPI=:KIEPT+420		;TRAP INSTRUCTIONS
EPTMLO=:KIEPT+600		;MAP FOR LOW EXEC (0-337)
KIEMP==:KIEPT			;BASE FOR MONITOR PAGES 400-777
RS CONOPG,1			;WORD FOR CONO PAG,@CONOPG

 ---------------------------------
	MXSECN==:37		;HIGHEST POSSIBLE SECTION NUMBER

EPTMHI=:KIEPT+200		;MAP FOR HIGH EXEC (400-777)
EPTTPI=:KIEPT+420		;TRAP INSTRUCTIONS
EPTMLO=:KIEPT+600		;MAP FOR LOW EXEC (0-337)
KIEMP==:KIEPT			;BASE FOR MONITOR PAGES 400-777
RSI CONOPG,0			;WORD FOR CONO PAG,@CONOPG


***** CHANGE #13; PAGE 33, LINE 1; PAGE 33, LINE 1
RS(DISBEG)			;LH=PROTECTION AND RELOCATION FOR DISPLAY
				;RH=STARTING ADDRESS OF DISPLAY PROGRAM

RS(VBCIOS)			;STATUS WORD
   >				;END IFG VBCN

IFN NETN,<

NDG NHOSTS,^D4001		;LENGTH OF HOSTN TABLE (MUST BE PRIME)
NDG NHSTN,<NHOSTS*4>		;LENGTH OF HOST NAME TABLE (TEXT)
IMPLBT==:10			;LENGTH OF OLD BIT TABLE
NHSTST==:200			;LENGTH OF OLD HOST STATUS TABLE
NDG NETHSZ,^D53			;SIZE OF NETWORK HASH TABLES

RS IGDMSG,1			;Last IMP going down message
RS IGDTIM,1			;Time of last IMP going down message
RS IMINFB,1			;BUFFERS MADE FREE BY PI ROUTINES
RS IMPGDM,1			;LAST IMP GOING DOWN MSG
RS SIQNXT,1			;TIME NEXT SPECIAL QUEUE MESSAGE EXPIRES
RS IBPTIM,1			;TIME OF NEXT IMPIBP RUN
RS BF18BI,1			;1822 BUFFER QUEUE INPUT
RS BF18BO,1			;1822 BUFFER QUEUE OUTPUT
NR(MHOSTS,1)			;MINUS NUMBER OF HOSTS IN TABLE.
RSI BBNOK,<1>			;BBN CONNECTIONS ALLOWED
RSI DECOK,<1>			;DEC CONNECTIONS ALLOWED

;Some of the following tables are kept in INTSEC

..OFST==1000			;STORAGE AFTER FIRST PAGE
				;THE FOLLOWING ARE SWAPPABLE
NDG BF18SZ,10000		;WORDS OF 1822 BUFFERS
DEFWRD(BF1822,BF18SZ)		;1822 BUFFER AREA
DEFWRD(HOSTN,NHOSTS)		;TABLE OF HOST FLAGS,,POINTERS TO NAMES.
DEFWRD(HOSTPN,NHOSTS)		;PRIMARY NAME FOR THIS HOST
DEFWRD(HSTNAM,NHSTN)		;TEXT TABLE OF HOST NAMES

BMNTLK==:..OFST			;FIRST LOCATION TO LOCK DOWN
DEFWRD(HOSTNN,NHOSTS)		;HOST NUMBERS
DEFWRD HSTSTS,NHOSTS		;DEAD HOST STATUS TABLE (18 BITS PER HOST)
EMNTLK==:..OFST			;LAST LOCATION TO LOCK DOWN

;The following parallel tables are used for looking up network numbers

RS(NETHTB,NETHSZ)		;Network number hash table
RS(NETGWY,NETHSZ)		;Path (interface of gateway) to a network

;These are part of the NETRDY GETAB Table

RS NETENT,1			;Flags to drive NETSER etc.

;Storage for Multinet device drivers and NCTs

	RESDT
	BLOCK	5		;**** TEMP *** trying to catch what's
				;clobbering NCT0
.NCTS::
	MKNCTS			;Make the actual NCTS

				;NCT Vector table
NCTVT::	MKPTRS			;And the pointers thereto

       				;Interrupt storage
RS NUMNCP,1			;Number of NCP Type nets up
RSI NETSUP,<0>			;Flag (-1) that MNTINI has completed

	RESDT

MAXWPM::EXP 400			;Exactly holds worst case 32 bit msg
RSI ANT3EN,<0>			;Type 3 messages 0 no, -1 enabled
NR BADPCT,1			;Packets received with bad checksum, etc
NR GENCFL,1			;General lock conflits counter
NR GWTAB,1			;(Extended) pointer to gateway table
RS INETID			;Our Internet ID
RS INTABC,NTACWD		;Per fork count of ABORTed TCBs
RS INTBFF,NTWBWD		;Free wait bit indicators
				;bypass is off for now.  will be on soon.
RSI INTBYP,-1			;Internet bypass 0 no, -1 ok
RSI INTCLS,<4B6,4B6,4B6,4B6,6B6,6B6,7B6,7B6> ;Network class mask
RS INTFLG,1			;Flag to make the Internet fork run
RS INTFRI,1			;Free input buffer list pointer
RS INTFRK,1			;FORKX Running Internet
RSI INTHST,<77777777,77777777,77777777,77777777,177777,177777,377,0>
RS INTIBI,1			;IMPPHY to Gateway input queue input pointer
RS INTIBO,1			;IMPPHY to Gateway input queue output pointer
RSI INTLHX,<3>			;Maximum logical host value delivered to protocols
RSI INTNET,<37700000000,37700000000,37700000000,37700000000,37777600000,37777600000,37777777400,37777777777>
RS INTNFB,1			;Empty output buffer list pointer
RS INTNFI,1			;Free input buffer count
RS INTNIB,1			;# of free input buffers desired (default 4*%NETS)
RS INTON,1			;-1 If IP initialized, 0 if not
NR INTRAN,1			;IP reassembly packet number
NR INTRAQ,1			;(Extended) pointer to IP fragment ra queue (PKTQ)
NR INTRAT,1			;TODLCK of next fragment to timeout (RCVFLS)
NR INTSCR,1			;Set non-0 to run in secure mode
				;exclusive of local leaders (max PIPL)
NR INTSVC,1			;Count of scavenges of free storage
NR INTSVR,1			;Flag to request a scavenge
NR INTSVT,1			;TODCLK of most recent scavenge BUGINF
RS INTTIM,1			;TODCLK when Internet fork should run next
RS INTTRC,1			;Set to 0 to turn OFF packet trace
;..MXSZ==^D1004
..MXSZ==^D972
RSI INTXPB,<..MXSZ>		;Maximum PIPL for all interfaces (bytes)
RSI INTXPW,<PKTELI+<<..MXSZ+3>/4>+1> ;# Words in max pkt bfr (+1) (PFSIZ=1)

IFN IPPDSW,<   			;Packet Printer
NR PPBLCK,LOCKSZ		;Lock on PPB
NR PPBUF,PPBWDS			;Packet printer buffer
TCPPFN:	ASCIZ /PS:<OPERATOR>IPTRAC.BIN/ ;P777777
BLOCK 3
NR TCPPSJ,1			;Jfn of Simulation file (containing PKTs)
NR TCPPSQ,1			;Queue of PKTs read so far (maybe RXed)
RSI TCPPSA,<BYTE (4)0(8)^D10,0,0,^D79> ;DEC-2136
RSI TCPPTB,<INTSEC,,600>	;Defaults if not CALL TCPPIN
RSI TCPPTC,<INTSEC,,600>	;Current buffer pointer
RSI TCPPTE,<INTSEC,,730>	;Sav Dump <N,,garb>,N<data>
RSI TCPPTO,<0>			;IMP Dump <N>,<time>,N<data>
				;IP  Dump <adr,,N+1>,N<data>
>				;end of IFN IPPDSW

	SWAPCD

				;Internet Free Storage
NR BULKST,1			;START OF BULK STORAGE
NR BULKND,1			;END OF BULK STORAGE
NR FRELCK,LOCKSZ		;LOCK ON THE FREE STORAGE AREA
NR INTBLK,NFIXED
INTFRE=:EMNTLK+1		;FIRST INTERNET FREE SPACE LOCATION
INTFSZ==:777777-<EMNTLK+1> 	;USE AS MUCH OF THIS SECTION AS POSSIBLE
NR INTFSP,1			;AMOUNT OF FREE SPACE CURRENTLY AVAILABLE
NR MRGFLG,1			;NON-0 IF A GARBAGE COLLECT MIGHT HELP
NR ODDBLK,1			;ODD LENGTH BLOCKS LIST

;Internet Protocol Tables
; *** Do not separate

	RESDT

DEFINE PCLTAB (NAM,PRT)<
.X=.
	RELOC	.X+.INTPC
	MSEC1,,NAM'CHK
	RELOC	.X+.INTPF
NAM'FLG::0
	RELOC	.X+.INTPI
	MSEC1,,NAM'INI
	RELOC	.X+.INTPQ
NAM'IPQ::0
	RELOC	.X+.INTPO
NAM'ON::	0
	RELOC	.X+.INTPL
IFB  <PRT>,<NAM'PCL::	.'NAM'FM>
IFNB <PRT>,<NAM'PCL::	PRT>
	RELOC	.X+.INTPP
	MSEC1,,NAM'PRC
	RELOC	.X+.INTPS
NAM'SID::0
	RELOC	.X+.INTPT
NAM'TIM::377777777777
	RELOC	.X+.INTPE
	MSEC1,,NAM'ICM
	RELOC	.X+.INTPZ
PURGE .X
>

INTPIX::-4,,.INTPZ	;-# protocols,,Table length
			;Tables follow INTPIX
	PCLTAB (TCP)	;TCP table
	PCLTAB (ICM)	;ICMP table

;Internet User Queue table
;NOTE:	This must be last since protocol -1 takes anything

	PCLTAB (INQ,-1)
	BLOCK .INTPZ	;Spare for tests
	SWAPCD

;*** End of do not separate

       			;Internet queue storage:
RSI INTQMX,<^D8>	;Max # of packets in RCVIN queue (INTSQP)
RSI INTQT0,<^D30000>	;Internet user queue (packet) timeout, msec (INTQTM)
NR INTQLK,LOCKSZ	;Lock on following queue tables
RS INTQFK,NIQ		;Forkx waiting for stuff on this queue (or -1)
			;in LH,,RH is FORKX of owner
NR INTQHD,1		;Pointer to INTSEC table of NIQ queue heads
NR INTQJB,NIQ		;Job owning this queue (Flags,,JOBN)
NR INTQM0,NIQ		;User queue mask for PIPRO (B24-31)
NR INTQM1,NIQ		;User queue mask for PIDH  (B00-31)
NR INTQM2,NIQ		;User queue mask for PISH  (B00-31)
NR INTQM3,NIQ		;User queue mask for for ports (l B0-15, f B16-31)
NR INTQSP,NIQ		;Number of messages on this queue
NR INTQTM,NIQ		;Timeout for this queue
NR INTQV0,NIQ		;User queue value for PIPRO (B24-31)
NR INTQV1,NIQ		;User queue value for PIDH  (B00-31)
NR INTQV2,NIQ		;User queue value for PISH  (B00-31)
NR INTQV3,NIQ		;User queue value for ports (l B0-15, f B16-31)
			;Additional ICMP storage
RSI ICMTM0,<^D120000>	;ICMP check time, msec
RSI NETHT0,<^D<30*60*1000>> ;Interval to clear hash table, msec
RS NETHTM		;Time to re-init the network cache
RSI PINGT0,<^D37000>	;Ping interval, msec (prime to reduce interactions)
RS PINGTM		;Time of next gateway ping

	RESDT
FKABCP::POINT ABTCBS,TCPABC	;Pointer to base of TCP abort counters
	SWAPCD

RS INTWTB,NTWBWD	;Scheduler wait bits. One per lock, buffer, etc
NR NXTLBL,1		;Next packet label
RS STATF,1		;Set non-0 to take statistics
NR TCBCNT,1		;Count of TCBs now in use
NR TCBDQ,1		;Head of dead TCB queue (SCAN:)
NR TCBH,1		;TCB Hash table location
NR TCBHLK,LOCKSZ	;TCBH Lock
RS TCBHUC,1		;TCBH Use Count
   TCBMAX::MAXTCB	;Maximum number of TCBs allowed
RS TCPABC,NTACWD	;Abort counters
RS TCPBDQ,1		;Buffer done queue
   TCPBGT::^D30000	;BG time interval, msec
   TCPBYS::4400+17*4+17*4 ;Max local (bypass) packet size - 1 page + overhead
   TCPCKT::^D10000	;TCP check interval, msec
   TCPDGT::^D30000	;DG time interval, msec
   TCPDTL::^D60		;Default Time to live, seconds
   TCPDXD::^D2		;Default RX denominator
   TCPDXI::^D3		;Default RX interval, seconds
   TCPDXN::^D3		;Default RX numerator
RS TCPIDQ,1		;Head of IP's dead queue
RS TCPIFG,1		;Set non-0 after TCP initialized
   TCPISN::^D<<MAXSEQ/8>*<<24*60*60>/CYCTIM>> ;TCP sequence # per second
           EXP 3	;Scale factor (8 above) to prevent MUL overflow
   TCPPTM::^D300	;Maximum persistence time, seconds (send timeout)
   TCPRA0::^D100	;Msec to wait for RA to process input packet
   TCPRX0::^D3000	;Initial RX interval, msec
   TCPRXF::0,,-4	;Scale factor for TCPRXS & TCPRXV (ASH x,@TCPRXF)
   TCPRXN::^D1000	;Minimum RX interval, msec
   TCPRXS::15		;0.1101 = 0.8125
   TCPRXV::30		;1.1000 = 1.5
   TCPRXW::^D120000	;RX probe into zero window, msec
   TCPRXX::^D60000	;Maximum RX interval, msec
   TCPSY0::^D2000	;Msec delay on re-synchronization attempts
RS TCPUS0,1		;Saved TCP fork UPT pointer
NR PROC,1		;Points to currently running "process" in TCP
			;"Process" control blocks
NR BG,PROCSZ		;Background
NR DY,PROCSZ		;Delayed Actions Generator
NR IP,PROCSZ		;InputProcessor
NR PZ,PROCSZ		;Packetizer
NR RA,PROCSZ		;Reassembler
NR RX,PROCSZ		;Retransmitter

       			;TCP virtual terminals
RS TVMSTM,1		;TVT sendall timer
RS TVTLSN,1		;TVT listening JCN
RS TVTNOF,1		;TCP output scan needed
RS TVTNTM,1		;Time to check for over TVT negotiations
   TVTWTM::^D60000	;Msec to wait when window filled

			;Statistics area
NR STAT0,0		;Marks first cell cleared by STSINI
NR BYTRCT,1		;Bytes received counter
NR BYTSCT,1		;Bytes sent counter
NR FINRCT,1		;FINs received counter
NR FINSCT,1		;FINs sent counter
NR RSTRCT,1		;RSTs received counter
NR RSTSCT,1		;RSTs sent counter
NR SYNRCT,1		;SYNs received counter
NR SYNSCT,1		;SYNs counter
NR DUPKCT,1		;Count of duplicates received
NR IPPKCT,1		;Packets handled by InputProcessor
NR OPPKCT,1		;Packets output to the network
NR PZPKCT,1		;Packets produced by Packtizer
NR RAPKCT,1		;Packets processed by Reassembler
NR RXPKCT,1		;Packets retransmitted

       			;Task counters
NR BGRNCT,1		;Count of times Background has run
NR DGRNCT,1		;Count of times Delay Action has run
NR IPRNCT,1		;Count of times InputProcessor has run
NR OPRNCT,1		;Count of times OutputProcessor has run
NR PZRNCT,1		;Count of times Packetizer has run
NR RARNCT,1		;Count of times Reassembler has run
NR RXRNCT,1		;Count of times Retransmitter has run
NR TASKCT,1		;Count of all tasks 


IFN IPPDSW,<
NR ACDLAY,HISTSZ	; Delay histograms
NR IPDLAY,HISTSZ
NR OPDLAY,HISTSZ
NR PZDLAY,HISTSZ
NR RADLAY,HISTSZ
NR RXDLAY,HISTSZ
>


NR BGUSE,1		; CPU Usage meters
NR DGUSE,1
NR IPUSE,1
NR OHUSE,1
NR OPUSE,1
NR PZUSE,1
NR RAUSE,1
NR RXUSE,1
NR STATZZ,0		; The last cell cleared by STSINI is ...
NR TIMPTR,1		; Current timer.  Points to one of the above



IFN ANXN,<			;Only if we have an AN20

RESDT

NDG NSQ,10			;Number of special imp queues to handle
IMP8XS==:100			;Size of irreg message buffer
NIMSTK==:24			;Size of PI level stack

RS IMSTK,NIMSTK			;Stack storage area
IMPPDP:: IOWD NIMSTK,IMSTK 	;Stack pointer for AN20 interrupts
RS SIQIBI,NSQ			;Special queues
RS SIQIBO,NSQ
RS SIQTIM,NSQ			;Time of first message on special queue
RS SIQSPC,NSQ			;Space occupied by special input q
RS SQJOB,NSQ			;Job to which special Q is assigned
RS SQVAL1,NSQ			;Values for up to 3 words of leader
RS SQVAL2,NSQ
RS SQVAL3,NSQ
RS SQMSK1,NSQ			;Masks for up to 3 words of leader
RS SQMSK2,NSQ
RS SQMSK3,NSQ
RS IIMBUF,3			;Imp irreg msg buffer, for output side
RS IMPTIM,1			;Time of next clock run down
RS IMP8XI,1			;Irreg mes buffer in ptr
RS IMP8XO,1			;.. .. out
RS IMP8XC,1			;.. .. count
RS IMP8XB,3*IMP8XS		;Irreg mes buffer
NIMPIB==:4			;Number of 1822 buffers to keep around
RSI IMPNIB,<NIMPIB*ANXN>	;Number of input buffers to keep on tap
RS IMPNFI,1			;Count of free input buffers
RS SQLCK,1			;Interlock for assigning special Q
RS IMPIBI,1			;In buffers in ptr
RS IMPIBO,1			;In buffers out ptr
RS NOIBFS,1			;Flag that input buffer queue ran dry
RS IMPFRI,1			;List of free input buffers

IFN IPPDSW,<
DBGNBF==:100			;DBGIM JSYS Storage
RS(DBGFAC,1)			;Failure counter
RS(DBGSP,1)			;Store pointer
RS(DBGRP,1)			;Read pointer
RS(DBGNWD,1)			;Number of words in buffer
RS(DBGERR,1)			;Report 1822 errors
RS(DBGNCP,1)			;Report 1822 traffic
RS(DBGINT,1)			;Report IP traffic
NR(DBGBUF,DBGNBF)		;Debug buffer
>				;End of IFN IPPDSW
>				;End of IFN ANXN



IFE ANXN,<			;Only if we don't have an AN20

	RESCD

SIQCHK::			;Dummy siqchk for when no AN20
	MOVX T1,<377777,,-1>	;A long time from now
IMINRB::			;Dummies for when no AN20
IMPIN0::
IMICHK::
IMPSTS::
NETLGO::
IMPFPF::
CHKI7::
	RET

.SNDIM::
.RCVIM::
.ASNSQ::
.RELSQ::
	JRST UJSYS0
>				;End of IFE ANXN

 ---------------------------------
RS(DISBEG)			;LH=PROTECTION AND RELOCATION FOR DISPLAY
				;RH=STARTING ADDRESS OF DISPLAY PROGRAM

RS(VBCIOS)			;STATUS WORD
   >				;END IFG VBCN
	SUBTTL	Network

IFN NETN,<

	RESDT			; Into RSDAT Psect

; The following tables are kept in MNTSEC

..MNTA==<MNTSEC,,1000>		; Start at first page in MNTSEC
DEFHDR(<Multinet Storage Section>,..MNTA)

NDG ETHRTS,^D100		; Size of ethernet address hash table
DEFHDR(<ETHERNET-HASH-TABLE>,..MNTA)
DEFWRD(ETHRTB,4*ETHRTS)

..MNTA==<..OFST+777>&37777000	; Beginning of next page

NDG NHOSTS,^D4001		;LENGTH OF HOSTN TABLE (MUST BE PRIME)
NDG NHSTN,<NHOSTS*4>		;LENGTH OF HOST NAME TABLE (TEXT)
NDG NHSTNL,<NHOSTS*2>		;LENGTH OF HOST ADDRESSES TABLE, ENTRIES

DEFHDR(<HOST-TABLES>,..MNTA)
DEFWRD(HOSTNN,NHOSTS)		; Host addresses
DEFWRD(HSTSTS,NHOSTS)		; Dead host status table
DEFWRD(HOSTPN,NHOSTS)		; Primary name for this address
DEFWRD(HOSTN,NHSTNL*HOSTNW)	; Table of host Flags address into HSTNAM,L
DEFWRD(HSTNAM,NHSTN)		; Text table of host names
DEFWRD(HSTNML,NHSTNL)		; Lists of addresses for each name

..MNTA==<..OFST+777>&37777000	; Page align

	SUBTTL	Network General Buffer Information

; Still IFN NETN, in RSDAT Psect

; General buffer allocation, here so sites may modifiy it, if needed.

ND MNTBSZ,400000			; Default to 400 pages
MNTBSZ==MNTBSZ&37777000			; Integral number of pages
IF2 <IFL MNTBSZ-20,<PRINTX % Warning - low network buffer allocation>>

DEFHDR(<Network Buffers>,..MNTA)
DEFWRD(BUFFRE,MNTBSZ)			; Define network buffers, low address
DEFWRD(BUFFRZ,0)			; Network buffers high address

NBITM==<<BUFFRZ-BUFFRE>/PGSIZ>		; Number of pages in bitmap
BITMSZ==<<NBITM+^D35>/^D36>		; Number of words in bitmap
NBITXB==NBITM-<<BITMSZ-1>*^D36>		; Number of bits in last word

	XWD -BITMSZ,0			; AOBJN pointer
PULMAP::
REPEAT <BITMSZ-1>,<	EXP -1>
			EXP -1_<^D36-NBITXB>

; Base address of storage

PULBAS:: EXP BUFFRE		; (section is 0 on a KA)
PULTOP:: EXP BUFFRZ-1	 	; Top of free storage


; The following parallel tables are used for looking up internet routes

NDG NETHSZ,^D53			;SIZE OF NETWORK HASH TABLES

RS(NETHTB,NETHSZ*NETHES)	; Destination Network number/Host address table
RS(NETIFC,NETHSZ*NETHES)	; NCT address for first-hop
RS(NETGWY,NETHSZ*NETHES)	; First-hop address (interface or gateway)
RS(NETWHO,NETHSZ*NETHES)	; Who reported most recent status
RS(NETWHN,NETHSZ*NETHES)	; HS%WHN/HS%CRD format TODCLK reported,
				; reporter's credability (RTCRxx)

;CS147 *** Begin ***
IFE IPMONF,<			;Global nonexistant monitoring storage
DEFWRD(MNTPTP,PGSIZ)		; Page for traps if do not have monitoring code
   >
;CS147 *** End ***

;CS*** The following should be conditional on level 6 interrupt code but the
;CS*** bogus code in MNETDV isn't, so undefined externals result. Someday, it
;CS*** should be made properly conditionalized --VAF, 23-Aug-85
; Interrupt storage

RS LV6ACS,20			; PI level 6 AC storage
LV6PDP::IOWD NPISK6,PI6STK	; and stack pointer


IFN ANXN+BBNN,<
NDG RFNTSZ,^D64		;1822DV	; Size of RFNM hash table (must be power of 2)
RS(RFNTAB,RFNTSZ)	;1822DV	; RFNM counting hash table
RS IIMBUF,4		;IMPANX	; Buffer used to create irreg msgs
>


; Still IFN NETN, in RSDAT Psect

; HASH table for looking up connected networks

NDG IHSHSZ,^D11			; Must be prime and GT the number
				; of NCTs, should be GT 2x
				; for hash to be efficent
RS(NCTHSH,IHSHSZ)		; Hash table
RS(NCTTBL,IHSHSZ)		; Parallel table
RS(ADRHSH,IHSHSZ)		; Hash table of addresses

NMLHST==:%NETS+3		;CS150 Number of local host entries

NLHOST:: REPEAT <NMLHST>,<-1>
	EXP 0

	SUBTTL	Tables Interfacing Multinet to Higher Level Protocols

; Still IFN NETN, in RSDAT Psect

; The following tables are indexed by the protocol number and must be
;ordered appropriately.  See NP.xxx in ANAUNV.MAC.


; Macros to define a table, and insert an entry

DEFINE PTABLE(TABLE,DEFAULT)<..DFLT==DEFAULT	;; default table entry
	..DFLT			;; Entry for unspecified protocol
TABLE::	..TBL==.		;; Remember table start


DEFINE PENTRY(PROTOCOL,FLAG,VALUE)<
	IF2,<IFN .-..TBL-PROTOCOL,PRINTX ? PROTOCOL at bad offset in TABLE>
IFN FLAG,<VALUE>
IFE FLAG,<..DFLT>
>;; End of DEFINE PENTRY
> ; End of DEFINE PTABLE

;Table giving address of ASCII name for each protocol suite.

PTABLE(PRONAM,<[ASCIZ /???/]>)
PENTRY(0,INETN,[ASCIZ /INTERNET/])	; ** Temporary IP
PENTRY(1,INETN,[ASCIZ /INTERNET/])	; IP
;PENTRY(NP.IP,INETN,[ASCIZ /INTERNET/])	; IP
PENTRY(NP.CS,1,[ASCIZ /CSNET/])		; CSNET
PENTRY(NP.CHA,1,[ASCIZ /CHAOS/]) 	; CHAOS
PENTRY(NP.OSI,1,[ASCIZ /OSI/])		; OSI
;PENTRY(NP.DNA,1,[ASCIZ /DECNET/])	; Decnet
;PENTRY(NP.PUP,1,[ASCIZ /PUP/])		; PUP
;PENTRY(NP.XNS,1,[ASCIZ /XNS/])		; XNS


; The PROHST routine translates a standard form network address and
; optional port to an ASCII string.
;0/	Flags, GH%ADR and GH%PRT
;1/	Destination designator
;2/	NP.xxx protocol suite code
;5,6/	Standard form host address & port
;7/	Output instruction for character in 2
;	CALL @PROHST(T2)
;Ret+1:	  Error
;Ret+2:	Ok,
;1/	Updated destination designator (<nul> not required by PROHST)


PTABLE(PROHST,<XWD MSEC1,GENHST>)
PENTRY(0,INETN,<XWD MSEC1,IPHST>)	; Zero should be unused after NP.IP fix
PENTRY(1,INETN,<XWD MSEC1,IPHST>)
;PENTRY(NP.IP,INETN,<XWD MSEC1,IPHST>)
PENTRY(NP.CS,0,???)
PENTRY(NP.CHA,0,???)
PENTRY(NP.OSI,0,???)
;PENTRY(NP.DNA,0,???)
;PENTRY(NP.PUP,0,???)
;PENTRY(NP.XNS,0,???)


GENHST:	TXNN 11,GH%ADR			; Want address?
	IFSKP.				; Yes
	  CALL MNOUTD ;internal NOUT%	; Output a decimal number
	    NOP
	  MOVX T2," "
	  TXNE 11,GH%PRT		; Want both?
	   XCT 7			; Yes, separate by space
	    ERJMP .+1
	ENDIF.
	
	TXNN 11,GH%PRT			; Do port?
	IFSKP.				; Yes
	  CALL MNOUTD ;internal NOUT%	; Output a decimal number
	    NOP
	ENDIF.
	RET


; The PROIAD function translates a binary address into a domain name for
; lookup from the address in AC3.  E.g., an address of the form 1.2.3.4
; is translated to 4.3.2.1.IN-ADDR.ARPA.

;1/	Destination byte pointer
;2/	NP.xxx protocol code
;3,4/	-1 on first call, should be changed to distinguish subsequent calls
;5,6/	Standard form network address
;	CALL @PROIAD(T2)	; Get next label
;Ret+1:	  Another label is needed
;Ret+2:	All labels completed,


PTABLE(PROIAD,<<XWD MSEC1,IAD.GN>>)
PENTRY(0,INETN,<XWD MSEC1,IAD.IP>)	; Zero should be unused after NP.IP fix
PENTRY(1,INETN,<XWD MSEC1,IAD.IP>)
;PENTRY(NP.IP,INETN,<XWD MSEC1,IAD.IP>)
PENTRY(NP.CS,0,???)
PENTRY(NP.CHA,0,???)
PENTRY(NP.OSI,0,???)
;PENTRY(NP.DNA,0,???)
;PENTRY(NP.PUP,0,???)
;PENTRY(NP.XNS,0,???)


IAD.GN:	MOVE T4,T1			; Save initial pointer
	IBP T1				; Reserve space for length

	MOVE T2,T2
	ANDX T2,NA%RST			; Just 32 bits
	MOVEI T3,^D10			; Output decimal numbers
	NOUT%				; Output number
	  ERJMP .+1

	SETZ T2,			; Count characters
	MOVE T3,T4
	IBP T3				; Reserved for length
	DO.
	  IBP T3
	  CAME T3,T1			; Match final position?
	    AOJA T2,TOP.		; Not yet
	ENDDO.
	IDPB T2,T4			; Back and insert length
	RETSKP				; All done

; PRONUM parses an ASCII address string into standard format network
; address and (optional) port

;0/	GTHST% Flags:	If GH%PRT is set, a port should be parsed
;1/	Source designator
;2/	Protocol suite id (NP.xxx)
;7/	Instruction to execute to get next input character
;	Call @PRONUM(T2)
;Ret+1:	  Failed,	1/ Updated source designator
;Ret+2:	Succeeded,	0/ GH%ADR set if an ASCII address was parsed
;			1/ Updated source designator
;			5/ Standard format network addess (GH%ADR is set)
;			6/ Port if GH%PRT was set


PTABLE(PRONUM,<XWD MSEC1,NUM.GN>)
PENTRY(0,INETN,<XWD MSEC1,NUM.IP>)	; Zero should be unused after NP.IP fix
PENTRY(1,INETN,<XWD MSEC1,NUM.IP>)
;PENTRY(NP.IP,INETN,<XWD MSEC1,NUM.IP>)
PENTRY(NP.CS,0,???)
PENTRY(NP.CHA,0,???)
PENTRY(NP.OSI,0,???)
;PENTRY(NP.DNA,0,???)
;PENTRY(NP.PUP,0,???)
;PENTRY(NP.XNS,0,???)

NUM.GN:	MOVX T3,^D10			; Decimal
	NIN%				; Get a number
	  TXO T2,NA%FLG			; Failed
	MOVE 5,T2			; Save it
	MOVX T2,NP.GEN
	STOR T2,NA%PRO,+5		; Set protocol suite

	TXNN 11,GH%PRT			; Get a port too?
	IFSKP.				; Yes
	  TXNE 5,NA%FLG			; Get an address?
	    RET				; No, fail
	  MOVX T3,^D10
	  NIN%				; Get port
	    RET				; Failed
	  MOVE 6,T2			; Save it
	ENDIF.
	RET

;Table of routines which convert a host number to a network number.
;T1/	Internet address (with NP.xxx protocol suite code)
;Ret+1:	Always, T1/ network number (with NP.xxx code)

PTABLE(PRONET,<XWD MSEC1,R>)
PENTRY(0,INETN,<XWD MSEC1,IPNET>)
PENTRY(1,INETN,<XWD MSEC1,IPNET>)
;PENTRY(NP.IP,INETN,<XWD MSEC1,IPNET>)
PENTRY(NP.CS,0,???)
PENTRY(NP.CHA,0,???)
PENTRY(NP.OSI,0,???)
;PENTRY(NP.DNA,0,???)
;PENTRY(NP.PUP,0,???)
;PENTRY(NP.XNS,0,???)


;Table indicating whether a protocol is ON (.lt.0) or OFF (.eq.0).
;Initial value 1B35 means initialize the protocol, 1B18 becomes ON/OFF.
;The variable should be resident.

PTABLE(PROON,<[0]>)			; Assume missing
PENTRY(0,INETN,<XWD MSEC1,INTON>)	; ** Temporary Internet
PENTRY(1,INETN,<XWD MSEC1,INTON>)	; Internet
;PENTRY(NP.IP,INETN,<XWD MSEC1,INTON>)	; Internet
PENTRY(NP.CS,0,???)			; CSNet
PENTRY(NP.CHA,0,???)			; CHAOSNET
PENTRY(NP.OSI,0,???)			; OSI
;PENTRY(NP.DNA,0,???)			; DECNET
;PENTRY(NP.PUP,0,???)			; PUP
;PENTRY(NP.XNS,0,???)			; XNS


;Table containing routine called to initialize a protocol suite.
;It should create (using MNTFKI) and start any forks which it
;requires, but the forks should wait for B0 of NETSUP to be one before
;proceeding.  If 1B18 is set, it should set B0 in the corresponding
;variable when the protocol has been initialized.  Called with T1
;containing the protocol suite index (NP.xxx).

PTABLE(PROINZ,<XWD MSEC1,R>)
PENTRY(0,0,???)				; Zero should be unused after NP.IP fix
PENTRY(1,INETN,<XWD MSEC1,INTBEG>)	; Internet initialization
;PENTRY(NP.IP,INETN,<XWD MSEC1,INTBEG>)	; Internet initialization
PENTRY(NP.CS,0,???)			; CSNet initialization
PENTRY(NP.CHA,0,<XWD MSEC1,CHAINI>)	; CHAOSNET initialization
PENTRY(NP.OSI,0,???)			; OSI initialization
;PENTRY(NP.DNA,0,???)			; DECNET initialization
;PENTRY(NP.PUP,0,???)			; PUP initialization
;PENTRY(NP.XNS,0,???)			; XNS initialization


;Table of routines called to release job resources when job JOBNO is killed.

PTABLE(PROKJB,<XWD MSEC1,R>)
PENTRY(0,INETN,<XWD MSEC1,INTKJB>)	; Zero should be unused after NP.IP fix
PENTRY(1,INETN,<XWD MSEC1,INTKJB>)	; Internet kill job
;PENTRY(NP.IP,INETN,<XWD MSEC1,INTKJB>)	; Internet kill job
PENTRY(NP.CS,0,???)			; CSNet kill job
PENTRY(NP.CHA,0,???)			; CHAOSNET kill job
PENTRY(NP.OSI,0,???)			; OSI kill job
;PENTRY(NP.DNA,0,???)			; DECNET kill job
;PENTRY(NP.PUP,0,???)			; PUP kill job
;PENTRY(NP.XNS,0,???)			; XNS kill job


;Table of routines called to release fork resources when fork FORKX
;kills itself.

PTABLE(PROKFK,<XWD MSEC1,R>)
PENTRY(0,INETN,<XWD MSEC1,INTKFK>)	; Zero should be unused after NP.IP fix
PENTRY(1,INETN,<XWD MSEC1,INTKFK>)	; Internet kill fork
;PENTRY(NP.IP,INETN,<XWD MSEC1,INTKFK>)	; Internet kill fork
PENTRY(NP.CS,0,???)			; CSNet kill fork
PENTRY(NP.CHA,0,???)			; CHAOSNET kill fork
PENTRY(NP.OSI,0,???)			; OSI kill fork
;PENTRY(NP.DNA,0,???)			; DECNET kill fork
;PENTRY(NP.PUP,0,???)			; PUP kill fork
;PENTRY(NP.XNS,0,???)			; XNS kill fork


;Table of routines called when fork-relative files are to be closed.
;T1/ FORKN of fork for which files should be closed.

PTABLE(PROCZF,<XWD MSEC1,R>)
PENTRY(0,INETN,<XWD MSEC1,INTCZF>)	; Zero should be unused after NP.IP fix
PENTRY(1,INETN,<XWD MSEC1,INTCZF>)	; Internet close fork-relative files
;PENTRY(NP.IP,INETN,<XWD MSEC1,INTCZF>)	; Internet close fork-relative files
PENTRY(NP.CS,0,???)			; CSNet close fork-relative files
PENTRY(NP.CHA,0,???)			; CHAOSNET close fork-relative files
PENTRY(NP.OSI,0,???)			; OSI close fork-relative files
;PENTRY(NP.DNA,0,???)			; DECNET close fork-relative files
;PENTRY(NP.PUP,0,???)			; PUP close fork-relative files
;PENTRY(NP.XNS,0,???)			; XNS close fork-relative files


;Table of routines called at network input completion to process a packet.
;The routine is called at interrupt level, so should queue the packet for
;later processing.
;T1/	Extended packet pointer

PTABLE(PROEIN,<XWD MSEC1,RETBUF>)
PENTRY(0,INETN,<XWD MSEC1,IPEIN>)	; ** Temporary Internet input
PENTRY(1,INETN,<XWD MSEC1,IPEIN>)	; Internet input
;PENTRY(NP.IP,INETN,<XWD MSEC1,IPEIN>)	; Internet input
PENTRY(NP.CS,0,???)			; CSNet input 
PENTRY(NP.CHA,0,???)			; CHAOSNET input 
PENTRY(NP.OSI,0,???)			; OSI input 
;PENTRY(NP.DNA,0,???)			; DECNET input
;PENTRY(NP.PUP,0,???)			; PUP input
;PENTRY(NP.XNS,0,???)			; XNS input


;Table of routines called at network output completion.
;The routine is called at interrupt level, so should queue the packet for
;later processing.
;T1/	Extended packet address

PTABLE(PROODN,<XWD MSEC1,RETBUF>)
PENTRY(0,INETN,<XWD MSEC1,IPODN>)	; ** Temporary Output done on IP buffer
PENTRY(1,INETN,<XWD MSEC1,IPODN>)	; Output done on IP buffer
;PENTRY(NP.IP,INETN,<XWD MSEC1,IPODN>)	; Output done on IP buffer
PENTRY(NP.CS,0,???)			; CSNet output
PENTRY(NP.CHA,0,???)			; CHAOSNET output
PENTRY(NP.OSI,0,???)			; OSI output
;PENTRY(NP.DNA,0,???)			; DECNET output
;PENTRY(NP.PUP,0,???)			; PUP output
;PENTRY(NP.XNS,0,???)			; XNS output


;Table giving the protocol specific buffer overhead required.
;This determines how much space is allocated in an input buffer,
;and where the data is stored. The overhead includes the general
;buffer header and room for the largest local leader.

PTABLE(PROOVH,MAXOVH)
PENTRY(0,INETN,PKTELI)
PENTRY(1,INETN,PKTELI)
;PENTRY(NP.IP,INETN,PKTELI)
PENTRY(NP.CS,0,???)
PENTRY(NP.CHA,0,???)
PENTRY(NP.OSI,0,???)
;PENTRY(NP.DNA,0,???)
;PENTRY(NP.PUP,0,???)
;PENTRY(NP.XNS,0,???)


;Table giving handler for an interface going down.
;P1/	VNCT

PTABLE(PRONDN,<XWD MSEC1,R>)
PENTRY(0,INETN,<XWD MSEC1,INTDWN>)
PENTRY(1,INETN,<XWD MSEC1,INTDWN>)
;PENTRY(NP.IP,INETN,<XWD MSEC1,INTDWN>)
PENTRY(NP.CS,0,???)
PENTRY(NP.CHA,0,???)
PENTRY(NP.OSI,0,???)
;PENTRY(NP.DNA,0,???)
;PENTRY(NP.PUP,0,???)
;PENTRY(NP.XNS,0,???)


;Table giving per-protocol handler for an interface coming up.
;P1/	VNCT

PTABLE(PRONUP,<XWD MSEC1,R>)
PENTRY(0,INETN,<XWD MSEC1,INTUP>)
PENTRY(1,INETN,<XWD MSEC1,INTUP>)
;PENTRY(NP.IP,INETN,<XWD MSEC1,INTUP>)
PENTRY(NP.CS,0,???)
PENTRY(NP.CHA,0,???)
PENTRY(NP.OSI,0,???)
;PENTRY(NP.DNA,0,???)
;PENTRY(NP.PUP,0,???)
;PENTRY(NP.XNS,0,???)

; Free some symbol table space
	PURGE PTABLE,PNAME,..TBL,..DFLT

;The following TBLUK% tables are used by the SITE-ADDRESS.TXT parsing routines.

;Start a table definition

DEFINE TABLE(NAME,.xtra.<0>)<..TABLE==0	;; Init count
NAME::	XWD ..'NAME,..'NAME+.xtra.
>
DEFINE TABEND(NAME)< ..'NAME==..TABLE>

DEFINE KEY(NAME,DATA)<..TABLE==..TABLE+1 ;; Increment the count of entries
	XWD [ASCIZ \NAME\],DATA
>

;Tables for address file keywords

;Table of Commands
;Data is the routine address to process the command

TABLE(CMDNAM)
KEY SET,ADSETC			;Set command
KEY UNRESTRICTED-FORWARDING,ADURFC  ;Unrestricted forwarding command
TABEND(CMDNAM)

;Table of interface names
;Data is the initialization table for that type of hardware

TABLE(INTNAM)
KEY ALTO,ALTNCT
KEY AN20,ANXNCT
KEY CMU-NFE,CMUNCT		;CS99
KEY IMP10,IMPNCT
KEY IPNI,NINCT
KEY NETWORK-DTE,NFENCT
KEY NI,NINCT
KEY VIRTUAL,VIRNCT
TABEND(INTNAM)

;Table of interface modifiers

TABLE(TYPNAM)
KEY 1822,AD1822			; Local network with 1822 protocol
KEY CHAOS,ADCHAP		; ChaosNet Protocol Suite
KEY CHAOSN,ADCHAN		; Local network with ChaosNet protocol
KEY CMU-NFE,ADCMU		;CS99 CMU Network-FE
KEY DECNET,ADDNAP		; Address for DECNET protocols
KEY ETHERNET,ADETHR		; Local network with 10 MB Ethernet protocol
KEY ID,ADIDKW			; ID keyword for the NCT
KEY INTERNET,ADIPP		; Address for IP protocols
KEY IP,ADIPP			; Address for IP protocols
KEY LOGICAL-HOST-MASK,ADLHM	; Set logical host mask
KEY MONITOR,ADMON		; Enable monitoring of interface
KEY NETWORK-FE,ADNFE		; Local network with Network front end
KEY PACKET-SIZE,ADPSIZ		; Maximum packet size allowed
KEY PRIORITY,ADPRIO		; Priority of interface
KEY PUP,ADPUPP			; Address for PUP protocols
KEY SERIAL,ADTTY		;CS99 TTY interface
KEY SET,ADSET			; Set a parameter
KEY XNS,ADXNSP			; Address for XNS protocols
TABEND(TYPNAM)


; Sample Network level interface routines for unsupported protocols

; Network level protocols (set NTHDRL, NTPSIZ, NTPSTI, NTTYP, NTPVEC)


; Local network with Chaos protocol

ADCHAN::
	MOVX T4,NH.CHA		; ChaosNet local network
	MOVX T3,MAXLDR-.NBHDL	; Header size correction
	XMOVEI T2,CHAVEC	; Protocol vector
	HRROI T1,[ASCIZ /
% ChaosNet ignored (not in system configuration)./]
	JRST ADPVEC		; Join below


; Here on ETHERNET keyword

ADETHR::
	MOVX T4,NH.ETH		; Ethernet
	MOVX T3,MAXLDR-0-0	; Header size correction
	XMOVEI T2,ETHVEC	; Ethernet protocol vector
	HRROI T1,[ASCIZ /
% Ethernet ignored (not in system configuration)./]
	JRST ADPVEC		; Join below


; All join  ADPVEC  to place NL.xxx, local leader adjustment, Protocol
; Vector address into NCT with:

;T1/	Pointer to ASCIZ error message
;T2/	Protocol Vector address (or 0 if not in configuration)
;T3/	Local leader adjustment
;T4/	NL.xxx code


; Setup higher level protocol code, local network net number, and local
; network interface address (set NTPRO, NTNET, NTLADR, NTNLHM, another NLHOST)

;HTBJFN/JFN for reading
;TERM/	Terminator from previous field
;P1/	NCT address
;	CALL ADxxP
;	...
;	CALLRET ADLNPA	with
;T1/	Pointer to ASCIZ error message, or 0 if ok
;T2/	Protocol code (NP.xxx) for NTPRO
;T3/	Interface Address for NTLADR
;T4/	Network number for NTNET

; CHAOS protocol address (funny for CHAOS over IP)

ADCHAP::
	HRROI T1,[ASCIZ /% CHAOS ignored (not in system configuration)./]
	SKIPE [CHAOS]		; Chaos Protocol Suite in configuration
	  CALLRET ADLNPA	; No
	MOVE T3,P2		; Get address
	ANDI T3,177777		; Make 16 bits
	MOVEM T3,MYCHAD		; Set as local chaosnet address
	JUMPE P1,R		; If No NCT associated with it, return
	SETZ T1,T4		; No error nor network number
	MOVX T2,NP.CHA		; Else set it
	CALL ADLNPA		; Finish setup

; DECNET protocol address

ADDNAP::
	HRROI T1,[ASCIZ /DECNET Addresses not yet supported./]
	SKIPL NTLADR(P1)	; Address already specified?
	  HRROI T1,[ASCIZ /Address multiply specified/] ; Yes
	CALLRET ADLNPA		; Error

;PUP protocol address

ADPUPP::
	HRROI T1,[ASCIZ /PUP addresses not yet supported./]	
	SKIPL NTLADR(P1)	; Address already set?
	  HRROI T1,[ASCIZ /Address multiply specified/]
	CALLRET ADLNPA		; Error

;XEROX XNS protocol address

ADXNSP::
	HRROI T1,[ASCIZ /XNS addresses not yet supported./]	
	SKIPL NTLADR(P1)	; Address already set?
	  HRROI T1,[ASCIZ /Address multiply specified/]
	CALLRET ADLNPA		; Error

;Table of keywords for the SET modifier

TABLE(SETTAB)
KEY HANDLING-TYPE,STRHTY	; Allow Handling Type field incrementing (1822)
KEY OUTPUT-RATE,STORAT		; Set Output Rate NTORAT (Physical NCTs only)
KEY RFNM-COUNTING,STRFNC	; Enable RFNM counting (1822)
KEY TYPE-3-MESSAGES,STAR3	; Allow type 3 messages (1822)
TABEND(SETTAB)



; Set command switch bits.  Data is the section 0 address of a word
; containing: SwitchAddress,,FlagAddress

TABLE(SWTTAB)
KEY BBNOK,[TCPSW,,[BBNOK]]		; Enable BBN TCP JSYS interface
KEY DECOK,[TCPSW,,[DECOK]]		; Enable DEC JFN TCP: device
KEY DMS%EC,[DOMPAR,,[DMS%EC]]		; Enable domain caching
KEY DMS%ED,[DOMPAR,,[DMS%ED]]		; Enable domain functions
KEY DMS%ER,[DOMPAR,,[DMS%ER]]		; Enable domain resolver
KEY DMS%ES,[DOMPAR,,[DMS%ES]]		; Enable domain server
KEY IPS%FR,[INPSW,,[IPS%FR]]		; Enable IP fragment chaining
KEY IPS%LP,[INPSW,,[IPS%LP]]		; Enable IP process looping
KEY LOCAL-LOGICAL-HOST,[LOGHST,,[-1]]	; Allow logical hosts on this host
KEY MNS%MP,[MNTSW,,[MNS%MP]]		; Monitoring via datagrams
KEY MNS%MT,[MNTSW,,[MNS%MT]]		; Monitoring via TCP
KEY MNS%RF,[MNTSW,,[MNS%RF]]		; Restrict forwarding
KEY TCS%BD,[TCPSW,,[TCS%BD]]		; RTT comp includes slow packets
KEY TCS%LA,[TCPSW,,[TCS%LA]]		; Load average used in RX comp
KEY TCS%LP,[TCPSW,,[TCS%LP]]		; Enable TCP process looping
KEY TCS%O1,[TCPSW,,[TCS%O1]]		; Enable "optimization 1" (flow)
KEY TCS%O2,[TCPSW,,[TCS%O2]]		; Enable "optimization 2" (hash)
KEY TCS%RX,[TCPSW,,[TCS%RX]]		; Use RX'd packets in RTT comp
TABEND(SWTTAB)


;GTHST%'s TBLUK% table for operating system names.

TABLE(OPSTAB,20)			; Leave some room for more
KEY AEGIS,0				; Most are not defined in MONSYM
KEY ANTS,.HSANT
KEY APOLLO,0
KEY BS-2000,0
KEY CEDAR,0
KEY CMOS,0
KEY CMS,0
KEY COINS,0
KEY CTOS,0
KEY DCN,0
KEY DOMAIN,0
KEY EDX,0
KEY ELF,.HSELF
KEY EMMOS,0
KEY EPOS,0
KEY FOONEX,.HS10X
KEY GCOS,0
KEY IMAGEN,0
KEY INTERLISP,0
KEY FUZZ,.HSFUZ
KEY ITS,.HSITS
KEY IOS,0
KEY KOS,0
KEY LISPM,0
KEY LOCUS,0
KEY LOS,0
KEY MOS,0
KEY MSDOS,.HSDOS
KEY MTIP,.HSMTP
KEY MULTICS,.HSMLT
KEY MVS,0
KEY MVS/SP,0
KEY NETWORK,.HSNET
KEY NMS,0
KEY NONSTOP,0
KEY NOS-2,0
KEY OS-1100,0
KEY PERQ-OS,0
KEY PLI,0
KEY PLURIBUS,0
KEY RMX/RDOS,0
KEY ROS,0
KEY RSX11M,0
KEY SATOPS,0
KEY SIMP,0
KEY SWIFT,0

KEY TAC,.HSTAC
KEY TENEX,.HS10X
KEY TFTPSP,0
KEY TIP,.HSTIP
KEY TOPS10,.HSDEC
KEY TOPS20,.HST20
KEY TOPS20AN,.HST20
KEY ULTRIX,0
KEY UNIX,.HSUNX
KEY WAITS,.HSDEC
KEY V,0
KEY VM,0
KEY VM/CMS,0
KEY VM/SP,0
KEY VMS,.HSVMS
KEY VMS/EUNICE,0
KEY WAITS,0
KEY XDE,0
KEY ZMOS,0
TABEND(OPSTAB)


;Static storage used by the parsing routines for both ADRINI and HSTINI.
;Lock: HTBLCK (1B0 - write/ing, 0 - free, RH is # readers)

NR HTBJFN			; JFN of input file
NR BOL				; Byte number of start of current line
NR (TMPBUF,10)			; Temporary input buffer
NR TERM				; Terminating character
NR SAVEDP			; Saved stack fence


;Dummy protocol vectors and setup routines if configuration does not have a
;particular interface type (used by MNETWK when processing SITE-ADDRESS file).

IFE ALTN,	<ALTNCT==:0>	; No ALTO
IFE ANXN+BBNN,<			; No 1822 type networks
AD1822::
STRFNC::
STAR3::
STRHTY::
	HRROI	T1,[ASCIZ /1822 networks not included in configuration/]
	CALLRET PERROT		; report the error
>

IFE ANXN,	<ANXNCT==:0>	; No AN20
IFE BBNN,	<IMPNCT==:0>	; No IMP10
IFE CHAOS,	<CHANCT==:0
		 CHAVEC==:0>	; No Chaos net
IFE ETHER,	<ETHNCT==:0
		 ETHVEC==:0>	; No Ethernet
IFE KNIN,	<NINCT==:0>	; No KL NI's
IFE NFEN,	<NFENCT==:0
ADNFE::
	HRROI T1,[ASCIZ /NFE not included in configuration/]
	CALLRET PERROT
>	; No NFE

;CS99 *** Begin ***
IFE CMUN,	<CMUNCT==:0	;No CMU network-FE
ADCMU::
	HRROI T1,[ASCIZ /CMU-NFE not included in configuration/]
	CALLRET PERROT
>;CMUN
IFE TTYN,	<TTYNCT==:0
ADTTY::
	HRROI T1,[ASCIZ /SERIAL not included in configuration/]
	CALLRET PERROT
>;TTYN
;CS99 *** End ***

RS NCTVT			; Pointer to NCT list
RS LUPNCT			; Pointer to Loop back NCT

	SUBTTL	Network and Internet+TCP

; Still IFN NETN, in RSDAT Psect

  IFN INETN,<			; Both NETN and INETN

; Internet Protocol Tables

DEFINE PCLTAB(NAM,PRT,FLAG)<IP.'NAM=.
    RELOC IP.'NAM+.INTPC
	MSEC1,,NAM'CHK
    RELOC IP.'NAM+.INTPF
NAM'FLG::0
    RELOC IP.'NAM+.INTPI
	MSEC1,,NAM'INI
    RELOC IP.'NAM+.INTPQ
NAM'IPQ::0
    RELOC IP.'NAM+.INTPO
NAM'ON::1B18+1B35;; Want on, not initialized
    RELOC IP.'NAM+.INTPL
IFB  <PRT>,<NAM'PCL::	.'NAM'FM>
IFNB <PRT>,<NAM'PCL::	PRT>
    RELOC IP.'NAM+.INTPP
	MSEC1,,NAM'PRC
    RELOC IP.'NAM+.INTPS
NAM'SID::0
    RELOC IP.'NAM+.INTPT
NAM'TIM::377777777777
    RELOC IP.'NAM+.INTPE
	MSEC1,,NAM'ICM
IFB <FLAG>,<
MNTM5<RELOC IP.'NAM+.INTPM>
MNTM5<	XWD -M'NAM'AB,CELL(NAM'AB,0,,NAM)>
>
    RELOC IP.'NAM+.INTKJ
	MSEC1,,NAM'KJB
    RELOC IP.'NAM+.INTKF
	MSEC1,,NAM'KFK
    RELOC IP.'NAM+.INTCZ
	MSEC1,,NAM'CZF
    RELOC IP.'NAM+.INTPZ
> ; End of DEFINE PCLTAB

INTPIX::-3,,.INTPZ	; -# protocols,,Table length
			; Tables follow INTPIX
	PCLTAB (TCP)	; TCP table
	PCLTAB (ICM)	; ICMP table

	PCLTAB (INQ,-1)	; Internet User Queue table NB:	must be last since
			; protocol -1 takes anything

; Still IFN NETN, IFN INETN, in RSDAT Psect

MNTRFN::ASCII /PS:<OPERATOR>MNTRAC.BIN/ ; RSI has trouble with angle brackets
	BLOCK 3		; Name of file for traced information
MNTCFN::EXP 0		; TCP user's file name
	BLOCK ^D11
RS PPBUF,PPBWDS		; Packet printer buffer

REPEAT 0,<
NR MNTPSJ,1		; Jfn of Simulation file (containing PKTs)
NR MNTPSQ,1		; Queue of PKTs read so far (maybe RXed)
RSI MNTPSA,<BYTE (4)0(8)^D10,0,0,^D79> ; DEC-2136
> ; End of REPEAT 0

			; Internet Free Storage
NDG INTFSZ,200000	; Default to 200 pages
IFSM <NR INTFRE,INTFSZ>	; The free storage area
IFKL <
INTFSZ==INTFSZ&37777000	; Integral number of pages
IFE INTFSZ,<PRINTX % No internet free storage>

..MNTA==<..OFST+777>&37777000

DEFHDR(<Internet Free Storage>,..MNTA)
DEFWRD(INTFRE,INTFSZ-2)	; Define internet storage, low address
DEFWRD(INTFRZ,2)	; Internet storage high address

..MNTA==<..OFST+777>&37777000	; Page align
>

IFGE ..MNTA-<MNTSEC+1,,0>,<PRINTX ? Network section (MNTSEC) overflow.>


INTCLS::EXP 4B6,4B6,4B6,4B6,6B6,6B6,7B6,7B6
	POINT 3,CX,6
INTLNN::POINT 8,CX,11
	POINT 8,CX,11
	POINT 8,CX,11
	POINT 8,CX,11
	POINT 16,CX,19
	POINT 16,CX,19
	POINT 24,CX,27
	POINT 32,CX,35
INTNET::EXP 37700000000,37700000000,37700000000,37700000000
	EXP 37777600000,37777600000,37777777400,37777777777
INTHST::EXP 00077777777,00077777777,00077777777,00077777777
	EXP 00000177777,00000177777,00000000377,00000000000

; Still IFN NETN, IFN INETN, in RSDAT Psect

ND NTWBTS,<^D<MAXTCB*<8+2>+10>> ; Appox. number of wait bits needed
IFG NTWBTS-^D511,<PRINTX % Number of network wait bits clamped to 511.
		NTWBTS==^D511	; Must fit in 9-bits for TCPOOT, TCPZOT,
				; and TOPNF and TERRF fields
	> ; End of IFG NTWBTS-^D511
NDG NTWBWD,<^D<NTWBTS+1+31>/^D32> ; Number of words of wait bits (from TCP)

RS INTBFF,NTWBWD	; Internet Free Wait Bit Table
RS INTWTB,NTWBWD	; Internet Wait Bit Table

			; Internet User Queue Storage

;Following in RSI with STAT0 so can be accessed via IPOPR% etc.
;Only INQFK HAS to be resident for scheduler tests.

INQFK::BLOCK NIQ	; FORKX waiting for stuff on this queue (or -1)
			; in LH,,RH is FORKX of owner
INQJB::BLOCK NIQ	; Job owning this queue (Flags,,JOBN)
INQM0::BLOCK NIQ	; User queue mask for PIPRO (B24-31)
INQM1::BLOCK NIQ	; User queue mask for PIDH  (B00-31)
INQM2::BLOCK NIQ	; User queue mask for PISH  (B00-31)
INQM3::BLOCK NIQ	; User queue mask for for ports (l B0-15, f B16-31)
INQV0::BLOCK NIQ	; User queue value for PIPRO (B24-31)
INQV1::BLOCK NIQ	; User queue value for PIDH  (B00-31)
INQV2::BLOCK NIQ	; User queue value for PISH  (B00-31)
INQV3::BLOCK NIQ	; User queue value for ports (l B0-15, f B16-31)
INQRS::BLOCK NIQ	; # datagrams received,,# datagrams sent
INQSP::BLOCK NIQ	; Number of datagrams in reveive queue
INQSQ::BLOCK NIQ	; Source quench time
INQTM::BLOCK NIQ	; Timeout for this queue (or time last accessed)

	SUBTTL	Network and Internet+TCP -- Internet Monitoring Area

; Still IFN NETN, IFN INETN, in RSDAT Psect

; Macro to Allocate Statistics Area Blocks

DEFINE NTITEM(nam,len,lvl,typ,init,fmt,symb)<..XL=-1;; Not found typ
IFGE MNTMN-lvl,<
IFB  <init>,<;;				No initializer, use 0
  IFIDN <typ><G>,<..XL=..XL+1;;		Found typ
    NTITEN(nam,len,nam::,0)>;;		Global entry
  IFIDN <typ><O>,<..XL=..XL+1;;		Found typ
    NTITEN(nam,len,nam:,0)>>;;		Offset to reduce GLOBS
IFNB <init>,<;;				Have initializer, use it
  IFIDN <typ><G>,<..XL=..XL+1;;		Found typ
    NTITEN(nam,len,nam::,<init>)>;;	Global entry
  IFIDN <typ><O>,<..XL=..XL+1;;		Found typ
    NTITEN(nam,len,nam:,<init>)>>;;	Offset to reduce GLOBS
IFIDN <typ><R>,<..XL=..XL+1>;;		Found typ, no storage required
IFIDN <typ><S>,<..XL=..XL+1>;;		Found typ, no storage required
IFIDN <typ><T>,<..XL=..XL+1>;;		Found typ, no storage required
IFN ..XL,<PRINTX % Type code for NAM must be G, O, R, S, or T>
>> ; End of DEFINE NTITEM


; Macro to generate first word(s) of entry

REST0==0

DEFINE NTITEN(nam,len,lab,init)<.ntiti==.;;	Starting location
IFNDEF nam,<lab>
IRP init,<IFB  <init>,<	EXP 0	>
	  IFNB <init>,<	init	>>;;		Item
.ntiti==.-.ntiti;;				Length of initial list
IFG len-.ntiti,< BLOCK len-.ntiti >;;		Allocate remainder, if any
IFL len-.ntiti,< LENER(nam,\len,\.ntiti) >;;	Complain if length too small
> ; End of DEFINE NTITEN


; Macro to complain when block length is less than number of items initialized

DEFINE LENER (nam,len,iln)<
PRINTX % nam''s length (len) is less than the initialization length (iln)>


; Still IFN NETN, IFN INETN, in RSDAT Psect

STAT0::;DEFSTS(1)		; Allocate the status area
	XLIST
	DEFSTS(1)		; Allocate the status area
	LIST
STATZZ::0

	PURGE .ntiti,..XL


; Monitoring Area (Still in RSDAT Psect)

IFN IPMONF,<			;CS147

INTMLK::EXP -1			; Lock on INTMIP, etc.
				; -1 means free, .ge. 0 means in use
INTMFR::EXP 0			; .gt. 0 is end of saved trap info
				; (I.e., offset from INTMBS)

; The IP monitoring buffer
;	BLOCK	NBHDRL		; Buffer header overhead
INTMIB::BLOCK	PKTELI		; IP overhead
; The IP packet itself
INTMIP::BYTE (4)4,5(8)0(16)4*<INTMBE-.> ; IP Ver, Do, Len
	BYTE (16)1,0		; Segment ID & Frag info
	BYTE (8)^D255,INMNPR(16)0; TTL, *** Have to get a real assignment
	BYTE (32)0		; Source Address
	BYTE (8)^D10,3,0,5	; Destination (BBNA)

INTMDA::BYTE (16)INMNPT,INMNPT-1; Source and destination ports ***
	EXP <STATZZ-STAT0+1>*20	; Status length, words, LJ

; Received datagram buffer begins here
; Data between INTMBS and INTMES has to be shifted left for the datagram

INTMBS::BLOCK STATZZ-STAT0	; For copy of Statistics area

INTMES::0		; Guard word - end of region shifted left

	BYTE (32)STSTAB-.	; Offset to Symbol table
	BYTE (32)STATCT-.	; Offset to count table
	BYTE (32)STATPT-.	; Offset to pointer table
	BYTE (32)MNTPTP-.	; Offset to Packet Printer info


; Still IFN NETN, IFN INETN, in RSDAT Psect

; Packet printer info is already left shifted (mostly)

MNTPTP::BLOCK MNTPTS		; Default tracing page (Resident)

INTMBE::			; End of Monitored Information (2 to nth)
   >;IFN IPMONF			;CS147


; Macro to Create Statistics Name Table
; NB:  DEFSTS MUST be sorted as this table has a binary search applied to it.
;      It is sorted by STSORT, at network startup time (should probably be
;      done by POSTLD).


DEFINE NTITEM (nam,len,lvl,typ,init,fmt,symb)<..XL=ASCII /NAM/
IFGE MNTMN-lvl,<
	EXP ..XL		; nam
>> ; End of DEFINE NTITEM

;STSTAB-2 has -Length,,Next 2**n larger than Length
	XWD -STABLN,<1_<^D36-^L<STABLN>>>
;STSTAB-1 has 0,,Length
	EXP STABLN
STSTAB::400000000000		; Minimum
;	DEFSTS			; Status names
	XLIST
	DEFSTS			; Status names
	LIST
	377777777777		; Maximum
STABLN==.-STSTAB


; Still IFN NETN, IFN INETN, in RSDAT Psect

; Macro to Construct the Count Table

DEFINE NTITEM (nam,len,lvl,typ,init,fmt,symb)<..XL=<<ASCII /nam/>&7777>*100
IFGE MNTMN-lvl,<
IFB  <len>,<	XWD 1,..XL 	; nam  >
IFNB <len>,<	XWD len,..XL	; nam  >
>> ; End of DEFINE NTITEM


STATCT::0			; Minimum
;	DEFSTS			; Status counts
	XLIST
	DEFSTS			; Status counts
	LIST
	0			; Maximum
	PURGE ..XL

; Macro to Construct the LDB Pointer Table

DEFINE	PNTR(ac,of,ms)<POINTR 20*of,ms>


; Macro to decide if a block is a word/block or structure
; and whether it is based on STAT0 or the TCB

DEFINE NTITEM (nam,len,lvl,typ,init,fmt,symb)<IFGE MNTMN-lvl,<
  IFIDN <typ><G>,<	POINTR <<NAM-STAT0>>*20(TCB-1),-1 >
  IFIDN <typ><O>,<	POINTR <<NAM-STAT0>>*20(TCB-1),-1 >
  IFIDN <typ><R>,<
    IFNDEF %'NAM,<	POINTR 20*<NAM+TCB>,-1 >
    IFDEF  %'NAM,<	%'NAM (PNTR,,,NAM) >
  >;; End of IFIDN <typ><R>
  IFIDN <typ><S>,<	POINTR <<NAM-STAT0>>*20(TCB-1),-1 >
  IFIDN <typ><T>,<
    IFNDEF %'NAM,<	POINTR 20*NAM(TCB),-1 >
    IFDEF  %'NAM,<	%'NAM (PNTR,,(TCB),NAM) >
  >;; End of IFIDN <typ><T>
>> ; End of DEFINE NTITEM


STATPT::0
;	DEFSTS			; Status pointers
	XLIST
	DEFSTS			; Status pointers
	LIST
	0
INTMBF::0			; End of Monitored Information (first)

	SWAPCD			; Leaving RSDAT Psect

	SUBTTL	Network and Internet+TCP

; Still IFN NETN, IFN INETN, in NRCOD Psect

; Macros used to create the initialization bit table. MSB of first word
; maps to STAT0, last (used) bit of last word maps to STATZZ.

; Macro to assign next word in bit table

DEFINE NXTWD(w)<.ntitb==1B0;;		First bit position in next word
	EXP w;;				Assign current word
.ntitw==0;;				Initialize next word
> ; End of DEFINE NXTWD

; Macro to build a bitmap word by or-ing in the current bit if the
; corresponding STAT0 word should be cleared by STSINI.
; Non-blank i means value should not be cleared; Blank i means clear it.

DEFINE NXTBT(i)<IFIDN <i><REST0>,<.ntitz==1>;;	Remember rest are non-blank
  IFB <i>,<IFE .ntitz,<.ntitw==.ntitw+.ntitb>>;;Blank w/o REST0 means clear
  .ntitb==.ntitb_-1;;				Move to next bit position
  IFE .ntitb,<NXTWD(\.ntitw)>;;			Save full word & begin next
  .ntiti==.ntiti+1;;				Count words processed
> ; End of DEFINE NXTBT

; Macro to call above for each statistics block in DEFSTS.

DEFINE NTITEM (nam,len,lvl,typ,inil,fmt,symb)<.ntitz==0;;Nothing to do
IFGE MNTMN-lvl,<
  IFIDN <typ><G>,<.ntitz==1>;;			Something to do for G
  IFIDN <typ><O>,<.ntitz==1>;;			Something to do for O
  IFN .ntitz,<.ntitl==<.ntitz==<.ntiti==0>>;;	Length 1(-1), no REST0, no ini
    IFNB <len>,<.ntitl==len-1>;;		Length specified(-1)
    IFNB <inil>,<IRP inil,<NXTBT(inil)>>;;	Bit depends on initial
    IFG .ntitl+1-.ntiti,<REPEAT .ntitl+1-.ntiti,<NXTBT>>;; Finish unspecified
    IFL .ntitl+1-.ntiti,<LENER(nam,\<.ntitl+1>,\.ntiti)>>;; Ini too long for ln
>> ; End of DEFINE NTITEM

; Create the initialization bit table

.ntitb==1B0			; Position of first bit within word (ie STAT0)
.ntitw==0			; Initial contents of word
				; A 1 bit means clear the entry
STSBTS:;DEFSTS			; Compute the Bitmap table
;	NXTWD(\.ntitw)		; Finish last word
	XLIST
	DEFSTS			; Compute the Bitmap table
	NXTWD(\.ntitw)		; Finish last word
	LIST
STSBTL==.-STSBTS

	PURGE .ntit0,.ntitb,.ntiti,.ntitl,.ntitw,.ntitz


; Still IFN NETN, IFN INETN, in NRCOD Psect

; Monitoring Extrema Reset Table

DEFINE LNBLKN(nctn,which)<
	CELL(MNMSQ,2,+NCT'nctn'A) ;; Max # packets in Driver Output Queue
.ntitx==.ntitx+1;;		Count index for nctn
> ; End of DEFINE LNBLKN

IFN IPMONF,<			;CS147
STSLIM::XWD -STSLIL+1,.+1
	CELL(INGIQ,2,,ING)	; Max # packets in IP Input Queue
	CELL(ICMAB,2,,ICM)	; Max # packets in ICMP Input Queue
	CELL(INQAB,2,,INQ)	; Max # packets in Internet User Queue IQ
	CELL(TCPAB,2,,TCP)	; Max # packets in TCP Input Queue
	CELL(TCMTC,2,,TCM)	; Max # simultaneous active connections
   >				;CS147
IFE IPMONF,<			;CS147
STSLIM::0
   >				;CS147
.ntitx==0 ;; The following generate names NCTxx, pointed to by NTPSTI/S
REPEAT ALTN,<LNBLKN (\.ntitx,1)>;;Alto Statistics
REPEAT ANXN,<LNBLKN (\.ntitx,4)>;;AN20 Statistics
REPEAT BBNN,<LNBLKN (\.ntitx,4)>;;IMP10 Statistics
MNTM5<				;;CS99
REPEAT CMUN,<LNBLKN (\.ntitx,4)>;;CS99 CMU-NFE statistics
   >;MNTM5			;;CS99
REPEAT IPNIN,<LNBLKN (\.ntitx,4)>;;KLNI Statistics
REPEAT NFEN,<LNBLKN (\.ntitx,4)>;;NFE Statistics
STSLIL==.-STSLIM


; Still IFN NETN, IFN INETN, in NRCOD Psect

;STSINI	Initialize Statistics

;	CALL STSINI
;Ret+1:	Always

STSINI::SETZB T1,T2		; Standard report to monitor
IFN IPMONF,<			;CS147
	SKIPGE INTON		; Unless not yet initialized!
	  CALL INTMON		; Report before clearing everything
   >;IPMONF			;CS147

	NOSKED
	CALL LGTAD		; Record time counters reset
	MOVE T2,TODCLK
	PUSH P,T2		; Save TODCLK

	CONI PI,T3		; Get PI channel stats
	CHNOFF IMPCHN		; Don't let network interrupts in
	PUSH P,T3		; Save state

; Clear words in STAT0 area whose corresponding STSBTS bit is set, i.e. the
; INIT vector was blank there

	MOVE T4,[-STSBTL,,STSBTS]; For all words in STAT0 to STATZZ
	MOVEI T3,STAT0		; Point to first
	DO.
	  HRLI T3,<-44>		; 36 bits per word
	  MOVE T2,(T4)		; Get flag word
	  DO.
	    TXNE T2,1B0		; Clear word?
	      SETZM (T3)	; Yes
	    LSH T2,1		; Next bit
	    AOBJN T3,TOP.	; Back for next bit
	  ENDDO.
	  AOBJN T4,TOP.		; Back for next word
	ENDDO.
	POP P,T3		; Restore channel state
	TXNE T3,1B<28+IMPCHN>	; Was IMPCHN enabled?
	  CHNON IMPCHN		; Yes, reenable it

	POP P,T2		; Restore TODCLK
	DMOVEM T1,AASDT+2	; Beginning times into data

	MOVEI T1,OHUSE		; Overhead time accumulator
	MOVEM T1,TIMPTR		; Select that as the timer
	OKSKED

	RET

  > ; End of IFN INETN, (still within IFN NETN)



;MNTFRT	Return fork's run time in T1, here for OS independence.

MNTFRT::
	CALL GETFRT			; Get fork's runtime til now
	RET


	SUBTTL	Network but no Internet+TCP

  IFE INETN,<			; NETN but not INETN dummys, NRCOD Psect

.ASNIQ::.RELIQ::.SNDIN::.RCVIN::	JRST UJSYS0
INTLGO::INTBEG::RET
RS INETID,1			; Our Internet ID
RS INETLB,1			; Logical host bits (a mask)
RS TNBFFL,1
RS INTNCP,1
RS INTOBI,1
RS INTOBO,1
RS INTON,1
RS INTIBI,1
RS INTIBO,1
RS INTNFI,1
RS INTFRI,1
RS INTNFB,1
RS INTFLG,1

.SEND::	.RECV::	.OPEN::	.CLOSE:: .SCSLV::
.ABORT::.STAT::	.CHANL::	JRST UJSYS0
CHKTVT:: ABTJCS::	RET

RS TCPON,1
RS TCPIPQ,1		; Pointer to TCP input queue head
RS TCPFLG,1

  > ; End IFE INETN


; Still IFN NETN, in NRCOD Psect

; System/OS dependent routine to initialize a new fork to run in
; monitor context in section 1 with high priority.
;Q2/	In-section address for illegal interrupts, or 0 if none
;	CALL MNTFKI
;Ret+1:	Always, T1 has FORKX.

MNTFKI::
	SE1CAL			; Enter section 1, stack has return
	POP P,Q1		; Save return

; TCP (Internet fork) requires that the previous mode be user so that
; it can access the user's data buffer.  The other network forks do
; not access anything from the user, so setting it everywhere is ok.
				
IFE REL6,<
	MOVSI T1,(PC%USR)	; User mode bit
	MOVEM T1,FPC>		; Fake a return PC
IFN REL6,<
	MOVX T1,USRCTX		; Init context
	MOVEM T1,FFL
	SETZM FPC>

	MCENTR			; Establish monitor context

	MOVX T1,.FHSLF		; Give internet fork some priority
	MOVX T2,001		; Only allow queue 0
	SPRIW%
	  ERJMP .+1
	MOVX T1,JP%SYS+101	; Get the sys bit
	MOVEM T1,JOBBIT		; Make sure we can go fast

	JUMPE Q2,MNTFKX		; Done if no channels to setup
IFE REL6,<HRLI Q2,ITFPC>
IFN REL6,<HRLI Q2,MSEC1>
	MOVEM Q2,MONBK		; Setup unexpected interrupt dispatch
	MOVE T1,CHNSON
	MOVEM T1,MONCHN		; Setup for panic channels

MNTFKX:	MOVE T1,FORKX		; For caller
	JRST (Q1)		; Return to caller, fork setup

***** CHANGE #14; PAGE 37, LINE 70; PAGE 67, LINE 70
ARPTRS::
	RET

NIPNCT::Z

>			;End of IFE IPNIN
>			;End of IFN NETN



;DUMMY ROUTINES FOR NON-ARPANET SYSTEMS

   IFE NETN,<

	RESCD

 ---------------------------------
ARPTRS::
	RET

NIPNCT::Z

>			;End of IFE IPNIN

IFE ANXN,<IMPFPF::RET>		;CS99 APRSRV calls on IO page fail...

> ; End of IFN NETN

	SUBTTL	No Network

IFE NETN,<		; Dummys for non-network systems

STSTAB==:0		; Let POSTLD link without network code or IFE NETN
STATCT==:0		; Let POSTLD link without network code or IFE NETN
STATPT==:0		; Let POSTLD link without network code or IFE NETN
TCPIX==:0
NTBFIX==:0
IMPGTN==:0
IMPRDY==:0
IMPLBT==:0
NHSTST==:0
NHSTN==:0
HSTNAM==:0
NHOSTS==:0
HOSTN==:0
IMPNLK==:0



***** CHANGE #15; PAGE 38, LINE 15; PAGE 68, LINE 27
.IPOPR::
.TCOPR::
.ASNIQ::
.RELIQ::
.SNDIN::
.RCVIN::
.SEND::
 ---------------------------------
.IPOPR::
.TCOPR::
.ASNIQ::
.RELIQ::
.SNDIN::
.RCVIN::
.ATNVT::
TATNVT::
.SEND::

***** CHANGE #16; PAGE 38, LINE 26; PAGE 68, LINE 40
.SCSLV::
.ABORT::
.STAT::
.CHANL::
.GTNCP::
.GTHST::
.ASNSQ::
.CVSKT::
.SNDIM::
.RELSQ::
.FLHST::
.CVHST::
.ATNVT::
.RCVIM::
TATNVT::
	JRST UJSYS0

TCPDTB::
CHKNVT::
GTBHRT::
GIMLT1::
GIMLT2::
GIMLT3::
GIMLT4::
GNTLSK::
GNTBUF::
GNTSTS::
GNTBTC::
GNTBAL::
GNTFSK::
GHSTNM::
GNTRDY::
GTBHSS::
GHOSTN::
GNTAWD::
CHKNVT::
CHKTVT::
TVTMSG::
NETKFK::
MNTHLT::
IMICHK::
MNTCHK::
CHKIMP::
MNTINI::
IMPBEG::
ABTBUF::
HSTINI::
CHKNET::
IMPFPF::
INTLGO::
NETLGO::
INTBEG::
CHKTVT::
ABTJCS::
NETINI::RET

TCPIX==:0
IMPGTN==:0
IMPRDY==:0
IMPLBT==:0
NHSTST==:0
NHSTN==:0
HSTNAM==:0
NHOSTS==:0
HOSTN==:0
IMPNLK==:0

 ---------------------------------
.SCSLV::
.ABORT::
.STAT::
.CHANL::
.GTNCP::
.GTHST::
.CVSKT::
.FLHST::
.CVHST::
.ASNSQ::
.RCVIM::
.SNDIM::
.RELSQ::
	JRST UJSYS0


; Still IFE NETN

	RESCD

ABTBUF::
ABTJCS::
CHKIMP::
CHKNET::
CHKNVT::
CHKTVT::
GHOSTN::
GHSTNM::
GIMLT1::
GIMLT2::
GIMLT3::
GIMLT4::
GNTAWD::
GNTBAL::
GNTBTC::
GNTBUF::
GNTFSK::
GNTLSK::
GNTRDY::
GNTSTS::
GTBHRT::
GTBHSS::
HSTINI::
IMICHK::
IMPBEG::
IMPFPF::
INTBEG::
INTLGO::
MNTCHK::
MNTHLT::
MNTINI::
NETKFK::
NETLGO::
TCPDTB::
TVTMSG::
	RET


***** CHANGE #17; PAGE 39, LINE 2; PAGE 70, LINE 2
>				; End of IFE NETN


;STORAGE FOR RS04 SERVICE
 ---------------------------------
>				; End of IFE NETN



IFE CHAOS,<			; 100  Dummy CHAOS entries

CHAINI::
CHIINT::
RELCON::RET
CATNVT::ITERR ILINS2
RS CVTPTR
CHASBS==:0		; length of status block
CHADTB==:0
CHPMXT::		; maximum packet size
CHAON::	0
CHASBS==0
MYCHAD::0		; Dummy local address

> ; End of IFE CHAOS

	SUBTTL

;STORAGE FOR RS04 SERVICE

***** CHANGE #18; PAGE 48, LINE 19; PAGE 80, LINE 19
MJFN==:<<JSJFNA-JFNPGA+MLJFN-1>/MLJFN> ;MAX NUMBER OF JFNS
RJFN==:MJFN*MLJFN		;ACTUAL SPACE USED BY JFNS

JS(JFN0,0)			; FIRST JFN
JS(FILTMP,0)			; Lh -0 ptr to temp string block for default
				; Rh -0 point to temp string block
JS(FILBFI,0)
JS(FILBYT)			; Byte pointers to current window
JS(FILBFO)
JS(FILPRT,0)			; Pointer to protection string or protection #
 ---------------------------------
MJFN==:<<JSJFNA-JFNPGA+MLJFN-1>/MLJFN> ;MAX NUMBER OF JFNS
RJFN==:MJFN*MLJFN		;ACTUAL SPACE USED BY JFNS

JS(JFN0,0)			; FIRST JFN
JS(FILTMP,0)			; Lh -0 ptr to temp string block for default
				; Rh -0 point to temp string block
JS(FILBFI,0)			;FOR DECNET/TCP:, POINTER TO INPUT BUFFER
JS(FILBYT)			; Byte pointers to current window
JS(FILBFO)			;FOR DECNET/TCP:, OUTPUT BUFFER POINTER
JS(FILPRT,0)			; Pointer to protection string or protection #

***** CHANGE #19; PAGE 48, LINE 85; PAGE 80, LINE 85
	.PFTSC==:30		;TCP SECURITY
	.PFTCM==:31		;TCP COMPARTMENTS
	.PFTHR==:32		;TCP HANDLING RESTRICTIONS
	.PFTTC==:33		;TCP TRANMISSION CONTROL
	.PFTLH==:34		;TCP LOCAL HOST
	.PFTFH==:35		;TCP FOREIGN HOST

 ---------------------------------
	.PFTSC==:30		;TCP SECURITY
	.PFTCM==:31		;TCP COMPARTMENTS
	.PFTHR==:32		;TCP HANDLING RESTRICTIONS
	.PFTTC==:33		;TCP TRANMISSION CONTROL
	.PFTLH==:34		;TCP LOCAL HOST
	.PFTFH==:35		;TCP FOREIGN HOST
	.PFFLG==:36		;FLAGS (e.g., TCP)


***** CHANGE #20; PAGE 54, LINE 80; PAGE 86, LINE 80
RS FEFLG,1			;USE 2ND PROTOCOL UNTIL FURTHER NOTICE
	>

   IFE DCN,<			;IF NO DECNET SUPPORT
DEDMCB::
NSPSPC::
NSPQ::	BUG(NSPUDF)
NODINI::
 ---------------------------------
RS FEFLG,1			;USE 2ND PROTOCOL UNTIL FURTHER NOTICE
	>

   IFE DCN,<			;IF NO DECNET SUPPORT
DEDMCB::
NSPSPC::
NSPQ::	;BUG(NSPUDF)
	BUG.(HLT,NSPUDF,STG,soft,<UNSUPPORTED NETWORK FUNCTION>,,<

Cause:	If the monitor is not built for decnet (DCN is 0), and
	a decnet utility routine is called, this BUGHLT will occur.
	Invoking the following routines will cause this BUGHLT:
	DEDMCB (shut links to dead MCB), NSPSPC (get a network buffer), and
	NSPQ (put a message in the scheduler queue).
>)
NODINI::

***** CHANGE #21; PAGE 55, LINE 17; PAGE 87, LINE 17
;TTYSRV

IFL NTTBL1-NTTBL,<PRINTX ?NTTBL IS GREATER THAN NTTBL1. THAT IS AN ERROR.>

IBFRC::	EXP <^D20>B7+<NTTBL>B11+<NTTBL>B15+<NTTBL*NCHBF-1>B25+<NTTBL*NCHBF-2>B35
IBFRC1:: EXP <^D100>B7+<NTTBL>B11+<NTTBL1>B15+<NTTBL*NCHBF-1>B25+<NTTBL1*NCHBF-2>B35
OVRBCT==:1			;NUMBER OF EXTRA BUFFERS FOR HIGH SPEED LINES
 ---------------------------------
;TTYSRV

IFL NTTBL1-NTTBL,<PRINTX ?NTTBL IS GREATER THAN NTTBL1. THAT IS AN ERROR.>

IBFRC::	EXP <^D20>B7+<NTTBL>B11+<NTTBL>B15+<NTTBL*NCHBF-1>B25+<NTTBL*NCHBF-2>B35
IBFRC1:: EXP <^D100>B7+<NTTBL>B11+<NTTBL1>B15+<NTTBL*NCHBF-1>B25+<NTTBL1*NCHBF-2>B35
;CS143 The following is a buffer description for network lines.  Give them
;CS143 3 buffers in order to keep TCP packet sizes up.
IBFRC2:: EXP <^D100>B7+<NTTBL>B11+<NTTBL2>B15+<NTTBL*NCHBF-1>B25+<NTTBL2*NCHBF-2>B35 ;CS143

OVRBCT==:1			;NUMBER OF EXTRA BUFFERS FOR HIGH SPEED LINES

***** CHANGE #22; PAGE 67, LINE 44; PAGE 99, LINE 44
SEBQOU=:24			;OUTPUT PTR FOR SYSERR BLOCKS
				;IN A FIXED PLACE SO JOB 0 CAN QUEUE UP
				;BUGHLT BLOCK AFTER A CRASH.
CRSHTM=:23			;INITIAL TIME FOR RELOADS. -1 INDICATES NOT SETUP YET

NPISK==24			;SIZE OF STACK FOR PI LEVELS
NPISK6==NPISK*<IFE KNIN,<1>>+200*<IFN KNIN,<1>>

 ---------------------------------
SEBQOU=:24			;OUTPUT PTR FOR SYSERR BLOCKS
				;IN A FIXED PLACE SO JOB 0 CAN QUEUE UP
				;BUGHLT BLOCK AFTER A CRASH.
CRSHTM=:23			;INITIAL TIME FOR RELOADS. -1 INDICATES NOT SETUP YET

NPISK==24			;SIZE OF STACK FOR PI LEVELS
IFN KNIN,<NPISK6==:200>
IFE KNIN,<
  IFN NETN,<NPISK6==:50>
  IFE NETN,<NPISK6==:NPISK>
  >


***** CHANGE #23; PAGE 69, LINE 12; PAGE 101, LINE 12
RS DEV'TIM>>

;SCHEDULER DEVICE-DEPENDENT CALLS

LV8CHK::ECALL VBC
   IFG DCN,<CALL NSPCH7>	;DO DECNET MESSAGES
	RET
 ---------------------------------
RS DEV'TIM>>

;SCHEDULER DEVICE-DEPENDENT CALLS

LV8CHK::ECALL VBC
   IFG DCN,<CALL NSPCH7>	;DO DECNET MESSAGES
   IFN CHAOS,<CALL CHTTC7>	; 100 Do Chaosnet NVT input handling
	RET

***** CHANGE #24; PAGE 70, LINE 24; PAGE 102, LINE 24
IFG PLTN,<	INICLL PLT>
IFG PCDPN,<	INICLL CDP>		;CARD PUNCH
	INICLL PTP
	INICLL PTR
	INICLL DTA
	INICLL VBC
	RET
 ---------------------------------
IFG PLTN,<	INICLL PLT>
IFG PCDPN,<	INICLL CDP>		;CARD PUNCH
	INICLL PTP
	INICLL PTR
	INICLL DTA
	INICLL VBC
IFG CMUN,<	CALL NFEINI>	;CS99 Init network front end
	RET

***** CHANGE #25; PAGE 71, LINE 34; PAGE 103, LINE 34
	DTV FE			;THE FE DEVICE

MAXDEV==:.-DTEDTV+MINDEV-1	;LAST KNOWN DEVICE
   >				;END OF PROTOCOL TRANSFER VECTOR LOGIC


;KEEP THESE CELLS TOGETHER. THE ARPA GETAB% .LHOSTN REQUIRES THIS
 ---------------------------------
	DTV FE			;THE FE DEVICE

MAXDEV==:.-DTEDTV+MINDEV-1	;LAST KNOWN DEVICE
   >				;END OF PROTOCOL TRANSFER VECTOR LOGIC


repeat 0,<;cwl
;KEEP THESE CELLS TOGETHER. THE ARPA GETAB% .LHOSTN REQUIRES THIS

***** CHANGE #26; PAGE 71, LINE 49; PAGE 103, LINE 50
RS PRFNFD			; PREFERRED NETWORK NUMBER, SHIFTED
RS PRFNET			; PREFERRED NETWORK FOR GTHST%
.PSECT RSDAT
NLHOST:: REPEAT <%NETS+1>,<-1>
	EXP 0
.ENDPS

 ---------------------------------
RS PRFNFD			; PREFERRED NETWORK NUMBER, SHIFTED
RS PRFNET			; PREFERRED NETWORK FOR GTHST%
.PSECT RSDAT
NLHOST:: REPEAT <%NETS+1>,<-1>
	EXP 0
.ENDPS
> ; end of repeat 0

***** CHANGE #27; PAGE 76, LINE 33; PAGE 108, LINE 33
	SKIPE [MSEC1]		;WANT TO TRY EXTENDED ADDRESSING SECTIONS?
	HRRZM 1,EXADFL		;YES, SET FLAG
	HRRZM 1,EXADF1		;AND SET PC FORMAT FLAG
   IFN EXADF,<			;IF INTENDING TO USE EXTENDED FEATURES
	TRNE T1,-1		;MUST HAVE EXTENDED MACHINE
	SKIPN [MSEC1]		;AND BE USING MULTIPLE SECTIONS
	BUG (NOXADR)
   >				;END OF IFN EXADF
 ---------------------------------
	SKIPE [MSEC1]		;WANT TO TRY EXTENDED ADDRESSING SECTIONS?
	HRRZM 1,EXADFL		;YES, SET FLAG
	HRRZM 1,EXADF1		;AND SET PC FORMAT FLAG
   IFN EXADF,<			;IF INTENDING TO USE EXTENDED FEATURES
	TRNE T1,-1		;MUST HAVE EXTENDED MACHINE
	SKIPN [MSEC1]		;AND BE USING MULTIPLE SECTIONS
;	BUG (NOXADR)
	BUG.(HLT,NOXADR,STG,soft,<EXTENDED ADDRESSING CONFUSION>,,<

Cause:	This BUGHLT will occur if the monitor is built to
	use extended addressing (EXADF is non-0), and either the APRID
	instruction states that this machine does not have extended
	addressing, or the monitor is only built to use one section (MSEC1
	is 0).
>)
   >				;END OF IFN EXADF

***** CHANGE #28; PAGE 79, LINE 42; PAGE 111, LINE 42
	DEVRST PTR
IFG PCDPN,<	DEVRST CDP>		;CARD PUNCH
	DEVRST PTP
IFG PLTN,<	DEVRST PLT>
	DEVRST VBC
	DEVRST DLX
	PUSH P,INSKED
 ---------------------------------
	DEVRST PTR
IFG PCDPN,<	DEVRST CDP>		;CARD PUNCH
	DEVRST PTP
IFG PLTN,<	DEVRST PLT>
	DEVRST VBC
	DEVRST DLX
IFG CMUN,<	CALL NFERST>	;CS99 Restart network front end
	PUSH P,INSKED

***** CHANGE #29; PAGE 80, LINE 29; PAGE 112, LINE 29
	DMOVEM CX,PIXCX		;SAVE REGS
	MOVE P,[IOWD XPISK,PIXSTK]

	EXCH T1,PI5CNT		;SAVE AC AND GET COUNTER
	AOS T1			;BUMP COUNTER
	TRNN T1,7		;REACHED THRESHOLD YET?
	BUG (PI5ERR)		;YES, COMPLAIN
	EXCH T1,PI5CNT		;RESTORE AC AND COUNT
 ---------------------------------
	DMOVEM CX,PIXCX		;SAVE REGS
	MOVE P,[IOWD XPISK,PIXSTK]

	EXCH T1,PI5CNT		;SAVE AC AND GET COUNTER
	AOS T1			;BUMP COUNTER
	TRNN T1,7		;REACHED THRESHOLD YET?
;	BUG (PI5ERR)		;YES, COMPLAIN
	BUG.(CHK,PI5ERR,STG,hard,<UNEXPECTED UNVECTORED INTERRUPT ON CHANNEL 5>,,<

Cause:	If no devices are supposed to exist on the system which cause
	unvectored interrupts on channel 5, and more than ten such interrupts
	happen, this bugchk will occur.

	This bugchk is only possible if your system has no dectape and no
	card punch and no papertape reader.

	It has been observed that the DX20 occasionally causes random
	interrupts.  Having the bugchk not happen until ten such interrupts
	have been received tends to prevent the bugchk from happening.
>)
	EXCH T1,PI5CNT		;RESTORE AC AND COUNT

***** CHANGE #30; PAGE 81, LINE 2; PAGE 113, LINE 2
>
	XJEN PI5R

RS PI5R,4

PISC6::	XWD PI6R,.+1
   IFN SMFLG!KNIN!<PLTN+VBCN+PTPN>,<		;GENERATE FOR KS10 AND/OR HELLIWELL
	DMOVEM T1,PI6AC		;SAVE TEMP ACS
 ---------------------------------
>
	XJEN PI5R

RS PI5R,4

PI6FLG==SMFLG!KNIN!<PLTN+VBCN+PTPN+BBNN+PUPN> ;CM236

PISC6::	XWD PI6R,.+1
   IFN PI6FLG,<			;CS36 Generate if PI6 code is present
	DMOVEM T1,PI6AC		;SAVE TEMP ACS

***** CHANGE #31; PAGE 81, LINE 11; PAGE 113, LINE 13
	DMOVEM T3,PI6AC+2
	DMOVEM CX,PI6CX		;SAVE CX, P
	MOVE P,[IOWD NPISK6,PI6STK]
	XJRSTF [0
		MSEC1,,.+1]
   IFN SMFLG,<
 ---------------------------------
	DMOVEM T3,PI6AC+2
	DMOVEM CX,PI6CX		;SAVE CX, P
	MOVE P,[IOWD NPISK6,PI6STK]
	XJRSTF [0
		MSEC1,,.+1]
IFN BBNN,<CALL IMPSV>		;Primary IMP iterface
   IFN SMFLG,<

***** CHANGE #32; PAGE 81, LINE 20; PAGE 113, LINE 23
   >
	PICALL PLT
	PICALL VBC
	PICALL PTP
	PICALL KNI
>
   IFE SMFLG!KNIN!<PLTN+VBCN+PTPN>,<		;IF NO PI CODE
	PIOFF
	DMOVEM CX,PIXCX
	MOVE P,[IOWD XPISK,PIXSTK]

	BUG (PI6ERR)
	CLSB	6		;CLEAR THE INTERRUPT ON 6
   >	;IFE SMFLG!<PLTN+VBCN+PTPN>

   IFN SMFLG!KNIN!<PLTN+VBCN+PTPN>,<	;GENERATE FOR KS10 AND/OR HELLIWELL
PLTCHR::
 ---------------------------------
   >
	PICALL PLT
	PICALL VBC
	PICALL PTP
	PICALL KNI
>
   IFE PI6FLG,<			;CS36 IF NO PI CODE
	PIOFF
	DMOVEM CX,PIXCX
	MOVE P,[IOWD XPISK,PIXSTK]

;	BUG (PI6ERR)
	BUG.(CHK,PI6ERR,STG,HARD,<UNEXPECTED UNVECTORED INTERRUPT ON CHANNEL 6>,,<

Cause:	This BUG is not documented yet.

Action:

>)
	CLSB	6		;CLEAR THE INTERRUPT ON 6
   >	;IFE PI6FLG		;CS36

   IFN PI6FLG,<			;CS36 GENERATE FOR KS10 AND/OR HELLIWELL
PLTCHR::

***** CHANGE #33; PAGE 81, LINE 40; PAGE 113, LINE 50
LPTCHR::
KNICHR::
IMPCHR::
	DMOVE T1,PI6AC
	DMOVE T3,PI6AC+2
	DMOVE CX,PI6CX		;RESTORE ACS

RS PI6AC,4
RS PI6CX,2
RS PI6STK,NPISK6
   >;IFN SMFLG!<PLTN+VBCN+PTPN>

   IFE SMFLG!KNIN!<PLTN+VBCN+PTPN>,<
	DMOVE CX,PIXCX
	PION
   >

	XJEN PI6R		;DONE

 ---------------------------------
LPTCHR::
KNICHR::
IMPCHR::
	DMOVE T1,PI6AC
	DMOVE T3,PI6AC+2
	DMOVE CX,PI6CX		;RESTORE ACS
   >;CS36
;CS36 *** Begin ***
   IFE PI6FLG,<
	DMOVE CX,PIXCX
	PION
   >
	XJEN PI6R		;DONE

   IFN PI6FLG,<
RS PI6AC,4
RS PI6CX,2
   >
;CS*** The following should be inside the above conditional, but due to some
;CS*** major braindamage in MNETDV (see other CS*** comment) it has to always
;CS*** be defined, even if no PI6 devices.
RS PI6STK,NPISK6		;CS***



***** CHANGE #34; PAGE 82, LINE 36; PAGE 114, LINE 36
; AC1/ 18-35 CORE PAGE NUMBER (CST INDEX) OF PAGE
;      DWRBIT WRITE OPERATION IF 1, READ IF 0

DRMIO::	HLLZS CST3(1)		;SINGLE PAGE REQUEST, ZERO REST OF LIST
DRMIOM::SKIPG DRUMP		;DRUM ON SYSTEM?
	JRST DSKIOM		;NO, USE DISK
	BUG(DRUMP1)

 ---------------------------------
; AC1/ 18-35 CORE PAGE NUMBER (CST INDEX) OF PAGE
;      DWRBIT WRITE OPERATION IF 1, READ IF 0

DRMIO::	HLLZS CST3(1)		;SINGLE PAGE REQUEST, ZERO REST OF LIST
DRMIOM::SKIPG DRUMP		;DRUM ON SYSTEM?
	JRST DSKIOM		;NO, USE DISK
;	BUG(DRUMP1)
	BUG.(HLT,DRUMP1,STG,soft,<DRMIO - DRUMP ON BUT NO DRUM CODE IN SYSTEM>,,<

Cause:	This can only happen on the 2020.

	This BUGHLT occurs at DRMIOM if DRUMP contains a positive number.
	DRUMP contains DRMN, which, if positive, causes the DRUMP1 BUGHLT code
	not to be assembled.
>)


