/* <WHP4>PFORK.C.1, 10-Jul-85 12:20:00, Edit by WHP4 */
/*  go back to using JRST (2) instead of SFRKV which breaks for some reason */
/*  when chaining */
/************************************************************
 * pfork(program_name,command_block,command_block_length,start_offset,chainf);
 *
 *	create inferior fork, runs a program on it, and
 *	  wait for the program to terminate. if chainf is nonzero,
 *	  we chain to program instead and never return.
 *
 *	command_block is the argument block for the
 *	  inferior process,
 *	command_block_length is the length of the argument 
 *	  block in words,
 *	entry_offset is an offset that can be added to the
 *	  usual starting location.
 ************************************************************
 */

#include "c-env.h"

#ifdef COMMENT
;<KCC.LIB>PFORK.FAI.7, 22-Apr-85 19:04:46, Edit by WHP4
; fix from KLH for mysterious LINK SIF bug
;<KCC.LIB>PFORK.FAI.6,  4-Feb-85 01:43:55, Edit by WHP4
; fix brain-damage in previous edit
;<KCC.LIB>PFORK.FAI.3,  4-Feb-85 01:18:04, Edit by WHP4
; add param to chain through instead of running subfork
;<KCC.LIB>PFORK.FAI.2,  3-Feb-85 23:54:03, Edit by WHP4
; add param for length of prarg block
#endif COMMENT



#if (SYS_T20+SYS_10X)==0
#error pfork() not implemented for this system.
#else

static int jfn;
static int frk;				/* fork handle */
static int pc;				/* starting pc of process */

int
_pfork(pgmname,argblk,blklen,stoffset,chainf)
char *pgmname;		/* Program name to run */
int *argblk, blklen;	/* PRARG argument block */
int *stoffset, chainf;	/* Starting offset, and chain flag */
{
#asm
	SEARCH	MONSYM
	EXTERN $RETZ, $RETT		/* From CRT */

#if SYS_T20
	movsi	1,(gj%old+gj%sht)
	%chrbp	2,-1(17)		; get pointer to program name
	gtjfn%				; get JFN of program file
	 erjmp	$retz			; cannot open exe file
	hrrzm	1,jfn			; save JFN

	skipe	-5(17)			; chain or run subfork?
	 jrst 	chain			; chain
	hrli	1,(cr%cap)		; run subfork
	cfork%				; create inferior with same capability
	 erjmp	$retz			; cannot create fork
	movem	1,frk			; save fork handle
	
	hrl	1,frk			; fork handle
	hrr	1,jfn			; JFN of program file
	get%				; load program image into inferior fork
	 erjmp	$retz

	hrrz	1,frk
	gevec%				; get entry vector
	 erjmp	$retz			; oops??
	add	2,-4(17)			; get offset
	hrrz	2,2
	movem	2,pc

	hrli	1,2			; set argument
	hrr	1,frk			; fork handle
	move	2,-2(17)		; address of command block
	move	3,-3(17)		; length of block
	skipe	2			; don't if no arg block passed
	 prarg%
	  erjmp	$retz
	
	hrrz	1,frk
	move	2,pc
	sfork%				; start fork
	wfork%				; wait for fork to terminate
	kfork%				; then kill fork
	jrst $rett

chain:	movei 	1,.fhslf		;fork handle
	hrli	1,.prast		;set arguments from block
	move	2,-2(17)			;addr of command block
	move	3,-3(17)			;length of block
	skipe 	2			;don't if no arg block
	 prarg%				;send along commands
	  erjmp $retz
	seto	1,
	move	2,[.fhslf,,1]
	move 	3,[pm%cnt!777]
	move	4,jfn
	hrli	4,.FHSLF
	move 	5,[PMAP]
	move	6,[ERJMP 7]
	move 	7,[MOVE 1,4]
	move	10,[GET]
	move	11,[MOVEI 1,.FHSLF]	/* NOTE!!  If this is changed to */
	move	12,[GEVEC%]		/* SFRKV% (to make ext addr work) */
	move	13,[JRST 0(2)]		/* KCC must start LINK with 1,,0 */
	hrr 	13,-4(17)		/* (1+.JBSA rather than .JBREN) */
	jrst	@[5]
#endif /* T20 */
	

#if SYS_10X
	skipe	-5(17)		; chain or run subfork?
	 jrst 	$retz		; Cannot chain, no PRARG%
	skipn	-3(17)		; Unless argblock addr or # args is 0,
	 jrst	pfrkt0		; we take failure return since don't have
	skipe	-2(17)		; PRARG% on TENEX.
	 jrst	$retz

pfrkt0:	movsi	1,(gj%old+gj%sht)
	%chrbp	2,-1(17)		; get pointer to program name
	gtjfn%				; get JFN of program file
	 erjmpa	$retz			; cannot open exe file
	hrrzm	1,jfn			; save JFN

	hrli	1,(cr%cap)		; run subfork
	cfork%				; create inferior with same capability
	 erjmpa	$retz			; cannot create fork
	movem	1,frk			; save fork handle
	
	hrl	1,frk			; fork handle
	hrr	1,jfn			; JFN of program file
	get%				; load program image into inferior fork
	erjmp	$retz

	hrrz	1,frk
	gevec%				; get entry vector
	erjmp	$retz			; oops??
	add	2,-4(17)		; get offset
	hrrz	2,2
	movem	2,pc

	hrrz	1,frk
	move	2,pc
	sfork%				; start fork
	wfork%				; wait for fork to terminate
	kfork%				; then kill fork
	jrst $rett				; non-error return
#endif /* 10X */

#endasm
}	/* End of _pfork() */
#endif /* T20+10X */
