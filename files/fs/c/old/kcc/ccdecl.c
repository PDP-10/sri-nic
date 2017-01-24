/* <KCC.CC>CCDECL.C.151, 20-Jul-85 15:52:07, Edit by KRONJ */
/*  Make normal tsize of pointer zero so can have nonzero tsize */
/*  for array coerced to pointer so  int x[]; sizeof x;  can work */
/* <KCC.CC>CCDECL.C.149, 27-Jun-85 13:22:52, Edit by KRONJ */
/*  Don't ignore static in function declaration */
/* <KCC.CC>CCDECL.C.148, 19-Jun-85 10:54:43, Edit by KRONJ */
/*  Improve entry statement for #asm in middle, funny ident chars */

/*
** ccdecl - Declaration parser for KCC
** (C) 1981  K.Chen
*/

#include "cc.h"

/* functions from outside used herein */
extern struct SYMBOL *findsym(), *creatsym(), *creatloc(), *newlabel();
extern struct TY *findtype();
extern node expression(), statement(), defnode();
extern int nextoken(), expect();
extern void warn(), error(), outsym(), nl();

/* functions herein exported to the outside */
struct TY *typename(), *typespec();
void extdef(), decllist();
int pconst(), tsize();

/* functions used entirely internally */
void entstmt(), funcdef(), datadef(), decln(), idecllist();
int sdecllist(), sdeclunion(), sdeclstruct(), fldsize(), tmismatch(), pscope();
node izer(), pizer();
struct SYMBOL *declarator();
struct TY *decl0(), *addpp(), *pushtype(), *strspec(), *enumspec();
struct SMEM **sdeclaration();

/* ------------------------------------- */
/*	process external definition      */
/* ------------------------------------- */

void
extdef()
{
    struct SYMBOL *s, tempsym, base, *fn;
    struct TY *t;
    char *cp;
    int sval;

    if (!firstime) {			/* first time through here? */
	while (token == ENTRY) {
	    s = csymbol;		/* get symbol */
	    if (nextoken() != IDENT) {	/* must be followed by ident */
		tokpush (ENTRY, 0);	/* put ENTRY back on stack */
		break;			/* stop doing entry statements */
	    }
	    entstmt();			/* process entry statements */
	}
	firstime++;			/* don't do this again */
	freesym (findsym ("entry"));	/* flush "entry" reserved word */
	if (token == ENTRY) {		/* if we found a bogus entry */
	    strcpy (ident, "entry");	/* put it back into ident string */
	    lexident();			/* turn back into IDENT token */
	} else if (token == IDENT) for (cp = csymbol->sname; *cp != '\0'; cp++)
	    if (*cp == '$' || *cp == '%') { /* make sure ident clean */
		error (EIDENT);		/* not, so complain */
		*cp = '\0';		/* break out of loop */
	    }
	if (eof) return;		/* go back if no more to do */

    }

    scope = XTERN;			/* default to global */
    tempsym.sname[0] = '\0';		/* init symbol */
    tempsym.sclass = pscope();		/* parse scope */
    tempsym.svalue = 0;			/* no val yet */
    tempsym.sptr = typespec();		/* parse type */
    if (token == SCOLON) {		/* just the type (struct)? */
	nextoken();			/* yes, skip over final semi */
	return;				/* and stop doing this def */
    }
    copysym(&base,&tempsym);

    curfnnew = line;			/* remember where line started */
    if ((s = declarator(&tempsym,0)) == NULL) return;
    fn = s;				/* remember function symbol */
    sval = s->svalue;			/* save value for later */
    s->svalue = (int) (struct SYMBOL *) NULL; /* no args */
    t = s->sptr;
    if (t->ttype == FNDEF) {		/* parsed "fnname (" */
	scope = SARG;
	while (1) {			/* parameter-list */
	    if (token != IDENT) break;	/* must be parameter */
	    symcpy(base.sname, csymbol->sname);	/* save name */
	    if (csymbol->sclass == SUNDEF) freesym(csymbol); /* flush global */
	    s->svalue = (int) creatloc(base.sname); /* get local, chain args */
	    s = (struct SYMBOL *) s->svalue; /* move on to end of chain */
	    s->sclass = SARG;		/* this is a fun arg */
	    s->svalue = (int) (struct SYMBOL *) NULL; /* so far its the last */
	    s->sptr = deftype;		/* integer unless declared */
	    nextoken();
	    if (token != COMMA) break;
	    nextoken();
	}
	expect(RPAREN);
	fn->sptr = findtype (FUNCTION, PTRSIZE, fn->sptr->tptr);
    } else if (t->ttype != FUNCTION || token == COMMA || token == SCOLON ||
	       token == ASGN) {
	fn->svalue = sval;		/* restore saved value */
	datadef(s, &base);		/* extern fun or variable */
	return;
    }
    s = (struct SYMBOL *) fn->svalue;	/* get arglist back */
    fn->svalue = sval;			/* restore saved value */
    funcdef(fn,s);			/* internal fun */
}

