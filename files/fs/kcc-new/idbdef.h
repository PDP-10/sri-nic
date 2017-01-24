/* IDBDEF.H - IDB internal definitions
**
**	User-visible macros are uppercase, prefixed with "ID_".
**	Package external routines and variables use "id_xxx"
**		with exception of idxxx_t typedefs.
**	Externals that are actually package-internal use "ii_" for routines,
**		and "iiv_" for variables.
**	Static functions use anything, but "c_" is prefixed to C versions of
**		T20 system calls.
*/
#ifndef _IDBDEF_INCLUDED
#define _IDBDEF_INCLUDED 1

#include <c-env.h>		/* For OS etc defs */
#include <nic/idb.h>		/* Make sure this is included */

/* Hack to use instead of #error, because SUN/BSD don't support it yet.
** Instead of "#error foo", say "#define C_ERROR "foo"" and that should cause
** a redefinition error message.
*/
#define C_ERROR		/* Redefine this to cause error */

/* Machine-dependent definitions.
**	word_t must be a basic integer type with at least 32 bits.
*/
#if (CPU_PDP10+CPU_M68)==0
#define C_ERROR "No definitions supported for this CPU type."
#endif

#if CPU_PDP10

#include <macsym.h>		/* Useful macros (XWD etc) */

#ifndef char7			/* Avoid conflicts */
#define char7 _KCCtype_char7	/* Special KCC hack - unsigned 7-bit char */
#endif

/* Word size definitions */
#define CHAR_BITS 9		/* # bits in a char byte */
#define CHAR7_BITS 7		/* # bits in a char7 byte */
#define WD_BITS 36		/* # bits in a word */
#define WD_CBTS 2		/* # bits for word size in char bytes */
typedef unsigned char uchar_t;	/* Unsigned char */
typedef int word_t;		/* Basic type with at least 32 bits */
typedef unsigned int uword_t;		/* Same type, unsigned */
#define SDECLHWD(name) short name	/* to decl a halfword within struct */
				/* (this could be "int name : HWD_BITS") */

/* Size of a "page" */
#define PG_WBTS 9		/* # bits for page size in words */
#define PG_CBTS 11		/* # bits for page size in char bytes */
#define PG_PBTS PG_WBTS		/* # bits to shift for pointer conversion */

/* # pages CPU/system allows */
typedef unsigned short pagnum_t; /* Basic type big enuf for file/mem page # */
#if SYS_T20
#define PG_SSZ 512		/* T20 Section size (# pages in a section) */
#define PG_SNM (1<<5)		/* T20 Section Number - # sections supported */
#define PG_NUM (PG_SSZ*PG_SNM)	/* Max # pages available in memory */
#else
#define PG_NUM 512		/* Non-T20s support only section 0 */
#endif
#endif /* CPU_PDP_10 */

#if CPU_M68		/* Also left-to-right, but only 32 bits */

#ifndef char7		/* Avoid conflicts */
#define char7 uchar_t	/* Unsigned char */
#endif

/* Word size definitions */
#define CHAR_BITS 8		/* # bits in a char byte */
#define CHAR7_BITS 8		/* # bits in a char7 byte */
#define WD_BITS 32		/* # bits in a word */
#define WD_CBTS 2		/* # bits for word size in char bytes */
typedef unsigned char uchar_t;	/* Unsigned char */
typedef long word_t;		/* Basic type with at least 32 bits */
typedef unsigned long uword_t;		/* Same type, unsigned */
#define SDECLHWD(name) short name	/* to decl a halfword within struct */
				/* (this could be "int name : HWD_BITS") */

/* Size of a "page" */
#define PG_WBTS 9		/* # bits for page size in words */
#define PG_CBTS 11		/* # bits for page size in char bytes */
#define PG_PBTS PG_CBTS		/* # bits to shift for pointer conversion */

/* # pages CPU/system allows */
typedef unsigned short pagnum_t; /* Basic type big enuf for file/mem page # */
#define PG_NUM (1<<(23-PG_CBTS))	/* Max # pages in mem (temp guess) */

#endif /* CPU_M68 */

/* Derived machine-dep definitions */

/*#define WD_BITS (sizeof(word_t)*CHAR_BITS) */	/* Size of word, in bits */
#define HWD_BITS (WD_BITS/2)			/* Size of halfword */
#define WD_CH7S (WD_BITS/CHAR7_BITS)		/* Size of word, in char7s */
#define WD_SIGN (1<<(WD_BITS-1))		/* Sign bit of word */
#define CHAR7_SIGN (1<<(CHAR7_BITS-1))		/* Sign bit of char7 byte */
#define CHAR7_MASK ((1<<CHAR7_BITS)-1)		/* Mask for char7 byte */
	/* # bits unused at low end of a word full of char7 bytes */
#define CH7_UNUSED_BITS (WD_BITS-(WD_CH7S*CHAR7_BITS))

/* Define macros we normally get from macsym.h */
#ifndef FLD
/* FLD(val,mask) - Make field value from right-justified value */
#define FLD(val,mask) (((unsigned)(val)*((mask)&(-(mask))))&(mask))

/* FLDGET(wd,mask)     - Get right-justified value from field in word */
#define FLDGET(wd,mask) (((unsigned)(wd)&(mask))/((mask)&(-(mask))))

