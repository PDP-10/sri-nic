/*
** int getpid()
**
** Greg Satz / Stanford University / 15-Sep-84
** TENEX additions by Ken Harrenstien, SRI 1-Jun-85
*/

#include "c-env.h"

entry getpid;

#if SYS_T20+SYS_10X
#define	GFRKS	0166
#define	GJINF	013

#define	FHTOP	0777776
#define	FHSLF	0400000

#define	MAP_SIZE 100

struct fork_map {
	unsigned *parap : 18;
	unsigned *infp : 18;
	unsigned *supp : 18;
	unsigned handle : 18;
	unsigned status;
	};
#endif /* T20+10X */

int getpid()
{
#if SYS_T20+SYS_10X
	int cnt, ablock[5];
	struct fork_map *sup, *fmp, fma[MAP_SIZE], *count();

	fmp = fma;
	ablock[1] = FHTOP;
#if SYS_10X
	ablock[2] = (int)fmp;		/* TENEX */
#endif
#if SYS_T20
	ablock[2] = 0;			/* TOPS-20 */
	ablock[3] = (-MAP_SIZE << 18) | (unsigned) fmp;
#endif

	if (jsys(GFRKS, ablock) <= 0)
		return -1;	/* failure for now */
	if (jsys(GJINF, ablock) == 0)	/* get job info */
		return -1;
	while (fmp->supp != 0)	/* find superior */
		fmp = (struct fork_map *) fmp->supp;
	sup = fmp;		/* save ptr to superior */
	cnt = 0;		/* initialize count */
	if (count(fmp, &cnt) == sup)	/* didn't find FHSLF */
		return -1;
	else
		return (ablock[3] << 8) | cnt;	/* return node count & job */
#else /* T20+10X */
	return(0);	/* Not implemented elsewhere yet */
#endif /* Not T20 or 10X */
}

#if SYS_T20+SYS_10X
/*
 * Count fork map in preorder
 */
static struct fork_map *count(ptr, cnt)
struct fork_map *ptr;
int *cnt;
{
	struct fork_map *sptr = 0;

	(*cnt)++;		/* count fork */
	if (ptr->handle == FHSLF)
		return ptr;
	else {
		if (ptr->infp != 0)	/* does inferior exist (left) */
			sptr = count(ptr->infp, cnt);
		if (sptr == 0 && ptr->parap != 0) /* check for parallel */
			sptr = count(ptr->parap, cnt);
		return sptr;
		
	}
}
#endif /* T20+10X */
