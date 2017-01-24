/* ----------------------------------- */
/*      duplicate file descriptor      */
/* ----------------------------------- */
#include "c-env.h"

entry dup, dup2;

#include "urtio.h"

dup(fd)
{
    int	newfd;

    newfd = _uiofd();
    dup2(fd,newfd);
    return newfd;
}

dup2(from,to)
{
    if (_uioch[from] < 0) return -1;
#if	SYS_T20+SYS_10X
    if(to == 0) {			/* redirecting PRIIN */
	_spjfn(_uioch[from],0);
	_uioch[from] = UIO_UNASS;
	return 1;
    }
    if(to == 1) {			/* redirecting PRIOU */
	_spjfn(0,_uioch[from]);
	_uioch[from] = UIO_UNASS;
	return 1;
    }
#endif
    if (_uioch[to] != UIO_UNASS && _uioch[to] > 2) close(to);
    _uioch[to] = _uioch[from];
    return 1;
}

#if SYS_T20+SYS_10X

static _spjfn();
#asm
; *************************************************************
;	low-level support for dup()
;	_spjfn(in,out)
;	sets .PRIIN to go to JFN in IN, .PRIOU to OUT
;	if either is zero, remains unchanged
; *************************************************************
	SEARCH MONSYM

.SPJFN:	MOVEI	1,.FHSLF	;On ourself
	GPJFN%			;Get input redirection
	SKIPE	-1(17)		;If want input redirected
	 HRL	2,-1(17)	;Input in left half
	SKIPE	-2(17)		;If want output redirected
	HRR	2,-2(17)	;Output in right half
	SPJFN%			;Set primary input JFNs
	POPJ	17,
#endasm
#endif /* T20+10X */
