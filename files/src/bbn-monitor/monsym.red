REDIT 1(104) COMPARE by user CLYNN, 10-Feb-86 18:57:46
File 1: CWL:<DEC-6-1-BASE>MONSYM.MAC.1
File 2: CWL:<5-4-NET>MONSYM.MAC.19
***** CHANGE #1; PAGE 4, LINE 86; PAGE 4, LINE 86
DEFJS XGSEV,614,MSEC1		;EXTENDED GET SPECIAL ENTRY VECTOR
DEFJS XPEEK,626,MSEC1
DEFJS NI,630,MSEC1		;Direct Ethernet Interface....very temporary

;TOPS20AN JSYS DEFINITIONS

DEFJS SEND,740,MSEC1      	;INTERNET+TCP ... TEMPORARY
DEFJS RECV,741,MSEC1      	;INTERNET+TCP ... TEMPORARY
DEFJS OPEN,742,MSEC1      	;INTERNET+TCP ... TEMPORARY
DEFJS CLOSE,743,MSEC1     	;INTERNET+TCP ... TEMPORARY
DEFJS SCSLV,744,MSEC1     	;INTERNET+TCP ... TEMPORARY
DEFJS STAT,745,MSEC1      	;INTERNET+TCP ... TEMPORARY
DEFJS CHANL,746,MSEC1     	;INTERNET+TCP ... TEMPORARY
DEFJS ABORT,747,MSEC1     	;INTERNET+TCP ... TEMPORARY
DEFJS SNDIM,750,MSEC1,,OLD      ;TOPS20AN
DEFJS RCVIM,751,MSEC1,,OLD      ;TOPS20AN
DEFJS ASNSQ,752,MSEC1,,OLD      ;TOPS20AN
DEFJS RELSQ,753,MSEC1,,OLD      ;TOPS20AN
DEFJS SNDIN,754,MSEC1     	;INTERNET
DEFJS RCVIN,755,MSEC1     	;INTERNET
DEFJS ASNIQ,756,MSEC1     	;INTERNET
DEFJS RELIQ,757,MSEC1     	;INTERNET
DEFJS IPOPR,760,MSEC1		;IP FUNCTIONS JSYS
DEFJS TCOPR,761,MSEC1		;TCP FUNCTIONS JSYS

 ---------------------------------
DEFJS XGSEV,614,MSEC1		;EXTENDED GET SPECIAL ENTRY VECTOR
DEFJS XPEEK,626,MSEC1
DEFJS NI,630,MSEC1		;Direct Ethernet Interface....very temporary

;TOPS20AN JSYS DEFINITIONS

DEFJS SEND,740,MSEC1,,OLD      	;INTERNET+TCP ... TEMPORARY
DEFJS RECV,741,MSEC1,,OLD      	;INTERNET+TCP ... TEMPORARY
DEFJS OPEN,742,MSEC1,,OLD      	;INTERNET+TCP ... TEMPORARY
DEFJS CLOSE,743,MSEC1,,OLD     	;INTERNET+TCP ... TEMPORARY
DEFJS SCSLV,744,MSEC1,,OLD     	;INTERNET+TCP ... TEMPORARY
DEFJS STAT,745,MSEC1,,OLD      	;INTERNET+TCP ... TEMPORARY
DEFJS CHANL,746,MSEC1,,OLD     	;INTERNET+TCP ... TEMPORARY
DEFJS ABORT,747,MSEC1,,OLD     	;INTERNET+TCP ... TEMPORARY
DEFJS SNDIM,750,MSEC1,NIM,OLD   ;TOPS20AN
DEFJS RCVIM,751,MSEC1,NIM,OLD   ;TOPS20AN
DEFJS ASNSQ,752,MSEC1,NIM,OLD   ;TOPS20AN
DEFJS RELSQ,753,MSEC1,NIM,OLD   ;TOPS20AN
DEFJS SNDIN,754,MSEC1,,OLD     	;INTERNET
DEFJS RCVIN,755,MSEC1,,OLD     	;INTERNET
DEFJS ASNIQ,756,MSEC1,,OLD     	;INTERNET
DEFJS RELIQ,757,MSEC1,,OLD     	;INTERNET
DEFJS IPOPR,760,MSEC1		;IP FUNCTIONS JSYS
DEFJS TCOPR,761,MSEC1		;TCP FUNCTIONS JSYS
DEFJS GTMPG,762,MSEC1,,OLD	;INTERNET/GENERAL

DEFJS TACLG,764,MSEC1		;TAC LOGIN JSYS
DEFJS GTDOM,765,MSEC1		;INTERNET


***** CHANGE #2; PAGE 6, LINE 97; PAGE 6, LINE 97
AQ%SPT==1B1			; Single port protocol
AQ%ICM==1B2			; Permit send/receive of ICMP messages

.INTVR==4			; Internet protocol version number

		; Queue descriptor block format
.IQPRV==^D0	; Internet protocol value (bits 24-31)
.IQFHV==^D1	; Internet foreign host value (bits 0-31)
.IQSHV==^D2	; Internet source host value (bits 0-31)
.IQPTV==^D3	; Internet port value, local bits 0-15, foreign bits 16-31
.IQPRM==^D4	; Mask for protocol value (bits 24-31)
.IQFHM==^D5	; Mask for foreign host value (bits 0-31)
.IQSHM==^D6	; Mask for source host value (bits 0-31)
.IQPTM==^D7	; Mask for port value, local bits 0-15, foreign bits 16-31
		; Note: port(s) follow IP header if .IQPTM is non-zero
		; Note: If AS%SPT is specified, the (single) port is
.IQLEN==^D8	; in bits 0-15 (bits 16-31 should be zero)

 ---------------------------------
AQ%SPT==1B1			; Single port protocol
AQ%ICM==1B2			; Permit send/receive of ICMP messages

.INTVR==4			; Internet protocol version number

		; Queue descriptor block format


***** CHANGE #3; PAGE 6, LINE 118; PAGE 6, LINE 107
.IPKSG==1              		; Word with segmentation info
.IPKPR==2              		; Word with time to live, checksum, protocol
.IPKSH==3              		; Word with source host
.IPKDH==4              		; Word with destination host