/* FLDPUT(wd,mask,val) - Put right-justified value into field in word */
#define FLDPUT(wd,mask,val) (((wd)&(~(mask)))|FLD(val,mask))

#ifndef XWD
#define XWD(a,b) (((unsigned)(a)<<HWD_BITS)|((b)&RH))	/* Put wd together */
#endif
#endif

/* Halfword hacking stuff */
#ifndef RH
#define RH ((1<<HWD_BITS)-1)	/* Right-half mask */
#endif
#ifndef LH
#define LH (~RH)		/* Left-half mask */
#endif

#define GLH(a) ((unsigned)(a)>>HWD_BITS)	/* Get LH */
#define GRH(a) ((a)&RH)				/* Get RH */

typedef union {
	word_t i;				/* Fullword */
	struct {				/* Unsigned halfwords */
		unsigned SDECLHWD(lh);
		unsigned SDECLHWD(rh);
	} uhwd;
	struct {			/* Signed halfwords */
		SDECLHWD(lh);
		SDECLHWD(rh);
	} hwd;
} wordh_t;

/* Conversion definitions - chars, words, pages */
#define WD_CSIZ (1<<WD_CBTS)	/* Word size in char bytes */
#define WD_CMSK (WD_CSIZ-1)	/* Mask for " */
#define PG_WSIZ (1<<PG_WBTS)	/* Page size in words */
#define PG_WMSK (PG_WSIZ-1)	/* Mask for " */
#define PG_CSIZ (1<<PG_CBTS)	/* Page size in char bytes */
#define PG_CMSK (PG_CSIZ-1)	/* Mask for " */

#define pagtowds(p)  ((uword_t)(p)<<PG_WBTS)	/* # pages to # words */
#define pagtochs(p)  ((uword_t)(p)<<PG_CBTS)	/* # pages to # chars */
#define wdstochs(ws) ((uword_t)(ws)<<WD_CBTS)	/* # words to # chars */
#define wdsuptopag(ws) ((ws+PG_WMSK)>>PG_WBTS)	/* # wds rounded up to # pgs */
#define chsuptopag(cs) ((cs+PG_CMSK)>>PG_CBTS)	/* # chs rounded up to # pgs */
#define chsuptowds(cs) ((cs+WD_CMSK)>>WD_CBTS)	/* # chs rounded up to # wds */
#define wdsdntopag(ws) ((ws)>>PG_WBTS)		/* # wds rounded dn to # pgs */
#define chsdntopag(cs) ((cs)>>PG_CBTS)		/* # chs rounded dn to # pgs */
#define chsdntowds(cs) ((cs)>>WD_CBTS)		/* # chs rounded dn to # wds */

#define pptowp(p)  ((word_t *)(p<<PG_PBTS))	/* Mem page loc to word ptr */
#define wptopp(wp) ((uword_t)(wp)>>PG_PBTS)	/* Word ptr to mem page loc */
#define wptocp(wp) ((char *)(wp))		/* Word ptr to char ptr */
#define cptowp(cp) ((word_t *)(cp))		/* Char ptr to word ptr */
#define pptocp(p)  wptocp(pptowp(p))		/* Mem page loc to char ptr */
#define cptopp(cp) wptopp(cptowp(cp))		/* Char ptr to mem page loc */

/* Pagptr type stuff */
#if (1<<HWD_BITS) < PG_NUM	/* Make sure page # field big enough! */
#define C_ERROR "Halfword not big enough for page number!"
#endif

typedef union {
	word_t i;
	struct { pagnum_t cnt, loc; } pp;
} pagptr_t;

/* Other bit mask definitions */
#define LBIT(n) (1<< ((WD_BITS-1)-(n)))	/* Bit n (left to right) */
#define RBIT(n) (1<< (n))		/* Bit n (right to left) */

/* System-dependent internal symbol redefinitions
**	TOPS-20/TENEX is limited to external symbols of 6 monocase chars.
**	Most UNIX systems are limited to external syms of 8 case-dep chars.
**
** Most of the redefinitions are done in idb.h.  These symbols are ones
** which are supposed to be internal to IDB and shouldn't be seen or used
** by user programs, except VOID which includes this file also.
*/

#if CPU_PDP10

/* Package-Internal routines */
#define ii_estrcpy	iiestr	/* strcpy but return endptr */
#define ii_ememcpy	iiemem	/* memcpy but etc */
#define ii_esfmt	iiesfm	/* sprintf but return endptr */
#define ii_getcrx	iigcrx	/* Get record */
#define ii_getpgt	iigpgt	/* Get PGT */
#define ii_lsyserr	iilsys	/* Get last system error string */
#define ii_fbstatus	iifbst	/* Output FB status */
#define ii_stfstatus	iisfst	/* Output Sort-Table File status */
#define ii_pgsstatus	iipgst	/* Output memory Pages status */
#define ii_oldinit	iioini	/* CIFP compat code init */

