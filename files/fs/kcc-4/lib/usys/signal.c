/* SIGNAL - Simulate 4.3BSD signal mechanism, via sigvec()
**
**	Copyright (c) 1987 by Ken Harrenstien, SRI International
**
** See the file SIGNAL.DOC for more background information on this code.
*/

#include "c-env.h"
#include "errno.h"
#include "signal.h"
#include "sys/usysig.h"		/* USYS_ macros */

typedef void (*hndlr_t)();	/* Type of sv_handler (for convenience) */

hndlr_t
signal(sig, func)
int sig;
hndlr_t func;
{
    struct sigvec sv;
    hndlr_t ofunc;

    USYS_BEG();
    if (sigvec(sig, (struct sigvec *)0, &sv) != 0)	/* Get old sigvec */
	USYS_RET(SIG_ERR);	/* errno already set to EINVAL by sigvec() */
    ofunc = sv.sv_handler;	/* Remember old handler */
    sv.sv_handler = func;	/* Set up new sigvec for this signal */
    if (sigvec(sig, &sv, (struct sigvec *)0) != 0)	/* Set new sigvec */
	USYS_RET(SIG_ERR);
    USYS_RET(ofunc);		/* Won, return previous handler */
}
