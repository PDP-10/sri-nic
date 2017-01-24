/* 
** GETENV, PUTENV - Get and put value for environment name
**
**	Copyright (c) 1987 by Ken Harrenstien, SRI International
**
** Original idea of using logical names from:
**	Bill Palmer / Stanford University / November 1984
*/

#include "c-env.h"
#include "stddef.h"		/* For NULL */

#ifndef _MAXENVNAME
#define _MAXENVNAME 40		/* Max size of environment name (less 1) */
#endif

struct evar {
	struct evar *enext;
	char *ename;
	char *evalue;
};

#if (SYS_T20+SYS_10X)
#include "jsys.h"
#include "ctype.h"		/* For isupper/tolower */
#endif

static struct evar *env_list = 0;

static struct evar *_putenv(), *_sevterm(), *_sevhome();

char *
getenv(name)
char *name;
{
    register struct evar *ep;
    register char *cp;

    /* Search existing env vars for a match */
    for (ep = env_list; ep; ep = ep->enext) {
	if (strcmp(name, ep->ename)==0)		/* If matched exactly, */
	    return ep->evalue;			/* return value for name! */
    }

    /* No existing match, try to check other sources */
#if SYS_T20
	/* For TOPS-20, we use silly hack of having logical name defs
	** be environment vars.  To avoid conflicts with real device
	** names etc, all such vars have "CENV-" prefixed to the normal
	** variable name.  e.g. "TERM" is looked up as the logical name
	** "CENV-TERM:".
	*/
    {
	int acs[5];
	char lognam[5 + _MAXENVNAME];
	char logbuf[1000];		/* Lots of room */

	strncat(strcpy(lognam,"CENV-"), name, _MAXENVNAME-5-1);
	acs[1] = _LNSJB;		/* First try jobwide logical name */
	acs[2] = (int)(lognam-1);	/* Need backed-up BP for jsys */
	acs[3] = (int)(logbuf-1);	/* Store into here */
	logbuf[0] = 0;			/* Ensure buffer starts clear */
	if (jsys(LNMST, acs) == 0) {	/* If jobwide failed, */
	    acs[1] = _LNSSY;		/* then try systemwide */
	    jsys(LNMST, acs);
	}
	if (logbuf[0]) {			/* If we won... */
	    for (cp = logbuf; *cp; ++cp)	/* Convert to lowercase */
		if (isupper(*cp))
		    *cp = tolower(*cp);
	    ep = _putenv(name, logbuf);		/* Store it as env var! */
	    return (ep ? ep->evalue : NULL);	/* and return alloced value */
	}
    }
#endif /*SYS_T20*/

    /* Try checking for specific names which have standard meanings */
    if (strcmp(name, "TERM")==0)	/* TERMCAP terminal type */
	return (ep = _sevterm()) ? ep->evalue : 0;
    if (strcmp(name, "HOME")==0)	/* User's home directory */
	return (ep = _sevhome()) ? ep->evalue : 0;

    return NULL;			/* Give up, failed */
}

/* PUTENV - based on SYSV description.
**	Argument is a string in "name=value" form.  Adds this to 
** the environment of current process.  Not clear if we want to
** emulate the (vague) SYSV description which indicates that putenv
** may only store a pointer, and the actual string buffer remains under
** user control.
*/
int
putenv(namval)
char *namval;
{
    register char *np, *cp;
    register int i;
    register struct evar *ep, *prep;
    char name[_MAXENVNAME];

    /* Get "name" part isolated in temp buffer (to avoid changing orig str) */
    for (i = _MAXENVNAME, cp = name, np = namval; *np != '=';) {
	if (*np == 0 || --i > 0)	/* If not in name=val format */
	    return -1;			/* then fail */
	*cp++ = *np++;
    }
    *cp = 0;			/* Tie off name string */
    ++np;			/* Point to value string */

    /* Search existing env vars for a match */
    for (ep = env_list, prep = NULL; ep; ep = ep->enext) {
	if (strcmp(name, ep->ename)==0)		/* If matched exactly, */
	    break;				/* stop loop */
	prep = ep;				/* Save previous ptr */
    }

    if (ep == NULL)				/* If didn't find, */
	return (_putenv(name, np) ? 0 : -1);	/* just add it */

    if (strlen(np) <= strlen(ep->evalue)) {	/* If new string small enuf */
	strcpy(ep->evalue, np);			/* just copy over old one */
	return 0;				/* and win! */
    }

    /* Must delete old entry, then add new one */
    if (prep) prep->enext = ep->enext;		/* Unlink from list */
    else env_list = ep->enext;
    free((char *)ep);				/* Free up old struct */
    return (_putenv(name, np) ? 0 : -1);	/* Then add new and return */
}

/* _PUTENV - auxiliary for getenv() and putenv().
**	Takes a name/value pair and strings it into the environment
** variable list.
*/
static struct evar *
_putenv(name, value)
char *name, *value;
{
    register struct evar *ep;
    register int nl, vl;
    extern char *malloc();

    nl = strlen(name);
    vl = strlen(value);
    ep = (struct evar *) malloc(sizeof(struct evar) + nl+1 + vl+1);
    if (ep != NULL) {
	ep->ename = (char *)(ep+1);	/* Name starts right after struct */
	ep->evalue = ep->ename + nl+1;	/* Value right after that */
	strcpy(ep->ename, name);	/* Copy the strings */
	strcpy(ep->evalue, value);
	ep->enext = env_list;		/* Link into head of list */
	env_list = ep;
    }
    return ep;		/* NULL or pointer to new linked-in var */
}

