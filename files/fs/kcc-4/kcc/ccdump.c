/*	CCDUMP.C - Debugging output routines
**
**	All changes after version 58 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCDUMP.C.71, 17-Dec-85 14:00:12, Edit by IAN
   Add NEG debugging output */
/* [SRI-NIC]SS:<C.KCC.CC>CCDUMP.C.70, 17-Dec-85 08:01:46, Edit by KLH */
/*  Rationalized names of constants and structures */
/* [SRI-NIC]SS:<C.KCC.CC>CCDUMP.C.61, 14-Dec-85 12:53:41, Edit by IAN
    Add COERCE debugging output */
/* <KCC.CC>CCDUMP.C.55, 26-Jun-85 22:00:34, Edit by KRONJ */
/*  Fix dump of symbol class for scoped locals */
/* <KCC.CC>CCDUMP.C.53, 22-May-85 10:36:59, Edit by KRONJ */
/*  Fix dump of struct tag types */
/* <KCC.CC>CCDUMP.C.52, 19-Mar-85 12:07:06, Edit by SATZ */
/*  Print out number of args for macro */
/* <KCC.CC>CCDUMP.C.51, 15-Mar-85 12:02:14, Edit by KRONJ */
/*  Coerce (smptr > 0) test do do int not char compare */
/* <KCC.CC>CCDUMP.C.49, 10-Mar-85 09:56:53, Edit by KRONJ */
/*  for(...; *s ; ...) no longer compiles when  s  is pointer to struct, */
/*  fix it to what it should have been -- for(...; s != NULL; ...) */
/* <KCC.CC>CCDUMP.C.46, 24-Feb-85 22:54:45, Edit by SATZ */
/*  add more codes to codedump */
/* <KCC.CC>CCDUMP.C.44, 24-Feb-85 22:02:04, Edit by SATZ */
/*  Remove SC_SCOPE */
/* <KCC.CC>CCDUMP.C.43, 24-Feb-85 02:29:11, Edit by SATZ */
/*  Symdump tells who it is printing symbols for */
/* <KCC.CC>CCDUMP.C.42, 24-Feb-85 01:30:53, Edit by SATZ */
/*  Symbol table format is now linked list instead of a heap */
/*  fix symdump routine appropiately. Make type table dump */
/*  another routine */
/* <KCC.CC>CCDUMP.C.41, 24-Feb-85 00:15:15, Edit by SATZ */
/*  add codedump routine that outputs intermediate forms to debugging file */
/* <KCC.CC>CCDUMP.C.27, 17-Feb-85 11:06:17, Edit by SATZ */
/*  For external symbols, output if they are internal to this routine */
/* <KCC.CC>CCDUMP.C.26,  2-Feb-85 16:02:34, Edit by SATZ */
/*  only print macro body if string ptr is non-negative */
/* <KCC.CC>CCDUMP.C.25,  2-Feb-85 01:19:23, Edit by SATZ */
/*  Output SC_MACRO as well as much more info about structures */
/*  and their members */
/* <KCC.CC>CCDUMP.C.17, 29-Jan-85 20:02:00, Edit by SATZ */
/*  Dump SC_MACRO and the bodies; fix bug where SC_TAG case was after statements */
/* <KCC.CC>CCDUMP.C.14,  3-Jan-85 13:12:30, Edit by SATZ */
/*  the @ now is prepended to label names */
/* <KCC.CC>CCDUMP.C.6,  1-Jan-85 18:20:05, Edit by KRONJ */
/*  rearrangement of struct type def structure */
/* SCORE:<KCC.CC>CC9.C.2, 12-May-84 22:11:43, Edit by KRONJ */
/*  improve type table dump */

/* cc9.c -- debug module   (C) 1981  K. Chen */

#include "cc.h"

/* Exported functions */
void symdump(), typedump(), dumpcode();

/* Internal functions */
static int dmpnsum();
static void shoffset();
static void dumpnode(), dmpntype(), dmpcast();