/* ----------------------------------- */
/*	type-name  Ref.[1]  A.8.7      */
/* ----------------------------------- */

struct TY *
typename()
{
    struct SYMBOL s, *t;

    s.sname[0] = '\0';
    s.sclass = s.svalue = 0;
    s.sptr = typespec();
    t = declarator(&s, 3);
    return t->sptr;
}

/* ----------------------------------------- */
/*	parse for a constant expression      */
/* ----------------------------------------- */

int
pconst()
{
    node e;

    e = expression();
    if (e->nop != ICONST) {
	error(ECONST);
	return 0;
    }
    return e->niconst;
}

/* ----------------------------------------- */
/*	declaration-list  Ref.[1] A.18.3     */
/* ----------------------------------------- */

void
decllist()
{
    int n;

    while (csymbol != NULL) {
	if ((n = csymbol->sclass) == SRW) {
	    n = csymbol->skey;
	    if (n != TYPESPEC && n != SCSPEC && n != SUSPEC) return;
	} else if (n != STYPEDEF) return;
	decln();
    }
}

/* ----------------------- */
/*	size of type  	   */
/* ----------------------- */

int
tsize(t)
struct TY *t;
{
    int s;

    if (t == NULL) return 0;		/* izlist => izer0/1 does this, why? */

    /* calculate factor for array dimensions */
    s = 1;				/* nothing multiplied in yet */
    while (t->ttype == ARRAY) {		/* array has to multiply out ranges */
	s *= t->tsize;			/* so multiply it in with rest */
	t = t->tptr;			/* and go to next in chain */
    }

    /* multiply that by size of base type */
    if ((t->ttype == STRUCT || t->ttype == UNION) && t->tsize < 0) {
	if (t->tvar.tsym->sclass != STAG)
	    error (ESTRSIZE, t->tvar.tsym->sname + 1);
	s *= t->tvar.tsym->svalue;	/* finish up size of tagged struct */
    } else if (t->ttype == CHAR) s = (s + NBYTES - 1) / NBYTES;	/* chr array */
    else if (t->ttype == PTR) s *= PTRSIZE; /* pointer to anything */
    else s *= t->tsize;			/* or of other type */
    return s;				/* return final number of words */
}

/* --------------------------------------- */
/*	type-specifier  Ref[1] A.18.2      */
/* --------------------------------------- */

