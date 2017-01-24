/*==================================================
 *  comnd.h
 *	#include file for COMND% jsys support for the V7 C library
 *
 *	Copyright (C) New Mexico Tech 1984, 1985.  All rights reserved.
 *
 *	Author: Greg Titus
 */

/*
 *  Has this file already been #included?  Matching #endif is at bottom of file
 */
#ifndef COMND_INCLUDED

#define COMND_INCLUDED

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
 *  the null device
 */
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
 *  the code for the COMND% jsys
 */
#ifndef COMND_
#   define COMND_   0544
#endif

/*
 *	a few handy error codes
 */
#ifndef _ERR_IOX4
    /*
     *	end-of-file reached
     */
#   define _ERR_IOX4	0600220
#endif _ERR_IOX4

/*
 *  the type of a command state block
 */
typedef struct
{
    unsigned int    csb_cmflg;		/* flags word */
    unsigned int    csb_cmioj;		/* input/output file designators */
    char	    *csb_cmrty;		/* prompt/reprompt text pointer */
    char	    *csb_cmbfp;		/* text buffer start pointer */
    char	    *csb_cmptr;		/* next input text pointer */
    int		    csb_cmcnt;		/* count of space left in buffer */
    int		    csb_cminc;		/* count of chars left in buffer */
    char	    *csb_cmabp;		/* atom buffer start pointer */
    int		    csb_cmabc;		/* size of atom buffer */
    int		    *csb_cmgjb;		/* GTJFN% argument block address */
} csb_entry;

/*
 *  bit masks and positions for the command state block flag halfword,
 *  reparse address halfword, and input and output jfns
 */
#define CSB_FLAGS_MASK	0777777000000
#define CSB_FLAGS_POS	0
#define CSB_RPAR_MASK	0777777
#define CSB_RPAR_POS	0
#define CSB_IJFN_MASK	0777777
#define CSB_IJFN_POS	18
#define CSB_OJFN_MASK	0777777
#define CSB_OJFN_POS	0

/*
 *  command state block flag bits
 *
 *  CM_ESC (returned):	field was terminated with <ESC> (ascii 033 octal)
 *  CM_NOP (returned):	field could not be parsed (ignore CM_ESC and CM_EOC)
 *  CM_EOC (returned):	field was terminated with <CR> (ascii 015 octal)
 *  CM_RPT (returned):	already-parsed chars need reparsing (they got edited)
 *  CM_SWT (returned):	a switch field was terminated with a colon
 *  CM_PFE (returned):	previous field was terminated with <ESC>
 *  CM_RAI (passed):	convert lowercase input to uppercase
 *  CM_XIF (passed):	do not recognize '@' as designating indirect file
 *  CM_WKF (passed):	begin parsing after each field;  do not wait for an
 *			action character (<CR>, <ESC>, <CTRL/F>, ?)
 */
#define CM_ESC	    (1 << 35)
#define CM_NOP	    (1 << 34)
#define CM_EOC	    (1 << 33)
#define CM_RPT	    (1 << 32)
#define CM_SWT	    (1 << 31)
#define CM_PFE	    (1 << 30)
#define CM_RAI	    (1 << 29)
#define CM_XIF	    (1 << 28)
#define CM_WKF	    (1 << 27)

/*
 *  the type of a command function descriptor block
 */
typedef struct
{
    unsigned int    fdb_cmfnp;		/* function code word */
    int		    fdb_cmdat;		/* function-specific data */
    char	    *fdb_cmhlp;		/* help text pointer */
    char	    *fdb_cmdef;		/* default text pointer */
    int		    *fdb_cmbrk;		/* break mask pointer, if CM_BRK */
    int		    fdb_next_index;	/* next chain index for _com_graph() */
} fdb_entry;

/*
 *  bit masks and positions for the command function descriptor block function
 *  code, flags, and next-function-block pointer
 */
