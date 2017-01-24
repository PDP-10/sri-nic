/*	CCSYM.H - Declarations for KCC type and symbol table structures
**
**	All changes after version 8 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
**
**	Split from cc.h / David Eppstein / 23 May 85
*/

#ifndef EXT
#define EXT extern
#endif

/* Must define these together here since they need each other */
#define SYMBOL struct _symbol
#define TYPE struct _type

/* SYMBOL - Symbol table entry
**	All symbols have a name (Sname) and a "symbol class" (Sclass) which
**	determines the meaning of the rest of the structure.
*/
SYMBOL {
  struct {
    int s_class;		/* Symbol class - a SC_ value */
    int s_flags;		/* Symbol flags (SF_) */
    char s_name[_IDSIZE];	/* Symbol identifier string */
    union {		/* Use depends on Sclass */
	int s_int;		/* symbol value, misc uses */
	SYMBOL *s_sym;		/* symbol for label,static */
    } s_val;
    union {		/* Use depends on Sclass */
	TYPE *s_type;		/* C type pointer */
	int s_rwkey;		/* reserved word key */
	char *s_macptr;		/* macro pointer */
    } s_var;
  } Scontents;		/* Copyable contents (for struct assigns) */

			/* Only used for SC_TAG and SC_MEMBER */
    SYMBOL *Ssmnext;		/* next structure member */
    SYMBOL *Ssmtag;		/* tag of structure that member belongs to */

    SYMBOL *Sprev;		/* ptr to prev symbol on glob/local list */
    SYMBOL *Snext;		/* ptr to next symbol on ditto */
    int Srefs;			/* # times referenced since created */
    SYMBOL *Snhash;		/* ptr to next sym on this hash chain */
};

/* Defs providing external access to structure fields, depending on
 * the kind of symbol.  No "s_" member should ever be referenced directly.
 *
 * Need to finish documenting how each SC_ type uses Svalue and other fields,
 * and clean up some bad refs.
 */

/* Every symbol class uses these fields */
#define Sclass Scontents.s_class	/* Symbol class */
#define Sflags Scontents.s_flags	/* Symbol flags */
#define Sname Scontents.s_name		/* Symbol name string */

/* Many (but not all) symbol classes use these fields */
#define Stype Scontents.s_var.s_type	/* C type of symbol */
#define Svalue Scontents.s_val.s_int	/* Misc uses */

/* Sclass == SC_UNDEF
**	When the lexer returns an identifier, it also points "csymbol" 
** to an entry in the symtab.  If the symbol did not already exist, it is
** created with class SC_UNDEF.
**	Only Sname and Sclass will be set.
**	Stype will be NULL and Svalue zero.
** Normally the parser changes this quickly to something else, or gets
** rid of the symbol.  The only known usages which keep SC_UNDEF symbols
** around are:
**	- A pair of dummy symbols at the start of the global & local sym lists.
**	- References to structures/unions that have not yet been defined.
**		Stype should indicate either TS_STRUCT or TS_UNION.
*/

/* Sclass == SC_RW (reserved word)
**	Stoken holds the token code to return when the lexer finds this word.
**	Skey contains a reserved-word token type key, one of the TKTY_ values.
*/
#define Stoken Scontents.s_val.s_int
#define Skey  Scontents.s_var.s_rwkey

/* Sclass == SC_MACRO (preprocessor macro definition)
**	Smacptr points to the macro body (a dynamically allocated string).
**		This may be NULL for special types of macros.
**	Svalue says how many arguments it takes, or indicates a special value.
**		See the documentation in CCINP for details.
*/
#define Smacptr Scontents.s_var.s_macptr

/* Sclass == SC_TAG - Structure, union, or enum tag
** Sclass == SC_MEMBER - Structure or union member (component)
** Sclass == SC_ENUM - Enum list member
**	See the discussion of structure tags and members farther on
**	for a thorough explanation.
*/
#define Ssmoff Svalue

/* Sclass == SC_TYPEDEF
**	This symbol is a "typedef name" and is lexically considered a
** type-specifier keyword.
**	Stype points to the C type which this symbol represents.
*/

