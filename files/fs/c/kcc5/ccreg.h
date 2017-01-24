/*	CCREG.H - Declarations for KCC register management
**
**	All changes after version 8 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/*
** David Eppstein / Stanford University / 8 Mar 1985
*/

#define R_REGZERO	000	/* AC0, never used for many reasons */
#define R_RETVAL	001	/* register for subroutine return */
#define R_RETDBL	002	/* second return for doublewords */
#define R_MINREG	003	/* lowest scratch register */
#define	R_MAXREG	015	/* highest scratch register */
#define R_SCRREG	016	/* scratch for realcode() */
#define	R_SP		017	/* push down stack pointer */

#define REGMASK 017		/* mask to get reg only from bp idx */
#define NREGS   020		/* number of physical registers */

#define VREG struct _vreg		/* Virtual Register definition */
struct _vreg {
    int flags;			/* Flags for spilled, double, etc */
    int location;		/* reg number or stack offset */
    TYPE *Vrtype;		/* C type of object in register */
    int prevloc;		/* where to back stack to */
    VREG *next, *prev;		/* double linked list pointers */
};

#define VRF_SPILLED 01		/* register is on stack */
#define VRF_REGPAIR 02		/* register is a doubleword */
#define VRF_HASVAL 04		/* Reg has a needed val, don't clobber it
				** (OK to change reg # if change instr that
				** set it)
				*/

VREG *getreg(), *getpair();	/* allocate register or pair */
VREG *getret(), *getrpair();	/* same for return value */
int rfree();			/* whether real register is assigned */
int rhasval();			/* whether real register has value */
void release(), spill();	/* deallocate reg; spill all regs */
void widen(), narrow();		/* change between reg and pair */
void iniregs();			/* set up regs for new routine */
int realreg();			/* despill and give physical reg */
void backreg();			/* skip over MOVE w/o releasing */
void setreg();			/* set a virtual regs real loc */

/* Register bit macros.  Turns register # into a bit mask. */

#define regbit(r) (1<<(r))	/* Single-register bit */
#define dregbit(r) (3<<(r))	/* Double-register bits */

/* Register bit routines */

extern int rbuse(), rbinreg(), rbinaddr(), rbmod();

/* Array of register bits indexed by register #, for faster use. */

extern int rbits[NREGS];	/* Array of regbit(n) */
extern int drbits[NREGS];	/* Array of dregbit(n) */