#define FDB_CODE_MASK	0777
#define FDB_CODE_POS	27
#define FDB_FLAGS_MASK	0777000000
#define FDB_FLAGS_POS	0
#define FDB_NFDB_MASK	0777777
#define FDB_NFDB_POS	0

/*
 *  fdb function codes
 *
 *  _CMKEY: parse a keyword;  data is address of TBLUK% keyword index table;
 *	    bits in argument blocks pointed to by table entries are:
 *	    CM_INV: suppress this keyword in the list output on '?'
 *	    CM_NOR: do not recognize this keyword
 *	    CM_ABR: make this keyword a valid abbreviation for another
 *  _CMNUM: parse a number;  data is radix (2 through 10, inclusive)
 *  _CMNOI: parse a guide word (noise);  data is un-parenthesized noise string
 *  _CMSWI: parse a switch;  data is address of TBLUK% switch index table;  see
 *	    also bits for argument blocks, defined for _CMKEY above
 *  _CMIFI: parse an input file spec
 *  _CMOFI: parse an output file spec
 *  _CMFIL: parse an arbitrary file spec
 *  _CMFLD: parse an arbitray field;  data is pointer to character buffer into
 *	    which comnd_() (possibly via com_graph()) will copy the contents of
 *	    the atom buffer on successful parse;  NULL for none
 *  _CMCFM: parse a confirm
 *  _CMDIR: parse a directory name;  data is one of:
 *	    CM_DWC: allow wildcard characters
 *  _CMUSR: parse a user name (login directory name)
 *  _CMCMA: parse a comma
 *  _CMINI: initialize the command line
 *  _CMFLT: parse a floating-point number
 *  _CMDEV: parse a device name
 *  _CMTXT: parse text up to a <CR>;  data is pointer to character buffer into
 *	    which comnd_() (possibly via com_graph()) will copy the contents of
 *	    the atom buffer on successful parse;  NULL for none
 *  _CMTAD: parse a date and/or/time field;  data (left) is bits:
 *	    CM_IDA: parse a date
 *	    CM_ITM: parse a time
 *	    CM_NCI: do not convert date and/or time to internal format
 *	    data (right) is address of three-word block for unconverted
 *	    date/time (CM_NCI off)
 *  _CMQST: parse a quoted string;  data is pointer to character buffer into
 *	    which comnd_() (possibly via com_graph()) will copy the contents of
 *	    the atom buffer on successful parse;  NULL for none
 *  _CMUQS: parse an unquoted string up to one of the given break chars;  data
 *	    is address of four-word block of 128 break char mask bits
 *  _CMTOK: parse and compare to string;  data is pointer to string
 *  _CMNUX: parse number, terminated by non-numeric char;  data is radix (2
 *	    through 10, inclusive)
 *  _CMACT: parse an account string;  data is pointer to character buffer into
 *	    which comnd_() (possibly via com_graph()) will copy the contents of
 *	    the atom buffer on successful parse;  NULL for none
 *  _CMNOD: parse a network node name;  data is pointer to character buffer
 *	    into which comnd_() (possibly via com_graph()) will copy the
 *	    contents of the atom buffer on successful parse;  NULL for none
 */
#define _CMKEY	    000
#define CM_INV	    (1 << 0)
#ifndef CM_NOR
#   define CM_NOR   (1 << 1)
#endif
#define CM_ABR	    (1 << 2)
#define _CMNUM	    001
#define _CMNOI	    002
#define _CMSWI	    003
#define _CMIFI	    004
#define _CMOFI	    005
#define _CMFIL	    006
#define _CMFLD	    007
#define _CMCFM	    010
#define _CMDIR	    011
#define CM_DWC	    (1 << 35)
#define _CMUSR	    012
#define _CMCMA	    013
#define _CMINI	    014
#define _CMFLT	    015
#define _CMDEV	    016
#define _CMTXT	    017
#define _CMTAD	    020
#define CM_IDA	    (1 << 35)
#define CM_ITM	    (1 << 34)
#define CM_NCI	    (1 << 33)
#define _CMQST	    021
#define _CMUQS	    022
#define _CMTOK	    023
#define _CMNUX	    024
#define _CMACT	    025
#define _CMNOD	    026

