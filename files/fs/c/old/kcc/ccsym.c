/* <KCC.CC>CCSYM.C.36, 25-Jun-85 16:17:05, Edit by KRONJ */
/*  Avoid rehash problems after symbol flush */
/* <KCC.CC>CCSYM.C.32, 29-May-85 10:29:15, Edit by KRONJ */
/*  Init free label list */
/* <KCC.CC>CCSYM.C.31, 19-Mar-85 11:41:12, Edit by SATZ */
/* Make sure function name gets output for symbol dump */
/* <KCC.CC>CCSYM.C.22,  9-Mar-85 10:13:58, Edit by KRONJ */
/*  Reinstate SSCOPE; scopeauto() was releasing symbols */
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

char *malloc();
struct SYMBOL *getsym();
struct TY *findtype();

/* ------------------------------------------------ */
/*	symbol table related routines	   	    */
/*						    */
/*	symbol table implemented as a linked list   */
/*	of symbol table entries. Global symbols     */
/*	are in the list symbol and local symbols    */
/*	are in the list locsymbol.		    */
/*						    */
/*	Symbols (including reserved words and	    */
/*	macros) are hashed through a hash 	    */
/*	table.					    */
/*						    */
/*	Symbol types have a table and hash	    */
/*	table of its own.			    */
/* ------------------------------------------------ */


/* ------------------------- */
/*	initializations      */
/* ------------------------- */

initsym()
{
    int i;
    struct RW *r;
    struct TNAME *tn;
    struct SYMBOL *s, *creatsym();
    struct TY *findtype();

    /* initialize symbol, type and hash tables */

    maxlabel = 0;
    symbol = symtail =  (struct SYMBOL *) malloc(sizeof(struct SYMBOL));
    locsymbol = loctail = (struct SYMBOL *) malloc(sizeof(struct SYMBOL));
    symbol->snext = symbol->sprev = NULL;
    fllist = flprev = locsymbol->snext = locsymbol->sprev = NULL;

    /* clear out hash tables */
    for (i = 0 ; i < MAXHSH ; i++) htable[i] = NULL;
    for (i = 0 ; i < TYPES ; i++)  ttable[i] = NULL;

    /* define built in typedefs */
    maxtype = 0;
    tn = tnames;
    while (tn->tnname != NULL) {
	s = creatsym (tn->tnname);
	s->sclass = STYPEDEF;
	s->sptr = findtype (tn->tntype, tn->tnsize, NULL);
	tn++;
    }
    deftype = findtype (INT, INTSIZE, NULL);
    chartype = findtype (CHAR, CHARSIZE, NULL);
    strtype = findtype (PTR, 0, chartype);
    flttype = findtype (FLOAT, FLOATSIZE, NULL);
    dbltype = findtype (DOUBLE, DBLESIZE, NULL);

    /* enter reserved words in symbol table */
    r = rw;	
    while (r->n != NULL) {
	s = creatsym(r->n);
	s->sclass = SRW;
	s->stoken = r->t;
	s->skey = r->k;
	r++;
    }

    /* enter special macro definitions into symbol table */
    s = creatsym("__LINE__");
    s->sclass = SMACRO;
    s->smptr = (char *) -1;	/* character string of line number */
    s->svalue = 0;
    s = creatsym("__FILE__");
    s->sclass = SMACRO;
    s->smptr = (char *) -2;	/* point to compiling file name */
    s->svalue = 0;

    /* make symbol for byte pointer constructor table */
    strcpy(bytsym.sname, "$BYTE");	/* make symbol for pitopc() */
    bytsym.svalue = 0;			/* no usages of it yet */
    minsym = symtail;
}

/* ------------------------------------------ */
/*	find pointer to the symbol table      */
/* ------------------------------------------ */

struct SYMBOL *findsym (s)
char *s;
{
    int h;
    struct SYMBOL *sym, *retsym;

    retsym = NULL;			/* note we want to return last match */
    for (h = hash(s); (sym = htable[h]) != NULL; h = (h + 1) & (MAXHSH - 1))
	if (symcmp(sym->sname, s) && sym->sclass != SSCOPE &&
	    sym->sclass != SSSTATIC) retsym = sym;
    return retsym;
}
/* ------------------------------------------ */
/*	find pointer to a specific symbol     */
/* ------------------------------------------ */