/* External vars, not really available to user (mainly for VOID) */
#define id_iniflg id_inf
#define id_ldflg  id_ldf
#define id_selflg id_slf
#define id_pgtitm id_sti
#define id_ifpver id_ver
#define id_recfln id_rfl
#define id_recrte id_rte
#define iiv_pagtab	iivpag
#define iiv_spgtab	iivspg
#define iiv_dbfprefix	iivdbf
#define id_fbacc id_fac		/* FB block addrs */
#define id_fbnac id_fna
#define id_fbrec id_fre
#define id_fbarec id_far
#define id_fbrat id_fra
#define id_fbwlok id_fwl
#define id_fbscr id_fsc
#define id_pthdr id_phd		/* PT block addrs */
#define id_ptift id_pif
#define id_ptilt id_pil
#define id_ptidt id_pid
#define id_ptrat id_pra
#define id_ptsrt id_psr
#define id_ptpgt id_ppg
#define id_ptwit id_pwi
#define id_ptscr id_psc
#define id_ptsc2 id_ps2

/* Error Signal stuff (internal globals) */
#define iiv_esptr	iivept
#define iiv_esiptr	iivepi
#define ii_eserr	iieser
#define ii_esdberr	iiesde
#define ii_esmsg	iiesmg
#define ii_esdbmsg	iiesdm

#else /* not CPU_PDP10 */

	/* Assume using 8-char case-dependent symbols. */
#endif

/****
***** SUBTTL Definitions for Database, SETs, and PTs
****/

/* For general details on the format of a database, see the
** IFPACK documentation file.
*/

/* Records are stored in a single RECORDS file and all modified records
** are re-written by appending at the end of this file, so that its
** existing contents are never changed.  Each record has the following
** 36-bit binary format:
*/

/* All records start on a word boundary:
**	-1,,<# chars following>	     total # wds in record is 1 + <# chs + 4>/5
**	<string of 7-bit ASCII bytes>
*/

/*	The string is composed of "items", each of which has the format:
**	Byte 1: <type>		type of item (value 0-176) 
**	Byte 2: <count>		# chars in item.  If bit 100 is set,
**	Byte 3: < " > or <data>	count is <low 6 bits>_6 + <next char>.
**	Bytes 4-n: <data>	allowing up to 2**13-1 chars (8191.)
**
**	Item #'s are defined by the item-name table.
**	Item 0 is always the "master key" or "handle", which is unique
**		among all records.
**	Items come in numerical order, hence item 0 is always first.
**	Record is terminated by an item of type 177, which has no count.
**	Note that this is the largest possible type and comes at end,
**		following numerical-order convention.
**	The next record will begin at the next word boundary.
*/

/* The first word in the RECORDS file is set to the same thing as the
** first word of ACCESS file (H_ID of the HDR SET) so that one never sees
** an address of 0 for a record.
*/

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
**;;;;;;;;	GENERAL DATABASE DEFINITIONS	;;;;;;;;;;;;;;;;;;;;
*/

#define ID_NITEMS (128)	/* # items including End-Of-Record item.  Hardwired. */
#define ID_MAXITM (126)	/* Max valid item number, hardwired.
			**  177 is reserved for E-O-R. */
#define ID_MAXNLN 50	/* Max # chars allowed for item name. */
			/*	(hardwired max = 255. due to 8-bit char cnt)*/
#define ID_MAXILN 1800	/* Max # chars allowed in an item string. */
			/*	(hardwired max = 8191. cuz 13-bit char cnt)*/
#define ID_MAXISL 1800	/* Max # chars allowed in a sort-table entry (item string). */
			/*	(hardwired max = <PG$SIZ-HE$SIZ-2>*5 due to */
			/*	size of a sortpage) */
#define ID_MAXRLN 2000	/* Max # chars in total record string.*/
			/* This is for dubious benefit of AUGMENT (L10) */
			/* string restrictions. */
			/*	(softwired max = 2560. cuz 2-page map area)*/
#define ID_MAXRFE 401	/* Max # words in total record file entry. */
			/* MAXRFE==:1+<<MAXRLN+4>/5> */

#ifndef ID_RECPGL	/* # pages to reserve for mapping record in core */
#define ID_RECPGL 2
#endif
#if ((ID_RECPGL-1)*PG_WSIZ)-ID_MAXRFE <= 0
#define C_ERROR "RECPGL too small or MAXRFE too big"
#endif

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
**;;;;;;;;;;	STRING/ENTRY TABLE (SET) FORMAT   ;;;;;;;;;;;;;;;;;;
*/

/* SET stands for "String/Entry Table".  All files, with the exception of
** the data record file itself, are composed of SETs.  A SET is
** always composed of an integral number of pages.  The first
** few words of a SET are the header, and all following words constitute
** the SET table data.
**
**	--- Beg of SET (page boundary) ---
**	SET Header:
**	    <Format Word>	Identifies SET type and format.
**	    <remainder of header>	Describes SET size and layout.
**	<table entries>
**	<free space>		This is a "gap" between table and strings.
**	<string space>		String space extends to end of the SET.
**	--- End of SET (page boundary) ---
**
** Historical note: It used to be possible to have 3 undefined gaps: between
** the header and table, table and freespace, and freespace and stringspace.
** This was flushed as of SET format V2 for simplicity.
*/

/* Contents of SET Header Format word */
struct setfmt {
	unsigned sid  : HWD_BITS;	/* SET ident (same for all SETs)*/
	unsigned styp : 3;		/* SET type */
	unsigned      : HWD_BITS-10;	/* unused */
	unsigned shl  : 4;		/* SET header length in words */
	unsigned sver : 3;		/* SET format version */
};

