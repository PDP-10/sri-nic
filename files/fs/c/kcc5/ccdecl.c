/*	CCDECL.C - Declaration Parser
**
**	All changes after version 154 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCDECL.C.290,  5-Mar-86 19:57:00, Edit by KLH */
/*  **MAJOR** overhaul and rewrite of just about everything.  Now
** conforms to H&S except for initializers (deferred work on pizer()
** for later).
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCDECL.C.162, 17-Dec-85 08:01:25, Edit by KLH */
/*  Rationalized names of constants and structures */
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
extern SYMBOL *findsym(), *makesym(), *uniqsym();	/* CCSYM */
extern SYMBOL *lsymhead;	/* CCSYM - var indicating loc sym blk */
extern SYMBOL *newlabel();	/* CCSYM */
extern SYMBOL *findsmem();	/* CCSYM */
extern int isdupsym();		/* CCSYM */
extern SYMBOL *beglsym();	/* CCSYM */
extern void endlsym(), funlsym();			/* CCSYM */
extern TYPE *findctype(), *findtype(), *findsztype();	/* CCSYM */
extern int sizetype();					/* CCSYM */
extern NODE *evalexpr();		/* CCFOLD */
extern NODE *funstmt(), *exprdecl(), *defnode();	/* From CCSTMT */
extern int pconst();			/* CCSTMT */
extern NODE *convasgn();		/* CCTYPE */
extern TYPE *convfparam();		/* CCTYPE */
extern int nextoken(), expect();
extern void warn(), error(), outid(), outnl();

/* functions herein exported to the outside */
void initpar();		/* Called by CC mainline */
void entdefs();		/* Ditto, someday this will be flushed */
NODE *extdef();		/* Called by CC mainline */
NODE *ldecllist();	/* Called by CCSTMT */
TYPE *typename();	/* Called by CCSTMT */
SYMBOL *defauto();	/* CCSTMT */

/* functions used entirely internally */
static NODE *funcdef(), *datadef();
static void paramlist(), funchk(), sdeclenum(), decllist();
static int sdecllist(), sdeclunion(), sdeclstruct(), fldsize(),
	tmismatch(), pstoclass(), isdecl(), pbase();
static NODE *dodecl();
static SYMBOL *declarator(), *decl0();
static TYPE *addpp(), *pushtype(), *pushsztype(), *tagspec();
static TYPE *typespec();
static SYMBOL *sdeclaration();

static NODE *pizer(), *npizer();
static int newpiz = 1;	/* Patch 0 to restore old pizer */

/* Internal data */

static int paramok;	/* True if OK to parse parameter list for a
			** function-type declaration.  Only true when at top
			** level and parsing the first declarator.
			*/
static SYMBOL *arglist;	/* At top level, points to list of any parameters
			** furnished with a function type declaration.
			*/
static NODE *statdecls,	/* Pointer to list of static decls within current fn */
	*stattail;	/* Pointer to tail of statdecls list */
static int nsdefs;	/* # of enum/struct/union side effect definitions seen.
			** The exact number doesn't matter; this is only used
			** to tell when a type-specifier has had the side
			** effect of defining a tag or enum constant.
			*/
static int itags;	/* # of internal tags defined.  This is used only
			** to create unique names for internal tags. 
			*/

/* Flags returned from pbase(). */
/* These aren't really used yet, maybe should be flushed. */
#define BASEF_SC 01	/* Storage class parsed */
#define BASEF_TS 02	/* Type-specifier parsed */
#define BASEF_TAG 04	/* enum/struct/union tag was defined as side effect */

/* ---------------------------------------- */
/* INITPAR - Initialize parser		    */
/*	Called once for each file compiled. */
/* ---------------------------------------- */
void
initpar()
{
    initnodes();		/* No parse-tree nodes used */
    curfn = NULL;		/* Not in any function yet */
    itags = 0;			/* Reset internal tag count for gensyms */
    nsdefs = 0;			/* Reset side-effect def count for neatness */
}

/* ----------------------------------------------------------
** ENTDEFS - Process "entry" statements at start of file
**	This is a hack routine in several respects, and should vanish
** someday after all library routines have been fixed to remove their
** existing "entry" statements.  The hacks, among other things, are that:
** (1) the char flag table is munged to allow otherwise invalid ident chars.
** (2) this does both parsing and generation and does not return a NODE *.
** (3) output to the assembler file is performed here (should not happen
**	until the code generation phase).  This is necessary to ensure that
**	the assembler ENTRY statements precede all other code.
*/

#include "ccchar.h"		/* To access setcsym and clrcsym */
void
entdefs()
{
    SYMBOL *s;

    /* Verify it's ok to do this stuff at all */
    if (!clevkcc) return;	/* Return if not using KCC extensions. */

    /* Check out first token to see if it's an entry statement. */
    if (token != Q_IDENT || strcmp(csymbol->Sname, "entry"))
	return;			/* 1st token not "entry", can just return. */

    /* Process possible ENTRY statement */
    setcsym('$');		/* Allow $ and % as identifier chars */
    setcsym('%');		/* (note '_' becomes '.', so needs no hack) */
    s = csymbol;		/* Save symbol */
    if(nextoken() != Q_IDENT) {	/* must be followed by ident */
	tokpush (Q_IDENT, s);	/* Nope, put "entry" back on stack */
	clrcsym('$');		/* No longer allow $ or % as ident chars */
	clrcsym('%');
	return;			/* And return */
    }
    freesym(s);		/* OK, can flush "entry" identifier sym */

    /* Process an ENTRY statement with at least one valid identifier arg */
    while (1) {
	outstr ("\tENTRY\t");		/* make ENTRY statement */
	outid (csymbol->Sname);		/* before we call expect() */
	outnl();			/* so #asm in middle won't screw us */
	expect (Q_IDENT);		/* it better have been a sym */
	if (token != T_COMMA) break;	/* repeat while we have more */
	nextoken();			/* skip COMMA */
    }
    clrcsym('$');		/* No longer allow $ or % as ident chars */
    clrcsym('%');
    expect(T_SCOLON);	/* Must end with ';', flush til get one */
			/* Note error recovery will not gobble improper
			** identifiers due to above clrcsym calls.
			*/
    /* Now invoke self again, until no more entry stmts.
    ** Tail recursion makes this efficient, though it hardly matters.
    */
    entdefs();
}

/* -------------------------------------- */
/* EXTDEF - Parse an external definition  */
/*	Main entry to input parser	  */
/* -------------------------------------- */

NODE *
extdef()
{
    SYMBOL *s, tempsym, base;
    TYPE *t;

    /* Do top level initializations */
    paramok = 1;		/* OK to parse a function parameter list */
    arglist = NULL;		/* No parameter list yet */

    curfnnew = fline;		/* Remember line in file where funct started */

    pbase(&base);		/* Parse base (storage class & type) */

    if (token == T_SCOLON) {	/* just the type (struct)? */
	nextoken();		/* yes, skip over final semi */
	if ((base.Sflags&SF_SIDEFF) == 0)	/* If no side effs, */
	    warn(EGEN,"Null declaration");	/* warn user */
	if (base.Sclass)	/* Useless to specify a storage class */
	    warn(EGEN,"Useless storage class");
	return NULL;		/* and stop doing this def */
    }

    copysym(&tempsym, &base);		/* Copy base storage class & type */
    if (tempsym.Sclass == SC_UNDEF)	/* Set up for defaults */
	tempsym.Sclass = SC_AEXTERN;	/* Default is assumed-extern */
    if (tempsym.Stype == NULL)
	tempsym.Stype = deftype;
    for(;;) {
	if (s = declarator(&tempsym))	/* Get decl sym if any */
	    break;
	if (arglist) ridlsym();		/* Just in case, flush. */
	warn(EGEN, "Null declarator");
	if (token == T_SCOLON) {
	    nextoken();
	    return NULL;
	}
	if (token == T_COMMA) {
	    nextoken();
	    continue;
	}
	error(EGEN,"Bad top-level declaration token");
	errflush();		/* Flush to probable end of statement */
	return NULL;		/* No identifier, so give up on it. */
    }

    paramok = 0;		/* No longer OK to parse function param list */

    /* If function type, need to examine more closely to see whether this
    ** is a definition or just a forward-reference declaration.
    ** It is a definition only if the current token is one of:
    **		Left brace (start of function body)
    **		A type-specifier keyword (a function parameter declaration).
    **		The "register" keyword (only permissible storage class for
    **			function parameter declarations).
    ** We permit any storage class here, for better error diagnostics later.
    */
    if (tempsym.Stype->Tspec == TS_FUNCT) {
	if (token == T_LBRACE || isdecl())
	    return funcdef(&base, &tempsym, s, arglist);  /* Parse funct def */
    }
    if (arglist) {
	error(EGEN,"Bad syntax - function parameters without body");
	ridlsym();		/* Flush them */
    }

    /* Not a function definition, so is either a function reference, or a
    ** data definition/reference.
    */
    return datadef(&base, &tempsym, s);	/* data def/ref or function ref */
}

/* -------------------------------------------- */
/*	function-definition  Ref[1] A.18.4      */
/* -------------------------------------------- */
/* Only called from extdef().  
** The symbol pointer argument always points to a temporary symbol
** structure not in the symbol table.  However, the argument list
** symbols ARE in the table, chained as local symbols.
*/

