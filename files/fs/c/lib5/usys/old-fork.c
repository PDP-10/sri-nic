/*
** Forking and program execution
**
** execl(), execle(), execv(), execve(), execlp(), execvp()
** fork(), vfork()
*/

#include "c-env.h"
#include "errno.h"
#include "sys/usysig.h"
#include "sys/file.h"
#if SYS_T20+SYS_10X
#include "sys/usysio.h"
#endif

#ifndef NULL
#define NULL	0
#endif

/* External URT data refs */
extern int _vfrkf;	/* Non-zero if mem shared by vfork() */
extern int _nfork;	/* Bumped every time a child is created */

/*
** Stubs for non-forking operating system execution.
** Failure return for any of the routines herein is -1.
*/

#if (SYS_T20+SYS_10X)==0
#error exec*() and *fork() not supported for this system.
#endif

/*
** Rest of file is in SYS_T20+SYS_10X conditionals.
**
** First the outer exec() calls.  These all call a common
** handler routine, doexec(), which in turn calls the
** assembler routine _exec() to actually run the program.
*/
#define SRCH_ON 1
#define SRCH_OFF 0

#if SYS_T20+SYS_10X
static int doexec();
static void _exec();

execl(prog, arg)			/* prog name, arg, arg, ..., 0 */
{
    return doexec(prog, &arg, -1, SRCH_OFF);
}

execlp (prog, arg)			/* ditto but look in SYS: */
{
    return doexec(prog, &arg, -1, SRCH_ON);
}

execle(prog, arg)			/* prog name, arg, arg, ..., 0, envp */
{
    return doexec(prog, &arg, -1, SRCH_OFF);
}

execv(prog, argv)			/* prog name, arglist */
{
    return doexec(prog, argv, 1, SRCH_OFF);
}

execvp(prog, argv)			/* ditto but look in SYS: */
{
    return doexec(prog, argv, 1, SRCH_ON);
}

execve(prog, argv)			/* prog name, arglist, envp */
{
    return doexec(prog, argv, 1, SRCH_OFF);
}

/*
** Find executable file and set up command line buffer
** Intermediate handler between execX() and _exec()
*/

static int
doexec(prog, arg, inc, srchflg)
char *prog, **arg;
int srchflg;			/* TRUE if should search for file */
{
    char buf[400], *s, *bprog;
    int jfn;

    USYS_BEG();

    /* first find the program we want to run */
    s = buf;				/* start at top */
#if SYS_T20
	*s = 'S'; *++s = 'Y'; *++s = 'S'; *++s = ':'; /* make SYS: */
#else
	strcpy(buf, "<SUBSYS>");
	s += strlen(buf)-1;
#endif
	bprog = s+1;

    while ((*++s = *prog++) && *s != '|' && *s != ' ' && *s != '\n'
			    && *s != '\r' && *s != '\0') ;
    *s = '.';				/* and ".EXE" */
#if SYS_T20
    *++s = 'E'; *++s = 'X'; *++s = 'E';
#else
    *++s = 'S'; *++s = 'A'; *++s = 'V';
#endif
    *++s = '\0';
    if (!srchflg || (jfn = _gtjfn(buf, O_RDONLY)) == 0) {/* try SYS:prog.EXE */
	if ((jfn = _gtjfn(bprog, O_RDONLY)) == 0) {	/* no, prog.EXE */
	    *(s - 4) = '\0';		/* drop null to block .EXE or .SAV */
	    if (!srchflg || (jfn = _gtjfn(buf, O_RDONLY)) == 0)	/* and try SYS:prog */
		if ((jfn = _gtjfn(bprog, O_RDONLY)) == 0)	/* try prog */
			USYS_RETERR(ENOENT);	/* _gtjfn screws errno */
	}
    }

    /* now we have a program, build its argument string */
    for (s = buf; *arg != NULL; arg += inc) if (*s = **arg) {
    	while (*++s = *++*arg) ;	/* add each non-null arg to string */
	*s++ = ' ';			/* and delimit with a space */
    }
    if (s != buf) *(s-1) = '\n';	/* tie off with linefeed over space */
    *s = '\0';				/* and a null */

    /* Now must do hack to ensure that stdin and stdout are preserved
    ** over the map, and work even if new program is not a C program.
    ** This is done by setting the primary JFNs .PRIIN and .PRIOU to
    ** whatever stdin and stdout currently are.
    */
    {
	int ijfn = -1, ojfn = -1;
	if (_uioufx[STDIN_CH])  ijfn = _uioch[_uioufx[STDIN_CH]];
	if (_uioufx[STDOUT_CH]) ojfn = _uioch[_uioufx[STDOUT_CH]];
	/* Map the primary JFNs if either JFN is non-standard! */
	_set_pio((ijfn << 18) | (ojfn & 0777777));
    }

    /* all set up, go run the program */
    _exec(buf, jfn);			/* set up RSCAN and chain to program */

    USYS_RETERR(ENOEXEC);
}

