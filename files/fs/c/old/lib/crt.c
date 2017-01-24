/* <KCC.LIB>CRT.C.14, 26-Jul-85 14:35:26, Edit by KRONJ */
/*  Fix $MAPSC to work for pre-release-5 TOPS-20 */

/*
** CRT - C Run Time machine-language support routines
** Split out and rewritten from TOPS20.FAI by KLH@SRI-NIC, June 1985
*/

#include "c-env.h"

	entry $start, _exit;	/* System-dependent routines */

	entry	etext;		/* 1st addr above program text (code) region */
	entry	edata;		/* 1st addr above initialized data region */
	entry	end;		/* 1st addr above uninitialized data */
			/*  == 1st addr of dynamically allocated stuff.*/
	entry	$byte,$adjbp,$subbp,$bpcnt;	/* Byte pointer support */
	entry	$dfix,$dflot;			/* int <-> double conversion */
	entry	$spush,$spop;			/* Structure args & retvals */
	entry	$ret;		/* Handy return points.  Simple return. */
	entry	$retp,$rett;	/* Returns +1 Positive (True) */
	entry	$retz,$retf;	/* Returns  0 Zero (False) */
	entry	$retn;		/* Returns -1 Negative */
	entry	$zero;		/* Double zero for floating constant ref */
#if SYS_T20+SYS_10X
	entry	$mapsc;		/* Hack, so fork() can get at it. */
#endif

extern _runtm();	/* C Unix-environment runtime */

/**************************************************************/
#asm			/* Start of moby assembler code stuff */

A=1
B=2
C=3
R=15		; for dfix, spush.
S=16		; for dfix, spush, byte.
P=17

	OPDEF	RET	[POPJ 17,]

; System-dependent runtime code should contain the following:
;	(1) A $START label.  The process will be started at this point,
;		and the code should:
;		- Set up the stack and point P at it.
;		- Arrange for whatever memory shuffling is desired,
;			including extended addressing.
;		- Set the global vars END, ETEXT, EDATA.
;		- Finally, jump back to SREADY.
;	(2) A .EXIT label, for a function that should halt the process
;		normally with the exit status provided by arg 1.

	; Link to runtime initializations
INITS:	BLOCK	1		;Make space for LINK to fill in with init chain
	.LNKEND	1,INITS		;Tell it to head the chain here

SREADY:	SETZ 0,		; Make sure any refs to 0 just get 0.
	SKIPE R,INITS	; Are there any inits set up by loader?
	 PUSHJ P,1(R)	; Yeah, go hack them (obscure feature)
	PUSHJ P,.RUNTM	; Invoke higher level setup.  This will call MAIN.

			; This should exit by itself, but just in case
	PUSH P,1	; we return, pass on the return value
	PUSHJ P,.EXIT	; and exit forcibly, without any cleanup.
	JRST .-1	; Insist.

#if SYS_T20+SYS_10X
; ************************************************************
;	C low-level Runtime Support Routines
;	TOPS-20/TENEX Operating System
; ************************************************************

	SEARCH	MONSYM

	.JBHSP==75		;Contains page number of start of high seg
	.JBHRL==115		;high seg first free addr,,highest addr
	.JBSA==120		;Contains first free loc low seg,,start addr


	; Start of program
$START:	RESET%			;Initialize the world.

	MOVE 	P,.JBSA		;Get top of low core (bottom of stack)
	HLR 	P,P		; in both halves.
	HRRZ	15,P		;Save ptr to end of data in words
	IOR	15,$BYTE	;Make a byte ptr
	MOVEM	15,EDATA	;Save the ptr to the end of data
	SETCM	16,.JBHSP	;Get neg page no of high core start, minus one.
	ROT	16,^D27		;Make negation of (address,,1).
	ADD	P,16		;Add to make stack pointer.
#if SYS_T20
	XMOVEI	15,0		;Get section number in left half
	JUMPN	15,ESTART	;If non-zero, jump to extended start
#endif
	MOVE	16,.JBHSP	;Get first high seg. page
	ROT	16,^D9		;Make it an address
	HLRZ	15,.JBHRL	;Get first free high seg. relative address
	ADD	15,16		;Make it absolute
	IOR	15,$BYTE	;Make a byte pointer
	MOVEM	15,END		;Save it as the END
	MOVEM	15,ETEXT	;Same as END
