/* <KCC.CC>CCGEN.H.31,  6-Jun-85 10:30:42, Edit by KRONJ */
/*  BOTH moves to opcode and cccode.h */
/* <KCC.CC>CCGEN.H.29,  3-Jun-85 14:45:52, Edit by KRONJ */
/*  Define SKIPPED flag */
/* <KCC.CC>CCGEN.H.12, 21-May-85 11:48:58, Edit by KRONJ */
/*  TLC and ASHC for inline int => double coercion */

/*
** ccgen.h - header file for code generation modules of KCC
** Renamed and cleaned up from CC.G / 10 Mar 85 / David Eppstein
*/

#include "ccreg.h"
#include "cccode.h"

#ifndef GEXT
#define GEXT extern
#endif

#ifndef NULL
#define NULL 0
#endif

#define	MAXCASE 512			/* max number of cases per switch */

struct CASES {				/* Structure to remember cases: */
    int csval;				/* case value */
    int cslab;				/* case label */
};

extern int optimize;			/* Do peepholing; defined in cc.h */
extern int stackrefs;			/* Safe to mung stack, ditto */

GEXT int
    stackoffset;			/* adjustments made to stack */

GEXT struct SYMBOL
    *brklabel,				/* the break label */
    *looplabel;				/* label to go to on continue */

extern struct SYMBOL *newlabel();	/* get new internal label */

/*
** Structures for the peephole buffer
**
** Emitted code is buffered for a while so that when further code is
** emitted a peephole optimizer can be run on the mess.  This
** optimizer is responsible for many improvements normally done at a
** higher level, such as common subexpression elimination and constant
** reassociation.
**
** The code is kept in a circular buffer of instructions.
** When the buffer fills, the bottom edge is moved along and the code
** it passes emitted.
**
** Note that all registers in the buffer are actual numbers of machine
** registers, rather than the virtual registers defined in ccreg.
*/

#define	MAXCODE	256			/* peephole size 2^n */

typedef struct PSEUDO {			/* pseudo op in peephole buffer */
    int ptype;				/* OPBASE and flags */
    int pop;				/* opcode */
    int preg;				/* register */
    struct SYMBOL *pptr;		/* ident for memory address */
    int poffset;			/* offset to add to ident */
    int pindex;				/* index register */
} *pcode;

#define pr1 preg			/* index reg for reg-reg codes */
#define pr2 pindex			/* and other reg for those codes */

#define pvalue poffset			/* immediate value */

#define pmantissa poffset		/* mantissa of immediate float */
#define pexponent pindex		/* exponent of same */

GEXT struct PSEUDO codes[MAXCODE];	/* the circular buffer of pseudo-ops */
GEXT pcode previous;			/* pointer to last non-null op */

GEXT int maxcode, mincode;		/* top and bottom indices into codes */

/*
** Structure of ptype field in pseudo code.
**
** This field (as with the opcodes defined in cccode.h) is divided
** into several components.  The main one is ADRMODE, which determines
** the meanings of most of the other fields in the instruction.
** Others modify the instruction in certain ways, or are used to
** pass information to the peephole optimizer about what is safe to do.
*/

#define	ADRMODE	 007			/* Mask for p->pop addressing mode */
#define	IMM 	 010			/* Flag to append "I" to op name */
#define SKIPPED	 020			/* Set if op before this can skip */
#define IND 	 040			/* Flag to use indirect addressing */

#define RCONST	    1			/* simple integer in pvalue */
#define MINDEXED    2			/* addr+offset(index) */
#define BYTEPOINT   3			/* [index,,addr+offset] */
#define	ONEREG	    4			/* no address, just register */
#define	PFLOAT	    5			/* [mantissa E exponent] */
#define	REGIS	    6			/* register to register */

#define	IMMED	    (RCONST+IMM)	/* Immediate simple integer type */
#define IINDEXED    (MINDEXED+IMM)	/* Immediate addressing type */

#define prevskips(p) ((p)->ptype & SKIPPED)	    /* if preceding op skips */
#define setskip(p)   (void) ((p)->ptype |= SKIPPED) /* say it does */
#define clrskip(p)   (void) ((p)->ptype &=~ SKIPPED) /* say it doesn't */
