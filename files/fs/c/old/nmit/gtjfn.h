/*==================================================
 *  gtjfn.h
 *	#include file for GTJFN% jsys support for the V7 C library
 *
 *	Copyright (C) New Mexico Tech 1984, 1985.  All rights reserved.
 *
 *	Author: Greg Titus
 */

/*
 *  Has this file already been #included?  Matching #endif is at bottom of file
 */
#ifndef GTJFN_INCLUDED

#define GTJFN_INCLUDED

/*==================================================
 *	O V E R V I E W
 *
 *
 */

/*==================================================
 *	E X T E R N A L   D A T A
 *
 */

/*
 *  empty
 */

/*==================================================
 *	E X T E R N A L   F U N C T I O N S
 *
 */

/*
 *  empty
 */

/*==================================================
 *	P R I V A T E   D A T A
 *
 */

/*
 *  empty
 */

/*==================================================
 *	P U B L I C   D A T A
 *
 */

/*
 *  some devices
 */
#ifndef _PRIIN
#   define _PRIIN	0100
#endif
#ifndef _PRIOU
#   define _PRIOU	0101
#endif
#ifndef _NULIO
#   define _NULIO   0377777
#endif

/*
 *  macros to set and extract bit fields
 */
#ifndef putbits
#   define putbits(m,p,x)	(((x) & (m)) << (p))
#endif

#ifndef getbits
#   define getbits(m,p,x)	(((x) >> (p)) & (m))
#endif

/*
 *  the code for the GTJFN% jsys
 */
#ifndef GTJFN_
#   define GTJFN_   020
#endif

/*
 *  the type of a GTJFN% long form argument table
 */
typedef struct
{
    unsigned int    ga_gjgen;		/* flags in left;  gen # in right */
    unsigned int    ga_gjsrc;		/* input/output jfns */
    char	    *ga_gjdev;		/* default device string */
    char	    *ga_gjdir;		/* default directory string */
    char	    *ga_gjnam;		/* default filename string */
    char	    *ga_gjext;		/* default extension string */
    char	    *ga_gjpro;		/* default protection string */
    char	    *ga_gjact;		/* default account string */
    unsigned int    ga_gjjfn;		/* jfn to associate with filespec */
    unsigned int    ga_gjf2;		/* begin extended block;  flags word */
    char	    *ga_gjcpp;		/* copy of user's typescript */
    int		    ga_gjcpc;		/* size of ga_gjcpp buffer (bytes) */
    char	    *ga_gjrty;		/* CTRL/R reprompt string */
    char	    *ga_gjbfp;		/* obsolete (destination buffer) */
    int		    *ga_gjatr;		/* ptr to filespec attribute block */
} gtjf_argtab;

/*
 *  bit masks and positions for the GTJFN% argument table flags and generation
 *  number word, the input and output jfn word, and the extended argument block
 *  flags and length word
 */
#define GA_FLAGS_MASK	0777777000000
#define GA_FLAGS_POS	0
#define GA_GEN_MASK	0777777
#define GA_GEN_POS	0
#define GA_IJFN_MASK	0777777
#define GA_IJFN_POS	18
#define GA_OJFN_MASK	0777777
#define GA_OJFN_POS	0
#define GA_EFLAGS_MASK	0777777000000
#define GA_EFLAGS_POS	0
#define GA_ELEN_MASK	0777777
#define GA_ELEN_POS	0

/*
 *  long form argument table flag bits
 *
 *  GJ_FOU: create a new version
 *  GJ_NEW: file must not exist
 *  GJ_OLD: file must exist
 *  GJ_MSG: type a message if the user presses <ESC> to terminate input
 *  GJ_CFM: confirmation (<CR>) from the user is required
 *  GJ_TMP: file is temporary
 *  GJ_NS:  search only the first spec of a multiple logical name definition
 *  GJ_ACC: jfn cannot be accessed by inferior processes
 *  GJ_DEL: ignore the "file deleted" bit in the fdb
 *  GJ_JFN: must be off for short form of GTJFN%;  for long form, force
 *	    association with the jfn in .ga_gjjfn -- field's values are:
 *	_GJDNU:	ignore the jfn supplied
 *	_GJERR:	attempt to return the supplied jfn;  return error on failure
 *	_GJALT:	attempt to return the supplied jfn;  use another on failure
 *  GJ_IFG: file spec can contain wildcard characters
 *  GJ_OFG: associate jfn with filespec string, not file (parse-only jfn)
 *  GJ_FLG: return flags in ac1 on successful return from GTJFN%
 *  GJ_PHY: use physical device (ignore job-wide logical names)
 *  GJ_XTN: must be off for short form of GTJFN%;  for long form, argument
 *	    block contains more than ten (octal) words
 *  GJ_FNS: for short form of GTJFN%, where to get filespec string;  ignored
 *	    for long form
 *  GJ_SHT: must be on for short form of GTJFN%;  off for long form
 */
