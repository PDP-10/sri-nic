/* PIPE
 */
#include "c-env.h"

entry pipe;

#include "urtio.h"

#if SYS_T20+SYS_10X
/* ------------------ */
/*      get pipe      */
/* ------------------ */

pipe(fildes)
int fildes[2];
{
    fildes[0] = _uiofd();		/* get a file descriptor */
    _uioch[fildes[0]] = 0;		/* don't get it again */
    fildes[1] = _uiofd();		/* another one for output */
    return _pipe(&_uioch[fildes[0]],&_uioch[fildes[1]]); /* fill pipes in */
}

static _pipe();
#asm
; *************************************************************
;	low-level support for pipe()
;	_pipe(read,write)
;	int *read,*write;
;
;	fildes[0] gets read end, fildes[1] gets write
;	returns success or failure
; *************************************************************
	SEARCH MONSYM

.PIPE:	MOVE	1,[GJ%SHT]	;Using short form
	HRROI	2,[ASCIZ/PIP:/]	;A pipe with default size buffers
	GTJFN%			;Create an instance of a pipe device
	 ERJMPA	$RETN
	MOVEM	1,@-2(17)	;Store it away
	MOVE	2,@-2(17)	;Get back the first JFN
	ADJSP	17,20		;Make some space
	HRROI	1,-17(17)	;And point to it
	MOVE	3,[1B2!1B8!JS%PAF] ;Output device and filename w/punc
	JFNS%			;Build a second string for GTJFN%
	MOVEI	3,"."		;Get dot to separate name and extension
	IDPB	3,1		;Tack on the dot
	MOVE	2,@-2-20(17)	;Get back that JFN
	MOVE	3,[1B8]		;Output filename again, no punc.
	JFNS%			;Create "PIP:#.#"
	SETZ	3,		;Get a null byte
	IDPB	3,1		;Tie off second GTJFN% string
	MOVE	1,[GJ%SHT]	;Using short form
	HRROI	2,-17(17)	;Point to string we just built
	GTJFN%			;Get a JFN on the other side of pipe
	 ERJMPA [ ADJSP	17,-20
		 JRST $RETN ]
	ADJSP	17,-20		;Give back temp space
	MOVEM	1,@-1(17)	;Use that as the read side
	MOVE	1,@-2(17)	;Get the JFN for the write side
	MOVE	2,[7B5!OF%WR]	;7-bits, write access
	OPENF%			;Create write side
	 ERJMPA	$RETN
	MOVE	1,@-1(17)	;Get the JFN to use for the read side
	MOVE	2,[7B5!OF%RD]	;7-bits, read-access
	OPENF%			;Create read side
	 ERJMPA	$RETN
	JRST	$RETF		;Success return value is 0
#endasm
#endif /* T20+10X */

#if SYS_WAITS
/* ------------------ */
/*      get pipe      */
/* ------------------ */

pipe(fildes)
int fildes[2];
{
    return -1;				/* pipes not implemented, fail */
}
#endif /* WAITS */

#if SYS_ITS
pipe()
{	return(-1);	/* Not supported yet */
}
#endif /* ITS */
