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
/*  Remove SSCOPE */
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
/*  Output SMACRO as well as much more info about structures */
/*  and their members */
/* <KCC.CC>CCDUMP.C.17, 29-Jan-85 20:02:00, Edit by SATZ */
/*  Dump SMACRO and the bodies; fix bug where STAG case was after statements */
/* <KCC.CC>CCDUMP.C.14,  3-Jan-85 13:12:30, Edit by SATZ */
/*  the @ now is prepended to label names */
/* <KCC.CC>CCDUMP.C.6,  1-Jan-85 18:20:05, Edit by KRONJ */
/*  rearrangement of struct type def structure */
/* SCORE:<KCC.CC>CC9.C.2, 12-May-84 22:11:43, Edit by KRONJ */
/*  improve type table dump */

/* cc9.c -- debug module   (C) 1981  K. Chen */

#include "cc.h"

/* ------------------------------------------------ */
/*	dump symbol table to symbol table file      */
/* ------------------------------------------------ */

symdump(table, name)
struct SYMBOL *table;
char *name;
{
    int u;
    char *str, *c;
    struct SYMBOL *s;

    fprintf(fsym, "\n-- Symbols for %s --\n\n", name);
    for (s = table; s != NULL; s = s->snext) {
	switch (u=s->sclass) {
	case SUNDEF: str = "undefined"; break;
	case SRW: str = "reserved word"; break;
	case SMACRO: str = "macro"; break;
	case STAG: str = "structure tag"; break;
	case STYPEDEF: str = "typedef"; break;
	case SEXTERN: str = "extern"; break;
	case SSTATIC: str = "static"; break;
	case SARG: str = "argument"; break;
	case SREGISTER: str = "register"; break;
	case SMEMBER: str = "struct member"; break;
	case SAUTO: case SSCOPE: str = "local"; break;
	case SLABEL: str = "goto label"; break;
	case SISTATIC: case SSSTATIC: str = "local static"; break;
	case SENUM: str = "enumerated type"; break;
	case SULABEL: str = "undefined goto label"; break;
	default: str = "unknown";
	}
	c = s->sname;
	if (*c == '@' || *c == '%' || *c == '#') c++;
	fprintf(fsym, "%10s: %s", c, str);
	if (u != SMACRO && u != STAG && s->sptr) {
	    fprintf(fsym, ", type %d", s->sptr-types);
	    if (s->sptr->tsize < 0)	/* tagged struct, show which one */
		fprintf(fsym, ", struct %s", s->sptr->tvar.tsym->sname+1);
	    fprintf(fsym, ", tsize %d", tsize (s->sptr));
	}
	switch (u) {
	case SAUTO: case SSCOPE:
	    fprintf (fsym, ", offset %d", s->svalue + 1);
	    break;

	case SARG:
	    fprintf (fsym, ", offset %d", - s->svalue);
	    break;

	case SENUM: case STAG:
	    fprintf(fsym, ", value %d", s->svalue);
	    break;

	case SEXTERN:
	    if (s->svalue) fprintf(fsym, ", internal");
	    break;

	case SMEMBER:
	    shoffset(s->svalue);
	    break;

	case SMACRO:
	    if (((int) s->smptr) > 0)
		fprintf(fsym, ", body %s, nargs %d", s->smptr, s->svalue);
	    break;
	}
	putc ('\n', fsym);
    }
}


/* ---------------------------- */
/*	dump type table		*/
/* ---------------------------- */

typedump()
{
    int i, u;
    char *str;
    struct TY *t;
    struct SMEM *sm;

    fputs("\n\n-- Types --\n\n", fsym);
    for(i = 0; i < maxtype; i++) {
	t = &types[i];
	switch (u = t->ttype) {
	case INT:
	    str = "int";
	    break;
	case CHAR:
	    str = "char";
	    break;
	case FLOAT:
	    str = "float";
	    break;
	case DOUBLE:
	    str = "double";
	    break;
	case VOID:
	    str = "void";
	    break;
	case ARRAY:
	    str = "array";
	    break;
	case FUNCTION:
	    str = "function";
	    break;
	case FNDEF:
	    str = "func-def";
	    break;
	case PTR:
	    str = "pointer";
	    break;
	case STRUCT:
	    str = "struct";
	    break;
	case UNION:
	    str = "union";
	    break;
	default:
	    str = "unknown";
	}

	fprintf(fsym, " %4d %s", t - types, str);

	switch (u) {
        case PTR:
	    if (t->tsize == 0) {
		str = "to";
		break;
	    }
	    fprintf (fsym, " for array");
	case ARRAY:
	    fprintf (fsym, ", size %d", t->tsize);
	    str = "of";
	    break;

	case FUNCTION:
	case FNDEF:
	    str = "returns";
	    break;

        case STRUCT:
	case UNION:
	    if (t->tsize < 0)
		fprintf (fsym, ", tag %s", t->tvar.tsym->sname + 1);
	    if (t->tsize < 0 && t->tvar.tsym->sclass != STAG)
		fprintf (fsym, ", not defined");
	    else {			/* defined, show tags and size */
		fprintf (fsym, ", size %d", tsize(t));
		sm = SMEMS (t);		/* get struct mem list */
		while (sm != NULL) {
		    fprintf (fsym, "\n      %s: type %d",
			    sm->smem->sname + 1, sm->smtype - types);
		    shoffset (sm->smoffset); /* display offset of member */
		    sm = sm->smnext;
		}
	    }
	default:
	    str = NULL;
	    break;
        }
	if (str != NULL && t->tptr != NULL) {
	    fprintf (fsym, ", %s %d", str, t->tptr - types);
	}
	putc ('\n', fsym);
    }
}

