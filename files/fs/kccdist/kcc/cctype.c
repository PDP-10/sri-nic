/*	CCTYPE.C - Type checking during parsing
**
**	All changes after version 41 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/

#include "cc.h"

/* Imported functions */
extern NODE *defnode(), *deficonst();	/* CCSTMT */
extern TYPE *findtype();		/* CCSYM */


/* Exported functions */
NODE *convcast(), *convbinary(), *convunary(), *convarrfn(),
	*convasgn(), *convfunarg(), *convnullcomb();
#if 0
NODE *undoconv();
#endif
TYPE *convfparam();

/* Internal functions and vars */
static int cast_op();
static void convboth();


#if 0

	How cast conversions and definitions are set up:

CCSYM.H: defines (as enums) all of the cast types entered in convtab[]
	and used in N_CAST parse-tree nodes.
This page: defines all possible cast conversions permitted by C.
Next page: Maps from all possible cast conversions into the ones actually
	supported.

Cast conversions must be supported in 3 places:
	(1) here in CCTYPE, which controls parsing semantics of cast exprs.
	(2) in CCFOLD which does compile-time evaluation of constants.
	(3) in CCGEN2 which generates code for run-time conversions.
There is also CCDUMP (to examine parse trees) which is rarely invoked.

The CONVTAB table, plus cast_op(), does two things.  In addition to
specifying whether a given cast is legal, it also specifies the general
nature of the cast.  The evaluator and generator must decide for themselves
how to best derive explicit actions from both this general hint and the
exact type values provided in the N_CAST node.  This allows the parser
to remain relatively independent of the evaluator/generator.  Note that
whenever an exact and unique value is needed for a specific cast, the
"castidx" macro can be used (e.g. in switch statements) to derive this.

	/* General-purpose casts */
	CAST_NONE,	/* No actual conversion needed */
	CAST_ILL,	/* Illegal conversion, not permitted in C */
	CAST_UNSUPP,	/* Legal but unsupported by code generator */