struct TY *
typespec()
{
    int signed, size;
    struct TY *t;

    t = NULL;				/* no type given yet */
    signed = 0;				/* signed unless marked otherwise */
    size = 0;				/* no size given */

    while (csymbol != NULL) {
	switch (csymbol->skey) {
	case TYPESPEC:
	    switch (token) {
	    case UNSIGNED:
		if (signed) error(ETYPCOMB); /* only one of these */
		signed = token;		/* remember we got one */
		break;

	    case SHORT:
	    case LONG:
		if (size) error(ETYPCOMB); /* only one of these */
		size = token;		/* remember we got one */
		break;

	    default:
		error(ETYPCOMB);	/* huh? */
	    }
	    nextoken();			/* move to next token */
	    continue;			/* and continue with loop */

	case SUSPEC:
	    if (signed || size) error(ETYPCOMB); /* can't qualify struct */
	    return strspec();		/* just go do it */

	default:
	    if (t == NULL && token == IDENT && csymbol->sclass == STYPEDEF) {
		t = csymbol->sptr;	/* get the type */
		nextoken();		/* skip over it */
		continue;		/* on to next */
	    }
	}				/* end switch(csymbol->skey) */
	break;				/* fall out of loop for default case */
    }					/* end while(csymbol != NULL) */

    return (t == NULL)? deftype : t;
}

/* ------------------------------------ */
/*	declarator  Ref[1]  A.18.2      */
/*					*/
/*	tag = 0    normal		*/
/*	    = 1	   structure tag	*/
/*	    = 2	   func arguments	*/
/*          = 3    abstract decl        */
/* ------------------------------------ */

static struct SYMBOL *
declarator (s, tag)
struct SYMBOL *s;
{
    s->sptr = decl0(s, s->sptr, tag);
    switch (s->sptr->ttype) {
    case ARRAY:
	if (tag == 2)			/* change array as arg to ptr */
	    s->sptr = pushtype(PTR, 0, s->sptr->tptr);
	break;

    case FLOAT:
	if (tag == 2) s->sptr = dbltype; /* change float arg to double */
	break;

    case VOID:
	if (tag != 3 && s->sclass != STYPEDEF)
	    error (EVOID, (tag == 1? s->sname + 1 : s->sname));
	break;
    }
    return s;
}

/* ------------------------------------- */
/*      parse type for declarator()      */
/* ------------------------------------- */

static struct TY *
decl0 (s, t, tag)
struct SYMBOL *s;
struct TY *t;
{
    struct TY *pp;

    /* parse stars before main type part */
    while (token == MPLY) {
	t = pushtype (PTR, 0, t);
	nextoken();
    }

    /* now parse main type part */
    switch (token) {
    case LPAREN:
	nextoken();			/* move over paren */
	pp = decl0(s, NULL, tag);	/* parse stuff in parens */
	expect(RPAREN);
	break;
    case IDENT:
	if (tag == 3) error(SYNTAX);	/* cast or sizeof, ident not allowed */
	else if (tag == 1) {
	    symcpy(s->sname, "#");	/* prefix for struct members */
	    symapp(s->sname, csymbol->sname);
	    if (csymbol->sclass == SUNDEF) freesym(csymbol);
	} else symcpy(s->sname, csymbol->sname);
	nextoken();
    default:
	pp = NULL;			/* no paren part */
	break;
    }

    /* function and array specifier part */
    while(1) {
	switch (token) {
	case LPAREN:
	    nextoken();
	    switch (token) {
	    case RPAREN:
		nextoken();
		pp = addpp(pp, pushtype(FUNCTION, PTRSIZE, NULL));
		break;
	    case IDENT:
		return addpp(pp, pushtype (FNDEF, PTRSIZE, t));
		/* function declaration with arguments, return pointing */
		/* to the first argument still. */
	    default:
		error(SYNTAX);
	    }
	    break;

	case LBRACK:
	    nextoken();
	    pp = addpp(pp, pushtype(ARRAY,
				    (token == RBRACK)? 0 : pconst(),
				    NULL));
	    expect(RBRACK);
	    break;
	    
	default:
	    /* go back and add paren part, return the result */
	    return addpp(pp, t);
	}
    }
}

/* --------------------------------------- */
/*      add type to inside of nesting      */
/* --------------------------------------- */

static struct TY *
addpp (pp, t)
struct TY *pp, *t;
{
    /*
    ** This takes a base-less type structure in pp, and returns the
    ** result of replacing the NULL where the base should be with t.
    ** Thus it is the inverse of pushtype(), adding the new type
    ** at the base of the structure rather than at the top.
    **
    ** I realize the recursive definition below may look messy,
    ** but an iterative definition of this function would be worse...
    */

