/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, Trustees of Columbia University in the City of New York.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained. 
*/
/* Internal symbols, declarations, and storage allocations required by
** the system modules of the ccmd package.  This file is generated
** automatically by m4 from information in cmfnc.h4 and the various
** cm???.cnf configuration files.  The initial portion of this file,
** including this comment, is copied verbatim from the file cmfnc.top.
*/


#include <ctype.h>		/* needed by many function handlers */
#undef ispunct			/* we have our own idea of punctuation */
#define ispunct(c) (!(isdigit(c) || isupper(c) || islower(c)))

/*
** The ftspec structure contains information for parsing a particular
** field type, including pointers to three handler functions, and a pointer
** to a default break table.  The parsing handler is invoked when an
** attempt is made to parse the current input using this field type.
** The help handler is invoked to provide a standard help message for
** this field.  The completion handler is invoked when the user has
** requested completion for this field type.  Calling conventions are as
** follows:
**
** _ftprs (parse handler):
**   Input arguments:
**      text - A pointer to the text to be parsed.
**      textlen - The number of characters in the text.
**      fdbp - A pointer to the FDB that caused this handler to be invoked.
**
**   Output arguments:
**     parselen - The number of characters consumed by a successful parse.
**     value - A pointer to a data item to be filled with the result of
**       a successful parse.  (The data item is not allocated by the
**       parse handler -- the passed pointer must point to an existing 
**	 item.)
**
**   Returns: Standard return code.
**
** _fthlp (help handler):
**   Input arguments:
**      text - A pointer to the text being parsed.
**      textlen - The number of characters in text.
**      fdbp - A pointer to the FDB that caused this handler to be invoked.
**      cust - TRUE if a custom help string was typed for this FDB
**
**   Output arguments: None.
**
**   Returns: Standard return code.
**
** _ftcmp (completion handler):
**   Input arguments:
**     text - A pointer to the text being parsed.
**     textlen - The number of characters in text.
**     fdbp - A pointer to the FDB that caused this handler to be invoked.
**     full - TRUE if full completion is requested, otherwise only partial
**       completion is required.
**
**   Output arguments:
**     cplt - A pointer to the completion text to be filled in.  NULL
**       means no completion text.
**     cpltlen - If cplt is not NULL, the number of characters to fill
**       in.  -1 means fill in to the end of the completion string
**       (zero terminated).
**
**   Returns: Flags (defined below) whether or not to:
**     - Beep at the user (flag CMP_BEL)
**     - Add a space after the completion text (flag CMP_SPC)
**     - Stop completion after first punctuation char (flag CMP_PNC)
**     - Wake up the parse (flag CMP_GO)
**/

typedef struct FTSPEC {
  int (*_ftprs)();		/* parse handler address */
  int (*_fthlp)();		/* help handler address */
  int (*_ftcmp)();		/* completion handler address */
  int _ftflg;			/* field type flags */
  brktab *_ftbrk;		/* standard break table */
} ftspec;
  
/* Flags defined for ftspec structure (_ftflg field) */

#define	FT_DFX	0x0001		/* blocks defaulting on confirm */

/* Flags to be returned by completion handlers */

#define CMP_BEL	0x0001		/* beep at the user */
#define CMP_SPC	0x0002		/* add space after completion text */
#define CMP_GO	0x0004		/* wakeup requested */
#define CMP_PNC	0x0008		/* end completion after first punctuation */

/* global break table shared by some parse functions */

extern brktab cmallbk;

/* Error table, indexed by the left byte of an error code,
** points to a function error structure, which includes a count
** of the number of error codes in the table, and a pointer to
** an array of error strings, indexed by the right byte of the
** error code.
**/

typedef struct FNERR {
  int _fecnt;			/* number of error strings in table */
  char **_ferrs;		/* pointer to array of error strings */
} fnerr;

/* badfnc - Macro to decide whether a given function code is legal.
** First, the code is checked for a range error.  If it passes, the
** function table entry is checked for a NULL parse function pointer,
** indicating that the function has been stubbed out of this application.
**/

#define badfnc(code)	(((code) < 1) || ((code) > cmfmax) ? TRUE : \
			 (cmfntb[(code)-1]->_ftprs == NULL))

/* Some useful character constants */

#define NULCHAR	'\0'
#define	BELL	'\007'
#define BS	'\010'
#define	TAB	'\t'		/* ASCII 011 */
#define	NEWLINE	'\n'		/* ASCII 012 */
#define FORMFEED '\014'
#define	RETURN	'\r'		/* ASCII 015 */
#define SPACE	'\040'
#define DELETE	'\177'

/*
 * Macros to inspect break tables:
 *  BREAK1(b,c) => nonzero iff character c is a 1st character break in
 *    break table *b.
 *  BREAKR(b,c) => nonzero iff character c is a non-1st character break in
 *    break table *b.
 *  BREAK(b,c,p) => nonzero iff character c is a break character in position
 *    p according to break table *b.  (p should be 0 to check for 1st char
 *    break, nonzero otherwise.)
 */

#define	BREAK1(b,c)	(0x80 & ((b)->_br1st[(c)/8] << ((c) % 8)))
#define	BREAKR(b,c)	(0x80 & ((b)->_brrest[(c)/8] << ((c) % 8)))
#define BREAK(b,c,p)	((p) == 0 ? BREAK1(b,c) : BREAKR(b,c))


/* Generic ccmd error messages */ 

#ifdef GENERR

