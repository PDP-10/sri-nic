/* EXIT, _EXIT - Terminate Process.
**
**	Copyright (c) 1987 by Ken Harrenstien, SRI International
*/

#include "c-env.h"
#include "stdlib.h"		/* For onexit() stuff */
#include "sys/usysig.h"
#include "sys/usysio.h"

extern int _vfrkf, _nfork;	/* Defined in URT, set by fork() and vfork() */
extern void __exit();		/* From CRT module, lowest-level exit */

/* EXIT - Top-level exit.
**	This is not actually a USYS call; it is more of a general
** library function.  Nevertheless it is closely associated with _exit() which
** IS a USYS call, and is documented in the UPM as exit(2), and therefore
** is coded here.
*/
void
exit(n)
int n;
{
    int i;
    void _exit();

    for (i = _n_exit_func; i > 0;)	/* If there are exit func registered */
	(*_exit_func[--i])();		/* call them reverse order, no args */
    _exit(n);				/* Then do URT simulation exit */
}

/* _EXIT - syscall simulation exit
**	This is the "middle-level" exit (in between exit() and __exit()).
** It cleans up all USYS stuff, including open FDs and child processes.
*/
void
_exit(n)
int n;
{
    int i;

    USYS_BEG();				/* No interrupts from here on. */
    if (_vfrkf)				/* If mem shared by vfork(), */
	__exit(n);			/* then just halt immediately. */
    for (i = 0; i < OPEN_MAX; i++)	/* For all FDs */
	if (_uioufx[i])			/* close FD if it's open */
	    close(i);
    if (_nfork)				/* If we created any forks, */
	while (wait(&i) != -1);		/* ensure all are flushed. */
    __exit(n);				/* go do lowest level exit */
}