/*
 *  fdb function word flag bits
 *
 *  CM_NSF: suffix is optional (meaningful only for _CMDEV and _CMNOD)
 *  CM_BRK: .fdb_cmbrk contains pointer to four-word break mask
 *  CM_PO:  this is a parse-only field (meaningful only for _CMDEV, _CMDIR,
 *	    _CMNOD, and _CMUSR)
 *  CM_HPP: pointer to user-supplied help string is in .fdb_cmhlp
 *  CM_DPP: pointer to user-supplied default string is in .fdb_cmdef
 *  CM_SDH: suppress output of default help message if user types '?'
 */
#define CM_NSF	    (1 << 23)
#define CM_BRK	    (1 << 22)
#define CM_PO	    (1 << 21)
#define CM_HPP	    (1 << 20)
#define CM_DPP	    (1 << 19)
#define CM_SDH	    (1 << 18)

/*
 *  macro to fill in an initialize (_CMINI) fdb
 */
#define flddb_ini(f)	    flddb_((f),_CMINI,0,0,NULL,NULL,NULL,NULL)

/*
 *  macro to fill in a noise (_CMNOI) fdb
 */
#define flddb_noi(f,s,n)    flddb_((f),_CMNOI,0,(s),NULL,NULL,(n),NULL)

/*
 *  macro to fill in a comma (_CMCMA) fdb
 */
#define flddb_cma(f,n)	    flddb_((f),_CMCMA,0,0,NULL,NULL,(n),NULL)

/*
 *  macro to fill in a confirm (_CMCFM) fdb
 */
#define flddb_cfm(f,n)	    flddb_((f),_CMCFM,0,0,NULL,NULL,(n),NULL)

/*
 *  offsets of important words in a long-form GTJFN% argument table
 */
#define GJAT_GJGEN_OFF	0
#define GJAT_GJDEV_OFF	2
#define GJAT_GJDIR_OFF	3
#define GJAT_GJNAM_OFF	4
#define GJAT_GJEXT_OFF	5
#define GJAT_GJPRO_OFF	6
#define GJAT_GJACT_OFF	7
#define GJAT_GJJFN_OFF	8
#define GJAT_GJATR_OFF	14

/*
 *  the type of an fdb chain head
 */
typedef struct
{
    fdb_entry	    *fch_fdbs;		/* addr of chain of fdbs */
    int		    *fch_gtj_tab;	/* pointer to long-form GTJFN% block */
    int		    fch_fail_index;	/* next-chain index for failure here */
} fdbchn_head;

/*
 *  the type of chain-matched entry
 */
typedef struct
{
    fdb_entry	    *fcm_fdb;		/* matched fdb for this chain */
    int		    fcm_extra_info;	/* other match info for this chain */
} fdb_match_entry;

/*==================================================
 *	P U B L I C   F U N C T I O N S
 *
 */

/*
 *  the command state block filler
 */
extern void	    csb_setup();

/*
 *  the command function descriptor block filler
 */
extern fdb_entry    *flddb_();

/*
 *  the fdb chain head descriptor filler
 */
extern void	    fdbc_setup();

/*
 *  the COMND% jsys executor, and its return values
 */
extern int	    comnd_();

#define COMND_SUCCESS	1
#define COMND_REPARSE	0
#define COMND_ERROR	(-1)

/*
 *  the COMND% graph executor, and its return values
 */
extern int	    com_graph();

#define COMGR_FULL	2
#define COMGR_SUCCESS	1
#define COMGR_REPARSE	0
#define COMGR_ERROR	(-1)

#endif	/* COMND_INCLUDED */