/* ------------------------------------------------ */
/*	dump symbol table to symbol table file      */
/* ------------------------------------------------ */
void
symdump(table, name)
SYMBOL *table;
char *name;
{
    int u;
    char *str, *c, tmpstr[50];
    SYMBOL *s;

    fprintf(fsym, "\n-- Symbols for %s --\n\n", name);
    for (s = table; s != NULL; s = s->Snext) {
	switch (u=s->Sclass) {
	case SC_UNDEF:	str = "undefined"; break;
	case SC_RW:	str = "reserved word"; break;
	case SC_MACRO:	str = "macro"; break;
	case SC_TAG:	str = "structure tag"; break;
	case SC_UTAG:	str = "undef structure tag"; break;
	case SC_TYPEDEF: str = "typedef"; break;
	case SC_AEXTERN: str = "assumed-extern"; break;
	case SC_EXTERN:	str = "extern"; break;
	case SC_STATIC:	str = "static"; break;
	case SC_ARG:	str = "argument"; break;
	case SC_RARG:	str = "register-arg";	break;
	case SC_REGISTER: str = "register";	break;
	case SC_MEMBER:	str = "struct member";	break;
	case SC_AUTO:	str = "auto";		break;
	case SC_RAUTO:	str = "register-auto";	break;
	case SC_LABEL:	str = "goto label"; break;
	case SC_ISTATIC: str = "local-static"; break;
	case SC_ENUM:	str = "enumerated type"; break;
	case SC_ULABEL:	str = "undefined goto label"; break;
	default: sprintf(tmpstr, "ILLEGAL symbol class %d", u);
		str = tmpstr;
	}
	c = s->Sname;
	fprintf(fsym, "%-10s: %s", c, str);
	if (s->Sflags)
	    fprintf(fsym," (%o)", s->Sflags);
	fprintf(fsym,", refs %d", s->Srefs);
	if (u != SC_MACRO && u != SC_TAG && s->Stype) {
	    fprintf(fsym, ", type %d", s->Stype-types);
	    if (s->Stype->Tspec == TS_STRUCT
		|| s->Stype->Tspec == TS_UNION)	/* struct or union? */
		fprintf(fsym, ", struct %s", s->Stype->Tsmtag->Sname+1);
	    fprintf(fsym, ", tsize %d", sizetype(s->Stype));
	}
	switch (u) {
	case SC_AUTO: case SC_RAUTO:
	    fprintf (fsym, ", offset %d", s->Svalue + 1);
	    break;

	case SC_ARG: case SC_RARG:
	    fprintf (fsym, ", offset %d", - s->Svalue);
	    break;

	case SC_ENUM: case SC_TAG:
	    fprintf(fsym, ", value %d", s->Svalue);
	    break;

	case SC_EXTERN:
	    if (s->Svalue) fprintf(fsym, ", internal");
	    break;

	case SC_MEMBER:
	    shoffset(s->Svalue);
	    break;

	case SC_MACRO:
	    fprintf(fsym, ", nargs %d body=\"%s\"", s->Svalue, s->Smacptr);
	    break;
	}
	putc ('\n', fsym);
    }
}


/* ---------------------------- */
/*	dump type table		*/
/* ---------------------------- */

struct typflg {
	int flag; int flchar; char *flstr;
} tflagtb[] = {
	TF_CONST,	'C', "Type is a (const) object",
	TF_VOLATILE,	'V', "Type is a (volatile) object",
	TF_INTEG,	'i', "Integral type",
	TF_FLOAT,	'f', "Floating-point type",
	TF_SCALAR,	's', "Scalar type",
	TF_UNSIGN,	'u', "Unsigned type",
	TF_CHAR,	'c', "Char type",
	TF_BITF,	'b', "Bitfield type",
	TF_STRUCT,	'S', "Struct or Union type",
	TF_BYTE,	'B', "Byte (non-word) type",
	0, 0, 0,
};

