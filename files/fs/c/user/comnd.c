/* -*- C -*- */
/*
 *	COMND interface for TOPS-20
 */

#include <c-env.h>
#if SYS_T20			/* Everything in file depends on this! */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/file.h>
#include <nic/comnd.h>
#include <nic/strung.h>

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

cm_def_fdb(cmini, _CMINI, 0, 0, 0, 0, 0, 0)	/* initialize for parse */
cm_def_fdb(cmcfm, _CMCFM, 0, 0, 0, 0, 0, 0)	/* parse confirmation */
cm_def_fdb(cmcma, _CMCMA, 0, 0, 0, 0, 0, 0)	/* parse a comma */
cm_def_fdb(cmnoi, _CMNOI, 0, 0, 0, 0, 0, 0)	/* parse for noise string */

struct cm_state cm_state_block;		/* pointer to users state block */

#define CSB	cm_state_block

/************* Forward-Referenced Procedure Declarations ************/
static int	cm_open_file();		/* open given file, read or write */

/************* Externally-Referenced Procedure Declarations *************/
extern int	open();			/* open or create a file */
extern char    *strerror();		/* get string for last error */
extern char    *strncpy();		/* copy chars between strings */

/*
 *	initialize for parsing.  this declares the users state block,
 *	initializes it, and then initializes the FDBs and keyword tables
 *	pointed to by the fdbs arg.
 */

void cm_init(fdbs, prompt)
cm_fdb *fdbs[];					/* array of all user FDBs */
cm_char *prompt;
{
    cm_fdb *fdb;
    cm_table *tp;			/* pointer to table structure */
    int i;				/* pointer to internalized keytab */
					/* this is the input buffer */
    CSB.cmbfp = CSB.cmptr = CSB.cmB_input - 1;
    CSB.cmcnt = CM_IBUF_SIZE;		/* it holds n 7-bit bytes */
    CSB.cmabp = CSB.cmB_atom - 1; 	/* this is the atom buf */
    CSB.cmabc = CM_ABUF_SIZE;		/* size of atom buffer */
    CSB.ijfn = _PRIIN;			/* input from primary in */
    CSB.ojfn = _PRIOU;			/* output to primary out */
    CSB.reparse = (int) cm_do_reparse;	/* set reparse address */
    CSB.cmrty = prompt;			/* pointer to ^R prompt */
    for (i = 0; fdb = fdbs[i]; i++) {
	switch (fdb->cmF_function) {
	    case _CMKEY: case _CMSWI:
		tp = (cm_table *) fdb->cmF_data;/* data is ptr to key table */
		if (!tp->cmT_internalized)	/* if it hasnt been interned */
		    cm_intern_keytab(tp);	/* then do it now. */
		fdb->cmF_data = (cm_char *) tp->cmT_intab;
		break;				/* point FDB to it */
	}
	if (fdb->cmF_next_fdb)			/* need to chain that fdb? */
	    fdb->cmF_next = (int) fdb->cmF_next_fdb;	/* yes, so do it */
    }
}

/*
 *	set up the noise fdb for your string then parse for noise
 */

int cm_noise(s)
cm_char *s;
{
    cmnoi.cmF_data = s;			/* point to your string */
    return cm_parse(&cmnoi);		/* and do the parse! */
}

/*
 *	do an actual parse.  returns 0 if the jsys failed, or CM%NOP was
 *	set (another failure indication).  else the return values are
 *	stored in the users cm_ret structure and 1 is returned.
 */
static int cm_dbgflg = 0;
static void cm_show();