/* SET HEADER FORMAT - V1
**	Note all "start of" addresses are relative to beginning of the SET.
*/
#define H_ID	0	/* <identifier>,,<SET type><reserved><version> */
#define H_TBLP	1	/* <# entries>,,<start of entries> */
#define H_FREP	2	/* <# free wds>,,<start of free> */
#define H_TOTP	3	/* <# total wds>,,<# garbage wds> */
#define H_STRS	4	/* 0,,<start of strings> */
#define SH1_SIZE (chsuptowds(sizeof(struct sethdr1)))	/* # wds in header */

struct sethdr1 {
	struct setfmt sh_fmt;		/* SET format word */
	unsigned sh_tbl : HWD_BITS;	/* <# entries> */
	unsigned sh_tbs : HWD_BITS;	/* <start of entries> */
	unsigned sh_frl : HWD_BITS;	/* <# free wds> */
	unsigned sh_frs : HWD_BITS;	/* <start of free> */
	unsigned sh_tot : HWD_BITS;	/* <# total words> */
	unsigned sh_grb : HWD_BITS;	/* <# garbage words> */
	unsigned        : HWD_BITS;	/* unused, leave 0 */
	unsigned sh_sts : HWD_BITS;	/* <start of strings> */
};

/* SET HEADER FORMAT - V2
*/
#define SH2_SIZE (chsuptowds(sizeof(struct sethdr2)))	/* # wds in header */

struct sethdr2 {
	struct setfmt sh2_fmt;	/* SET format word */
	uword_t sh2_tot;	/* # total words in SET */
	uword_t sh2_tbl;	/* # words in table */
	uword_t sh2_frl;	/* # free wds in gap */
	uword_t sh2_grb;	/* # garbage wds in strings */
};

	/* Define canonical identifier.  If SET's identifier is different, */
	/* implication is that file was garbaged.  This should NEVER change */
	/* even through version changes. */
#define	SH_ID (0535450&RH)	/* Magic number of obvious derivation */

	/* Define SET format versions (use symbols for easier creffing) */
#define	SH_V1 1		/* Current format. */
#define SH_V2 2		/* New SET V2 format */

/* Define SET types for benefit of routines that operate on SETs in general.
** All SETs have 1-word entries.  Certain types point to strings
** of various formats.  A <straddr> value is an offset from the beginning of
** the SET, and points to the start of a string in string space.
*/

/* <SET type>  <Name>	- Format of entry word */

#define	ET_ENT	0 /* Entry  - Simple word, no string space used. */
#define	ET_ASZ	1 /* ASCIZ  - Sign bit means RH is <rel addr of ASCIZ> */
#define	ET_ASC	2 /* --obsolete-- No longer used */
#define	ET_ISC	3 /* Item Str Cnt - If sign set, EM%ICH has <chs>, RH <straddr>
			**	For V2, maybe change RH to <ASCIZ straddr>? */
#define	ET_SRT	4 /* Sort-Table	  - <# chs>,,<straddr>
			**	one extra word is reserved at <straddr>-1 */
#define	ET_SWD	5 /* String-Word  - one word is reserved
			**  at <entry #>+<start of strs> */

#define	ET_N	6	/* # possible entry types. */

/* Auxiliary defs for ET_ASZ, ET_ISC entries */
#define EA_SGN (1<<(WD_BITS-1))	/* Sign bit, indicates have string */
#define EA_ADR RH		/* String address in here */

/* Auxiliary defs for ET_ISC type entries (used by ILT, IDT, and V1 IST) */
#if CPU_PDP10
#define	E_CBTS	8		/* # bits allowed for <# chars> field */
#else
#define E_CBTS 7		/* Not enough room in 32-bit wds, sigh */
#endif
#define	E_CMAX	((1<<E_CBTS)-1)	/* Maximum <# chars> allowed in the field */
#define	EM_ICH	XWD(E_CMAX,0)	/* Mask for <# chars> */
struct iscent {
	unsigned : HWD_BITS - E_CBTS;	/* Junk */
	unsigned ef_ich : E_CBTS;
	unsigned ef_str : HWD_BITS;
};

#if 0 /* Comment */

		DATABASE FORMAT
		  VERSION 2

The first database format (V1) had some limitations on the size of
sort-table files (E$PBTS too small).  V2 resolved these problems by
restructuring the IST and PGT.  V1 format can still be read since
IFPACK will automatically convert old databases to V2; the old V1
definitions can be found alongside the routines which perform this
conversion.


#endif /* Comment */


/*-----------------------------------------------------------------------------
** HDR - Header Table		part of ACCESS file
**	Type ET_ASZ = if <sign>, RH is ptr to ASCIZ string.
**
** The HDR is the first SET in the access file, and its table points
** to ALL other components of the database, either by filename strings or
** by "pagptrs" (words of the form <# pages>,,<page #>) into other
** portions of the access file.  When a database is updated, a completely
** new access file is written.  Later format versions may minimize
** some overhead by keeping the RAT in a separate file.
*/