DEFINE DEFIP.<
DEFSTR(PIVER,.IPKVR,3,4)        ; PACKET.INTERNET.VERSION
DEFSTR(PIDO,.IPKVR,7,4)         ; PACKET.INTERNET.DATA-OFFSET
DEFSTR(PITOS,.IPKVR,15,8)       ; PACKET.INTERNET.TYPE-OF-SERVICE
  DEFSTR(PIPRC,.IPKVR,10,3)     ; PACKET.IP.PRECEDENCE
  DEFSTR(PILDY,.IPKVR,11,1)     ; PACKET.IP.LOW-DELAY
  DEFSTR(PIHTR,.IPKVR,12,1)     ; PACKET.IP.HIGH-THROUGHPUT
  DEFSTR(PIHRL,.IPKVR,13,1)     ; PACKET.IP.HIGH-RELIABILITY
DEFSTR(PIPL,.IPKVR,31,16)       ; PACKET.INTERNET.PACKET-LENGTH
DEFSTR(PISID,.IPKSG,15,16)      ; PACKET.INTERNET.SEGMENT-ID
DEFSTR(PIFLG,.IPKSG,18,3)       ; PACKET.INTERNET.FLAGS
  DEFSTR(PIDF,.IPKSG,17,1)      ; PACKET.INTERNET.DONT-FRAGMENT
  DEFSTR(PIMF,.IPKSG,18,1)      ; PACKET.INTERNET.MULTIFRAGMENT
DEFSTR(PIFO,.IPKSG,31,13)       ; PACKET.INTERNET.FRAGMENT-OFFSET
DEFSTR(PITTL,.IPKPR,7,8)        ; PACKET.INTERNET.TIME-TO-LIVE
DEFSTR(PIPRO,.IPKPR,15,8)       ; PACKET.INTERNET.PROTOCOL
DEFSTR(PICKS,.IPKPR,31,16)      ; PACKET.INTERNET.HEADER-CHECKSUM
DEFSTR(PISH,.IPKSH,31,32)       ; PACKET.INTERNET.SOURCE-HOST
DEFSTR(PIDH,.IPKDH,31,32)       ; PACKET.INTERNET.DESTINATION-HOST
>				; END OF DEFINE DEFIP.

 ---------------------------------
.IPKSG==1              		; Word with segmentation info
.IPKPR==2              		; Word with time to live, checksum, protocol
.IPKSH==3              		; Word with source host
.IPKDH==4              		; Word with destination host

DEFINE DEFIP.<
DEFSTR(IPVER,.IPKVR,3,4)        ; PACKET.INTERNET.VERSION
DEFSTR(IPDO,.IPKVR,7,4)         ; PACKET.INTERNET.DATA-OFFSET
DEFSTR(IPTOS,.IPKVR,15,8)       ; PACKET.INTERNET.TYPE-OF-SERVICE
  DEFSTR(IPPRC,.IPKVR,10,3)     ; PACKET.IP.PRECEDENCE
  DEFSTR(IPLDY,.IPKVR,11,1)     ; PACKET.IP.LOW-DELAY
  DEFSTR(IPHTR,.IPKVR,12,1)     ; PACKET.IP.HIGH-THROUGHPUT
  DEFSTR(IPHRL,.IPKVR,13,1)     ; PACKET.IP.HIGH-RELIABILITY
DEFSTR(IPPL,.IPKVR,31,16)       ; PACKET.INTERNET.PACKET-LENGTH
DEFSTR(IPSID,.IPKSG,15,16)      ; PACKET.INTERNET.SEGMENT-ID
DEFSTR(IPFLG,.IPKSG,18,3)       ; PACKET.INTERNET.FLAGS
  DEFSTR(IPDF,.IPKSG,17,1)      ; PACKET.INTERNET.DONT-FRAGMENT
  DEFSTR(IPMF,.IPKSG,18,1)      ; PACKET.INTERNET.MULTIFRAGMENT
DEFSTR(IPFO,.IPKSG,31,13)       ; PACKET.INTERNET.FRAGMENT-OFFSET
DEFSTR(IPTTL,.IPKPR,7,8)        ; PACKET.INTERNET.TIME-TO-LIVE
DEFSTR(IPPRO,.IPKPR,15,8)       ; PACKET.INTERNET.PROTOCOL
DEFSTR(IPCKS,.IPKPR,31,16)      ; PACKET.INTERNET.HEADER-CHECKSUM
DEFSTR(IPSH,.IPKSH,31,32)       ; PACKET.INTERNET.SOURCE-HOST
DEFSTR(IPDH,.IPKDH,31,32)       ; PACKET.INTERNET.DESTINATION-HOST
>				; END OF DEFINE DEFIP.

.IQPRV==^D0	; Internet logical host (B0-23) & protocol value (bits 24-31)
.IQFHV==^D1	; Internet foreign host value (bits 0-31)
.IQSHV==^D2	; Internet source host value (bits 0-31)
.IQPTV==^D3	; Internet port value, local bits 0-15, foreign bits 16-31
.IQPRM==^D4	; Mask for logical host (B0-23) & protocol value (bits 24-31)
.IQFHM==^D5	; Mask for foreign host value (bits 0-31)
.IQSHM==^D6	; Mask for source host value (bits 0-31)
.IQPTM==^D7	; Mask for port value, local bits 0-15, foreign bits 16-31
		; Note: port(s) follow IP header if .IQPTM is non-zero
		; Note: If AS%SPT is specified, the (single) port is
.IQLEN==^D8	; in bits 0-15 (bits 16-31 should be zero)
;AC2 must be 0
;AC3 must be 0


***** CHANGE #4; PAGE 17, LINE 24; PAGE 17, LINE 24
.DVCDP==:21			;CARD PUNCH
.DVDCN==:22			;DECNET ACTIVE COMPONENT
.DVSRV==:23			;DECENT PASSIVE COMPONENT
.DVATS==:24			;APPLICATIONS TERMINAL SERVICE
.DVADS==:25			;AYDIN DISPLAY


;DSKOP
 ---------------------------------
.DVCDP==:21			;CARD PUNCH
.DVDCN==:22			;DECNET ACTIVE COMPONENT
.DVSRV==:23			;DECENT PASSIVE COMPONENT
.DVATS==:24			;APPLICATIONS TERMINAL SERVICE
.DVADS==:25			;AYDIN DISPLAY

;DSKOP

***** CHANGE #5; PAGE 23, LINE 51; PAGE 23, LINE 51

.GFAUT==:0			;GET FILE AUTHOR
.GFLWR==:1			;GET FILE LAST WRITER

;GTHST				;TOPS20AN

.GTHSZ==:0			;HOST TABLE SIZES
.GTHIX==:1			;INDEX TO STRING CONVERSION
.GTHNS==:2			;NUMBER TO STRING CONVERSION
.GTHSN==:3			;STRING TO NUMBER CONVERSION
.GTHHN==:4			;HOST NUMBER TO STATUS
.GTHHI==:5			;HOST INDEX TO STATUS
.GTHLN==:6			;GET LOCAL NUMBER ON A NETWORK
.GTHNT==:7			;GET STATUS TABLE OF A NETWORK

 ---------------------------------