/* Sclass == SC_EXTERN or SC_AEXTERN
**   SC_EXTERN means the object (var or fun) was explicitly declared "extern".
**   SC_AEXTERN means that "extern" was ASSUMED -- it was not stated.
** When parsing top-level declarations, SC_AEXTERN is the default storage class
** when no explicit storage class is specified.  This allows KCC
** to distinguish between "extern" that is assumed, and "extern" that
** is stated.  See H&S 4.3.1 and 4.8.  Svalue is used as a flag:
**	Svalue == 0  A declaration reference (outputs EXTERN in asm file)
**	       != 0  A definition (outputs INTERN in asm file)
**	Stype specifies the symbol's C type.
*/

/* Sclass == SC_STATIC
**	The object (var or fun) has static extent and the symbol is not
** exported to the linker.  This can only happen if "static" is explicitly
** stated.
**	Stype specifies the symbol's C type.
**	Svalue is unused except for function types, in which case
**		== 0 means a reference and != 0 a definition, just as for
**		SC_EXTERN and SC_AEXTERN.
*/

/* Sclass == SC_ARG, SC_RARG	- Function parameters
**	In extdef(), Sargp is used to hold a symbol pointer to the next
** SC_ARG in a chain.  In funcdef() this is changed so that Svalue
** is used thereafter.  This is tricky because these two are really union
** co-members and cannot both be used simultaneously!
**	Stype specifies the parameter's C type.
**	Sargp is a symbol pointer to the next parameter (or null if last one)
**	Svalue contains stack offset, similar to SC_AUTO except that it is
**		a positive number (if all args are 1 word, this is same as
**		argument number) and thus needs to be inverted for an actual
**		stack reference.
*/
#define Sargp   Scontents.s_val.s_sym

/* Sclass == SC_REGISTER
**	This isn't really used anywhere.  For the time being,
** the classes SC_RAUTO and SC_RARG remember whether the "register" keyword
** was specified with auto vars or function parameters.  This could be used
** to help the optimizer.
*/


/* Sclass == SC_AUTO, SC_RAUTO, SC_XAUTO, SC_XRAUTO
**  SC_AUTO describes an "auto" variable, within a block.  This is the
**	default storage class when parsing declarations within a block.
**  SC_RAUTO is the same but indicates that the "register" storage class
**	was given in the declaration.
**  SC_XAUTO and SC_XRAUTO are what SC_AUTO and SC_RAUTO are turned
**	into when parsing has left the block within which the variable
**	was declared.  That is, the declaration is no longer active.
**
**	Stype specifies the symbol's C type.
**	Svalue holds the stack offset value, in words.
*/


/* Sclass == SC_LABEL, SC_ULABEL, SC_ISTATIC, SC_STATIC (label)
 *	Ssym points to a label symbol for goto or for static data.
 */
#define Ssym   Scontents.s_val.s_sym

/*
** Certain types of symbols have a prefix character in their names
** as a hack to provide for "overloading classes" or "name spaces"
** [Ref H&S 4.2.4].
** These prefixes (none of which are legal as part of an input
** identifier) are:
**	+ - Structure member symbols	("component names")
**	^ - Structure/enum tag symbols	("struct, union, and enum tags")
**	@ - Goto label symbols		("statement labels")
** The other two name classes ("preprocessor macro names" and "other names")
** do not need a prefix, because the former are handled during preprocessing
** prior to compilation, and the latter are normal.  For reference, here
** is a list of the overloading classes as defined in H&S, with a code:
**   OC=	Overloading Class
**	M	Preprocessor macro names
**	T	Struct/union/enum tags
**	L	Statement labels
**	*	Structure component names (unique class for each structure)
**	-	Normal
*/

#define SPC_SMEM ('+')		/* * Structure/union members */
#define SPC_TAG  ('^')		/* T Structure/union/enum tags */
#define SPC_LABEL ('@')		/* L Statement labels */
#define SPC_MACDEF ('?')	/* Special for hiding the "defined" macro */
#define SPC_IDQUOT ('`')	/* Special for hiding quoted idents */
#define SPC_IAUTO ('<')		/* Special for hiding internal auto vars */