/*
** Low level support for exec()
**
** _exec (buf, jfn)
**    sets the RSCAN% command line to buf, then chains to the program
**    found at the file pointed to by jfn.  never returns if successful.
**
**    if we were in vfork() we HALTF% to give the superior a chance to synch.
*/

static void
_exec(buf, jfn)
int buf, jfn;
{
#asm
	search monsym			/* Get JSYS, flag defs */
	extern .vfrkf			/* in URT */
	extern $retz, $retn		/* in CRT */
#if SYS_T20
	%chrbp	1,-1(17)		/* Get RSCAN buffer as ILDB pointer */
	rscan%				/* Set it */
	 erjmp	$retz			/* Lost */
#endif /* T20 */

	move	0,[rconts,,1]		/* Get BLT pointer to set regs */
	blt 	0,nregs			/* Copy all the registers across */
	hrr	4,-2(17)		/* Get JFN in right half */

	/* Now no longer need memory for anything, all is in the ACs. */
	skipe	.vfrkf			/* If in vfork() */
	 haltf%				/* Stop and let superior catch up */
vfksyn:	/* This PC is checked to ensure HALTF% is right one! */

	jrst	@[5]			/* Jump to section 0 regs */


rconts:					/* where to copy regs from */
	-1				/* AC1  unmapping */
	.fhslf,,0			/* AC2  into self starting page 0 */
#if SYS_T20
	pm%cnt!pm%epn!1000		/* AC3  to bottom of the section */
	.fhslf,,0			/* AC4  doing GET% into this process */
	pmap%				/* AC5  unmap section zero */
	hrri	2,1			/* AC6  make pointer to section 1 */
	movei	3,37			/* AC7  count of sections to unmap */
	smap%				/* AC10 unmap all nonzero sections */
	erjmp	12			/* AC11 win with pre-rel-5 TOPS-20 */
	move	1,4			/* AC12 get back GET% argument */
	get%				/* AC13 map new program into ourself */
	movei	1,.fhslf		/* AC14 this process again */
	setz	2,			/* AC15 main entry point */
	sfrkv%				/* AC16 start ourself */
#else /* end T20, begin 10X */
	1000				/* AC3  TENEX has no PM%CNT */
	.fhslf,,0			/* AC4  doing GET% into this process */
	pmap%				/* AC5  unmap a page */
	addi	2,1			/* AC6  point to next */
	sojg	3,5			/* AC7  loop until all unmapped */
	move	1,4			/* AC10 get back GET% argument */
	get%				/* AC11 map new program into ourself */
	movei	1,.fhslf		/* AC12 this process again */
	setz	2,			/* AC13 main entry point */
	sfrkv%				/* AC14 start ourself */
#endif /* 10X */
	nregs==.-rconts			/* remember how many to copy */
#endasm
}	/* End of _exec() */

static
_set_pio(jfns)
int jfns;
{
#asm
	movei 1,.fhslf
	gpjfn%			; Get current settings for primary i/o
	move 5,2		; Save them elsewhere
	hlre 4,-1(17)		; Check input half of arg
	jumpl 4,skipin		; If no setting, skip input stuff.
	caie 4,.priin
	 hrl 2,4		; If non-standard, set LH to new value!
skipin:	hrre 4,-1(17)		; Check output half of arg
	jumpl 4,skipo		; If no setting, skip output stuff.
	caie 4,.priou
	 hrr 2,4		; If non-standard, set RH to new value!
skipo:
	camn 2,5		; OK, is new setting any different from old?
	 popj 17,
	movei 1,.fhslf		; One is different, must map!
	spjfn%			; Do it
#endasm
}

/*
** vfork()
**
** This keeps the same map as the superior, but makes the superior
** wait for an exec() call before continuing.  Thus if all that is
** done is a store of the return value, that will get done again
** and nothing will be the worse.
**
** This cannot however be implemented merely by leaving the maps
** of the two the same, because then we wouldn't know which pid
** would be stored last.  UNIX claims to borrow the thread of control
** of the superior up to the exec(), but we fake it by making the
** superior wait until a HALTF% in exec(), triggered by _vfrkf != 0.
*/

