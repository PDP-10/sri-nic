/* MUUO and CALLI - TOPS-10/WAITS monitor call interface for KCC.
**
**	Copyright (c) 1986 by Ken Harrenstien, SRI International
**
** MUUO(instr, D, E)	- Used to implement all MUUOs except CALL and CALLI.
**		"instr" is a MUUO instruction value (with AC,I,X,E fields 0)
** CALLI(n, acval, &retval) - Implements CALLI.
**		"n" is a CALLI index number (E field of CALLI).
**		"acval" is the desired contents of the AC for the call.
**		 New AC value returned to *retval (unless retval == NULL).
**
**	All calls return 1 if the MUUO skips, 0 if it doesn't.
**	Whether or not 0 is an error depends on the specific call.
*/

int
muuo(instr, d, e)
int instr, d, *e;
{
#asm
	EXTERN ABORT

	MOVE 3,-2(17)		; Get the AC field value
	CAILE 3,17		; Make sure it's reasonable
	 PUSHJ 17,ABORT		; Ugh, drop dead.
	LSH 3,27		; Shift into AC field
	IOR 3,-1(17)		; Add the MUUO
	HRRZ 3,-3(17)		; and the address (must not be extended!)
	XCT 3			; Execute it
	 TDZA 1,1		; Returned .+1, so return 0
	  MOVEI 1,1		; Returned .+2, so return 1
#endasm
}

int
calli(n, acval, retval)
int n, acval, *retval;
{
#asm
	MOVE 2,-2(17)		; Get the value to put into AC
	CALLI 2,@-1(17)		; Execute CALLI 2,n
	 TDZA 1,1		; Returned .+1, so return 0
	  MOVEI 1,1		; Returned .+2, so return 1
	SKIPE 3,-3(17)		; Get address for return value, if any
	 MOVEM 2,(3)		; Store it!
#endasm
}