int cm_parse(fdb)
cm_fdb *fdb;
{
    int *key_p, ablock[5];
    cm_tbluk_table_entry *tp;

    ablock[1] = (int) &CSB;
    ablock[2] = (int) fdb;
    if ((jsys(COMND, ablock) < 1) || (ablock[1] & CM_NOP)) {
	cm_terpri();				/* get to new line */
	printf("?%s\n", strerror(_ERRNO_LASTSYSERR));	/* output sys error */
	longjmp(cm_error_jmpbuf,0);		/* jump to user error */
    }						/* restart point. */
    CSB.r_flags = ablock[1];			/* save returned flags, */
    CSB.r_used = (cm_fdb *) cm_global_ptr(ablock[3]);
    switch (CSB.r_used->cmF_function) {
	case _CMKEY: case _CMSWI:
	    tp = (cm_tbluk_table_entry *) cm_global_ptr(ablock[2] & RH);
	    key_p = (int *) cm_global_ptr(tp->cmT_key);
	    if ((*key_p & CM_FW) && !(*key_p & 0770000000000))
		key_p++;
	    CSB.r_data.cmR_table_key = (cm_char *) key_p;
	    CSB.r_data.cmR_table_data = tp->cmT_value;
	    break;
	case _CMNUM:
	    CSB.r_data.cmR_number = ablock[2];
	    break;
	case _CMOFI:
	    CSB.r_data.cmR_fd = cm_open_file(ablock[2], FALSE);
	    break;
	case _CMIFI:
	    CSB.r_data.cmR_fd = cm_open_file(ablock[2], TRUE);
	    break;
	case _CMFIL:
	    CSB.r_data.cmR_fd = cm_open_file(ablock[2],
					     (CSB.cmgjb->flags&GJ_OLD));
	    break;
	case _CMDIR:
	    CSB.r_data.cmR_directory = ablock[2];
	    break;
	case _CMUSR:
	    CSB.r_data.cmR_user = ablock[2];
	    break;
	case _CMFLT:
	    CSB.r_data.cmR_float = (float) ablock[2];
	    break;
    }
    if (cm_dbgflg) cm_show();		/* Show results if debugging. */
    return TRUE;
}

/*
 *	given a JFN and a direction (TRUE for input, FALSE for output),
 *	open the given file, returning an FD to it, or fatal error-reset.
 */

static int cm_open_file(jfn, direction)
int jfn, direction;
{
    int fd;

    fd = open((char*) jfn, O_SYSFD | ((direction) ? O_RDONLY : O_WRONLY), jfn);
    if ((CSB.r_data.cmR_fd = fd) < 0) {
	cm_terpri();
	printf("?Can't open file -- %s\n", strerror(_ERRNO_LASTSYSERR));
	longjmp(CSB.err_buf,0);
    }
    return fd;
}

/*
 *	turn an 18-bit FDB pointer into the real thing
 */

int *cm_global_ptr(a)
int a;
{
    asm("hrrz 1,-1(17)\n xmovei 1,(1)\n");
}

/*
 *	internalize a keyword table.  given a pointer to the raw table,
 *	gets the address of the destination internal-format array from
 *	the key field of the first element.  the first word of the internal
 *	keytab has already been filled in, so the keys and data just need
 *	to be copied over.  too bad this can't be done at compile-time!
 */

int cm_intern_keytab(table)
cm_table *table;
{
    int i, *rt, *key_p;				/* raw table pointer */
    cm_tent *p;

    if (table->cmT_count > 0 && !table->cmT_intab) {
	rt = (int *) calloc(1, (table->cmT_count + 1) * sizeof(int));
	if (!rt) return FALSE;
	table->cmT_intab = rt;			/* this is internal table */
    } else
	rt = table->cmT_intab;			/* point to internal-format */
    *rt = (table->cmT_count << 18) | (table->cmT_size & 0777777);
    for (p = table->cmT_entries, i = 0; i < table->cmT_count; p++, i++) {
	key_p = (p->cmT_flags) ? (int *) &p->cmT_flags : (int *) p->cmT_key;
	*++rt = ((int) key_p << 18) | ((int) p->cmT_data & 0777777);
    }
    return table->cmT_internalized = TRUE;	/* it's been internalized */
}

/* CM_SHOW - for debugging, show result of a successful call to cm_parse.
 */