void
typedump()
{
    int i, u;
    char *str;
    TYPE *t;
    SYMBOL *sm;
    char flagstr[30], *cp;
    struct typflg *fp;

    fprintf(fsym,"\n\n-- Types --\n\n\tType flags:");
    for (fp = tflagtb; fp->flag; ++fp)
	fprintf(fsym,"\t  %c - %s\n", fp->flchar, fp->flstr);
    fprintf(fsym, " Idx Flags  Bits Type\n");

    for(i = 0; i < maxtype; i++) {
	t = &types[i];			/* Get pointer to a type */

	/* Get name for type */
	if (0 <= (u = t->Tspec) && u < TS_MAX)
	    str = tsnames[u];		/* Get name of basic type */
	else
	    str = "ILLEGAL - unknown";

	/* Get flags for type */
	for (cp = flagstr, fp = tflagtb; fp->flag; ++fp)
	    if (t->Tflag & fp->flag) *cp++ = fp->flchar;
	*cp = 0;

	/* Now print basic information about type */
	fprintf(fsym, "%4d %-7s %3d %s",
			t - types, flagstr, tbitsize(t), str);

	switch (u) {
        case TS_PTR:
	    str = "to";
	    break;

	case TS_ARRAY:
	    fprintf (fsym, ", size %d", t->Tsize);
	    str = "of";
	    break;

	case TS_FUNCT:
	    str = "returns";
	    break;

        case TS_STRUCT:
	case TS_UNION:
	    fprintf (fsym, ", tag %s", t->Tsmtag->Sname);
	    if (t->Tsmtag->Sclass != SC_TAG)
		fprintf (fsym, " (not defined)");
	    else {			/* defined, show tags and size */
		fprintf (fsym, ", size %d", sizetype(t));
		sm = t->Tsmtag->Ssmnext;	/* get struct mem list */
		while (sm != NULL) {
		    fprintf (fsym, "\n      %s: type %d",
			    sm->Sname, sm->Stype - types);
		    shoffset (sm->Ssmoff); /* display offset of member */
		    sm = sm->Ssmnext;
		}
	    }
	default:
	    str = NULL;
	    break;
        }
	if (str != NULL && t->Tsubt != NULL) {
	    fprintf (fsym, ", %s %d", str, t->Tsubt - types);
	}
	putc ('\n', fsym);
    }
}

/* -------------------------------------- */
/*      show structure member offset      */
/* -------------------------------------- */

static void
shoffset(off)
int off;
{
    if (off >= 0) fprintf(fsym, ", offset %d", off);
    else {
	unsigned int o = -off;			/* negate */
	fprintf(fsym, ", offset %d, width %d, bit offset %d",
		      o >> 12, o & 077, 36 - ((o & 07700) >> 6) - (o & 077));
    }    
}

/* ---------------------------- */
/*	dump code 		*/
/* ---------------------------- */

char dmphlp[] = "\
Each line represents one parse-tree node, in the format:\n\
 # <L/R>: <Nname> (N#), nflag: <#>, ntype: <desc>, <extra stuff>\n\
where\n\
   #	- Node index.  This is sometimes shown in internal error messages.\n\
  <L/R> - Left or Right.  Left nodes are considered inferior (child) nodes\n\
	and are indented.  Right nodes are considered successors and keep\n\
	the existing indentation.  L/R == Lisp CAR/CDR.\n\
  <Nname> - Node op name, as used in KCC, followed by actual decimal index.\n\
  <#> - octal value of nflag member, if non-zero.\n\
  <desc> - description of ntype member, if non-zero: #n -> ttype\n\
	where n is its index in the types table and ttype the type's type.\n\
  <extra> - node specific information, if any.\n\
";

void
dumpcode(n)
NODE *n;
{
    char *s;
    static int helpdone = 0;
    int ind = 0;

    if (n == NULL) return;
    if(!helpdone) {
	fputs(dmphlp,fdeb);
	helpdone++;
    }
    switch (n->Nop) {
    case N_DATA:
	s = "Data";
	break;
    case N_FUNCTION:
	s = "Function";
	break;
    default:
	s = "ILLEGAL toplevel node";
	ind++;		/* Don't process further */
	break;
    }
    fprintf(fdeb, "---- %s ----\nTop: ", s);
    dmpnsum(n);
    fprintf(fdeb, "\n");

    if(ind) return;		/* Hack to avoid unknown nodes */

    if (n->Nleft)
	dumpnode(n->Nleft, "L", 8);
    if (n->Nright)
	dumpnode(n->Nright, "R", 4);
}