    return (pp != NULL)?
	     pushtype(pp->ttype, pp->tsize, addpp(pp->tptr, t)) :
	     t;
}

/* ----------------------------------------------------------- */
/*      Check for valid type combinations and return type      */
/* ----------------------------------------------------------- */

static struct TY *
pushtype(typ, siz, ptr)
struct TY *ptr;
{
    /* check for functions returning other functions */
    if (ptr != NULL) switch (typ) {
    case ARRAY:
	switch (ptr->ttype) {
	case VOID:
	    error (EVOID, "array element");
	    break;

	case ARRAY:
	    if (ptr->tsize != 0) break;	/* array[][x] ok, array[x][] not */
	case FUNCTION:
	case FNDEF:
	    error(ETYPCOMB);
	    ptr = NULL;
	}
	break;

    case FUNCTION:
    case FNDEF:
	switch (ptr->ttype) {
	case ARRAY:
	case FUNCTION:
	case FNDEF:
	    error(ETYPCOMB);
	    ptr = NULL;
	}
    }

    /* now hash up the actual type and return the canonicalized version */
    return findtype (typ, siz, ptr);
}

/* --------------------------------------- */
/*      entry statement  Ref[1] A.2.3      */
/* --------------------------------------- */

static void
entstmt()
{
    while (1) {
	outstr ("\tENTRY\t");		/* make ENTRY statement */
	outsym (csymbol->sname);	/* before we call expect() */
	nl();				/* so #asm in middle won't screw us */
	expect (IDENT);			/* it better have been a sym */
	if (token != COMMA) break;	/* repeat while we have more */
	nextoken();			/* skip COMMA */
    }
    expect(SCOLON);			/* end with semicolon */
}

/* -------------------------------------------------- */
/*	struct-or-union specifier  Ref[1] A.18.2      */
/* -------------------------------------------------- */

static struct TY *
strspec()
{
    struct SYMBOL s, *tagname;
    struct TY *t;			/* really either SMEM or SYMBOL */
    int typ, siz;

    typ = token;			/* remember whether struct or union */
    nextoken();
    if (typ == ENUM) return enumspec();	/* enum, go handle differently */

    if (token != IDENT) {
	expect(LBRACE);			/* gotta have something */
	siz = sdecllist(&t, typ == UNION); /* parse unnamed struct */
    } else {				/* tagged struct */
	symcpy(s.sname, "%");		/* tptr points to tag, */
	symapp(s.sname, csymbol->sname); /* tag->sptr points to smems */
	if (csymbol->sclass == SUNDEF) freesym(csymbol);
	if ((tagname = findsym(s.sname)) == NULL) { /* no such struct? */
	    tagname = creatsym(s.sname); /* make a new tag symbol */
	    tagname->sclass = SUNDEF;	/* but with no defined body */
	}
	nextoken();			/* skip over ident */
	if (token == LBRACE) {			/* struct FOO { */
	    if (tagname->sclass != SUNDEF)	/* can't define twice */
		error((typ == STRUCT)? EDSTRUC : EDUNION, s.sname + 1);
	    nextoken();			/* skip over open brace */
	    tagname->sclass = STAG;	/* this is now a defined struct */
	    tagname->svalue = sdecllist(&tagname->svar.ssmem, typ == UNION);
	}
	siz = -1;			/* special size means tagged */
	t = (struct TY *) tagname;	/* with the tag we found or made */
    }
    return findtype(typ, siz, t);	/* look up its type */
}

/* ------------------------------------------ */
/*	struct-decl-list  Ref.[1]  A.8.5      */
/* ------------------------------------------ */

static int
sdecllist(smem, isunion)
struct SMEM **smem;
{
    if (isunion) return sdeclunion(smem);
    return sdeclstruct(smem);
}

static int
sdeclunion(smem)
struct SMEM **smem;
{
    int maxsize, offset, boffset;

