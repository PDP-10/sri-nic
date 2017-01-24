/*	CCDUMP.C - Debugging output routines
**
**	(c) Copyright Ken Harrenstien 1989
**		All changes after v.142, 8-Apr-1988
**	(c) Copyright Ken Harrenstien, SRI International 1985, 1986
**		All changes after v.58, 8-Aug-1985
**
**	Original version (C) 1981  K. Chen
*/

#include "cc.h"

/* Exported functions */
void dumpcode();

/* Internal functions */
static int dmpnsum();
static void dumpnode(), dmpntype(), dmpcast();

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
    fprintf(fdeb,"%4d ", nodeidx(n));

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

    case Q_GOTO:
    case N_LABEL:
	fprintf(fdeb, " = label sym \"%s\"", n->Nxfsym->Sname);
	break;

    case Q_SWITCH:
	fprintf(fdeb, ", caselist %o", nodeidx(n->Nxswlist));
	break;		/* Now do normal linkages */

    case Q_DEFAULT:
	fprintf(fdeb, ", caseptr %o\n", nodeidx(n->Nright));
	if(n->Nleft)
	    dumpnode(n->Nleft, "L", ind+4);
	return;

    case Q_CASE:
	fprintf(fdeb, " = case value %d, caseptr %o\n",
		n->Nxfint, nodeidx(n->Nright));
	if(n->Nleft)
	    dumpnode(n->Nleft, "L", ind+4);
	return;

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

    i = nodeidx(n);		/* Attempt to derive index of node */
    if(i < 0) {
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