static NODE *
funcdef (b, d, syment, args)
SYMBOL *b, *d, *syment, *args;
{
    static int nsetjmps();
    int n, siz, nsjmps;
    TYPE *t;
    NODE *nnode, *header;
    SYMBOL *s1, argbase, stemp;

    /* Function name symtab entry is pointed to by syment.
    ** Lexer will have created the symtab entry with
    ** class SC_UNDEF if it didn't already exist.
    */
    curfn = syment;	/* remember context for error messages */
    curfnloc = curfnnew;

    d->Svalue = 1;		/* This is a defining declaration */
    funchk(b, d, syment); /* Check out storage class and type, fix up sym */

    /* parse declarations of arguments */
    funlsym();			/* Restore function parameter symbols, */
				/* and set up a local sym block for function */

    while (token != T_LBRACE && token != T_EOF) {	/* type-decl-list */

	pbase(&argbase);	/* Parse storage class and type specifier */
	copysym(&stemp, &argbase);
	switch (stemp.Sclass) {	/* Check storage class */
	    case SC_RAUTO:		/* If "register" seen, */
		stemp.Sclass = SC_RARG;	/* use right symbol class */
	    case SC_TYPEDEF:		/* Only other one OK is typedef. */
		break;
	    default:
		error(EGEN, "Illegal storage class for function parameter");
	    case SC_UNDEF:		/* Default becomes this. */
		stemp.Sclass = SC_ARG;
		break;
	}
	if ((t = stemp.Stype) == NULL) {	/* Check type-specifier */
	    if (argbase.Sclass == SC_UNDEF)
		error(EGEN,"No type-specifier for parameter decl, assuming int");
	    t = deftype;
	}
	while (1) {
	    stemp.Sname[0] = '\0';	/* no symbol given yet */
	    stemp.Stype = t;		/* Reset base type */
	    for(;;) {
		if (s1 = declarator(&stemp))	/* Get sym and rest of type */
		    break;
		if (token == T_COMMA) {
		    warn(EGEN, "Null parameter declarator");
		    nextoken();
		    continue;
		}
		if (token == T_SCOLON) {
		    nextoken();
		    break;
		}
		error(EGEN,"Bad parameter declaration token");
		errflush();	/* Flush to probable end of statement */
		break;		/* Will get two err msgs, but so what */
	    }

	    if (s1 == NULL)
		warn(EGEN, "Null parameter declaration");
	    else if (s1->Sclass == SC_TYPEDEF) {
		dodecl(&stemp, &argbase, s1);	/* Do the typedef */
	    } else if (s1->Sclass != SC_ARG) {
		error(EGEN, "Identifier \"%s\" not in function parameter list",
			s1->Sname);		/* not an arg to this fn */
		if (s1->Sclass == SC_UNDEF)
		    freesym(s1);		/* Clean up if boo-boo */
	    } else if (s1->Sflags & SF_PARAMDECL) {
		error(EGEN,"Duplicate parameter declaration: \"%s\"",s1->Sname);
	    } else {		/* Is arg, set type to what we parsed */
		s1->Sclass = stemp.Sclass;	/* Maybe indicate register */
		s1->Sflags |= SF_PARAMDECL;	/* Say decl seen for arg */
		s1->Stype = convfparam(stemp.Stype);	/* Get right type */
	    }
	    if (token != T_COMMA) break;	/* repeat through list */
	    nextoken();			/* skipping over commas */
	}
	expect(T_SCOLON);		/* decl line ends with a semicolon */
    }

    /* now that types are set, make sizes of local vars */
    n = 0;				/* set up for first arg */
    siz = sizetype(syment->Stype->Tsubt);	/* get size of return val */
    if (siz > 2)
#if 1
	n = 1;			/* Allow space for struct-return pointer */
#else
	n = siz;		/* allow space for struct return */
#endif
    while (args != NULL) {
	s1 = args;			/* get arg symbol */
	args = args->Sargp;		/* Move on before zapping it! */
	n += sizetype(s1->Stype);	/* count off by size */
	s1->Svalue = n;			/* set offset */
    }

    maxauto = 0;			/* no local variables yet */
    stackrefs = 0;			/* and therefore no refs to them */
    nsjmps = nsetjmps();		/* Remember # of setjmp refs */
    statdecls = stattail = NULL;	/* No static declarations yet */
    nnode = funstmt();			/* Parse function statement */
    endlsym((SYMBOL *)NULL);		/* End the local sym blk for params */
    nextoken();		/* Now safe to flush the right brace
			** and set up new current token.
			** See CCSTMT's compound() for discussion of this.
			*/
    stkgoto = (nsjmps != nsetjmps());	/* Say whether any setjmps in funct */
    header = defnode(N1, N_NODE);	/* Put together the function header */
    header->Nright = statdecls;		/* Point to any static decls found */
    header->Nleft = defnode(N2, Q_IDENT, syment->Stype, 0, (NODE *)NULL);
    header->Nleft->Nid = syment;

    /* Return completed parse tree */
    return defnode(N3, N_FUNCTION, (TYPE *)NULL, 0, header, nnode);
}

/* NSETJMPS - Auxiliary to find current # of references to the "setjmp"
**	function.  Any functions which contain calls to setjmp have to
**	avoid using tail recursion.
*/
static int
nsetjmps()
{
    SYMBOL *s;
    if (s = findsym("setjmp")) {
	--(s->Srefs);			/* Not a real reference */
	if ((s->Sclass == SC_AEXTERN || s->Sclass == SC_EXTERN)
	 && (s->Stype->Tspec == TS_FUNCT))
	    return s->Srefs;
    }
    return 0;
}

/* FUNCHK - Check out a function definition or reference for
**	proper use of storage class and type specifier.
** Called from funcdef() for a definition and dodecl() for a reference.
** d->Svalue will be 1 for a definition and 0 for a reference.
** Returns with the symbol table entry completely set up.
*/
static void
funchk(base, d, s)
SYMBOL *base,	/* Parsed base (storage class and type specifier) */
	*d,	/* Parsed declarator (Sclass and Stype) */
	*s;	/* Symtab entry for parsed identifier */
{
    
    switch (base->Sclass) {	/* Not all storage classes are allowed */
	case SC_UNDEF:		/* Default is OK, make it assumed-extern */
	    d->Sclass = SC_AEXTERN;
	    if (s->Sclass == SC_EXTERN) d->Sclass = SC_EXTERN;
	    break;
	case SC_EXTERN:		/* Explicit "extern" and "static" also OK */
	case SC_STATIC:
	    if (s->Sclass == SC_AEXTERN) s->Sclass = d->Sclass;
	    break;
	default:
	    error (EGEN,"Illegal storage class for function");
	    d->Sclass = SC_STATIC;	/* Assume static and carry on */
	    break;
    }

    if (s->Sclass == SC_UNDEF) {	/* Symbol not defined yet? */
	s->Sclass = d->Sclass;	/* No, copy the parsed class */
	s->Stype = d->Stype;	/* and the type specification */
	s->Svalue = d->Svalue;	/* mark as a def or ref */
	s->Srefs = 0;		/* and reset usage count in case a ref. */
	return;
    }

    /* Symbol already defined, check for match or multiple declaration */

    /* Make sure basic type is also "function returning ..." */
    if (s->Stype->Tspec != TS_FUNCT) {
	error(ETWOTYP, s->Sname);	/* Complain */
	if (d->Svalue == 0) return;	/* If only a ref, ignore it */
	/* else def, go on to overlay the sym, hope nothing vital clobbered */
	s->Svalue = 0;		/* Pretend existing sym is just a ref */
    } else {
	if (s->Sclass != d->Sclass) {		/* Verify storage class OK */
	    warn (EGEN, "Storage class conflict for \"%s\"; assuming static",
		s->Sname);
	    d->Sclass = SC_STATIC;
	}
	if (tmismatch (s->Stype, d->Stype))	/* Verify type OK */
	    warn (ETWOTYP, s->Sname);
	if (d->Svalue && s->Svalue)		/* Check for duplicate defs */
	    error (EDSYMB, s->Sname);
    }

    /* Force the symbol table entry to match current declaration.
    ** Note that Srefs was incremented by the symbol lookup, hence needs
    ** to be set back if this was only a ref-type declaration.
    */
    s->Sclass = d->Sclass;	/* Force the storage class */
    s->Stype = d->Stype;	/* and the type specification */
    if (d->Svalue)
	s->Svalue = d->Svalue;	/* Force definition if defining */
    if (s->Svalue == 0)		/* and if not yet defined, */
	--(s->Srefs);		/* decrement usage count to undo lookup bump */
}

/* ----------------------------------------- */
/*	sc-type, type-specifier, symbol      */
/*	      Ref[1] A.18.4		     */
/* ----------------------------------------- */
/* Only called from extdef().
** This parses a top-level data declaration or function reference.
*/
static NODE *
datadef (base, s, syment)
SYMBOL *base, *s, *syment;
{
    SYMBOL defbase;
    NODE *root = NULL, *tail = NULL;

    /* Check out the storage class and type
    ** specifications.  At top level, some storage class or type specifier
    ** must have been given.
    */
    defbase.Scontents = base->Scontents;	/* Copy contents of base */
    switch (defbase.Sclass) {
	case SC_UNDEF:
	    defbase.Sclass = s->Sclass;		/* Copy whatever default was */
	    if (defbase.Stype == NULL) {
		error(EGEN,"Declaration without storage class or type-spec");
	    }
	    break;		
	case SC_EXTERN:		/* One of these three is okay */
	case SC_STATIC:
	case SC_TYPEDEF:
	    break;
	default:
	    error(EGEN,"Illegal top-level storage class");
	    s->Sclass = defbase.Sclass = SC_AEXTERN;	/* Use default */
	    break;
    }
    if (defbase.Stype == NULL)
	defbase.Stype = s->Stype;	/* Copy whatever default was */

    decllist(base, &defbase, s, syment, &root, &tail);

    if (!expect(T_SCOLON) && token == T_RBRACE) nextoken();
    return (root);
}

/* ----------------------------------------- */
/*	declaration-list  Ref.[1] A.18.3     */
/* ----------------------------------------- */
/* Only called by CCSTMT's compound() to parse the declarations
** at the start of a compound statement.  Returns a node pointer
** to a list of initializations that must be done.
*/

NODE *
ldecllist()
{
    SYMBOL base, defbase;
    NODE *autodecls, *autotail;	/* Pointers to list of inits for decls
				** within a block */

    if (!isdecl()) return NULL;		/* Most common case -- no decls */

    autodecls = autotail = NULL;
    do {
	/* If current token is start of a declaration, handle it. */
	pbase(&base);		/* Parse base storage-class and type */
				/* Note all classes are OK */
	copysym(&defbase, &base);
	if (defbase.Sclass == SC_UNDEF)
	    defbase.Sclass = SC_AUTO;	/* Default class is AUTO */
	if (defbase.Stype == NULL)
	    defbase.Stype = deftype;

	/* Handle the local declaration, adding defs to the right list. */
	if (defbase.Sclass == SC_STATIC)
	     decllist(&base, &defbase, (SYMBOL *)NULL, (SYMBOL *)NULL,
			&statdecls, &stattail);
	else decllist(&base, &defbase, (SYMBOL *)NULL, (SYMBOL *)NULL,
			&autodecls, &autotail);

	expect(T_SCOLON);
    } while (isdecl());

    return(autodecls);
}

