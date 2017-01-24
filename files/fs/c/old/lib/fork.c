/*
** Forking and program execution
**
** execl(), execle(), execv(), execve(), execlp(), execvp()
** fork(), vfork()
*/

#include "c-env.h"

entry execl, execle, execv, execve, execlp, execvp;
entry fork, vfork;

/*
** Stubs for non-forking operating system execution.
** Failure return for any of the routines herein is -1.
*/

#if (SYS_T20+SYS_10X)==0
execl()  { execv(); }
execle() { execv(); }
execve() { execv(); }
execlp() { execv(); }
execvp() { execv(); }
execv()  { return -1; }

vfork()  { fork(); }
fork()   { return -1; }
#endif

/*
** Rest of file is in SYS_T20+SYS_10X conditionals.
**
** First the outer exec() calls.  These all call a common
** handler routine, doexec(), which in turn calls the
** assembler routine _exec() to actually run the program.
*/

#if SYS_T20+SYS_10X

#define	GJOLD	0100000000000		/* get JFN for existing file */
#define NULL 0

execl(prog, arg)			/* prog name, arg, arg, ..., 0 */
{
    doexec(prog, &arg, -1);
}

execlp (prog, arg)			/* ditto but look in SYS: */
{
    doexec(prog, &arg, -1);
}

execle(prog, arg)			/* prog name, arg, arg, ..., 0, envp */
{
    doexec(prog, &arg, -1);
}

execv(prog, argv)			/* prog name, arglist */
{
    doexec(prog, argv, 1);
}

execvp(prog, argv)			/* ditto but look in SYS: */
{
    doexec(prog, argv,1);
}

execve(prog, argv)			/* prog name, arglist, envp */
{
    doexec(prog, argv, 1);
}

/*
** Find executable file and set up command line buffer
** Intermediate handler between execX() and _exec()
*/