/* Symbol Classes (some relationship to "storage classes") */

enum symbolspec {	/* OC {v=variable,f=function} */
	SC_UNDEF,	/*	Undefined  - should be zero */
	SC_RW,		/* -	Reserved Word */
	SC_MACRO,	/* -	Macro name */
	SC_TAG,		/* T	struct/union/enum tag */
	SC_UTAG,	/* T	undefined struct/union/enum tag */
	SC_MEMBER,	/* * v	struct/union member */
	SC_ENUM,	/* - v	Enumeration constant "member" */
	SC_TYPEDEF,	/* -	Typedef name */
	SC_AEXTERN,	/* - vf Assumed-extern	variable or function */
	SC_EXTERN,	/* - vf Stated-extern	variable or function */
	SC_STATIC,	/* - vf Static		variable or function */
	SC_REGISTER,	/* - v	Register	variable */
	SC_ARG,		/* - v	Formal parameter to function */
	SC_RARG,	/* - v	Register formal parameter */
	SC_AUTO,	/* - v	Auto		variable */
	SC_RAUTO,	/* - v	Register auto	variable */
	SC_ISTATIC,	/* - v	Internal static	variable (within a block) */
	SC_LABEL,	/* L	Goto label */
	SC_ULABEL,	/* L	Undefined goto label */
	SC_ILABEL	/* L	Internal ($xxx) label. Not used in symtab. */
};

/* Symbol flags */
#define SF_XLOCAL	01	/* Local symbol no longer active */
/* (old) #define SF_NLOCAL	02 */	/* Local symbol "new", just defined. */
#define SF_PARAMDECL	04	/* SC_ARG only: parameter declaration seen */
#define SF_SIDEFF	010	/* Side-effect (tag or enum) during parsing.
				** This flag is only set by CCDECL's pbase()
				** for dummy "base" symbol structures
				** and only used to detect null declarations.
				*/
#define SF_MACRO	020	/* SC_MACRO only: another indicator of
				** macro-ness, mainly to keep findsym() fast.
				*/
#define SF_MACEXP	040	/* SC_MACRO only: macro being expanded.
				** This is used to detect and suppress
				** recursive expansions.
				*/
#define SF_MACSHADOW	0100	/* Any class, indicates this symbol is
				** shadowed by a macro symbol (ie has the
				** same identifier).
				*/
				
#if 0
#define SF_REGIS	02	/* User declared this as "register" */
#endif

/*
** Global vs Local symbols
**	Local symbols are kept on the "locsymbol" list and global ones on
** the "symbol" list.  Local symbols can have the following classes:
**	SC_ARG, SC_RARG			(function parameters)
**	SC_AUTO, SC_RAUTO, SC_ISTATIC	(auto, register, static)
**	SC_TAG, SC_UTAG, SC_ENUM, SC_MEMBER (struct/union/enum tags,enums,mems)
**	SC_LABEL, SC_ULABEL, SC_ILABEL	(goto labels)
**
**	When parsing enters a scope block, "beglsym()" is called to set
** a pointer to the start of the list of symbols local to that block.
** All symbols before this pointer will belong to outer blocks; all symbols
** after this pointer will belong to this block or inner blocks.
**	When parsing leaves a block, the symbols local to that block are
** still retained so that code generation can refer to them, but they are
** rendered inactive by setting the SF_XLOCAL (ex-local) flag.  This is
** done by having "compound()" call "endlsym()".  The findsym
** routine will never return a symbol which has this flag set.
** Thus, at any time, the symbols defined within any specific block (and
** not outer or inner ones) are represented by all of the local symbols
** following this remembered "head pointer" which do NOT have SF_XLOCAL set.
**
**	All local symbols -- ie everything on the locsymbol list -- are
** flushed completely when the code generator has finished dealing with
** a function's parse tree.  "ridlsym()" is the function called to do this.
** Note that SC_ARG and SC_RARG are active over the entire body of
** a function.
**
**	Note that SC_ILABEL symbols are never found in the symbol table
** itself.  These are generated by CCLAB and kept on a separate label list.
** Certain symbols may point to these "label symbols", however.
*/

