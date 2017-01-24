/* CLOCK - returns process CPU run time
**	As described in [H&S v2 sec 20.1]
** All of these PDP-10 implementations return time in milliseconds
** (that is, CLK_TCK is 1000)
*/

#include "c-env.h"
#include "time.h"

clock_t
clock()
{
#if SYS_T20
	asm("SEARCH MONSYM\n");
	asm("	MOVEI 1,.FHSLF\n");	/* For this process, */
	asm("	RUNTM%\n");		/* get runtime in AC 1 */

#elif SYS_10X
	asm("SEARCH MONSYM\n");
	asm("	JOBTM\n");		/* Puts result in AC 1 */

#elif SYS_WAITS+SYS_T10
	asm("	SETZ	1,\n");	/* For our job, */
	asm("	RUNTIM	1,\n");	/* get runtime in return AC */

#elif SYS_ITS
	asm("	.SUSET [.RRUNT,,1]\n");	/* Get runtime in 4.096 usec units */
	asm("	MULI 1,^D4069\n");	/* Convert to nanoseconds, */
	asm("	DIV 1,[^D1000000]\n");	/*   then to milliseconds. */
#else
#error clock() not supported for this system.
#endif
}
