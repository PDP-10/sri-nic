/*	CCSYM.C - Symbol table management (type table too)
**
**	All changes after version 39 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCSYM.C.46, 17-Dec-85 08:00:41, Edit by KLH */
/*  Rationalized names of constants and structures */
/* <KCC.CC>CCSYM.C.36, 25-Jun-85 16:17:05, Edit by KRONJ */
/*  Avoid rehash problems after symbol flush */
/* <KCC.CC>CCSYM.C.32, 29-May-85 10:29:15, Edit by KRONJ */
/*  Init free label list */
/* <KCC.CC>CCSYM.C.31, 19-Mar-85 11:41:12, Edit by SATZ */
/* Make sure function name gets output for symbol dump */
/* <KCC.CC>CCSYM.C.22,  9-Mar-85 10:13:58, Edit by KRONJ */
/*  Reinstate SC_SCOPE; scopeauto() was releasing symbols */
/*  before they were used by gencode(), so if someone else */
/*  (like symbols in another block or register alloc) */
/*  did a malloc() in between we'd lose big. */
/* <KCC.CC>CCSYM.C.19,  7-Mar-85 17:42:59, Edit by KRONJ */
/*  More flexible initialization of built in types. */
/*  Use hash buckets rather than rehashing in type hash table. */
/* <KCC.CC>CCSYM.C.18, 27-Feb-85 10:57:59, Edit by SATZ */
/* Add a routine called findasym which will look for a symbol */
/* with a specific class type */
/* <KCC.CC>CCSYM.C.17, 24-Feb-85 21:25:53, Edit by SATZ */
/* Make symbol table entries dynamic. Added two routines */
/* That allocate and free symbol table entries, getsym and retsym */
/* <KCC.CC>CCSYM.C.9,  2-Feb-85 16:35:57, Edit by SATZ */
/* Have to let cclex handle __FILE__ also */
/* <KCC.CC>CCSYM.C.7,  2-Feb-85 16:06:32, Edit by SATZ */
/* Make initsym() initialize the __FILE__ and __LINE__ macros */
/* <KCC.CC>CCSYM.C.6,  3-Jan-85 13:47:17, Edit by SATZ */
/* make sure the @ is skipped whe printing undefined label name */
/* <KCC.CC>CCSYM.C.5, 29-Dec-84 17:23:19, Edit by KRONJ */
/*  give error for undefined local labels */
/* SCORE:<KCC.CC>CC7.C.2, 12-May-84 12:53:19, Edit by KRONJ */
/*  Rework type hash table */

/* cc7.c -- Symbol table   (C) 1981  K. Chen */

#include "cc.h"

/* Exported functions */
void initsym();		/* CC */
SYMBOL *findsym(), *findsmem(), *findmsym(), *findnsym();
SYMBOL *makesym(), *creatgsym(), *uniqsym(), *shmacsym();
void freesym(), copysym();
int isdupsym();
void idcpy(), idpfcpy();
int hash();		/* For CCFOLD's optexpr() */
SYMBOL *beglsym();
void endlsym(), ridlsym(), funlsym();

SYMBOL *newlabel();			/* Label functions */
void reflabel(), freelabel(), cleanlabs();

TYPE *findtype(), *findsztype(), *findctype();
int sizetype();		/* For CCDECL, CCSTMT, CCGEN* */
int sizeptobj();	/* For CCGEN2 */
int sizearray();	/* For CCGEN, CCSTMT */
int elembsize();	/* ditto */
int chararray(), charpointer(), bytearray(), bytepointer();

/* Imported functions */
char *malloc();

extern SYMBOL *htable[];	/* Hash table, allocated in CCDATA */

/* Local functions */
static void initype(), initlab();
static void inisymlist();
static void makelsym();
static SYMBOL *getsym();
static void retsym();
static int idcmp();

#if 0	/* For debugging */
#define bugmsg(a) if(symdeb) printf a;
static int symdeb = 0;
#else
#define bugmsg(a) ;	/* Null stmt */
#endif

#if 0
		SYMBOL TABLE STRUCTURE

The "symbol table" is implemented as a collection of dynamically allocated
symbol entries.  A symbol is always linked either to the global symbol
list, or the local symbol list.  In addition to this linkage, all symbols
also belong to some hash chain list.

The hash table is used to look up symbols.  The identifier is hashed
to produce a index into the hash table, which contains pointers to all
of the hash chains; a given chain consists of all symbols whose
identifiers produce that specific hash value.  This chain is then
searched sequentially, doing full string comparison on the identifiers,
until the matching symbol (if any) is found.

Symbols on a hash chain are linked MOST-RECENT-FIRST, and the first
matching symbol is considered to hide or shadow all other instances of
that identifier (unless it is flagged as no longer active).  This is how
the scope and visibility of symbols are implemented for symbol lookup.
Some special checking is done for macro symbols; see further comments at
end of this page.