/* -------------------------------------------- */
/*	DECLLIST - parse a list of declarators	*/
/* -------------------------------------------- */
static void
decllist(base, defbase, d, s, root, tail)
SYMBOL *base,		/* Base storage class & type-spec furnished */
	*defbase,	/* Same but completely defaulted as necessary */
	*d,		/* First parsed declarator (NULL if none) */
	*s;		/* Symbol table entry for d's ident */
NODE **root,		/* Addr of Root of declaration parse tree, if any */
	**tail;		/* Addr of Tail of parse tree, if any */
{
    SYMBOL tempsym;
    NODE *n, *z;
  
    if (d == NULL) {		/* Already parsed first declarator? */
	d = &tempsym;		/* No, so do first one here. */
	copysym(d, defbase);
	s = declarator(d);
	if (s == NULL && token == T_SCOLON && (base->Sflags&SF_SIDEFF))
	    return;		/* No declarators but have side-effect */
    }
    while (1) {
	if (s == NULL)
	    warn(EGEN, "Null declarator");
	else if ((n = dodecl(d, base, s)) != NULL) {
	    z = defnode(N2, N_DATA, (TYPE *)NULL, 0, n);
	    if (*tail == NULL)		/* Add parse result onto tree */
		*root = *tail = z;		/* Either as 1st node */
	    else *tail = (*tail)->Nright = z;	/* or at end of current tree */
	}
	if (token != T_COMMA) break;
	nextoken();
	copysym(d, defbase);
	s = declarator(d);
    }
    return;
}

/* ------------------------------------------------------------ */
/*	PBASE - parse base of declaration:			*/
/*		{storage-class-specifier} {type-specifier}	*/
/* ------------------------------------------------------------ */

static int
pbase(bsym)
SYMBOL *bsym;
{
    int savnsdefs = nsdefs;		/* Remember # side-eff defs so far */

    bsym->Sname[0] = '\0';		/* init symbol */
    bsym->Svalue = 0;			/* no val yet */

    bsym->Sclass = pstoclass();		/* parse storage class if any */
    bsym->Stype = typespec();		/* parse type if any */

    /* Set flag if any side effects (tag or enum defined) during parse */
    bsym->Sflags = (savnsdefs == nsdefs ? 0 : SF_SIDEFF);
}

/* ISDECL - returns true if current token is either a
**	storage-class-specifier or a type-specifier,
**	i.e. token is the start of a declaration.
*/
static int
isdecl()
{
    return (csymbol != NULL && (
	(tok[token].tktype == TKTY_RWSC || tok[token].tktype == TKTY_RWTYPE)
	|| (csymbol->Sclass == SC_TYPEDEF)
	));
}

/* --------------------------------------- */
/*      parse storage class specifier      */
/* --------------------------------------- */

static int
pstoclass()
{
    switch (token) {
    case T_AUTO:
	nextoken();
	return SC_AUTO;
    case T_STATIC:
	nextoken();
	return SC_STATIC;
    case T_EXTERN:
	nextoken();
	return SC_EXTERN;
    case T_REGISTER:
	nextoken();
	return SC_RAUTO;		/* Assume "auto" */
    case T_TYPEDEF:
	nextoken();
	return SC_TYPEDEF;
    }
    return SC_UNDEF;			/* no storage class, return nothing */
}

/* --------------------------------------- */
/*	type-specifier  Ref[1] A.18.2      */
/* --------------------------------------- */
static TYPE *findbtype();	/* Auxiliary for TYPESPEC */

static TYPE *
typespec()
{
    TYPE *t;
    int sign;

    if (tok[token].tktype == TKTY_RWTYPE) {
	/* Check out a reserved-word type specifier */
	switch (token) {
	    case T_CONST:		/* "const" */
		if (sign = TF_CONST) ;	/* Hack to help share code */
		else {
	    case T_VOLATILE:		/* "volatile" */
		    sign = TF_VOLATILE;
		}
		nextoken();
		if ((t = typespec()) == NULL) {	/* Then get real type. */
		    error(EGEN, "Type-specifier must follow \"%s\"",
				(sign==TF_CONST ? "const" : "volatile"));
		    return deftype;
		}
		return findctype(t->Tspec, sign|t->Tflag, t->Tsize, t->Tsubt);

	    case T_STRUCT: return tagspec(TS_STRUCT);
	    case T_UNION:  return tagspec(TS_UNION);

	    /* Enums are handled specially.  Until the code generation knows
	    ** about them, we always substitute "int" as the type for an enum.
	    */
	    case T_ENUM:   (void) tagspec(TS_ENUM);	/* Ignore return val */
			   return inttype;		/* and pretend int */

	    case T_VOID:	t = voidtype; break;
	    case T_FLOAT:	t = flttype; break;
	    case T_DOUBLE:	t = dbltype; break;
	    case T_CHAR:	t = chartype; break;
	    case T_INT:		t = inttype; break;

	    case T_SHORT:			/* "short" */
		if (nextoken() == T_INT)
		    nextoken();			/* "short int" */
		return shrttype;

	    case T_LONG:			/* "long" */
		if (nextoken() == T_INT)
		    nextoken();			/* "long int" */
		else if (token == T_FLOAT) {
		    nextoken();			/* "long float" */
		    return dbltype;
		} else if (token == T_DOUBLE) {	/* "long double" */
		    nextoken();
		    return lngdbltype;
		}
		return longtype;

	    /* Special KCC extended char types */
	    case T_CHAR6: t = findbtype(6, 0); break;
	    case T_CHAR7: t = findbtype(7, 0); break;
	    case T_CHAR8: t = findbtype(8, 0); break;
	    case T_CHAR9: t = findbtype(9, 0); break;
	    case T_CHAR18:t = findbtype(18,0); break;

	    case T_SIGNED:			/* "signed" */
		if (sign = 1) ;	/* silly crock to skip next stmt */
		else {
	    case T_UNSIGNED:			/* "unsigned" */
		    sign = 0;
		}
		if (tok[nextoken()].tktype != TKTY_RWTYPE)
		    return (sign ? inttype : uinttype);
		switch (token) {
		    case T_CHAR:		/* "un/signed char" */
			t = (sign ? schartype : uchartype);
			break;
		    case T_INT:			/* "un/signed int" */
			t = (sign ? inttype : uinttype);
			break;
		    case T_SHORT:		/* "un/signed short" */
			if (nextoken() == T_INT)
			    nextoken();		/* "un/signed short int"*/
			return (sign ? shrttype : ushrttype);
		    case T_LONG:		/* "un/signed long" */
			if (nextoken() == T_INT)
			    nextoken();		/* "un/signed long int" */
			return (sign ? longtype : ulongtype);
		    default:
			error(ETYPCOMB);	/* Default "un/signed" */
			return (sign ? inttype : uinttype);
		    /* Special KCC extended char types */
		    case T_CHAR6: t = findbtype(6, sign); break;
		    case T_CHAR7: t = findbtype(7, sign); break;
		    case T_CHAR8: t = findbtype(8, sign); break;
		    case T_CHAR9: t = findbtype(9, sign); break;
		    case T_CHAR18:t = findbtype(18,sign); break;
		}
		break;

	    default:
		error(EGEN,"Internal error - Illegal RWTYPE token");
		return deftype;
	}
	nextoken();			/* move to next token */
	return t;
    }

    /* Token wasn't a reserved-word type-specifier; check for typedef names. */
    if (csymbol && csymbol->Sclass == SC_TYPEDEF) {
	t = csymbol->Stype;	/* get the type */
	nextoken();		/* skip over it */
	return t;
    }

    return NULL;
}

/* Auxiliary for above routine to handle KCC extended char types */
static TYPE *
findbtype(bsiz, sign)
{
    return findctype((sign ? TS_CHAR : TS_UCHAR), bsiz, 1, (TYPE *)NULL);
}

/* ------------------------------------ */
/*	declarator  Ref[1]  A.18.2      */
/*					*/
/* ------------------------------------ */
/* The symbol pointer "s" always points to a temporary symbol structure
** which is NOT in the symbol table.
** All declaration statements eventually call this routine.
**	normal = extdef (toplevel) & decllist (toplevel & local)
**	smems = sdeclaration (decl within structure definition)
**	params = funcdef (declaration of function parameters)
**	abstract = typename (abstract declaration for casts & sizeof)
*/

static SYMBOL *
declarator (s)
SYMBOL *s;
{
    SYMBOL *syment;

    syment = decl0(s);
    if (s->Stype->Tspec == TS_VOID && (s->Sclass != SC_TYPEDEF))
	    error (EVOID, syment->Sname);
    return syment;
}