/* TYPE - Type table entry
*/

TYPE {
    int Tspec;			/* Type specifier, set to a TS_ value */
    int Tsize;			/* size in words, or # elements.  See desc */
    int Tflag;			/* Flags, plus size in bits (mask 0777) */
    union {
	TYPE *t_subtype;	/* subtype if not a basic type */
	SYMBOL *t_sym;		/* or tag name of struct or union */
    } t_var;
    TYPE *Trehash;		/* Pointer to next in hash chain */
				/* only used by CCSYM's findtype() */
};

/* Define members for public consumption. "t_" members should never
** be used by code. */
#define Tsmtag t_var.t_sym	/* TS_STRUCT or TS_UNION - tag pointer */
#define Tsubt t_var.t_subtype	/* Subtype - used only by:
				** TS_FUNCT (function returning subtype)
				** TS_ARRAY (array of subtype)
				** TS_PTR   (pointer to subtype)
				*/
/* Tsize normally represents the size of the object in terms of words.
** However, there are some exceptions:
** If TS_STRUCT or TS_UNION and the structure is not yet defined
**	(i.e. Tsmtag->Sclass == SC_UTAG) then Tsize is 0 since size is unknown.
** If TS_ARRAY, Tsize is the # of objects in the array, each of which has
**	the given subtype.  So the total size is Tsize times the size of
**	the subtype.
**	If Tsize is 0, then the size of the array is unknown.  This can
**	happen when the user declares something like "int foo[];"
**	However, only the first dimension of a (possibly multi-dimensional)
**	array can be left unspecified in this way, and obviously it will not
**	work to do a sizeof on this array.
**
** It used to be the case that a type of TS_PTR used Tsize as an array
** indicator.  THIS IS NO LONGER TRUE, but for reference:
**	If Tsize 0, this was a normal TS_PTR to some object and the pointer
**		had the size PTRSIZE.
**	If Tsize non-zero, then this was a hybrid type that represented an
**		array name coerced into a pointer.  Tsize and Tsubt
**		were the same as the Tsize and Tsubt of the array in question.
**		The array type itself could be found by doing a findtype
**		of TS_ARRAY with the given Tsize and Tsubt.
** To repeat: TS_PTR is now just a normal type, and its Tsize always
** represents the pointer size (1 word).
*/

/* Tflag is used to hold both flags and the # of bits occupied by an object
** of this type.  While flags may be used by any type, the size in bits is
** only meaningful for objects of scalar type, and is only really used for
** objects of integral type.
*/
#define tbitsize(t) ((t)->Tflag&0777)	/* Get size in bits for a type */

#define TF_CONST	01000	/* Type is a "const" object */
#define TF_VOLATILE	02000	/* Type is a "volatile" object */
		/*	04000	reserved */
#define TF_INTEG	010000	/* Integral type */
#define TF_FLOAT	020000	/* Floating-point type */
#define TF_SCALAR	040000	/* Scalar type */
#define TF_UNSIGN	0100000	/* Unsigned type */
#define TF_CHAR		0200000 /* Char type */
#define TF_BITF		0400000 /* Bitfield type */
#define TF_STRUCT	01000000 /* Struct or Union type */
#define TF_BYTE		02000000 /* Byte (non-word) type (MACH DEPENDENT) */

#define _TF_UINTEG (TF_INTEG+TF_UNSIGN)	/* to save space in alltypemacro */

/* Quick-check macros.  The TF_ types shouldn't be directly used. */
#define tisinteg(t)  ((t)->Tflag&TF_INTEG)
#define tisfloat(t)  ((t)->Tflag&TF_FLOAT)
#define tisarith(t)  ((t)->Tflag&(TF_INTEG+TF_FLOAT))
#define tisscalar(t) ((t)->Tflag&(TF_INTEG+TF_FLOAT+TF_SCALAR))
#define tischar(t)   ((t)->Tflag&TF_CHAR)
#define tisbitf(t)   ((t)->Tflag&TF_BITF)
#define tisunsign(t) ((t)->Tflag&TF_UNSIGN)
#define tissigned(t) (!tisunsign(t))
#define tisstruct(t) ((t)->Tflag&TF_STRUCT)	/* struct or union */
#define tisbyte(t)   ((t)->Tflag&TF_BYTE)	/* Byte (non-word) object */