Symbols must also be linked into either the global or local symbol
list.  These lists are doubly linked and new entries are added
MOST-RECENT-LAST (as opposed to the hash chain lists).  All symbols on
the global list have the same scope, and no duplicate identifiers
should exist on that list.  Symbols may initially be linked onto the
global list for a short time before being flushed or re-linked onto
the global list, but in general nothing is deleted from the global
symbol list unless re-initializing to compile a new file.

The local symbol list, however, is more dynamic.  During the parsing,
local symbols are added on the end of this list as they are
encountered; once their scope has expired (the end of a block was
reached), they are marked inactive but remain on the list for the
benefit of the code generation routines.  Once the entire function
has been generated, all local symbols are flushed and the list
re-initialized.

The tricky part of the local symbol list has to do with how the block
structure of a C function is represented so that local symbols have
only their proper scope.  At any given moment, the pointer "lsymhead"
points to the first symbol of the innermost active block; if there is
no active block (i.e. parsing at top level) this pointer is NULL.
The symbols belonging to this active block consist of this first symbol
and all succeeding symbols on the list, except for those which are marked
inactive (by setting the SF_XLOCAL flag).  Inactive symbols, if they exist,
will be those belonging to inner blocks (inside the current block) 
that have been exited.

When a block is first entered (via beglsym()) the old value is saved,
and lsymhead is set to the current tail of the local symbol list.
Now, whenever a local symbol is defined in this block, it will be
added to the end of the list; to see whether a duplicate definition of
the symbol already exists, it suffices to scan the list starting at
the lsymhead pointer (see isdupsym()).  When the block is finally
ended (via endlsym()), all symbols belonging to this block will be
marked inactive, and the old value of lsymhead restored so that it now
points to the next outer block.

The hash chain lists and the global/local lists are completely independent
of each other.

Note that the global and local lists are doubly linked and each has a "dummy"
initial symbol entry to render checks for NULL unnecessary.  These are the
only two "symbols" not also on a hash chain.  The hash chain lists are singly
linked and end in NULL.  Unused symbol entries are kept around on a freelist
to avoid the overhead of calls to malloc/free; they are never given to free(),
even at the start of a new file compilation, under the assumption that the
efficiency improvement is worth the (very slight) risk that storage will
become excessively fragmented over many compilations.

MACRO SYMBOLS:
	Although normally macro symbols are unique, and thus shadowing is
never an issue, ANSI makes it possible for macro self-references to generate
non-macro symbols that are identical to macro names.  In other words, macro
symbols can also be shadowed.  But because this should only happen in
special circumstances, special checking is needed.
	There are only two places where a macro symbol is looked for:
CCLEX's getident() to handle an identifier token, and CCINP's
findmacsym() to explicitly look for a macro name.  These two places
invoke findmsym(), which finds the first hashed symbol (whether macro
or not).  ALL other symbol lookups can safely assume that any identifiers
they deal with have already been expanded if necessary, and so they all
use findsym(), which ignores any macro symbols it encounters and will
thus find any shadowed symbols.
	In order to ensure that findmsym() finds the macro symbol first
if one exists, that symbol always has to come BEFORE the shadowed symbol
on the hash chain.  This ordering is ensured by the shmacsym() function,
which getident() invokes whenever a new symbol is being shadowed by a
macro.  uniqsym() also invokes this routine if it is about to create
a duplicate of a macro-shadowed symbol.

#endif

SYMBOL *lsymhead;	/* NULL at top level, else points to head of
			** current local symbol block.  The first sym
			** on the list is lsymhead->Snext.
			** Only reason this isn't static is cuz CCDECL
			** wants to know if we're in a local block.
			*/
static SYMBOL *symflist = NULL;	/* Symbol entry freelist, for efficiency */

static SYMBOL
/*  *symbol,	*/	/* Global symbol list head */
    *symtail,		/* ptr to tail of global list */
    *locsymbol,		/* Local symbol list head */
    *loctail;		/* ptr to tail of local list */
int nsymbols = 0;	/* # of symbols allocated (except dummies) */

/* ------------------------- */
/*	initializations      */
/* ------------------------- */

void
initsym()
{
    int i;
    RESWD *r;
    SYMBOL *s;

    /* Initialize labels, symbols, and types */
    initlab();				/* Initialize internal label stuff */

    inisymlist(&symbol, &symtail);	/* Initialize global symbol list */
    inisymlist(&locsymbol, &loctail);	/* Initialize local symbol list */
    lsymhead = NULL;			/* Currently at top level */

    /* Clear out symbol hash table and set initial reserved-word symbols */
    for (i = 0 ; i < MAXHSH ; i++)		/* Clear hash table */
	htable[i] = NULL;
    for (r = rwtab; r->rw_name != NULL; r++) {	/* Enter all reserved words */
	if ((i = r->rw_token)&(RWF_ANSI+RWF_KCC)) {	/* Any flags set? */
	    if (((i & RWF_ANSI) && clevel >= CLEV_ANSI)	/* If ANSI and OK, */
		|| ((i & RWF_KCC) && clevkcc))		/* or KCC and OK, */
		; else continue;	/* then go ahead, else skip sym. */
	}
	s = creatgsym(r->rw_name);	/* Make symbol for the word */
	s->Sclass = SC_RW;		/* Say it's a reserved word */
	s->Stoken = i & RWF_TOKENMASK;		/* Get token value */
	s->Skey = tok[s->Stoken].tktype;	/* and token's type */
    }
    minsym = symtail;		/* Crock for CCDUMP's symdump, someday flush */

    initype();		/* Now initialize tables etc. for C data types */
}

