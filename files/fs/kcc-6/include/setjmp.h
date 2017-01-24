/* <SETJMP.H> - KCC definitions for setjmp() and longjmp() facilities.
**
**	(c) Copyright Ken Harrenstien 1989
**
**	Implements 4.3BSD notions.
** Note that jmp_buf must be an array type so that references to a
** jmp_buf object will be either an array (definition) or a pointer to
** an array (declaration).  Sigh!  --KLH
**	We use a sigcontext structure for jmp_buf even though we don't
** need all of the elements.  The reason for this is to make sure we have
** enough room for any future features, and to allow possible future use
** of the 4.3BSD sigreturn() system call.
**	sc_acs[2] is used to hold a simple checksum of the important parts.
*/

#ifndef _SETJMP_INCLUDED
#define _SETJMP_INCLUDED
#ifndef __STDC__	/* Canonicalize this indicator to avoid err msgs */
#define __STDC__ 0
#endif

#include <signal.h>		/* Get sigcontext definition */

typedef struct sigcontext jmp_buf[1];	/* Define jmp_buf */

#if __STDC__			/* ANSI prototype decls */
extern int setjmp(jmp_buf),	/* Declare setjmp functions */
	_setjmp(jmp_buf);
extern void longjmp(jmp_buf, int),
	_longjmp(jmp_buf, int);
#define longjmperror _ljerr	/* Reduce name to 6-char external */
extern void longjmperror(void);	/* Routine called if longjmp finds problems */

#else				/* Old pre-ANSI style */
extern int setjmp(), _setjmp();	/* Declare setjmp functions */
extern void longjmp(), _longjmp();
#define longjmperror _ljerr	/* Reduce name to 6-char external */
extern void longjmperror();	/* Routine called if longjmp finds problems */
#endif

#endif /* ifndef _SETJMP_INCLUDED */
