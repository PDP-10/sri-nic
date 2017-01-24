/*----------------------------------------------------------------------#
 |
 |                           Command package
 |
 :----------------------------------------------------------------------#
 |
 |	This command package provides a standard interface to both the
 |	TOPS-20 COMND/COMND% library, and the Columbia CCMD C-based
 |	package.
 |
 |	The native character type used in character strings must be
 |	used as cm_char, since on TOPS-20 the COMND% jsys demands
 |	that all strings be 7-bit.  Also, strings themselves must be
 |	constructed with cm_str(); on T20, a string pointer points to
 |	the character BEFORE the start of the string, while on Un*x
 |	it points to the first char itself.
 |
 |	All external symbols defined in this package start with "cm";
 |	functions are named cm_xxx.  Structure member symbols are
 |	named cmX_yyy where X is 'R' for return values, 'F' for FDB
 |	members, 'T' for keyword tables, and 'B' for buffers.
 |
 %----------------------------------------------------------------------*/

#include <c-env.h>

/*----------------------------------------------------------------------#
 |
 |                    TOPS-20 COMND/COMND% interface
 |
 %----------------------------------------------------------------------*/

#if SYS_T20

#include "jsys.h"
#include "stdio.h"
#ifndef _SETJMP_INCLUDED
#include "setjmp.h"
#define _SETJMP_INCLUDED 1
#endif

#define cm_char		_KCCtype_char7
#define cm_str(s)	(((cm_char *) s) - 1)

/*----------------------------------------------------------------------#
 |
 |                       Function Decriptor Block
 |
 :----------------------------------------------------------------------*/

typedef struct {
    int word1;			/* 32 bits, left-justified per word: ^@ - ^_ */
    int word2;			/* <space> - '?' */
    int word3;			/* '@' - '_' */
    int word4;			/* '`' - \177 (rubout) */
} cm_brktab;

#define cm_break(char)		(1 << (35 - (char % 32)))

typedef struct CM_FDB {
    unsigned cmF_function:9,	/* function code */
		cmF_flags:9,	/* flag bits */
		 cmF_next:18;	/* address of next fdb */
    cm_char *cmF_data;		/* data for function */
    cm_char *cmF_help;		/* help text */
    cm_char *cmF_def;		/* default text */
    cm_brktab *cmF_break_mask;	/* pointer to break mask */
    struct CM_FDB *cmF_next_fdb;/* next fdb in chain; moved to next later */
} cm_fdb;

#define cm_def_fdb(name, funct, flags, data, help, def, brk, next) \
    cm_fdb name = { \
	funct, ((flags) >> 18), 0, \
	(cm_char *) data, help, def, brk, next \
    };

/*----------------------------------------------------------------------#
 |
 |	This is the long-form GTJFN block used for file-type parses
 |
 %----------------------------------------------------------------------*/

typedef struct gtjfn_block_struct {
    int flags;				/* flags,,generation */
    unsigned ijfn:18, ojfn:18;		/* input,,outjfn jfn */
    cm_char *device;			/* default device, */
    cm_char *directory;			/* directory, */
    cm_char *name;			/* name, */
    cm_char *extension;			/* extension, */
    cm_char *protection;		/* protection, */
    cm_char *account;			/* and account strings. */
    int jfn;				/* jfn to associate with filespec */
    unsigned xflags:18, count:18;	/* xtra flags,,# remaining words */
    cm_char *typescript;		/* gadzooks, read the jsys man */
    int buf_count;			/* space in above buffer */
    cm_char *prompt;			/* ^R prompt */
    int obsolete;			/* obsolete, don't use */
    int *atr_block;			/* pointer to attribute block */
} gtjfn_block;

/*----------------------------------------------------------------------#
 |
 |                            Keyword Table
 |
 :----------------------------------------------------------------------#
 |
 |	Since we can't create the right format keyword table on the
 |	fly, we'll have to do it at runtime.
 |
 %----------------------------------------------------------------------*/

#define CM_MAX_KEYWORD		40		/* max length of keyword */

typedef struct cm_tent_struct {
    int *cmT_data;				/* data, only RH is good */
    int cmT_flags;				/* optional flags */
    cm_char cmT_key[CM_MAX_KEYWORD];		/* key string */
} cm_tent;

#define cm_key_tent		cm_tent
#define cm_switch_tent		cm_tent

typedef struct cm_table_struct {
    int cmT_count;				/* # entries in table */
    cm_tent *cmT_entries;			/* ptr to internal table */
    int cmT_size;				/* size of table */
/* --- The following are for TOPS-20 only, to faciliate building directly
   --- formatted keyword tables. --- */
    int cmT_internalized;			/* been internalized yet? */
    int *cmT_intab;				/* internal-format table */
} cm_table;

