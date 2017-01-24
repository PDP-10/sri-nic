/*
** setbuf - change buffering for standard I/O
** David Eppstein / Stanford University / 16-Aug-84
*/

#include <stdio.h>

char _sobuf[BUFSIZ];			/* some people want this buffer */

setbuf(f, buf)
FILE *f;
char *buf;
{
    fflush(f);				/* clear out old buffering */
    if (f->siopbuf)			/* release old buffer */
	free(f->siopbuf);
    f->siopbuf = buf;			/* point to buffer */

    if (buf == NULL) {
	f->sioflgs &= ~_SIOF_BUF;	/* no buffering */
	return;				/* that's all folks */
    }

    f->sioflgs |= _SIOF_BUF;		/* remember we are buffered */
    switch (f->sioflgs & (_SIOF_READ | _SIOF_WRITE)) {
    case _SIOF_READ:			/* if reading from file: */
	f->siocnt = 0;			/* no chars to read in buffer */
	return;
    case _SIOF_WRITE:			/* if writing to file: */
	f->siocp = buf - 1;		/* buffer points to start */
	f->siocnt = BUFSIZ;		/* full of empty space */
	return;
    }

    fputs("setbuf: file not open for reading or writing\n", stderr);
    exit(1);
}
