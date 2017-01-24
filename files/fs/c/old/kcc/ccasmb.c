/* <KCC.CC>CCASMB.C.16, 20-Mar-85 16:11:49, Edit by WHP4 */
/*  clean up some code */
/* <KCC.CC>CCASMB.C.12, 27-Feb-85 13:06:14, Edit by SATZ */
/* Zero all of block in case garbage is getting left over */
/* <KCC.CC>CCASMB.C.10, 21-Feb-85 23:36:53, Edit by WHP4 */
/*  handle case where PRARG doesn't get anything, warn user */
/*  should eventually understand nnnKCC.TMP files for this case */
/* <WHP4>CCASMB.C.9, 21-Feb-85 15:41:38, Edit by WHP4 */
/*  fix makprg to only pass appropriate parts of filenames */
/* <WHP4>CCASMB.C.1, 12-Feb-85 18:55:04, Edit by WHP4 */
/*  makprg routine for making prarg blocks when exec doesn't make */
/*  one for us (e.g. we ran it by doing cc foo) */
/* <KCC.CC>CCASMB.C.7,  4-Feb-85 16:09:50, Edit by WHP4 */
/*  fix arg so that kcc should be able to communicate with macro */
/* <KCC.CC>CCASMB.C.6,  4-Feb-85 01:03:01, Edit by WHP4 */
/*  chain to link when loading */
/* <KCC.CC>CCASMB.C.4,  4-Feb-85 00:24:18, Edit by WHP4 */
/*  KCC now talks to exec and link */
/* SCORE:<KCC.CC>CCA.C.9,  7-Jul-84 14:55:04, Edit by KRONJ */
/*  extended addressing */
/* SCORE:<KCC.CC>CCA.C.3, 12-May-84 15:31:01, Edit by KRONJ */
/*  fix arg() to count chars correctly */
/* SCORE:<KCC.CC>CCA.C.2, 10-May-84 22:52:21, Edit by KRONJ */
/*  expunge() unused, flush */

/* cca.c -- assembly tasks   (C) 1981  K. Chen */

#include "ccsite.h"
#include "cc.h"

asmb(m, o)
char *m, *o;
{
    char str[64], *s;
#if SYS_T20
    int *arg();
#endif
    s = str;
    while (*m) *s++ = *m++;
    *s++ = ',';
    *s++ = '=';
    while (*o) *s++ = *o++;
    *s++ = '\n';
    *s = '\0';

#if SYS_T20
    if (!pfork(
		(asmtyp==AST_FAIL  ? "sys:fail.exe" :
		(asmtyp==AST_MACRO ? "sys:macro.exe" :
		(asmtyp==AST_MIDAS ? "sys:midas.exe" : NULL))),
		arg(str),PRGBLEN,1,0))
#endif
	fatal(EASM);			/* run fail or macro, don't chain */
    if (verbose) fprintf(stderr, "\n");
}

#if SYS_T20
/* ----------------------------------- */
/*      make a 7-bit byte pointer      */
/* ----------------------------------- */

static char *
bp7 (ip)
int *ip;
{
    int i = ip;
    i |= ((i &~ 0777777) ? 0620000000000 : 0350700000000);
    return (char *) i;
}
#endif /* T20 */

#if SYS_T20
static int block[PRGBLEN];

static int *
arg(s)
char *s;
{
    int  i;
    char *t,*u;

    /* clear out soon-to-be prarg block to zeroes */
    for (i = 0; i < PRGBLEN; i++) block[i] = 0;

    /* perform gross klugery to get 7-bit byte pointers */
    t = u = bp7 (&block[17]);

    /* get length word for block */
    i = 0;
    while (*t++ = *s++) i++;		/* count chars in string */
    i = i / 5 + 1;			/* turn into number of words */

    /* fill out with calculated info and return */
    block[0] = 1;
    block[1] = 16;
    block[2] = 16 + i;
    switch(asmtyp) {
	case AST_FAIL:				/* using FAIL */
	        block[16] = 0464151000000 + i;	/* sixbit 'FAI',,i */
		break;
	case AST_MACRO:				/* using MACRO */
		block[16] = 0554143000000 + i;	/* sixbit 'MAC',,i */
		break;
	case AST_MIDAS:				/* using MIDAS */
		block[16] = 0555144000000 + i;	/* sixbit 'MID',,i */
		break;
	default: block[16] = 0;
    }
    return block;
}
#endif /* T20 */

/* ------------------------------ */
/*      get prarg args from exec  */
/* ------------------------------ */
char *
gprarg(loc,len)
int *loc;
int len;
{
#if SYS_T20
    int acs[5];				/* ac block for jsys */
    int i;
		
    acs[1] = 01400000;			/* .prard,,.fhslf */
    acs[2] = (int) loc;			/* location */ 
    acs[3] = len;			/* length */
    acs[4] = 0;				/* just to be safe */

#define PRARG 0545			/* prarg% jsys */

    for (i = 0; i < PRGBLEN; i++)
	loc[i] = 0;			/* zero out storage on stack */
    if (!jsys(PRARG,acs)) return NULL;	/* no prarg */
    if (!acs[3]) {
	fprintf(stderr, "kcc: nothing from PRARG, maybe someone should write TMP code!\n");
	return NULL;			/* no words returned, punt */
    }
    if (!*loc) return NULL;		/* first cell 0, punt */
    return bp7(loc[1] + loc + 1);	/* form byte pointer to text */
#else
    return(NULL);
#endif /* Not T20 */
}