#define HE_VER	0	/* Version information.  See details below. */
#define HE_DID	1	/* ASCIZ Database-ID string */
#define HE_REC	2	/* ASCIZ filename string for data (record) file */
#define HE_IFT	3	/* pagptr to IFT (Item Flag/Name table) (0 if V2 fmt)*/
#define HE_ILT	4	/* pagptr to ILT (Item Lookup Table) */
#define HE_IDT	5	/* pagptr to IDT (Item Definition Table) (IST in V1) */
#define HE_RAT	6	/* pagptr to RAT (Record Address Table) */
#define HE_AVR	7	/* Access file Version number */
#if SYS_UNIX
#define HE_SIZ	8
#else
#define HE_SIZ	7
#endif

struct hdrtab {
    unsigned : HWD_BITS-3;
    unsigned hv_sys : 3;	/* 3 bits of system type */
    unsigned hv_vno : HWD_BITS;	/* RH is general version # */
    word_t he_did;	/* Rel addr of ASCIZ Database-ID string */
    word_t he_rec;	/* Rel addr of ASCIZ filename for .RECORDS file */
    pagptr_t he_ift;	/* pagptr to IFT (Item Flag/Name table) (0 if V2 fmt)*/
    pagptr_t he_ilt;	/* pagptr to ILT (Item Lookup Table) */
    pagptr_t he_idt;	/* pagptr to IDT (Item Definition Table) (IST in V1) */
    pagptr_t he_rat;	/* pagptr to RAT (Record Address Table) */
    uword_t he_avr;	/* Access file version number */
};

	/* Contents of HE$VER version information word */
#define	HV_VNO XWD<0,-1)	/* RH is general version # */
#define    HVV_V1	1	/*   Version 1 database format */
#define    HVV_V2	2	/*   Version 2    "       "   (current) */
#define	HV_SYS XWD(7,0)		/* 3 bits of system type */
#define    HVS_10X	0	/*   TENEX */
#define    HVS_T20	1	/*   TOPS-20 */
#define    HVS_T10	2	/*   TOPS-10 (not implem) */
#define    HVS_ITS	3	/*   ITS (not implem) */
#define    HVS_UNX	4	/*   UNIX (may need to differentiate CPUs) */
	/* Other fields are reserved for future use. */

/*-----------------------------------------------------------------------------
** RAT - Record Address Table		part of ACCESS file
**	Type ET_ENT - Indexed by Record Index #
**
** Each record has one entry in the RAT, and the index of this
** entry is called the "record index".  The disk address value
** points to the location of the record in the RECORDS data file.
** Currently no flags are used.  An entry of 0 means there is no
** record for that index.  Entry 0 is always kept zero.
*/

/*-----------------------------------------------------------------------------
** ILT - Item Lookup Table		part of ACCESS file
**	Type ET_ISC = <item #><synflg><# chs>,,<straddr>
** maybe V3 type ET_SRT = <# chs>,,<straddr>  (flags stored at straddr-1)
**
** Strings in the ILT are all uppercased and sorted for fast lookup.
** No zero entries should exist.  If the <synflg> bit (EL_ISY) is set
** it indicates that the name is a synonym, and is not one of the
** official names for an item.  All official names from the IDT
** (Item Definition Table) are of course duplicated in the ILT.
*/
#define	EL_ITM XWD(0177000,0)	/* Mask for item # */
#define	EL_ISY XWD(0000400,0)	/* Synonym flag */
struct iltent {
#if HWD_BITS-(8+E_CBTS)
	unsigned : HWD_BITS-(8+E_CBTS);
#endif
	unsigned el_itm : 7;	/* Item # */
	unsigned el_isy : 1;	/* Synonym flag */
	unsigned el_ich : E_CBTS;
	unsigned el_str : HWD_BITS;	/* RH is rel addr of string */
};

/*-----------------------------------------------------------------------------
** IDT - Item Definition Table		part of ACCESS file
**	Type ET_ISC = variousness
**
**	The old V1 IST and IFT have been merged into this table, which
** contains all item definitions indexed by item number.  In order to
** provide several words per item, the table is divided into "arrays"
** each of the constant length NITEMS.  Each array is separately indexed by
** item number.
** 	If at some future time more item definition data needs to be
** stored, there are two alternatives:
**	(1) Define more IDT arrays.  All the mechanisms for this are
**		already set up, and references are fast.  However,
**		this approach wastes a lot of space.  Adding one more
**		array would enlarge the IDT from 1 page to 2.
**	(2) Restructure the IDT into a single array where each entry
**		points to a N-word item definition block.  This completely
**		solves the space wastage problem, and allows expansion in a
**		very general way.  However, this would require extensive
**		revision of the SET manipulation routines. (eg consider
**		GC'ing the IDT while adding a string to a definition block)
*/

/* IDT arrays. */
#define EIA_ST	(ID_NITEMS*0)	/*<str><TIC+TNW+TWR+flags><#chs>,,<filename/JFN/pgtpage> */
#define EIA_PT	(ID_NITEMS*1)	/*<0><flags><page cnt>,,<page # in sortfile> */
#define EIA_IF	(ID_NITEMS*2)	/* <flags><#chs>,,<item name string> */
#define EIA_N	3		/* # of arrays */


/* EIA_ST - Search Table (1st level of search)
** 1st array definitions
**	if <sign>, RH is ptr to ASCIZ sort-table filename, and
**		EI_JFN (= EM_ICH) holds the # chars in string (including null)
**	if not <sign>, then
**		EI_JFN contains JFN for file, and
**		if EI_TIC=1, RH is <page # of PGT in core> (else RH 0)
*/