/* ---------------------------------------------------------- */
/*      parse a declarator's identifier and type information  */
/*	[H&S 4.5]					      */
/* ---------------------------------------------------------- */
/*
**	A DECLARATOR specifies the identifier being declared and may also
** supply additional type information.  The resulting type is one of:
**	( decl )	- parens used to establish precedence
**	ident		- Simple declarator
**	* decl		- Pointer to
**	decl [...]	- Array of
**	decl (...)	- Function (definition or reference)
**
** Only called by declarator() and itself recursively.
** The argument symbol pointer "s" must always point to a temporary symbol
** structure which is NOT in the symbol table.  The parsed type is
** returned in this symbol.
** If a non-null symbol pointer is returned, this is the parsed identifier
** which IS in the symbol table.
*/
static SYMBOL *
decl0 (s)
SYMBOL *s;
{
    TYPE *pp;		/* Holds "paren part" of type */
    SYMBOL *idsym;	/* Symtab ptr to parsed identifier */
    int saveidsc;	/* Temp while parsing function params */

    /* parse stars before ident part */
    while (token == Q_MPLY) {
	s->Stype = pushtype(TS_PTR, s->Stype);
	nextoken();
    }

    /* now parse main part (normally the ident) */
    switch (token) {
    case T_LPAREN:
	nextoken();			/* move over paren */
	{
	    TYPE *savt = s->Stype;	/* Save type thus far */
	    s->Stype = NULL;		/* and pretend it's null */
	    idsym = decl0(s);		/* Now parse stuff in parens */
	    pp = s->Stype;
	    s->Stype = savt;		/* Then can restore saved type */
	}
	expect(T_RPAREN);
	break;

    case Q_IDENT:
	idsym = csymbol;		/* Remember sym ptr for this ident */
	nextoken();
	pp = NULL;			/* No paren part */
	break;

    default:
	idsym = NULL;			/* No identifier */
	pp = NULL;			/* No paren part */
	break;
    }

    /* Check for function or array specifiers */
    while(1) {
	switch (token) {
	case T_LPAREN:		/* Function definition or reference */
	    /* Parse param list of function.  If idsym is a new symbol,
	    ** we set it temporarily so as to avoid bashing our global
	    ** function-name symbol if it turns out to have a parameter
	    ** name the same as the function name!
	    */
	    if (idsym && ((saveidsc = idsym->Sclass) == SC_UNDEF))
		idsym->Sclass = SC_AEXTERN;	/* Fake out paramlist */
	    paramlist();		/* Parse the parameter list if any */
	    if (idsym)			/* Restore real class after fakeout */
		idsym->Sclass = saveidsc;
	    pp = addpp(pp, pushtype(TS_FUNCT, (TYPE *)NULL));
	    break;

	case T_LBRACK:		/* Array of something */
	    nextoken();
	    pp = addpp(pp, pushsztype(TS_ARRAY,
				    (token == T_RBRACK)? 0 : pconst(),
				    (TYPE *)NULL));
	    expect(T_RBRACK);
	    break;
	    
	default:
	    /* go back and add paren part, return the result */
	    if (pp) s->Stype = addpp(pp, s->Stype);
	    return idsym;
	}
    }
}

/* PARAMLIST - Parse optional parameter list
**	Only called by decl0() to handle a function type declaration.
** Current token will be the left paren; on return, current token is
** first thing after the right paren.  (if error, current token is whatever
** halted the parse).
*/
static void
paramlist()
{
    SYMBOL *lastarg, *arg, *savehead;

    switch (nextoken()) {	/* Flush left paren, get next token */
	case Q_IDENT:
	    break;
	case T_RPAREN:
	    nextoken();
	    return;		/* No param list, nothing to do */

	default:
	    error(EGEN,"Bad syntax in function parameter list");
		/* Here, maybe should flush input until a ')', ';', or
		** '}' is seen.  Currently we pretend a ')' is seen.
		*/
	    return;
    }

    /* Parse parameter list.
    ** If not in proper context, we complain but parse the params anyway
    ** without remembering them.
    */
    if (!paramok)	/* Complain if wrong context for params */
	error(EGEN,"Bad context for function parameters");
    if (arglist) {
	error(EGEN,"Already parsed function parameters");
	paramok = 0;		/* Don't store any more params */
    }
    if (paramok) savehead = beglsym();	/* Begin a local symbol block */

    while (token == Q_IDENT) {		/* Token must be parameter */
	if (csymbol->Sclass == SC_ARG)	/* Already a parameter? */
	    error(EGEN,"Duplicate parameter: \"%s\"", csymbol->Sname);
	else if (paramok) {		/* If OK to remember param list, */
	    arg = uniqsym(csymbol);	/* make local sym for param */
	    if (arglist == NULL)	/* Save pointer to this param */
		arglist = arg;		/* either in head of list */
	    else lastarg->Sargp = arg;	/* or in last param so far */
	    lastarg = arg;		/* Move on to end of chain */
	    arg->Sargp = NULL;		/* This one is now the last */
	    arg->Sclass = SC_ARG;	/* Say it's a function parameter */
	    arg->Stype = deftype;	/* and (int) unless declared later */
	} else if (csymbol->Sclass == SC_UNDEF)
	    freesym(csymbol);		/* Not using sym, flush it if can. */

        nextoken();
        if (token != T_COMMA) break;
        nextoken();
    }
    if (paramok) endlsym(savehead);	/* End the local symbol block */

    expect(T_RPAREN);
}

/* --------------------------------------- */
/*      add type to inside of nesting      */
/* --------------------------------------- */

static TYPE *
addpp (pp, t)
TYPE *pp, *t;
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
	     pushsztype(pp->Tspec, pp->Tsize, addpp(pp->Tsubt, t)) :
	     t;
}

/* ------------------------------------------------------------------- */
/*	PUSHTYPE - Check for valid type combinations and return type   */
/* ------------------------------------------------------------------- */
/* As per [H&S 4.5.5] checks for the following illegal type
** combinations:
**	(1) Any type with "void" except "... function returning void"
**	(2) "Array of function ..."
**	(3) "Function returning array of ..."
**	(4) "Function returning function ..."
** There is also one other special case (5) that this list omits.
** As per [H&S 4.5.3 and 5.5.3] a N-dimensional array must always have
** all of the last N-1 sizes specified; only the first dimension's size
** can be omitted.
**	If the type combination is illegal, some plausible type is
** substituted, not so that something useful will be compiled but so
** that no bizarre "types" will unduly interfere with scanning the rest of the
** source file for further errors.
**	Note that "void" is OK all by itself as a type.  The checks here
** only look for illegal COMBINATIONS of types.
*/
static TYPE *
pushtype(typ, ptr)
TYPE *ptr;
{	return pushsztype(typ, typsiztab[typ], ptr);
}
static TYPE *
pushsztype(typ, siz, ptr)
TYPE *ptr;
{
    if (ptr != NULL) switch (typ) {	/* If making a combination, check it */
	default:		/* Check for (1) */
	    if (typ == TS_VOID) {	/* Nothing can be a subtype of void */
		error(ETYPCOMB);
		ptr = NULL;	/* Just make it "void" */
	    }
	    break;

	case TS_VOID:		/* Another check for (1) */
	    if (typ != TS_FUNCT) {	/* only functs can have void subtype */
		error(ETYPCOMB);
		ptr = inttype;	/* Substitute "int" for "void" */
	    }
	    break;

	case TS_ARRAY:		/* Check for (2) and (5) */
	    switch (ptr->Tspec) {
		case TS_ARRAY:		/* Array can have arrays, if... */
		    if (ptr->Tsize != 0)
			break;		/* array[][x] ok, array[x][] not */
		case TS_FUNCT:		/* Array cannot have functions */
		    error(ETYPCOMB);
		    ptr = inttype;	/* Lose, substitute "int" */
		}
		break;

	case TS_FUNCT:		/* Check for (3) and (4) */
	    switch (ptr->Tspec) {
		case TS_ARRAY:		/* Function cannot return array */
		case TS_FUNCT:		/* Function cannot return function */
		    error(ETYPCOMB);
		    ptr = voidtype;	/* Lose, substitute void */
	    }
    }

    /* now hash up the actual type and return the canonicalized version */
    return findsztype (typ, siz, ptr);
}

/* -------------------------------------------------- */
/* TAGSPEC - struct/union/enum tag handling */
/* -------------------------------------------------- */

static TYPE *
tagspec(typ)
int typ;		/* TS_STRUCT, TS_UNION, or TS_ENUM */
{
    SYMBOL s, *tagsym;
    TYPE *t;
    int siz;

    tagsym = NULL;
    switch (nextoken()) {
    case Q_IDENT:	/* Have tag ident */
	/* Get right sym for tagged entity by adding special prefix. */
	idpfcpy(s.Sname, SPC_TAG, csymbol->Sname);
	if (csymbol->Sclass == SC_UNDEF) freesym(csymbol);
	tagsym = findsym(s.Sname);
	if (nextoken() == T_LBRACE) nsdefs++;
	break;

    case T_LBRACE:
	/* No tag given, so we make up an internal one. */
	/* Note assumption here that symbol identifier is big enough */
	sprintf(s.Sname, "%c%d", SPC_TAG, ++itags);
	tagsym = NULL;
	break;

    default:
	error(EGEN,"struct/union/enum not followed by tag or definition");
	while (1)  switch (nextoken()) {
	    case T_EOF: case T_RBRACE: case T_SCOLON:
		return NULL;
	}
    }

    /* If a tag symbol already exists, check it to see whether we can use
    ** that one or need to create a new one.  This also checks for
    ** duplicate definitions.
    ** Note that if this is a reference (not a definition) then we always
    ** just use whatever tagsym is.
    */
    if (tagsym && token == T_LBRACE) {	/* If this will be a definition */
	if (tagsym->Sclass == SC_TAG) {	/* Tag already defined? */
	    if (isdupsym(tagsym))	/* Dup def? */
		error(EGEN, "Duplicate tag definition: \"%s\"", s.Sname + 1);
	    tagsym = NULL;		/* Force a new definition */
	} else if (tagsym->Sclass == SC_UTAG) {	/* A ref already exists? */
	    if (!isdupsym(tagsym))	/* If not in same block, */
		tagsym = NULL;		/* don't satisfy the ref! */
	}
    }
    if (tagsym && tagsym->Stype->Tspec != typ) {
	error(EGEN,"Tag redefined with different type: \"%s\"", s.Sname+1);
	tagsym = NULL;
    }

    /* If no tag (specified or internal) exists, make one and pretend it
    ** was seen as a forward reference (which it may well be).
    */
    if (tagsym == NULL) { 		/* Need tag symbol? */
	tagsym = makesym(s.Sname);	/* make a new tag symbol */
	tagsym->Sclass = SC_UTAG;	/* but with no defined body */
	tagsym->Ssmnext = NULL;		/* No members yet */
	tagsym->Srefs++;		/* This creation is a reference too */
					/* Create new type for it */
	tagsym->Stype = findtype(typ, (TYPE *)tagsym);
	tagsym->Stype->Tsmtag = tagsym;	/* Make sure type points to tag */
    }
    if (token != T_LBRACE)		/* If no definition, just return */
	return tagsym->Stype;

    /* Define the structure. */
    nextoken();				/* Flush the left brace */
    if (typ == TS_ENUM) sdeclenum(tagsym);
    else tagsym->Stype->Tsize =
	(typ == TS_STRUCT) ? sdeclstruct(tagsym) : sdeclunion(tagsym);

    tagsym->Sclass = SC_TAG;		/* Struct is now defined */
    return tagsym->Stype;
}

