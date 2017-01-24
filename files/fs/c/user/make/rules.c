/*
 *	Control of the implicit suffix rules
 */

#include "h.h"


/*
 *	Return a pointer to the suffix of a name
 */
char *
suffix(name)
char *			name;
{
	return rindex(name, '.');
}


/*
 *	Dynamic dependency.  This routine applies the suffix rules
 *	to try and find a source and a set of rules for a missing
 *	target.  If found, np is made into a target with the implicit
 *	source name, and rules.  Returns TRUE if np was made into
 *	a target.
 */
bool
dyndep(np)
NAMEP		np;
{
	register char *		p;
	register char *		q;
	register char *		suff;		/*  Old suffix  */
	register char *		basename;	/*  Name without suffix  */
	NAMEP		op;		/*  New dependent  */
	NAMEP		sp;		/*  Suffix  */
	LINEP		lp;
	DEPENDP		dp;
	char *			newsuff;


	p = str1;
	q = np->n_name;
	if (debug)
	{
	    fprintf(stderr,"\tDEPENDENCY(%s)\n",q);
	}
	if (!(suff = suffix(q)))
		return FALSE;		/* No suffix */
	while (q < suff)
		*p++ = *q++;
	*p = '\0';
	basename = setmacro("*", str1)->m_val;
	if (debug)
	{
	    fprintf(stderr,"\t\tBASENAME(%s)\n",basename);
	}

	if (!((sp = newname(".SUFFIXES"))->n_flag & N_TARG))
		return FALSE;

	for (lp = sp->n_line; lp; lp = lp->l_next)
		for (dp = lp->l_dep; dp; dp = dp->d_next)
		{
			newsuff = dp->d_name->n_name;
			if (strlen(suff)+strlen(newsuff)+1 >= LZ)
				fatal("Suffix rule too long");
			p = str1;
			q = newsuff;
			while (*p++ = *q++)
				;
			p--;
			q = suff;
			while (*p++ = *q++)
				;
			sp = newname(str1);
			if (sp->n_flag & N_TARG)
			{
				p = str1;
				q = basename;
				if (strlen(basename) + strlen(newsuff)+1 >= LZ)
					fatal("Implicit name too long");
				while (*p++ = *q++)
					;
				p--;
				q = newsuff;
				while (*p++ = *q++)
					;
				op = newname(str1);
				if (!op->n_time)
					modtime(op);
				if (op->n_time)
				{
					dp = newdep(op, NULL_DEPENDP);
					newline(np, dp, sp->n_line->l_cmd, FALSE);
					(void)setmacro("<", op->n_name);
					return TRUE;
				}
			}
		}
	return FALSE;
}


/*
 *	Make the default rules
 */
