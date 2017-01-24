/* SETJMP.H - KCC definitions for setjmp() and longjmp() facilities.
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

#include <signal.h>		/* Get sigcontext definition */

typedef struct sigcontext jmp_buf[1];

#define longjmperror _ljerr	/* Reduce name to 6-char external */
