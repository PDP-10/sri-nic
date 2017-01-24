REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:05:55
File 1: SRC:<7.MONITOR>DEVICE.MAC.1
File 2: SRC:<7.MONITOR.AP20>DEVICE.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8880 to DEVICE.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation. 

***** CHANGE #2; PAGE 3, LINE 96; PAGE 3, LINE 96

Cause:	The monitor could not assign a terminal to a job because
	    1.  It failed to get resident storage.
	    2.  The line is not fully active; it is okay for system messages
		and sendalls.  Need a CNTRL/C on line.
	    3.  Of a programming error.
>)
 ---------------------------------

Cause:	The monitor could not assign a terminal to a job because
	    1.  It failed to get resident storage.
	    2.  The line is not fully active; it is okay for system messages
		and sendalls.  Need a CNTRL/C on line.
	    3.  Or a programming error.
>)

***** CHANGE #3; PAGE 10, LINE 58; PAGE 10, LINE 58
	BUG.(CHK,DEVUCF,DEVICE,SOFT,<DEVAV - Unexpected CHKDES failure>,,<

Cause:	While checking to see if a device is available to the job,
	an invalid device designator was passed to a subroutine.


>)
 ---------------------------------
	BUG.(CHK,DEVUCF,DEVICE,SOFT,<DEVAV - Unexpected CHKDES failure>,,<

Cause:	While checking to see if a device is available to the job,
	an invalid device designator was passed to a subroutine.

Action:	If this problem persists, submit a dump provided by the DOB%
	facility or create a dump by changing this to a BUGHLT. Look
	at the dump and see who the caller is that is passing in bad
	information.
>)