    maxsize = 0;
    while (token != RBRACE) {
	if (eof) earlyend();
	offset = boffset = 0;		/* each member starts at zero */
	smem = sdeclaration(smem, &offset, &boffset);
	if (boffset > 0) offset++;	/* round out to full word */
	if (offset > maxsize) maxsize = offset;	/* find max with old size */
    }
    nextoken();				/* skip over close brace */
    return maxsize;			/* largest elt size is union size */
}

static int
sdeclstruct(smem)
struct SMEM **smem;
{
    int offset, boffset;

    offset = boffset = 0;		/* structure starts at zero */
    while (token != RBRACE) {
	if (eof) earlyend();
	smem = sdeclaration(smem, &offset, &boffset);
    }
    nextoken();				/* skip over close brace */
    if (boffset > 0) offset++;		/* round offset out to full word */
    return offset;			/* return as size of struct */
}

/* -------------------------------------------- */
/*	struct-declaration  Ref.[1]  A.8.5      */
/* -------------------------------------------- */

static struct SMEM **
sdeclaration (smem, offset, boffset)
struct SMEM **smem;
int *offset, *boffset;
{
    struct SYMBOL base, tempsym, *t, *u;
    int s;

    base.sname[0] = '\0';
    base.sclass = SMEMBER;
    base.svalue = 0;
    base.sptr = typespec();

    while (1) {
	if (token == COLON) {

	    /*
	    ** Colon without declarator before it - specifies space
	    ** left for alignment.  Constant expression following
	    ** colon is how much space, or zero to align to a word.
	    */

	    nextoken();			/* skip over colon */
	    s = fldsize(offset, boffset); /* parse size */
	    if (s == 0 && *boffset > 0) { /* if rounding to word offset */
		*boffset = 0;		/* round off bit offset */
		(*offset)++;		/* and move up to next word bdy */
	    }
	} else {

	    /*
	    ** Normal declarator.  Parse it, then check if there is
	    ** a colon expression after it making it a bit field, or
	    ** if it is a whole word expression.
	    **
	    ** For bitfields, the offset is encoded as follows:
	    **  s % 07777 - high 12 bits of byte pointer to the field
	    **  s >> 12   - word offset in struct
	    ** and then the whole thing is negated.
	    **
	    ** Note that we let the bit offset remain at 36 rather
	    ** than folding it to zero - the calculations are easier.
	    */

	    copysym(&tempsym, &base);
	    t = declarator(&tempsym, 1);
	    if (token == COLON) {	/* bitfield */
		nextoken();		/* move over colon */
		s = fldsize(offset, boffset);
		s = -(((*offset * 64) + 36 - *boffset) * 64 + s);
	    } else {			/* normal size member */
		if (*boffset > 0) {	/* if we need to go to a word bdy */
		    *boffset = 0;	/* do it */
		    (*offset)++;	/* and move up */
		}
		s = *offset;		/* starts at offset */
		*offset += tsize(t->sptr); /* remember size */
	    }

	    /*
	    ** Now we have parsed the declarator, and the encoded
	    ** offset is in s.  Make a new symbol, or make sure that
	    ** this is the same as the old one.
	    */

	    u = findsym(t->sname);	/* look up member name */
	    if (u == NULL) {		/* struct member not seen before */
		t->sclass = SMEMBER;	/* remember that it is a struct mem */
		t->svalue = s;		/* with the given offset */
		u = creatsym(t->sname);	/* make a permanent location */
		copysym(u, t);		/* store it */
	    } else if (s != u->svalue || u->sptr != t->sptr) {
		u->svalue = AMBIGMEM;	/* deglobalize */
	    }
	    *smem = &smems[maxsmem++];	/* get new struct mem pointer */
	    (*smem)->smem = u;		/* remember the pointer */
	    (*smem)->smnext = NULL;	/* no pointer */
	    (*smem)->smoffset = s;	/* keep offset locally */
	    (*smem)->smtype = t->sptr;	/* keep type locally */
	    smem = &(*smem)->smnext;	/* but that is where next one goes */
	}
	if (token != COMMA) break;
	nextoken();
    }
    expect(SCOLON);
    return smem;			/* return with latest pointer */
}

