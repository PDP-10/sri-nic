/* EXEC - execl, execle, execv, execve
 *	UNIX syscall support.
 */
#include "c-env.h"

entry execl, execle, execv, execve;

#if (SYS_T20+SYS_10X)==0
execl() { execv(); }
execle() { execv(); }
execve() { execv(); }
execv()
{	return(-1);		/* EXEC call not supported */
}
#else

#define	GJOLD	0100000000000		/* get JFN for existing file */
#define NULL 0
/* ------------------------------------- */
/*      run new program in old fork      */
/* ------------------------------------- */

execl(prog, arg)
{
    doexec(prog, &arg, -1);
}

execle(prog, arg)
{
    doexec(prog, &arg, -1);
}

execv(prog, argv)
{
    doexec(prog, argv, 1);
}

execve(prog, argv)
{
    doexec(prog, argv, 1);
}

static doexec(prog, arg, inc)
char *prog, **arg;
{
    char buf[400], *s, *bprog;
    int jfn;

    /* first find the program we want to run */
    s = buf;				/* start at top */
#if SYS_10X
    strcpy(buf, "<SUBSYS>");
    s += strlen(buf)-1;
#endif
#if SYS_T20
    *s = 'S';				/* make "SYS:" */
    *++s = 'Y';
    *++s = 'S';
    *++s = ':';
#endif
    bprog = s+1;
    while ((*++s = *prog++) && *s != '|') ; /* Add prog name, break on vbar */
    *s = '.';				/* and ".EXE" */
#if SYS_10X
    *++s = 'S';
    *++s = 'A';
    *++s = 'V';
#endif
#if SYS_T20
    *++s = 'E';
    *++s = 'X';
    *++s = 'E';
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

#asm
; *************************************************************
;	low level support for exec()
; *************************************************************
	SEARCH MONSYM
.EXEC:
#if SYS_T20
	%CHRBP	1,%ARG1(P)	;Get RSCAN buffer
	RSCAN%			;Set it
	 ERJMP	$RETF		;Lost
	SETO	1,		;Unmapping
	MOVSI	2,.FHSLF	;From the top
	MOVE	3,[PM%CNT!PM%EPN!1000]	;To the bottom
	MOVE	4,%ARG2(P)	;Get JFN
	HRLI	4,.FHSLF	;For this process
	DMOVE	5,[  PMAP%		;  5	Unmap section zero
		     HRRI 2,1 ]		;  6    Make pointer to section one
	DMOVE	7,[  MOVEI 3,37		;  7    Section count to unmap
		     SMAP% ]		; 10    Unmap non-zero sections
	DMOVE	11,[ ERJMP 12		; 11    Win with pre-release-5 TOPS-20
		     MOVE 1,4 ]		; 12	Get back GET% argument
	DMOVE	13,[ GET%		; 13	Map new program into ourself
		     MOVEI 1,.FHSLF ]	; 14	This process again
	DMOVE	15,[ SETZ 2,		; 15	Main entry point
		     SFRKV% ]		; 16	Start ourself
	JRST	@[5]		;Go do it in the section zero ACs
#endif /* T20 */
#if SYS_10X
	; TENEX PMAP doesnt have PM%CNT.  Sigh.
	SETO	1,		;Unmapping
	MOVSI	2,.FHSLF	;From the bottom
	MOVEI	3,1000		;To the top
	MOVE	4,%ARG2(P)	;Get JFN
	HRLI	4,.FHSLF	;For this process
	MOVE	5,[	PMAP%	]	;  5	Unmap a page
	MOVE	6,[	ADDI 2,1]	;  6	point to next
	MOVE	7,[	SOJG 3,5]	;  7  Loop til all done
	MOVE	10,[	MOVE 1,4]	; 10	Get back GET% argument
	MOVE	11,[	GET%	]	; 11	Map new program into ourself
	MOVE	12,[	MOVEI 1,.FHSLF]	; 12	This process again
	MOVE	13,[	SETZ 2,	]	; 13	Main entry point
	MOVE	14,[	SFRKV%	]	; 14	Start ourself
	JRST	5			; Go do it!
#endif /* 10X */
#endasm

#endif /* T20+10X */