/* Same macros, but with a Tspec argument. */
#define tspisinteg(ts)  (_tfltab[ts]&TF_INTEG)
#define tspisfloat(ts)  (_tfltab[ts]&TF_FLOAT)
#define tspisarith(ts)  (_tfltab[ts]&(TF_INTEG+TF_FLOAT))
#define tspisscalar(ts) (_tfltab[ts]&(TF_INTEG+TF_FLOAT+TF_SCALAR))
#define tspischar(ts)   (_tfltab[ts]&TF_CHAR)
#define tspisunsigned(ts) (_tfltab[ts]&TF_UNSIGN)

extern int _tfltab[];	/* Flag table, kept in CCDATA */

/* Type Specifiers */

/* Define moby macro defining all type specs.
**	typespec(enum_name, string, size_in_bits, flags)
**
**	The size_in_bits for bitfields is ostensibly 1 here only so that
**	tables will be initialized properly.  In actuality their size
**	varies (and the same is possible for char).
*/
#define alltypemacro \
 /* Misc types (keep TS_VOID as zero for elegance) */\
    typespec(TS_VOID, "void",	 0,0)	/* (void) */\
    typespec(TS_FUNCT,"function",0,0)	/* Function (rets subtype) */\
 /* Aggregate types */\
    typespec(TS_ARRAY, "array",  0,0)		/* Array (of a subtype) */\
    typespec(TS_STRUCT,"struct", 0,TF_STRUCT)	/* Structure */\
    typespec(TS_UNION, "union",  0,TF_STRUCT)	/* Union */\
 /* Scalar misc types */\
    typespec(TS_PTR, "pointer",TGSIZ_PTR, TF_SCALAR)	/* Ptr (to subtype) */\
    typespec(TS_ENUM,"enum",   TGSIZ_ENUM,TF_SCALAR)	/* Enum */\
 /* Scalar Arithmetic types - Floating-point */\
    typespec(TS_FLOAT, "float", TGSIZ_FLOAT, TF_FLOAT) /* (float) */\
    typespec(TS_DOUBLE,"double",TGSIZ_DOUBLE,TF_FLOAT) /* (double) */\
    typespec(TS_LNGDBL,"lngdbl",TGSIZ_LNGDBL,TF_FLOAT) /* (long double) */\
 /* Scalar Arithmetic types - Integral */\
    typespec(TS_BITF, "bitf",    1,          TF_INTEG+TF_BITF)	/* bitfield */\
    typespec(TS_CHAR, "char",    TGSIZ_CHAR, TF_INTEG+TF_CHAR)	/* char  */\
    typespec(TS_SHORT,"short",   TGSIZ_SHORT,TF_INTEG)		/* short */\
    typespec(TS_INT,  "int",     TGSIZ_INT,  TF_INTEG)		/* int   */\
    typespec(TS_LONG, "long",    TGSIZ_LONG, TF_INTEG)		/* long  */\
    typespec(TS_UBITF,"u_bitf",  1,          _TF_UINTEG+TF_BITF) \
    typespec(TS_UCHAR,"u_char",  TGSIZ_CHAR, _TF_UINTEG+TF_CHAR) \
    typespec(TS_USHORT,"u_short",TGSIZ_SHORT,_TF_UINTEG) \
    typespec(TS_UINT, "u_int",   TGSIZ_INT,  _TF_UINTEG) \
    typespec(TS_ULONG,"u_long",  TGSIZ_LONG, _TF_UINTEG)

/* Define indices */

enum typespecs {
#define typespec(ts,str,bsiz,fl) ts,
	alltypemacro		/* Expand */
#undef typespec
	TS_MAX			/* # types; 1st non-existent typespec index */
};

#if 0 /* Comments on types */

TS_VOID - Void.
	Tsize is 0.
TS_FUNCT - Function returning Tsubt.
	Tsize is 0.
