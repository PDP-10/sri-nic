 5-May-84 14:23:30-EDT,4356;000000000001
Received: from MIT-MC by MIT-OZ via Chaosnet; 5 May 84 14:23-EDT
Received: From bostonu.csnet by csnet-relay;  5 May 84 14:12 EDT
Received: by csvaxa.ARPA (4.12/4.7)
	id AA23837; Sat, 5 May 84 13:55:57 edt
Date: Sat, 5 May 84 13:55:57 edt
From: God <root%bostonu.csnet@csnet-relay.arpa>
Message-Id: <8405051755.AA23837@csvaxa.ARPA>
To: rms@mit-mc.arpa
Subject: brig.c

#include "brig.h"
#ifdef DEBUG
#include <stdio.h>
#else
#define NULL	0
/*LINTLIBRARY*/
#endif
#ifndef REGISTER
#define REGISTER register
#endif
#ifndef REGARG
#define REGARG register
#endif
char *malloc() ;
/*
 *	GNU LIB
 *
 *	A brigade is a chain of buckets, a bucket
 *	is a structure with a pointer to next and
 *	an array of data.
 *
 *	They have the property that referring to an element
 *	forces it to be allocated, otherwise they are referred
 *	to quite like arrays.
 *
 *	USER ROUTINES:
 *		bopen(sz,n) n:		number of items per bucket
 *			    sz:		sizeof item
 *			    returns:	ptr for later usage
 *		bclose(hp)  hp:		ptr earlier gotten from bopen()
 *			    returns:	0 always (assume non-0 error tho)
 *		getbp(hp,n) hp:		ptr earlier gotten from bopen()
 *			    n:		index (like an array index)
 *			    returns:	ptr to storage for item
 *
 *	INTERNAL ROUTINES:
 *		newb(hp)    hp:		head ptr
 *			    returns:	ptr to new bucket
 *
 *	NOTES:
 *		Typing of the return value from getbp() presents a
 *		similar problem to malloc(). Macros help a lot.
 *	BUGS:
 *		Shouldn't be any, very straightforward algorithm.
 *		getbp() certainly tempts the user to not check for
 *		null returns, malloc() should probably interrupt on
 *		memory failure rather than returning NULL.
 *	AUTHOR:
 *		Barry Shein, Boston University (bzs%bostonu@csnet-relay)
 *		5/4/84
 */

/*
 *	allocate a new bucket (internal only)
 */
static buck_t *
newb(hp) REGARG bhead_t *hp ;
{
	REGISTER buck_t *bp ;

	/*NOSTRICT*/
	if((bp = (buck_t *) malloc(sizeof *bp)) == NULL)
			;
	/*NOSTRICT*/
	else if((bp->bdata = (char *) malloc(hp->bperb * hp->nperb)) == NULL)
		/*NOSTRICT*/
		free(bp) ;
	else return(bp) ;
	return(NULL) ;
}
/*
 *	Start a new brigade
 */
bhead_t *
bopen(sz,n) int sz, n ;
{
	REGISTER bhead_t *res ;

	/*NOSTRICT*/
	if((res = (bhead_t *) malloc(sizeof *res)) == NULL)
		return(NULL) ;
	else
	{
		res->bperb = sz ;
		res->nperb = n ;
	}
	if((res->bhead = newb(res)) == NULL)
		/*NOSTRICT*/
		free(res) ;
	else
	{
		res->bhead->bnext = NULL ;
		res->bhead->bfirst = 0 ;
		return(res) ;
	}
	return(NULL) ;
}
/*
 *	free all memory associated with a brigade
 */
bclose(hdrp) bhead_t *hdrp ;
{
	REGISTER buck_t *hp, *lp ;

	lp = hdrp->bhead ;
	hp = lp->bnext ;
	for(;;)
	{
		/*NOSTRICT*/
		free(lp->bdata) ;
		/*NOSTRICT*/
		free(lp) ;
		if((lp = hp) != NULL) hp = lp->bnext ;
		else break ;
	}
	return(0) ;
}
/*
 *	return a pointer to the desired item
 *	the type of the return should be coerced.
 *	This could cause portability problems to
 *	those machines that have different sized ptrs.
 */
char *
getbp(hp,n) REGARG bhead_t *hp ; REGARG int n ;
{
	REGISTER buck_t *lp ;

	lp = hp->bhead ;
	for(;; lp = lp->bnext)
	{
		/*
		 *	See if it is in current bucket. If so done.
		 */
		if((lp->bfirst <= n) && ((lp->bfirst + hp->nperb) > n))
			return(lp->bdata + (hp->bperb * (n - lp->bfirst))) ;
		/*
		 *	If no next or a hole then create appropiate
		 *	bucket
		 */
		if((lp->bnext == NULL) || (lp->bnext->bfirst > n))
		{
			buck_t *nb ;

			if((nb = newb(hp)) == NULL)
				return(NULL) ;
			nb->bnext = lp->bnext ;
			lp->bnext = nb ;
			nb->bfirst = hp->nperb * (n / hp->nperb) ;
			return(nb->bdata + (hp->bperb * (n - nb->bfirst))) ;
		}
	}
}
#ifdef DEBUG
/*
 *	main I used to test the code, just #define DEBUG at top and recompile
 */
#define IP(n) (*((int *) getbp(hp,n)))
main(argc,argv) int argc ; char **argv ;
{
	bhead_t *hp ;
	int i ;

	if((hp = bopen(sizeof i,10)) == NULL)
	{
		fprintf(stderr,"bopen => NULL?\n") ;
		exit(1) ;
	}
	for(i = 0 ; i < 99 ; i++)
		IP(i) = i ;
	IP(500) = 500 ;
	IP(350) = 350 ;
	IP(250) = 250 ;
	IP(351) = 351 ;
	for(i = 0 ; i < 99 ; i++)
		printf("%d\n",IP(i)) ;
	printf("%d %d %d %d\n",IP(500),IP(350),IP(250),IP(351)) ;
	printf("%d\n",bclose(hp)) ;
}
#endif