#define cm_table_begin(cname) \
    cm_tent cname[] = {

#define cm_table_entry(key, flags, data) \
    { (int *) (data), (int) flags, key },

#define cm_table_end(name, cname, iname) \
    }; \
    static int iname[(sizeof(cname) / sizeof(cm_tent)) + 1]; \
    cm_table name = { \
	sizeof(cname) / sizeof(cm_tent), \
	cname, 0, 0, iname \
    };

#define cm_dynamic_table(name, size, exname, inname) \
    cm_tent exname[size]; \
    int inname[size + 1]; \
    cm_table name = { 0, exname, size, 0, inname };

typedef struct {
    unsigned cmT_key:18;		/* LH is addr of string */
    int cmT_value:18;			/* RH is signed data value */
} cm_tbluk_table_entry;

/*----------------------------------------------------------------------#
 |
 |                             State Block
 |
 :----------------------------------------------------------------------#
 |
 |	This is the COMND state block, which contains flags, JFNs,
 |	pointers and counts for buffers, main input and atom buffers,
 |	a jmp_buf for handling reparse and error-restart, and 3 return
 |	values.
 |
 |	Note that the top of this struct (through cmgjb) is an exact copy
 |	of the T20 COMND% jsys state block.  on purpose.
 |
 %----------------------------------------------------------------------*/

#define CM_IBUF_SIZE	4096	/* plenty big for main input buffer */
#define CM_ABUF_SIZE	1024	/* plenty big for an atom buffer */

struct cm_state {
    unsigned flags:18, reparse:18;	/* flag bits and reparse address */
    unsigned ijfn:18, ojfn:18;		/* input and output jfns */
    cm_char *cmrty;			/* pointer to ^R text */
    cm_char *cmbfp;			/* pointer to start of buffer */
    cm_char *cmptr;			/* pointer to next input to parse */
    int cmcnt;				/* count of space left in buffer */
    int cminc;				/* count of unparsed characters */
    cm_char *cmabp;			/* pointer to atom buffer */
    int cmabc;				/* size of that atom buffer */
    gtjfn_block *cmgjb;			/* pointer to GTJFN block */
    jmp_buf rep_buf;			/* for saving state so can reparse */
    jmp_buf err_buf;			/* jump back here on parse errors */
    int r_flags;			/* flag word returned by COMND% */
    union {
	struct {
	    int ts_table_data;
	    cm_char *ts_keyword;
	} table_stuff;		/* table data/keyword (KEY,SWI) */
	int cmR_number;		/* number typed by user (NUM) */
	int cmR_fd;		/* FD for file parses (FIL,OFI,IFI) */
	int cmR_directory;	/* directory number of dir (DIR) */
	int cmR_user;		/* user number for usr (USR) */
	float cmR_float;	/* floating-point value (FLT) */
    } r_data;
    cm_fdb *r_used;			/* which cm_fdb was actually used */
    cm_char cmB_input[CM_IBUF_SIZE];	/* main input buffer */
    cm_char cmB_atom[CM_ABUF_SIZE];	/* atom buffer */
};

extern struct cm_state cm_state_block;

#define cm_data		cm_state_block.r_data
#define cm_used		cm_state_block.r_used

#define cmR_table_data	table_stuff.ts_table_data
#define cmR_table_key	table_stuff.ts_keyword

/*----------------------------------------------------------------------#
 |
 |                   Function and macro declarations.
 |
 :----------------------------------------------------------------------*/

#define cm_reparse_jmpbuf		cm_state_block.rep_buf
#define cm_error_jmpbuf			cm_state_block.err_buf

#define cm_clear_fdb_flags(fdb, bits)	fdb.flags &= ~(bits >> 18)
#define cm_confirm()			cm_parse(&cmcfm)
#define cm_done()			/* no-op */
#define cm_link_fdb(old, new)		((old)->cmF_next = (int) new)
#define cm_set_error_trap()		setjmp(cm_error_jmpbuf)
#define cm_set_file_block(fblk)		(cm_state_block.cmgjb = &fblk)
#define cm_set_fdb_flags(fdb, bits)	fdb.flags |= (bits >> 18)
#define cm_set_prompt(s)		cm_prompt_set(s)
#define cm_set_reparse()		setjmp(cm_reparse_jmpbuf)
#define cm_set_table(fdb, table)	(fdb.cmF_data = (cm_char *) table)
#define cm_start_parse()		cm_parse(&cmini)

