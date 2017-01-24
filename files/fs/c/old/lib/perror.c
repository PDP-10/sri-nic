/*	perror - print error string from system
 */
#include "c-env.h"

	entry	perror;
#if SYS_T20+SYS_10X
#asm
	search	monsym
	extern	$byte

perror:
	movei	1,.cttrm	;to controlling tty (stderr)
	%chrbp	2,-1(17)	; Get BP to start of arg string (1st arg)
	setz	3,		; Send up to null
	sout%			;send off arg string
	 erjmp	errerr
	hrroi	2,[asciz/: /]	;colon
	setz	3,		;to null
	sout%			;that too
	 erjmp	errerr
	hrloi	2,.fhslf	;last error on self
	setz	3,		;no limit
	erstr%			;make error string
	 jrst	errerr
	 jrst	errerr
	hrroi	2,[asciz/
/]				;crlf
	setz	3,		;to null
	sout%			;send off
	 erjmp	errerr
	popj	17,		;done

errerr:	hrroi	1,[asciz/Error within an error/]
	esout%
	popj	17,
#endasm
#endif

#if (SYS_T20+SYS_10X)==0
#include <stdio.h>
perror()
{	fprintf(stderr,"Error: perror() not implemented on this system\n");
}
#endif

