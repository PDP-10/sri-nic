/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* ccmdmd.h
**
** This file contains machine-dependent macros and declarations for
** the ccmd package.  Use preprocessor conditionals to include the
** correct code, but PLEASE -- try not to nest conditionals!!
**/

/* Data type that can accomodate int and all pointer data in an
** initializer.  This is how the _cmdat field of the FDB is
** declared, to ease declaration of FDB structures.
**/

typedef char * pdat;

/* On Venix, one cannot pass around pointers to static functions
** because of code mapping difficulties.  Therefore, the following is
** used for functions that really should be static, but which need to
** be invoked via pointers in other modules.  Since they will not be
** static in venix implementations, they should be named so as to reduce
** chances of name conflicts.
**/

#ifdef VENIX
#define PASSEDSTATIC
#else
#define PASSEDSTATIC static
#endif

/*
** cmsetrp macro - invoke at an appropriate
** reparse point, to install a handler into the CSB that will automatically
** revert execution back to this point whenever a reparse is triggered.
**/

#ifdef LATTICE
#define cmsetrp() \
   { setjmp(&cmrpjb); cmcsb._cmrph = cmrpjmp; }
#else
#define cmsetrp() \
   { setjmp(cmrpjb); cmcsb._cmrph = cmrpjmp; }
#endif

/*
** cmseter macro - invoke just prior to issuing a prompt,
** to install a handler into the CSB that will automatically
** revert execution back to this point whenever a parse error
** is encountered.
**/

#ifdef LATTICE
#define cmseter() \
   { setjmp(&cmerjb); cmcsb._cmerh = cmerjmp; }
#else
#define cmseter() \
   { setjmp(cmerjb); cmcsb._cmerh = cmerjmp; }
#endif

#ifdef LATTICE
#define cmsetbl() \
   { setjmp(&cmbljb); cmcsb._cmblh = cmbljmp; }
#else
#define cmsetbl() \
   { setjmp(cmbljb); cmcsb._cmblh = cmbljmp; }
#endif
#define cmclrbl() \
   { cmcsb._cmblh = NULL; }

#include "setjmp.h"
extern jmp_buf cmrpjb;		/* global jump buffer for auto reparse */
extern jmp_buf cmerjb;		/* global jump buffer for auto error */
extern jmp_buf cmbljb;		/* global jump buffer for nonblocking io */
extern cmrpjmp();		/* reparse handler - longjmps via cmrpjb */
extern cmerjmp();		/* error handler - longjmps via cmerjb */
extern cmbljmp();		/* error handler - longjmps via cmerjb */