/* The following definitions list all of the possible cast conversions
** which are legal in C.  They are grouped by target type, just as for
** the type conversion table in CCTYPE.
*/
	/* All conversions to TS_VOID are allowed. */
	CAST_VOID,	/* Value is discarded in all cases. */

	/* No conversions to TS_FUNCT are allowed. */
	/* No conversions to TS_ARRAY are allowed. */
	/* The only conversion to TS_STRUCT is the trivial one. */
	/* The only conversion to TS_UNION  is the trivial one. */
	CAST_TRIV,	/* Permit "conversion" only if types identical */

	/* Conversions to TS_PTR.  Lots of checking involved. */
	/* Pointer subtype conversions.  A pointer may have
	** any subtype except "void".  This means there are 16 different
	** pointer subtypes which may be converted into any of the other
	** 16 subtypes.  We'll let the code generation worry about it
	** and punt with "pointer to pointer".
	** Conversions from void, struct/union, enum or floats are illegal.
	*/
	CAST_FN_PF,	/* Function -> Pointer to Function */
	CAST_AR_PA,	/* Array -> Pointer to 1st Array element */
	CAST_PT_PT,	/* Pointer to a Type -> Pointer to another Type */
	CAST_SB_PT,CAST_UB_PT,				/* Bitfield to ptr */
	CAST_SC_PT,CAST_SS_PT,CAST_SI_PT,CAST_SL_PT,	/* Integers to ptrs */
	CAST_UC_PT,CAST_US_PT,CAST_UI_PT,CAST_UL_PT,

	/* Conversions to TS_ENUM.  All others are illegal. */
	CAST_EN_EN,		/* Enum to Enum.  (may wish to check tags) */
	CAST_SB_EN,CAST_UB_EN,				/* Bitfield to enum */
	CAST_SC_EN,CAST_SS_EN,CAST_SI_EN,CAST_SL_EN,	/* Integers to enums */
	CAST_UC_EN,CAST_US_EN,CAST_UI_EN,CAST_UL_EN,

	/* Conversions to TS_FLOAT. */
		   CAST_DF_FL,CAST_LD_FL,		/* Floats to float */
	CAST_SB_FL,CAST_UB_FL,				/* Bitfield to float */
	CAST_SC_FL,CAST_SS_FL,CAST_SI_FL,CAST_SL_FL,	/* Integers to float */
	CAST_UC_FL,CAST_US_FL,CAST_UI_FL,CAST_UL_FL,

	/* Conversions to TS_DOUBLE. */
	CAST_FL_DF,	      CAST_LD_DF,		/* Floats to double */
	CAST_SB_DF,CAST_UB_DF,				/* Bitfield to dble */
	CAST_SC_DF,CAST_SS_DF,CAST_SI_DF,CAST_SL_DF,	/* Ints to double */
	CAST_UC_DF,CAST_US_DF,CAST_UI_DF,CAST_UL_DF,

	/* Conversions to TS_LNGDBL. */
	CAST_FL_LD,CAST_DF_LD		,		/* Floats to lngdbl */
	CAST_SB_LD,CAST_UB_LD,				/* Bitfield to ldbl */
	CAST_SC_LD,CAST_SS_LD,CAST_SI_LD,CAST_SL_LD,	/* Ints to lngdbl */
	CAST_UC_LD,CAST_US_LD,CAST_UI_LD,CAST_UL_LD,

	/* Conversions to TS_BITF. */
	CAST_PT_SB,CAST_EN_SB,		/* Pointer and Enum to signed bitf */
	CAST_FL_SB,CAST_DF_SB,CAST_LD_SB,		/* Floats to bitf */
	CAST_SB_SB,CAST_UB_SB,				/* Bitfield to bitf */
	CAST_SC_SB,CAST_SS_SB,CAST_SI_SB,CAST_SL_SB,	/* Integers to bitf */
	CAST_UC_SB,CAST_US_SB,CAST_UI_SB,CAST_UL_SB,

	/* Conversions to TS_UBITF. */
	CAST_PT_UB,CAST_EN_UB,		/* Pointer and Enum to signed bitf */
	CAST_FL_UB,CAST_DF_UB,CAST_LD_UB,		/* Floats to bitf */
	CAST_UB_UB,CAST_UB_UB,				/* Bitfield to bitf */
	CAST_SC_UB,CAST_SS_UB,CAST_SI_UB,CAST_SL_UB,	/* Integers to bitf */
	CAST_UC_UB,CAST_US_UB,CAST_UI_UB,CAST_UL_UB,

	/* Conversions to TS_CHAR. */
	CAST_PT_SC,CAST_EN_SC,		/* Pointer and Enum to signed char */
	CAST_FL_SC,CAST_DF_SC,CAST_LD_SC,		/* Floats to char */
	CAST_SB_SC,CAST_UB_SC,				/* Bitfield to char */
		   CAST_SS_SC,CAST_SI_SC,CAST_SL_SC,	/* Integers to char */
	CAST_UC_SC,CAST_US_SC,CAST_UI_SC,CAST_UL_SC,

	/* Conversions to TS_SHORT. */
	CAST_PT_SS,CAST_EN_SS,
	CAST_FL_SS,CAST_DF_SS,CAST_LD_SS,
	CAST_SB_SS,CAST_UB_SS,
	CAST_SC_SS,	      CAST_SI_SS,CAST_SL_SS,
	CAST_UC_SS,CAST_US_SS,CAST_UI_SS,CAST_UL_SS,

	/* Conversions to TS_INT. */
	CAST_PT_SI,CAST_EN_SI,
	CAST_FL_SI,CAST_DF_SI,CAST_LD_SI,
	CAST_SB_SI,CAST_UB_SI,
	CAST_SC_SI,CAST_SS_SI,		 CAST_SL_SI,
	CAST_UC_SI,CAST_US_SI,CAST_UI_SI,CAST_UL_SI,

	/* Conversions to TS_LONG. */
	CAST_PT_SL,CAST_EN_SL,
	CAST_FL_SL,CAST_DF_SL,CAST_LD_SL,
	CAST_SB_SL,CAST_UB_SL,
	CAST_SC_SL,CAST_SS_SL,CAST_SI_SL	   ,
	CAST_UC_SL,CAST_US_SL,CAST_UI_SL,CAST_UL_SL,

	/* Conversions to TS_UCHAR. */
	CAST_PT_UC,CAST_EN_UC,
	CAST_FL_UC,CAST_DF_UC,CAST_LD_UC,
	CAST_SB_UC,CAST_UB_UC,
	CAST_SC_UC,CAST_SS_UC,CAST_SI_UC,CAST_SL_UC,
		   CAST_US_UC,CAST_UI_UC,CAST_UL_UC,

	/* Conversions to TS_USHORT. */
	CAST_PT_US,CAST_EN_US,
	CAST_FL_US,CAST_DF_US,CAST_LD_US,
	CAST_SB_US,CAST_UB_US,
	CAST_SC_US,CAST_SS_US,CAST_SI_US,CAST_SL_US,
	CAST_UC_US,	      CAST_UI_US,CAST_UL_US,

	/* Conversions to TS_UINT. */
	CAST_PT_UI,CAST_EN_UI,
	CAST_FL_UI,CAST_DF_UI,CAST_LD_UI,
	CAST_SB_UI,CAST_UB_UI,
	CAST_SC_UI,CAST_SS_UI,CAST_SI_UI,CAST_SL_UI,
	CAST_UC_UI,CAST_US_UI,		 CAST_UL_UI,

	/* Conversions to TS_ULONG. */
	CAST_PT_UL,CAST_EN_UL,
	CAST_FL_UL,CAST_DF_UL,CAST_LD_UL,
	CAST_SB_UL,CAST_UB_UL,
	CAST_SC_UL,CAST_SS_UL,CAST_SI_UL,CAST_SL_UL,
	CAST_UC_UL,CAST_US_UL,CAST_UI_UL	   ,

};
#endif

