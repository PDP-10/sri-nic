/*
 *	Parse a makefile
 */

#include "h.h"

NAME		namehead;
NAMEP		firstname;

char		str1[LZ];		/*  General store  */
char		str2[LZ];


/*
 *	Intern a name.  Return a pointer to the name struct
 */
NAMEP
newname(name)
char *			name;
{
	register NAMEP	rp;
	register NAMEP	rrp;
	register char *		cp;


	for
	(
		rp = namehead.n_next, rrp = &namehead;
		rp;
		rp = rp->n_next, rrp = rrp->n_next
	)
		if (strcmp(name, rp->n_name) == 0)
			return rp;

	if ((rp = (NAMEP)malloc(sizeof (NAME))) == NULL_NAMEP)
		fatal("No memory for name");
	rrp->n_next = rp;
	rp->n_next = NULL_NAMEP;
	if ((cp = malloc(strlen(name)+1)) == NULL_CHARP)
		fatal("No memory for name");
	strcpy(cp, name);
	rp->n_name = cp;
	rp->n_line = NULL_LINEP;
	rp->n_time = (time_t)0;
	rp->n_flag = 0;

	return rp;
}


/*
 *	Add a dependant to the end of the supplied list of dependants.
 *	Return the new head pointer for that list.
 */
DEPENDP
newdep(np, dp)
NAMEP		np;
DEPENDP		dp;
{
	register DEPENDP	rp;
	register DEPENDP	rrp;


	if ((rp = (DEPENDP)malloc(sizeof (DEPEND))) == NULL_DEPENDP)
		fatal("No memory for dependant");
	rp->d_next = NULL_DEPENDP;
	rp->d_name = np;

	if (dp == NULL_DEPENDP)
		return rp;

	for (rrp = dp; rrp->d_next; rrp = rrp->d_next)
		;

	rrp->d_next = rp;

	return dp;
}


/*
 *	Add a command to the end of the supplied list of commands.
 *	Return the new head pointer for that list.
 */
CMDP
newcmd(str, cp)
char *		str;
CMDP		cp;
{
	register CMDP	rp;
	register CMDP	rrp;
	register char *		rcp;


	if (rcp = rindex(str, '\n'))
		*rcp = '\0';		/*  Loose newline  */

	while (isspace(*str))
		str++;

	if (*str == '\0')		/*  If nothing left, the exit  */
		return ((CMDP)NULL);

	if ((rp = (CMDP)malloc(sizeof (CMD))) == NULL_CMDP)
		fatal("No memory for command");
	rp->c_next = NULL_CMDP;
	if ((rcp = malloc(strlen(str)+1)) == NULL_CHARP)
		fatal("No memory for command");
	strcpy(rcp, str);
	rp->c_cmd = rcp;

	if (cp == NULL_CMDP)
		return rp;

	for (rrp = cp; rrp->c_next; rrp = rrp->c_next)
		;

	rrp->c_next = rp;

	return cp;
}


/*
 *	Add a new 'line' of stuff to a target.  This check to see
 *	if commands already exist for the target.  If flag is set,
 *	the line is a double colon target.
 *
 *	Kludges:
 *	i)  If the new name begins with a '.', and there are no dependents,
 *	    then the target must cease to be a target.  This is for .SUFFIXES.
 *	ii) If the new name begins with a '.', with no dependents and has
 *	    commands, then replace the current commands.  This is for
 *	    redefining commands for a default rule.
 *	Neither of these free the space used by dependents or commands,
 *	since they could be used by another target.
 */
