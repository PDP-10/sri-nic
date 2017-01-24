/*
** ccsym.h - Declarations for KCC symbol table structures
** Split from cc.h / David Eppstein / 23 May 85
*/

#ifndef EXT
#define EXT extern
#endif

#define	_IDSIZE	20			/* identifier size */
#define MAXHSH	8192			/* hashtable size (2^N) */

typedef	char id[_IDSIZE];

struct TY {				/* type table entry   */
    int ttype;				/* type, e.g. INT or ARRAY */
    int tsize;				/* size or number of elts */
    union {
	struct TY *tsubtype;		/* what it's a pointer to */
	struct SMEM *tsmem;		/* or list of structure elements */
	struct SYMBOL *tsym;		/* name of struct or union */
    } tvar;
    struct TY *trehash;			/* pointer to next in hash chain */
};

struct SYMBOL {				/* symbol table entry */
    id sname;				/* symbol name */
    int sclass;				/* storage class */
    union {
	int stok;			/* symbol value or rw token */
	struct SYMBOL *ssym;		/* symbol for label,static */
    } sval;
    union {
	struct TY *stype;		/* type pointer */
	int srwkey;			/* reserved word key */
	char *smacptr;			/* macro pointer */
	struct SMEM *ssmem;		/* struct member list */
    } svar;
    struct SYMBOL *sprev;		/* ptr to prev. symbol */
    struct SYMBOL *snext;		/* ptr to next symbol */
};

/*
** These used to be simple fields in the structures.
** Define them here so I don't have to find all references
** to them throughout KCC.
*/

#define stoken sval.stok
#define svalue stoken
#define sptr svar.stype
#define skey svar.srwkey
#define smptr svar.smacptr
#define tptr tvar.tsubtype

/* storage classes */

#define	SUNDEF    0	/* undefined */
#define	SRW	  1	/* reserved word */
#define	SMACRO	  2	/* macro */
#define	STAG	  3	/* structure tag */
#define	STYPEDEF  4	/* typedef */
#define	SEXTERN   5	/* extern */
#define	SSTATIC	  6	/* static */
#define	SARG	  7	/* formal argument */
#define	SREGISTER 8	/* register */
#define SMEMBER   9	/* struct member */
#define SAUTO	  10	/* auto variables */
#define SLABEL	  11	/* goto label */
#define SISTATIC  12	/* internal static */
#define	SENUM	  13	/* enumerated type */
#define SULABEL	  14	/* undefined goto label */
#define SSCOPE	  15	/* local to block which we have done with */
#define SILABEL	  16	/* internal (i.e. $xxx) label */
#define SSSTATIC  17	/* scoped SISTATIC */

/*
** Structure member information
**
** If a type has ttype STRUCT or UNION, it's tptr will point to a struct SMEM
** if it is an unnamed struct, or to the symbol for its name if it has one
** (in which case the tsize of the type will be < 0, and the sptr of the
** name symbol points to the struct SMEM for that struct).
**
** This gives a list of the members of that struct, in order of definition,
** with name, offset from structure start, and data type of each member.
*/

struct SMEM {				/* structure member links */
    struct SYMBOL *smem;		/* the name (points to type etc) */
    struct SMEM *smnext;		/* the next member of the struct */
    int smoffset;			/* offset in case not global */
    struct TY *smtype;			/* type in case not global */
};

#define isstruct(t) ((t)->ttype == STRUCT || (t)->ttype == UNION)
#define namedstruct(t) ((t)->tsize < 0)
#define SMEMS(t) (isstruct(t)? structSMEMS(t) : (struct SMEM *) NULL)
#define structSMEMS(t) (namedstruct(t)? namedSMEMS(t) : typeSMEMS(t))
#define namedSMEMS(t) ((t)->tvar.tsym->svar.ssmem)
#define typeSMEMS(t) ((t)->tvar.tsmem)

#define AMBIGMEM (-07777)		/* impossible P/S combo */
					/* to denote non-global offset */

#define	MAXSMEM	2000			/* number structure members */

/*
** Storage
** Most of this is still in cc.h, but some files that
** don't include that (like ccopt.c) want some of these variables.
*/

EXT struct SMEM smems[MAXSMEM];		/* structure member links */
EXT struct SYMBOL bytsym;		/* symbol for $BYTE */

EXT int
    maxlabel,				/* maximum label */
    spushes,				/* used $SPUSH, $SPOP */
    bytsim,				/* used $ADJBP, $BPCNT or $SUBBP */
    fltsim;				/* used $DFIX, $DFLOT */

EXT struct SYMBOL *fllist, *flprev;	/* free label list */