/* ----------------------------- */
/*      parse bitfield size      */
/* ----------------------------- */

static int
fldsize(offset, boffset)
int *offset, *boffset;
{
    int s;

    s = pconst();			/* get num bits */
    if (s > 36 || s < 0) error(EBFSIZE); /* range check */
    *boffset += s;			/* advance by that many bits */
    if (*boffset > 36) {		/* not enough room */
	*boffset = s;			/* move bit offset to word bdy */
	(*offset)++;			/* in next word */
    }
    return s;				/* return with size */
}

/* ------------------------- */
/*      enumerated type      */
/* ------------------------- */

static struct TY *
enumspec()
{
    int val;
    struct SYMBOL *s;

    if (token == IDENT) {		/* ignore enum name */
	if (csymbol->sclass == SUNDEF) freesym(csymbol);
	nextoken();
    }
    if (token == LBRACE) {		/* is this definition? */
	val = 0;			/* start at zero */
	do {
	    nextoken();			/* skip comma or brace */
	    s = csymbol;		/* get identifier */
	    expect (IDENT);		/* that better be what it is */
	    if (token == ASGN) {
		nextoken();		/* want specific value for this one */
		val = pconst();		/* so set it to given constant */
	    }
	    if (s->sclass != SUNDEF) error(EDSYMB, s->sname);
	    s->sclass = SENUM;		/* this is an enum */
	    s->svalue = val++;		/* with this value */
	    s->sptr = deftype;		/* acting like an int */
	} while (token == COMMA);
	expect(RBRACE);			/* done with def, end with right */
    }
    return deftype;			/* treat as integer */
}

/* -------------------------------------------- */
/*	function-definition  Ref[1] A.18.4      */
/* -------------------------------------------- */

static void
funcdef (s,args)
struct SYMBOL *s, *args;
{
    int n, siz;
    struct TY *t;
    node nnode, header;
    struct SYMBOL *s1, *s2, stemp;

    s1 = findsym(s->sname);
    if (s->sclass == XTERN) {
	s->sclass = SEXTERN;
	s->svalue = 1;
    }

    /* remember context for error messages */
    curfn = s1;
    curfnloc = curfnnew;

    /* check for multiple decl */
    if (s1->sclass == SUNDEF) ;
    else if (s1->sclass != SEXTERN || s1->svalue) error (EDSYMB, s->sname);
    else if (tmismatch (s1->sptr, s->sptr)) warn (ETWOTYP, s->sname);

    copysym (s1, s);			/* fix up symbol to fit new decl */
    siz = tsize (s1->sptr->tptr);	/* get size of return val for later */

    header = defnode(N1, 0);
    header->left = defnode(N2, IDENT, s1->sptr, 0, s1);
    nnode = defnode(N2, FUNCTION, NULL, 0, header);

    /* parse declarations of arguments */
    while (token != LBRACE && token != EOF) {		/* type-decl-list */
	if (token == REGISTER) nextoken(); /* Ignore REGISTER on args */
	t = typespec();
	while (1) {
	    stemp.sname[0] = '\0';	/* no symbol given yet */
	    stemp.sptr = t;		/* base is parsed type name */
	    s1 = declarator(&stemp, 2);	/* look up type structure and sym */
	    if ((s2 = findsym(s1->sname)) == NULL || s2->sclass != SARG)
		error(ENOTARG, s1->sname); /* not an arg to this fn */
	    else s2->sptr = s1->sptr;	/* is, set type to what we parsed */
	    if (token != COMMA) break;	/* repeat through list */
	    nextoken();			/* skipping over commas */
	}
	expect(SCOLON);			/* decl line ends with a semicolon */
    }