static void
inisymlist(ahead, atail)
SYMBOL **ahead, **atail;
{
    SYMBOL *s, *head;

    if (*ahead == NULL) {	/* Initialize for first time only */
	s = (SYMBOL *) malloc(sizeof(SYMBOL));	/* Allocate a sym entry */
	if (s == NULL) efatal(EOUTMEM);
    }
    else {			/* Symbols already exist, free them. */
	for (head = (*ahead)->Snext; s = head;) {	/* For all but 1st */
	    if (s->Sclass == SC_MACRO && s->Smacptr)
		free(s->Smacptr);	/* If macro body exists, free it */
	    head = s->Snext;
	    retsym(s);			/* Free up the symbol */
	}
	s = *ahead;			/* Done, re-use 1st sym */
    }

    /* Initialize the 1st sym on list, which is just a dummy that
    ** is never used for anything.  Its existence allows the list routines
    ** to skip some checks for NULL-ness of pointers.
    */
    *ahead = *atail = s;		/* Head and tail point to dummy sym */
    s->Snext = s->Sprev = NULL;		/* Nothing else on list */
    s->Sclass = SC_UNDEF;		/* Just in case... */
}

/* FINDSYM - find a symbol in the symtab, and return pointer to the
**	currently active definition of the symbol.
**	Ignores macros and local symbols which are no longer active.
**	This is the routine used by most of the parser code.
*/
SYMBOL *
findsym (s)
char *s;
{
    register SYMBOL *sym;

bugmsg(("findsym \"%s\"\n", s))
    for (sym = htable[hash(s)]; sym != NULL; sym = sym->Snhash)
	if (((sym->Sflags&(SF_XLOCAL|SF_MACRO)) == 0) && idcmp(sym->Sname, s)){
	    sym->Srefs++;
	    break;
	}
    return sym;
}

/* FINDMSYM - Same as FINDSYM but searches for macro symbols as well.
**	This is only used by CCLEX and CCINP in contexts where a macro
**	can be recognized.
**	Ignores local symbols which are no longer active.
*/
SYMBOL *
findmsym (s)
char *s;
{
    register SYMBOL *sym;

bugmsg(("findmsym \"%s\"\n", s))
    for (sym = htable[hash(s)]; sym != NULL; sym = sym->Snhash)
	if (((sym->Sflags&SF_XLOCAL) == 0) && idcmp(sym->Sname, s)) {
	    sym->Srefs++;
	    break;
	}
    return sym;
}

/* FINDNSYM - Finds Next Symbol.
**	Used by getident() to find non-macro instance of a symbol.
** Note that the argument is a symbol pointer, not an identifier string.
*/
SYMBOL *
findnsym (osym)
SYMBOL *osym;
{
    register SYMBOL *sym = osym;
bugmsg(("findnsym \"%s\"\n", sym->Sname))
    while (sym = sym->Snhash)
	if (((sym->Sflags&(SF_XLOCAL|SF_MACRO)) == 0)
	 && idcmp(sym->Sname, osym->Sname)) {
	    sym->Srefs++;
	    break;
	}
    return sym;
}

/* FINDSMEM - Find a structure member.
*/
SYMBOL *
findsmem (id, tag)
char *id;
SYMBOL *tag;
{
    register SYMBOL *sym;

    for (sym = htable[hash(id)]; sym != NULL; sym = sym->Snhash)
	if ( sym->Sclass == SC_MEMBER && sym->Ssmtag == tag
	  && ((sym->Sflags&SF_XLOCAL) == 0) && idcmp(sym->Sname, id)) {
	    sym->Srefs++;
	    break;
	}
    return sym;
}


/* -------------------------------------- */
/*	free a symbol table location      */
/* -------------------------------------- */
void
freesym(s)
SYMBOL *s;
{
    register SYMBOL *sym, *psym;
    int h;

bugmsg(("freesym \"%s\"\n", s->Sname))
    sym = htable[h = hash(s->Sname)];	/* Find head of hash chain */
    if (sym == s)
	htable[h] = sym->Snhash;
    else {
	do if (sym->Snhash == s) {
	    sym->Snhash = s->Snhash;
	    retsym(s);
	    return;
	} while (sym = sym->Snhash);
	efatal(EINT,"freesym arg not on hash list");
    }
    retsym(s);				/* give sym back to mem allocator */
}