static doexec(prog, arg, inc)
char *prog, **arg;
{
    char buf[400], *s, *bprog;
    int jfn;

    /* first find the program we want to run */
    s = buf;				/* start at top */
#if SYS_T20
    *s = 'S'; *++s = 'Y'; *++s = 'S'; *++s = ':'; /* make SYS: */
#else
    strcpy(buf, "<SUBSYS>");
    s += strlen(buf)-1;
#endif
    bprog = s+1;
    while ((*++s = *prog++) && *s != '|') ; /* Add prog name, break on vbar */
    *s = '.';				/* and ".EXE" */
#if SYS_T20
    *++s = 'E'; *++s = 'X'; *++s = 'E';
#else
    *++s = 'S'; *++s = 'A'; *++s = 'V';
#endif
    *++s = '\0';
    jfn = _gtjfn(buf, GJOLD);		/* try SYS:prog.EXE */
    if (jfn < 1) {
	jfn = _gtjfn(bprog, GJOLD);	/* no, prog.EXE */
	if (jfn < 1) {			/* neither worked */
	    *(s - 4) = '\0';		/* drop null to block .EXE or .SAV */
	    jfn = _gtjfn(buf, GJOLD);	/* and try SYS:prog */
	    if (jfn < 1) {
		jfn = _gtjfn(bprog, GJOLD); /* still no, just try prog */
		if (jfn < 1) return;	/* still no, lose */
	    }
	}
    }

    /* now we have a program, build its argument string */
    for (s = buf; *arg != NULL; arg += inc) if (*s = **arg) {
    	while (*++s = *++*arg) ;	/* add each non-null arg to string */
	*s++ = ' ';			/* and delimit with a space */
    }
    if (s != buf) *(s-1) = '\n';	/* tie off with linefeed over space */
    *s = '\0';				/* and a null */

    /* all set up, go run the program */
    _exec(buf, jfn);			/* set up RSCAN and chain to program */
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

static _exec();				/* trick into no extern */

#asm
	SEARCH MONSYM			/* get JSYS defs */

RCONTS:					/* where to copy regs from */
	-1				/* AC1  unmapping */
	.FHSLF,,0			/* AC2  into self starting page 0 */
#if SYS_T20
	PM%CNT!PM%EPN!1000		/* AC3  to bottom of the section */
	.FHSLF,,0			/* AC4  doing GET% into this process */
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
#else /* end T20, begin 10X */
	1000				/* AC3  TENEX has no PM%CNT */
	.FHSLF,,0			/* AC4  doing GET% into this process */
	PMAP%				/* AC5  unmap a page */
	ADDI	2,1			/* AC6  point to next */
	SOJG	3,5			/* AC7  loop until all unmapped */
	MOVE	1,4			/* AC10 get back GET% argument */
	GET%				/* AC11 map new program into ourself */
	MOVEI	1,.FHSLF		/* AC12 this process again */
	SETZ	2,			/* AC13 main entry point */
	SFRKV%				/* AC14 start ourself */
#endif /* 10X */
	NREGS==.-RCONTS			/* remember how many to copy */

.EXEC:
#if SYS_T20
	%CHRBP	1,-1(17)		/* Get RSCAN buffer as ILDB pointer */
	RSCAN%				/* Set it */
	 ERJMP	$RETF			/* Lost */
#endif /* T20 */
	SKIPE	vforkf			/* If in vfork() */
	 HALTF%				/* Stop and let superior catch up */
	MOVE	0,[RCONTS,,1]		/* Get BLT pointer to set regs */
	BLT 	0,NREGS			/* Copy all the registers across */
	HRR	4,-2(17)		/* Get JFN in right half */
	JRST	@[5]			/* Jump to section 0 regs */
#endasm

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
** superior wait until a HALTF% in exec(), triggered by vforkf != 0.
*/

static int vforkf = 0;

#asm
vfork:
	POP	17,16			/* Get ret addr off stack for safety */
	MOVE	1,[CR%MAP!CR%CAP!CR%ACS!CR%ST!vforkr] /* Set fork going */
	SETZ	2,			/* Copying registers from ours */
	CFORK%				/* Make a fork */
	 ERJMPA	$RETN			/* Lose lose */
	WFORK%				/* Wait for it to do exec */
	HRLI	1,(SF%CON)		/* Continuing after HALTF% */
	SFORK%				/* Start it again */
	SETZM	vforkf			/* No longer inside vfork */
	HRRZ	1,1			/* Just get fork handle */
	JRST	(16)			/* All done */

vforkr:
	SETOM	vforkf			/* Set flag for inside vfork */
	SETZ	1,			/* This is inferior fork */
	JRST	(16)			/* Return */
#endasm

/*
** fork()
** This does safe map copy before starting inferior
*/

#asm
	EXTERN $MAPSC			/* in CRT */

#if SYS_T20
	NSECTS==40
#else
	NSECTS==1
#endif

fork:
	MOVE	1,[CR%CAP!CR%ACS]	/* Same caps and regs as us */
	SETZ	2,			/* Copying registers from ours */
	CFORK%				/* Make a fork */
	 ERJMPA	$RETN			/* Lose lose */
	MOVE	6,1			/* Copy handle to safer place */
	MOVEI	7,.FHSLF		/* Mapping from self */

	/*
	** Set inferior map looking like ours.  Can't just do fork-to-fork
	** PMAP% or CR%MAP CFORK% because then any changes
	** in impure data in the mother fork would be reflected in
	** the daughter (but not vice versa if PM%CPY set)
	*/

	MOVEI	5,NSECTS		/* Counting off all sections */
	SETZB	10,11			/* Start with sect zero in each fork */
	PUSHJ	17,$mapsc		/* Map section across */
	SOJG	5,.-1			/* Until we are done */

	/* Map all set, now we can safely start the subfork and return. */
	MOVE	1,6			/* Pages all mapped, recover handle */
	XMOVEI	2,$RETF			/* Set subfork to return zero */
	TLNE	2,-1			/* If addr is in non-zero section, */
	 JRST	fork8			/* must start using extended call */
	SFORK%
	POPJ	17,			/* Return with fork handle in AC1 */
fork8:	MOVE	3,2
	SETZ	2,
	XSFRK%				/*  Start it at extended address. */
	POPJ	17,
#endasm
#endif /* T20+10X */
