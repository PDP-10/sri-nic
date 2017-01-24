/* SLEEP - sleep for N seconds
**	Copyright (c) 1987 by Ken Harrenstien, SRI International
*/

#include "c-env.h"
#include "sys/usysig.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

/* sleep(secs)	- Sleep for N seconds
** unsigned secs;
*/
void
sleep(n)
int n;
{
#if SYS_T20+SYS_10X
    int acs[5];
    acs[1] = n * 1000;			/* Multiply by 1000 to get msec */
    jsys(DISMS|JSYS_OKINT, acs);	/* Sleep, return if signal */

#elif SYS_WAITS+SYS_T10
    asm("	MOVE 1,-1(17)\n");	/* Get argument into AC 1 */
    asm("	SLEEP 1,\n");

#elif SYS_ITS
    asm("	MOVE 1,-1(17)\n");	/* Get argument into AC 1 */
    asm("	IMULI 1,036\n");	/* Multiply by 30 to get 30ths */
    asm("	.SLEEP 1,\n");

#else
#error sleep() not supported for this system.
#endif
}