void
makerules()
{
	CMDP		cp;
	NAMEP		np;


#ifdef EON
	(void)setmacro("BDSCC", "asm");
	/*	(void)setmacro("BDSCFLAGS", "");	*/
	cp = newcmd("$(BDSCC) $(BDSCFLAGS) -n $<", NULL_CMDP);
	np = newname(".c.o");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("CC", "c");
	(void)setmacro("CFLAGS", "-O");
	cp = newcmd("$(CC) $(CFLAGS) -c $<", NULL_CMDP);
	np = newname(".c.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("M80", "asm -n");
	/*	(void)setmacro("M80FLAGS", "");	*/
	cp = newcmd("$(M80) $(M80FLAGS) $<", NULL_CMDP);
	np = newname(".mac.o");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("AS", "zas");
	/*	(void)setmacro("ASFLAGS", "");	*/
	cp = newcmd("$(ZAS) $(ASFLAGS) -o $@ $<", NULL_CMDP);
	np = newname(".as.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	newsuffix(".SUFFIXES: .as .obj .c .o .mac");
#endif

#ifdef KCC_20
/*
 *	Some of the TOPS-20 implicit rules
 */
	(void)setmacro("MV","rename");	/* works only on single disk structure */
	(void)setmacro("RM","delete");

	(void)setmacro("TeXFLAGS", "\\batchmode");
	(void)setmacro("TeX", "TeX");
	cp = newcmd("$(TeX) $(TeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".tex.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("LaTeXFLAGS", "\\batchmode");
	(void)setmacro("LaTeX", "LaTeX");
	cp = newcmd("$(LaTeX) $(LaTeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".ltx.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".latex.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("AMSTeXFLAGS", "\\batchmode");
	(void)setmacro("AMSTeX", "AMSTeX");
	cp = newcmd("$(AMSTeX) $(AMSTeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".atx.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".amstex.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("CC", "kcc");
	(void)setmacro("CFLAGS", "");
	cp = newcmd("$(CC) $(CFLAGS) -c $<", NULL_CMDP);
	np = newname(".c.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("AS", "compile");
	cp = newcmd("$(AS) $<", NULL_CMDP);
	np = newname(".mac.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	cp = newcmd("$(AS) $<", NULL_CMDP);
	np = newname(".fai.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	cp = newcmd("$(AS) $<", NULL_CMDP);
	np = newname(".mid.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("PFLAGS", "");
	(void)setmacro("PC", "compile");
	cp = newcmd("$(PC) /pascal/language:\"$(PFLAGS)\" $<", NULL_CMDP);
	np = newname(".pas.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".p.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("FFLAGS", "/debug:(arg,lab)");
	(void)setmacro("FC", "compile");
	cp = newcmd("$(FC) /fortran/language:\"$(FFLAGS)\" $<", NULL_CMDP);
	np = newname(".for.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".f.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("SF3", "sf3");
	cp = newcmd("$(SF3) $<",NULL_CMDP);
	np = newname(".sf3.for");
	newline(np, NULL_DEPENDP, cp, FALSE);

	cp = newcmd("$(SF3) $<",NULL_CMDP);
	cp = newcmd("$(FC) /fortran/language:\"$(FFLAGS)\" $<", cp);
	np = newname(".sf3.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("YACC", "yacc");
	/*	(void)setmacro("YFLAGS", "");	*/
	cp = newcmd("$(YACC) $(YFLAGS) $<", NULL_CMDP);
	cp = newcmd("$(MV) ytab.c $@", cp);
	np = newname(".y.c");
	newline(np, NULL_DEPENDP, cp, FALSE);

	cp = newcmd("$(YACC) $(YFLAGS) $<", NULL_CMDP);
	cp = newcmd("$(CC) $(CFLAGS) -c ytab.c", cp);
	cp = newcmd("$(RM) ytab.c", cp);
	cp = newcmd("$(MV) ytab.rel $@", cp);
	np = newname(".y.rel");
	newline(np, NULL_DEPENDP, cp, FALSE);

	newsuffix(".SUFFIXES: .rel .dvi .c .y .l .mac .fai .mid .for .f .sf3 \
.pas .p .tex .latex .ltx .amstex .atx .inc .h");
#endif

#ifdef MSC
	(void)setmacro("MV","rename");
	(void)setmacro("RM","delete");

	(void)setmacro("TeXFLAGS", "\\batchmode");
	(void)setmacro("TeX", "TeX");
	cp = newcmd("$(TeX) $(TeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".tex.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("LaTeXFLAGS", "\\batchmode");
	(void)setmacro("LaTeX", "LaTeX");
	cp = newcmd("$(LaTeX) $(LaTeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".ltx.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("AMSTeXFLAGS", "\\batchmode");
	(void)setmacro("AMSTeX", "AMSTeX");
	cp = newcmd("$(AMSTeX) $(AMSTeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".atx.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("CC", "msc");
	(void)setmacro("CFLAGS", "");
	cp = newcmd("$(CC) $* $(CFLAGS);", NULL_CMDP);
	np = newname(".c.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("AS", "masm");
	cp = newcmd("$(AS) $*;", NULL_CMDP);
	np = newname(".asm.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("YACC", "yacc");
	/*	(void)setmacro("YFLAGS", "");	*/
	cp = newcmd("$(YACC) $(YFLAGS) $<", NULL_CMDP);
	cp = newcmd("mv ytab.c $@", cp);
	np = newname(".y.c");
	newline(np, NULL_DEPENDP, cp, FALSE);

	cp = newcmd("$(YACC) $(YFLAGS) $<", NULL_CMDP);
	cp = newcmd("$(CC) $(CFLAGS) -c ytab.c", cp);
	cp = newcmd("rm ytab.c", cp);
	cp = newcmd("mv ytab.obj $@", cp);
	np = newname(".y.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	newsuffix(".SUFFIXES: .obj .dvi .c .y .l .asm .for .f .sf3 \
.pas .p .tex .ltx .atx .inc .h");
#endif

#ifdef OS9
/*
 *	Fairlight use an enhanced version of the C sub-system.
 *	They have a specialised macro pre-processor.
 */
	(void)setmacro("CC", "cc");
	(void)setmacro("CFLAGS", "-z");
	cp = newcmd("$(CC) $(CFLAGS) -r $<", NULL_CMDP);

	np = newname(".c.r");
	newline(np, NULL_DEPENDP, cp, FALSE);
	np = newname(".ca.r");
	newline(np, NULL_DEPENDP, cp, FALSE);
	np = newname(".a.r");
	newline(np, NULL_DEPENDP, cp, FALSE);
	np = newname(".o.r");
	newline(np, NULL_DEPENDP, cp, FALSE);
	np = newname(".mc.r");
	newline(np, NULL_DEPENDP, cp, FALSE);
	np = newname(".mca.r");
	newline(np, NULL_DEPENDP, cp, FALSE);
	np = newname(".ma.r");
	newline(np, NULL_DEPENDP, cp, FALSE);
	np = newname(".mo.r");
	newline(np, NULL_DEPENDP, cp, FALSE);

	newsuffix(".SUFFIXES: .r .mc .mca .c .ca .ma .mo .o .a");
#endif

#ifdef UNIX
/*
 *	Some of the UNIX implicit rules
 */
	(void)setmacro("CC", "cc");
	(void)setmacro("CFLAGS", "-O");
	cp = newcmd("$(CC) $(CFLAGS) -c $<", NULL_CMDP);
	np = newname(".c.o");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("AS", "as");
	cp = newcmd("$(AS) -o $@ $<", NULL_CMDP);
	np = newname(".s.o");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("YACC", "yacc");
	/*	(void)setmacro("YFLAGS", "");	*/
	cp = newcmd("$(YACC) $(YFLAGS) $<", NULL_CMDP);
	cp = newcmd("mv y.tab.c $@", cp);
	np = newname(".y.c");
	newline(np, NULL_DEPENDP, cp, FALSE);

	cp = newcmd("$(YACC) $(YFLAGS) $<", NULL_CMDP);
	cp = newcmd("$(CC) $(CFLAGS) -c y.tab.c", cp);
	cp = newcmd("rm y.tab.c", cp);
	cp = newcmd("mv y.tab.o $@", cp);
	np = newname(".y.o");
	newline(np, NULL_DEPENDP, cp, FALSE);

	newsuffix(".SUFFIXES: .o .c .p .e .r .f .y .l .s");	/* like Unix make */
#endif

#ifdef VMS
/*
 *	Some of the VMS implicit rules
 */
	(void)setmacro("MV","rename");	/* works only on single disk structure */
	(void)setmacro("RM","delete");

	(void)setmacro("TeXFLAGS", "\\batchmode");
	(void)setmacro("TeX", "TeX");
	cp = newcmd("$(TeX) $(TeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".tex.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("LaTeXFLAGS", "\\batchmode");
	(void)setmacro("LaTeX", "LaTeX");
	cp = newcmd("$(LaTeX) $(LaTeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".ltx.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".latex.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("AMSTeXFLAGS", "\\batchmode");
	(void)setmacro("AMSTeX", "AMSTeX");
	cp = newcmd("$(AMSTeX) $(AMSTeXFLAGS) \\input $<", NULL_CMDP);
	np = newname(".atx.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".amstex.dvi");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("CC", "kcc");
	(void)setmacro("CFLAGS", "/debug=(symbols,traceback)/nolist");
	cp = newcmd("$(CC) $(CFLAGS) $<", NULL_CMDP);
	np = newname(".c.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("AS", "macro");
	cp = newcmd("$(AS) $<", NULL_CMDP);
	np = newname(".mar.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("PFLAGS", "/debug=(symbols,traceback)/nolist");
	(void)setmacro("PC", "pascal");
	cp = newcmd("$(PC) /language:\"$(PFLAGS)\" $<", NULL_CMDP);
	np = newname(".pas.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".p.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("FFLAGS", "/debug=(symbols,traceback)/nolist");
	(void)setmacro("FC", "fortran");
	cp = newcmd("$(FC) $(FFLAGS) $<", NULL_CMDP);
	np = newname(".for.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".f.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	np = newname(".ftn.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("SF3", "sf3");
	cp = newcmd("$(SF3) $<",NULL_CMDP);
	np = newname(".sf3.for");
	newline(np, NULL_DEPENDP, cp, FALSE);

	cp = newcmd("$(SF3) $<",NULL_CMDP);
	cp = newcmd("$(FC) $(FFLAGS) $<", NULL_CMDP);
	np = newname(".sf3.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	(void)setmacro("YACC", "yacc");
	/*	(void)setmacro("YFLAGS", "");	*/
	cp = newcmd("$(YACC) $(YFLAGS) $<", NULL_CMDP);
	cp = newcmd("$(MV) ytab.c $@", cp);
	np = newname(".y.c");
	newline(np, NULL_DEPENDP, cp, FALSE);

	cp = newcmd("$(YACC) $(YFLAGS) $<", NULL_CMDP);
	cp = newcmd("$(CC) $(CFLAGS) ytab.c", cp);
	cp = newcmd("$(RM) ytab.c", cp);
	cp = newcmd("$(MV) ytab.obj $@", cp);
	np = newname(".y.obj");
	newline(np, NULL_DEPENDP, cp, FALSE);

	newsuffix(".SUFFIXES: .obj .dvi .c .y .l .mar .for .f .ftn .sf3 \
.pas .p .tex .latex .ltx .amstex .atx .inc .h");
#endif

}

/*
 * Convert a string of the form ".SUFFIXES: .sfx .sfx ... .sfx" to a
 * dependency list, easing coding above.  Suffixes are separated by any
 * amount of white space.
 */

void
newsuffix(s)
char * s;			/* ".SUFFIXES: ..." */
{
	int		len;	/* string length */
	register char *	t;	/* copy of ".SUFFIXES: ..." */
	register char *	w;	/* temporary pointer into t[] */
	register char * p;	/* temporary pointer into t[] */
	NAMEP		np;
	DEPENDP		dp;

	len = strlen(s);
	if (len <= 0)
	    return;

	if (strncmp(s,".SUFFIXES:",10) != 0)
	    fatal("Internal error: invalid .SUFFIXES string `%s'",s);

	/* Get copy of s[] in t[]; we fill form name tokens in t[] */
	if ((t = malloc(len+1)) == NULL_CHARP)
		fatal("No memory for temporary string");
	strcpy(t,s);

	dp = NULL_DEPENDP;

	w = index(t,':');	/* point past ".SUFFIXES" */
	*w = '\0';		/* and clobber colon */
	while (++w < (t + len))	/* loop collecting tokens */
	{
	    while (isspace(*w))	/* trim leading whitespace */
	        ++w;
	    p = w;		/* remember start of token */
	    while (isprint(*w) && !isspace(*w))	/* advance over token */
	        ++w;
	    *w = '\0';		/* terminate string p[] */
	    np = newname(p);
	    dp = newdep(np, dp);
	}
	np = newname(t);	/* this is ".SUFFIXES" */
	newline(np, dp, NULL_CMDP, FALSE);

	(void)free(t);		/* all done with temporary string */
}
