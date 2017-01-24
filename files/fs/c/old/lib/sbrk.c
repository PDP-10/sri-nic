/*
** sbrk() and brk()
** low level memory allocation for KCC runtimes
*/

#include "c-env.h"

entry brk, sbrk;

static char *curbrk = 0;
extern char *end;

/*
** char *brk(addr)
** char *addr;
**
** Sets the current idea of the break to the given byte pointer
** (which is not checked particularly closely, so don't feed it garbage).
*/

#if (SYS_T20+SYS_10X+SYS_WAITS+SYS_ITS)==0
brk()
{
	return -1;
}
#else

static brk(); /* really not, but we don't want to produce an EXTERN */

#asm

BRK:	MOVE 6,-1(17)			/* Get arg char ptr */
#if SYS_T20
	JUMPL 6,.BRK0			/* OWGBP means extended addressing */
#endif
	HRRZ 1,END			/* Get hard start (assume wd align) */
	TLNN 6,77			/* Is pointer overflowed */
	 CAILE 1,(6)			/* Or are we going back too far? */
	 JRST $RETN			/* Either way, return lossage */

	/*
	** TENEX and TOPS20 assume the first reference will create
	** whatever new pages are necessary, but ITS must explicitly
	** create the pages; unexpected references are assumed to be
	** errors.  WAITS also requires explicit allocation, I think.
	*/

#if SYS_ITS

PG$BTS==12				/* log 2 of ITS page size */
PG$SIZ==2000				/* number of words in ITS page */
PG$MSK==<PG$SIZ-1>			/* mask for address within page */

	MOVEI 1,-1(6)			/* Find highest address we'll need */
	LSH 1,-PG$BTS			/* As page number */
	SKIPN 2,CURBRK			/* Get previously set break */
	 MOVE 2,END			/* Initialize if necessary */
	MOVEI 2,-1(2)			/* As word address */
	LSH 2,-PG$BTS			/* And as page address */
	SUBI 1,(2)			/* Find # of pages we need to gobble */
	JUMPLE 1,.BRK9			/* No new pages needed, go on */
	IMUL 1,[-1,,0]			/* Get (- # pages) in left half */
	HRRI 1,1(2)			/* And 1st new page in rt for AOBJN */
	.CALL [	SETZ
		SIXBIT /CORBLK/
		MOVEI %CBNDR+%CBNDW
		MOVEI %JSELF
		1
		SETZI %JSNEW]		/* Ask ITS for the memory */
	 JRST $RETN			/* Shouldn't fail but lose nicely */
.BRK9:
#endif /* ITS */
#if SYS_WAITS
	MOVEI 1,(6)			/* Get word address (1st free) */
	CORE 1,				/* ask for more core */
	 JRST $RETN			/* no space, tough */
#endif /* WAITS */

	MOVEM 6,CURBRK			/* Set up new break address */
	JRST $RETZ			/* Return 0 for success */

/*
** brk() for extended addressing.
** This needs to be made more efficient someday.
*/

#if SYS_T20
	SEARCH MONSYM			/* Get syms for SMAP% */
	EXTERN $SUBBP			/* And routine to compare bps */

.brk0:	MOVE 6,-1(17)			/* Get ptr to desired new break */
	MOVE 15,6			/* Copy BP here */
	MOVE 16,END			/* And other one here */
	PUSHJ 17,$SUBBP			/* Subtract this one from END */
	SKIPGE 15			/* Never drop below END */
	 JRST $RETN			/* Did, fail */
	HLRZ	5,6		;Get new break swapped
	TRZ	5,770000	;Isolate section number
	SKIPN	4,CURBRK	;Get current break pointer
	 MOVE	4,END		; Initialize if necessary
	HLRZS	4
	TRZ	4,770000	;Again, isolate section number
	CAMG	5,4		;Is new greater than old?
	 JRST	.brk1		;No, check less
	SETZ	1,		;Create new sections
	HRRZ	2,4		;Get current section
	ADDI	2,1		;Point to next available
	SUB	5,4		;Get number of sections to make
	MOVE	3,5		;In AC3
.brk01:	HRLI	2,.FHSLF	;For me, of course
	SMAP%			;Make them or remove them
	 ERJMP	$RETN		;Probably ran out
.brk02:	MOVEM	6,CURBRK	;Save new break
	JRST	$RETZ		;Return success

.brk1:	CAML	5,4		;Is new less than old
	 JRST	.brk02		;Still within same section
	SETO	1,		;Delete sections
	HRRZ	2,5		;Get new (lower) section
	ADDI	2,1		;Add one as starting section to return
	SUB	4,5		;Get number to return
	MOVE	3,4		;Put it where SMAP% can get it
	JRST	.brk01		;Join common code
#endif /* T20 */

#endasm
#endif

/*
** sbrk()
**
** Since the operating system does all the virtual address space
** management, we can allocate things a char at a time, so we don't
** need to take the time to round all our byte counts to words.
** This code should work on any machine as long as brk() works.
**
** Note the strange calling conventions:
**    brk() returns -1 on failure, 0 on success
**    we return -1 on failure, the old break on success
** luckily -1 is not a valid PDP-10 byte pointer.
**
** If nbytes is not positive, we simply return the current break
** without taking the extra effort of calling brk().
*/

char *sbrk (nbytes)
unsigned nbytes;
{
    char *obrk = (curbrk != 0? curbrk : end); /* save old break */
    return (nbytes > 0 && brk (obrk + nbytes)? (char *) -1 : obrk);
}