    /* now that types are set, make sizes of local vars */
    n = 0;				/* set up for first arg */
    if (siz > 2) n = siz;		/* allow space for struct return */
    while (args != NULL) {
	s1 = args;			/* get arg symbol */
	args = (struct SYMBOL *) args->svalue; /* move on */
	n += tsize(s1->sptr);		/* count off by size */
	s1->svalue = n;			/* set offset */
    }

    maxauto = 0;			/* no local variables yet */
    stackrefs = 0;			/* and therefore no refs to them */
    scope = SAUTO;			/* function-statement */
    nnode->right = defnode(N3, STATEMENT, NULL, 0,
			   defnode(N1, RETURN), statement()); /* parse body */
    gencode(nnode);			/* emit code for function */
    ridauto();				/* flush local variables */
}

/* ----------------------------------------- */
/*	sc-type, type-specifier, symbol      */
/*	      Ref[1] A.18.4		     */
/* ----------------------------------------- */

static void
datadef (s, base)
struct SYMBOL *s, *base;
{
    struct SYMBOL t;
    node z;

    z = NULL;
    while (1) {
	z = defnode(N3, DATA, NULL, 0, z, izer(s));
	if (token != COMMA) break;
	nextoken();
	copysym(&t, base);
	s = declarator(&t, 0);
    }
    if (!expect(SCOLON) && token == RBRACE) nextoken();
    gencode(z);
}

/* ----------------------- */
/*	initializer        */
/*      Ref.[1] A.8.6      */
/* ----------------------- */

static node
izer (s)
struct SYMBOL *s;
{
    struct SYMBOL *t;
    struct TY nt, *tp;
    node y, z;
    int cnt;
    char *c;

    /* hack null array */
    if (s->sclass == XTERN && s->sptr->ttype == ARRAY && s->sptr->tsize == 0 &&
	token != ASGN) s->sclass = SEXTERN; /* nothing here, pretend extern */

    /* look up ident in symbol table */
    t = findsym(c=s->sname);		/* look it up in symbol table */

    if (s->sptr->ttype == FUNCTION) {	/* extern or auto function def? */
	if (t->sclass == SUNDEF) {	/* seen this function before? */
	    freesym(t);			/* no, get rid of local cell */
	    t = creatsym(c);		/* create an entry in global table */
	    t->sclass = SEXTERN;	/* external unless we see body */
	    t->svalue = 0;		/* mark as a decl (not definition) */
	    t->sptr = s->sptr;		/* copy type definition we had */
	} else if (tmismatch (t->sptr, s->sptr)) warn (ETWOTYP, c);
	if (s->sclass == SSTATIC) t->sclass = SSTATIC; /* make static */
	return NULL;			/* function decl doesn't use storage */
    }

    /* real variable, do things depending on class */
    switch (s->sclass) {
    case SAUTO:				/* local to function */
	if (t->sclass == SUNDEF) freesym(t); /* release sym pointer */
	t = creatloc(c);		/* make cell known to be local */
	s->svalue = maxauto;		/* remember its stack offset */
	maxauto += tsize(s->sptr);	/* and count it in to frame size */
	if (token == ASGN && (s->sptr->ttype == ARRAY ||
			      s->sptr->ttype == STRUCT))
	    error(EBINIT);		/* say no good for auto */
	break;

    case SEXTERN:			/* extern, not defined here */
	s->svalue = 0;
    case STYPEDEF:			/* type definition */
	if (t->sclass == SUNDEF) {
	    freesym(t);			/* get rid of local symbol */
	    t = creatsym(c);		/* and make a global one */
	    s->svalue = 0;		/* marked as an extern */
	    copysym(t, s);		/* fill in rest of symbol */
	} else if (t->sclass != s->sclass) error (EDSYMB, c);
	else if (tmismatch (t->sptr, s->sptr)) warn (ETWOTYP, c);
	return NULL;			/* no initialization or storage */

    case XTERN:				/* extern, defined here */
	s->sclass = SEXTERN;		/* make extern for rest of file */
	s->svalue = 1;			/* mark as INTERN if memory */
	if (t->sclass == SEXTERN && t->svalue == 0) {
	    if (tmismatch (t->sptr, s->sptr)) warn (ETWOTYP, c);
	    t->sclass = SUNDEF;
	}
    case SSTATIC:			/* extern, not visible as global */
	if (scope == SAUTO) {		/* "own" variable? */
	    if (t->sclass == SUNDEF) freesym(t); /* yes, flush glob sym */
	    t = creatloc(c);		/* and make local cell */
	    s->sclass = SISTATIC;	/* internal static */
	    s->sval.ssym = newlabel (1); /* create handle on object */
	    break;
	} else if (t->sclass == SUNDEF) {
	    freesym(t);			/* no, get rid of local symbol */
	    t = creatsym(c);		/* and make a global one */
	} else error (EDSYMB, c);
    }

