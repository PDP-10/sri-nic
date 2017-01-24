/*
** access(2) - determine accessibility of file.
** David Eppstein / Stanford University / 9-Aug-84
** TENEX additions by Ken Harrenstien, SRI 1-Jun-85
**	TENEX does not have the CHKAC JSYS.  This routine will at the moment
**	always succeed on TENEX (unless the file doesn't exist).
**	Eventually a simulation can be coded.
*/

#include "c-env.h"
#include "errno.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif
#include "sys/file.h"
#include "sys/usysig.h"

int access(filnam, mode)
char *filnam;
int  mode;
{
#if SYS_T20+SYS_10X
    int jfn;

    USYS_BEG();
    if (!(jfn = _gtjfn(filnam, O_RDONLY)))	/* get JFN on the file */
	USYS_RETERR(ENOENT);	/* Can't trust _gtjfn to set errno right! */
#if SYS_10X
	_rljfn(jfn);
	USYS_RET(0);		/* If TENEX always claim win for now,
				 * because we don't have CHKAC% */
#endif
#if SYS_T20
    if ( ((mode & R_OK) && !_chkac(_CKARD, jfn))	/* can't read? */
      || ((mode & W_OK) && !_chkac(_CKAWR, jfn))	/* can't write? */
      || ((mode & X_OK) && !_chkac(_CKAEX, jfn)) )	/* can't execute? */
	USYS_RETERR(EACCES);		/* Say permission denied */
    _rljfn(jfn);			/* release the JFN */
    USYS_RET(0);			/* return success */
#endif /*SYS_T20*/
#else /* Not T20 or 10X */
#error access() not supported for this system.
#endif
}

#if SYS_T20
static _chkac(acc, jfn)
{
    int ablock[5], cblock[6];

    cblock[_CKAAC] = acc;		/* get desired access code */
    cblock[_CKAUD] = jfn;		/* and jfn */

    jsys(GJINF, ablock);		/* get job information */
    cblock[_CKALD] = ablock[1];		/* user number */
    cblock[_CKACD] = ablock[2];		/* and connected directory */

    ablock[1] =  _FHSLF;		/* reading our own process caps */
    jsys(RPCAP, ablock);		/* do it */
    cblock[_CKAEC] = ablock[3];		/* enabled capabilities */

    ablock[1] = 6 + CK_JFN;		/* AC1: length, flags */
    ablock[2] = (int) cblock;		/* AC2: argument block */
    jsys(CHKAC, ablock);		/* check file access */
    if (ablock[1] == -1) return 1;	/* success, return truth */
    _rljfn(jfn);			/* failure, release jfn */
    return 0;				/* and return falsehood */
}
#endif /*SYS_T20*/
