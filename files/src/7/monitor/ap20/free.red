REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:18:01
File 1: SRC:<7.MONITOR>FREE.MAC.1
File 2: SRC:<7.MONITOR.AP20>FREE.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8937 to FREE.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8912 to FREE.MAC on 17-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 6, LINE 24; PAGE 6, LINE 24
	MOVEI T2,STKCD4		;TYPE
	CALL JSFRMV		;REMOVE ENTRY
	POP P,T1		;RESTORE DATUM
	JRST RELMES		;AND PROCEED
RELSWP::HRRZM T2,0(T1)		;GLOBAL CALL WITH LENGTH IN T2
	JUMPLE T2,[	BUG.(HLT,RELFRM,FREE,SOFT,<Illegal to deassign 0 free space>,,<

 ---------------------------------
	MOVEI T2,STKCD4		;TYPE
	CALL JSFRMV		;REMOVE ENTRY
	POP P,T1		;RESTORE DATUM
	JRST RELMES		;AND PROCEED
RELSWP::HRRZM T2,0(T1)		;GLOBAL CALL WITH LENGTH IN T2
	JUMPLE T2,[ BUG.(HLT,RELFRM,FREE,SOFT,<Illegal to deassign 0 free space>,,<


***** CHANGE #3; PAGE 7, LINE 19; PAGE 7, LINE 19
	BUG.(HLT,ASGFR0,FREE,SOFT,<ASGFRE - Illegal to assign 0 free space>,,<

Cause:	An illegal request for free space is being made.  The calling routine
	is asking for zero words of free space.

Action:	Look at the dump.  By backing up the stack you
	should be able to tell what routine called for the illegal
	free space.


>)
 ---------------------------------
	BUG.(HLT,ASGFR0,FREE,SOFT,<ASGFRE - Illegal to assign 0 free space>,,<

Cause:	An illegal request for free space is being made.  The calling routine
	is asking for zero words of free space.

Action:	Look at the dump.  By backing up the stack you should be able to 
	tell what routine called for the illegal free space.
>)

***** CHANGE #4; PAGE 7, LINE 36; PAGE 7, LINE 33
	routines should be made only while the calling process is NOINT.  The
	calling routine is not protecting itself from losing free space.  It is
	OKINT.  Since it is OKINT it could get interupted and never return,
	thus losing the free block assigned.

Action:	Make the routine be NOINT until it has ensured that the block will
	be freed when it is interrupted (e.g. JSB stack).

Data:	CALLER -  The address of the calling routine

>)
 ---------------------------------
	routines should be made only while the calling process is NOINT.  The
	calling routine is not protecting itself from losing free space.  It is
	OKINT.  Since it is OKINT it could get interupted and never return,
	thus losing the free block assigned.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  The dump will show the routine
	which is calling OKINT.  Make the routine be NOINT until it has 
	ensured that the block will be freed when it is interrupted 
	(e.g. JSB stack).

Data:	CALLER - The address of the calling routine
>)

***** CHANGE #5; PAGE 10, LINE 31; PAGE 10, LINE 31
	JRST [	MOVE A,-1(P)	;GET ADDRESS OF CALL
		BUG.(CHK,RELINT,FREE,SOFT,<RELFRE called OKINT>,<<A,CALLER>>,<

Cause:	This is a free space problem.  The calling routine is trying to release
	a swapable free space block while it is OKINT.  This is dangerous since
	it could get interrupted and loose the block.  All
	free space actions should occur while NOINT.

Action:	Make the calling routine become NOINT when it removes the address
	of the block about to be released from the database.  The routine
 ---------------------------------
	JRST [	MOVE A,-1(P)	;GET ADDRESS OF CALL
		BUG.(CHK,RELINT,FREE,SOFT,<RELFRE called OKINT>,<<A,CALLER>>,<

Cause:	This is a free space problem.  The calling routine is trying to release
	a swapable free space block while it is OKINT.  This is dangerous since
	it could get interrupted and loose the block.  All free space actions 
	should occur while NOINT.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  The dump will show the routine
	which is calling OKINT.  Make it be NOINT when it removes the address
	of the block about to be released from the database.  The routine

***** CHANGE #6; PAGE 11, LINE 39; PAGE 11, LINE 39
Cause:	This is a free space problem.  The caller to the free space
	routines is trying to return a block that was not given
	out by the free space manager.  The block is outside the
	range of free space management.

Action:	If the problem persists, change the CHK to a HLT and
	look through the dump.  By looking at the stack you
	should be able to determine who called for the releasing
	of the block.

Data:	BLOCK - Address of block being released
	POLHDR - Address of free storage header (e.g. ASGRES)
 ---------------------------------
Cause:	This is a free space problem.  The caller to the free space
	routines is trying to return a block that was not given
	out by the free space manager.  The block is outside the
	range of free space management.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  By looking at the stack you
	should be able to determine who called for the releasing
	of the block.

Data:	BLOCK  - Address of block being released
	POLHDR - Address of free storage header (e.g. ASGRES)

***** CHANGE #7; PAGE 21, LINE 62; PAGE 21, LINE 62
	BUG.(HLT,JSTERR,FREE,SOFT,<JSB stack error>,,<

Cause:	This is a problem with the JSB-stack logic; the count for the stack
	indicated that free cells were available, however none could be
	found.

Action: Analyze the dump.

>)
 ---------------------------------
	BUG.(HLT,JSTERR,FREE,SOFT,<JSB stack error>,,<

Cause:	This is a problem with the JSB-stack logic; the count for the stack
	indicated that free cells were available, however none could be
	found.
>)

***** CHANGE #8; PAGE 30, LINE 48; PAGE 30, LINE 48
	term, it may be possible to insert an EA.ENT at the beginning of
	the routine that called ASGRES. Note that this may lead to other
	problems, including slower performance and ILMNRF BUGHLT's.

Data:	PC - the PC from which ASGRES was called

>)
 ---------------------------------
	term, it may be possible to insert an EA.ENT at the beginning of
	the routine that called ASGRES. Note that this may lead to other
	problems, including slower performance and ILMNRF BUGHLT's.

Data:	PC - the PC from which ASGRES was called
>)

***** CHANGE #9; PAGE 31, LINE 11; PAGE 31, LINE 11
	SKIPE T2		;0 ISN'T A LEGAL POOL NUMBER
	CAML T2,.REPMX(W1)	;IS POOL NUMBER TOO LARGE?
	JRST [	BUG.(CHK,ASGREQ,FREE,SOFT,<Illegal pool number given to ASGRES>,,<

Cause:	This is a free space problem.  The caller is requesting resident
	free space.  In T2 the caller is providing a pool number for where
	the free space should come from.  This pool number is incorrect.

Action:	If the problem persists change the BUGCHK to a BUGHLT to find the
	culprit. The caller putting a bad value into T2.


>)
 ---------------------------------
	SKIPE T2		;0 ISN'T A LEGAL POOL NUMBER
	CAML T2,.REPMX(W1)	;IS POOL NUMBER TOO LARGE?
	JRST [	BUG.(CHK,ASGREQ,FREE,SOFT,<Illegal pool number given to ASGRES>,,<

Cause:	This is a free space problem.  The caller is requesting resident
	free space.  In T2 the caller is providing the pool number from 
	which the free space should come.  This pool number is incorrect.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #10; PAGE 32, LINE 16; PAGE 32, LINE 16
Cause:	This is a free space problem.  The caller is asking for resident
	free space.  In T3 the caller gives a priority for this request.
	The priority determines how ASGRES is going to handle this request
	when free space is low.  This priority is out of range.

Action:	If the problem persists change the BUGCHK to a BUGHLT and look
	through the dump to find the caller.


>)
 ---------------------------------
Cause:	This is a free space problem.  The caller is asking for resident
	free space.  In T3 the caller gives a priority for this request.
	The priority determines how ASGRES is going to handle this request
	when free space is low.  This priority is out of range.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #11; PAGE 42, LINE 64; PAGE 42, LINE 64
	term, it may be possible to insert an EA.ENT at the beginning of
	the routine that called RELRES. Note that this may lead to other
	problems, including slower performance and ILMNRF BUGHLT's.

Data:	PC - the PC from which RELRES was called

>)
 ---------------------------------
	term, it may be possible to insert an EA.ENT at the beginning of
	the routine that called RELRES. Note that this may lead to other
	problems, including slower performance and ILMNRF BUGHLT's.

Data:	PC - the PC from which RELRES was called
>)

***** CHANGE #12; PAGE 43, LINE 8; PAGE 43, LINE 8
	;..
	SUBI T1,RSHLEN		;POINT TO HEADER
	TRNE T1,3		;ON A 4-WORD BOUNDARY?
	JRST [	MOVE T1,RELRAD	;NO.  GET THE BAD ADDRESS
		MOVE T2,RELCAL	; AND THE CALLER
		BUG.(CHK,RESBAD,FREE,SOFT,<RELRES - Illegal address passed to RELRES>,<<T1,BADADR>,<T2,CALLER>>,<

 ---------------------------------
	;..
	SUBI T1,RSHLEN		;POINT TO HEADER
	TRNE T1,3		;ON A 4-WORD BOUNDARY?
	JRST [	MOVE T1,RELRAD	;NO.  GET THE BAD ADDRESS
		MOVE T2,RELCAL	; AND THE CALLER
		BUG.(CHK,RESBAD,FREE,SOFT,<Illegal address passed to RELRES>,<<T1,BADADR>,<T2,CALLER>>,<


***** CHANGE #13; PAGE 43, LINE 15; PAGE 43, LINE 15
Cause:	This is a free space problem.  The caller is trying to release some
	resident free space.  The address being specified is not a legal
	resident free space address.

Action:	If the problem persists change the BUGCHK to a BUGHLT and find the
	caller.  The caller is providing an illegal address.
	Find where the caller get the address and how that location gets
	modified.

 ---------------------------------
Cause:	This is a free space problem.  The caller is trying to release some
	resident free space.  The address being specified is not a legal
	resident free space address.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  The dump will indicate the 
	caller which is providing the illegal address.  Find where the 
	caller gets the address and how that location gets modified.


***** CHANGE #14; PAGE 45, LINE 10; PAGE 45, LINE 10

Cause:	This is a free space problem.  The caller is trying to release resident
	free space.  The address passed to RELRES is outside the range of the
	resident free space pool.

Action:	If the problem persists change the BUGCHK to a BUGHLT.

 ---------------------------------

Cause:	This is a free space problem.  The caller is trying to release resident
	free space.  The address passed to RELRES is outside the range of the
	resident free space pool.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.


***** CHANGE #15; PAGE 61, LINE 29; PAGE 61, LINE 29

Action:	Look at the dump.  By backing up the stack you
	should be able to tell what routine called for the illegal
	free space.

Data:	POOLN		Pool number
	CALRPC		PC of caller of ASGFSP
>)
 ---------------------------------

Action:	Look at the dump.  By backing up the stack you
	should be able to tell what routine called for the illegal
	free space.

Data:	POOLN  - Pool number
	CALRPC - PC of caller of ASGFSP
>)

***** CHANGE #16; PAGE 61, LINE 38; PAGE 61, LINE 38

ASGFS1:	SKIPL INTDF		;Is the process NOINT?
	JRST ASGFS2		;Yes
	MOVE T4,DESNDX		;Get pool number
	MOVE T3,CALRPC		;Get caller's PC

	BUG.(HLT,FSPANN,FREE,SOFT,<ASGFRE called OKINT>,<<T4,POOLN>,<T3,CALRPC>>,<
 ---------------------------------

ASGFS1:	SKIPL INTDF		;Is the process NOINT?
	JRST ASGFS2		;Yes
	MOVE T4,DESNDX		;Get pool number
	MOVE T3,CALRPC		;Get caller's PC
	BUG.(HLT,FSPANN,FREE,SOFT,<ASGFRE called OKINT>,<<T4,POOLN>,<T3,CALRPC>>,<

***** CHANGE #17; PAGE 61, LINE 52; PAGE 61, LINE 51

Action:	The data supplied gives the address of the calling routine.  Make
	the routine NOINT until it has insured that the block will be
	freed when it is interrupted (e.g. JSB stack).

Data:	POOLN - Pool number
	CALRPC - Caller of RELFSP
 ---------------------------------

Action:	The data supplied gives the address of the calling routine.  Make
	the routine NOINT until it has insured that the block will be
	freed when it is interrupted (e.g. JSB stack).

Data:	POOLN  - Pool number
	CALRPC - Caller of RELFSP

***** CHANGE #18; PAGE 64, LINE 67; PAGE 64, LINE 67
Cause:	This is a free space problem.  There is no more space available
	in the freespace pool.

Action:	The data supplied gives the pool number (pool descriptor index) of
	the pool in question.  If the pool repeatedly runs out of space,
	the pool size must be increased and the monitor rebuilt.

 ---------------------------------
Cause:	This is a free space problem.  There is no more space available
	in the freespace pool.

Action:	The data supplied gives the pool number (pool descriptor index) of
	the pool in question.  If the pool repeatedly runs out of space,
	the pool size must be increased and the monitor rebuilt.  The pool
	size is specified in STG.MAC as the third argument in the FSPPL. 
	macro used to build the freespace pools.


***** CHANGE #19; PAGE 65, LINE 22; PAGE 65, LINE 22
	MOVE P1,FSPTAB(T2)	;Get address of descriptor
	SKIPL INTDF		;Are we NOINT?
	JRST RLF1		;Yes
	MOVE T4,DESNDX		;Get pool number
	MOVE T3,CALRPC		;Get caller's PC

	BUG.(HLT,FSPDNN,FREE,SOFT,<RELFSP called OKINT>,<<T4,POOLN>,<T3,CALRPC>>,<

Cause:	This is a free space problem.  The calling routine is trying to release
	a swapable free space block while it is OKINT.  This is dangerous since
	it could get interrupted and loose the block.  All
	free space actions should occur while NOINT.

Action:	The data supplied gives the address of the calling routine.  Make
	the routine become NOINT when it removes the address of the block
	about to be released from the database.  The routine
	can be made OKINT when control is returned to it.

Data:	POOLN - Pool number
	CALRPC - PC of caller of RELFSP
 ---------------------------------
	MOVE P1,FSPTAB(T2)	;Get address of descriptor
	SKIPL INTDF		;Are we NOINT?
	JRST RLF1		;Yes
	MOVE T4,DESNDX		;Get pool number
	MOVE T3,CALRPC		;Get caller's PC
	BUG.(HLT,FSPDNN,FREE,SOFT,<RELFSP called OKINT>,<<T4,POOLN>,<T3,CALRPC>>,<

Cause:	This is a free space problem.  The calling routine is trying to release
	a swapable free space block while it is OKINT.  This is dangerous since
	it could get interrupted and loose the block.  All free space actions 
	should occur while NOINT.

Action:	The data supplied gives the address of the calling routine.  Make
	the routine become NOINT when it removes the address of the block
	about to be released from the database.  The routine can be made 
	OKINT when control is returned to it.

Data:	POOLN  - Pool number
	CALRPC - PC of caller of RELFSP

***** CHANGE #20; PAGE 66, LINE 28; PAGE 66, LINE 28
	JRST CHKPRE		;No
	MOVE T4,CALRPC		;Get caller's PC
	MOVE Q1,DESNDX		;Get pool number
	BUG.(HLT,FSPARB,FREE,SOFT,<RELFSP - Bad block being released>,<<Q1,POOLN>,<T4,CALRPC>,<P2,BLKADR>>,<

Cause: The caller is attempting to release a block that has already been
	released.
 ---------------------------------
	JRST CHKPRE		;No
	MOVE T4,CALRPC		;Get caller's PC
	MOVE Q1,DESNDX		;Get pool number
	BUG.(HLT,FSPARB,FREE,SOFT,<RELFSP - Bad block being released>,<<Q1,POOLN>,<T4,CALRPC>,<P2,BLKADR>>,<

Cause:  The caller is attempting to release a block that has already been
	released.

***** CHANGE #21; PAGE 66, LINE 37; PAGE 66, LINE 37
Action:	Looking at the stack will show the caller.  It is possible that the
	length of the current block is incorrect.  It is equally likely that
	the block(s) before this block (in free space) have had incorrect
	lengths on return.  Thus, the caller may not be the culprit.

Data:	POOLN - Pool number
	CALRPC - PC of caller of RELFSP
 ---------------------------------
Action:	Looking at the stack will show the caller.  It is possible that the
	length of the current block is incorrect.  It is equally likely that
	the block(s) before this block (in free space) have had incorrect
	lengths on return.  Thus, the caller may not be the culprit.

Data:	POOLN  - Pool number
	CALRPC - PC of caller of RELFSP

***** CHANGE #22; PAGE 66, LINE 62; PAGE 66, LINE 62
Action:	Looking at the stack will show the caller.  It is possible that the
	length of the current block is incorrect.  It is equally likely that
	the block(s) before this block (in free space) have had incorrect
	lengths on return.  Thus, the caller may not be the culprit.

Data:	POOLN - Pool number
	CALRPC - PC of caller of RELFSP
 ---------------------------------
Action:	Looking at the stack will show the caller.  It is possible that the
	length of the current block is incorrect.  It is equally likely that
	the block(s) before this block (in free space) have had incorrect
	lengths on return.  Thus, the caller may not be the culprit.

Data:	POOLN  - Pool number
	CALRPC - PC of caller of RELFSP

***** CHANGE #23; PAGE 66, LINE 86; PAGE 66, LINE 86
Action:	Looking at the stack will show the caller.  It is possible that the
	length of the current block is incorrect.  It is equally likely that
	the block(s) before this block (in free space) have had incorrect
	lengths on return.  Thus, the caller may not be the culprit.

Data:	POOLN		Pool number
	CALRPC		PC of caller of RELFSP
	BLKADR		Address of user block


>)
 ---------------------------------
Action:	Looking at the stack will show the caller.  It is possible that the
	length of the current block is incorrect.  It is equally likely that
	the block(s) before this block (in free space) have had incorrect
	lengths on return.  Thus, the caller may not be the culprit.

Data:	POOLN  - Pool number
	CALRPC - PC of caller of RELFSP
	BLKADR - Address of user block
>)

***** CHANGE #24; PAGE 67, LINE 70; PAGE 67, LINE 70
Action:	Looking at the stack will show the caller.  It is possible that the
	length of the current block is incorrect.  It is equally likely that
	the block(s) before this block in free space have had incorrect
	lengths on return.  Thus, the caller may not be the culprit.

Data:	POOLN - Pool number
	CALRPC - PC of caller of RELFSP
 ---------------------------------
Action:	Looking at the stack will show the caller.  It is possible that the
	length of the current block is incorrect.  It is equally likely that
	the block(s) before this block in free space have had incorrect
	lengths on return.  Thus, the caller may not be the culprit.

Data:	POOLN  - Pool number
	CALRPC - PC of caller of RELFSP

***** CHANGE #25; PAGE 67, LINE 99; PAGE 67, LINE 99
Action:	Look through the dump.  If the blocksize is wrong, then study the
        code at the calling PC for possible errors.  If the pool count is
	wrong, then more investigation is required.  The history buffer for the
	pool may contain helpful data.

Data:	POOLN - Pool number
	CALRPC - PC of caller to RELFSP
 ---------------------------------
Action:	Look through the dump.  If the blocksize is wrong, then study the
        code at the calling PC for possible errors.  If the pool count is
	wrong, then more investigation is required.  The history buffer for the
	pool may contain helpful data.

Data:	POOLN  - Pool number
	CALRPC - PC of caller to RELFSP

***** CHANGE #26; PAGE 67, LINE 124; PAGE 67, LINE 124

Action:	Look through the dump.  By looking at the stack you
	should be able to determine who called for the releasing
	of the block.

Data:	POOLN - Pool number
	CALRPC - PC of caller to RELFSP
 ---------------------------------

Action:	Look through the dump.  By looking at the stack you
	should be able to determine who called for the releasing
	of the block.

Data:	POOLN  - Pool number
	CALRPC - PC of caller to RELFSP

***** CHANGE #27; PAGE 73, LINE 55; PAGE 73, LINE 55
	SKIPA			;Bad blocksize
	RETSKP			;Everything OK

BADSZ:	BUG.(HLT,FSPBBS,FREE,SOFT,<Bad blocksize>,<<P1,POOLN>,<P2,BLKADR>>,<


Cause:	This is a free space problem; 	The block size is either smaller
 ---------------------------------
	SKIPA			;Bad blocksize
	RETSKP			;Everything OK

BADSZ:	BUG.(HLT,FSPBBS,FREE,SOFT,<Bad blocksize>,<<P1,POOLN>,<P2,BLKADR>>,<

Cause:	This is a free space problem; 	The block size is either smaller

***** CHANGE #28; PAGE 73, LINE 68; PAGE 73, LINE 67
	If the list header is large enough to include it, the PC, job 
	number and fork number of the assigner and deassigner are stored in  
	the header/trailer.  These may give a clue as to what code caused the  
	problem.

	If the problem persists, change the BUGCHK to a BUGHLT and analyze the 
	dump.


Data:	POOLN - Pool number
	BLKADR - Address of the block. Zero indicates it is the pool
 ---------------------------------
	If the list header is large enough to include it, the PC, job 
	number and fork number of the assigner and deassigner are stored in  
	the header/trailer.  These may give a clue as to what code caused the  
	problem.

Data:	POOLN  - Pool number
	BLKADR - Address of the block. Zero indicates it is the pool

***** CHANGE #29; PAGE 73, LINE 96; PAGE 73, LINE 91
	If the list header is large enough to include it, the PC, job 
	number and fork number of the assigner and deassigner are stored in 
	the header/trailer.  These may give a clue as to what code caused the  
	damage.

	If the problem persists, change the BUGCHK to a BUGHLT and analyze the 
	dump.


Data:	POOLN - Pool number
	BLKADR - Address of the block. Zero indicates it is the pool
 ---------------------------------
	If the list header is large enough to include it, the PC, job 
	number and fork number of the assigner and deassigner are stored in 
	the header/trailer.  These may give a clue as to what code caused the  
	damage.

Data:	POOLN  - Pool number
	BLKADR - Address of the block. Zero indicates it is the pool

***** CHANGE #30; PAGE 74, LINE 66; PAGE 74, LINE 66

Cause:	This is a free space problem.  A free block in the freespace pool
	was found to contain a pointer that did not fall within the legal
	address interval for that pool.


Action:	Analyze the Dump.


Data:	POOLN		Pool number
	BLKADR		Address of the block containing the bad pointer.
	NXTPTR		The bad pointer
>)
 ---------------------------------

Cause:	This is a free space problem.  A free block in the freespace pool
	was found to contain a pointer that did not fall within the legal
	address interval for that pool.

Data:	POOLN  - Pool number
	BLKADR - Address of the block containing the bad pointer.
	NXTPTR - The bad pointer
>)

***** CHANGE #31; PAGE 74, LINE 87; PAGE 74, LINE 83
	is incremented by its blocksize, the result should be the address of  
	the next block in the chain.  This is not the case. It indicates that  
	the blocksize of the current block is incorrect, or that the successor  
	of the current block has suffered header damage.




Action:	If the problem persists, change the BUGCHK to a BUGHLT and analyze the 
	dump.


Data:	POOLN		Pool number
	BLKADR		Address of the current block.
>)
 ---------------------------------
	is incremented by its blocksize, the result should be the address of  
	the next block in the chain.  This is not the case. It indicates that  
	the blocksize of the current block is incorrect, or that the successor  
	of the current block has suffered header damage.

Data:	POOLN  - Pool number
	BLKADR - Address of the current block.
>)

***** CHANGE #32; PAGE 74, LINE 107; PAGE 74, LINE 96
	BUG.(HLT,FSPASN,FREE,SOFT,<Assigned-block in free-list>,<<Q1,POOLN>,<T1,BLKADR>>,<

Cause:	This is a free space problem.  A block in the freespace pool was
	found to have the "block-assigned" bit set.


Action:	Analyze the Dump.


Data:	POOLN		Pool number
	BLKADR		Address of the block.
>)
 ---------------------------------
	BUG.(HLT,FSPASN,FREE,SOFT,<Assigned-block in free-list>,<<Q1,POOLN>,<T1,BLKADR>>,<

Cause:	This is a free space problem.  A block in the freespace pool was
	found to have the "block-assigned" bit set.

Data:	POOLN  - Pool number
	BLKADR - Address of the block
>)

***** CHANGE #33; PAGE 75, LINE 55; PAGE 75, LINE 55
Cause:	This BUGCHK occurs when a routine calls routine FSPREM in FREE
	to determine how much free space is left in a given pool and
	the pool number supplied is either out of range (greater or
	equal to FSPTBL) or does not exit (FSPTAB entry equals zero).

Action:	Examine the calling routine and fix it to supply a valid pool
	number.

Data:	POOLN - Pool number
	CALLER - Address of calling routine
 ---------------------------------
Cause:	This BUGCHK occurs when a routine calls routine FSPREM in FREE
	to determine how much free space is left in a given pool and
	the pool number supplied is either out of range (greater or
	equal to FSPTBL) or does not exit (FSPTAB entry equals zero).

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Use the dump to examine the 
	calling routine and fix it to supply a valid pool number.

Data:	POOLN  - Pool number
	CALLER - Address of calling routine