/* Flag def */		/* Flag meaning */
#define EI_SGN LBIT(0)	/* Sign bit (indicates have string) */
#define EI_TWR LBIT(1)	/* PGT-Written (further updates can overwrite) */
#define EI_TNW LBIT(2)	/* PGT-Needs-Writing */
#define EI_TIC LBIT(3)	/* PGT-In-Core  (RH is page # of PGT) */
#define EI_CVT LBIT(4)	/* PGT-Needs-Conversion (0 = cvted to V2) */
#define EI_OPN LBIT(5)	/* Sortfile is open */

struct eiastent {
	unsigned ei_sgn : 1;	/* If set, RH points to sortfile name string */
	unsigned ei_twr : 1;	/* PGT-written */
	unsigned ei_tnw : 1;	/* Needs-writing */
	unsigned ei_tic : 1;	/* PGT-in core */
	unsigned ei_cvt : 1;	/* PGT-needs-conversion */
	unsigned ei_opn : 1;	/* Sortfile is open */
#if HWD_BITS-(E_CBTS+6)
	unsigned  : HWD_BITS-(E_CBTS+6);	/* reserved */
#endif
	unsigned ei_ich : E_CBTS;	/* same as em_ich */
	unsigned ei_rh : HWD_BITS;	/* RH */
};

/*	Except for EI_CVT, all flags are only set for the in-core array
**	and have no meaning for the version on disk.
**	Not all items have sort-tables due to the extra resources required.
*/

/* EIA_PT - PGT-Locator
** 2nd array definitions
**	<flags><# pgs>,,<page #>
*/

#define EI_RS1 XWD(0377700,0)	/* reserved (possibly for more item flags) */
#define EI_TPN XWD(0000077,0)	/* Mask for in-file PGT page count */
#define EI_TPG XWD(0,-1)	/* Mask for in-file PGT page # */
struct eiaptent {
	unsigned : (HWD_BITS-6);
	unsigned ei_tpn : 6;	/* in-file PGT page count */
	unsigned ei_tpg : HWD_BITS;	/* in-file PGT page # */
};

/* EIA_IF - Item Flag/name Table
** 3rd array definitions (Item Flags/Names)
**	<flags><# chs>,,<straddr of official item name>
**	If there is no definition for an item, the entry is zero.
*/
struct eiaifent {
	unsigned : 1;
	unsigned ei_flg : HWD_BITS-(1+E_CBTS);	/* 9 or 8 */
	unsigned ef_ich : E_CBTS;	/* Same as ET_ISC stuff */
	unsigned ei_adr : HWD_BITS;
};

#if CPU_PDP10

#define IFL_SRT LBIT(1)	/* Item has a search-table */
#define IFL_CSD LBIT(2)	/* Item is case-dependent */
#define IFL_NUM LBIT(3)	/* Item is numerical */
#define IFL_LNK LBIT(4)	/* Item is a "link" field.  Only set for 0 or 2 items. */
#define IFL_UPD LBIT(5)	/* Item is "Update" field.  Only set for 0 or 1 item. */
#define IFL_SSI LBIT(6)	/* Item has sub-fields. */
#define IFL_WSI LBIT(7)	/* Item has word-fields. */
#define IFL_LSI LBIT(8)	/* Item has line-fields. */
#define IFL_UNQ LBIT(9)	/* Item must be unique.  (Must also have IFL_SRT) */
#endif /* CPU_PDP10 */

#if CPU_M68
#define IFL_SRT LBIT(1)	/* Item has a search-table */
#define IFL_CSD LBIT(2)	/* Item is case-dependent */
#define IFL_NUM 0	/* Item is numerical */
#define IFL_LNK LBIT(3)	/* Item is a "link" field, only set for 0 or 2 items */
#define IFL_UPD LBIT(4)	/* Item is "Update" field, only set for 0 or 1 item. */
#define IFL_SSI LBIT(5)	/* Item has sub-fields. */
#define IFL_WSI LBIT(6)	/* Item has word-fields. */
#define IFL_LSI LBIT(7)	/* Item has line-fields. */
#define IFL_UNQ LBIT(8)	/* Item must be unique.  (Must also have IFL_SRT) */
#endif /* CPU_M68 */

#define IFL_SI (IFL_SSI|IFL_WSI|IFL_LSI)	/* All subitem flags */
#define EI_FLG (IFL_SRT+IFL_CSD+IFL_NUM+IFL_LNK+IFL_UPD+IFL_UNQ+IFL_SI) /* All */


	/* If/when more flags are needed, they can be stored in the */
	/* EIA_PT entry word, and considered "right-half" flags in the */
	/* flag word returned by ITMFLG. */

/* Now can define structure for entire IDT. */
struct idttab {
	struct eiastent stt[ID_NITEMS];
	struct eiaptent ptt[ID_NITEMS];
	struct eiaifent ift[ID_NITEMS];
};