.GFAUT==:0			;GET FILE AUTHOR
.GFLWR==:1			;GET FILE LAST WRITER

;GTHST				;TOPS20AN

;Flags passed in the left half of AC1 for GTHST%
;Name		;For Function	;Meaning

GH%ANY==:1B1	;.GTHSN		;Match Host, Gateway, or Network
GH%GWY==:1B2	;.GTHSN		;Only match Gateway
GH%NET==:1B3	;.GTHSN		;Only match Network
				;None of the above (old way) Only match Host
GH%MOD==:3B5	;All		;Mode field
 .GHDEF==:0			; Default
 .GHLCL==:1			; Local information only
 .GHNRF==:2			; No referall
 .GHRF==:3			; Referall
GH%INI==:1B6	;All		;Initiate resolution and fail return
GH%MBA==:1B7	;.GTHNS,SN,RR	;Must be authoritive
GH%VIR==:1B8	;.GTHNS,SN,RR	;Use virtual circuit
GH%RRF==:1B9	;.GTHNS,SN,RR	;Resource Record Format
GH%CNM==:1B10	;.GTHRR		;Return canonical name

GH%SNM==:1B11	;.GTHNS,SN	;Suppress printing name, no name parsed
GH%ADR==:1B12	;.GTHNS,SN	;Print ASCII address, ASCII address parsed
GH%PRT==:1B13	;.GTHNS,SN	;Print port, parse a port
GH%PSU==:1B14	;.GTHIX,.GTHNS,SN ;Include protocol suite identifier, found
GH%SPC==:1B15	;.GTHNS		;Print trailing space

GH%AKA==:1B16	;.GTHSN,RR	;Alias found
GH%TRN==:1B17	;.GTHRR		;Truncated reply

.GTHSZ==:0			;HOST TABLE SIZES
.GTHIX==:1	;Obsolete	;INDEX TO STRING CONVERSION
.GTHNS==:2			;NUMBER TO STRING CONVERSION
.GTHSN==:3			;STRING TO NUMBER CONVERSION
.GTHHN==:4			;HOST NUMBER TO STATUS
.GTHHI==:5	;Obsolete	;HOST INDEX TO STATUS
.GTHLN==:6			;GET LOCAL NUMBER ON A NETWORK
.GTHNT==:7			;GET STATUS TABLE OF A NETWORK
.GTHRT==:10			; Route to a host
.GTHRR==:11			; Return Resource Record
GH%OPR==:3B1	;.GTHRR AC3	;Query operation
.GHSTD==:0			; Standard
.GHINV==:1			; Inverse
.GHCM==:2			; Completion, multiple
.GHCU==:3			; Completion, unique
GH%TYP==:377B9	;.GTHRR AC3	;Query type
.GHA==:^D1			; Host address
.GHNS==:^D2			; Authoritative name server
.GHMD==:^D3			; Mail destination
.GHMF==:^D4			; Mail forwarder
.GHCNA==:^D5			; Canonical name
.GHSOA==:^D6			; Start of zone of authority
.GHMB==:^D7			; Mailbox domain name
.GHMG==:^D8			; Mailbox group member
.GHMR==:^D9			; Mail rename
.GHNUL==:^D10			; Null resource record
.GHWKS==:^D11			; Well known service description
.GHPTR==:^D12			; Domain name pointer
.GHHIN==:^D13			; Host information
.GHMIN==:^D14			; Mail information
.GHZON==:^D252			; Zone transfer
.GHWMB==:^D253			; Wild mailbox (e.g., .GHMB, .GHMG, .GHMR)
.GHWMA==:^D254			; Wild mail agent (e.g., .GHMD, .GHMF)
.GHSTR==:^D255			; Wild
GH%CLA==:377B17	;.GTHRR AC3	;Query class
.GHIN==:^D1			; Internet
.GHCS==:^D2			; CSNet
;.GHSTR==:^D255			; Wild
GH%LEN==:777777B35		; Maximum length field


***** CHANGE #6; PAGE 24, LINE 51; PAGE 24, LINE 51
.GOANA==:22			;ACCESS TO ARPANET

.GOATJ==:23			;ATACH JSYS
	.GOTJB==:1		;TAGET JOB NUMBER
	.GOTTY==:2		;SOURCE TTY NUMBER

.GOKMZ==:^D20			;MAX ARGUMENT BLOCK SIZE FOR GETOK REQUEST
 ---------------------------------
.GOANA==:22			;ACCESS TO ARPANET

.GOATJ==:23			;ATACH JSYS
	.GOTJB==:1		;TAGET JOB NUMBER
	.GOTTY==:2		;SOURCE TTY NUMBER

.GOAIQ==:24			;ASSIGN INTERNET QUEUE
	;...			;Arguments: Queue Descriptor Block (QDB)

.GOKMZ==:^D20			;MAX ARGUMENT BLOCK SIZE FOR GETOK REQUEST

***** CHANGE #7; PAGE 38, LINE 45; PAGE 38, LINE 45
.MOSLM==:47			;SET LINE COUNTER MAXIMUM
.MORLM==:50			;READ LINE COUNTER MAXIMUM
.MOTPS==:51			;PSI FOR NON-CONTROLLING TERMINAL
.MOPCS==:52			;SET PAGE PAUSE CHARACTER
.MOPCR==:53			;READ PAGE PAUSE CHARACTER

;NET MTOPR NUMBERS
 ---------------------------------
.MOSLM==:47			;SET LINE COUNTER MAXIMUM
.MORLM==:50			;READ LINE COUNTER MAXIMUM
.MOTPS==:51			;PSI FOR NON-CONTROLLING TERMINAL
.MOPCS==:52			;SET PAGE PAUSE CHARACTER
.MOPCR==:53			;READ PAGE PAUSE CHARACTER

; TELNET negotiation functions
.MONEG==:60			;Do TELNET negotiation
   ; Bits defined for AC3
   MO%RNS==1B0			; Read the state of the option
   MO%NEG==377B17		; Negotiation to send
   MO%OPT==377B35		; Option Code
   ; Returns option state bits in AC3:
   MO%OND==1B35			; Outstanding DO
   MO%ONW==1B34			; Outstanding WILL
   MO%CSD==1B33			; Current State DO
   MO%CSW==1B32			; Current State WILL
   MO%SBR==1B31			; Subnegotiation Byte Received & Available
   MO%SBS==1B30			; Subnegotiation Byte Sent (Not used)

