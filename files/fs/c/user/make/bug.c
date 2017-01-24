/* BUG.C -- KCC internal register allocation error demonstration */
/*
 *	Do the actual making for make
 */

#include "h.h"

#ifdef EON
#include <sys/stat.h>
#include <sys/err.h>
#endif

#ifdef KCC_20
#include <stat.h>
#include <errno.h>
#endif

#ifdef MSC
#include <sys/stat.h>
#include <errno.h>
#endif

#ifdef OS9
#include <time.h>
#include <os9.h>
#include <modes.h>
#include <direct.h>
#include <errno.h>
#endif

#ifdef UNIX
#include <sys/stat.h>
#include <sys/errno.h>
#endif

#ifdef VMS
#include <stat.h>
#include <errno.h>
#endif
/*
 *	Recursive routine to make a target.
 */
int
make(np, level)
NAMEP		np;
int			level;
{
	register DEPENDP	dp;
	register LINEP		lp;
	register DEPENDP	qdp;
	time_t				dtime = 1;
	bool				didsomething = 0;


	if (np->n_flag & N_DONE)
		return 0;

	if (!np->n_time)
		modtime(np);		/*  Gets modtime of this file  */

        if (debug)
	{
	    fprintf(stderr,"MAKE(%s,%d):\tTIME(%ld)\n",np->n_name,(int)level,
	        (long)np->n_time);
	}

	if (rules)
	{
		for (lp = np->n_line; lp; lp = lp->l_next)
			if (lp->l_cmd)
				break;
		if (!lp)
			(void)dyndep(np);
	}

	if (!(np->n_flag & N_TARG) && np->n_time == (time_t)0)
		fatal("Don't know how to make %s", np->n_name);

	for (qdp = NULL_DEPENDP, lp = np->n_line; lp; lp = lp->l_next)
	{
		for (dp = lp->l_dep; dp; dp = dp->d_next)
		{
			(void)make(dp->d_name, level+1);
			if (np->n_time < dp->d_name->n_time)
				qdp = newdep(dp->d_name, qdp);
			dtime = max(dtime, dp->d_name->n_time);
		}
		if (!quest && (np->n_flag & N_DOUBLE) && (np->n_time < dtime))
		{
			make1(np, lp, qdp);	/* free()'s qdp */
			dtime = 1;
			qdp = NULL_DEPENDP;
			didsomething++;
		}
	}

	np->n_flag |= N_DONE;

	if (quest)
	{
		long		t;

		t = np->n_time;
		time(&np->n_time);
		return t < dtime;
	}
	else if (np->n_time < dtime && !(np->n_flag & N_DOUBLE))
	{
	        if (debug)
		{
			fprintf(stderr,"\tTIME(%ld) < %ld\n",
				(long)np->n_time,(long)dtime);
		}
		make1(np, NULL_LINEP, qdp);	/* free()'s qdp */

/* WRONG!! Must not change dependent time to current time.   Otherwise target
   will always be found older than its dependents.
		time(&np->n_time);
   Get real modification time instead!! */

		modtime(np);
	}
	else if (level == 0 && !didsomething)
		printf("%s: `%s' is up to date\n", myname, np->n_name);
	return 0;
}
