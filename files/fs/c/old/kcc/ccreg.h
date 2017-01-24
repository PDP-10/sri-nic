/*
** ccreg.h - declarations for KCC register management
** David Eppstein / Stanford University / 8 Mar 1985
*/


#define REGZERO	000			/* AC0, never used for many reasons */
#define RETVAL	001			/* register for subroutine return */
#define RETDBL	002			/* second return for doublewords */
#define MINREG	003			/* lowest scratch register */
#define	MAXREG	015			/* highest scratch register */
#define SCRREG	016			/* scratch for realcode() */
#define	SP	017			/* push down stack pointer */

#define REGMASK 017			/* mask to get reg only from bp idx */
#define NREGS   020			/* number of physical registers */

struct vreg {				/* virtual register */
    int flags;				/* spilled, double, etc */
    int location;			/* reg number or stack offset */
    int prevloc;			/* where to back stack to */
    struct vreg *next, *prev;		/* double linked list pointers */
};

#define SPILLED 01			/* register is on stack */
#define REGPAIR 02			/* register is a doubleword */

struct vreg *getreg(), *getpair();	/* allocate register or pair */
struct vreg *getret(), *getrpair();	/* same for return value */
int rfree();				/* whther register is assigned */
void release(), spill();		/* deallocate reg; spill all regs */
void widen(), narrow();			/* change between reg and pair */
void iniregs();				/* set up regs for new routine */
int realreg();				/* despill and give physical reg */
void backreg();				/* skip over MOVE w/o releasing */