/*-----------------------------------------------------------------------------
** PGT - Page Table		part of sort-table (ST-*) file
**	Type ET_SWD = <flags>,,<sortpage #>   ==>   <1 wd of key bits>
**
** (2nd level of search)
** Each PGT entry describes a single sortpage, which lives in the same file
** as the PGT.  This SET type provides a "stringword" with each entry,
** which the PGT uses to store its key bits for each sortpage.
**	This key is taken from the first entry that the sortpage contains.
** For ASCII-sorted items the stringword consists of the item's
** first 5 ASCII chars shifted 1 bit right, so that the sign bit is 0.
** For numeric-sorted items the word is a 36-bit binary number.
**	Note that the flags are only meaningful while the PGT is in core;
** they are not used for the final version on disk, although they should
** then be cleared just in case.
*/
#define E_PBTS HWD_BITS		/* # bits allowed for representing page #'s. */
#define E_PMAX ((1<<E_PBTS)-1)	/* Max # of pages in a sort-file */

#if 0
#define EP_SGN LBIT(0)	/* Sign bit reserved; keep 0 */
#define EP_PWR LBIT(1)	/* Page-Written flag	(updates can overlay) */
#define EP_PIC LBIT(2)	/* Page-In-Core flag	(RH is SPG entry #) */
/*EP%PNW */		/* Note that the equivalent to V1's EP%TNW (sortpage */
			/* modified, must write out) is furnished by */
			/* the sign bit of the SPG entry, SM%PNW. */
#define EP_PPG XWD(0,-1)	/* Page # of sortpage in sortfile.  If */
			/*    EP$PIC = 1 then this is SPG entry #. */
#endif

struct pgtent {
	unsigned ep_sgn : 1;		/* Sign bit reserved */
	unsigned ep_pwr : 1;		/* Page-written */
	unsigned ep_spg : HWD_BITS-2;	/* SPG # if sortpage in core, else 0 */
	unsigned ep_fpg : E_PBTS;	/* File page # */
};

/*-----------------------------------------------------------------------------
** SRT - Sortpage Table			part of sort-table (ST-*) file
**	Type ET_SRT = <# chrs>,,<straddr>	Recidx stored at straddr-1
**
** (3rd and last level of search)
** All sortpage entries are sorted as appropriate for the item.
** Note one important aspect of sortpage format: the record index #
** associated with each key is stored at <straddr>-1, i.e. in the
** word just preceding the string.
*/


#if 0 /* Comment */

	Future possible changes

As the size of the database grows, it will become necessary to stop treating
the RAT as a completely in-core table.  One possible new architecture is for
the RAT to become a 2-level structure like a sort-table:
	HDR
	  RAT filespec "Database.RAT"
	  RAT pagptr --> RAT-PGT (Type ENT or SWD) within ACCESS or RAT file.
			   Entry points to RATpage in RATfile
			   Could have SWD value with # active/free entries?

	Each RATpage would be simply a page of words, without a SET
structure imposed on it, unlike sortpages.  This is possible because no
strings are ever involved.  Thus, given a RAT index #, one can very
quickly derive the RAT-PGT entry # and the offset within that page:
	RAT-PGT entry = index_<-PG_BTS>
	offset in RATpage = index&PG_MSK

This allows specific RAT entries to be retrieved by means of the RIN% JSYS.
Might be faster to PMAP pages in, however, especially when all pages will
fit.

There is a minor problem with respect to sort-table items when there are
a great many identical items; it takes a long time to find a specific
record reference.  (Imagine the extreme case of several thousand identical
items).  This causes some functions like record deletion (removal of
sort-table entry) and cross-checking (verification that entry exists) to
be very slow.  Some possible solutions/improvements:
	Keep "back pointers" in each record, pointing to its sorttable entries.
		Difficult or impossible.
	Use a flag in the sort-table entry to indicate it's the same as
		previous entry.  Allows faster retrieval of "next" entry
		so scanning is sped up.
		Still wastes a lot of space, but avoids yet another structure.
	For large runs of identical entries, have a pointer (identified by
		a flag in the entry) to a table of record index numbers,
		all of which have the same item.  Saves lots of space and
		allows most efficient search, but makes things a lot more
		complicated.  SLE format needs changing:
			<slot #>,,<table #><entry #>
		Since 9 bits is 512 (length of sortpage), this works out OK.
	Use a flag in the sort-table entry to indicate whether it's a
		real entry, or a record index # (with same string as the
		entry which starts the "run").
		Not good because can't do binary search.
	Provide another FNDX entry point which asks for a specific
		record index # value; then FNDX can optimize the search
		for this value.

OTHER STUFF

	May wish to provide more support routines for user programs.
The 1022 system has some nice ideas in that respect, such as
	* a "currently selected record group" which functions can set up 
		and operate on.
	* General FIND command routine with ASCIZ keyword args and
		boolean capabilities (sets up a selection group)
	* A SORT routine (presumably capable of using temp files to
		achieve sorts of huge selection groups)

	One way of manipulating a "current record group" would be to
build a temporary sort-table for item 0177, which is always undefined.
Probably still want a simple table (possibly page-structured if large)
of record index #s for general operations, and use temp sort-table for
one-shot sorts.... handy even for small N because keys are extracted.
Should remember details of current ordering in case sort step can be
skipped (because records gathered by running through a sort-table).

	Faster database switching?  Lots of things need to be saved and
restored, but it is possible.  The thing the package really cannot be
coerced into doing is providing access to two or more databases
simultaneously.  In other words, there can only be one "current" database.

	Eventually we may want to run in a non-zero section so as to
