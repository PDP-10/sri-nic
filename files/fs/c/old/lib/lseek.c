/*
** lseek - low level support for random access files
** David Eppstein / Stanford University / 14-Aug-84
*/

#include <c-env.h>
#include <urtio.h>

#if SYS_T20+SYS_10X
#define SFPTR 027			/* set file pointer */
#define RFPTR 043			/* read file pointer */
#endif /* T20+10X */

long lseek(fildes, offset, whence)
long offset;
{
#if SYS_T20+SYS_10X
    int ablock[5], jfn;

    if ((jfn = _uioch[fildes]) < 1) return -1; /* no such file descriptor */
    switch (whence) {
    case L_XTND:			/* ptr = eof + offset */
	ablock[1] = jfn;		/* AC1: JFN */
	ablock[2] = -1;			/* AC2: -1 */
	if (!jsys(SFPTR, ablock)) return -1; /* go to EOF */

    case L_INCR:			/* ptr += offset */
	ablock[1] = jfn;		/* AC1: JFN */
	if (!jsys(RFPTR, ablock)) return -1; /* read ptr into AC2 */
	if (offset == 0) return ablock[2]; /* no change, just ret ptr */
	offset += ablock[2];

    case L_SET:				/* ptr = offset */
	ablock[1] = jfn;		/* AC1: JFN */
	ablock[2] = offset;		/* AC2: offset */
	if (!jsys(SFPTR, ablock)) return -1; /* go to ptr */
	return offset;			/* return what we set it to */

    default:				/* unknown setting */
	return -1;			/* return badness */
    }
#else /* T20+10X */
	return(-1);		/* Not implemented elsewhere yet */
#endif
}

long tell(fildes)
{
    return lseek(fildes, 0L, L_INCR);
}