/* CONVTAB - Conversion table.
**	This table attempts to be relatively machine independent by only
** indicating the general nature of the conversion required for a particular
** cast.  It is indexed by use of the "castidx" macro.
**
** The value of convtab[castidx(from,to)] will specify the general kind
** of conversion that the code generator will have to apply.
** There are certain special values
** which indicate errors or a need for further checking.
** If the ordering of the TS_ values defined in CCSYM.H is ever changed,
** this table and pconvtab[] needs to be changed too!!
**	Note that most self-to-self integer conversions, such as long to
** long, have CAST_NONE.  However, both bitfields and chars have the general
** CAST_IT_IT type instead, because the size of bitfields can vary, and
** because KCC has a special feature which allows the size of chars to
** also vary.
*/
static int
convtab[TS_MAX*TS_MAX] = {
/* 18 basic types, plus 2 internal (bitfields) */
/* Vo Fn Ar St Un Pt En Fl Db LD SB SC SS SI SL UB UC US UI UL */
/*	Void, Funct, Array,
**	Struct, Union, Pointer, Enum,
**	Float, Double, Long Double,
**	Signed   Bitfield, Char, Short, Int, Long,
**	Unsigned Bitfield, Char, Short, Int, Long
*/

/* Conversions to TS_VOID.  All such conversions are allowed. */
	CAST_VOID,CAST_VOID,CAST_VOID,
	CAST_VOID,CAST_VOID,CAST_VOID,CAST_VOID,
	CAST_VOID,CAST_VOID,CAST_VOID,
	CAST_VOID,CAST_VOID,CAST_VOID,CAST_VOID,CAST_VOID,
	CAST_VOID,CAST_VOID,CAST_VOID,CAST_VOID,CAST_VOID,

/* Conversions to TS_FUNCT.  No such conversion is allowed. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,

/* Conversions to TS_ARRAY.  No such conversion is allowed. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,

/* Conversions to TS_STRUCT.  Only the trivial conversion is allowed. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_TRIV ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,	/* Check it out. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,

/* Conversions to TS_UNION.  Only the trivial conversion is allowed. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_TRIV ,CAST_ILL  ,CAST_ILL  ,	/* Check it out. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,

/* Conversions to TS_PTR.  Lots of checking involved. */
	CAST_ILL  ,CAST_FN_PF,CAST_AR_PA,		/* OK from Fn and Ar */
	CAST_ILL  ,CAST_ILL  ,CAST_PT_PT,CAST_ILL  ,	/* Ptr to ptr */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_IT_PT,CAST_IT_PT,CAST_IT_PT,CAST_IT_PT,CAST_IT_PT,	/* Ints to */
	CAST_IT_PT,CAST_IT_PT,CAST_IT_PT,CAST_IT_PT,CAST_IT_PT,	/* ptrs */

/* Conversions to TS_ENUM.  Only enum and integer source allowed. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_EN_EN,	/* Maybe check tags. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_IT_EN,CAST_IT_EN,CAST_IT_EN,CAST_IT_EN,CAST_IT_EN,	/* Ints to */
	CAST_IT_EN,CAST_IT_EN,CAST_IT_EN,CAST_IT_EN,CAST_IT_EN,	/* enums */

/* Conversions to TS_FLOAT. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_NONE ,CAST_FP_FP,CAST_FP_FP,		/* float to float */
	CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,	/* Ints to */
	CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,	/* float */

/* Conversions to TS_DOUBLE. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_FP_FP,CAST_NONE ,CAST_FP_FP,		/* float to float */
	CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,	/* Ints to */
	CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,	/* float */

/* Conversions to TS_LNGDBL. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_FP_FP,CAST_FP_FP,CAST_NONE ,		/* float to float */
	CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,	/* Ints to */
	CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,CAST_IT_FP,	/* float */

/* Conversions to TS_BITF. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* ints */

/* Conversions to TS_CHAR. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* ints */

/* Conversions to TS_SHORT. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_NONE ,CAST_IT_IT,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* ints */

/* Conversions to TS_INT. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_NONE ,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* ints */

/* Conversions to TS_LONG. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_NONE ,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* ints */

/* Conversions to TS_UBITF. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* ints */

/* Conversions to TS_UCHAR. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* ints */

/* Conversions to TS_USHORT. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_NONE ,CAST_IT_IT,CAST_IT_IT,	/* ints */

/* Conversions to TS_UINT. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_NONE ,CAST_IT_IT,	/* ints */

/* Conversions to TS_ULONG. */
	CAST_ILL  ,CAST_ILL  ,CAST_ILL  ,
	CAST_ILL  ,CAST_ILL  ,CAST_PT_IT,CAST_EN_IT,
	CAST_FP_IT,CAST_FP_IT,CAST_FP_IT,		/* Floats to integer */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,	/* Ints to */
	CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_IT_IT,CAST_NONE ,	/* ints */
};