/* ------------------------- */
/*    get another prarg arg  */
/* ------------------------- */

gparg(pptr, prstr)
char **pptr;
char *prstr;
{
#if SYS_T20
    int ch;
    char *p;

    p = *pptr;

    while (((ch = *p++) != '\0') && (ch != '='))
	;				/* skip first part */
    if (!ch) {				/* ran out of stuff */
	*prstr++ = '\0';		/* give back empty string */
	*pptr = p;			/* reset pointer */
	return(0);			/* and return failure */
    }
    while (((ch = *p++) != '\0') && (ch != '\n') && (ch != '\015'))
	*prstr++ = ch;			/* copy string */
    *prstr++ = '\0';			/* null terminate */
    *pptr = p;				/* reset pointer */
    return(1);				/* return success */
#else
    return(0);
#endif /* Not T20 */
}

/* ------------------------- */
/*   make prarg block for    */
/*  link if none from exec   */
/* ------------------------- */
makprg(argct,argvt,loc)
int argct,*loc;
char **argvt;
{
#if SYS_T20
    int i;
    char *s, *t, *u, *pstamb = "\015\012/go\015\012";
    char module[_FILSIZ];

    for (i = 0; i < PRGBLEN; i++)
	loc[i] = 0;			/* zero out storage on stack */

    t = u = bp7(loc+17);

    s = *argvt++;
    argct--;
    while (**argvt == '-') {
	argvt++;			/* assume switches all before files */
	argct--;
    }
    while (argct--) {			/* while we have arguments left */
	s = *argvt++;
	fnpart(module, s);
        if (module[0] != '\0') {
	    s = module;
	    while ((*s) && (*s != '.')) *t++ = *s++;
        }
        *t++ = ',';
    }
    t--;
    while (*pstamb) *t++ = *pstamb++;
    *t++ = '\0';

    i = 0;
    while (*u++) i += 1;
    i = i/5+1;				/* round off to words */
    loc[0] = 2;				/* 2 argument strings for runlnk */
    loc[1] = 16;			/* fake out runlnk */
    loc[2] = 16;			/* type at word 16 */
    loc[3] = 0;				/* no more strings */
    loc[16] = 0545653000000 + i;	/* make sixbit 'LNK' ,, i */
#endif /* T20 */
}
    
/* ------------------------------ */
/*     pass prarg args to link    */
/*     and chain through (groan)  */
/* ------------------------------ */

runlnk(loc)
int *loc;
{
    int i;
		
    if (loc[0] != 2) return(0);		/* punt if we don't have right args */
    loc[1] = loc[2];			/* shuffle things around */
    loc[2] = 0;				/* zero old cell for safety */
    loc[0] = 1;				/* only one set of arguments */
    fflush(stdout);			/* make sure no output lost */
					/* when we chain to link */
#if SYS_T20
    if (!pfork("sys:link.exe",loc,PRGBLEN,1,1))
#endif /* T20 */
	fatal(EASM);			/* never to return */
}

/* ----------------------------------------------------- */
/*      extract filename part of file specification      */
/* ----------------------------------------------------- */

fnpart(dest, source)
char *dest, *source;
{
    char *s;
    int ext;

    s = dest;				/* start with dest */
    *s = '\0';				/* terminate so can tell if get name */
    ext = 0;				/* start copying file name */

    while (1) switch (*source) {	/* go through filename... */
    case '\0':				/* null */
    case '|':				/* and vert bar terminate filename */
	return;

#if SYS_ITS
    case ' ':
#endif
    case '.':				/* period delimits name from ext */
    case '<':				/* directory start chars also work */
    case '[':
	ext = 1;			/* remember not copying any more */
	source++;			/* move on */
	break;

#if SYS_ITS
    case ';':
#endif
    case '>':				/* various directory terminators */
    case ']':
    case ':':
    case '/':
	s = dest;			/* set up to start again */
	ext = 0;			/* but no null in case this is the */
	source++;			/* end of a SAIL file name */
	break;				/* or TOPS-20 logical name */

#if SYS_ITS
    case '\021':			/* ^Q */
#endif
    case '\026':			/* ctrl-V */
    case '\\':				/* and backslash: */
	if (!ext) *s++ = '\026';	/* copy across */
	source++;			/* and move on to quoted char */
    default:				/* normal filename char */
	if (!ext) {
	    *s++ = *source++;		/* copy across */
	    *s = '\0';			/* null terminate */
	} else source++;
	break;
    }
}