    copysym(t, s);			/* fill out the symbol */

    /* parse initializer */
    if (token == ASGN) {
	nextoken();			/* skip equal sign */
	cnt = 0;			/* 0 initializers seen */
	z = pizer(&cnt);		/* read initializer */
    } else z = NULL;

    y = defnode(N2, IDENT, t->sptr, 0, t); /* make a node giving its name */
    return defnode(N3, IZ, NULL, 0, y, z); /* and what to fill it with */
}

/* --------------------------- */
/*      parse initializer      */
/* --------------------------- */

static node
pizer(cnt)
int *cnt;
{
    node root, n;

    if (token != LBRACE) {
	return expression(); /* not list, simple init */
    }
    nextoken();				/* skip open brace */
    root = n = defnode(N3, IZLIST, NULL, 0, pizer(0), NULL);
    if (cnt)
	(*cnt)++;
    while (token == COMMA) {
	nextoken();			/* skip comma */
	if (token == RBRACE) break;	/* allow comma at end of initializer */
	n = n->right = defnode(N3, IZLIST, NULL, 0, pizer(0), NULL);
	if (cnt)
	    (*cnt)++;
    }
    expect(RBRACE);			/* finish with close brace */
    return root;			/* return whole thing */
}

/* -------------------------------------- */
/*	declaration  Ref.[1]  A.18.2      */
/* -------------------------------------- */

static void
decln ()
{
  struct SYMBOL s;
  s.sclass = pscope();
  s.sptr = typespec();
  s.sname[0] = 0;
  idecllist(&s);
  expect(SCOLON);
}

/* ------------------------------------------- */
/*      check two types for compatibility      */
/* ------------------------------------------- */

static int
tmismatch (t1, t2)
struct TY *t1, *t2;
{
    if (t1 == t2) return 0;
    if (t1->ttype != ARRAY || t2->ttype != ARRAY) return 1;
    if (t1->tptr != t2->tptr) return 1;
    return (t1->tsize != 0 && t2->tsize != 0);
}

/* --------------------------------------- */
/*      parse storage class specifier      */
/* --------------------------------------- */

static int
pscope()
{
    switch (token) {
    case AUTO:
	nextoken();
	return scope;
    case STATIC:
	nextoken();
	return SSTATIC;
    case EXTERN:
	nextoken();
	return SEXTERN;
    case REGISTER:
	nextoken();
	return scope;			/* ignore register decl */
    case TYPEDEF:
	nextoken();
	return STYPEDEF;
    }
    return scope;			/* no storage class, return default */
}

/* ----------------------------------------------- */
/*	init-declarator-list  Ref.[1]  A.18.2      */
/* ----------------------------------------------- */

static void
idecllist(base)
struct SYMBOL *base;
{
    struct SYMBOL t;
    node z;
    int savemax;
  
    if (scope == SAUTO && base->sclass != SSTATIC) z = decls;
    else z = NULL;
    while (1) {
	copysym(&t, base);
	z = defnode(N3, DATA, NULL, 0, z, izer(declarator(&t, 0)));
	if (token != COMMA) break;
	nextoken();
    }
    if (scope == SAUTO && base->sclass != SSTATIC) decls = z;
    else {
	savemax = maxnode;
	gencode(z);
	maxnode = savemax;
    }
}