/* ------------------------- */
/*      enumeration type definition      */
/* ------------------------- */

static void
sdeclenum(tag)
SYMBOL *tag;
{
    int val;
    SYMBOL *s, *last;

    s = NULL;
    val = 0;			/* start at zero */
    last = tag;
    while (1) {
	if (token == T_RBRACE)	/* Allow ", }" without complaint */
	    break;
	if (token != Q_IDENT) {
	    error(EGEN,"Identifier expected as enum constant");
	    break;
	}
	nsdefs++;		/* Enum constant def as "side effect" */
	s = csymbol;		/* get identifier */
	if (isdupsym(s)) error (EDSYMB, s->Sname);
	s = uniqsym(s);		/* Create or re-use symbol as needed */
	s->Sclass = SC_ENUM;	/* this is an enum constant */
	s->Stype = inttype;	/* acting like an int */
	s->Ssmtag = tag;	/* Remember tag defined within */
	s->Ssmnext = NULL;
	last->Ssmnext = s;	/* Link onto list of enum members */

	if (nextoken() == Q_ASGN) {
	    nextoken();		/* want specific value for this one */
	    val = pconst();	/* so set it to given constant */
	}
	s->Svalue = val++;		/* with this value */
	if (token != T_COMMA)
	    break;
	nextoken();
    }
    expect(T_RBRACE);		/* done with def, end with right brace */
    if (s == NULL)
	warn(EGEN,"Empty enum definition list");
}


/*	Structure type definition */

static int
sdeclstruct(tag)
SYMBOL *tag;
{
    SYMBOL *lastmem;
    int offset, boffset, inbitf;

    lastmem = tag;
    offset = boffset = 0;		/* structure starts at zero */
    inbitf = 0;
    while (token != T_RBRACE) {
	if (eof) { error(EEOF,"in struct decl"); break; }
	lastmem = sdeclaration(tag, lastmem, &offset, &boffset, &inbitf);
    }
    nextoken();				/* skip over close brace */
    if (boffset > 0) offset++;		/* round offset out to full word */
    return offset;			/* return as size of struct */
}

/*      Union type definition      */
static int
sdeclunion(tag)
SYMBOL *tag;
{
    SYMBOL *lastmem;
    int maxsize, offset, boffset, inbitf;

    lastmem = tag;
    maxsize = 0;
    while (token != T_RBRACE) {
	if (eof) { error(EEOF,"in union decl"); break; }
	offset = boffset = 0;		/* each member starts at zero */
	inbitf = 0;
	lastmem = sdeclaration(tag, lastmem, &offset, &boffset, &inbitf);
	if (boffset > 0) offset++;	/* round out to full word */
	if (offset > maxsize) maxsize = offset;	/* find max with old size */
    }
    nextoken();				/* skip over close brace */
    return maxsize;			/* largest elt size is union size */
}

/* -------------------------------------------- */
/*	struct-declaration  Ref.[1]  A.8.5      */
/* -------------------------------------------- */

static SYMBOL *
sdeclaration (tag, prevsmem, offset, boffset, inbitf)
SYMBOL *tag, *prevsmem;
int *offset, *boffset, *inbitf;
{
    SYMBOL base, tempsym, *u;
    int offcode, bsiz, ts;

    pbase(&base);		/* Parse base storage-class and type */
    if (base.Stype == NULL) {
	error(EGEN,"No type-specifier for struct member, assuming int");
	base.Stype = deftype;
    }
    if (base.Sclass != SC_UNDEF)
	error(EGEN,"Storage class illegal for struct member");
    base.Sclass = SC_MEMBER;

    while (1) {
	if (token == T_COLON) {

	    /*
	    ** Colon without declarator before it - specifies space
	    ** left for alignment.  Constant expression following
	    ** colon is how much space, or zero to align to a word.
	    */
	    if (tag->Stype->Tspec == TS_UNION)
		error(EGEN,"Bit fields not allowed in unions");
	    nextoken();			/* skip over colon */
		/* If not on word boundary and previous member was not a
		** bitfield, force alignment.  There can be non-bitfield
		** objects smaller than a word (eg chars).
		*/
		if (*boffset && !(*inbitf)) {
		    *boffset = 0;	/* Force to word boundary */
		    (*offset)++;
		}
	    fldsize(pconst(), offset, boffset); /* parse & handle size */
	    (*inbitf)++;			/* Say in bitfield now */
	} else {

	    /*
	    ** Normal declarator.  Parse it, then check if there is
	    ** a colon expression after it making it a bit field, or
	    ** if it is a whole word expression.
	    **
	    ** For bitfields, the offset is encoded as follows:
	    **  offcode % 07777 - high 12 bits of byte pointer to the field
	    **  offcode >> 12   - word offset in struct
	    ** and then the whole thing is negated.
	    **
	    ** Note that we let the bit offset remain at 36 rather
	    ** than folding it to zero - the calculations are easier.
	    */

	    copysym(&tempsym, &base);
	    u = declarator(&tempsym);
	    if (u == NULL) {		/* Check for case of no identifier */
		warn(EGEN,"Null declarator");
		if (token == T_COLON) continue;	/* Pretend no declarator */
		if (token == T_COMMA) {
		    nextoken();
		    continue;
		}
		break;			/* Something bad, stop loop */
	    }

	    if (token == T_COLON) {

		/* Handle bitfield */
		if (tag->Stype->Tspec == TS_UNION)
		    error(EGEN,"Bit fields not allowed in unions");
		switch (tempsym.Stype->Tspec) {
		    case TS_INT:
			ts = TS_BITF;  break;
		    default:
			error(EGEN,"Bit field must be int or unsigned int");
		    case TS_UINT:		/* Above error drops thru */
			ts = TS_UBITF; break;
		}
		/* If not on word boundary and previous member was not a
		** bitfield, force alignment.  There can be non-bitfield
		** objects smaller than a word (eg chars).
		*/
		if (*boffset && !(*inbitf)) {
		    *boffset = 0;	/* Force to word boundary */
		    (*offset)++;
		}
		(*inbitf)++;			/* Say now in bitfield */

		nextoken();			/* move over colon */
		bsiz = pconst();		/* Parse size */
		offcode = fldsize(bsiz, offset, boffset);	/* Handle it */
		tempsym.Stype = findctype(ts,		/* Make bitfld type */
			bsiz | (tempsym.Stype->Tflag&(TF_CONST|TF_VOLATILE)),
			1, (TYPE *)NULL);
	    } else {			/* not bitfield */

		/* Handle normal non-bitfield member */

		/* Leave byte mode if necessary.
		** If new member is not a byte object, or if byte mode is
		** due to previous bitfield, always leave it, to force
		** word alignment.
		*/
		bsiz = tisscalar(tempsym.Stype) ?	/* Get object size */
			tbitsize(tempsym.Stype)		/* in bits if can */
			: TGSIZ_WORD;		/* else force wd mode */
		if (*boffset > 0		/* If in byte mode */
		  && ((bsiz >= TGSIZ_WORD)	/* and obj not byte */
		      || (*inbitf))) {		/* or prev obj was bitfield */
		    *boffset = 0;	/* Then force word alignment */
		    (*offset)++;	/* and move up */
		}
		*inbitf = 0;		/* Say no longer in bitfield */

		/* Now see if this type should be in byte mode or not.
		** Sizes smaller than a word will either enter or remain
		** in byte mode.  If the size is >= to a word, then the
		** above code will have already taken us out of byte mode.
		*/
		if (bsiz < TGSIZ_WORD) {
		    if (*boffset % bsiz)	/* Align to byte bndry */
			*boffset += bsiz - (*boffset % bsiz);
		    offcode = fldsize(bsiz, offset, boffset);

		} else {			/* One or more words */
		    offcode = *offset;		/* starts at offset */
		    *offset += sizetype(tempsym.Stype); /* remember size */
		}
	    }

	    /*
	    ** Now we have parsed the declarator, and the encoded
	    ** offset is in offcode.  Always make a new symbol for each
	    ** structure member.
	    */
	    idpfcpy(tempsym.Sname, SPC_SMEM, u->Sname);	/* Make smem ident */
	    if (u->Sclass == SC_UNDEF) freesym(u);	/* Clean up */
	    if ((u = findsmem(tempsym.Sname, tag)) != NULL) {
		error(EGEN,"Duplicate struct member declaration: \"%s\"",
			tempsym.Sname+1);
		/* If a dup, just ignore current declaration. */
	    } else {
		u = makesym(tempsym.Sname);	/* Make smem symbol entry */
		u->Sclass = SC_MEMBER;		/* Say it is a struct mem */
		u->Ssmoff = offcode;		/* with the given offset */
		u->Stype = tempsym.Stype;	/* Store type of member */
		u->Ssmtag = tag;		/* Point to parent structure */
		u->Ssmnext = NULL;		/* This is last mem so far */
		prevsmem->Ssmnext = u;	/* Point prev smem to this one */
		prevsmem = u;		/* This is new current smem */
	    }
	}
	if (token != T_COMMA) break;
	nextoken();
    }
    expect(T_SCOLON);
    return prevsmem;			/* return with latest pointer */
}

/* ----------------------------- */
/*      parse bitfield size      */
/* ----------------------------- */

static int
fldsize(bsiz, offset, boffset)
int bsiz, *offset, *boffset;
{
    if (bsiz > TGSIZ_WORD || bsiz < 0)	/* range check */
	error(EGEN, "Bit field longer than word (%d bits)", TGSIZ_WORD);
    if (bsiz == 0 && *boffset > 0)	/* Zero size means round to wd bdry */
	*boffset = TGSIZ_WORD+1;	/* Hack so roundup is forced */

    *boffset += bsiz;			/* advance by that many bits */
    if (*boffset > TGSIZ_WORD) {	/* If not enough room */
	*boffset = bsiz;		/* move bit offset to word bdy */
	(*offset)++;			/* in next word */
    }
    /* Return encoded offset */
    return -(((*offset * 64) + TGSIZ_WORD - *boffset) * 64 + bsiz);
}