static void
dumpnode(n, branch, ind)
NODE *n;
char *branch;
int ind;
{
    char *s;
    int size;
    SYMBOL *t;

    /* First print node index as an identifier (helps relate to err msgs) */
    fprintf(fdeb,"%4d ", n-nodes);

    /* Do indentation.  Avoid using fancy printf features for now. */
    size = (ind - 1) - strlen(branch);
    while(--size >= 0)
	putc(' ', fdeb);

    /* Output same summary for all nodes */
    fprintf(fdeb, "%s: ", branch);	/* Do indentation */
    if(!dmpnsum(n)) return;

    /* Now do stuff specific to each node op.  In particular, every
     * op which does NOT have left+right pointers should be trapped.
     */
    switch (n->Nop) {

	/* Default is to assume that node has a link structure, since this
	 * is true for almost all node ops.  The exceptions should be
	 * handled as specific cases below.
	 */
    default: break;

    /* Take care of weirdnesses applied to n_varf (normally the flags) */
    case Q_GOTO:
    case N_LABEL:
	fprintf(fdeb, " = label sym \"%s\"", n->Nxfsym->Sname);
	break;

    case Q_CASE:	/* This hacks n_var2 also (normally Nright) */
	fprintf(fdeb, " = case value %d, caseptr %o",
		n->Nxfint, n->Nright);
	if(n->Nleft)
	    dumpnode(n->Nleft, "L", ind+4);
	return;

    /* Now handle nodes which pervert n_var2 (normally the Nright link) */
    case Q_DOT:
    case Q_MEMBER:
	fprintf(fdeb, ", offset %d\n", n->Nxoff);
	if(n->Nleft)
	    dumpnode(n->Nleft, "L", ind+4);
	return;

    case N_CAST:
	fprintf(fdeb, ", ");
	dmpcast(n->Ncast);
	putc('\n', fdeb);
	if(n->Nleft)
	    dumpnode(n->Nleft, "L", ind+4);
	return;

    /* Now handle special cases which never have links */
    case N_FCONST:
	fprintf(fdeb, ", val = %.20g\n", n->Nfconst);
	return;
    case N_ICONST:
	fprintf(fdeb, ", val = %d\n", n->Niconst);
	return;
    case N_SCONST:
	fprintf(fdeb, ", val = \"%s\" (%d)\n", n->Nsconst, n->Nsclen);
	return;
    case Q_IDENT:
	fprintf(fdeb, ", name \"%s\"\n", n->Nid->Sname);
	return;

    }

    if (tok[n->Nop].tktype == TKTY_ASOP && (n->Nascast != CAST_NONE)) {
	fprintf(fdeb, ", ascast=");
	dmpcast(n->Nascast);
    }

    /* Do default */
    putc('\n', fdeb);
    if (n->Nleft)
	dumpnode(n->Nleft, "L", ind+4);
    if (n->Nright)
	dumpnode(n->Nright, "R", ind);
}

/* dmpnsum - dump a standard summary of the node's generic contents.
 *	Returns 0 if node address is suspected to be bad.
 */
static int
dmpnsum(n)
NODE *n;
{
    int i;

    i = n - &nodes[0];		/* Attempt to derive index of node */
    if(i < 0 || i >= MAXNODE) {
	fprintf(fdeb, "ERROR: bad node address %o (= index %d)\n", n, i);
	return(0);
    }
    fprintf(fdeb, "%s (%d)", ((n->Nop > nopmax) ? "??" : nopname[n->Nop]),
		n->Nop);
    if(n->Nflag) fprintf(fdeb, ", nflag: 0%o", n->Nflag);
    if(n->Ntype) {
	fprintf(fdeb, ", ntype: ");
	dmpntype(n->Ntype);
    }
    return(1);
}

/* dmpntype - dump node's ntype value in readable form */
static void
dmpntype(typ)
TYPE *typ;
{   char *s;

    if(!typ) {
	fprintf(fdeb, "0");
	return;
    }
    fprintf(fdeb,"#%d ->", typ - types);
    while(typ) {
	if (0 <= typ->Tspec && typ->Tspec < TS_MAX) {
	    s = tsnames[typ->Tspec];
	    if (typ->Tspec == TS_STRUCT || typ->Tspec == TS_UNION)
		typ = 0;
	} else {
	    fprintf(fdeb, "ILLEGAL! %d=", typ->Tspec);
	    s = "?";
	    typ = 0;
	}
	fprintf(fdeb, " %s", s);
	if(typ) typ = typ->Tsubt;	/* Get next subtype */
    }
}

static char *castnames[] = {
#define castspec(op,str) str,
	allcastmacro		/* Expand */
#undef castspec
};

static void
dmpcast(castop)
int castop;
{    char *s;

    if (0 <= castop && castop < CAST_MAX)
	fprintf(fdeb, "%s", castnames[castop]);
    else fprintf(fdeb, "ERROR: unknown cast-type = %d", castop);
}
