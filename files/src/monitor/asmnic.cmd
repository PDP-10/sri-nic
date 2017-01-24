;  v7.0 assemble
;
COMPIL/NOBINARY SITE.MAC+MONSYM.MAC	;Monitor symbols
COMPIL SITE.MAC+REL1.MAC+MONSYM.MAC	;Monsym REL file
COMPIL MON:SYSFLG.MAC+MON:PROLOG.MAC R:PROLOG
COMPIL MON:GLOBS.MAC R:GLOBS		;Global symbols
COMPIL MON:NIPAR.MAC R:NIPAR		;Universal for NISRV users
COMPIL MON:ANAUNV.MAC R:ANAUNV		;Universal for ARPANET TCP/IP
COMPIL MON:PHYPAR.MAC R:PHYPAR		;Universal for PHYSIO and like modules
COMPIL MON:SCAPAR.MAC R:SCAPAR		;Universal for SCA
COMPIL MON:MSCPAR.MAC R:MSCPAR		;Universal for MSCP drivers and servers
COMPIL MON:D36PAR.MAC R:D36PAR		;DECnet symbols
COMPIL MON:SCPAR.MAC R:SCPAR		;Session control symbols
COMPIL MON:CTERMD.MAC R:CTERMD		;CTERM symbols
COMPIL MON:TTYDEF.MAC R:TTYDEF		;Teletype service symbols
COMPIL MON:SERCOD.MAC R:SERCOD		;Codes and fields for SYSERR facility
COMPIL MON:ENQPAR.MAC R:ENQPAR		;[7.1072] Universal for ENQ
COMPIL MON:CLUPAR.MAC R:CLUPAR		;[7.1076] CLUDGR parameters
COMPIL MON:CFSPAR.MAC R:CFSPAR		;[7.1190] CFS parameters
!
! Local Universal libraries
!
;COMPIL MON:PUPSYM.MAC R:PUPSYM          ;PUP symbols
COMPIL MON:DOMSYM.MAC R:DOMSYM		;GTDOM% symbols
!
COMPIL MON:SCHED.MAC R:SCHED		;Scheduler
COMPIL MON:PAGEM.MAC R:PAGEM		;Page management/working set management
COMPIL MON:PAGUTL.MAC R:PAGUTL		;Page management subroutines/utilities
COMPIL MON:TAPE.MAC R:TAPE		;Label handler and record processor
COMPIL MON:COMND.MAC R:COMND		;Command scanner JSYS
COMPIL MON:CRYPT.MAC R:CRYPT		;Encryption routines
COMPIL MON:DATIME.MAC R:DATIME		;Time and date routines
COMPIL MON:DEVICE.MAC R:DEVICE		;Device table (DEVTAB) routines
COMPIL MON:DIRECT.MAC R:DIRECT		;Disk directory management
COMPIL MON:DISC.MAC R:DISC		;Disk file management
COMPIL MON:ENQ.MAC R:ENQ		;ENQ and DEQ JSYS
COMPIL MON:FESRV.MAC R:FESRV		;Device code for FE devices
COMPIL MON:FILINI.MAC R:FILINI		;File system initialization
COMPIL MON:FILMSC.MAC R:FILMSC		;DTBs, PTY support
COMPIL MON:FORK.MAC R:FORK		;Fork controlling JSYS and functions
COMPIL MON:FREE.MAC R:FREE		;Storage routines
COMPIL MON:FUTILI.MAC R:FUTILI		;Miscellaneous routines for file system
COMPIL MON:GETSAV.MAC R:GETSAV		;Get and save routines
COMPIL MON:GTJFN.MAC R:GTJFN		;GTJFN and GNJFN JSYS
COMPIL MON:IO.MAC R:IO			;Sequential IO routines and JSYSes
COMPIL MON:IPCF.MAC R:IPCF		;Interprocess communications facility
COMPIL MON:JSYSA.MAC R:JSYSA		;Non-file system JSYSes
COMPIL MON:JSYSF.MAC R:JSYSF		;File system JSYSes
COMPIL MON:JSYSM.MAC R:JSYSM		;[7.1200] JSYSes and support for MEXEC
COMPIL MON:LDINIT.MAC R:LDINIT		;Define JSYS dispatch table, mon vers.
COMPIL MON:LINEPR.MAC+MON:LPFEDV.MAC R:LINEPR	;Line printer device routine
COMPIL MON:LOGNAM.MAC R:LOGNAM		;Logical name JSYSes and support
COMPIL MON:LOOKUP.MAC R:LOOKUP		;File name lookup utilities (for GTJFN)
COMPIL MON:MFLIN.MAC R:MFLIN		;Floating point input routines
COMPIL MON:MFLOUT.MAC R:MFLOUT		;Floating point outputl routines
COMPIL MON:POSTLD.MAC R:POSTLD		;Post-loading one-shot init
COMPIL MON:SWPALC.MAC R:SWPALC		;Swapping space allocation
COMPIL MON:SYSERR.MAC R:SYSERR		;SPEAR routines
COMPIL MON:TIMER.MAC R:TIMER		;TIMER JSYS & schedular clock routines
COMPIL MON:SCAMPI.MAC R:SCAMPI		;Systems communications architecture
COMPIL MON:CFSSRV.MAC R:CFSSRV		;Common File System
COMPIL MON:CFSUSR.MAC R:CFSUSR		;[7.1190] CFS user related stuff
COMPIL MON:PHYKLP.MAC R:PHYKLP		;Device dependent code for KLIPA port
COMPIL MON:SCSJSY.MAC R:SCSJSY		;The SCS% JSYS
COMPIL MON:PHYMSC.MAC R:PHYMSC		;MSCP driver
COMPIL MON:PHYMVR.MAC R:PHYMVR		;MSCP Server
COMPIL MON:APRSRV.MAC R:APRSRV		;Processor-dependent paging
COMPIL MON:DIAG.MAC R:DIAG		;The DIAG JSYS
COMPIL MON:DSKALC.MAC R:DSKALC		;Device independent disk code
COMPIL MON:PHYH2.MAC R:PHYH2		;Channel dependent code for RH20
COMPIL MON:PHYM2.MAC R:PHYM2		;Device dependent code for TM02/TU45
COMPIL MON:PHYM78.MAC R:PHYM78		;Device dependent code for TM78/TU78
COMPIL MON:PHYP4.MAC R:PHYP4		;Device dependent code for RP04 DISKS
COMPIL MON:PHYSIO.MAC R:PHYSIO		;Device independent physical IO
COMPIL MON:MAGTAP.MAC R:MAGTAP		;MTA routines
COMPIL MON:MEXEC.MAC R:MEXEC		;Swappable monitor routines
COMPIL MON:MSTR.MAC R:MSTR		;Mountable structure monitor call
COMPIL MON:DTESRV.MAC R:DTESRV		;DTE support. RSX20F interface
COMPIL MON:IPIPIP.MAC R:IPIPIP		;Arpanet internet protocols
COMPIL MON:IPCIDV.MAC R:IPCIDV		;Internet CI Driver
COMPIL MON:IPFREE.MAC R:IPFREE		;Internet free storage routines
COMPIL MON:TCPTCP.MAC R:TCPTCP		;Arpanet transmission control
COMPIL MON:TCPCRC.MAC R:TCPCRC		;Cyclic redundancy check routines
COMPIL MON:TCPBBN.MAC R:TCPBBN		;BBN TCP JSYS service routines
COMPIL MON:TCPJFN.MAC R:TCPJFN		;DEC JSYS interface for BBN TCP
COMPIL MON:MNETDV.MAC R:MNETDV		;Multinet software for ARPANET TCP/IP
COMPIL MON:TTYSRV.MAC R:TTYSRV		;Teletype service routines
COMPIL MON:RSXSRV.MAC R:RSXSRV		;Teletype service routines
COMPIL MON:TVTSRV.MAC R:TVTSRV		;Internet terminal service
COMPILE MON:CIDLL.MAC R:CIDLL		;CI data link layer
COMPILE MON:LATSRV.MAC R:LATSRV		;LAT host server
COMPILE MON:D36COM.MAC R:D36COM		;Common routines for DECnet
!
! DECnet and KLNI modules
!
!COMPILE MON:LLINKS.MAC R:LLINKS	;DECnet NSP (ECL) layer
!COMPILE MON:LLMOP.MAC R:LLMOP		;DECnet low level MOP support
COMPILE MON:TOPS.MAC+MON:NISRV.MAC+MON:PHYKNI.MAC R:NISRV ;KLNI device driver
COMPILE MON:NIUSR.MAC R:NIUSR		;NI% JSYS
;new
!COMPIL MON:NITEST.MAC R:NITEST		;NI test module (Temporary)
!COMPILE MON:NTMAN.MAC R:NTMAN		;Network management
!COMPILE MON:ROUTER.MAC R:ROUTER	;DECnet router layer
!COMPILE MON:SCJSYS.MAC R:SCJSYS	;DECnet JSYSes
!COMPILE MON:SCLINK.MAC R:SCLINK	;DECnet session control layer
!COMPILE MON:DNADLL.MAC R:DNADLL	;Common data link layer interface
!COMPILE MON:JNTMAN.MAC R:JNTMAN	;TOPS20 specific network management 
COMPIL MON:IPNIDV.MAC R:IPNIDV		;Internet Ethernet Driver
!COMPIL MON:FILNFT.MAC R:FILNFT		;DECNET DAP support
!COMPIL MON:NRTSRV.MAC R:NRTSRV		;NRT service routines
!COMPILE MON:CTHSRV.MAC R:CTHSRV	;CTERM terminal support
!
! Funky peripherals
!
!COMPIL MON:PHYX2.MAC R:PHYX2		;Device dependent code for DX20A/TU70S
!COMPIL MON:PHYP2.MAC R:PHYP2		;Device dependent code for DX20B/RP20
!COMPIL MON:CDPSRV.MAC R:CDPSRV		;Card punch service
!COMPIL MON:CDRSRV.MAC+MON:CDKLDV.MAC R:CDRSRV	;Card reader service
!COMPIL MON:PTP.MAC R:PTP		;Paper tape punch service
!COMPIL MON:PTR.MAC R:PTR		;Paper tape reader service
!COMPIL MON:PLT.MAC R:PLT		;Plotter service
!
! DDT modules
!
COMPIL MON:F2MDDT.MAC+MON:DDT.MAC R:MDDT        ;MDDT
COMPIL MON:F2KDDT.MAC+MON:DDT.MAC R:KDDT        ;KDDT
!
! Stanford monitor modules
!
COMPIL MON:GTDOM.MAC R:GTDOM		;GTDOM% support
!COMPIL MON:PHYMEI.MAC R:PHYMEI		;MEIS device driver
!COMPIL MON:ENET.MAC R:ENET		;Ethernet support of PUP and IP
!COMPIL MON:ARP.MAC R:ARP		;Address Resolution Protocol
!COMPIL MON:PKOPR.MAC R:PKOPR		;Network Dependent Packet Operations
!COMPIL MON:PUP.MAC R:PUP		;PUP protocol support
!COMPIL MON:PUPNM.MAC R:PUPNM		;More PUP protocol support
!COMPIL MON:PNVSRV.MAC R:PNVSRV		;PUP NVT support
COMPIL MON:PIPE.MAC R:PIPE		;UNIX-style pipe support (PIP: device)
!
! LOTS-only Modules
!
!COMPIL MON:NSPTOP.MAC R:NSPTOP         ;Decnet topology support
!COMPIL MON:GTWAA.MAC R:GTWAA           ;Allocation system
!
! AN20 Modules
!
COMPIL MON:IMPANX.MAC R:IMPANX         ;IMP driver for AN20
COMPIL MON:IMPDV.MAC R:IMPDV           ;Arpanet IMP Communication Protocols
!
;some new stuff -- needed??
COMPIL MON:CLUDGR.MAC R:CLUDGR		;[7.1076] CLUDGR SYSAP
COMPIL MON:CLUFRK.MAC R:CLUFRK		;[7.1076] CLUDGR's fork
;COMPIL MON:ENQSRV.MAC R:ENQSRV		;[7.1072] Cluster-wide ENQ/DEQ protocol
COMPIL MON:DOB.MAC R:DOB		;[7.1081] DOB JSYS and code
!
! Site specific storage modules
!
COMPIL MON:NAMNIC.MAC+MON:VEDIT.MAC+MON:VERSIO.MAC R:VERSIO
COMPIL MON:PARNIC.MAC+MON:PARNEW.MAC+MON:PARAMS.MAC+MON:STG.MAC R:STG
