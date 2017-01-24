REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:04:12
File 1: SRC:<7.MONITOR>CIDLL.MAC.1
File 2: SRC:<7.MONITOR.AP20>CIDLL.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8875 to CIDLL.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation again.

***** CHANGE #2; PAGE 24, LINE 45; PAGE 24, LINE 45
	This may happen if you are running DECnet/CI between two different
	versions of TOPS-20, or if you are running DECnet/CI between TOPS-20
	and a VMS system. DECnet/CI between TOPS-20 and VMS is not 
	supported.

Data:	REMVER - The remote ends protocol version
 ---------------------------------
	This may happen if you are running DECnet/CI between two different
	versions of TOPS-20, or if you are running DECnet/CI between TOPS-20
	and a VMS system. DECnet/CI between TOPS-20 and VMS is not 
	supported.

Action:	If this is caused by a VMS system, then remove the system from
	your cluster or disable the CI dircuit to that system. If it is
	caused by a TOPS-20 system, then you should make both systems
	run the same version of the software or disable the CI circuit
	between them.

Data:	REMVER - The remote ends protocol version

***** CHANGE #3; PAGE 25, LINE 58; PAGE 25, LINE 58
Cause:	The remote port does not understand the protocol version of DECnet/CI.
	The other system may be running another version of TOPS-20, or the
	remote system may be running VMS. DECnet/CI between TOPS-20
	and VMS is not supported.

Data:	LOCVER - TOPS-20 DECnet/CI protocol version
 ---------------------------------
Cause:	The remote port does not understand the protocol version of DECnet/CI.
	The other system may be running another version of TOPS-20, or the
	remote system may be running VMS. DECnet/CI between TOPS-20
	and VMS is not supported.

Action:	If this is caused by a VMS system, then remove the system from
	your cluster or disable the CI dircuit to that system. If it is
	caused by a TOPS-20 system, then you should make both systems
	run the same version of the software or disable the CI circuit
	between them.

Data:	LOCVER - TOPS-20 DECnet/CI protocol version