/* Pointer representation conversion table
**	A C pointer can have several different representations or formats
** depending on what it is pointing to.  We handle this by defining a
** PTRREP_ value for each possible format, and assuming that
** all possible formats are encompassed by pointers to each of the
** possible TS_ values.  This allows us to quickly determine pointer
** format and identify any conversions that are necessary.
**
** PCONVTAB is indexed by the first subtype T (as in "pointer to T").
**	The exception is "pointer to array of T" which becomes
**	"pointer to T" iteratively.
*/

#if 0	/* Commented out for now */

enum ptrspec {
	PTRREP_WD,	/* Pointer to word */
	PTRREP_CH,	/* Pointer to normal char */
	PTRREP_MAX,	/* Max # of pointer rep types */
	PTRREP_CH7,	/* Pointer to 7-bit char */
	PTRREP_ILL,	/* Illegal */
};

static ptyptab[20] = {
 /* 18 basic types */
/* Vo Fn Ar St Un Pt En Fl Db SB SC SS SI SL UB UC US UI UL */
	PTRREP_ILL,	/* (void *) illegal at moment, until ANSI comes */
	PTRREP_WD,	/* function addr */
	PTRREP_ILL,	/* No ptrs to array - should be caught earlier */
	PTRREP_WD,	/* (struct foo *) */
	PTRREP_WD,	/* (union foo *) */
	PTRREP_WD,	/* (type **) */
	PTRREP_WD,	/* (enum foo *) */
	PTRREP_WD,	/* (float *) */
	PTRREP_WD,	/* (double *) */
	PTRREP_WD,	/* (long double *) */
	PTRREP_CH,	/* (bitfield *) */
	PTRREP_CH,	/* (char *) */
	PTRREP_WD,	/* (short *) */
	PTRREP_WD,	/* (int *) */
	PTRREP_WD,	/* (long *) */
	PTRREP_CH,	/* (unsigned bitfield *) */
	PTRREP_CH,	/* (unsigned char *) */
	PTRREP_WD,	/* (unsigned short *) */
	PTRREP_WD,	/* (unsigned int *) */
	PTRREP_WD,	/* (unsigned long *) */
};