;;	MOVEM	15,CURBRK	;Save it as the current break
#if SYS_T20
	SKIPE	$EXADF		;Want extended addressing?
	 JRST	DOEXTA		; Yeah, must do hairy setup
#endif
	JRST SREADY		; All ready to go, jump to start the rest.

.EXIT:	HALTF%			;Exit to monitor.
	HRROI	1,[ASCIZ/Cannot continue/]
	ESOUT%			;Complain if continued
	JRST	.EXIT		; and stop again.

#if SYS_T20
DOEXTA:	SETZ	11,		;From section zero
DOMOVE:	MOVEI	10,1		;Destination section is 1
	DMOVE	6,[ .FHSLF
		    .FHSLF ]	;From and to ourself
	PUSHJ P,$mapsc		;Map section

	SETO	1,			;1 Unmapping
	MOVSI	2,.FHSLF		;2 Starting on page zero of ourself
	MOVE	3,[PM%CNT!PM%EPN!1000]	;3 All pages of section zero
	SETZ	4,			;4 No PC flags
	MOVSI	5,1			;5 Section number 1
	HRRI	5,ESTART		;  To make extended address
	DMOVE	6,[ PMAP%		;6 Unmap current section
		    XJRSTF 4 ]		;7 Jump into section 1
	JRST	6		;Go do it

ESTART:	XMOVEI	11,.		;Get section number we are in
	HLRZS	11		;Isolate it
	CAIE	11,1		;Are we in section 1?
	 JRST	DOMOVE		;No, so go there
	MOVEI	1,1		;Get section number for code
	MOVEM	1,$EXADF	;Remember where code lives

	;; Make a stack in section 2
	SETZ	1,		;Creating
	HRRZI	2,2		;In section 2
	HRLI	2,.FHSLF	;On ourself
	MOVEI	3,1		;One section
	SMAP%			;Make it
	MOVEI	P,1000		;Make stack starting at first page
	HRLI	P,2		;Of section 2
	SETZM	-1(P)		;Clear out word before
	SETZM	@[170000,,776000] ;And word after (EFIW)
	MOVEI	1,2000		;Make page number of start
	HRLI	1,.FHSLF	;On self
	SETZ	2,		;No access privileges
	SPACS%			;Write-protect page zero
	ADDI	1,777		;Now last page
	SPACS%			;Similarly write-protect to bracket stack

	;; Set up the EDATA, ETEXT, and END for memory allocation
	HRLZI	1,2		;In section 1
	IOR	1,XBYTE		;Make it a byte pointer
	MOVEM	1,EDATA		;Both initialized data and code
	MOVEM	1,ETEXT		;are in section 1
	SETZ	1,		;Make a new section
	HRRZI	2,3		;In section 3
	HRLI	2,.FHSLF	;For us
	MOVEI	3,1		;One section
	SMAP%
	HRLZI	1,3		;In section 3
	IOR	1,XBYTE		;Make it a byte pointer
	MOVEM	1,END		;First location past end of program
;;	MOVEM	1,CURBRK	;Setup the current break

	;; Set up OWGBPs in byte-pointer table
	MOVE	1,[XBYTE,,$BYTE] ;From OWG table into byte constructor table
	BLT	1,$BYTE+^D8	;Move OWGs over so contructed BPs are OWGs
	JRST SREADY		; Done, return to main startup.
#endif /* T20 */

/*
** $MAPSC - Check map for one section and copy across to new fork.
** This auxiliary subroutine is also used by FORK().
** Call with
**    5/unchangeable by $mapsc
**    6/destination fork handle
**    7/source fork handle
**    10/destination section
**    11/source section
**
** We use the stack for scratch space, so have to be careful.
*/

$mapsc:	MOVE 16,P		/* Copy stack pointer */
	ADJSP P,10000		/* Give us some stack space */

/*
** Normal TOPS-20 (releases 5 and beyond) code.
** Read in the map all at once, and then do individual copies.
**
** Note that no SMAP% is needed to create the destination section;
** it will be done implicitly by the PMAP%s.
*/