TS_ARRAY - Array of Tsubt.
	Tsize is # of elements in array.  This may be 0 if the size of
	the array is currently unknown.
TS_STRUCT, TS_UNION - Structure/Union of Tsmtag.
	Tsize is # of words in structure.
TS_PTR - Pointer to Tsubt.
	Tsize is 1 (# wds in a pointer).

All arithmetic types plus TS_ENUM:
	Tsize = size in words (if less than 1, is 1).
	Tflag = Low-order bits have size in bits.
	Tsubt/Tsmtag is unused.

All types have Tflag set with appropriate flags.  These are
	(1) the ones specified in the alltypemacro definition
plus	(2) TF_BYTE if the object is a bitfield OR is smaller than a word.
		This is machine dependent!  All other flags are pure C.
plus	(3) TF_CONST or TF_VOLATILE if so declared.

#endif

/* Cast (Type Conversion) types (NODE.Ncast) */

#define allcastmacro \
    /* General-purpose cast types */\
	castspec(CAST_ILL,"ill")	/* convtab[] only: Illegal cast */\
	castspec(CAST_TRIV,"triv")	/* convtab[] only: types must be == */\
	castspec(CAST_NONE,"none")	/* No actual conversion needed */\
    /* Casts to Integer Type */\
	castspec(CAST_IT_IT,"it_it")	/* from Integer Type */\
	castspec(CAST_FP_IT,"fp_it")	/* from Floating-Point type */\
	castspec(CAST_EN_IT,"en_it")	/* from ENumeration type */\
	castspec(CAST_PT_IT,"pt_it")	/* from Pointer Type */\
    /* Casts to Floating-Point type */\
	castspec(CAST_FP_FP,"fp_fp")	/* from Floating-Point type */\
	castspec(CAST_IT_FP,"it_fp")	/* from Integer Type */\
    /* Casts to ENum type */\
	castspec(CAST_EN_EN,"en_en")	/* from ENumeration type */\
	castspec(CAST_IT_EN,"it_en")	/* from Integer Type */\
    /* Casts to Pointer Type */\
	castspec(CAST_PT_PT,"pt_pt")	/* from Pointer Type */\
	castspec(CAST_IT_PT,"it_pt")	/* from Integer Type */\
    /* Misc casts */\
	castspec(CAST_AR_PA,"ar_pa")	/* Array -> Ptr to 1st element */\
	castspec(CAST_FN_PF,"fn_pf")	/* Function -> Pointer to Function */\
	castspec(CAST_VOID,"void")	/* Any type to void type (discard) */


enum castspecs {
#define castspec(op,str) op,
	allcastmacro		/* Expand */
#undef castspec
	CAST_MAX		/* # of cast types */
};

/* Macro to generate a unique index for every possible type cast combo */
#define castidx(from,to) (((to)*TS_MAX)+(from))


/* Structure/Union/Enum tags and members
**
** If a type has Tspec TS_STRUCT, TS_UNION, or TS_ENUM, then it has no subtype
** pointer.  Instead, t_var is interpreted as "Tsmtag" - that is, a pointer
** to a tag symbol (Sclass SC_TAG).  In the following discussion, the word
** "structure" is understood to mean both "struct" and "union".
**
** Struct/Union/Enum types:
**	Tspec - TS_STRUCT, TS_UNION, or TS_ENUM.
**	Tsize - size of the structure, in words.  0 if structure not yet
**			defined.  INTSIZE if an enum type.
**	Tsmtag - pointer to the tag symbol for this type.
**
** Tag symbols:
**	Sclass - SC_TAG (or SC_UTAG if not yet defined).
**	Sname - the tag name, prefixed with SPC_TAG.  This will be
**		a unique internal name if the structure or enum list was
**		not named with a tag at definition time.
**	Stype - pointer to the C type.  This is in effect a backpointer
**		to the type specification described above.
**	Ssmnext - pointer to the first component symbol of this object.
**		This will be NULL if the tag is still undefined (SC_UTAG).
**		For a structure, the components are structure members
**		with Sclass SC_MEMBER.  For an enum, the components are
**		enum constants with Sclass SC_ENUM.
**
** Structure member symbols:
**	Each component of a structure is represented by an unique
** symbol table entry.  The members are linked together in order of
** definition.
**	Sclass - SC_MEMBER
**	Stype - C type of this member.
**	Ssmoff - offset from beginning of structure.
**		If positive, it is a word offset from start of struct.
**		If negative, it is a bitfield.  After making it positive again,
**			the low 12 bits are the P+S fields of a byte pointer,
**			and the remaining high bits are the word offset.
**	Ssmnext - pointer to next member (NULL if this is last one).
**	Ssmtag - pointer back to "parent" structure tag symbol.
**		The sole reason for the existence of Ssmtag is so that
**		it is possible to find out very quickly whether an
**		arbitrarily selected SC_MEMBER symbol does in fact
**		belong to a specific structure.
**
** Enum constant symbols:
**	Each component of an enum list is represented by an unique
** symbol table entry, linked together in order of definition.
** These are almost the same as SC_MEMBERs except that Svalue is used
** instead of Ssmoff.
**	Sclass - SC_ENUM
**	Stype - C type (always "int" for now).
**	Svalue - integer value of this enum constant.
**	Ssmnext - pointer to next component (NULL if this is last one).
**	Ssmtag - pointer back to "parent" tag symbol.
**		This is not really used at the moment but is kept for
**		consistency with SC_MEMBER and (someday) error messages.
**
** Tag, enum constant, and structure member symbols are treated just as
** regular variable symbols are.  They can be made global or local, and in
** particular can be "shadowed" by structure definitions in inner blocks,
** through use of the global/local symbol lists and the SF_XLOCAL flag.
**	Note that each structure member always has its own symbol cell, even
** though the identifier may duplicate that of a another structure's
** member.  They are distinguished by checking the "Ssmtag" pointer,
** which indicates the structure enclosing any particular member.
**	Enum constant symbols likewise are all unique to a particular tag,
** even though the language does not require this (in fact it requires
** that enum constant identifiers be treated in the same overloading class
** as regular variable identifiers!)  This is so the list structure won't
** be screwed up by redefinitions.  However, error messages will be
** generated whenever a duplicate enum constant definition is seen.
**
**	K&R "global" structure members are no longer allowed.
** It would not be particularly hard to allow this, but why bother?
*/

/* Define macro to get pointer to list of structure member symbols */
#define SMEMS(t) ( ((t)->Tspec == TS_STRUCT || (t)->Tspec == TS_UNION) \
	? (t)->Tsmtag->Ssmnext : (SYMBOL *)NULL )


