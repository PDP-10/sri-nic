/*
** access(2) - determine accessibility of file.
** David Eppstein / Stanford University / 9-Aug-84
** TENEX additions by Ken Harrenstien, SRI 1-Jun-85
*/

#include "c-env.h"

entry access;

#define CHKAC	0521			/* Check user access to file */
#define	RPCAP	0150			/* Read process caps */
#define	GJINF	0013			/* Get job information */

#define	FHSLF	0400000			/* process handle for self */

#define	GJOLD	0100000000000		/* Looking for existing file */

#define	CKARD	0			/* Check read access */
#define	CKAWR	1			/* Check write access */
#define	CKAEX	2			/* Check execute access */

#define	CKJFN	0400000000000		/* JFN supplied in .CKAUD */

#define	CKAAC	0			/* Code of desired access to files */
#define	CKALD	1			/* User number */
#define	CKACD	2			/* Conn dir number */
#define	CKAEC	3			/* Enabled caps */
#define	CKAUD	4			/* JFN of file being accessed */
#define	CKAPR	5			/* File protection (not used) */

int access(filnam, mode)
char *filnam;
int  mode;
{
#if SYS_T20+SYS_10X
    int jfn;

    jfn = _gtjfn(filnam, GJOLD);	/* get JFN on the file */
    if (jfn < 0) return -1;		/* no such file, give up */
#if SYS_10X
	_rljfn(jfn);
	return 0;		/* If TENEX always claim win for now,
				 * because we don't have CHKAC% */
#endif
#if SYS_T20
    if ((mode & 4) && !_chkac(CKARD, jfn)) return -1; /* can't read */
    if ((mode & 2) && !_chkac(CKAWR, jfn)) return -1; /* can't write */
    if ((mode & 1) && !_chkac(CKAEX, jfn)) return -1; /* can't execute */
    _rljfn(jfn);			/* release the JFN */
    return 0;				/* return success */
#endif /*SYS_T20*/
#else
	return(-1);
#endif /* Not T20 or 10X */
}

#if SYS_T20
static _chkac(acc, jfn)
{
    int ablock[5], cblock[6];

    cblock[CKAAC] = acc;		/* get desired access code */
    cblock[CKAUD] = jfn;		/* and jfn */

    jsys(GJINF, ablock);		/* get job information */
    cblock[CKALD] = ablock[1];		/* user number */
    cblock[CKACD] = ablock[2];		/* and connected directory */

    ablock[1] =  FHSLF;			/* reading our own process caps */
    jsys(RPCAP, ablock);		/* do it */
    cblock[CKAEC] = ablock[3];		/* enabled capabilities */

    ablock[1] = 6 + CKJFN;		/* AC1: length, flags */
    ablock[2] = (int) cblock;		/* AC2: argument block */
    jsys(CHKAC, ablock);		/* check file access */
    if (ablock[1] == -1) return 1;	/* success, return truth */
    _rljfn(jfn);			/* failure, release jfn */
    return 0;				/* and return falsehood */
}
#endif /*SYS_T20*/