/* -------------------------------------- */
/*      show structure member offset      */
/* -------------------------------------- */

static shoffset(o)
int o;
{
    if (o >= 0) fprintf(fsym, ", offset %d", o);
    else if (o == AMBIGMEM) fprintf(fsym, ", offset ambiguous");
    else {
	o = - o;			/* negate */
	fprintf(fsym, ", offset %d, width %d, bit offset %d",
		      o >> 12, o & 077, 36 - ((o & 07700) >> 6) - (o & 077));
    }    
}

/* ---------------------------- */
/*	dump code 		*/
/* ---------------------------- */

dumpcode(n)
struct NODE *n;
{
    char *s;
    int ind = 0;

    switch (n->nop) {
    case DATA:
	s = "Data";
	break;
    case FUNCTION:
	s = "Function";
	break;
    default:
	s = "unknown";
	break;
    }
    fprintf(fdeb, "%s node, type: %d, flags: %d\n",
		s, n->ntype ? n->ntype - types : n->ntype, n->nflag);
    ind += 8;
    if (n->left)
	dumpnode(n->left, "left", ind);
    if (n->right)
	dumpnode(n->right, "right", ind);
}

dumpnode(n, branch, ind)
struct NODE *n;
char *branch;
int ind;
{
    char *s, *buffer = "                                        ";
    int size;
    struct SYMBOL *t;

    size = (ind - 1) - strlen(branch);
    buffer[size] = NULL;
    switch (n->nop) {
    case ASGN:
	s = "ASGN";
	break;
    case DATA:
	s = "DATA";
	break;
    case DCONST:
	s = "DCONST";
	break;
    case DEC:
	s = "DEC";
	break;
    case DOT:
	s = "DOT";
	break;
    case EXPRESS:
	s = "EXPRESS";
	break;
    case FNCALL:
	s = "FNCALL";
	break;
    case ICONST:
	s = "ICONST";
	break;
    case IDENT:
	s = "IDENT";
	break;
    case INC:
	s = "INC";
	break;
    case IZ:
	s = "IZ";
	break;
    case IZLIST:
	s = "IZLIST";
	break;
    case MEMBER:
	s = "MEMBER";
	break;
    case PDEC:
	s = "PDEC";
	break;
    case PINC:
	s = "PINC";
	break;
    case RETURN:
	s = "RETURN";
	break;
    case SCONST:
	s = "SCONST";
	break;
    case STATEMENT:
	s = "STATEMENT";
	break;
    default:
	s = "Unknown";
	break;
    }
    fprintf(fdeb, "%s%s op: %s (%d), flag: %d",
	buffer, branch, s, n->nop, n->nflag);
    buffer[size] = ' ';
    switch (n->nop) {
    case DCONST:
	fprintf(fdeb, ", mantissa: %d, exponent: %d\n",
	    n->nmantissa, n->nexponent);
	break;
    case ICONST:
	fprintf(fdeb, ", const: %d\n", n->niconst);
	break;
    case IDENT:
	fprintf(fdeb, ", type: %d, name: %s\n",
	    n->ntype-types, n->nid->sname);
	break;
    case SCONST:
	fprintf(fdeb, ", const: %s (%d)\n", n->nsconst, n->nsclen);
	break;
    case 0:
    case ASGN:
    case DATA:
    case DEC:
    case DOT:
    case EXPRESS:
    case FNCALL:
    case INC:
    case IZ:
    case IZLIST:
    case MEMBER:
    case PDEC:
    case PINC:
    case STATEMENT:
	putc('\n', fdeb);
	if (n->left)
	    dumpnode(n->left, "left", ind+4);
	if (n->right)
	    dumpnode(n->right, "right", ind+4);
	break;
    default:
	putc('\n', fdeb);
	break;
    }
}