/* --------------------------------------------- */
/*	Do processing for a parsed declarator    */
/*      Ref.[1] A.8.6				 */
/* --------------------------------------------- */
/* Called by decllist() to process the results of parsing a declarator.
** This IS called for:
**	toplevel declarations
**	local (at head of block) declarations
** This is NOT called for:
**	abstract declarators - handled by typename().
**	function parameter declarations - handled by funcdef().
**	structure/union declarations - handled by sdeclaration().
** This also checks for an initializer, and handles it if one
** exists.  Note both args refer to temporary symbol structures which are
** NOT in the symbol table.
*/

static NODE *
dodecl (d, base, s)
SYMBOL *d,	/* Declarator identifier, with sc and type all set up */
	*base,	/* Whatever was actually parsed as sc and type */
	*s;	/* Symbol table entry for the identifier */
{
    TYPE nt, *tp;
    NODE *y, *z;
    int cnt;
    char *id;

    /* Symbol table entry will always exist, because the
    ** lexer will have created it if necessary as a global symbol with
    ** symbol class SC_UNDEF.  If the new symbol actually should be a local one
    ** then it needs to be flushed and then created again in the right place.
    */

    /* Check for a function reference, which needs special handling */
    if (d->Stype->Tspec == TS_FUNCT
	&& d->Sclass != SC_TYPEDEF) {	/* A function reference? */
	d->Svalue = 0;		/* Indicate this is a reference */
	funchk(base, d, s);
	return NULL;		/* function decl doesn't use storage */
    }

    /* Real variable or typedef, do things depending on class */
    id = d->Sname;		/* Get convenient pointer to temp sym name */
    idcpy(id, s->Sname);	/* Copy identifier there from real symbol */

    switch (d->Sclass) {
    case SC_TYPEDEF:			/* type definition */
	if (isdupsym(s)) {	/* This symbol already def'd in same block? */
		error(EDSYMB, id);
		return NULL;
	}
	s = uniqsym(s);		/* Create or re-use symbol as needed */
	copysym(s, d);		/* fill in rest of symbol */
	return NULL;		/* no initialization or storage */

    case SC_AUTO:		/* local extent variable, in function */
    case SC_RAUTO:
	if (isdupsym(s))
	    error(EGEN,"Duplicate definition: \"%s\"", id);	/* Barf */
	s = uniqsym(s);			/* Always make local cell */
	d->Svalue = maxauto;		/* remember its stack offset */
	maxauto += sizetype(d->Stype);	/* and count it in to frame size */
	break;			/* Go copy symbol and check for init */

    case SC_EXTERN:		/* extern - assume not a definition */
	d->Svalue = 0;
	if (s->Sclass == SC_AEXTERN)
	    s->Sclass = SC_EXTERN;
	else if (s->Sclass == SC_UNDEF) {
	    copysym(s, d);		/* fill in rest of symbol */
	    s->Srefs = 0;		/* Reset # references */
	} else if (s->Sclass != d->Sclass) error (EDSYMB, id);
	else if (tmismatch (s->Stype, d->Stype)) warn (ETWOTYP, id);
	if (token == Q_ASGN) {
	    warn(EGEN, "extern shouldn't have initializer");
	    d->Svalue = 1;	/* Make it a definition instead */
	    break;		/* and go handle as for assumed-extern */
	}
	return NULL;		/* no initialization or storage */

    case SC_AEXTERN:		/* Assumed-extern, assume defined here. */
	d->Svalue = 1;			/* mark as definition, not ref */
	if ((s->Sclass == SC_EXTERN || s->Sclass == SC_AEXTERN)
	  && s->Svalue == 0) {
	    if (tmismatch (s->Stype, d->Stype)) warn (ETWOTYP, id);
	    s->Sclass = SC_UNDEF;
	}
	if (s->Sclass != SC_UNDEF)	/* Better not be defined yet */
	    error (EDSYMB, id);
	break;

    case SC_STATIC:		/* static, not visible as external */
	if (!lsymhead		/* Check for existing extern ref */
	  && s->Sclass == SC_EXTERN
	  && s->Svalue == 0) {
	    warn(EGEN,"Identifier \"%s\" was extern, changing to static", id);
	    if (tmismatch (s->Stype, d->Stype)) warn(ETWOTYP, id);
	    s->Sclass = SC_UNDEF;	/* Pretend not already declared */
	} else if (isdupsym(s))		/* Check for duplicate def */
	    error(EDSYMB, id);
	s = uniqsym(s);			/* Then always make unique cell */
	if (lsymhead) {		/* If within a local block */
	    d->Sclass = SC_ISTATIC;	/* internal static */
	    d->Ssym = newlabel();	/* create internal handle on object */
	}
	break;

    default:
	error(EGEN, "Internal error - illegal symbol class %d for %s",
	    d->Sclass, id);
	return NULL;
    }

    copysym(s, d);			/* fill out the symbol */

    /* Parse initializer.
    ** At this point the symbol is guaranteed to have one of these classes:
    **	SC_EXTERN, SC_AEXTERN, SC_STATIC, SC_ISTATIC, SC_AUTO, SC_RAUTO.
    */
    if (token == Q_ASGN) {
	nextoken();			/* skip equal sign */
	if (newpiz)
	    z = npizer(s);		/* New initializer parsing */
	else {	/* Old code */
	    cnt = 0;			/* 0 initializers seen */
	    z = pizer(&cnt);		/* read initializer */

	    /* If initializing an automatic variable, apply the 
	    ** usual assignment conversions.  Stopgap for now.
	    */
	    if (z && cnt==0 && (s->Sclass == SC_AUTO || s->Sclass == SC_RAUTO))
		z = convasgn(s->Stype, z);
	}
	if (z == NULL)
	    error(EGEN,"Null initializer for \"%s\"", s->Sname);
    } else {
	/* No initializer.  Check to make sure this is okay. */
	z = NULL;

	if (s->Sclass != SC_EXTERN	/* Unless an external ref */
	  && s->Stype->Tspec == TS_ARRAY	/* then if type is array */
	  && s->Stype->Tsize == 0) {		/* it must have a size. */
	    error(EGEN, "Missing size for definition of array \"%s\"", id);
	}
    }

    /* Make a node giving its name */
    y = defnode(N2, Q_IDENT, s->Stype, 0, (NODE *)NULL);
    y->Nid = s;
    return defnode(N3, N_IZ, (TYPE *)NULL, 0, y, z);	/* and its izer */
}

/* DEFAUTO - Define an automatic variable of the given type.
**	This routine is for the benefit of CCSTMT's function call
**	parsing, which sometimes needs to create temporary internal
**	variables to hold return values.
**	The id is assumed to be unique.
*/
SYMBOL *
defauto(id, typ)
char *id;
TYPE *typ;
{
    SYMBOL *s;
    s = makesym(id);		/* Make symbol in current scope */
    s->Sclass = SC_AUTO;
    s->Stype = typ;
    s->Svalue = maxauto;	/* Remember stack offset for it */
    maxauto += sizetype(typ);
    return s;
}

/* --------------------------- */
/*      parse initializer      */
/* --------------------------- */
/* Called by dodecl() and by itself recursively.
** NEEDS WORK!!  NEEDS LOTS OF WORK!!  Needs to check parsed expression
** result for legality in context, and apply the usual assignment
** conversions on the type.
*/

static NODE *
pizer(cnt)
int *cnt;
{
    NODE *root, *n;

    if (token != T_LBRACE) {
	return exprdecl(); /* not list, simple init.  Get expression. */
    }
    nextoken();				/* skip open brace */
    root = n = defnode(N3, N_IZLIST, (TYPE *)NULL, 0, pizer(0), (NODE *)NULL);
    if (cnt)
	(*cnt)++;
    while (token == T_COMMA) {
	nextoken();			/* skip comma */
	if (token == T_RBRACE) break;	/* allow comma at end of initializer */
	n = n->Nright = defnode(N3, N_IZLIST, (TYPE *)NULL, 0, pizer(0), (NODE *)NULL);
	if (cnt)
	    (*cnt)++;
    }
    expect(T_RBRACE);			/* finish with close brace */
    return root;			/* return whole thing */
}

/* NPIZER - Parse initializer.
**	Argument is pointer to symbol identifier being initialized.
**	Current token should be first thing after the '='.
*/
#if 1
static NODE *npizer();
static NODE *piztype(), *chkinteg(), *pizstruct(), *pizarray(),
	*pexizer(), *pizlist();
static void pizflush();
static int isauto(), nisconst();
			/* These are set by npizer and refed by its subrs */
static int izautof;	/* True if symbol being initialized is automatic */
static SYMBOL *izsym;	/* Symbol for var being initialized */

static NODE *
npizer(s)
SYMBOL *s;
{
    NODE *n;
    int saveopt = optpar;	/* Save value of parser optimization flag */

    izautof = isauto(izsym = s);	/* Set "globals" for subroutines */
    if (!izautof)		/* If we'll want a constant expression, */
	optpar = 1;		/* parse with full optimization. */

    n = piztype(s->Stype, 0);	/* Do outermost-level parse for this type */
    optpar = saveopt;		/* Restore original optimization flag */
    return n;
}