manipulate really large databases faster.  This is a long way off however.

#endif /* Comment */

/*------------------------------------------------------------------
** While mapped in core, a SET is represented by a PT, which stands
** for "Pointer Table" (since it points to the appropriate parts of
** the mapped-in SET).
** The internal PT format is defined below:
*/

typedef struct id_pttab_ {
	word_t *pt_adr;		/* Address in core that SET starts at. */
	int pt_shl;		/* SET Header Length in words */
	int pt_ver;		/* Format version (HV_xxx) */
	int pt_etyp;		/* Entry Type (ET_xxx) */
	int pt_ltbl;		/* # entries */
	word_t *pt_tbl;		/* Address of start of entries (in core) */
	int pt_lfre;		/* # words free */
	word_t *pt_free;	/* Address of start of free words (in core) */
	int pt_ltot;		/* # words total in SET */
	int pt_garb;		/* # garbage words */
	word_t *pt_strs;	/* Address of start of strings (in core) */
	pagptr_t pt_cpgs;	/* page area spec in core */
	pagptr_t pt_fpgs;	/* page area spec in file */
	int pt_jfn;		/* JFN for file pages mapped from */
	int pt_fd;		/* FD for file pages mapped from */
} idpt_t;


/* FB (File Block) definition.  There is one FB for each file
** being used by IFPACK, except for sortfiles.
*/
#ifndef MAXPATHLEN
#define MAXPATHLEN (4*40+10)
#endif
struct filblk {
	/* Only used when opening file, not when active */
	char *f_ext;	/* Default file extension to use if needed */
	char *f_inpt;	/* File pathname input spec */

	/* Can specify version if opening; set to version # if active */
	int f_ver;	/* File version # (-1 if none) */

	/* Only set when active (opened) */
	int f_fd;	/* FD for file if active, else 0 */
	int f_jfn;	/* JFN for FD if active (on T20/10X) */
	int f_mode;	/* Mode file opened with (O_ flags) */
	char f_name[MAXPATHLEN];	/* Holds entire pathname if active */
};


/* Sortpage table definitions */

#ifndef ID_SPGBFL
#define ID_SPGBFL (4*512)	/* Fix up later */
#endif
#if (E_PMAX - ID_SPGBFL) < 0
#define C_ERROR "IIV_SPGTAB too big (or sortfile size too small)"
#endif

	/* SPGTAB (sortpage table) entry format */
#define	SM_PGT_BITS 14	/* # bits for PGT entry # */

struct spgent {
	unsigned sm_pnw : 1;	/* Page-Needs-Writing flag */
	unsigned sm_itm : 7;	/* Item # */
	unsigned sm_pag : 14;	/* Page # in memory */
	unsigned sm_pgt :	/* PGT entry # in sort-table for item */
			SM_PGT_BITS;
};
extern struct spgent iiv_spgtab[ID_SPGBFL];

#ifndef NEWSTG
#define NEWSTG 1
#endif
#if !NEWSTG
extern word_t iiv_pagtab[PG_NUM];	/* Page table */
#else
extern word_t *iiv_pagtab;	/* Pointer to page table portion */
#endif

/* Error signal definitions
*/

#ifndef _SETJMP_INCLUDED	/* Unix lossage: ensure don't re-include */
#include <setjmp.h>		/* No, so include it */
#define _SETJMP_INCLUDED 1
#endif

#define ES_RESET (iiv_esptr = iiv_esiptr)
#define ES_TRAP (_setjmp(++iiv_esptr))	/* Use in "if (ES_TRAP) { }" */
#define ES_FREE  (--iiv_esptr)
#define ERRMSG   ii_esmsg		/* Uppercase to stand out better */
#define DBERRMSG ii_esdbmsg
#define ERRSIG   ii_eserr
#define DBERRSIG ii_esdberr
extern void
	ERRMSG(), DBERRMSG(),	/* Build error message */
	ERRSIG(), DBERRSIG();	/* Same but trigger signal too */

#define LSYSERR ii_lsyserr()
extern char *ii_lsyserr();
extern jmp_buf *iiv_esiptr;	/* Initial ptr to error-trap stack */
extern jmp_buf *iiv_esptr;	/* Current ptr to error-trap stack */

/* Declarations of IDB internal functions shared among modules
*/
extern char *ii_estrcpy(), *ii_ememcpy(), *ii_esfmt();	/* IDB */
extern int ii_getcrx(), ii_getpgt();			/* IDB */
extern void ii_oldinit();				/* IDBIFP */

/* Declare internal variables.
** VOID is the only user program that references them.
*/
extern idpt_t
	id_pthdr,
	id_ptift,
	id_ptilt,
	id_ptidt,
	id_ptrat,
	id_ptsrt,
	id_ptpgt,
	id_ptwit,
	id_ptscr,
	id_ptsc2;

extern int
	id_errflg,
	id_iniflg,
	id_ldflg,
	id_selflg,
	id_pgtitm,
	id_ifpver;
extern idrad_t	id_recrte;
extern long	id_recfln;

/* filblks defined by ifpack file handling stuff */
extern struct filblk
	id_fbrec, id_fbarec,
	id_fbacc, id_fbnac,
	id_fbrat, id_fbwlok, id_fbscr;


#endif /* ifndef _IDBDEF_INCLUDED */