/* Auxiliary routines for specific environment names */

/* Find and set HOME variable */
static struct evar *
_sevhome()
{
    register struct evar *ep;
    char buf[500];
#if SYS_T20+SYS_T10
    int acs[5];
    jsys(GJINF, acs);		/* Get user # in AC 1 */
    acs[2] = acs[1];		/* Set up for DIRST% */
#if SYS_T20
    strcpy(buf, "PS:<");	/* on T20 must add punctuation */
    acs[1] = (int) (buf+3);
#else				/* on 10X system adds punct for us */
    acs[1] = (int) (buf-1);
#endif
    if (jsys(DIRST, acs)) {
#if SYS_T20
	strcat((char *)acs[1], ">");
#endif
	return _putenv("HOME", buf);
    }
#endif

    return NULL;
}

/* Find and set TERM variable */

static struct evar *
_sevterm()
{
#if SYS_T20+SYS_10X
    register char *str;
    int acs[5];

    acs[1] = _CTTRM;
    jsys(GTTYP, acs);		/* Get our terminal type */
    switch (acs[2]) {
	default:	str = NULL;	break;	/* If don't know type */

#ifdef _TT33
	case _TT33:	str = "tty33";	break;
#endif
#ifdef _TT35
	case _TT35:	str = "tty35";	break;
#endif
#ifdef _TT37
	case _TT37:	str = "tty37";	break;
#endif
#ifdef _TTEXE
	case _TTEXE:	str = "execuport"; break; /* Probably not in TERMCAP */
#endif
#ifdef _TTDEF
	case _TTDEF:	str = "DEFAULT";	break;	/* ???? */
#endif
#ifdef _TTIDL
	case _TTIDL:	str = "IDEAL";	break;		/* ???? */
#endif
#ifdef _TTV05
	case _TTV05:	str = "vt05";	break;		/* Not in TERMCAP */
#endif
#ifdef _TTV50
	case _TTV50:	str = "vt50";	break;
#endif
#ifdef _TTL30
	case _TTL30:	str = "la30";	break;		/* Not in TERMCAP? */
#endif
#ifdef _TTG40
	case _TTG40:	str = "gt40";	break;
#endif
#ifdef _TTL36
	case _TTL36:	str = "la36";	break;		/* Not in TERMCAP? */
#endif
#ifdef _TTV52
	case _TTV52:	str = "vt52";	break;
#endif
#ifdef _TT100
	case _TT100:	str = "vt100";	break;
#endif
#ifdef _TTL38
	case _TTL38:	str = "la38";	break;		/* Not in TERMCAP? */
#endif
#ifdef _TT120
	case _TT120:	str = "la120";	break;
#endif
#ifdef _TT125
	case _TT125:	str = "vt125";	break;
#endif
#ifdef _TTK10
	case _TTK10:	str = "gigi";	break;	/* DEC VK100 */
#endif
#ifdef _TT102
	case _TT102:	str = "vt102";	break;
#endif
#ifdef _TTH19
	case _TTH19:	str = "h19";	break;
#endif
#ifdef _TT131
	case _TT131:	str = "vt131";	break;		/* Not in TERMCAP? */
#endif
#ifdef _TT200
	case _TT200:	str = "vt200";	break;
#endif
#ifdef _TTADM
	case _TTADM:	str = "adm3a";	break;
#endif
#ifdef _TTDAM
	case _TTDAM:	str = "dm2500";	break;
#endif
#ifdef _TTHP
	case _TTHP:	str = "hp2645";	break;
#endif
#ifdef _TTHAZ
	case _TTHAZ:	str = "h1500";	break;
#endif
#ifdef _TT43
	case _TT43:	str = "tty43";	break;
#endif
#ifdef _TTSRC
	case _TTSRC:	str = "soroc";	break;	/* Soroc 120 */
#endif
#ifdef _TTGIL
	case _TTGIL:	str = "gillotine"; break;	/* Not in TERMCAP? */
#endif
#ifdef _TTTEL
	case _TTTEL:	str = "t1061";	break;	/* Teleray 1061 */
#endif
#ifdef _TTTEK
	case _TTTEK:	str = "tek4025";	break;
#endif
#ifdef _TTANN
	case _TTANN:	str = "aaa";	break;	/* Ann Arbor */
#endif
#ifdef _TTCPT
	case _TTCPT:	str = "concept100";	break;
#endif
#ifdef _TTIBM
	case _TTIBM:	str = "ibm3101";	break;	/* IBM 3101-20 */
#endif
#ifdef _TTTVI
	case _TTTVI:	str = "tvi912";	break;
#endif
#ifdef _TTTK3
	case _TTTK3:	str = "tek4023";	break;
#endif
#ifdef _TTDM2
	case _TTDM2:	str = "dm1520";	break;
#endif
#ifdef _TTAMB
	case _TTAMB:	str = "ambassador";	break;	/* Ann Arbor */
#endif
#ifdef _TTESP
	case _TTESP:	str = "esprit";	break;	/* Hazeltine */
#endif
#ifdef _TTFRD
	case _TTFRD:	str = "freedom100";	break;
#endif
#ifdef _TTFR2
	case _TTFR2:	str = "freedom200";	break;
#endif
#ifdef _TTANS
	case _TTANS:	str = "ansi";	break;
#endif
#ifdef _TTAVT
	case _TTAVT:	str = "avt";	break;	/* Concept AVT */
#endif
    }	/* end of switch on terminal type */

    if (str)				/* If found something */
	return _putenv("TERM", str);	/* return that! */
#endif	/* T20+10X */

    return NULL;			/* Fail */
}
