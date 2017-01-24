/*==================================================
 *  tb.h
 *	#include file for TBxxx% jsys support for the V7 C library
 *
 *	Copyright (C) New Mexico Tech 1984, 1985.  All rights reserved.
 *
 *	Author: Greg Titus
 */

/*
 *  Has this file already been #included?  Matching #endif is at bottom of file
 */
#ifndef TB_INCLUDED

#define TB_INCLUDED

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
 *  macros to set and extract bit fields
 */
#ifndef putbits
#   define putbits(m,p,x)	(((x) & (m)) << (p))
#endif

#ifndef getbits
#   define getbits(m,p,x)	(((x) >> (p)) & (m))
#endif

/*
 *  the codes for the TBxxx% jsys's
 */
#ifndef TBADD_
#   define TBADD_   0536
#endif
#ifndef TBDEL_
#   define TBDEL_   0535
#endif
#ifndef TBLUK_
#   define TBLUK_   0537
#endif

/*
 *  the type of a TBxxx% command table
 *
 *  NOTE 1: this structure presumes that the compiler generates code in
 *	    such a way that member .tbc_count immediately precedes member
 *	    .tbc_entry[] in the address space, with no intervening words
 *  NOTE 2: no member can be defined in this structure after member
 *	    .tbc_entry[], because the addressing for such a member will be
 *	    wrong for any instance of the structure in which TB_TAB_SIZE is
 *	    defined as something other than the value it is given below
 */
#ifndef TB_TAB_SIZE
#   define TB_TAB_SIZE	100
#endif
typedef struct
{
    unsigned int    tbc_count;		/* # entries in left;  max in right */
					/* the table entries */
    unsigned int    tbc_entry[TB_TAB_SIZE];
} tb_comtab;

/*
 *  bit masks and positions for the TBxxx% table entry count and address
 *  fields
 */
#define TB_CURRSIZ_MASK	0777777
#define TB_CURRSIZ_POS	18
#define TB_MAXSIZ_MASK	0777777
#define TB_MAXSIZ_POS	0

/*
 *  the type of a tb_build() argument block array entry
 */
typedef struct
{
    unsigned int    tbbe_flags;		/* this arg block's flags */
    char	    *tbbe_string;	/* this arg block's string */
} tbb_entry;

/*
 *  bit masks and positions for the TBxxx% table entry address and user-defined
 *  fields
 */
#define TBE_ADDR_MASK	0777777
#define TBE_ADDR_POS	18
#define TBE_UDEF_MASK	0777777
#define TBE_UDEF_POS	0

/*
 *  argument block first-word flag bits
 *
 *  CM_FW:  remainder of word is flag bits
 *  CM_NOR: do not recognize this string
 */
#ifndef CM_FW
#   define CM_FW    (1 << 28)
#endif
#ifndef CM_NOR
#   define CM_NOR   (1 << 1)
#endif

/*==================================================
 *	P U B L I C   F U N C T I O N S
 *
 */

/*
 *  the table builder, and its return values
 *
 *  TBB_BADSIZ:	    maximum table size is less than current table size, or is
 *		    zero (table was not built)
 *  TBB_SUCCESS:    table built successfully
 *  TBB_OVERWRITE:  the string of some argument for which flags were specified
 *		    did not begin with either five spaces or five nuls (this is
 *		    only a warning -- other than this problem, the table was
 *		    built correctly)
 *  TBB_UNSORTED:   the keyword strings were not in sorted order (this is only
 *		    a warning -- other than this problem, the table was built
 *		    correctly)
 *
 *  NOTE:   if the problems indicated by TBB_OVERWRITE and TBB_UNSORTED are
 *	    both present, TBB_OVERWRITE is returned (on the supposition that
 *	    if the overwritten keywords had had the proper blank word on the
 *	    the front of them, the keyword strings would have been in order)
 */
extern int	    tb_build();
#define TBB_BADSIZ	(-1)
#define TBB_SUCCESS	0
#define TBB_OVERWRITE	1
#define TBB_UNSORTED	2

#endif	/* TB_INCLUDED */