.MOGSB==:61			;Receive Telnet subnegotiation
.MOSSB==:62			;Send TELNET subnegotiation
   ; For .MOGSB and .MOSSB, AC3 points to a block with format:
   .MOSBO==0			; Timeout,,Option
      MO%TMO==777777B17		; Timeout in seconds
      ; Also MO%OPT as defined above
   .MOSBP==1			; Source/Destination designator
   .MOSBN==2			; Number of bytes to transfer


;NET MTOPR NUMBERS

***** CHANGE #8; PAGE 42, LINE 2; PAGE 42, LINE 2


;ODTNC -- SEE IDCNV FOR BITS


;OPEN - TCP connection establishment

;AC1 LH flags:
TCP%JS==1B0	; JCN Supplied
TCP%WT==1B1	; Wait for completion
TCP%FS==1B5	; Force Synchronization (active open)
TCP%PS==1B6	; Persistent open
TCP%ST==1B7	; Return statistics (STAT)
TCP%SY==1B8	; Symbolic name list given (STAT)
TCP%SC==1B8	; Secure connection (OPEN, SEND)
TCP%HP==1B9	; High priority (OPEN, SEND)
TCP%VT==1B10	; TCP Virtual Terminal (OPEN)
TCP%TV==1B11	; TVT Supplied (STAT)
TCP%NT==1B12	; Return AOBJN pointer over TVTs (STAT)
TCP%IX==1B13	; Connection index supplied (STAT)
TCP%NI==1B14	; Return AOBJN pointer over connections (STAT)
TCP%SD==1B15	; Return STAT definitions instead of values (STAT)
TCP%ET==1B16	; ERROR info flag
TCP%PT==1B17	; Packet trace flag

; The connection block format is

.TCPLH==0	; Connection Local Host (0 is wild)
.TCPLP==1	; Connection Local Port (0 is illegal)
.TCPFH==2	; Connection Foreign Host (0 is wild)
.TCPFP==3	; Connection Foreign Port (0 is wild)
.TCPOP==4	; Connection Option addresses
 .TCPOW==^D10	; Connection Option address word count
.TCPIP==5	; Connection IP parameters (Flag=3B1, TTL=377B17, TOS=377B35)
.TCPCS==6	; Connection block word size

 ---------------------------------


;ODTNC -- SEE IDCNV FOR BITS


;OPEN/SEND/RECV/STAT - TCP connection

;AC1 LH flags for BBN TCP JSYSes:

;TCP%JS==1B0	; JCN Supplied in RH instead of connection block adr
		; This bit must be zero for OPEN
TCP%WT==1B1	; Wait for completion
TCP%IC==1B2	; Word .TCPIC of CDB contains Interrupt Channel word (OPEN)
TCP%RX==1B3	; Don't kill connection on RTX timeout (OPEN)
TCP%FR==1B4	; Flush RECV data after close (OPEN,CLOSE)
TCP%FS==1B5	; Force Synchronization (active OPEN)
TCP%PS==1B6	; Persistent open (retry on error) (OPEN)
;TCP%ST==1B7	; Return statistics (STAT)
;TCP%SY==1B8	; Symbolic name list given (STAT)
TCP%SC==1B8	; Secure connection (OPEN, SEND) (OBSOLETE)
TCP%HP==1B9	; High priority (OPEN, SEND) (OBSOLETE)
;TCP%SL==1B9	; Supply vector lengths with symbolic names (STAT)
TCP%VT==1B10	; TCP Virtual Terminal (OPEN)
;TCP%TV==1B11	; TVT Supplied (STAT)
;TCP%NT==1B12	; Return AOBJN pointer over TVTs (STAT)
;TCP%IX==1B13	; Connection index supplied (STAT)
;TCP%NI==1B14	; Return AOBJN pointer over connections (STAT)
;TCP%SD==1B15	; Return STAT definitions instead of values (STAT)
TCP%ET==1B16	; ERROR info flag
TCP%PT==1B17	; Packet trace flag

;The connection block format is

.TCPLH==^D0	; Connection Local Host (0=wild) (internet format), bits 5-35
.TCPLP==^D1	; Connection Local Port (0=illegal), bits 20-35
.TCPFH==^D2	; Connection Foreign Host (0=wild) (internet format), bits 5-35
.TCPFP==^D3	; Connection Foreign Port (0=wild), bits 20-35
.TCPOP==^D4	; Connection Option addresses
		; LH - Address of IP options, or 0 if none
		; RH - Address of TCP options, or 0 if none
 .TCPOW==^D10	; Connection Option address word count, read with a POINT 8,adr
.TCPIP==^D5	; Connection IP parameters (Flag=3B1, TTL=377B17, TOS=377B35)
.TCPIC==^D6	; Connection Interrupt Channels for TCP events
.TCPCS==^D7	; Connection block word size

;The buffer header block format is

.TCPBF==^D0			;Buffer Flags, RH for use by user
TCP%ER==1B0			;ERROR
TCP%LE==1B1			;Local error flag (0 is remote)
TCP%PE==1B2			;Permanent error (0 is temporary)
TCP%EC==37B7			;Error code w/o flags
TCP%DN==1B12			;DONE
TCP%UR==1B15			;URGENT data
TCP%PU==1B16			;PUSH buffered data
TCP%WM==1B17			;WORD mode (unimplemented)
.TCPBA==^D1			;Buffer data address, data in bits 0-31
.TCPBC==^D2			;Buffer data count
				;Octets to send, octets unsent (SEND)
				;Octets available, octets unused (RECV)
.TCPBO==^D3			;Buffer option addresses
 .TCPOW==^D10			;Buffer option address word count
.TCPBI==^D4			;Buffer IP info
.TCPBS==^D5			;Buffer header size


***** CHANGE #9; PAGE 48, LINE 51; PAGE 48, LINE 51
.SFRDU==:55			;READ DATE UPDATE FUNCTION
.SFACY==:56			;SET MAX ARCHIVE EXP PERIOD
.SFRTW==:57			;SET [NO] RETRIEVAL WAITS NON-0 => NO WAIT
.SFTDF==:60			;TAPE MOUNT CONTROLS
  MT%UUT==1B0			;UNLOAD UNREADABLE TAPES
.SFWSP==:61			;WORKING SET PRELOADING
.SFSEA==:72			;SET ETHERNET ADDRESS
 ---------------------------------