#ifndef GJ_FOU
#   define GJ_FOU   (1 << 35)
#endif
#ifndef GJ_NEW
#   define GJ_NEW   (1 << 34)
#endif
#ifndef GJ_OLD
#   define GJ_OLD   (1 << 33)
#endif
#define GJ_MSG	    (1 << 32)
#define GJ_CFM	    (1 << 31)
#ifndef GJ_TMP
#   define GJ_TMP   (1 << 30)
#endif
#define GJ_NS	    (1 << 29)
#define GJ_ACC	    (1 << 28)
#define GJ_DEL	    (1 << 27)
#define GJ_JFN	    (3 << 25)
#define	    _GJDNU  (0 << 25)
#define	    _GJERR  (2 << 25)
#define	    _GJALT  (3 << 25)
#define GJ_IFG	    (1 << 24)
#define GJ_OFG	    (1 << 23)
#ifndef GJ_FLG
#   define GJ_FLG   (1 << 22)
#endif
#define GJ_PHY	    (1 << 21)
#define GJ_XTN	    (1 << 20)
#ifndef GJ_FNS
#   define GJ_FNS   (1 << 19)
#endif
#ifndef GJ_SHT
#   define GJ_SHT   (1 << 18)
#endif

/*
 *  values for the generation number halfword in the long form GTJFN% argument
 *  table (other than specific generation numbers)
 *
 *  _GJDEF: use next higher gen # if GJ_FOU is on;  highest if not
 *  _GJNHG: use next higher gen # if none is supplied
 *  _GJLEG: use lowest existing gen # if none is supplied
 *  _GJALL: use all gen #s (GJ_IFG must be on)
 */
#define _GJDEF	    0
#define _GJNHG	    ((-1) & 0777777)
#define _GJLEG	    ((-2) & 0777777)
#define	_GJALL	    ((-3) & 0777777)

/*
 *  bits in GTJFN% long form extended argument table flags word
 *
 *  G1_RND: return when filename buffer is empty and user tries to delete
 *  G1_NLN: length(name) <= 6 chars;  length(extension) <= 3 chars
 *  G1_RCM: put confirmation message in destination buffer
 *  G1_RIE: return when input buffer is empty and user tries to delete
 *  G1_IIN: consider invisible files also
 *  G1_SLN: prohibit logical name expansion
 */
#define G1_RND	    (1 << 35)
#define G1_NLN	    (1 << 33)
#define G1_RCM	    (1 << 32)
#define G1_RIE	    (1 << 31)
#define G1_IIN	    (1 << 30)
#define G1_SLN	    (1 << 29)

/*
 *  returned bits in left halfword of indexable file handle (if GJ_IFG, GJ_OFG,
 *  or GJ_FLG was on)
 *
 *  GJ_DEV: device field had wildcards
 *  GJ_UNT: unit field had wildcards (never happens)
 *  GJ_DIR: directory field had wildcards
 *  GJ_NAM: name field had wildcards
 *  GJ_EXT: extension field had wildcards
 *  GJ_VER: version (generation number) field had wildcards
 *  GJ_UHV: file used has highest generation number (_GJDEF)
 *  GJ_NHV: file used has next higher generation number (_GJDEF or _GJNHG)
 *  GJ_ULV: file used has lowest generation number (_GJLEG)
 *  GJ_ACT: account field was given in filespec
 *  GJ_TFS: file is temporary
 *  GJ_GND: did not consider deleted files (GJ_DEL was off)
 *  GJ_GIV: did not consider invisible files (G1_IIN was off)
 */
#define GJ_DEV	    (1 << 35)
#define GJ_UNT	    (1 << 34)
#define GJ_DIR	    (1 << 33)
#define GJ_NAM	    (1 << 32)
#define GJ_EXT	    (1 << 31)
#define GJ_VER	    (1 << 30)
#define GJ_UHV	    (1 << 29)
#define GJ_NHV	    (1 << 28)
#define GJ_ULV	    (1 << 27)
#define GJ_ACT	    (1 << 25)
#define GJ_TFS	    (1 << 24)
#define GJ_GND	    (1 << 23)
#define GJ_GIV	    (1 << 18)

#endif	/* GTJFN_INCLUDED */