static SYMBOL *mksym();		/* Workhorse routine for stuff below */

/* MAKESYM - Create a symbol table entry.
**	Symbol will be local or global depending on current context,
** i.e. the setting of lsymhead.  If NULL, current "block" is top level
** and symbol will be put on the global list.  Otherwise, it is put on
** the local list.
*/

SYMBOL *
makesym (id)
char *id;
{
    bugmsg(("makesym %s \"%s\"", (lsymhead?"local":"global"), id))
    return mksym(id, (lsymhead == NULL ? &symtail : &loctail));
}

/* CREATGSYM - create a global symbol table entry
*/
SYMBOL *
creatgsym (id)
char *id;
{
    bugmsg(("creatgsym \"%s\"", id))
    return mksym(id, &symtail);
}

/* MAKELSYM - Make global symbol a local one.
**	Sometimes needed when lexer creates a global entry for
** a new identifier, and it later needs to be made local instead.
*/
static void
makelsym(s)
SYMBOL *s;
{
    /* Remove from global list */
    if (s == symtail)		/* If sym is most recent one on global list, */
	symtail = s->Sprev;	/* must update the tail pointer. */
    if (s->Sprev)
	s->Sprev->Snext = s->Snext;
    if (s->Snext)
	s->Snext->Sprev = s->Sprev;

    /* Add to local list */
    s->Sprev = loctail;
    s->Snext = (SYMBOL *) NULL;
    loctail->Snext = s;
    loctail = s;
}

/* UNIQSYM - Given a symbol pointer, returns a pointer to an
**	unique symbol table entry with the same name.  This
** will either re-use the current entry (if it is undefined)
** or will create a new duplicate symbol of the appropriate scope.
** Any checks for duplicate definition errors should be made before
** this routine is called.
**	If a new duplicate symbol is made, the reference count of the
** original symbol is decremented to compensate for what was a mistaken
** reference.
*/
SYMBOL *
uniqsym(s)
SYMBOL *s;
{
    if (s->Sclass != SC_UNDEF) {	/* If sym already exists, */
	s->Srefs--;			/* correct its ref count. */

	/* If this symbol has same identifier as a macro symbol, then
	** after creating it, the shadow flag needs to be propagated, and
	** the macro sym entry needs to be moved up.  shmacsym() does this.
	*/
	if (s->Sflags & SF_MACSHADOW)	/* Shadowing a macro def? */
	    return shmacsym(makesym(s->Sname));	/* Yes, shadow after create! */
	return makesym(s->Sname);	/* No, just return a duplicate. */
    }

    /* Can use this symtab entry, just make sure it has right scope. */
    if (lsymhead)		/* If current scope is local, */
	makelsym(s);		/* change sym from global to local */
    return s;
}

/* ISDUPSYM - Returns TRUE if symbol is already defined in current block
*/
int
isdupsym(sym)
SYMBOL *sym;
{
    SYMBOL *ls;

    if (sym->Sclass == SC_UNDEF)	/* If it has this class */
	return 0;			/* then it was never defined before */
    if (lsymhead == NULL)	/* Symbol is defined.  If now at top level, */
	return 1;		/* the symbol is always top-level also. */
    /* We are in a local block.  To see whether the symbol is defined
    ** within this block, we start scanning the local symbol list beginning
    ** with the current local block head.  If we encounter the symbol then
    ** it is defined in the current block.
    ** Symbols from inner blocks will never be given as args since findsym
    ** never returns inactivated symbols.
    ** Symbols from outer blocks will never be scanned because all symbols
    ** for outer blocks precede those of the current block in the local
    ** symbol list.
    */
    for (ls = lsymhead; ls = ls->Snext;)
	if (ls == sym) return 1;
    return 0;
}

/* MKSYM - Create a symbol table entry - internal workhorse auxiliary.
**	Symbol will be added to whatever list is provided (locsym or symtail).
*/
static SYMBOL *
mksym(id, tailptr)
char *id;
SYMBOL **tailptr;
{
    register SYMBOL *sym;
    int n;

    sym = getsym(tailptr);
    sym->Snhash = htable[n = hash(id)];
    htable[n] = sym;

    bugmsg((" = %o, hash %o\n", sym, n))	/* Finish off bug msg if one */

    idcpy(sym->Sname, id);
    sym->Sclass = SC_UNDEF;
    sym->Sflags = 0;
    sym->Stype = NULL;
    sym->Svalue = 0;
    sym->Srefs = 0;
    return sym;
}

