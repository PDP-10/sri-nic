/*
 *	Do the actual making for make
 */

#include "h.h"

#ifdef EON
#include <sys/stat.h>
#include <sys/err.h>
#endif

#ifdef KCC_20
#include <sys/stat.h>
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
 *	Exec a shell that returns exit status correctly (/bin/esh).
 *	The standard EON shell returns the process number of the last
 *	async command, used by the debugger (ugg).
 *	[exec on eon is like a fork+exec on unix]
 */
int
dosh(string, shell)
char *			string;
char *			shell;
{

#ifdef EON
	int	number;

	return ((number = execl(shell, shell,"-c", string, 0)) == -1) ?
		-1:	/* couldn't start the shell */
		wait(number);	/* return its exit status */
#endif

#ifdef KCC_20
	return (system(string));
#endif

#ifdef MSC
	return (system(string));
#endif

#ifdef OS9
	int	number, pid, status;

	strcat(string, "\n");
	if ((number = os9fork(shell, strlen(string), string, 0, 0, 0)) == -1)
		return -1;		/* Couldn't start a shell */
	do
	{
		if ((pid = wait(&status)) == -1)
			return -1;	/* child already died!?!? */
	} while (pid != number);

	return status;
#endif

#ifdef UNIX
	return (system(string));
#endif

#ifdef VMS
	return (system(string));
#endif

}


/*
 *	Do commands to make a target
 */
void
docmd2(np, lp)
NAMEP		np;
LINEP		lp;
{
	bool			ssilent;
	bool			signore;
	int			estat;
	register char *		q;
	register char *		p;
	char *			shell;
	register CMDP	cp;


	if (*(shell = getmacro("SHELL")) == '\0')
#ifdef EON
		shell = ":bin/esh";
#endif

#ifdef KCC_20
		shell = "SYSTEM:EXEC.EXE";
#endif

#ifdef MSC
		shell = getenv("COMSPEC");
		if (shell == (char*)NULL)
			shell = "command.com";
#endif

#ifdef OS9
		shell = "shell";
#endif

#ifdef UNIX
		shell = "/bin/sh";
#endif

#ifdef VMS
		shell = "SYS$SYSTEM:DCL.EXE";
#endif

	for (cp = lp->l_cmd; cp; cp = cp->c_next)
	{
		strcpy(str1, cp->c_cmd);
		expand(str1);
		q = str1;
		ssilent = silent;
		signore = ignore;
		while ((*q == '@') || (*q == '-'))
		{
			if (*q == '@')	   /*  Specific silent  */
				ssilent = TRUE;
			else		   /*  Specific ignore  */
				signore = TRUE;
			q++;		   /*  Not part of the command  */
		}

		if (!domake)
			ssilent = 0;

		if (!ssilent)
			fputs("    ", stdout);

		for (p=q; *p; p++)
		{
			if (*p == '\n' && p[1] != '\0')
			{
				*p = ' ';
				if (!ssilent)
					fputs(MORE, stdout);
			}
			else if (!ssilent)
				putchar((char)*p);
		}
		if (!ssilent)
			putchar('\n');

		if (domake)
		{			/*  Get the shell to execute it  */
			if ((estat = dosh(q, shell)) != 0)
			{
				if (estat == -1)
					fatal("Couldn't execute %s", shell);
				else
				{
					printf("%s: Error code %d", myname, estat);
					if (signore)
						fputs(" (Ignored)\n", stdout);
					else
					{
						putchar('\n');
						if (!(np->n_flag & N_PREC))
							if (unlink(np->n_name) == 0)
								printf("%s: `%s' removed.\n", myname, np->n_name);
						EXIT(estat);
					}
				}
			}
		}
	}
}


void
docmds(np)
NAMEP		np;
{
	register LINEP	lp;

	for (lp = np->n_line; lp; lp = lp->l_next)
		docmd2(np, lp);
}


#ifdef OS9
/*
 *	Some stuffing around to get the modified time of a file
 *	in an os9 file system
 */
getmdate(fd, tbp)
struct sgtbuf *		tbp;
{
	struct registers	regs;
	static struct fildes	fdbuf;


	regs.rg_a = fd;
	regs.rg_b = SS_FD;
	regs.rg_x = &fdbuf;
	regs.rg_y = sizeof (fdbuf);

	if (_os9(I_GETSTT, &regs) == -1)
	{
		errno = regs.rg_b & 0xff;
		return -1;
	}
	if (tbp)
	{
		_strass(tbp, fdbuf.fd_date, sizeof (fdbuf.fd_date));
		tbp->t_second = 0;	/* Files are only accurate to mins */
	}
	return 0;
}


/*
 *	Kludge routine to return an aproximation of how many
 *	seconds since 1980.  Dates will be in order, but will not
 *	be linear
 */
time_t
cnvtime(tbp)
struct sgtbuf		*tbp;
{
	long			acc;


	acc = tbp->t_year - 80;		/* Baseyear is 1980 */
	acc = acc * 12 + tbp->t_month;
	acc = acc * 31 + tbp->t_day;
	acc = acc * 24 + tbp->t_hour;
	acc = acc * 60 + tbp->t_minute;
	acc = acc * 60 + tbp->t_second;

	return acc;
}


/*
 *	Get the current time in the internal format
 */
time(tp)
time_t *		tp;
{
	struct sgtbuf		tbuf;


	if (getime(&tbuf) < 0)
		return -1;

	if (tp)
		*tp = cnvtime(&tbuf);

	return 0;
}
#endif


/*
 *	Get the modification time of a file.  If the first
 *	doesn't exist, it's modtime is set to 0.
 */