/* Pointer conversion table, [to][from] */
static pconvtab[PTRREP_MAX][PTRREP_MAX] = {
	/* To word ptr */
	CAST_NONE,		/* From word ptr */
	CAST_PC_PI,		/* From char ptr */

	/* To char ptr */
	CAST_PI_PC,		/* From word ptr */
	CAST_NONE,		/* From char ptr */
};
#endif

/* CAST_OP	Find CAST_ conversion type to use.	[H&S 6.2 - 6.9 incl]
**	This is the only routine that references the conversion table.
*/
static int
cast_op(to, frm)
TYPE *to, *frm;
{
    int cop;

    switch (cop = convtab[castidx(frm->Tspec,to->Tspec)]) {
	case CAST_TRIV:		/* Verify that types are identical */
	    if (to == frm)	/* This should have been caught before call */
		return CAST_NONE;
	    cop = CAST_ILL;	/* Otherwise drop through to complain */
	case CAST_ILL:
	    error(EGEN,"Illegal conversion - %s to %s",
		tsnames[frm->Tspec], tsnames[to->Tspec]);
	    break;
	case CAST_VOID:		/* Discard -- converting to void */
	case CAST_NONE:		/* No actual conversion operation needed */
	    break;

	/* Check for conversions that should have been caught before this.
	*/
	case CAST_AR_PA:	/* Array to T should be ptr to T */
	case CAST_FN_PF:	/* Function should be ptr to function */
	    error(EINT,"unexpected cast - %s to %s",
		tsnames[frm->Tspec], tsnames[to->Tspec]);
	    cop = CAST_ILL;
	    break;

	case CAST_PT_PT:	/* OK as is.  Someday convert into more */
	    break;		/* specific type of conversion? */

	/* Integer to integer conversion.
	**	May be able to detect that no actual conversion is needed.
	**	This code assumes that signed and unsigned types occupy the
	**	same amount of space, and that there is no representation
	**	change when converting between the two!  This will be true
	**	for just about all computers.
	*/
	case CAST_IT_IT:
	    if ((tbitsize(frm) == tbitsize(to))		/* If same size */
		/* && tisunsigned(frm) == tisunsigned(to) *//* & signedness? */
		)
		return CAST_NONE;
	    break;
    }
    return cop;
}

/* CONVCAST - Apply "cast conversions" to an expression.  [H&S 6.10]
**	Note this ought to always force a cast to the indicated type, even
** if it may not be necessary.  Optimization can later remove this.
** For now, easier to debug if just duplicate old behavior.
*/
NODE *
convcast(t,n)
NODE *n;
TYPE *t;
{
    int op;

    if (n->Ntype->Tspec == TS_FUNCT || n->Ntype->Tspec == TS_ARRAY)
	n = convarrfn(n);		/* Pre-convert funct/arrays here */

    if (t == n->Ntype)			/* May now be a trivial cast */
	return n;
    op = cast_op(t, n->Ntype);		/* find out which cast to use */
    if (op == CAST_PT_PT && clevkcc	/* Special hack for KCC extension */
	&& n->Nop == N_SCONST		/* If casting string literal */
	&& tischar(t->Tsubt)) {		/* to another kind of char ptr */
	n->Ntype = t;			/* then just change type of const! */
	return n;			/* And code gen will win. */
    }
    n = defnode(N2, N_CAST, t, 0, n);	/* make cast */
    n->Ncast = op;			/* of given type */
    return n;				/* and return with the node */
}

