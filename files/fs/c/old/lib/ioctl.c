/*
 * Ioctl support for TOPS-20
 *
 * Greg Satz / Stanford University / 3-Jan-85
 */

entry gtty, stty, ioctl;

#include <sys/ioctl.h>

#define	DVCHR	0117
#define	DVDSK	0
#define	DVMTA	02
#define	DVLPT	07
#define	DVTTY	012
#define	DVNUL	015

#define	MTOPR	077
#define	MOSPD	026
#define	MORSP	027

#define	RFMOD	0107
#define	TT_ECO	04000
#define	TT_DAM	0300
#define	TTBIN	0000
#define	TTASC	0100
#define	TTATO	0200
#define	TTATE	0300
#define	TT_PGM	01

#define	RUBOUT	'\177'

static int speedtab[] = {0, 50, 75, 110, 134, 150, 200, 300,
			 600, 1200, 1800, 2400, 4800, 9600};
/*
 * Get tty characteristics
 * Compatability routine
 */
gtty(fd, argp)
int fd;
struct sgttyb *argp;
{
	return ioctl(fd, TIOCGETP, argp);
}

/*
 * Set tty characteristics
 * Compatability routine
 */
stty(fd, argp)
int fd;
struct sgttyb *argp;
{
	return ioctl(fd, TIOCSETP, argp);
}

extern _ch[];

/*
 * Ioctl(2)
 */
ioctl(fd, func, arg)
int fd, func, *arg;
{
	int i, spd, jfn, ablock[5];
	struct sgttyb *argb;

	jfn = _ch[fd];		/* fetch jfn */
	switch (func) {		/* see what we need to do */

	case TIOCGETP:
	    if (!_device(jfn, DVTTY))	/* is it a tty device? */
		return -1;
	    ablock[1] = jfn;
	    ablock[2] = MORSP;
	    if (jsys(MTOPR, ablock) == 0)
		return -1;
	    argb = (struct sgttyb *) arg;
	    spd = (ablock[3] >> 18) & 0777777;
	    for (i = 0; i < (sizeof speedtab)/(sizeof speedtab[0]); i++)
		if (speedtab[i] == spd)
		    break;
	    argb->sg_ispeed = i;
	    spd = ablock[3] & 0777777;
	    for (i = 0; i < (sizeof speedtab)/(sizeof speedtab[0]); i++)
		if (speedtab[i] == spd)
		    break;
	    argb->sg_ospeed = i;
	    argb->sg_erase = RUBOUT;
	    argb->sg_kill = CTRL(u);	/* fixed for now */
	    argb->sg_flags = 0;
	/* TOPS-20 doesn't have any delays */
	/* TOPS-20 doesn't have parity */
	    ablock[1] = jfn;
	    if (jsys(RFMOD, ablock) == 0)
		return -1;
	    switch (ablock[2] & TT_DAM) {
	    case TTBIN:
		argb->sg_flags |= RAW;
	    case TTATO:
	    case TTASC:
	    case TTATE:
		break;
	    }
	    if (ablock[2] & TT_ECO)	/* is the echo bit on */
		argb->sg_flags |= ECHO;
	    if (ablock[2] & TT_PGM)	/* respond to ^S/^Q */
		argb->sg_flags |= TANDEM;
	    return 0;

	case TIOCSETP:

static _device(jfn, what)
int jfn, what;
{
	int type, ablock[5];

	ablock[1] = jfn;
	if (jsys(DVCHR, ablock) == 0)
	    return 0;
	type = (ablock[2] >> 18) & 0777;
	return (type == what);
}