static void
cm_show()
{
    int fun = CSB.r_used->cmF_function;
    char *name = NULL;
    static struct fnam { int val; char *str; } *fp, fnam[] = {
	_CMKEY, "KEY",
	_CMNOI, "NOI",
	_CMNUM, "NUM",
	_CMSWI, "SWI",
	_CMIFI, "IFI",
	_CMOFI, "OFI",
	_CMFIL, "FIL",
	_CMFLD, "FLD",
	_CMCFM, "CFM",
	_CMDIR, "DIR",
	_CMUSR, "USR",
	_CMCMA, "CMA",
	_CMINI, "INI",
	_CMFLT, "FLT",
	_CMDEV, "DEV",
	_CMTXT, "TXT",
	_CMTAD, "TAD",
	_CMQST, "QST",
	_CMUQS, "UQS",
	_CMTOK, "TOK",
	_CMNUX, "NUX",
	_CMACT, "ACT",
	_CMNOD, "NOD",
	0, NULL
	};

    for (fp = fnam; fp->str; ++fp)	/* Look up name for function */
	if (fun == fp->val) {
	    name = fp->str;
	    break;
	}
    if (!name) {
	fprintf(stderr, "[?%d?: \"%s\" %#o]", fun, CSB.cmabp+1, CSB.r_used);
	return;
    }
    fprintf(stderr, "[%s: \"%s\" %#o => ", name, CSB.cmabp+1, CSB.r_used);

    switch (fun) {
	case _CMKEY:
	case _CMSWI:
	    fprintf(stderr,"\"%s\" val %d",
			CSB.r_data.cmR_table_key,
			CSB.r_data.cmR_table_data);
	    break;

	case _CMNUM:
	    fprintf(stderr,"%d. %#o",
			CSB.r_data.cmR_number, CSB.r_data.cmR_number);
	    break;
	case _CMNOI:
	    fprintf(stderr, "(%s)", CSB.r_used->cmF_data + 1);
	    break;
	case _CMOFI:
	case _CMIFI:
	case _CMFIL:
	    fprintf(stderr,"fd %d", CSB.r_data.cmR_fd);
	    break;
	case _CMDIR:
	    fprintf(stderr,"dir %#o", CSB.r_data.cmR_directory);
	    break;
	case _CMUSR:
	    fprintf(stderr,"user %#o", CSB.r_data.cmR_user);
	    break;
	case _CMFLT:
	    fprintf(stderr,"%f", CSB.r_data.cmR_float);
	    break;
    }
    fprintf(stderr, "]");
}

/*
 *	dynamic table building routines
 */

int cm_put_table(table, key, flags, data)
cm_table *table;
cm_char *key;
int flags;
int *data;
{
    cm_tent *tp;

    if (table->cmT_count >= table->cmT_size)
	return FALSE;				/* table full */
    tp = &table->cmT_entries[table->cmT_count++];
    strncpy(tp->cmT_key, key, CM_MAX_KEYWORD);
    tp->cmT_flags = flags;
    tp->cmT_data = data;
    return TRUE;				/* success!!! */
}

int cm_finalize_table(table)
cm_table *table;
{
    if (table->cmT_count)
	qsort((char *) table->cmT_entries, table->cmT_count,
	    sizeof(cm_tent), cm_table_compare);
    return cm_intern_keytab(table);
}

int cm_table_compare(s1, s2)
cm_char *s1, *s2;
{
    return strCMP(((cm_tent *) s1)->cmT_key, ((cm_tent *) s2)->cmT_key);
}

/*
 *	set the prompt
 */

void cm_prompt_set(s)
cm_char *s;
{
    CSB.cmrty = s;			/* set it in state block */
}

/*
 *	setup a gtjfn block
 */

void cm_setup_gtjfn(gjb, flags, dev, dir, name, ext)
gtjfn_block *gjb;
int flags;
cm_char *dev, *dir, *name, *ext;
{
    CSB.cmgjb = gjb;			/* use this gtjfn_block */
    gjb->flags = flags;
    gjb->ijfn = gjb->ojfn = 0;
    gjb->device = dev;				/* default device */
    gjb->directory = dir;			/* default directory */
    gjb->name = name;				/* default filename */
    gjb->extension = ext;			/* default extension */
    gjb->protection = gjb->account = NULL;	/* none of that junk */
    gjb->jfn = gjb->xflags = gjb->count = 0;	/* no xtra gubbish */
}

/*
 *	when reparse hits, we come here.  the user should have set their
 *	reparse address with a cm_set_reparse() command before doing this,
 *	which would have setjmp'd the state at that point into a jmp_buf
 *	in the users cm_state structure.  on reparse, we longjmp back to
 *	that saved state (and location).
 */

void cm_do_reparse()
{
    longjmp(CSB.rep_buf, 0);
}

/*
 *	do a terpri, which means gets to the beginning of a fresh line.
 *	if we're already at the beginning of a line, do nothing
 */

void cm_terpri()
{
    int ablock[5];

    ablock[1] = _PRIOU;					/* JFN */
    if (jsys(RFPOS, ablock) && (ablock[2] & RH))	/* in middle of line?*/
	putchar('\n');					/* yes, get to new */
}

#endif /* SYS_T20 */