.SFRDU==:55			;READ DATE UPDATE FUNCTION
.SFACY==:56			;SET MAX ARCHIVE EXP PERIOD
.SFRTW==:57			;SET [NO] RETRIEVAL WAITS NON-0 => NO WAIT
.SFTDF==:60			;TAPE MOUNT CONTROLS
  MT%UUT==1B0			;UNLOAD UNREADABLE TAPES
.SFWSP==:61			;WORKING SET PRELOADING
.SFDLA==:62			;CONTROL DOUBLE LOGIN AVOIDANCE (DLA)
.SFSEA==:72			;SET ETHERNET ADDRESS

***** CHANGE #10; PAGE 50, LINE 25; PAGE 50, LINE 25
.SWRLT==:3			;READ LUUO ADDRESS
	.ARPFL==:0		;OFFSET IN TRAP BLOCK FOR PC FLAGS
	.AROPC==:1		;OFFSET FOR OLD PC VALUE
	.AREFA==:2		;OFFSET FOR E
	.ARNPC==:3		;OFFSET FOR NEW PC WORD

;TBLUK
 ---------------------------------
.SWRLT==:3			;READ LUUO ADDRESS
	.ARPFL==:0		;OFFSET IN TRAP BLOCK FOR PC FLAGS
	.AROPC==:1		;OFFSET FOR OLD PC VALUE
	.AREFA==:2		;OFFSET FOR E
	.ARNPC==:3		;OFFSET FOR NEW PC WORD

;TACLG% - TAC LOGIN JSYS (Implements Double Login Avoidance (DLA))

; FUNCTION CODES FOR THE TACLG% JSYS
.TAGIU==:0			;GET THE IUID FOR THE CURRENT JOB
.TACIU==:1			;CONVERT AN IUID TO A USERNAME
.TACUI==:2			;CONVERT A USERNAME TO AN IUID
.TARRI==:3			;RETREIVE A USER RECORD GIVEN AN IUID
.TARRU==:4			;RETREIVE A USER RECORD GIVEN A USERNAME
.TALGI==:5			;LOGIN A TOPS-20 JOB
.TAIST==:6			;INSTALL SYSTEM TRUSTED HOSTS BINARY FILE
.TAIUI==:7			;INSTALL A USER IUID BINARY FILE
.TAVHI==:10			;VALIDATE HOST ADDRESS GIVEN IUID
.TAVHU==:11			;VALIDATE HOST ADDRESS GIVEN USERNAME

; OFFSET INTO USER RECORD DESTINATION BLOCK
.TARIU==:0			;IUID
.TARUN==:1			;BYTE POINTER TO USERNAME
.TARHM==:2			;HOST MASK
.TARAT==:3			;ADDRESS OF USER TRUSTED HOSTS TABLE
.TARNT==:4			;NUMBER OF TABLE ENTRIES
  TA%RAC==:777777B17		;ACTUAL NUMBER OF ENTRIES
  TA%RAV==:777777B35		;AVAILABLE NUMBER OF ENTRIES
.TARSZ==:5			;SIZE IN WORDS OF USER RECORD DESTINATION BLOCK

; FORMAT OF SYSTEM/USER TRUSTED HOSTS ADDRESSES
TA%TFL==:17B3			;FLAGS IN BITS 0-3
TA%THA==:37777777777B35		;TRUSTED HOST ADDRESS IN BITS 4-35

MSKSTR TATFL,,TA%TFL		;FLAGS STRUCTURE
MSKSTR TATHA,,TA%THA		;HOST ADDRESS STRUCTURE

; FLAGS FOUND IN USER TRUSTED HOST ADDRESSES (TA%TFL)
TA%PWD==:1B0			;PASSWORD REQUIRED FROM THIS HOST ADDRESS

; TMON%/SMON% SYMBOLS FOR FUNCTION .SFDLA
; DEFINED FLAG BITS
TA%ENA==:1B0			;SET = DLA IS ENABLED
				;CLEAR = DLA IS DISABLED
TA%USP==:1B1			;SET = ALWAYS REQUIRE PASSWORDS FOR ALL USERS
				;CLEAR = REQUIRE PASSWORDS FOR ALL USERS ON A
				; PER USER BASIS
TA%WOP==:1B2			;SET = ALWAYS REQUIRE PASSWORDS FOR WOPRS
				;CLEAR = REQUIRE PASSWORDS FOR WOPRS ON A PER
				; USER BASIS

;TBLUK

***** CHANGE #11; PAGE 60, LINE 24; PAGE 60, LINE 24
HS%STY==77B26			;SYSTEM TYPE MASK
HS%NEW==1B27			;HOST DOES NEW PROTOCOL
HS%NAM==1B28			;HOST HAS NAME
HS%SLF==1B29			; Host is one of my alias'
HS%NET==1B30			;HOST IS REALLY A NETWORK NAME
HS%GAT==1B31			;HOST IS REALLY A GATEWAY

 ---------------------------------
HS%STY==77B26			;SYSTEM TYPE MASK
HS%NEW==1B27			;HOST DOES NEW PROTOCOL
HS%NAM==1B28			;HOST HAS NAME
HS%SLF==1B29			; Host is one of my alias'
HS%NET==1B30			;HOST IS REALLY A NETWORK NAME
HS%GAT==1B31			;HOST IS REALLY A GATEWAY
HS%PRM==1B32			;Permanent (cache) table entry


***** CHANGE #12; PAGE 60, LINE 81; PAGE 60, LINE 82
DEFTCO .TCSSR,16		;SET THE STRICT ROUTE
DEFTCO .TCRLB,17		;READ LOWER BOUND FOR RETRANSMISSION
DEFTCO .TCSLB,20		;SET UPPER BOUND FOR RETRANSMISSION
DEFTCO .TCRUB,21		;READ UPPER BOUND FOR RETRANSMISSION
DEFTCO .TCSUB,22		;SET UPPER BOUND FOR RETRANSMISSION
DEFTCO .TCSFN,23		;SEND A FIN WITHOUT AN ABORT%

 ---------------------------------
DEFTCO .TCSSR,16		;SET THE STRICT ROUTE
DEFTCO .TCRLB,17		;READ LOWER BOUND FOR RETRANSMISSION
DEFTCO .TCSLB,20		;SET UPPER BOUND FOR RETRANSMISSION
DEFTCO .TCRUB,21		;READ UPPER BOUND FOR RETRANSMISSION
DEFTCO .TCSUB,22		;SET UPPER BOUND FOR RETRANSMISSION
DEFTCO .TCSFN,23		;SEND A FIN WITHOUT AN ABORT%
DEFTCO .TCRTC,24		;READ BLOCK OF TCP INFORMATION


