/************************************************************
 *	int setjmp(env);	- non-local goto
 *	jmp_buf env;
 *
 *	longjmp(env,val);
 *	jmp_buf env;
 *	int val;
 ************************************************************/

#include "c-env.h"

entry setjmp,longjmp;

#asm
setjmp:	MOVE	2,-1(17)	;Get pointer to block
	POP	17,0(2)		;Put return PC in first word
	MOVEM	17,1(2)		;Drop stack in other word
	SETZ	1,		;Returning zero
	JRST	@0(2)		;Return to caller


longjmp:
	MOVE	1,-2(17)	;Get return value
	MOVE	2,-1(17)	;And environment block
	MOVE	17,1(2)		;Get stack back
	JRST	@0(2)		;Return to caller of setjmp

#endasm
