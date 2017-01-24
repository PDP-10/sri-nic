/* UIODAT - USYS I/O data definitions
**
**	Copyright (c) 1987 by Ken Harrenstien & Ian Macky, SRI International
**
** This module contains declarations for various data structures needed by
** the USYS routines, particularly the I/O functions, which must be global.
** They are still "internal" from the viewpoint of the user and must not
** be referenced by anything but USYS calls.
*/

#include "c-env.h"
#include "sys/usysio.h"
#include "sys/usytty.h"		/* Includes <sgtty.h> and <sys/ioctl.h> */

/*
 * UFX (Unix File indeX) storage.
 *		We maintain a single active UFX for each operating-system
 *	channel/JFN.  A UFX is separate from a FD so that we can have
 *	several FDs referencing the same UFX, as happens inside the UNIX
 *	kernel.
 *
 *	FDs can be numbered from 0 to OPEN_MAX-1 inclusive, as on UNIX.
 *	UFXs are from 1 to OPEN_MAX inclusive, as it is easier to maintain
 *	the FD-to-UFX mapping table this way (a UFX value of 0 means the
 *	FD is unassigned).
 *
 *	Ordinarily these arrays would be combined into
 *	a nice structure, but for better code generation (directly indexed
 *	arrays instead of computation+index) we are keeping them apart.
 *
 *	The _uioufx table determines whether a FD is open or not.
 *	The _uionopen table determines whether a UFX is open or not.
 *		If not open ( >0), all other tables for that UFX are invalid.
 */

	/* Per-FD storage.  Only one such table, for mapping FD into UFX. */
int _uioufx[OPEN_MAX];		/* Indexed by FD, gets UFX! */

	/* Per-UFX storage. */
int _uionopen [OPEN_MAX+1];	/* # of FDs which have this UFX open */
int _uioch    [OPEN_MAX+1];	/* JFN for channel */
int _uiotype  [OPEN_MAX+1];	/* OS device type     (== major device #) */
int _uiodnum  [OPEN_MAX+1];	/* USYS device number (== minor device #) */
int _uiobsize [OPEN_MAX+1];	/* byte-size file was opened in */
int _uioflgs  [OPEN_MAX+1];	/* flag bits; can read with fcntl's F_GETFL */
int _uiocnt   [OPEN_MAX+1];	/* count of chars left in buffer */
char *_uiocp  [OPEN_MAX+1];	/* current pointer into buffer */
char *_uiopbuf[OPEN_MAX+1];	/* pointer to start of buffer */
long _uiopos  [OPEN_MAX+1];	/* current position within file */
int _uioeof   [OPEN_MAX+1];	/* EOF on channel */
	/* See also the buffer code on following pages */

#if SYS_WAITS
struct bufhead _buffers[UIO_MAXFILE]; /* file I/O buffers */
int _uioeof;			/* end of file reached on terminal */
#endif


/* Controlling TTY variables */

/* On T20/10X there can be only one controlling terminal
** (i.e. source of terminal interrupts).  This is always _ttys[0].
** We only attempt full emulation for this terminal rather than for all
** TTY: devices, but up to _NTTYS-1 other random TTY devices are possible.
*/
#define ctl(a) (a&037)
struct _tty _ttys[_NTTYS];	/* Controlling TTY plus any others */

/* Default internal terminal structure, used to initialize _tty structs. */
struct _tty _deftty = {
	0, 0,			/* Zero UFX ensures more initialization */
#if SYS_T20+SYS_10X
	0, 0, 0, 0,				/* Various T20 words */
	{-1,-1,'\177',ctl('U'),EVENP|ODDP|CRMOD|ECHO},	/* sgtty */
	{-1, -1, ctl('Q'), ctl('S'), ctl('Z'), -1},	/* tchars */
	NTTYDISC, 0,					/* line disc, mode */
	{ctl('C'),ctl('C'),ctl('R'),ctl('O'),ctl('W'),ctl('V')}	/* ltchars */

#else	/* Defaults as per BSD UNIX */
	0, 0, 0, 0,				/* Various words? */
	{B9600, B9600, '#', '@', CRMOD|ECHO},
	{'\177', ctl('\\'), ctl('Q'), ctl('S'), ctl('D'), -1},
	OTTYDISC, 0,
	{ctl('Z'),ctl('Y'),ctl('R'),ctl('O'),ctl('W'),ctl('V')}
#endif
};

/*	I/O buffer data and routines.
**		The I/O buffers are done as static data areas for
**	now because the syscalls cannot invoke malloc() as needed.
*/
#ifndef NIOBUFS
#define NIOBUFS (OPEN_MAX/2)
#endif
struct iob {
	int b_inuse;
	char b_buf[UIO_BUFSIZ];
};
static struct iob _iob[NIOBUFS];	/* Gross! */
static struct iob *_getiob();
static void _freiob();

char *
_getbuf()
{
    register struct iob *b;
    if (b = _getiob())
	return &b->b_buf[0];
    return NULL;
}

static struct iob *
_getiob()
{
    register int i;
    register struct iob *b;

    for (i = NIOBUFS, b = _iob; --i >= 0; ++b)
	if (b->b_inuse == 0) {
	    b->b_inuse++;
	    return b;
	}
    return NULL;		/* No more buffers */
}

void
_frebuf(bfp)
char *bfp;
{
    _freiob((struct iob *)(bfp - sizeof(int)));
}

static void
_freiob(bp)
struct iob *bp;
{
    bp->b_inuse = 0;
}