/* SHMACSYM - Say this symbol shadows a macro symbol.
**	Sets flag and moves macro symbol matching this one to top of hash list.
**	This is used by uniqsym() and CCLEX's getident() whenever they
**	create a new symbol which shadows an existing macro symbol; the
**	macro symbol needs to be moved up to the head of the hash list
**	to ensure that it is always found by findmsym() before the new symbol.
** Always returns its arg, for convenience.
*/
SYMBOL *
shmacsym(sym)
SYMBOL *sym;
{
    register SYMBOL *s, *prev = NULL;
    int n;

    sym->Sflags |= SF_MACSHADOW;		/* Set flag in this symbol */
    for (s = htable[n = hash(sym->Sname)]; s != NULL; prev = s, s = s->Snhash)
	if (s->Sclass == SC_MACRO && idcmp(sym->Sname, s->Sname)) {
	    /* Found the matching macro symbol!  Unlink from hash chain */
	    if (!prev)			/* If already at start of chain, */
		break;			/* nothing to do, just return */
	    prev->Snhash = s->Snhash;	/* Unlink */
	    s->Snhash = htable[n];	/* Then put at start */
	    htable[n] = s;
	    break;
	}
    return sym;
}

/* GETSYM - allocate a symbol entry.
*/
static SYMBOL *
getsym(tailptr)
SYMBOL **tailptr;
{
    SYMBOL *newptr;

    if (newptr = symflist)	/* If freelist has one, take it off */
	symflist = newptr->Snext;
    else if ((newptr = (SYMBOL *) malloc(sizeof(SYMBOL))) == NULL)
	    efatal(EOUTMEM);
    else nsymbols++;		/* Bump # of symbols allocated */
    newptr->Sprev = *tailptr;
    newptr->Snext = (SYMBOL *) NULL;
    (*tailptr)->Snext = newptr;
    *tailptr = newptr;
    return newptr;
}

/* RETSYM - De-allocate a symbol entry.
*/
static void
retsym(syment)
SYMBOL *syment;
{
    if (syment == symtail)
	symtail = syment->Sprev;
    else
	if (syment == loctail)
	    loctail = syment->Sprev;
    if (syment->Sprev)
	syment->Sprev->Snext = syment->Snext;
    if (syment->Snext)
	syment->Snext->Sprev = syment->Sprev;
/*    free((char *)syment); */
    syment->Snext = symflist;		/* Put entry on freelist */
    symflist = syment;
}
	
/* ------------------------------------ */
/*	copy symbol representation      */
/* ------------------------------------ */
/* Does not copy Snhash, Sprev, Snext, or Srefs. */
void
copysym(s,t)
SYMBOL *s, *t;
{			/* Use struct assignment since have unions inside */
    s->Scontents = t->Scontents;
}

/* IDCMP - Compare symbol identifiers for equality
**	Can do without count since existing symbol string is null-terminated
*/
static int
idcmp(s,t)
char *s, *t;
{
    if (*s == *t)
	while (*++s == *++t)
	    if (*s == '\0')
		return 1;
    return 0;
}

/* --------------------------------- */
/*	copy symbol identifiers      */
/* --------------------------------- */
void
idcpy(s, t)
char *s, *t;
{
    register int i;

    if (*s = *t)
	for(i = _IDSIZE-1; *++s = *++t; )
	    if (--i <= 0) {	/* Stop if written into last char */
		*s = '\0';	/* Change last char to a null */
		break;
	    }
}

/* --------------------------------------------------- */
/*	copy symbol identifiers with a prefix char     */
/* --------------------------------------------------- */
void
idpfcpy(s, c, t)
char *s;	/* Destination */
int c;		/* Prefix char (a SPC_ value) */
char *t;	/* Source */
{
    int i;

    *s = c;
    if(*++s = *t)
	for(i = 1; *++s = *++t; )
	    if (++i >= _IDSIZE-1) {	/* Stop if written into last char */
		*s = '\0';		/* Change last char to a null */
		break;
	    }
}


/* ------------------------------------ */
/*	compute symbol hash (MAXHSH = 2^N)     */
/* ------------------------------------ */
int
hash (s)
register char *s;
{
    register int i, j, count;
  
    i = 0;
    if (j = *s) {
	count = _IDSIZE-1;
	do i = (i+i + j);
	while (--count > 0 && (j = *++s));
    }
    return (i & (MAXHSH-1));
}

/* BEGLSYM - Begin a local symbol block.
**	Called when a block (compound statement or function) is entered.
** Sets up a new block pointer, a global variable which points to the start of
** the local symbol list for this block.
** Returns the PREVIOUS value of this pointer (the parent block).  This
** should be saved by the caller and given back to endlsym() when the
** new block is finished.
**	Only called by CCSTMT's compound() and CCDECL's funcdef().
*/
SYMBOL *
beglsym()
{
    SYMBOL *retsym;
bugmsg(("beglsym %o => %o\n", lsymhead, loctail))
    retsym = lsymhead;		/* Remember current block head */
    lsymhead = loctail;		/* Set new head */
    return retsym;		/* Return what is now previous block head */
}

