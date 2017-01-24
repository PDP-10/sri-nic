/*
 *	IOCTL definitions
 */

#ifndef	_IOCTL_INCLUDED
#define	_IOCTL_INCLUDED
#ifndef _SGTTY_INCLUDED
#include <sgtty.h>	/* sgtty.h and sys/ioctl.h include each other */
#endif /* ifndef _SGTTY_INCLUDED */

struct tchars {
    char t_intrc;		/* interrupt */
    char t_quitc;		/* quit */
    char t_startc;		/* start output */
    char t_stopc;		/* stop output */
    char t_eofc;		/* end-of-file */
    char t_brkc;		/* input delimiter (like nl) */
};

struct ltchars {
    char t_suspc;		/* stop process signal */
    char t_dsuspc;		/* delayed stop process signal */
    char t_rprntc;		/* reprint line */
    char t_flushc;		/* flush output (toggles) */
    char t_werasc;		/* word erase */
    char t_lnextc;		/* literal next character */
};

struct winsize {		/* Window size - BSD4.3 addition */
    unsigned ws_row;		/* Height in chars (# lines) */
    unsigned ws_col;		/* Width  in chars (# columns) */
    unsigned ws_xpixel;		/* Width  in pixels */
    unsigned ws_ypixel;		/* Height in pixels */
};

/*
 *	ioctl() function definitions
 */
/* Functions to support V6 gtty/stty calls */
#define	TIOCGETP	0	/* Get parameters -- V6/V7 gtty() */
#define	TIOCSETP	1	/* Set parameters -- V6/V7 stty() */

/* Functions to support V7 ioctl() */
#define	TIOCSETN	2	/* V7:   as above, but no flushtty */
#define	TIOCEXCL	3	/* V7: set exclusive use of tty */
#define	TIOCNXCL	4	/* V7: reset excl. use of tty */
#define	TIOCHPCL	5	/* V7: hang up on last close */
#define	TIOCFLUSH	6	/* V7: flush buffers */

/* All other functions are for BSD (4.3) */
#define	TIOCSTI		7	/* simulate terminal input */
#define	TIOCSBRK	8	/* set   break bit */
#define	TIOCCBRK	9	/* clear break bit */
#define	TIOCSDTR	10	/* set   data terminal ready */
#define	TIOCCDTR	11	/* clear data terminal ready */
#define	TIOCGPGRP	12	/* get pgrp of tty */
#define	TIOCSPGRP	13	/* set pgrp of tty */
#define	TIOCGETC	14	/* get special characters */
#define	TIOCSETC	15	/* set special characters */
#define TIOCLBIS	16	/* set   bits in local mode word */
#define TIOCLBIC	17	/* clear bits in local mode word */
#define TIOCLGET	18	/* get local mode mask */
#define TIOCLSET	19	/* set local mode mask */
#define	TIOCSLTC	20	/* set local special chars */
#define	TIOCGLTC	21	/* get local special chars */

#define	FIONREAD	22	/* get # bytes to read */

#define TIOCGETD	23	/* Get line discipline */
#define TIOCSETD	24	/* Set line discipline */

#define TIOCGWINSZ	25	/* Get window size info */
#define TIOCSWINSZ	26	/* Set window size info (maybe gen SIGWINCH) */

/*
 *	sg_flags word flags
 */

#define ALLDELAY	0177400		/* Delay algorithm selection */
#define BSDELAY		0100000		/* Select backspace delays */
#define BS0		0
#define BS1		0100000
#define VTDELAY		0040000		/* for,-feed/v-tab delay */
#define FF0		0
#define FF1		0040000
#define CRDELAY		0030000		/* carriage-return delay */
#define CR0		0
#define CR1		0010000
#define CR2		0020000
#define CR3		0030000
#define TBDELAY		0006000		/* tab delays */
#define TAB0		0
#define TAB1		0001000
#define TAB2		0004000
#define XTABS		0006000
#define NLDELAY		0001400		/* new-line delays */
#define NL0		0
#define NL1		0000400
#define NL2		0001000
#define NL3		0001400
#define EVENP		0000200		/* even parity allowed on input */
#define ODDP		0000100		/* odd parity allowed on input */
#define RAW		0000040		/* wake on all chars, 8-bit input */
#define CRMOD		0000020		/* map CR->LF; echo LF or CR as CRLF */
#define ECHO		0000010		/* echo (full duplex) */
#define LCASE		0000004		/* map upper case to lower case */
#define CBREAK		0000002		/* return each char as soon as typed */
#define TANDEM		0000001		/* automatic flow control */

/* Bits for BSD local mode word */
#define LCRTBS	01	/* Backspace on erase rather than echoing erase */
#define LPRTERA	02	/* Printing terminal erase mode */
#define LCRTERA 04	/* Erase char echoes as BS-SP-BS */
#define LTILDE	010	/* Convert ~ to ` on output (for Hazeltines) */
#define LMDMBUF	020	/* Stop/start output when carrier drops */
#define LLITOUT	040	/* Suppress output translations */
#define LTOSTOP	0100	/* Send SIGTTOU for background output */
#define LFLUSHO	0200	/* Output is being flushed */
#define LNOHANG	0400	/* Don't send hangup when carrier drops */
#define LETXACK	01000	/* Diablo style buffer hacking (??) */
#define LCRTKIL	02000	/* Use BS-SP-BS to erase entire line on line kill */
#define LINTRUP	04000	/* Generate SIGTINT when input ready to read */
#define LCTLECH	010000	/* Echo input control chars as ^X (DEL as ^?) */
#define LPENDIN	020000	/* Retype pending input at next read or input char */
#define LDECCTQ	040000	/* Only ^Q restarts after ^S, like DEC systems */

/* Line discipline values */
#define OTTYDISC 0	/* Old V7-style discipline (must be zero) */
#define NTTYDISC 1	/* New BSD-style discipline */
#define NETLDISC 2	/* high-speed "net" discipline (not supported) */

#endif /* ifndef _IOCTL_INCLUDED */