void
modtime(np)
NAMEP		np;
{
#ifdef EON
	struct stat		info;
	int			fd;

	errno = 0;	/* clear error code in case of left-over value */
	if ((fd = open(np->n_name, 0)) < 0)
	{
		if (errno != ER_NOTF)
			fatal("Can't open %s; error %02x", np->n_name, errno);

		np->n_time = (time_t)0;
	}
	else if (getstat(fd, &info) < 0)
		fatal("Can't getstat %s; error %02x", np->n_name, errno);
	else
		np->n_time = info.st_mod;

	close(fd);
#endif

#ifdef KCC_20
	struct stat		info;

	errno = 0;	/* clear error code in case of left-over value */
	if (stat(np->n_name, &info) < 0)
	{
		if (errno == 0)
			errno = ENOENT;	/* KCC doesn't set errno if file does not exist */
		if (errno != ENOENT)
			fatal("Can't open %s; error %d", np->n_name, errno);

		np->n_time = (time_t)0;
	}
	else
		np->n_time = info.st_mtime;
#endif

#ifdef MSC
	struct stat		info;

	errno = 0;	/* clear error code in case of left-over value */
	if (stat(np->n_name, &info) < 0)
	{
		if (errno != ENOENT)
			fatal("Can't open %s; error %d", np->n_name, errno);

		np->n_time = (time_t)0;
	}
	else
		np->n_time = info.st_mtime;
#endif

#ifdef OS9
	struct sgtbuf		info;
	int			fd;

	errno = 0;	/* clear error code in case of left-over value */
	if ((fd = open(np->n_name, 0)) < 0)
	{
		if (errno != E_PNNF)
			fatal("Can't open %s; error %02x", np->n_name, errno);

		np->n_time = (time_t)0;
	}
	else if (getmdate(fd, &info) < 0)
		fatal("Can't getstat %s; error %02x", np->n_name, errno);
	else
		np->n_time = cnvtime(&info);

	close(fd);
#endif

#ifdef UNIX
	struct stat		info;

	errno = 0;	/* clear error code in case of left-over value */
	if (stat(np->n_name, &info) < 0)
	{
		if (errno != ENOENT)
			fatal("Can't open %s; error %d", np->n_name, errno);

		np->n_time = (time_t)0;
	}
	else
		np->n_time = info.st_mtime;
#endif

#ifdef VMS
	struct stat		info;

	errno = 0;	/* clear error code in case of left-over value */
	if (stat(np->n_name, &info) < 0)
	{	/* stat() in VMS C 2.0 (VMS 4.4) does not set errno, sigh... */
		if (errno && (errno != ENOENT))
			fatal("Can't open %s; error %d", np->n_name, errno);

		np->n_time = (time_t)0;
	}
	else
		np->n_time = info.st_mtime;
#endif

        if (debug)
	{
	    fprintf(stderr,"TIME(%s) = %ld\n",np->n_name,(long)np->n_time);
	}
}


/*
 *	Update the mod time of a file to now.
 */
void
touch(np)
NAMEP		np;
{
	if (!domake || !silent)
		printf("    touch(%s)\n", np->n_name);

	if (domake)
	{

#ifdef EON
		char			c;
		int			fd;

		if ((fd = open(np->n_name, 0)) < 0)
			printf("%s: `%s' not touched - non-existant\n",
					myname, np->n_name);
		else
		{
			uread(fd, &c, 1, 0);
			uwrite(fd, &c, 1);
		}
		close(fd);
#endif

#ifdef KCC_20
		static int first = 1;

		if (first)
		    printf("touch not implemented for KCC-20 (no utime())\n");
		first = 0;
#endif

#ifdef MSC
		struct utimbuf	a[2];

		a[0].modtime = a[1].modtime = time((time_t*)NULL);
		if (utime(np->n_name, &a[0]) < 0)
			printf("%s: `%s' not touched - non-existant\n",
					myname, np->n_name);
#endif

#ifdef OS9
		int			fd;

		/*
		 *	Strange that something almost as totally useless
		 *	as this is easy to do in os9!
		 */
		if ((fd = open(np->n_name, S_IWRITE)) < 0)
			printf("%s: `%s' not touched - non-existant\n",
					myname, np->n_name);
		close(fd);
#endif

#ifdef UNIX
		long		a[2];

		a[0] = a[1] = time((time_t*)NULL);
		if (utime(np->n_name, &a[0]) < 0)
			printf("%s: `%s' not touched - non-existant\n",
					myname, np->n_name);
#endif

#ifdef VMS
		static int first = 1;

		if (first)
		    printf("touch not implemented for VMS (no utime())\n");
		first = 0;
#endif

	}
}


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

void
make1(np, lp, qdp)
register DEPENDP	qdp;
LINEP			lp;
NAMEP			np;
{
	register DEPENDP	dp;


        if (debug)
	{
	    fprintf(stderr,"MAKE1(%s):\tTIME(%ld)\tCMD(%s%s)\n",np->n_name,
	        (long)np->n_time,
		((lp != NULL_LINEP) && (lp->l_cmd != NULL_CMDP) &&
		(lp->l_cmd->c_cmd != NULL_CHARP)) ? lp->l_cmd->c_cmd : "",
		((lp != NULL_LINEP) && (lp->l_cmd != NULL_CMDP) &&
		(lp->l_cmd->c_next != NULL_CMDP)) ? " etc." : "");
	}

	if (dotouch)
		touch(np);
	else
	{
		strcpy(str1, "");
		for (dp = qdp; dp; dp = qdp)
		{
			if (strlen(str1))
				strcat(str1, " ");
			strcat(str1, dp->d_name->n_name);
			qdp = dp->d_next;
			free((char *)dp);
		}
		(void)setmacro("?", str1);
		(void)setmacro("@", np->n_name);
		if (lp)		/* lp set if doing a :: rule */
			docmd2(np, lp);
		else
			docmds(np);
	}
}