/* This structure is only used by CCSYM in order to initialize the built-in
 * types from the "tnames" table in CCDATA.
 */
#define TYNAME struct _tyname
struct _tyname {	/* built in types */
    char *tnname;	/* Reserved keyword name, e.g. "int" */
    int tntype;		/* type specifier code (set to a TS_ value) */
    int tnsize;		/* how big it is in terms of words, e.g. INTSIZE */
};

/* Structure of entries in rwtab (reserved word table).
 * This is only used by CCSYM in order to initialize the built-in
 * types from the "rwtab" table in CCDATA.
 */
#define RESWD struct _reswd
struct _reswd {		/* reserved word entry */
    char *rw_name;	/* Reserved-word string */
    int	rw_token;	/* Token number for it (plus flags in high bits) */
};
extern RESWD rwtab[];		/* Reserved Word table */
#define RWF_TOKENMASK	0777	/* Mask for token identifier */
#define RWF_ANSI	01000	/* Word is an ANSI addition */
#define RWF_KCC		02000	/* Word is a KCC extension */

/*
** Misc shared storage
*/

EXT SYMBOL bytsym;		/* symbol for $BYTE */

EXT SYMBOL
    *csymbol,		/* current symbol */
    *symbol,		/* symbol table head ptr */
    *minsym;		/* minimum variable ptr (CC, CCSYM) */

extern TYPE *ttable[];	/* hash table of types (CCSYM, CCDATA) */
extern TYPE types[];	/* table of type records (CCDUMP, CCSYM, CCDATA) */