***** CHANGE #13; PAGE 60, LINE 97; PAGE 60, LINE 99
TC%TSC==000077,,000000		;State change channel
TC%TXX==000000,,777777		;Unused (must be 77) fields

;Connection States (from GDSTS)

.TCNOT==0			;not synch'ed
.TCFIN==2			;fin sent or received
 ---------------------------------
TC%TSC==000077,,000000		;State change channel
TC%TXX==000000,,777777		;Unused (must be 77) fields

;Connection States (from GDSTS)

.TCNOT==0			;not synch'ed
.TCTIM==1			;time-wait
.TCFIN==2			;fin sent or received

***** CHANGE #14; PAGE 60, LINE 123; PAGE 60, LINE 126
DEFIPO .IPNIP,6			;IP ON THE NI - ENABLE/DISABLE NI IP PORTAL
DEFIPO .IPNAP,7			;IP ON THE NI - ENABLE/DISABLE NI ARP PORTAL
DEFIPO .IPIGH,10		;IP ON THE NI - RELOAD NI IP GHT
DEFIPO .IPRGH,11		;IP ON THE NI - RETURN NI IP GHT TABLE
DEFIPO .IPRIC,12		;IP ON THE NI - RETURN NI IP PORTAL COUNTERS
DEFIPO .IPRAC,13		;IP ON THE NI - RETURN NI ARP PORTAL COUNTERS

 ---------------------------------
DEFIPO .IPNIP,6			;IP ON THE NI - ENABLE/DISABLE NI IP PORTAL
DEFIPO .IPNAP,7			;IP ON THE NI - ENABLE/DISABLE NI ARP PORTAL
DEFIPO .IPIGH,10		;IP ON THE NI - RELOAD NI IP GHT
DEFIPO .IPRGH,11		;IP ON THE NI - RETURN NI IP GHT TABLE
DEFIPO .IPRIC,12		;IP ON THE NI - RETURN NI IP PORTAL COUNTERS
DEFIPO .IPRAC,13		;IP ON THE NI - RETURN NI ARP PORTAL COUNTERS
DEFIPO .IPNIF,14		;INITIALIZE ARPANET LOCAL NETWORK INTERFACES
DEFIPO .IPRIP,15		;READ A BLOCK OF IP INFORMATION
;SEE FUNCTION .TCRTC OF TCOPR%
DEFIPO .IPCKS,16		;Compute IP-flavor 16-bit checksum
DEFIPO .IPDOM,17		;Initialize domain database


; Network JSYSes related symbols

; Offsets into the data blocks for the .TCRTC function of TCOPR% and
;  the .IPRIP function of IPOPR%
.NTLEN==0			;LENGTH OF ARGUMENT BLOCK, INCLUDING SELF
.NTFLG==1			;FLAG WORD[+CONNECTION ID(IPOPR%)](TCOPR%)
  NT%JS==1B0  ; JCN Supplied in RH
  NT%ST==1B7  ; Return statistics (IPOPR%,STAT%)
  NT%SY==1B8  ; Symbolic name list given (TCOPR%,IPOPR%,STAT%)
  NT%SL==1B9  ; Supply vector lengths with symbolic names (TCOPR%,IPOPR%,STAT%)
  NT%TV==1B11 ; TVT Supplied (IPOPR%,STAT%)
  NT%NT==1B12 ; Return AOBJN counter for TVTs (IPOPR%,STAT%)
  NT%IX==1B13 ; Connection index supplied (IPOPR%,STAT%)
  NT%NI==1B14 ; Return AOBJN counter for TCP connections (IPOPR%,STAT%)
  NT%SD==1B15 ; Return STAT definitions instead of values (TCOPR%,IPOPR%,STAT%)
	      ; Following are only available via IPOPR%
  NT%NQ==1B16 ; Return AOBJN counter for User Queues (IPOPR%)
  NT%IQ==1B17 ; Return information about specified User Queue (IPOPR%)
.NTNMP==2			;SOURCE AOBJN COUNTER
.NTDTP==3			;DESTINATION AOBJN COUNTER
.NTLNP==4			;OPTIONAL LENGTH AOBJN COUNTER

; Old symbol names for the above
.TCLEN==.NTLEN
.TCFLG==.NTFLG
.TCNMP==.NTNMP
.TCDTP==.NTDTP
.TCLNP==.NTLNP

TCP%JS==NT%JS
TCP%SY==NT%SY
TCP%SL==NT%SL
TCP%SD==NT%SD
TCP%ST==NT%ST
TCP%TV==NT%TV
TCP%NT==NT%NT
TCP%IX==NT%IX
TCP%NI==NT%NI
TCP%NQ==NT%NQ
TCP%IQ==NT%IQ


***** CHANGE #15; PAGE 63, LINE 236; PAGE 64, LINE 236
.ERR (660,FLOTX1,<Column overflow in field 1 or 2>)
.ERR (661,FLOTX2,<Column overflow in field 3>)
.ERR (662,FLOTX3,<Invalid format specified>)
.ERR (670,HPTX1,<Undefined clock number>)
.ERR (700,FDFRX1,<Not a multiple-directory device>)
.ERR (701,FDFRX2,<Invalid directory number>)
.ERR (703,GTHSX1,<Invalid function code for GTHST>)	;TOPS20AN
.ERR (704,GTHSX2,<Unknown host number>) ;TOPS20AN
.ERR (705,GTHSX3,<No number for that host name>) ;TOPS20AN
.ERR (706,GTHSX4,<Index out of range>)	;TOPS20AN
.ERR (707,GTHSX5,<No string for that Host number>) ;TOPS20AN
.ERR (710,ATNX1,<Invalid receive JFN>) ;TOPS20AN
 ---------------------------------
.ERR (660,FLOTX1,<Column overflow in field 1 or 2>)
.ERR (661,FLOTX2,<Column overflow in field 3>)
.ERR (662,FLOTX3,<Invalid format specified>)
.ERR (670,HPTX1,<Undefined clock number>)
.ERR (700,FDFRX1,<Not a multiple-directory device>)
.ERR (701,FDFRX2,<Invalid directory number>)
.ERR (703,GTHSX4,<Invalid network number>)	;TOPS20AN
.ERR (704,GTHSX1,<Unknown host number>) ;TOPS20AN
.ERR (705,GTHSX2,<No number for that host name>) ;TOPS20AN
.ERR (706,GTHSX5,<Route not known - would try gateway>)	;TOPS20AN
.ERR (707,GTHSX3,<No string for that host number>) ;TOPS20AN
.ERR (710,ATNX1,<Invalid receive JFN>) ;TOPS20AN