/* CONVASGN - Apply "assignment conversions" to an expression.  [H&S 6.11]
**	Takes two args, a pointer to the type of the left-hand op
** (destination type) and a node pointer to the right-hand expression
** which needs to have its type checked and perhaps fixed up.
*/
NODE *
convasgn(lt,n)
TYPE *lt;		/* Left (destination) type */
NODE *n;
{
    TYPE *rt;		/* Right (source) type */
    NODE *cn;		/* New cast node if any */

    /* If same type, OK */
    if (lt == n->Ntype) return n;
    rt = n->Ntype;

    /* Not same type.  Check to see if it's OK to silently convert the
    ** right-hand type to left-hand type.  If so, call convcast() to do it.
    */
    if (				/* OK to convert: */
       (tisarith(lt) && tisarith(rt))	/* Any arith type to any arith type */
    || (lt->Tspec == TS_PTR
	&& (  niszero(n)		/* Integer 0 to any ptr type */
	   || (rt->Tspec == TS_ARRAY 	/* "array of T" to "ptr to T" */
	      && rt->Tsubt == lt->Tsubt)
	   || (rt->Tspec == TS_FUNCT	/* "function" to "ptr to function" */
	      && lt->Tsubt->Tspec == TS_FUNCT)
	   )
	)
    )
	return convcast(lt, n);		/* OK, do the specified conversion */

    /* Technically nothing else is OK.  But we try to do it anyway
    ** and print a warning message.  If the attempted conversion is illegal
    ** then cast_op will complain.
    */
    cn = convcast(lt, n);		/* Attempt to force the cast */
    if (cn != n && cn->Nop == N_CAST	/* If cast was done, print warning */
      && cn->Ncast != CAST_ILL)		/* unless cast_op already barfed. */
	warn(EGEN,"Implicit conversion - %s to %s",
		tsnames[rt->Tspec], tsnames[lt->Tspec]);
    return cn;
}

/* CONVARRFN - Apply array/function conversions to an expression node.
**	Returns pointer to a converted expression (usually the same node)
*/
NODE *
convarrfn(n)
NODE *n;
{
    switch (n->Ntype->Tspec) {
	default: break;		/* Not array or function */

	case TS_ARRAY:		/* Convert "array of T" to "pointer to T" */
	    if (!(n->Nflag & NF_GLOBAL)) { /* If obj has local extent */
		stackrefs++;		/* then count it as a */
		n->Nflag |= NF_STKREF;	/* stack reference */
	    }

	    /* For array name, or array subscripting, just set the type.
	    ** Anything else requires using & to get the address.
	    ** That can only be N_PTR, Q_DOT, or Q_MEMBER.
	    */
	    if (n->Nop == Q_IDENT || n->Nop == Q_PLUS)
		n->Ntype = findtype(TS_PTR, n->Ntype->Tsubt);
	    else
		return defnode(N2, N_ADDR, findtype(TS_PTR, n->Ntype->Tsubt),
				 n->Nflag &~ NF_LVALUE, n);
	    break;

	case TS_FUNCT:	/* Convert "function of T" to "pointer to fun of T" */
	    if (n->Nop == Q_IDENT)	/* For function name, just set type */
		n->Ntype = findtype(TS_PTR, n->Ntype);
	    else
		return defnode(N2, N_ADDR,
			findtype(TS_PTR, n->Ntype), 0, n);
    }
    return n;
}

/* CONVUNARY - Apply "usual unary conversions" to a node. [H&S 6.12]
**	Returns pointer to a converted expression (usually the same node)
**	Note that for ANSI there are two differences:
**		(1) the float -> double conversion is suppressed!
**		(2) integer promotions preserve value, not unsignedness.
**			For the PDP-10 this means chars/shorts always
**			are converted to "int", as are bitfields less
**			than the length of an int.
*/
NODE *
convunary(n)
NODE *n;
{
    int castop;
    TYPE *newtype;

    switch (n->Ntype->Tspec) {
	default: return n;	/* Usual case, no conversion needed */

	case TS_BITF:
	case TS_CHAR:
	case TS_SHORT:		/* Signed bitfield, char and short */
		newtype = inttype;	/* always become signed int */
		break;

	case TS_UBITF:
	case TS_UCHAR:
	case TS_USHORT:		/* But if unsigned, depends on ANSIness */
	    if ((clevel >= CLEV_ANSI)			/* If ANSI, */
		&& (TGSIZ_INT > tbitsize(n->Ntype)))	/* check size */
		newtype = inttype;	/* ANSI and int is big enough */
	    else newtype = uinttype;	/* Not ANSI, or not big enough */
	    break;

	case TS_FLOAT:			/* If ANSI, leave float alone */
	    if (clevel >= CLEV_ANSI)
		return n;
	    newtype = dbltype;			/* Float to double */
	    break;

	case TS_ARRAY:	/* Convert "array of T" to "pointer to T" */
	case TS_FUNCT:	/* Convert "function of T" to "pointer to fun of T" */
	    return convarrfn(n);
    }

    /* Apply some simple cast operation */
    castop = cast_op(newtype, n->Ntype);
    n = defnode(N2, N_CAST, newtype, 0, n);
    n->Ncast = castop;			/* Specify the conversion */

    return n;
}

