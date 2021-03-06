/* <SIGNAL.H> - Signal value definitions.
**
**	(c) Copyright Ken Harrenstien 1989
**
**	These definitions are extremely unportable; however,
**	all of the 4.3BSD signal structures are defined and implemented
**	(at least on TOPS-20).
*/

#ifndef _SIGNAL_INCLUDED	/* Include only once */
#define _SIGNAL_INCLUDED
#ifndef __STDC__	/* Canonicalize this indicator to avoid err msgs */
#define __STDC__ 0
#endif

/* First take care of ANSI definitions */
typedef int sig_atomic_t;	/* Atomic integral object */

#if __STDC__			/* ANSI prototype decls */
#define	SIG_DFL	(void (*)(int))0 /* Arg to "signal" to resume default action */
#define	SIG_IGN	(void (*)(int))1 /* Arg to "signal" to ignore this sig */
#define	SIG_ERR	(void (*)(int))(-1) /* Return val from "signal" if error */
extern void (*signal(int, void(*)(int)))(int);
extern int raise(int);
#else
#define	SIG_DFL	(void (*)())0	/* Arg to "signal" to resume default action */
#define	SIG_IGN	(void (*)())1	/* Arg to "signal" to ignore this sig */
#define	SIG_ERR	(void (*)())(-1) /* Return val from "signal" if error */
extern void (*signal())();
extern int raise();
#endif

#define SIGABRT SIGIOT	/* New signal for ANSI; re-use SIGIOT as per H&S */

/* Signals here are marked with a 3-character code indicating the actions
** taken when the signal happens, for each of 3 possible settings for
** the signal handling routine:
**	    
**	  /----- D: SIG_DFL (default action)
**	 | /---- I: SIG_IGN (ignore signal)
**	 || /--- H: User-defined handler routine
**	<DIH>
**	 |||
**	 \\\--> Each letter is one of:
**			-: Ignore signal.
**			T: Terminate process.
**			D: Terminate process, dump core image.
**			S: Stop process (suspend - can continue later).
**			H: Call user handler.
*/

/*	Signal	#	  <DIH>  Description */
#define	SIGHUP	1	/* T-H Hangup (eg dialup carrier lost) */
#define	SIGINT	2	/* T-H Interrupt (user TTY interrupt, default DEL) */
#define	SIGQUIT	3	/* D-H Quit      (user TTY interrupt, default ^\) */
#define	SIGILL	4	/* D-H Illegal Instruction (not reset when caught) */
#define	SIGTRAP	5	/* D-H Trace Trap (not reset when caught) */
#define	SIGIOT	6	/* D-H IOT instruction */
#define	SIGEMT	7	/* D-H EMT instruction */
#define	SIGFPE	8	/* D-H Floating Point Exception */
#define	SIGKILL	9	/* TTT Kill (cannot be caught, blocked, or ignored) */
#define	SIGBUS	10	/* D-H Bus Error */
#define	SIGSEGV	11	/* D-H Segmentation Violation */
#define	SIGSYS	12	/* D-H Bad argument to system call */
#define	SIGPIPE	13	/* T-H Write on a pipe with no one to read it */
#define	SIGALRM	14	/* T-H Alarm Clock */
#define	SIGTERM	15	/* T-H Software termination signal (from "kill" pgm) */

/* BSD additions */
#define SIGURG 16	/* --H Urgent condition present on socket */
#define SIGSTOP 17	/* SSS Stop process(cannot be caught/blocked/ignored)*/
#define SIGTSTP 18	/* S-H Stop      (user TTY interrupt, default ^Z) */
#define SIGCONT 19	/* --H Continue after stop (cannot be blocked) */
#define SIGCHLD 20	/* --H Child status changed */
#define SIGTTIN 21	/* S-H Background read attempted from control TTY */
#define SIGTTOU 22	/* S-H Background write attempted from control TTY */
#define SIGIO 23	/* --H I/O is possible on a FD (see fcntl) */
#define SIGXCPU 24	/* T-H CPU time limit exceeded (see setrlimit) */
#define SIGXFSZ 25	/* T-H File size limit exceeded (see setrlimit) */
#define SIGVTALRM 26	/* T-H Virtual time alarm (see setitimer)*/
#define SIGPROF 27	/* T-H Profiling time alarm (see setitimer) */
#define SIGWINCH 28	/* --H Window size change */
#define SIG29	29	/*     (not used -- placeholder definition) */
#define SIGUSR1	30	/* T-H User-defined signal 1 */
#define SIGUSR2	31	/* T-H User-defined signal 2 */

/* TOPS-20/TENEX additions */
#define SIGT20EOF 32	/* --H .ICEOF PSI interrupt */
#define SIGT20NXP 33	/* --H .ICNXP PSI interrupt */

#define _NSIGS 36	/* KCC implementation allows up to 36 signals! */

/* Definitions for 4.3BSD signal mechanism */

/* SIGMASK(sig) - Macro to furnish mask bit for specified signal */
#define sigmask(sig) (1<<((sig)-1))	/* BSD signal bit from sig # */

#include <sys/types.h>		/* For caddr_t */
struct sigvec {
	void (*sv_handler)();
	int sv_mask;
	int sv_flags;
	union {			/* Extra word for OS-dependent stuff */
	    struct {			/* T20/10X stuff:	*/
		unsigned psichn : 6;	/* PSI channel # (1-35) */
		unsigned psilev : 2;	/* PSI level # (1-3)	*/
		signed   tic : 7;	/* .TICxx code plus 1 (1-36) */
	    } t20;
	} sv_os;		/* OS-dependent stuff */
#define sv_onstack sv_flags	/* For 4.2BSD compatibility */
};

/* Flags for sv_flags.  The first must be 1 for compatibility with 4.2BSD. */
#define SV_ONSTACK 01		/* Use signal stack for handling this signal */
#define SV_INTERRUPT 02		/* (New in 4.3) OK to interrupt syscall */

#define SV_HNDLR_RESET 04000	/* KCC: Reset signal to SIG_DFL when caught */
#define SV_XOS	   010000	/* KCC: "os" extension valid */
#define SV_XINTLEV 020000	/* KCC: handler should run at OS int level */
#define SV_XASMHAN 040000	/* KCC: handler is assumed to be a special
				**   assembler routine and is invoked directly
				**   in an OS-dependent way. */

struct sigstack {
	int *ss_sp;		/* Stack pointer (note not caddr_t!) */
	int ss_onstack;		/* Whether on this stack now */
};

/* System-dependent saved context for restoring state prior to signal.
** If this is changed, the asm code in SIGVEC needs to be changed too!
*/
struct sigcontext {
	int sc_pc;		/* Interrupt PC (must be 1st thing!) */
	int sc_pcflgs;		/* Interrupt PC flags */
	int sc_osinf;		/* OS signal info */
				/* T20/10X: <previous PSI level>,,<chan #> */
	int sc_sig;		/* Signal # that caused interrupt */
	struct sigcontext *sc_prev;	/* Previous sig frame */
	int sc_stkflg;		/* Previous signal stack state */
	int sc_mask;		/* Previous signal block mask */
	int sc_acs[16];		/* Saved PDP-10 registers */
};

/*
 *	Stuff for SYS V softsigs as per H&S V2, sec 21.6
 */

extern int (*ssignal())();
extern int gsignal();

#define _SS_FIRST	1	/* softsigs in the range 1 - */
#define _SS_LAST	15	/* through 15 */
#define _SS_NSIGS	(_SS_LAST - _SS_FIRST + 1)

#endif /* ifndef _SIGNAL_INCLUDED */
