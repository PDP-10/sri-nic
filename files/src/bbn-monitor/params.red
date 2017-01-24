REDIT 1(104) COMPARE by user CLYNN, 13-Feb-86 21:25:33
File 1: CWL:<DEC-6-1-BASE>PARAMS.MAC.1
File 2: CWL:<5-4-NET>PARAMS.MAC.8
***** CHANGE #1; PAGE 1, LINE 184; PAGE 1, LINE 184
NDG	.STDFE,^D30*6		; Standard offline expiration
NDG	.STDAE,^D365*^10	; STANDARD ARCHIVE OFFLINE EXPIRATION


; Network parameters

NDG	NETN,0			;ARPANET INTERFACE EXIST
IFN NETN,<
NDG	NNTBFS,60000		;NETWORK BUFFER AREA
NDG	ANXN,1			;# of DEC AN10/20 interfaces
NDG	IPNIN,0			;# of KLNI IP interfaces

TCPVER==:4			; Want TCP4
NDG TCBHSZ,^D17			; Size of TCB Hash table
NDG NTCPWP,4			; Number of monitor window pages
NDG NTTTVT,^D32			; Default to 32 TCP Virtual Terminals
IFGE <^D50-<2*NTTTVT>>,<NDG MAXTCB,<NTTTVT*2>>
IFL <^D50-<2*NTTTVT>>,<NDG MAXTCB,<^D50>>
NDG MAXJCN,NTTTVT		; Each job allowed this many JCNs
NDG ABTCBS,^D35-^L<2*MAXJCN>	; Abort counter byte size
NDG NTACWD,<NFKS+<^D36/ABTCBS>>/ABTCBS	; Number of words for abort counters

NTWBTS==^D<MAXTCB*<8+2>+10>	; Appox. number of wait bits needed
IFG NTWBTS-^D511,<PRINTX % TOO MANY WAIT BITS
		NTWBTS==^D511	; Must fit in 9-bits for TCPOOT, TCPZOT,
				; and TOPNF and TERRF fields
	> ; End of IFG NTWBTS-^D511
NDG INTFSZ,20000		; Size of free area (if not separate section)
NDG MAXNET,^D64			; Maximum nets we will handle
NDG NFIXED,^D61			; Num fixed size blk lists (prime best)
NDG NIQ,^D32			; Number of Internet User Queues
NDG NTWBWD,<NTWBTS+1+31>/^D32	; Number of words of wait bits (from TCP)
> ; End of IFN NETN

IFE NETN,<			; If no TCP
NTWBTS==0			; No wait bits
 ---------------------------------
NDG	.STDFE,^D30*6		; Standard offline expiration
NDG	.STDAE,^D365*^10	; STANDARD ARCHIVE OFFLINE EXPIRATION


; Network parameters

NDG	NETN,0			;Default to no DARPA network support
IFN NETN,<
NDG ANXN,1			;# of DEC AN10/20 interfaces
NDG BBNN,0			;# IMP10s
NDG IPNIN,0			;# of KLNI IP interfaces
NDG NFEN,0			;# Network FE's

NDG NETHSZ,^D23			;Size of network hash tables

NDG INETN,1			; INET included
NDG NFIXED,^D11			; Num fixed size blk lists (prime best)
NDG NIQ,^D32			; Number of Internet User Queues

NDG TCBHSZ,^D17			; Size of TCB Hash table
NDG NTCPWP,4			; Number of monitor window pages
NDG NTTTVT,^D32			; Default to 32 TCP Virtual Terminals
IFL <^D50-<2*NTTTVT>>,<NDG MAXTCB,<NTTTVT*2>>
IFGE <^D50-<2*NTTTVT>>,<NDG MAXTCB,<^D50>>
NDG MAXJCN,NTTTVT		; Each job (Job0) allowed this many JCNs

> ; End of IFN NETN

IFE NETN,<			; If no DARPA networks
NTWBTS==0			; No wait bits

