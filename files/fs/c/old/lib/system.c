#include <stdio.h>
main()
{
	char	testline[100];

	printf("Enter test line: ");
	fgets(testline,99,stdin);
	*(testline+strlen(testline)-1) = 0;
	system(testline);
	printf("\nDone.\n");
}

system(s)
char *s;
{
	char *p, t[256], q[256];
	char *index();

	strcpy(t,"SYS:");
	strcat(t,s);
	p = t;
	if (p = index(t,' '))
		*p = '\0';
	strcat(t,".EXE");
	strcpy(q,s);
	strcat(q,"\n");

/* _xfork(prg_name,cmdline,start_offset,chainf) */

	_xfork(t,q,0,0);
}

static _xfork();
#asm
	search	monsym

RCONTS:					/* where to copy regs from */
	-1				/* AC1  unmapping */
	.FHSLF,,0			/* AC2  into self starting page 0 */
	PM%CNT!PM%EPN!1000		/* AC3  to bottom of the section */
	.FHSLF,,0			/* AC4  GET% into this process */
	PMAP%				/* AC5  unmap section zero */
	HRRI	2,1			/* AC6  make pointer to section 1 */
	MOVEI	3,37			/* AC7  count of sections to unmap */
	SMAP%				/* AC10 unmap all nonzero sections */
	ERJMP	12			/* AC11 win with pre-rel-5 TOPS-20 */
	MOVE	1,4			/* AC12 get back GET% argument */
	GET%				/* AC13 map new program into ourself */
	MOVEI	1,.FHSLF		/* AC14 this process again */
	SETZ	2,			/* AC15 main entry point */
	SFRKV%				/* AC16 start ourself */
	NREGS==.-RCONTS			/* remember how many to copy */

.xfork:
	movsi	1,(gj%old+gj%sht)
	%chrbp	2,-1(17)		; get pointer to program name
	gtjfn%				; get JFN of program file
	 erjmp	$retz			; cannot open exe file
	hrrzm	1,jfn			; save JFN

	skipe	-4(17)			; chain or run subfork?
	 jrst 	chain			; chain
	hrli	1,(cr%cap)		; run subfork
	cfork%				; create inferior with same capability
	 erjmp	$retz			; cannot create fork
	movem	1,frk			; save fork handle
	ffork%				; and freeze the fork
	
	hrlz	1,frk			; fork handle
	hrr	1,jfn			; JFN of program file
	setzm	2
	get%				; load program image into inferior fork
	 erjmp	$retz

	hrrz	1,frk
	gevec%				; get entry vector
	 erjmp	$retz			; oops??
	add	2,-3(17)		; get offset
	hrrz	2,2
	movem	2,pc

	%CHRBP	1,-2(17)		/* Get RSCAN buffer as ILDB pointer */
	RSCAN%				/* Set it */
	 ERJMP	$RETF			/* Lost */

	hrrz	1,frk
	move	2,pc
	sfork%				; start fork
	rfork%				; resume frozen fork
	wfork%				; wait for fork to terminate
	kfork%				; then kill fork
	jrst $rett

chain:
	%CHRBP	1,-2(17)		/* Get RSCAN buffer as ILDB pointer */
	RSCAN%				/* Set it */
	 ERJMP	$RETF			/* Lost */
	MOVE	0,[RCONTS,,1]		/* Get BLT pointer to set regs */
	BLT 	0,NREGS			/* Copy all the registers across */
	HRR	4,jfn			/* Get JFN in right half */
	JRST	@[5]			/* Jump to section 0 regs */


	$$DATA

jfn:	0				; program file JFN
frk:	0				; fork handle
pc:	0				; starting pc of process

	$$CODE

#endasm