#if SYS_T20
	MOVE 1,11		/* Copy source section number */
	HRL 1,7			/* and source handle */
	RSMAP%			/* Read section map */
	 ERJMP mapsc3		/* lost, go try it the old way */
	AOJE 1,mapsc2		/* If unmapped, dont copy */

	/* Have section, read page maps */
	MOVEI 1,4		/* Length of argument block is 4 */
	MOVEI 2,1000		/* Want a full section of pages */
	DMOVEM 1,1(16)		/* Save as first two words of argument block */
	MOVE 1,11		/* Get source section number */
	LSH 1,^D9		/* into page number */
	XMOVEI 2,5(16)		/* and a pointer to some scratch space */
	DMOVEM 1,3(16)		/* as third and fourth words of block */
	HRLZ 1,7		/* On source handle */
	XMOVEI 2,1(16)		/* with argument block */
	XRMAP%			/* read in a bunch of paging info at once */

	/* Now set up for individual page loop */
	XMOVEI 13,4000(16)	/* Get a place above the top of XRMAP% info */
	LSH 13,-^D9		/* as page number */
	HRLI 13,.FHSLF		/* of self (not source) */
	MOVE 14,10		/* Get destination section this time */
	LSH 14,^D9		/* as page number */
	HRLI 14,-1000		/* make AOBJN pointer */
	XMOVEI 12,5(16)		/* point to start of XRMAP% block */

mapse0:	DMOVE 1,0(12)		/* Get # of page in 1, page access wd in 2 */
	PUSHJ 17,mappag		/* Map that page */
	ADDI 12,2		/* Move over XRMAP% info to next page */
	AOBJN 14,mapse0		/* Loop over all pages in section */
	JRST mapsc4		/* Go on to cleanup and return */
#endif /* T20 */

/*
** Fall into here when not T20 (i.e. TENEX).
** Also come here if RSMAP% fails (pre-rel-5 TOPS20 or out of range section).
** Do it the old way, with a RPACS% per page.
*/

mapsc3:	CAIN 10,0		/* Make sure destination */
	 CAIE 11,0		/* and source sections are both zero */
	  JRST mapsc2		/* Can't handle non-zero, just return */
	MOVEI 13,777(16)
	LSH 13,-^D9		/* Get page # of a scratch page */
	HRLI 13,.FHSLF		/* Make page handle */
	MOVSI 14,-1000		/* Set up AOBJN pointer */

mapsc5:	MOVSI 1,(7)		/* Get source fork handle in LH */
	HRRI 1,(14)		/* Source page # */
	RPACS%			/* Get access of page */
	PUSHJ P,MAPPAG		/* Map the page over */
	AOBJN 14,mapsc5

/*
** Here after either sequence of code.
** Unmap the scratch page and return.
*/

mapsc4:	SETO 1,			/* Unmapping */
	MOVE 2,13		/* Into temporary page */
	MOVE 3,[PM%EPN]		/* One page, extended addressing */
	PMAP%			/* Do the unmap */

/*
** Here after scratch space unmapped or if error occurred.
** Give our space back to the stack and return.
*/

mapsc2:	ADJSP 17,-10000		/* Get space back */
	POPJ 17,		/* Done with mapping section */