/* PIZTYPE - Parse initializer for a given type.
**	Recursive; makes use of izautof/izsym globals.
**	Returns what was parsed even if encountered error, mainly so
**	debugging can dump out the parse tree.
*/
static NODE *
piztype(t,lev)
TYPE *t;
int lev;			/* Level being parsed.  0 - outermost */
{
    NODE *e, *n;

    switch (t->Tspec) {
	case TS_BITF:	case TS_UBITF:
	case TS_CHAR:	case TS_UCHAR:
	case TS_SHORT:	case TS_USHORT:
	case TS_INT:	case TS_UINT:
	case TS_LONG:	case TS_ULONG:
	    /* Parse a single expression */
	    if ((e = pexizer(lev)) == NULL)
		break;
	    if (!tisarith(e->Ntype)) {
		error(EGEN,"Initializer must be of arithmetic type");
		break;
	    }
	    return chkinteg(t, e);

	case TS_FLOAT:	case TS_DOUBLE: case TS_LNGDBL:
	    /* Parse a single expression */
	    if ((e = pexizer(lev)) == NULL)
		break;
	    if (!tisarith(e->Ntype)) {
		error(EGEN,"Initializer must be of arithmetic type");
		break;
	    }
	    if ((n = convasgn(t, e)) != e)	/* Apply assignment convs */
		e = optpar ? evalexpr(n) : n;	/* and optimize if needed */

	    if (!izautof && e->Nop != N_FCONST) {
		error(EGEN,"Floating-point constant required as initializer");
		break;
	    }
	    break;		/* OK, return e */

	/* Enums are treated as integers, thus any integer type is allowed */
	case TS_ENUM:
	    /* Parse a single expression */
	    if ((e = pexizer(lev)) == NULL)
		break;
	    if (!tisinteg(e->Ntype)) {
		error(EGEN,"Initializer must be of integral type");
		break;
	    }
	    if ((n = convasgn(t, e)) != e)	/* Apply assignment convs */
		e = optpar ? evalexpr(n) : n;	/* and optimize if needed */

	    if (!izautof && e->Nop != N_ICONST) {
		error(EGEN,"Enum or integer constant required as initializer");
		break;
	    }
	    break;		/* OK, return e */


	case TS_PTR:
	    /* Parse a single expression */
	    if ((e = pexizer(lev)) == NULL)
		break;
	    if ((n = convasgn(t, e)) != e)	/* Apply assignment convs */
		e = optpar ? evalexpr(n) : n;	/* and optimize if needed */
	    if (t != e->Ntype) {		/* Types must match */
		error(EGEN,"Pointer initializer has wrong type");
		break;
	    }

	    /* A constant expression for a pointer requires hairy checks. */
	    if (!izautof		/* If must be a constant expr */
	      && !nisconst(e)) {	/* then check it out */
		error(EGEN,"Pointer initializer not constant");
		break;
	    }
	    break;		/* OK, return e */

	case TS_ARRAY:
	    return pizarray(t, lev);

	case TS_STRUCT:
	    return pizstruct(t, lev);

	case TS_UNION:		/* Forget about unions for now */
	    error(EGEN,"Initialization of unions is not permitted");
	    return pizlist();	/* Parse it anyway for debugging */

	case TS_VOID:
	    error(EGEN, "Cannot initialize void type");
	    return pizlist();	/* Parse it anyway for debugging */

	case TS_FUNCT:
	    error(EGEN, "Cannot initialize function type");
	    return pizlist();	/* Parse it anyway for debugging */

	default:
	    error(EINT, "Unknown Tspec = %d", t->Tspec);
	    return NULL;
    }
    return e;
}

/* CHKINTEG - Check initializer for integral type.
**	This is a subroutine for code sharing purposes (see pizarray).
*/
static NODE *
chkinteg(t, e)
TYPE *t;
NODE *e;
{
    NODE *n;

    if ((n = convasgn(t, e)) != e)	/* Apply assignment convs */
	e = optpar ? evalexpr(n) : n;	/* and optimize if needed */

    if (!izautof && e->Nop != N_ICONST)
	error(EGEN,"Integer constant required as initializer");
    return e;
}

/* PIZSTRUCT - Parse initializer for a structure.
**	Returns parsed stuff even if error, for debugging.
*/
static NODE *
pizstruct(t, lev)
TYPE *t;
int lev;
{
    register SYMBOL *smem;
    NODE *e, *n, *root;
    int braces;

    if (izautof)			/* Complain now */
	error(EGEN,"Initialization of automatic structures is not allowed");

    braces = (token == T_LBRACE);	/* Remember if have braces */
    if (!braces && lev == 0) {		/* If top level, must have them */
	error(EGEN,"Structure initializer must be enclosed in braces");
	return pizlist();		/* Flush entire izer */
    }
    if ((smem = t->Tsmtag->Ssmnext) == NULL) {
	error(EGEN,"Attempting to initialize an undefined structure");
	return pizlist();		/* Flush entire izer */
    }

    /* Now loop through structure members, reading an initializer for each */
    if (braces) nextoken();			/* Skip over left-brace */
    if (smem->Sclass != SC_MEMBER)		/* Paranoia check */
	error(EINT,"bad smem class");
    root = n = defnode(N3, N_IZLIST, (TYPE *)NULL, 0,	/* Parse 1st member */
		piztype(smem->Stype, 1), (NODE *)NULL);

    if (!braces) {
	while (smem = smem->Ssmnext) {
	    if (token != T_COMMA) {
		error(EGEN,"Too few elements in unbraced struct initializer list");
		return root;
	    }
	    nextoken();				/* Skip comma */
	    if (smem->Sclass != SC_MEMBER)	/* Paranoia check */
		error(EINT,"bad smem class");
	    n = n->Nright = defnode(N3, N_IZLIST, (TYPE *)NULL, 0,
		piztype(smem->Stype, 1), (NODE *)NULL);	/* Parse element */
	}
	/* When loop ends, fall thru to successful return */

    } else {	/* Have braces, life is more complicated */
	while (token == T_COMMA) {
	    if (nextoken() == T_RBRACE) break;	/* Skip comma, allow ",}" */
	    if ((smem = smem->Ssmnext) == NULL) {
		error(EGEN,"Too many components in struct initializer list");
		pizflush(1);			/* Flush rest of list */
		if (token == T_RBRACE) nextoken();
		return root;
	    }
	    if (smem->Sclass != SC_MEMBER)	/* Paranoia check */
		error(EINT,"bad smem class");
	    n = n->Nright = defnode(N3, N_IZLIST, (TYPE *)NULL, 0,
		piztype(smem->Stype, 1), (NODE *)NULL);	/* Parse element */
	}
	/* Check for proper ending */
	if (token != T_RBRACE) {
	    error(EGEN,"Bad initializer list syntax");
	    pizflush(1);		/* Flush inside of list */
	    if (token == T_RBRACE)
		nextoken();		/* Flush end close-brace if one */
	    return root;
	}
	nextoken();			/* Skip close-brace */
    }

    return root;
}

/* PIZARRAY - Parse initializer for an array.
*/
static NODE *
pizarray(t, lev)
TYPE *t;
int lev;
{
    register TYPE *subt;
    NODE *e, *n, *root;
    int cnt, braces, size;

    if (izautof)		/* Complain now */
	error(EGEN,"Initialization of automatic arrays is not allowed");

    if ((subt = t->Tsubt) == NULL) {	/* Paranoia */
	error(EINT,"undefined array");
	return pizlist();		/* Flush entire izer */
    }
    braces = (token == T_LBRACE);	/* Remember whether have braces */
    if (braces) nextoken();		/* Skip over left-brace */

    /* Now handle first array element.  Requires a bunch of special hackery
    ** for initializing array of char with a string literal.
    ** This is only allowed if there are no braces.
    ** We have to do special hackery here if the initializer is NOT a
    ** string literal, because we've already parsed a complete expression
    ** and it's too late to pass it on to piztype for normal handling.
    ** So we duplicate some code and call a common integral-type handler.
    */
    if (!braces && tischar(subt)) {
	if (!(e = exprdecl()))		/* Get expression, will use or flush */
	    return NULL;
	if (e->Nop == N_SCONST) {	/* Is expr a string constant? */
	    if (t->Tsize == 0)		/* If size undefined, make new type */
		izsym->Stype = t = findsztype(TS_ARRAY, e->Nsclen, subt);
	    else if (t->Tsize < e->Nsclen)
		error(EGEN,"String exceeds char array bounds");
	    return e;			/* Return result */
	}
	
	/* This code duplicates that found in piztype() for integral types */
	if (!tisarith(e->Ntype))
	    error(EGEN,"Char array initializer must be string literal or of arith type");
	n = chkinteg(subt, e);	/* This is 1st element of unbraced array */

    } else n = NULL;		/* Say no 1st element parsed yet */

    if (!braces && lev == 0) {
	error(EGEN,"Array initializer must be enclosed in braces");
	return n;
    }

    /* Check array size.  If unspecified, we must be handling a braced list
    ** at outermost level.  This is just a paranoid internal consistency check.
    */
    if (((size = t->Tsize) == 0) && (!braces || lev)) {
	error(EINT, "inner array size unspecified");
	pizflush(1);		/* Flush to end of inner list */
	return n;		/* Return whatever we have */
    }

    /* Now loop through list, reading array elements */
    if (!n)			/* Unless already set by char array check */
	n = piztype(subt, 1);	/* Parse 1st element */
    root = n = defnode(N3, N_IZLIST, (TYPE *)NULL, 0,	/* Put into list */
		n, (NODE *)NULL);
    cnt = 1;
    if (!braces) {
	while (++cnt <= size) {			/* Stop when array filled */
	    if (token != T_COMMA) {		/* Fail if bad separator */
		error(EGEN,"Too few elements in unbraced array initializer list");
		return root;			/* Bah, return stuff so far */
	    }
	    nextoken();				/* Skip comma */
	    n = n->Nright = defnode(N3, N_IZLIST, (TYPE *)NULL, 0,
		piztype(subt, 1), (NODE *)NULL);	/* Parse element */
	}
	/* When loop ends, fall thru to successful return */

    } else {	/* Have braces, life is more complicated */
	for (; token == T_COMMA; ++cnt) {
	    if (nextoken() == T_RBRACE) break;	/* Skip comma, allow ",}" */
	    n = n->Nright = defnode(N3, N_IZLIST, (TYPE *)NULL, 0,
		piztype(subt, 1), (NODE *)NULL);	/* Parse element */
	}
	if (token != T_RBRACE) {
	    error(EGEN,"Bad initializer list syntax");
	    pizflush(1);			/* Flush inner list */
	    if (token == T_RBRACE)
		nextoken();
	    return root;			/* Sigh, return stuff so far */
	}
	nextoken();			/* Skip close-brace */

	if (t->Tsize == 0)		/* If setting size, cnt always OK */
	    izsym->Stype = t = findsztype(TS_ARRAY, cnt, subt);
	else if (cnt > size)		/* Complain if too many */
		error(EGEN,"Too many elements in array initializer list");
    } /* End of brace handling */

    return root;
}

