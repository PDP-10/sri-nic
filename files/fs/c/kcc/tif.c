/* TIF.C - Test #if handling of preprocessor
**	Compile with -E only.
**
**	Currently this is mostly to check out the error messages
**	generated when conditionals are unbalanced across files or at EOF.
*/
#if 1			/* This will be unterminated at EOF */

#include "tif1.c"	/* Generate warning cuz this has if w/o endif */
#endif

#if 1
#include "tif2.c"	/* Generate warning cuz this endifs top-level if */

#include "tif3.c"

/* Now check unterminated conditionals at EOF */
#include "tif1.c"		/* Get an #if from a file that will vanish */
#ifdef FOO
#else