/* ENDLSYM - End a local symbol block.
**	Deactivates all symbols in the current block, and restores the
** previous block head pointer (which must be furnished by caller).
**	Only called by CCSTMT's compound() and CCDECL's funcdef().
*/
void
endlsym(prevptr)
SYMBOL *prevptr;
{
    register SYMBOL *sym;

bugmsg(("endlsym %o => %o\n", lsymhead, prevptr))
    if ((sym = lsymhead) == NULL)
	efatal(EGEN,"Internal error - treating top level as block");

    while ((sym = sym->Snext) != NULL)
	switch (sym->Sclass) {
	    default:		/* Zap everything except labels */
		sym->Sflags |= SF_XLOCAL;	/* Zap the sym */
	    case SC_LABEL:	/* Labels are active throughout function */
	    case SC_ULABEL:
		break;
	}
    lsymhead = prevptr;		/* Then restore ptr to parent block */
}

/* RIDLSYM - Flush all local symbols.
**	Only called after the code generation for a function is
** completely finished, by CCGEN's gencode().
*/
void
ridlsym()
{
    SYMBOL *s, *prev;

bugmsg(("ridlsym ---"))
    if (debsym && locsymbol->Snext) symdump(locsymbol->Snext, curfn->Sname);
    for (s = loctail; s != locsymbol; s = prev) {
	switch (s->Sclass) {
	case SC_ULABEL:			 /* Undefined label? (fall through) */
	    error(EGEN,"Goto label \"%s\" never defined", s->Sname+1);
	case SC_ISTATIC:
	case SC_LABEL:
	    freelabel(s->Ssym);		/* Flush no longer useful label */
	}
	prev = s->Sprev;
	freesym (s);
    }
    lsymhead = NULL;
bugmsg(("ridlsym done---"))
}

/* FUNLSYM - Set up local symbol list for function.
**	Initialize local symbol list with any existing parameter
** symbols.  Called only by CCDECL's funcdef() when about to start parsing
** function parameter declarations.
*/
void
funlsym()
{
    SYMBOL *s;
    /* Scan entire local symbol list.  The only symbols on it, if any,
    ** should be function parameters (class SC_ARG).  Make sure that each
    ** is now visible; this is the only place where SF_XLOCAL is ever
    ** revoked.
    */
bugmsg(("funlsym:"))
    for (s = locsymbol; s = s->Snext;) {
bugmsg((" %s,",s->Sname))
	if (s->Sclass == SC_ARG)
	    s->Sflags &= ~SF_XLOCAL;	/* Make parameter sym visible again */
	else error(EGEN,"Internal error - non-parameter local sym");
    }

    /* Now set up as if all local symbols belonged to a new block.
    ** This will be the first such block; its outer block is actually
    ** the top level.
    ** This block is never undone by endlsym().  Instead, ridlsym just
    ** flushes everything after the function is all done.
    */
    lsymhead = locsymbol;	/* Starts at start of all local syms */
bugmsg((" ptr=%o",lsymhead))
}

/* LABEL MANAGEMENT CODE */

static int maxlabel;		/* Current highest-numbered label */
static SYMBOL *fllist = NULL;	/* Free label list */
static SYMBOL *flprev = NULL;	/* queue of almost-free labels */
static int nlabels = 0;		/* # labels allocated */

/* INITLAB - Initialize label stuff.
**	Called by INITSYM at start of compilation for each file.
*/
static void
initlab()
{
    maxlabel = 0;		/* Reset internal label numbering to 0 */
    cleanlabs();		/* Ensure no queued labels */
	/* Note that "fllist" is left alone in case it contains free labels,
	** which will save us the bother of allocating them.
	*/
}

/*
** Get a new label to play with.
**
** The argument should be nonzero if the label will be emitted after all
** uses of it, rather than before.  If it is zero, the label is emitted.
*/

SYMBOL *
newlabel()
{
    SYMBOL *lab;

    /* find a free label */
    if (lab = fllist)			/* If have one already free, */
	fllist = fllist->Snext;		/* remove it from freelist */
    else if ((lab = (SYMBOL *) malloc(sizeof (*lab))) == NULL)	/* make new */
	efatal(EOUTMEM);
    else nlabels++;			/* Bump # of labels allocated */

    /* fill it out */
    lab->Sclass = SC_ILABEL;		/* this is an internal label */
    sprintf(lab->Sname, "$%d", ++maxlabel); /* give it a name */
    lab->Svalue = 0;			/* no uses yet */
    return lab;
}

/* REFLABEL - Reference or dereference a label.
**
** The second argument is how much to add to the reference count.
** The label may be NULL or not a SC_ILABEL; in that case nothing happens.
*/
void
reflabel(lab, count)
SYMBOL * lab;
{
    if (lab != NULL && lab->Sclass == SC_ILABEL)
	lab->Svalue += count;
}