/* CONVBINARY - Apply "usual binary conversions" to a node. [H&S 6.13]
**	Takes pointer to a binary operator node, with the operands
** pointed to by the left and right links.
** Returns pointer to a converted expression (usually the same node)
**	Note that normally a float-float rule exists ("2a" in H&S 6.13),
** as ANSI requires this.
*/
NODE *
convbinary(n)
NODE *n;
{
    TYPE  *lt, *rt;

    n->Nleft = convunary(n->Nleft);
    n->Nright = convunary(n->Nright);
    n->Ntype = n->Nleft->Ntype;		/* Set type in case of quick return */

    if (n->Nleft->Ntype == n->Nright->Ntype) return n;
    lt = n->Nleft->Ntype;
    rt = n->Nright->Ntype;

    /* 1. If either operand is not arith type, do no additional conversion */
    if (!tisarith(lt) || !tisarith(rt)) return n;

    /* 2x. If one operand is "long double", make both "long double". */
    if (lt == lngdbltype || rt == lngdbltype)
	convboth(n, lngdbltype);

    /* 2. If one operand is "double", make both "double". */
    else if (lt == dbltype || rt == dbltype)
	convboth(n, dbltype);

    /* 2a. If one operand is "float", make both "float".
    ** Need not do clevel check here as convunary already did that;
    ** i.e. if clevel isn't ANSI then floats would be doubles by now.
    */
    else if (lt == flttype || rt == flttype)
	convboth(n, flttype);

    /* 3.  If one operand is "unsigned long", make both "unsigned long" */
    else if (lt == ulongtype || rt == ulongtype)
	convboth(n, ulongtype);

    /* 4+5. CARM only: If one operand is "long" and other is "unsigned int",
    ** make both "unsigned long".
    **  ANSI makes both "long".
    */
    else if (lt == longtype || rt == longtype) {
	if ((clevel < CLEV_ANSI)
		&& (lt == uinttype || rt == uinttype))
	    convboth(n, ulongtype);
	else convboth(n, longtype);
    }

    /* 6.  If one operand is "unsigned", then both are made "unsigned" */
    else if (lt == uinttype || rt == uinttype)
	convboth(n, uinttype);

    /* Else should be int, and should have caught the equivalence earlier! */
    else error(EINT,"operand types escaped convbinary!");

    n->Ntype = n->Nleft->Ntype;		/* Result type is the common type */
    return n;
}

static void
convboth(n, newtyp)
NODE *n;
TYPE *newtyp;
{
    NODE *retn;

    if (n->Nleft->Ntype != newtyp) {
	retn = defnode(N2, N_CAST, newtyp, 0, n->Nleft);
	retn->Ncast = cast_op(newtyp, n->Nleft->Ntype);
	n->Nleft = retn;
    }
    if (n->Nright->Ntype != newtyp) {
	retn = defnode(N2, N_CAST, newtyp, 0, n->Nright);
	retn->Ncast = cast_op(newtyp, n->Nright->Ntype);
	n->Nright = retn;
    }
}

/* CONVFUNARG - Apply the "function argument conversions" to a node.
**	[H&S 6.14]
**	Note that a float argument is always converted to a double,
** regardless of whether this conversion is done for unary or binary
** expressions.
*/
NODE *
convfunarg(n)
NODE *n;
{
    switch (n->Ntype->Tspec) {
	case TS_VOID:
	    error(EGEN,"Illegal use of void type - function arg");
	    return deficonst(0);

	case TS_FLOAT:			/* Cast float to double */
	    n = defnode(N2, N_CAST, dbltype, 0, n);
	    n->Ncast = cast_op(dbltype, n->Nleft->Ntype);
	    return n;
    }
    return convunary(n);
}