struct SYMBOL *
findasym (s, class)
char *s;
int class;
{
    int h;
    struct SYMBOL *sym;

    /* this is only called with class=MACRO so safe to return first match */
    for (h = hash(s); (sym = htable[h]) != NULL; h = (h + 1) & (MAXHSH - 1))
	if (sym->sclass == class && symcmp(sym->sname, s)) break;
    return sym;
}

/* -------------------------------------- */
/*	free a symbol table location      */
/* -------------------------------------- */

freesym(s)
struct SYMBOL *s;
{
    int h, e;

    for (h = hash (s->sname); htable[h] != s; h = (h + 1) & (MAXHSH - 1)) ;
    while (htable[e = (h + 1)&(MAXHSH - 1)] != NULL && /* if sym immed after */
	   hash (htable[e]->sname) != e) { /* is shifted from initial hash */
	htable[h] = htable[e];		/* unshift so it doesn't disappear */
	h = e;				/* continue at this point in table */
    }
    htable[h] = NULL;			/* drop final slot */
    retsym(s);				/* give sym back to mem allocator */
}

/* -------------------------------------- */
/*	create an entry for a symbol      */
/* -------------------------------------- */

struct SYMBOL *creatsym (s)
char *s;
{
    struct SYMBOL *t;
    int    n;

    n = findhash(s);
    t = htable[n] = getsym(&symtail);
    symcpy(t->sname, s);
    t->sptr = NULL;
    t->svalue = 0;
    t->sclass = SUNDEF;
    return t;
}


/* -------------------------------------------- */
/*	create an entry for a local symbol      */
/* -------------------------------------------- */

struct SYMBOL *creatloc (s)
char *s;
{
    struct SYMBOL *t;
    int    n;

    n = findhash(s);
    t = htable[n] = getsym(&loctail);
    symcpy(t->sname, s);
    t->sptr = NULL;
    t->svalue = 0;
    t->sclass = SUNDEF;
    return t;
}

/* ------------------------------------- */
/*	find unused hash table slot      */
/* ------------------------------------- */

findhash(s)
char *s;
{
  int h;
  h = hash(s);
  while (htable[h] != NULL) {   /* find empty hash slot */
      h = (h+1) & (MAXHSH-1);
  }
  return h;
}

/* ------------------------------------ */
/*	compute hash (MAXHSH = 2^N)     */
/* ------------------------------------ */

hash (s)
char *s;
{
  int i,j, count;
  
  i = 0;
  count = 1;
  while ((j = *s++) && count++ <= _IDSIZE) i = ((i+i) + j);
  return (i & (MAXHSH-1));
}


/* ---------------------------- */
/*	cobble up a symbol	*/
/*	entry			*/
/* ---------------------------- */

struct SYMBOL *getsym(tailptr)
struct SYMBOL **tailptr;
{
    struct SYMBOL *newptr;

    newptr = (struct SYMBOL *) malloc(sizeof(struct SYMBOL));
    if (newptr == (struct SYMBOL *) NULL)
	fatal(EOUTMEM);
    newptr->sprev = *tailptr;
    newptr->snext = (struct SYMBOL *) NULL;
    (*tailptr)->snext = newptr;
    *tailptr = newptr;
    return newptr;
}

/* ---------------------------- */
/*	return a symbol table	*/
/*	entry			*/
/* ---------------------------- */

retsym(syment)
struct SYMBOL *syment;
{
    if (syment == symtail)
	symtail = syment->sprev;
    else
	if (syment == loctail)
	    loctail = syment->sprev;
    if (syment->sprev)
	syment->sprev->snext = syment->snext;
    if (syment->snext)
	syment->snext->sprev = syment->sprev;
    free((char *)syment);
}
	
/* ------------------------------------ */
/*	copy symbol representation      */
/* ------------------------------------ */