/* FREELABEL - release a label
**
** Unfortunately we can't know when the last instance in the peephole
** buffer has been emitted, at least until we flush the whole thing out.
** So we keep explicitly freed labels on another list and only transfer
** them after we have emitted a new label (and thus called flushcode()).
**
** This list chains through sprev rather than snext to keep things simple.
*/
void
freelabel(lab)
SYMBOL *lab;
{
    lab->Sprev = flprev;		/* chain old freelist onto it */
    flprev = lab;			/* it is now head of freelist */
}

/* CLEANLABS() - actually free up all "almost-free" labels.
*/
void
cleanlabs()
{
    static void realfreelabel();

    while (flprev != NULL) {	/* peephole buffer is now empty */
	realfreelabel(flprev);	/* so free the list of labels queued */
	flprev = flprev->Sprev;	/* by freelabel() */
    }				/* (nb sprev unchanged by realfree) */
}

/* REALFREELABEL - Really free a label.
**
** This should be called after the last possible reference to the label.
** It will be called automatically on emission of forward labels.
** Note sprev must not be changed (see cleanlabs()).
*/
static void
realfreelabel(lab)
SYMBOL *lab;
{
    lab->Snext = fllist;		/* chain old freelist onto it */
    fllist = lab;			/* it is now head of freelist */
}

/* INITYPE - Initialize things for support of C types.
** Mainly initializes the type table with the supported basic types.
*/
static void
initype()
{
    int i;

    /* First a crock to ensure "char" byte size selection has effect */
    typbsiztab[TS_CHAR] = typbsiztab[TS_UCHAR] = tgcsize;

    maxtype = 0;
    for (i = 0 ; i < TYPES ; i++)  ttable[i] = NULL;
    for (i = 0; i < TS_MAX; i++)
	if (i == TS_VOID || typsiztab[i] != 0)
	    typeptr[i] = findtype(i, (TYPE *)NULL);
	else typeptr[i] = NULL;

    /* Machine-dependent... clobber table so some types are equivalent */
    /* Someday clean this up and make it table-driven also */
    chartype = uchartype;	/* Say plain "char" is "unsigned char" */
    deftype = inttype;		/* Default type is "int" */
    strcontype = findtype(TS_PTR, chartype);	/* Type of string constant */
}


/* FINDTYPE - Get pointer to type, create if necessary
**	Note CCDECL's tagspec() calls this with a symbol (tag) pointer instead
**	of a type pointer.
*/

TYPE *
findtype(typ, subt)
TYPE *subt;
{    return findctype(typ, typbsiztab[typ],
		typsiztab[typ], subt);
}

TYPE *
findsztype(typ, siz, subt)
TYPE *subt;
{    return findctype(typ, typbsiztab[typ],
		siz, subt);
}

TYPE *
findctype(typ, flags, siz, subt)
TYPE *subt;
{
    TYPE *t;
    int hash;

    flags |= _tfltab[typ];	/* Add in usual flags */

    /* hash up attributes of this type and look up in table */
    hash = ( ((int) subt) + (typ * 43) + (siz * 101) ) % THASHSIZE;
    for (t = ttable[hash]; t != NULL; t = t->Trehash)
	if (t->Tspec == typ && t->Tflag == flags
		&& t->Tsize == siz && t->Tsubt == subt)
	    return t;

    /* not found, have to make up a new one */
    t = &types[maxtype++];		/* someday this should be a malloc() */
    if (maxtype >= TYPES)
	efatal(EGEN,"Internal error - increase type table size");
    t->Tspec = typ;
    t->Tsize = siz;
    t->Tflag = flags;
    t->Tsubt = subt;
    t->Trehash = ttable[hash];		/* link old types with same hash */
    ttable[hash] = t;			/* add this one in to hash table */
    return t;
}


/* SIZETYPE - Find size of type, in words.
**	Note this is words, not bytes such as "sizeof" evaluates to!
** The only "funny" value is that for "char" which is simply 1 (no smaller
** value can be represented).  This requires interpreting the size specially
** when the type is char (see sizeptobj below for an example).
*/
int
sizetype(t)
TYPE *t;
{
    int s;

    if (t == NULL) {			/* izlist => izer0/1 does this, why? */
	error(EINT,"null type to sizetype()");	/* Flush later if never hit */
	return 0;
    }

    /* calculate factor for array dimensions */
    s = 1;				/* nothing multiplied in yet */
    while (t->Tspec == TS_ARRAY) {	/* array has to multiply out ranges */
	s *= t->Tsize;			/* so multiply it in with rest */
	t = t->Tsubt;			/* and go to next in chain */
    }

    /* multiply that by size of base type */
    if (tisbyte(t))		/* Bytes are special case, round up. */
	return (s + (TGSIZ_WORD/tbitsize(t)) - 1) / (TGSIZ_WORD/tbitsize(t));

    switch (t->Tspec) {
	case TS_STRUCT:
	case TS_UNION:				/* If structure or union, */
	    if (t->Tsmtag->Sclass != SC_TAG)	/* make sure it's defined */
		/* Otherwise must be SC_UTAG (undefined) and size is unknown */
		error(EGEN, "Structure \"%s\" undefined, size unknown",
			t->Tsmtag->Sname + 1);	/* Complain */
	    break;

	case TS_PTR:		/* Temporary check installed when changing to
				** new type-size scheme, take out if never hit
				** and just use default.
				*/
	    if (t->Tsize != typsiztab[TS_PTR]) {
		error(EINT,"bad pointer size: %d", t->Tsize);
		return s * typsiztab[TS_PTR];
	    }			/* Drop thru to normal case */
	    break;
    }
    return s * t->Tsize;	/* return final number of words */
}