static char *(generr[21]) = {
        "No error",
        "Reparse required",
        "Unable to parse input",
        "Unknown function code in FDB",
        "Command buffer overflow",
        "Atom buffer overflow",
        "End of file encountered on command input",
        "More input required for successful parse",
        "Empty FDB chain presented for parsing",
        "Wakeup requested",
        "Deferred action requested",
        "Automatic reparse mechanism failed or not available",
        "Default string could not be inserted",
        "Internal work buffer overflow",
        "Error in command line I/O operation",
        "Must allocate parse buffers before beginning parse",
        "Must issue prompt before parsing command line",
        "Retry requested by parse routine",
        "Automatic parse error handler failed",
        "Completion routine requested a bell",
        "Completion routine requested a bell and wakeup"
};
fnerr fe_gen = { 21, generr };
#else
extern fnerr fe_gen;
#endif

/* Error table and other configuration data for cmcfm */


#ifdef CFMERR

static char *(cfmerr[1]) = {
        "Not confirmed"
};
fnerr fe_cfm = { 1, cfmerr };
#else
extern fnerr fe_cfm;
#endif

/* Error table and other configuration data for cmkey */


#ifdef KEYERR

static char *(keyerr[3]) = {
        "Does not match keyword",
        "Ambiguous",
        "Invalid abbreviation chain in keyword table"
};
fnerr fe_key = { 3, keyerr };
#else
extern fnerr fe_key;
#endif

/* Error table and other configuration data for cmnum */


#ifdef NUMERR

static char *(numerr[4]) = {
        "Radix must be from 2 to 16",
        "Number must be unsigned",
        "Not a valid number in specified radix",
        "Number too large for this machine"
};
fnerr fe_num = { 4, numerr };
#else
extern fnerr fe_num;
#endif

/* Error table and other configuration data for cmqst */


#ifdef QSTERR

static char *(qsterr[3]) = {
        "First character is not a valid delimiter",
        "Invalid character inside delimited string",
        "Not a proper delimiter"
};
fnerr fe_qst = { 3, qsterr };
#else
extern fnerr fe_qst;
#endif

/* Error table and other configuration data for cmnoi */


#ifdef NOIERR

static char *(noierr[1]) = {
        "Invalid noise word"
};
fnerr fe_noi = { 1, noierr };
#else
extern fnerr fe_noi;
#endif

/* Error table and other configuration data for cmtxt */


#ifdef TXTERR

static char *(txterr[1]) = {
        "Illegal characters in line of text"
};
fnerr fe_txt = { 1, txterr };
#else
extern fnerr fe_txt;
#endif

/* Error table and other configuration data for cmfld */


#ifdef FLDERR

fnerr fe_fld = { 0, NULL };
#else
extern fnerr fe_fld;
#endif

/* Error table and other configuration data for cmswi */


#ifdef SWIERR

static char *(swierr[5]) = {
        "Does not match switch",
        "Ambiguous",
        "Invalid abbreviation chain in switch table",
        "Switch does not begin with required punctuation",
        "Final switch punctuation not allowed on selected switch"
};
fnerr fe_swi = { 5, swierr };
#else
extern fnerr fe_swi;
#endif

/* Error table and other configuration data for cmtok */


#ifdef TOKERR

static char *(tokerr[1]) = {
        "Does not match token string"
};
fnerr fe_tok = { 1, tokerr };
#else
extern fnerr fe_tok;
#endif

/* Error table and other configuration data for cmtad */


#ifdef TADERR

static char *(taderr[4]) = {
        "Both time and date are suppressed in time/date parse",
        "Invalid time",
        "Invalid date",
        "Invalid time and date"
};
fnerr fe_tad = { 4, taderr };
#else
extern fnerr fe_tad;
#endif

/* Error table and other configuration data for cmfil */


#ifdef FILERR

static char *(filerr[6]) = {
        "Does not match filename",
        "Ambiguous",
        "Cannot specify wildcards in nonwild parse",
        "Invalid combination of parse flags",
        "Invalid file spec",
        "Ambiguous"
};
fnerr fe_fil = { 6, filerr };
#else
extern fnerr fe_fil;
#endif

/* Error table and other configuration data for cmusr */


#ifdef USRERR

static char *(usrerr[2]) = {
        "Does not match username",
        "ambiguous"
};
fnerr fe_usr = { 2, usrerr };
#else
extern fnerr fe_usr;
#endif

/* Error table and other configuration data for cmgrp */


#ifdef GRPERR

static char *(grperr[2]) = {
        "Does not match group name",
        "ambiguous"
};
fnerr fe_grp = { 2, grperr };
#else
extern fnerr fe_grp;
#endif

/* Error table and other configuration data for cmpara */


#ifdef PARAERR

static char *(paraerr[1]) = {
        "Out of memory"
};
fnerr fe_para = { 1, paraerr };
#else
extern fnerr fe_para;
#endif

#ifdef STORAGE

/* Function handler table */

extern ftspec
  ft_cfm, ft_key, ft_num, ft_qst, ft_noi, ft_txt, ft_fld, ft_swi, ft_tok, 
  ft_tad, ft_fil, ft_usr, ft_grp, ft_para ;

ftspec *(cmfntb[14]) = {
  &ft_cfm, &ft_key, &ft_num, &ft_qst, &ft_noi, &ft_txt, &ft_fld, &ft_swi, 
  &ft_tok, &ft_tad, &ft_fil, &ft_usr, &ft_grp, &ft_para 
};

/* Error table table */

fnerr *(fnetab[15]) = {
  &fe_gen, &fe_cfm, &fe_key, &fe_num, &fe_qst, &fe_noi, &fe_txt, &fe_fld, 
  &fe_swi, &fe_tok, &fe_tad, &fe_fil, &fe_usr, &fe_grp, &fe_para 
};

/* Number of defined functions */

int cmfmax = 15;

#else
extern ftspec *(cmfntb[]);
extern fnerr *(fnetab[]);
extern int cmfmax;
#endif