/* PEXIZER - Parse single initializer expression.
**	Should be only one expression; outermost braces are allowed.
**	Never returns a list; complains and fixes up as necessary.
*/
static NODE *
pexizer(lev)
int lev;			/* 0 if outermost level */
{
    NODE *n;
    if (token != T_LBRACE)
	return exprdecl();	/* All's well */

    /* We have a list of some sort.  This is only legal if at outermost
    ** level AND there is only one thing in the list.
    */
    if (lev)
	error(EGEN,"Inner initializer for this object cannot be a list");
    n = pizlist();		/* Parse the list, always returns N_IZLIST */
    if (n->Nright && lev == 0)	/* More than one thing in list? */
				/* Complain unless already complained above */
	error(EGEN, "Initializer cannot be a list of more than one element");
    if ((n = n->Nleft) == NULL) {
	error(EGEN, "Null initializer");
	return NULL;
    }
    if (n->Nop == N_IZLIST) {	/* Sublist? */
	error(EGEN, "Initializer cannot have sublist");
	while (n->Nop == N_IZLIST && (n = n->Nleft));
    }	
    return n;
}

/* PIZLIST - Parse initializer list
**	Mainly for error recovery, when contents aren't analyzed closely.
*/
static NODE *
pizlist()
{
    NODE *n, *root;

    if (token != T_LBRACE)	/* If not a list */
	return exprdecl();	/* Get expression and return simple izer */

    nextoken();				/* Skip open brace */
    root = n = defnode(N3, N_IZLIST, (TYPE *)NULL,0, pizlist(0), (NODE *)NULL);
    while (token == T_COMMA) {
	nextoken();			/* Skip comma */
	if (token == T_RBRACE) break;	/* allow comma at end of initializer */
	n = n->Nright = defnode(N3, N_IZLIST, (TYPE *)NULL, 0,
		pizlist(), (NODE *)NULL);
    }
    expect(T_RBRACE);			/* finish with close brace */
    return root;			/* return whole thing */
}

/* PIZFLUSH - Flush initializer tokens
**	Used during error recovery.  Type says what situation we're in.
**	0 - at start of izer, flush entire izer.
**		(stop if top-level comma, gobble end close-brace if one seen)
**		This also works to flush only a single list, if at its start.
**	1 - inside a list, flush to higher-level close-brace and don't
**		gobble it, so caller can handle close-brace.
*/
static void
pizflush(typ)
int typ;
{
    int lev = 0;
    if (typ) lev = 1;		/* Pretend already in a list */
    for(;; nextoken()) switch(token) {
	case T_COMMA:
	    if (typ == 0 && lev <= 0) return;
	    break;
	case T_LBRACE:		/* Start of list */
	    lev++;
	    break;
	case T_RBRACE:		/* End of list */
	    if (--lev <= 0) {
		if (typ == 0) nextoken();	/* Maybe gobble close-brace */
		return;
	    }
	    break;

	case T_EOF:		/* Input EOF */
	case T_SCOLON:		/* End of declaration statement */
	    return;
    }
}

/* NISCONST - Returns true if expression is an allowable initializer constant.
**	Return value indicates something about the type of constant:
**	0 - not a constant
**	1 - definitely a constant (arithmetic, or a cast pointer)
**	2 - address of some kind
**	3 - function address (cannot add or subtract from this)
*/
static int
nisconst(e)
NODE *e;
{
    int res;

    switch(e->Nop) {
	case N_CAST:		/* Assume generator will be able to handle */
	    return nisconst(e->Nleft);

	case N_ICONST:
	case N_FCONST:
	case N_PCONST:
	    return 1;		/* Simple constant */
	case N_SCONST:
	    return 2;		/* Address */

	case Q_IDENT:
		/* Identifier.  See documentation for Q_IDENT in cctoks.h
		** for explanation of this method of testing.
		*/
	    switch (e->Nid->Stype->Tspec) {
		case TS_FUNCT:	return 3;	/* Function address */
		case TS_ARRAY:	return 2;	/* Array address */
	    }
	    break;

	case N_ADDR:
	    switch (e->Nleft->Nop) {
		case N_PTR:
		    return nisconst(e->Nleft->Nleft);

#if 0
		/* Allow for conversion of arrays generated by subscripting */
		case Q_PLUS:
		    if (e->Nleft->Ntype->Tspec == TS_ARRAY)
			return nisconst(e->Nleft);	/* OK, continue */
		    break;				/* Not array, fail */
#endif
		/* Structure hair.
		** For MEMBER (->) the Nleft must be a constant address.
		**	Can just apply nisconst to this.
		** For DOT (.) the Nleft can be anything that evaluates into
		**	a static structure.  We assume this is only possible
		**	with either Q_IDENT, or N_PTR of a struct addr.
		*/
		case Q_DOT:
		    if (tisbitf(e->Nleft->Ntype))	/* No bitfield ptrs */
			return 0;
		    switch (e->Nleft->Nleft->Nop) {
			case Q_IDENT:
			    switch (e->Nleft->Nleft->Nid->Sclass) {
				case SC_AEXTERN: case SC_EXTERN:
				case SC_STATIC: case SC_ISTATIC:
				    return 2;	/* Good address of object */
			    }
			    break;
			case N_PTR:
			    if (nisconst(e->Nleft->Nleft->Nleft)==2)
				return 2;
			    break;
		    }
		    break;			/* Otherwise fail. */

		case Q_MEMBER:
		    if (!tisbitf(e->Nleft->Ntype)	/* No bitfield ptrs */
		      && nisconst(e->Nleft->Nleft)==2)	/* If struct addr is */
			return 2;		/*  OK, then we're OK */
		    break;			/* Otherwise fail. */

		case Q_IDENT:	/* Addr OK if of external or static */
			/* Needn't test type since parser checks it while
			** parsing "&" to verify not function or array.
			*/
		    switch (e->Nleft->Nid->Sclass) {
			case SC_AEXTERN: case SC_EXTERN:
			case SC_STATIC: case SC_ISTATIC:
			    return 2;		/* Good address of object */
		    }
		    break;
	    }
	    break;

	/* Non-atomic expression checks, for plus and minus. */
	case Q_PLUS:
	    if (e->Nleft->Nop == N_ICONST	/* Integ constant */
		&& nisconst(e->Nright) == 2)	/* plus address */
		    return 2;
	    /* Fall through into Q_MINUS code */
	case Q_MINUS:
	    if (nisconst(e->Nleft) == 2		/* Address */
	      && e->Nright->Nop == N_ICONST)	/* plus/minus integ constant */
		return 2;
	    break;
    }
    return 0;
}

#if 0
/* CHKPTRCON - utility routine to check out a pointer initializer.
**	Allowed initializers (per CARM 4.6.3) must be an integer or
**	a static/external address plus/minus an integer.
*/
static NODE *
chkptrcon(e)
NODE *e;
{
    NODE *n, *addr, *con;

    n = (e->Nop == N_CAST) ? e->Nleft : e;	/* Allow casts */
    if (n->Nop == N_ICONST) return e;	/* Integer const OK (case 1,6) */
    if (n->Nop == Q_IDENT) return e;	/* Funct or array name OK (case 2,3) */
					/* (type cked by convasgn) */

    if (n->Nop == Q_PLUS || n->Nop == Q_MINUS) {
	addr = n->Nleft;
	con = n->Nright;
	if (n->Nop == Q_PLUS && con->Nop != N_ICONST)
	    addr = n->Nright, con = n->Nleft;
	if (con->Nop != N_ICONST) {
	    error(EGEN,"Pointer initializer constant has bad expression");
	    return NULL;
	}
    } else addr = n;

    /* Check out address */
    n = (addr->Nop == N_CAST) ? addr->Nleft : addr;	/* Allow casts */
    if (n->Nop == N_ICONST) return e;	/* Integer const OK (case 6) */
    if (n->Nop == N_SCONST) return e;	/* String const OK (case 7) */
    if (n->Nop == N_ADDR) {		/* Addr of something (case 4) */
	addr = n->Nleft;
	if (addr->Nop != Q_IDENT
	  || isauto(addr->Nid)) {
	    error(EGEN,"Constant pointer initializer has non-constant addr");
	    return NULL;
	}

    }
    return e;
}
#endif /* commented-out code */

/* ISAUTO - Returns true if symbol has automatic extent
*/
static int
isauto(s)
SYMBOL *s;
{
    switch(s->Sclass) {
	case SC_ARG:  case SC_RARG:
	case SC_AUTO: case SC_RAUTO:
		return 1;
	default:
		return 0;
    }
}
#endif

/* ----------------------------------- */
/*	type-name  Ref.[1]  A.8.7      */
/* ----------------------------------- */
/*	This parses a "type name" built from an abstract declarator.
** Used only for cast expressions and the sizeof operator.
*/

TYPE *
typename()
{
    SYMBOL s, *t;

    pbase(&s);			/* Parse base */
    if (s.Sclass != SC_UNDEF) {
	error(EGEN,"Storage class not allowed for type-name");
	s.Sclass = SC_UNDEF;
    }
    if (s.Stype == NULL) {
	error(EGEN,"No type-specifier for type-name, assuming int");
	s.Stype = deftype;
    }
    if ((t = decl0(&s)) != NULL) {
	error(EGEN, "Identifier not allowed in type-name");
	if (t->Sclass == SC_UNDEF)
	    freesym(t);			/* Clean up */
    }
    return s.Stype;
}

/* ------------------------------------------- */
/*      check two types for compatibility      */
/* ------------------------------------------- */

static int
tmismatch (t1, t2)
TYPE *t1, *t2;
{
    if (t1 == t2) return 0;
    if (t1->Tspec != TS_ARRAY || t2->Tspec != TS_ARRAY) return 1;
    if (t1->Tsubt != t2->Tsubt) return 1;
    return (t1->Tsize != 0 && t2->Tsize != 0);
}