***** CHANGE #16; PAGE 63, LINE 268; PAGE 64, LINE 268
.ERR (736,SNDIX5,<IMP down>) ;TOPS20AN
.ERR (737,NTWZX1,<NET WIZARD capability  required>) ;TOPS20AN
.ERR (740,ASNSX1,<Insufficient system resources (All special queues in use)>) ;TOPS20AN
.ERR (741,ASNSX2,<Link(s) assigned to another special queue>) ;TOPS20AN
.ERR (742,SQX1,<Special network queue handle out of range>) ;TOPS20AN
.ERR (743,SQX2,<Special network queue not assigned>) ;TOPS20AN
.ERR (746,GTNCX1,<Invalid network JFN>) ;TOPS20AN
 ---------------------------------
.ERR (736,SNDIX5,<IMP down>) ;TOPS20AN
.ERR (737,NTWZX1,<NET WIZARD capability  required>) ;TOPS20AN
.ERR (740,ASNSX1,<Insufficient system resources (All special queues in use)>) ;TOPS20AN
.ERR (741,ASNSX2,<Link(s) assigned to another special queue>) ;TOPS20AN
.ERR (742,SQX1,<Special network queue handle out of range>) ;TOPS20AN
.ERR (743,SQX2,<Special network queue not assigned>) ;TOPS20AN
.ERR (744,ASNSX3,<Insufficient capabilities to assign system reserved port numbers>) ;TOPS20AN
;;.ERR (744,GTNCX3,<Invalid function code>)	;TOPS20AN
;;.ERR (745,GTNCX4,<Invalid NCP connection index>)	;TOPS20AN
.ERR (746,GTNCX1,<Invalid network JFN>) ;TOPS20AN

***** CHANGE #17; PAGE 63, LINE 996; PAGE 64, LINE 999
.ERR (2467,BOTX19,<Overdue TO-11 transfer aborted>)
.ERR (2470,BOTX20,<Overdue T0-10 transfer aborted>)
.ERR (2471,ILLX05,<Illegal memory reference, section greater than 37>)
.ERR (2472,XSEVX1,<Illegal entry vector type>)
.ERR (2473,XSEVX2,<Invalid entry vector length>)
.ERR (2474,XSEVX3,<Cannot get extended values with this monitor call>)
.ERR (3033,MSTX43,<Illegal to dismount structure during initialization>)
 ---------------------------------
.ERR (2467,BOTX19,<Overdue TO-11 transfer aborted>)
.ERR (2470,BOTX20,<Overdue T0-10 transfer aborted>)
.ERR (2471,ILLX05,<Illegal memory reference, section greater than 37>)
.ERR (2472,XSEVX1,<Illegal entry vector type>)
.ERR (2473,XSEVX2,<Invalid entry vector length>)
.ERR (2474,XSEVX3,<Cannot get extended values with this monitor call>)

;GTDOM/GTHST

.ERR (2500,GTDX1,<Invalid domain specification>)
.ERR (2501,GTDX2,<Referenced domain name does not exist>)
.ERR (2502,GTDX3,<Requested domain data not present at name>)
.ERR (2503,GTDX4,<Requested domain data not available>)
.ERR (2504,GTDX5,<Bad domain output specification>)
.ERR (2505,GTDX6,<Domain system error>)

.ERR (2776,GTMX01,<User address must be on a page boundary>)
.ERR (2777,GTMX02,<SPT index error>)

; [BBN] Job-Based Scheduling Errors
.ERR (3000,ARGX99,<No more job slots available in this priority level>)
.ERR (3001,ARGX98,<Invalid job limit>)
.ERR (3002,ARGX97,<Invalid service type>)
.ERR (3003,ARGX96,<Invalid job cap>)
.ERR (3004,ARGX95,<Invalid job overflow pointer>)
; [BBN]

.ERR (3033,MSTX43,<Illegal to dismount structure during initialization>)

***** CHANGE #18; PAGE 63, LINE 1005; PAGE 64, LINE 1029
; DEC TCP JSYS Interface Errors

.ERR (3400,TCPXX1,<No IP free space for TCB>)
.ERR (3401,TCPXX2,<Unable to decode local side TCP of specification>)
.ERR (3402,TCPXX3,<Unable to decode foreign side TCP of specification>)
.ERR (3403,TCPXX4,<Generation found in TCP specification>)
.ERR (3404,TCPXX5,<TCP specification atrribute not known to TCP>)
.ERR (3405,TCPXX6,<Unable to decode CONNECTION attribute in TCP specification>)
 ---------------------------------
; DEC TCP JSYS Interface Errors

.ERR (3400,TCPXX1,<No IP free space for TCB>)
.ERR (3401,TCPXX2,<Unable to decode local side TCP of specification>)
.ERR (3402,TCPXX3,<Unable to decode foreign side TCP of specification>)
.ERR (3403,TCPXX4,<Generation found in TCP specification>)
.ERR (3404,TCPXX5,<TCP specification attribute not known to TCP>)
.ERR (3405,TCPXX6,<Unable to decode CONNECTION attribute in TCP specification>)

***** CHANGE #19; PAGE 63, LINE 1020; PAGE 64, LINE 1044
.ERR (3414,TCPX13,<Unable to decode COMPARTMENTS attribute in TCP specification>)
.ERR (3415,TCPX14,<Unable to decode HANDLING-RESTRICTIONS attribute in TCP specification>)
.ERR (3416,TCPX15,<Unable to decode TRANSMISSION-CONTROL attribute in TCP specification>)
.ERR (3417,TCPX16,<TCP not initialized and available>)
.ERR (3420,TCPX17,<Illegal IO mode for TCP device>)
.ERR (3421,TCPX18,<Illegal byte size for TCP device>)
.ERR (3422,TCPX19,<TCP connection allready exists>)
.ERR (3423,TCPX20,<Maximum TCP connections exceeded>)
 ---------------------------------
.ERR (3414,TCPX13,<Unable to decode COMPARTMENTS attribute in TCP specification>)
.ERR (3415,TCPX14,<Unable to decode HANDLING-RESTRICTIONS attribute in TCP specification>)
.ERR (3416,TCPX15,<Unable to decode TRANSMISSION-CONTROL attribute in TCP specification>)
.ERR (3417,TCPX16,<TCP not initialized and available>)
.ERR (3420,TCPX17,<Illegal IO mode for TCP device>)
.ERR (3421,TCPX18,<Illegal byte size for TCP device>)
.ERR (3422,TCPX19,<TCP connection already exists>)
.ERR (3423,TCPX20,<Maximum TCP connections exceeded>)