void
newline(np, dp, cp, flag)
NAMEP		np;
DEPENDP		dp;
CMDP		cp;
bool		flag;
{
	bool			hascmds = FALSE;  /*  Target has commands  */
	register LINEP	rp;
	register LINEP	rrp;


	/* Handle the .SUFFIXES case */
	if (np->n_name[0] == '.' && !dp && !cp)
	{
		for (rp = np->n_line; rp; rp = rrp)
		{
			rrp = rp->l_next;
			free((char *)rp);
		}
		np->n_line = NULL_LINEP;
		np->n_flag &= ~N_TARG;
		return;
	}

	/* This loop must happen since rrp is used later. */
	for
	(
		rp = np->n_line, rrp = NULL_LINEP;
		rp;
		rrp = rp, rp = rp->l_next
	)
		if (rp->l_cmd)
			hascmds = TRUE;

	if (hascmds && cp && !(np->n_flag & N_DOUBLE))
		/* Handle the implicit rules redefinition case */
		if (np->n_name[0] == '.' && dp == NULL_DEPENDP)
		{
			np->n_line->l_cmd = cp;
			return;
		}
		else
			error("Commands defined twice for target %s", np->n_name);
	if (np->n_flag & N_TARG)
		if (!(np->n_flag & N_DOUBLE) != !flag)		/* like xor */
			error("Inconsistent rules for target %s", np->n_name);

	if ((rp = (LINEP)malloc(sizeof (LINE))) == NULL_LINEP)
		fatal("No memory for line");
	rp->l_next = NULL_LINEP;
	rp->l_dep = dp;
	rp->l_cmd = cp;

	if (rrp)
		rrp->l_next = rp;
	else
		np->n_line = rp;

	np->n_flag |= N_TARG;
	if (flag)
		np->n_flag |= N_DOUBLE;
}


/*
 *	Parse input from the makefile, and construct a tree structure
 *	of it.
 */
void
input(fd)
FILE *			fd;
{
	char *			p;		/*  General  */
	char *			q;
	NAMEP		np;
	DEPENDP		dp;
	CMDP		cp;
	bool		dbl;


	if (getline(str1, fd))	/*  Read the first line  */
		return;

	for(;;)
	{
#ifdef OS9
		if (*str1 == ' ')	/*  Rules without targets  */
#else
		if (*str1 == '\t')	/*  Rules without targets  */
#endif
			error("Rules not allowed here");

		p = str1;

		while (isspace(*p))	/*  Find first target  */
			p++;

		while (((q = index(p, '=')) != NULL_CHARP) &&
		    (p != q) && (q[-1] == '\\'))	/*  Find value */
		{
			register char *		a;

			a = q - 1;	/*  Del \ chr; move rest back  */
			p = q;
			while(*a++ = *q++)
				;
		}

		if (q != NULL_CHARP)
		{
			register char *		a;

			*q++ = '\0';		/*  Separate name and val  */
			while (isspace(*q))
				q++;
			if (p = rindex(q, '\n'))
				*p = '\0';

			p = str1;
			if ((a = gettok(&p)) == NULL_CHARP)
				error("No macro name");

			(void)setmacro(a, q);

			if (getline(str1, fd))
				return;
			continue;
		}

		expand(str1);
		p = str1;

		while (((q = index(p, ':')) != NULL_CHARP) &&
		    (p != q) && (q[-1] == '\\'))	/*  Find dependents  */
		{
			register char *		a;

			a = q - 1;	/*  Del \ chr; move rest back  */
			p = q;
			while(*a++ = *q++)
				;
		}

		if (q == NULL_CHARP)
			error("No targets provided");

		*q++ = '\0';	/*  Separate targets and dependents  */

		if (*q == ':')		/* Double colon */
		{
			dbl = TRUE;
			q++;
		}
		else
			dbl = FALSE;

		for (dp = NULL_DEPENDP; ((p = gettok(&q)) != NULL_CHARP);)
					/*  get list of dep's */
		{
			np = newname(p);		/*  Intern name  */
			dp = newdep(np, dp);		/*  Add to dep list */
		}

		*((q = str1) + strlen(str1) + 1) = '\0';
			/*  Need two nulls for gettok (Remember separation)  */

		cp = NULL_CMDP;
		if (getline(str2, fd) == FALSE)		/*  Get commands  */
		{
#ifdef OS9
			while (*str2 == ' ')
#else
			while (*str2 == '\t')
#endif
			{
				cp = newcmd(&str2[0], cp);
				if (getline(str2, fd))
					break;
			}
		}

		while ((p = gettok(&q)) != NULL_CHARP)	/* Get list of targ's */
		{
			np = newname(p);		/*  Intern name  */
			newline(np, dp, cp, dbl);
			if (!firstname && p[0] != '.')
				firstname = np;
		}

		if (feof(fd))				/*  EOF?  */
			return;

		strcpy(str1, str2);
	}
}