; Map one page of fork into another (can be same) fork
; 1/ page handle (fork/file,,#) of source
; 2/ page access bits for above
; 6  / fork handle of destination
; 12 / <don't touch>
; 13 / page handle of temporary page on stack for our own use
; 14 / AOBJN pointer to page number of dest, including section

mappag:	TDNN	2,[RM%PEX]	; Check page access bits - page exists?
	 RET			;No, dont map across
	CAMN	1,13		; Page handle same as our temporary page?
	 RET			;Yes, dont copy
	MOVE	4,2		; Save access bits.
	MOVEI	2,(14)		; OK, copy to this dest page number
	HRLI	2,(6)		; In new inferior fork
	MOVE	3,[PM%RD!PM%EX!PM%CPY!PM%EPN] ;Copy-on-write, one page
	PMAP%			;Copy across, creating section if empty
#if SYS_T20
	JUMPGE	1,MAPPG9	;If directly from file, done with mapping
#endif
#if SYS_10X
	TDNN	4,[1B10]	; See if "Private" access bit is set.
	 JRST MAPPG9		; Nope.
#endif

	;; Here when page is private, break copy-on-write link
	MOVE	1,2		;Now get page again as source this time
	MOVE	2,13		;Get PMAP pointer to temporary page
	MOVE	3,[PM%RD!PM%WR!PM%EPN] ;Write not copy-on-write to self from fork
	PMAP%			;Copy the page
	 ERJMP	.+1		;Dont die if tried to make circular
	HRRZ	1,13		;Get pointer again
	LSH	1,^D9		;As address
	MOVES	(1)		;Break copy-on-write link in subfork to us
MAPPG9:	RET			;All done
#endif /* T20+10X */

#if SYS_WAITS
; ************************************************************
;	C low-level Runtime Support Routines
;	WAITS Operating System
; ************************************************************

JOBREL==44			; highest core usage
JOBFF==121			; first free location from monitor tables
JOBHRL==115			; Highest addr in high segment

$START:	RESET
	MOVE	P,JOBFF
	HRL	P,P		; Set up stack pointer

	MOVE	R,JOBREL	; get max core usage
	ADD	R,$STKSZ	; get some stack space
	CORE	R,		; ask for more core
	 JRST	.+1		; no space, tough
	MOVE	R,JOBREL	; get highest usage
	MOVEM	R,JOBFF		; Update JOBFF past allocated stack
	IOR	R,$BYTE		; Make byte ptr
	MOVEM	R,END		; This is where to allocate new memory.

	MOVEI	R,(P)		; Get 1st free data loc (same as PDL start)
	IOR	R,$BYTE		; Make it a byte ptr
	MOVEM	R,EDATA		; This is where data ended.

	HRRZ	R,JOBHRL	; Get highest addr in upper segment
	IOR	R,$BYTE
	MOVEM	R,ETEXT		; This is end of code.
	
	JRST SREADY		; All set, jump to start the rest.


.EXIT:	EXIT	0,		; exit to monitor
	JRST	.EXIT		; no reentry

#endif /* WAITS */

#if SYS_ITS
; ************************************************************
;	C low-level Runtime Support Routines
;	ITS Operating System
; ************************************************************

SSEGLO==20	; STINK sets this to <loseg org>,,<loseg top>	; org always 0
SSEGHI==21	; STINK sets this to <hiseg org>,,<hiseg top>
PG$BTS==12	; log 2 of ITS page size
PG$SIZ==2000	; # words in ITS page
PG$MSK==<PG$SIZ-1>

; For the time being, we duplicate the TOPS-20 arrangement of having
; the stack at the end of the low segment, and new allocated memory
; at the end of the high segment.  If we want to use all of the address
; space, we can always make malloc smarter.

$START:
	HRRZ P,SSEGLO	; Set up stack pointer
	ADDI P,1
	MOVN R,$STKSZ
	HRL P,R		; Now have -<count> in LH

	MOVEI 1,PG$SIZ-1(P)
	LSH 1,-PG$BTS	; Find 1st page # we dont have
	MOVE 2,$STKSZ
	ADDI 2,PG$SIZ-1
	LSH 2,-PG$BTS	; Find # pages we need for stack
	IMUL 2,[-1,,0]
	HRRI 2,(1)	; Now have page AOBJN
	.CALL [	SETZ	; Allocate pages for the stack.
		SIXBIT /CORBLK/
		MOVEI %CBNDR+%CBNDW
		MOVEI %JSELF
		2
		SETZI %JSNEW]
	 .LOSE		; Shouldnt fail.

	MOVEI R,(P)	; Now set end of loaded data
	IOR R,$BYTE
	MOVEM R,EDATA

	HRRZ R,SSEGHI
	ADDI R,1
	IOR R,$BYTE		; Must make into char ptrs.
	MOVEM R,ETEXT		; End of code
	ADDI R,PG$SIZ-1
	ANDCMI R,PG$MSK
	MOVEM R,END		; This is where to allocate new mem.

	JRST SREADY		; All set, jump to start the rest.


.EXIT:	MOVE 1,ARG1(P)		; Just in case, leave "exit status" in AC 1.
	.LOGOUT 1,		; Return to superior (log out if disowned)
	JRST .EXIT		; Never continue.

#endif /* ITS */

	$$DATA

$EXADF:	0		; Patch non-zero to run extended.
$STKSZ:	10000		; Minimum stack size to allocate at startup.

END:	0		; End of the address space (where to alloc new stuff)
ETEXT:	0		; End of text segment
EDATA:	0		; End of data segment

	$$CODE
$RETF:
$RETZ:	SETZ 1,			; Return Zero (also False)
$RET:	RET

$RETT:
$RETP:	MOVEI 1,1		; Return Positive (also True)
	RET
$RETN:	SETO 1,			; Return Negative
	RET

$ZERO:	0		; Double zero, for floating constant reference.
	0

	$$DATA
$BYTE:	331100,,0		;Bits to drop onto pointers to make them bytes
	221100,,0		;(Changed to OWGBPs if we go extended)
	111100,,0
	001100,,0

	350700,,0		;And the same for 7-bit bytes
	260700,,0
	170700,,0
	100700,,0
	010700,,0

	$$CODE

XBYTE:	700000,,0		;OWGBPs to replace $BYTE in extended sections
	710000,,0
	720000,,0
	730000,,0

	620000,,0
	630000,,0
	640000,,0
	650000,,0
	660000,,0

; What this code section used to be:
;<KCC.LIB>BYTE.FAI.14, 24-Mar-85 11:35:42, Edit by KRONJ
; Fix calls to $BPCNT in $SUBBP
;<KCC.LIB>BYTE.FAI.13, 15-Mar-85 12:26:44, Edit by KRONJ
; New fixed register calling convention
;
;	TITLE	BYTE - Byte manipulation subroutines for C
;	SUBTTL	David Eppstein / Stanford University / 22-May-1984
;
; Routines for pseudo-op expansion: $ADJBP $SUBBP $BPCNT
; calling convention is:
;	MOVE	15,R
;	MOVE	16,ADDR
;	PUSHJ	P,$FOO
;
; A return value if any is left in S.
;
; These routines should be as general as possible so that I can decide to
; use other byte pointers than are currently in KCC and make it still work.
;
; First the jacket routines:

; Here at start of simulation, set up registers and stack
BYTENT:	EXCH	A,0(P)		;Save register, get return
	PUSH	P,B		;Save another register
	PUSH	P,A		;Save return address again
	DMOVE	A,15		;Put registers where caller expects them
	RET			;Go run main routine

; Here when simulation complete, return value in R
BYTRET:	MOVE	15,A		;Get return value
	POP	P,B		;Restore registers
	POP	P,A
	RET			;Return from opcode simulation

; PTRSIZ - return bytes/word of pointer
; Call with B/pointer, returns S/size

PTRSIZ:	CAML	B,[444500,,0]	;OWGBP?
	 JUMPL	B,PTRSZ0	;Yes, handle differently
	PUSH	P,A		;Save registers
	PUSH	P,B
	LDB	S,[POINT 6,B,11] ;No, just get S field
	MOVEI	A,^D36		;Get number of bits per word
	IDIV	A,S		;Make number of bytes per word
	MOVE	S,A		;Put where caller expects it
	POP	P,B
	POP	P,A
	RET			;And return with it

PTRSZ0:	LDB	S,[POINT 6,B,5]	;OWG, get P field
	HLRZ	S,OWGSIZ-45(S)	;Find size for that one
	RET			;Return with it

; For each OWG P field we have an entry in the following with
; LH/bytes per word, RH/offset in word
OWGSIZ:	6,,-1			;45 = (POINT 6,)
	6,,0			;46
	6,,1			;47
	6,,2			;50
	6,,3			;51
	6,,4			;52
	6,,5			;53
	4,,-1			;54 = (POINT 8,)
	4,,0			;55
	4,,1			;56
	4,,2			;57
	4,,3			;60
	5,,-1			;61 = (POINT 7,)
	5,,0			;62
	5,,1			;63
	5,,2			;64
	5,,3			;65
	5,,4			;66
	4,,-1			;67 = (POINT 9,)
	4,,0			;70
	4,,1			;71
	4,,2			;72
	4,,3			;73
	2,,-1			;74 = (POINT 18,)
	2,,0			;75
	2,,1			;76

; $ADBJP - simulate ADJBP instruction for losing CPUs

$ADJBP:	PUSHJ P,BYTENT		;Get count and pointer
	PUSH	P,B		;Put pointer in a safe place
	PUSHJ P,PTRSIZ		;Get byte size of pointer
	IDIV	A,S		;Get num words in A, num bytes in B
	JUMPGE	B,ADJBP0	;If non-negative, ok
	ADD	B,S		;Negative, make positive
	SUBI	A,1		;And fix up word count
ADJBP0:	POP	P,S		;Get byte pointer back
	ADD	A,S		;Offset by word count
ADJBP1:	JUMPE	B,BYTRET	;If no byte offset, done
	IBP	A		;Offset by one
	SOJA	B,ADJBP1	;Back for another

; $SUBBP - subtract one byte pointer from another
;	PUSHJ	P,$SUBBP
;	IFIW	R,ADDR
;	ADJBP	R,ADDR
; should leave R as it was originally.

$SUBBP:	PUSHJ P,BYTENT		;Copy pointers in A and B
	PUSHJ P,$BPCNT		;Convert copy of B still in S into count in 15
	MOVE	16,A		;Now get other pointer
	MOVN	A,15		;And save count negated
	PUSHJ P,$BPCNT		;Convert first pointer to count to count
	ADD	A,15		;Combine results
	JRST	BYTRET		;Thats all!

; $BPCNT - turn byte pointer into pure number
; suitable for comparison, subtraction, etc.
; Even works with indirect and indexed pointers.

$BPCNT:	PUSHJ P,BYTENT		;Get byte pointer
	PUSHJ P,PTRSIZ		;Find number of bytes per word
	MOVE	A,B		;Get pointer
	CAML	B,[444500,,0]	;OWGBP?
	 JUMPL	B,BPCNT0	;Yes, handle differently
	TLZ	A,777700	;Not, clear out P and S
	TLO	A,400000	;Make IFIW
	XMOVEI	A,(A)		;Canonicalize
	IMUL	A,S		;Make number of bytes
	PUSH	P,A		;Save
	LDB	S,[POINT 6,B,11] ;Get S field
	MOVEI	A,^D36		;Get bits per word
	LSH	B,-^D30		;Get P field
	SUB	A,B		;Negate mod 36 to number of bits from start
	IDIV	A,S		;Make number of bytes that fit in that
	SUBI	A,1		;And a pinch to grow an inch
	POP	P,S		;Get count back
	ADD	A,S		;Add in
	JRST	BYTRET		;All done

; Here to count OWGBP
BPCNT0:	TLZ	A,770000	;Get just address part, make absolute
	IMUL	A,S		;Turn into number of bytes
	LSH	B,-^D30		;Get PS field again
	HRRE	B,OWGSIZ-45(B)	;Byte offset
	ADD	A,B		;Add it up
	JRST	BYTRET		;All done

; This is where the code came from:
;<KCC.LIB>DFIX.FAI.6, 15-Mar-85 12:38:40, Edit by KRONJ
; New reentrant fixed register calling conventions
;
;	TITLE	DFIX - Double precision conversions for C
;	SUBTTL	David Eppstein / Stanford University / 31-May-1984

; Convert double precision to integer
;
; 	PUSHJ	17,$DFIX
;
; takes double from registers 15 and 16, leaves int in 15.
; Mostly stolen from Footran.

$DFIX:
#if CENV_DFL_H+CENV_DFL_S	/* Hardware or software fmt */
	PUSH	P,A		;Save a register
	HLRE	A,R		;This mattered when shifts were bit-at-a-time
	ASH	A,-^D9		;Get just exponent
	JUMPGE	R,DFIX0		;Positive?
#if CENV_DFL_H
	DMOVN	R,R		;No, negate, orig sign still in 1B0[A]
#endif
#if CENV_DFL_S
	DFN	R,R+1
#endif
	TRC	A,777777	;Watch for diff between twos and ones comp
DFIX0:	TLZ	R,777000	;Bash exponent and sign ... now positive
#if CENV_DFL_S
	LSH	S,^D8		; Flush exponent from low order word.
#endif
	ASHC	R,-233(A)	;Make an integer (may overflow)
	TLNE	A,(1B0)		;Original negative?
	 MOVN	R,R		;Yup, negate first
	POP	P,A		;Restore registers
#endif /* H+S */

#if CENV_DFL_G
GFIXR==<026000,,0>
	EXTEND R,[GFIXR R]
#endif
	RET

; Convert integer to double precision
;
; 	PUSHJ	17,$DFLOT
;
; takes int from register 16, leaves double in 15 and 16.
; Again, stolen from Footran.

$DFLOT:
#if CENV_DFL_H
	; Hardware format
	LSHC	R,^D36		;Shift integer into R, zero 16
	ASHC	R,-10		;Shift out to make room for exponent
	TLC	R,243000	;Put exponent in place
	DFAD	R,[ 0
		    0 ]		;Normalize by adding zero
#endif
#if CENV_DFL_S
	PUSH P,14
	MOVE 14,R
	SETZ 15,		; Get integer in 14, zero low order wd
	ASHC 14,-10		; Shift out to make room for exponent
	TLC 14,243000		; Put exponent in place
	ASH 15,-10		; Shift low order wd for more exponent room
	TLZ 15,777000		; Clear all bits of low wd sign+exp
	TLO 15,<243000>-<033000>	; Low exp is 27. less than high exp

	; Normalize by adding zero
	UFA 15,[0]	; Sum of low parts to A+2
	FADL 14,[0]	; Sum of high parts to A,A+1
	UFA 15,16	; Add low part of high sum to A+2
	FADL 14,16	; Add low sum to high sum

	; Now return the double
	MOVE 16,15
	MOVE 15,14
	POP P,14
#endif

#if CENV_DFL_G
GFLTR==<030000,,0>
	EXTEND R,[GFLTR 16]
#endif
	POPJ	P,		;All done

; Where this code came from:
;<KCC.LIB>SPUSH.FAI.12, 15-Mar-85 13:12:39, Edit by KRONJ
; New reentrant calling conventions
;
;	TITLE	SPUSH - Push structure as function argument for KCC
;	SUBTTL	David Eppstein / Stanford University / 3-Jan-85
;
;	PUSHJ	17,$SPUSH
;
; Pushes the number of words held in AC15 onto the stack.
; AC16 holds the address of the struct to push.
; AC15 should be the same on return.
; The count must be at least two for this to work correctly.
; KLH: the non-ADJSP version ought to be made better.

$SPUSH:	PUSH	P,S		;Save address
	MOVE	S,P		;To get another stack pointer

#if CENV_ADJSP
	ADJSP P,-2(R)	;Make enough room for struct
#else
	MOVEI R,-2(R)
	HRLI R,(R)
	ADD P,R
	MOVEI R,2(R)
#endif /* No ADJSP */

	PUSH	P,A		;Then save a register
	PUSH	P,R		;Save count
	POP	S,A		;To get address back
	POP	S,R		;And return address
	EXCH	R,-1(P)		;Save ret addr where saved A was
	PUSH	P,R		;And save old A again
	MOVE	R,-1(P)		;Then get back count

; Stack now contains space for struct, then ret addr, then R, then A
; R contains count, A contains addr, S points to bottom of space - 1
SMOVE:	PUSH	S,(A)		;Move a word to stack
	ADDI	A,1		;Prepare to move on
	SOJG	R,SMOVE		;Loop until done
	POP	P,A		;Restore saved register
	POP	P,R		;And count
	POPJ	17,		;And return to caller


;	PUSHJ	17,$SPOP
;
; Pops a stacked structure into the given address.
; Same calling conventions as $SPUSH.
; The stack pointer is left unchanged.
; Note that we go from bottom to top, so it is safe for
; the source and destination to overlap.

$SPOP:	PUSH	17,R		;Save count
	PUSH	17,A		;Get another register
	XMOVEI	A,-2(17)	;Point just above end of structure
	SUB	A,R		;Point to begining of source
	SUBI	S,1		;Point one before beginning of dest
	JRST	SMOVE		;Go do move loop

#endasm		/* Finally end moby assembler section! */