extern void cm_init();			/* initialize for parsing */
extern void cm_prompt_set();		/* set the prompt */
extern int cm_parse();			/* do an actual parse */
extern int cm_noise();			/* do a noise parse */
extern void cm_terpri();		/* get to a fresh line for output */
extern void cm_do_reparse();		/* come here to effect a reparse */
extern int cm_intern_keytab();		/* internalize a keyword table */
extern int *cm_global_ptr();		/* !!!gross necessary kludge!!! */
extern void cm_setup_gtjfn();		/* setup gtjfn block for file parse */

extern cm_table *cm_make_table();	/* make a dynamic table */
extern int cm_put_table();		/* add entry to dynamic table */
extern int cm_finalize_table();		/* finalize a dynamic table */
extern int cm_release_table();		/* release dynamic table */
extern int cm_table_compare();		/* for qsort'ing */

extern cm_fdb cmini, cmcfm, cmcma;	/* static, supplied FDBs */

#endif /* end SYS_T20 */

#if SYS_UNIX

/*----------------------------------------------------------------------#
 |
 |                        Unix/CCMD definitions
 |
 %----------------------------------------------------------------------*/

#include <local/ccmd.h>

#define cm_brktab	brktab
#define cm_char		char
#define cm_str(s)	s		/* no special handling needed */
#define cm_table	keytab
#define cm_tent		keywrd
#define cm_fdb		fdb

/*----------------------------------------------------------------------*
 |
 |                            Define an FDB
 |
 %----------------------------------------------------------------------*/

#define CM_HPP		0		/* presence of string instead of */
#define CM_DPP		0		/* flag bits turns on actions... */
#define CM_BRK		0

#define CM_INV		KEY_INV		/* give standard T20 bitnames to */
#define CM_ABR		KEY_ABR		/* the ccmd codes... */
#define CM_NOR		KEY_NOR

#define cm_def_fdb(name, funct, flags, data, help, def, brk, next) \
    static fdb name = { funct, flags, next, (pdat) data, help, def, brk };

/*----------------------------------------------------------------------*
 |
 |                            Keyword stuff
 |
 %----------------------------------------------------------------------*/

#define cm_table_begin(cname) \
    static keywrd cname[] = {

#define cm_table_entry(key, flags, data) \
	{ (char *) key, (short) flags, (keyval) data },

#define cm_table_end(name, cname, iname) \
	{ NULL, 0, 0 } \
    }; \
    static keytab name = { (sizeof(cname) / sizeof(keywrd)) - 1, cname };

#define cm_dynamic_table(name, size, exname, inname) \
    cm_tent exname[size]; \
    cm_table name = { 0, exname, size };

/*----------------------------------------------------------------------#
 |
 |                               Storage
 |
 %----------------------------------------------------------------------*/

pval cm_data;				/* parse-value */
cm_fdb *cm_used;			/* ptr to fdb which was used */
cm_char *cm__prompt;

#define cm_state_block	cmcsb

#define cmB_atom	_cmabp
#define cmB_input	_cmwbp		/* work buffer?  is this right? */

#define cmF_function	_cmfnc
#define cmF_help	_cmhlp

#define cmR_table_data	_pvkey
#define cmR_table_key	_pvkwd
#define cmR_number	_pvint
#define cmR_fd		_pvfil
#define cmR_float	_pvflt

#define cmT_count	_ktcnt
#define cmT_entries	_ktwds
#define cmT_size	_ktsiz
#define cmT_key		_kwkwd

/*----------------------------------------------------------------------#
 |
 |                                Macros
 |
 %----------------------------------------------------------------------*/

#define cm_confirm()		confirm()
#define cm_done()		cmdone()
#define cm_error_jmpbuf		cmerjb
#define cm_finalize_table	dynfinish
#define cm_global_ptr(p)	p
#define cm_init(b, c)		cm_set_prompt(c); cmini()
#define cm_link_fdb(old, new)	(old)->_cmlst = (fdb *) new
#define cm_noise(s)		noise(s)
#define cm_parse(fdb)		parse(fdb, &cm_data, &cm_used)
#define cm_put_table		dynput
#define cm_reparse_jmpbuf	cmrpjb
#define cm_set_error_trap();	cmseter()
#define cm_set_prompt(s)	cm__prompt = s
#define cm_set_reparse()	cmsetrp()
#define cm_start_parse()	prompt(cm__prompt)
#define cm_terpri()		cmxbol()

#endif /* SYS_UNIX */