/* SIZEPTOBJ - Find size of object that a pointer points to.
**	This helps determine how much to increment/decrement pointers by.
** The return value is funny, however.  If the pointer is a normal word
** pointer then the "size" is size in words.  If the pointer is a byte
** pointer then its "size" is expressed in terms of bytes.
*/
int
sizeptobj(t)
TYPE *t;
{
#if 0	/* Old wrong code */
	/* Because the result of sizetype is in terms of words, we need to
	** multiply this to get the # of bytes, UNLESS we are pointing directly at
	** a char (in which case sizetype will have returned 1 and we have to
	** leave this value alone).
	*/
    int size;
    size = sizetype(t->Tsubt);		/* First get size of obj in words */

    /* If this pointer will evaluate to a byte pointer of some kind
    ** then must multiply # words by # bytes per word -- unless
    ** pointing directly at a byte rather than a multidimensional array.
    */
    if (bytepointer(t) && !(tisbyte(t->Tsubt)))
	size *= (TGSIZ_WORD / elembsize(t));	/* Multiply by # bytes/wd */
    return size;
#else	/* New right code */
    return (bytepointer(t)		/* If byte pointer, handle specially */
	? sizearray(t->Tsubt)		/* Bytes: either 1 or # elements */
	: sizetype(t->Tsubt));		/* Words: use # words */
#endif
}

/* SIZEARRAY - returns number of elements in array (1 if not an array).
**	This has nothing to do with the size or type of the elements.
**	This is used to find the # of bytes in a byte array.
*/
int
sizearray(t)
register TYPE *t;
{
    register int s = 1;

    while (t->Tspec == TS_ARRAY) {
	s *= t->Tsize;			/* Multiply size for each index */
	t = t->Tsubt;			/* of the array structure. */
    }
    return s;
}

/* ELEMBSIZE - Returns size in bits of an element (in an array, or pointed to)
**	Value is zero if element is not a scalar object.
*/
int
elembsize(t)
TYPE *t;
{
    if (t->Tspec != TS_PTR && t->Tspec != TS_ARRAY) return 0;
    while (t = t->Tsubt)
	if (t->Tspec != TS_ARRAY)
	    return (tisscalar(t) ? tbitsize(t) : 0);
    error(EINT,"array of nothing");
    return 0;
}

/* CHARARRAY - Return true if type is "array of char".
**	This works for either signed or unsigned chars.
** BYTEARRAY - Similar, true if elements are "bytes" (smaller than words).
*/
int
chararray(t)
TYPE *t;
{
    if (t->Tspec != TS_ARRAY) return 0;
    while (t = t->Tsubt)
	if (t->Tspec != TS_ARRAY)
	    return(tischar(t));
    error(EINT,"array of nothing");
    return 0;
}
int
bytearray(t)
TYPE *t;
{
    if (t->Tspec != TS_ARRAY) return 0;
    while (t = t->Tsubt)
	if (t->Tspec != TS_ARRAY)
	    return tisbyte(t);
    error(EINT,"array of nothing");
    return 0;
}


/* CHARPOINTER - Return true if pointer representation for this type
**	is a pointer to some kind of char.
** BYTEPOINTER - Likewise for bytes.
*/
int
charpointer(t)
TYPE *t;
{
    if (t->Tspec != TS_PTR && t->Tspec != TS_ARRAY) return 0;
    while (1) {
	t = t->Tsubt;
	if (t->Tspec != TS_ARRAY) return tischar(t);
  }
}
int
bytepointer(t)
TYPE *t;
{
    if (t->Tspec != TS_PTR && t->Tspec != TS_ARRAY) return 0;
    while (1) {
	t = t->Tsubt;
	if (t->Tspec != TS_ARRAY) return tisbyte(t);
  }
}

#if 0	/* debug stuff */
shohash()
{
    int n;
    SYMBOL *s;
    for(n=0; n < MAXHSH; n++)
	if(s = htable[n]){
	    printf("Hash %o:", n);
	    do printf(" %o=%s", s, s->Sname);
	    while (s = s->Snhash);
	    printf("\n");
	}
}
#endif
