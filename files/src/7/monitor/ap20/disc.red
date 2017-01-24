REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:06:23
File 1: SRC:<7.MONITOR>DISC.MAC.1
File 2: SRC:<7.MONITOR.AP20>DISC.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8886 to DISC.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.
; Edit= 8870 to DISC.MAC on 29-Jul-88 by LOMARTIRE, for SPR #21930
;Prevent OPNX9 when file goes long and PT0 access is incorrect 

***** CHANGE #2; PAGE 12, LINE 40; PAGE 12, LINE 40
	MOVX B,FB%TMP
	IORM B,.FBCTL(A)
	CALL USTDIR
	RET

DSKINB:	BUG.(CHK,GTFDB1,DISC,SOFT,<DSKINS - GETFDB FAILURE.>,,<

 ---------------------------------
	MOVX B,FB%TMP
	IORM B,.FBCTL(A)
	CALL USTDIR
	RET

DSKINB:	BUG.(CHK,GTFDB1,DISC,SOFT,<DSKINS - GETFDB failure.>,,<


***** CHANGE #3; PAGE 20, LINE 26; PAGE 20, LINE 26
	LOAD A,STGADR,0(B)	;GET PT 0 ADDRESS
	SKIPN A			;BE SURE THERE IS ONE
	BUG.(HLT,NOPGT0,DISC,SOFT,<OPNLNG - No page table 0 in long file.>,,<

Cause:	There is no page 0 for long file being opened.

>)
 ---------------------------------
	LOAD A,STGADR,0(B)	;GET PT 0 ADDRESS
	SKIPN A			;BE SURE THERE IS ONE
	BUG.(HLT,NOPGT0,DISC,SOFT,<OPNLNG - No page table 0 in long file.>,,<

Cause:	There is no page 0 for long file being opened.
>)

***** CHANGE #4; PAGE 25, LINE 5; PAGE 25, LINE 5

	IBP A			;GETS LOC OF FIRST WORD OF FILE
	XSFM T2			;Get previous context section
	TXNN T2,PCU		;User mode?
	 BUG.(CHK,DMPIOM,DISC,SOFT,<DSKDM - I/O disk dump mode I/O called from monitor>,,<

Cause:	DSKDMI or DSKDMO called and the previous context indicates
	an exec mode DUMPI% or DUMPO% JSYS.  There aren't any.

Action:	Look at the code because something is broken.  It is also possible 
	that some code has been changed to do dump mode I/O.

>)
 ---------------------------------

	IBP A			;GETS LOC OF FIRST WORD OF FILE
	XSFM T2			;Get previous context section
	TXNN T2,PCU		;User mode?
	BUG.(CHK,DMPIOM,DISC,SOFT,<DSKDM - I/O disk dump mode I/O called from monitor>,,<

Cause:	DSKDMI or DSKDMO called and the previous context indicates
	an exec mode DUMPI% or DUMPO% JSYS.  There aren't any.

Action:	If this becomes persistent, change this to a BUGHLT and submit
	an SPR. Look at the code because something is broken.
	It is also possible that some code has been changed to
	do dump mode I/O.
>)

***** CHANGE #5; PAGE 32, LINE 8; PAGE 32, LINE 8
	CALL UPDOFN		;WRITE IT TO DISK
	HLRZ A,FILOFN(JFN)	; GET OFN OF PT0
	HRLZ T2,FILOFN(JFN)	;GET XB2DAT (PTT OFN,,0)
	EXCH T2,SPTO4(T1) ;SAVE IT
	SKIPE T2		;OK. CHECK FOR STRANGE RACES
	 BUG.(HLT,LNGLNG,DISC,SOFT,<NEWLFP - File going long is already long>,,<

 ---------------------------------
	CALL UPDOFN		;WRITE IT TO DISK
	HLRZ A,FILOFN(JFN)	; GET OFN OF PT0
	HRLZ T2,FILOFN(JFN)	;GET XB2DAT (PTT OFN,,0)
	EXCH T2,SPTO4(T1) ;SAVE IT
	SKIPE T2		;OK. CHECK FOR STRANGE RACES
	BUG.(HLT,LNGLNG,DISC,SOFT,<NEWLFP - File going long is already long>,,<


***** CHANGE #6; PAGE 35, LINE 12; PAGE 35, LINE 12
	EXCH A,NLFT1		;GET DISK ADDRESS SAVE OFN
	MOVE B,NLFT2		;GET PTR ADDRESS
	IOR A,IMMPTR		;CONSTRUCT STANDARD PTR
	EXCH A,(B)		;CHECK FOR IMPOSSIBLE? RACES
	SKIPE A			;SHOULD BE NEW
	 BUG.(HLT,XTRAPT,DISC,SOFT,<NEWLFT - Extra page table in long file>,,<

Cause:	The monitor is attempting to create a new file section in a long
	file. This bughlt indicates that the page table slot in the super PT
	already contains a pointer to a second level PT. This indicates a race
 ---------------------------------
	EXCH A,NLFT1		;GET DISK ADDRESS SAVE OFN
	MOVE B,NLFT2		;GET PTR ADDRESS
	IOR A,IMMPTR		;CONSTRUCT STANDARD PTR
	EXCH A,(B)		;CHECK FOR IMPOSSIBLE? RACES
	SKIPE A			;SHOULD BE NEW
	BUG.(HLT,XTRAPT,DISC,SOFT,<NEWLFT - Extra page table in long file>,,<

Cause:	The monitor is attempting to create a new file section in a long
	file. This BUGHLT indicates that the page table slot in the super PT
	already contains a pointer to a second level PT. This indicates a race

***** CHANGE #7; PAGE 42, LINE 21; PAGE 42, LINE 21

OFJBAD:	BUG.(HLT,OFJFBD,DISC,SOFT,<OFNJFN - OFNJFN found bad data>,,<

Cause:	An OFN was found whose bits indicated that it was or was not a
	secondary index block. SPTO4 was found to disagree.
>)
 ---------------------------------

OFJBAD:	BUG.(HLT,OFJFBD,DISC,SOFT,<OFNJFN - OFNJFN found bad data>,,<

Cause:	An OFN was found whose bits indicated that it was or was not a
	secondary index block. SPTO4 was found to disagree.

Action:	One already discovered cause of this BUGHLT is the accidental
	clearing of the OFN2XB bit by CHKLAC. There may be other spots
	in the monitor where this bit is handled incorrectly.
>)

***** CHANGE #8; PAGE 51, LINE 3; PAGE 51, LINE 3
	AOBJN C,NFACK1		;LOOP TILL DONE
	RETSKP			;SUCCESS RETURN

;HERE TO CLOSE LONG FILE--FIRST COUNT PAGES

CNTLNG:	LOAD A,FLP0,(JFN)	;GET OFN OF PT0
	CALL RELOFN		;UNDO EXTRA COUNT
 ---------------------------------
	AOBJN C,NFACK1		;LOOP TILL DONE
	RETSKP			;SUCCESS RETURN

;HERE TO CLOSE LONG FILE--FIRST COUNT PAGES
;
;[8870] Note:  The  write  bit  (FILWB)  must be passed to RELOFN when PT0 is
;[8870] released to prevent OPNX9 errors from occurring. These can  occur  if
;[8870] the  file  once  was  short  and was opened for read. Then, an append
;[8870] occurs which makes the file go long and causes FILWB to be set in the
;[8870] PTT and PT0. When the append is over, and the OFN  is  being  closed,
;[8870] the  PTT  will be cached but the PT0 will not since there is still an
;[8870] opener; the one which opened the file for read when it was short.  If
;[8870] FILWB is not passed in to RELOFN, then PT0 will still reflect that it
;[8870] is  open  for  write.  So, if another opener tries to open this file,
;[8870] which is now long, CHKLAC will believe that there is already a writer
;[8870] of PT0 and an OPNX9 will result.

CNTLNG:	LOAD A,FLP0,(JFN)	;GET OFN OF PT0
	TQNE <WRTF>		;[8870] File open for write?
	TXO A,FILWB		;[8870] Yes, propagate bit
	CALL RELOFN		;UNDO EXTRA COUNT

***** CHANGE #9; PAGE 57, LINE 13; PAGE 57, LINE 13
Cause:	A long file was being deleted and ASGOFN could not assign a system file
	number (OFN).  This usually happens because there were not enough OFN
	slots.

Action:	If this happens frequently, rebuild the monitor with more OFN slots.

>)
 ---------------------------------
Cause:	A long file was being deleted and ASGOFN could not assign a system file
	number (OFN).  This usually happens because there were not enough OFN
	slots.

Action:	If this happens frequently, rebuild the monitor with more OFN slots.
>)

***** CHANGE #10; PAGE 68, LINE 10; PAGE 68, LINE 10

Cause:	The RNAMF JSYS has detected a monitor internal error. It has created
	an FDB for the destination file, and an internal routine that
	finds an FBD in a directory has returned with a failure,
	indicating an inconsistency in the newly-created FDB.

>)
 ---------------------------------

Cause:	The RNAMF JSYS has detected a monitor internal error. It has created
	an FDB for the destination file, and an internal routine that
	finds an FBD in a directory has returned with a failure,
	indicating an inconsistency in the newly-created FDB.
>)

***** CHANGE #11; PAGE 77, LINE 28; PAGE 77, LINE 28
	MOVEI C,10		;[7118] AND ADD IT IN OCTAL
	NOUT			;[7118] 
	 BUG.(CHK,NOUTF1,DISC,SOFT,<SPLOPN - NOUT of directory number failed>,,<

Cause:	The NOUT JSYS failed in trying to open the spooled disk file.
>)
 ---------------------------------
	MOVEI C,10		;[7118] AND ADD IT IN OCTAL
	NOUT			;[7118] 
	 BUG.(CHK,NOUTF1,DISC,SOFT,<SPLOPN - NOUT of directory number failed>,,<

Cause:	The NOUT JSYS failed in trying to open the spooled disk file.

Action:	See what the JSYS error from NOUT% was and try to determine what
	may be wrong. It is possible that the destination disk may be
	having problems.
>)