/* CONVFPARAM - Apply the "function parameter promotions" to a type.
**	[H&S 9.4]  No explicit casts are possible at this point; we
** simply set the variable type to what actually will be passed as an argument.
**	This routine is called by CCDECL when parsing a function declaration
** and is unlike the others in that it returns a TYPE * pointer
** rather than a NODE * pointer.
**	Note the difference between ANSI and CARM: ANSI preserves value
** whereas CARM preserves unsignedness.  This has the effect of requiring
** ANSI level to return "int" for both signed and unsigned chars/shorts,
** whereas CARM level returns "unsigned int" if the original type was unsigned.
*/
TYPE *
convfparam(t)
TYPE *t;		/* Type of function parameter to check */
{
    switch (t->Tspec) {	/* Apply funct param convs */
	case TS_VOID:
	    error(EGEN,"Function parameter cannot have type void");
	    break;
	case TS_FUNCT:
	    error(EGEN,"Function parameter cannot have type function");
	    break;

	case TS_ARRAY:		/* Array arg becomes ptr */
	    return findtype(TS_PTR, t->Tsubt);
	case TS_STRUCT:
	case TS_UNION:
	case TS_PTR:
	case TS_ENUM:
	    break;

	case TS_FLOAT:		/* float -> double, always. */
	    return dbltype;
	case TS_DOUBLE:
	case TS_LNGDBL:
	    break;

	case TS_BITF:		/* bitfield -> int */
	case TS_UBITF:		/* Ditto for unsigned bitf */
	    error(EINT,"Function parameter has bitfield type");
	    break;

	case TS_CHAR:		/* char -> int */
	case TS_UCHAR:		/* and unsigned char */
	    if ((TGSIZ_WORD % tbitsize(t)) != 0)
		error(EGEN,"Function param cannot have char size %d",
				tbitsize(t));
	    if (clevel >= CLEV_ANSI)	/* If ANSI, */
		return inttype;		/* chars always promoted to int */
	    return (t->Tspec == TS_CHAR ? inttype : uinttype);

	case TS_SHORT:		/* short -> int */
	    return inttype;
	case TS_USHORT:		/* unsigned short must check for ANSIness */
	    if (clevel >= CLEV_ANSI)
		return inttype;
	    return uinttype;

	case TS_INT:
	case TS_LONG:
	case TS_UINT:
	case TS_ULONG:
	    break;

	default:
	    error(EINT, "unknown funct param typespec: %d", t->Tspec);
    }
    return t;
}

/* Auxiliary miscellaneous routines */

/* CONVNULLCOMB - Converts combination of pointer and NULL if present,
**	else does nothing.  Takes a binary op node with left and right
**	expressions already crunched through binary conversions.
*/
NODE *
convnullcomb(n)
NODE *n;
{
    if (n->Nleft->Ntype->Tspec == TS_PTR && niszero(n->Nright))
	n->Nright = convcast(n->Nleft->Ntype, n->Nright);
    else if (n->Nright->Ntype->Tspec == TS_PTR && niszero(n->Nleft))
	n->Nleft = convcast(n->Nright->Ntype, n->Nleft);
    return n;
}

#if 0
/* UNDOCONV -  Apply hair to undo the automatic immediate conversion of a
**	function name to a pointer-to-function.  This is painful, but
**	believe me it's easier than the alternatives!
**	Used by sizeof and function call parsing.
*/
NODE *
undoconv(n)
NODE *n;
{
    NODE *top, *np;

    top = NULL;
    if (n->Nop == N_EXPRLIST)	/* Find root expression if a seq expr */
	for (top = n; n->Nop == N_EXPRLIST; n = n->Nright) ;

    if (n->Nop == Q_IDENT) switch (n->Nid->Stype->Tspec) {
	case TS_FUNCT:
	case TS_ARRAY:
	    n->Ntype = n->Nid->Stype;	/* Restore original type */
	    if (top) {			/* Fix up expr list if one */
		for(np = top; np->Nop == N_EXPRLIST; np = np->Nright)
		    np->Ntype = n->Ntype;
		return top;
	    }
    }
    return n;
}
#endif