***** CHANGE #20; PAGE 63, LINE 1034; PAGE 64, LINE 1058
.ERR (3431,TCPX26,<Illegal Persist parameters>)
.ERR (3432,TCPX27,<Illegal TCOPR Function on an OPEN TCP JFN>)
.ERR (3433,TCPX28,<Invalid BBN TCP JSYS call>)
.ERR (3434,TCPX29,<Assigned JFN too large for TCPJFN>)
.ERR (3435,TCPX30,<Illegal TCP IO mode>)
.ERR (3436,TCPX31,<Connection error or connection rejected>)
.ERR (3437,TCPX32,<Retranmission timeout>)
.ERR (3440,TCPX33,<Connection closed or closing>)
.ERR (3441,TCPX34,<TCOPR Argument>)
 ---------------------------------
.ERR (3431,TCPX26,<Illegal Persist parameters>)
.ERR (3432,TCPX27,<Illegal TCOPR Function on an OPEN TCP JFN>)
.ERR (3433,TCPX28,<Invalid BBN TCP JSYS call>)
.ERR (3434,TCPX29,<Assigned JFN too large for TCPJFN>)
.ERR (3435,TCPX30,<Illegal TCP IO mode>)
.ERR (3436,TCPX31,<Connection error or connection rejected>)
.ERR (3437,TCPX32,<Tranmission timeout>)
.ERR (3440,TCPX33,<Connection closed>)
.ERR (3441,TCPX34,<TCOPR Argument>)

***** CHANGE #21; PAGE 63, LINE 1058; PAGE 64, LINE 1082
.ERR (3455,IPHSEQ,<GHT Internet host numbers not in ascending order>)
.ERR (3456,IPFLAD,<Local Internet host number not in GHT>)
.ERR (3457,ARPNSP,<Insufficient system resources (No space for ARP buffers>)
.ERR (3460,IPARP1,<Cannot start ARP until TCPNI service is running>)
.ERR (3461,TCPX44,<Monitor does not support TCP over Ethernet>)

>				;END OF .ERCOD DEFINITION
 ---------------------------------
.ERR (3455,IPHSEQ,<GHT Internet host numbers not in ascending order>)
.ERR (3456,IPFLAD,<Local Internet host number not in GHT>)
.ERR (3457,ARPNSP,<Insufficient system resources (No space for ARP buffers>)
.ERR (3460,IPARP1,<Cannot start ARP until TCPNI service is running>)
.ERR (3461,TCPX44,<Monitor does not support TCP over Ethernet>)

;More general TCP/IP error codes

.ERR (3462,TCPX45,<Connection closing>)
.ERR (3463,TCPX46,<Connection aborted>)
.ERR (3464,TCPX47,<Error in option>)

; Warning: The MNTXxx codes must have contiquous values

.ERR (3500,MNTX00,<No errors detected>)
.ERR (3501,MNTX01,<ADRINI failed to find SYSTEM:SITE-ADDRESS.TXT file>)
.ERR (3502,MNTX02,<Datagram too long for specified local network>)
.ERR (3503,MNTX03,<Error in protocol header>)
.ERR (3504,MNTX04,<Flow control restriction>)
.ERR (3505,MNTX05,<GWYINI failed to find SYSTEM:INTERNET.GATEWAYS file>)
.ERR (3506,MNTX06,<Host down>)
.ERR (3507,MNTX07,<Host unknown>)
.ERR (3510,MNTX10,<HSTINI failed to find SYSTEM:HOSTS.TXT file>)
.ERR (3511,MNTX11,<Interface to local network down>)
.ERR (3512,MNTX12,<Invalid argument>)
.ERR (3513,MNTX13,<Invalid host or network specified>)
.ERR (3514,MNTX14,<Invalid parameter or function bit combination>)
.ERR (3515,MNTX15,<Invalid value>)
.ERR (3516,MNTX16,<Network down>)
.ERR (3517,MNTX17,<Protocol not available>)
.ERR (3520,MNTX20,<Protocol not supported>)
.ERR (3521,MNTX21,<Protocol not supported by specified local network>)
.ERR (3522,MNTX22,<Protocol option error>)
.ERR (3523,MNTX23,<Requested service not available>)
.ERR (3524,MNTX24,<Resource shortage>)
.ERR (3525,MNTX25,<Route invalid or failed>)
.ERR (3526,MNTX26,<Route to host updated>)
.ERR (3527,MNTX27,<Route to network updated>)
.ERR (3530,MNTX30,<Table overflow>)
.ERR (3531,MNTX31,<Transmission aborted>)
.ERR (3532,MNTX32,<Transmission acknowledged>)
.ERR (3533,MNTX33,<Transmission timeout>)
.ERR (3534,MNTX34,<Unspecified network error>)
.ERR (3535,MNTX35,<DOMINI failed to intialize from DOMAIN:FLIP.DD and FLOP.DD>)
.ERR (3536,MNTX36,<>)

; TACLG JSYS ERROR CODES
.ERR (3600,TLGX01,<System Trusted Hosts File too large>)
.ERR (3601,TLGX02,<User IUID File too large>)
.ERR (3602,TLGX03,<Invalid format for System Trusted Hosts File>)
.ERR (3603,TLGX04,<Invalid format for User IUID File>)
.ERR (3604,TLGX05,<Version number mismatch for System Trusted Hosts File>)
.ERR (3605,TLGX06,<Version number mismatch for User IUID File>)
.ERR (3606,TLGX07,<Invalid IUID>)
.ERR (3607,TLGX08,<Invalid port number>)
.ERR (3610,TLGX09,<Unknown port number>)
.ERR (3611,TLGX10,<Unknown IUID>)
.ERR (3612,TLGX11,<Unknown Username>)
.ERR (3613,TLGX12,<Host not trusted by system>)
.ERR (3614,TLGX13,<Host not trusted by user>)
.ERR (3615,TLGX14,<Double Login Avoidance is disabled>)
.ERR (3616,TLGX15,<Password required for Double Login Avoidance>)
.ERR (3617,TLGX16,<IUID not available for this job>)
.ERR (3620,TLGX17,<System Trusted Hosts Table not initialized>)
.ERR (3621,TLGX18,<User IUID Table not initialized>)

>				;END OF .ERCOD DEFINITION

***** CHANGE #22; PAGE 64, LINE 64; PAGE 65, LINE 64

	END .ERGO

>				;END OF IFN .ERBLD CONDITIONAL

PURGE .ERR,REL

END

 ---------------------------------

	END .ERGO

>				;END OF IFN .ERBLD CONDITIONAL

PURGE .ERR,REL
IF2 <PURGE .ERCOD >

END