int
vfork()
{
	asm(USYS_BEG_ASM);		/* Disable interrupts */
#asm
	pop	17,16			/* Get ret addr off stack for safety */
	move	1,[cr%map!cr%cap!cr%acs!cr%st!vforkr] /* Set fork going */
	setz	2,			/* Copying registers from ours */
	cfork%				/* Make a fork */
	 erjmpa	vfrker			/* Lose lose */
	wfork%				/* Wait for it to synch in exec() */
	move 4,1			/* Save handle (T20 may clobber AC3) */
	rfsts%				/* Read fork status to verify synch */
	exch 1,4			/* Restore fork handle */
	hlrzs 4				/* Get status code in RH */
	tlz 2,-1			/* Clear LH of PC */
	cain 4,.rfhlt			/* Must be HALTF%'d and unfrozen */
	 caie 2,vfksyn			/* at location of synch */
	  jrst vfork2
	hrli	1,(sf%con)		/* OK, continue child after synch! */
	sfork%				/* Start it again */
vfork2:	setzm .vfrkf			/* No longer inside vfork */
	aos .nfork			/* Bump count of inferiors created */
	lsh 1,11			/* Shift fork handle up by 9 bits */
	andi 1,777000			/* Zap all but low 9 bits of handle */
	push 17,1			/* Save that */
	gjinf%				/* Get job # */
	dpb 3,[001100,,(17)]		/* Store in low 9 bits of PID */
	caia
vfrker:	 push 17,[-1]			/* CFORK failed, return -1 as error */
#endasm
	asm(USYS_END_ASM);
	asm("	pop 17,1\n	jrst (16)\n");		/* All done */

	/* Child fork of vfork() starts here!
	** Note we do NOT re-enable interrupts, even though the child process
	** doesn't have its PSI system on, because our memory is still shared
	** and clearing .sigusys would permit the parent to handle signals.
	** This could cause unexpected changes to the shared memory before
	** the child gets to its exec() call.
	*/
#asm
vforkr:	setom	.vfrkf			/* Set flag saying inside vfork */
	setz	1,			/* This is inferior fork */
	jrst	(16)			/* Return */
#endasm
}

/*
** fork()
** This does safe map copy before starting inferior
*/
static int _fork();

int
fork()
{
    int ret;

    USYS_BEG();			/* Disable interrupts */
    ret = _fork();		/* Try to fork and see what we get */
    if (ret == -1)
	USYS_RETERR(EAGAIN);	/* Assume no more processes */
    _nfork++;			/* Won, bump count of inferiors created! */
    if (ret != 0)
	USYS_RET(ret);		/* We're parent, just return fork handle */

    /* Here, we're the child process.  May need to do some cleanup or
    ** init stuff here eventually, such as enabling the PSI system!
    ** As it is, we're in big trouble if a signal happens between now
    ** and an exec() call.
    */
    _nfork = 0;			/* Child has no inferiors yet */
    USYS_RET(0);
}

static int
_fork()
{
#asm
	extern $mapsc
#if SYS_T20
	nsects==40
#else
	nsects==1
#endif
	move	1,[cr%cap!cr%acs]	/* Same caps and regs as us */
	setz	2,			/* Copying registers from ours */
	cfork%				/* Make a fork */
	 erjmpa	$retn			/* Lose lose, return -1 */
	move	6,1			/* Copy handle to safer place */
	movei	7,.fhslf		/* Mapping from self */

	/*
	** Set inferior map looking like ours.  Can't just do fork-to-fork
	** PMAP% or CR%MAP CFORK% because then any changes
	** in impure data in the mother fork would be reflected in
	** the daughter (but not vice versa if PM%CPY set)
	*/

	movei	5,nsects		/* Counting off all sections */
	setzb	10,11			/* Start with sect zero in each fork */
	pushj	17,$mapsc		/* Map section across */
	sojg	5,.-1			/* Until we are done */

	/* Map all set, now we can safely start the subfork and return. */
	move	1,6			/* Pages all mapped, recover handle */
	xmovei	2,forkst		/* Set subfork to start here */
	tlne	2,-1			/* If addr is in non-zero section, */
	 jrst	fork8			/* must start using extended call */
	sfork%
	jrst fork9
fork8:	move	3,2
	setz	2,
	xsfrk%			/* Start it at extended address. */

fork9:				/* Return PID.  Fork handle in AC1 */
	move 6,1		/* Save handle */
	gjinf%			/* Get job # in AC3 */
	dpb 6,[111100,,3]	/* Put low 9 bits of handle inside PID */
	hrrz 1,3		/* Clear LH and return the PID */
	popj	17,

	/* Child fork starts here. */
	extern .pidslf, .pidpar		/* From GETPID */
forkst:	move 1,.pidslf
	movem 1,.pidpar		/* If have a PID, remember as parent's */
	setzb 1,.pidslf		/* Zap own PID to force gen of new one */
	popj 17,		/* Return 0 as child's result for fork() */
#endasm
}	/* End of fork() */
#endif /* T20+10X */