copysym(s,t)
struct SYMBOL *s, *t;
{
  symcpy(s->sname, t->sname);
  s->sclass = t->sclass;
  s->svalue = t->svalue;
  s->sptr = t->sptr;
}

/* -------------------------------------- */
/*	compare symbols for equality      */
/* -------------------------------------- */

symcmp(s,t)
char *s, *t;
{
  int i;

  i = 1;
  while (*s == *t++) {
      if (!(*s++) || i++ >= _IDSIZE) return 1;
  }
  return 0;
}

/* ---------------------- */
/*	copy symbols      */
/* ---------------------- */

symcpy(s, t)
char *s, *t;
{
  int i;

  i = 1;
  while (*s++ = *t++) {
      if (i++ >= _IDSIZE) return;
  }
}

/* ------------------------ */
/*	append symbols      */
/* ------------------------ */

symapp(s, t)
char *s, *t;
{
  int i;

  i = 1;
  while (*s) {
      s++;
      i++;
  }
  while (*s++ = *t++) {
      if (i++ >= _IDSIZE) return;
  }
}

/* ----------------------------- */
/*	flush auto variables     */
/* ----------------------------- */

ridauto()
{
    struct SYMBOL *s, *prev;

    if (dump && locsymbol->snext) symdump(locsymbol->snext, curfn->sname);
    for (s = loctail; s != locsymbol; s = prev) {
	switch (s->sclass) {
	case SULABEL:
	    error (EULABEL, s->sname+1); /* undef lab? (fall through) */
	case SISTATIC:
	case SSSTATIC:
	case SLABEL:
	    freelabel (s->sval.ssym);	/* flush no longer useful lab */
	}
	prev = s->sprev;
	freesym (s);
    }
}

/* ----------------------------------------------- */
/*	undefine auto variables for a subblock     */
/* ----------------------------------------------- */

scopeauto(tailptr)
struct SYMBOL *tailptr;
{
    while ((tailptr = tailptr->snext) != NULL)
	if (tailptr->sclass == SAUTO)
	    tailptr->sclass = SSCOPE;
	else if (tailptr->sclass == SISTATIC)
	    tailptr->sclass = SSSTATIC;
}

/* ------------------------------------------------------ */
/*	return pointer to type, create if neccessary      */
/* ------------------------------------------------------ */

struct TY *
findtype(typ, siz, ptr)
struct TY *ptr;
{
    struct TY *t;
    int hash;

    /* hash up attributes of this type and look up in table */
    hash = ( ((int) ptr) + (typ * 43) + (siz * 101) ) % THASHSIZE;
    for (t = ttable[hash]; t != NULL; t = t->trehash)
	if (t->ttype == typ && t->tsize == siz && t->tptr == ptr)
	    return t;

    /* not found, have to make up a new one */
    t = &types[maxtype++];		/* someday this should be a malloc() */
    t->ttype = typ;
    t->tsize = siz;
    t->tptr = ptr;
    t->trehash = ttable[hash];		/* link old types with same hash */
    ttable[hash] = t;			/* add this one in to hash table */
    return t;
}

/* ------------------------------ */
/*	find underlying type      */
/* ------------------------------ */

stype(t)
struct TY *t;
{
  struct TY *u, *v;

  u = t;
  while ((v=u->tptr) != NULL) u = v;
  return u->ttype;  
}

/* -------------------------------------------- */
/*	return if type is an array of char      */
/* -------------------------------------------- */

chararray(t)
struct TY *t;
{
  if (t->ttype != ARRAY) return 0;
  while (1) {
      t = t->tptr;
      if (t->ttype != ARRAY) return (t->ttype == CHAR);
  }
}


/* --------------------------------------------- */
/*	return if type is a pointer of char      */
/* --------------------------------------------- */

charpointer(t)
struct TY *t;
{
  if (t->ttype != PTR && t->ttype != ARRAY) return 0;
  while (1) {
      t = t->tptr;
      if (t->ttype != ARRAY) return (t->ttype == CHAR);
  }
}
