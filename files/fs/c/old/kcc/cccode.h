/*
** cccode.h - Aliases of pseudo codes for KCC
** David Eppstein / Stanford University / 24 May 85
**
** These values go in the p->pop field of a pseudo code instruction.
** They correspond to the names of PDP-10 instructions without any
** "I" or "B" for immediate or both modifiers on them.
** The names of these ops can be found in dec20op[].
**
** Many of the numbers (especially arithmetic ops) are those of the
** corresponding token.  Be careful when rearranging them.
*/

#ifndef CODESTRINGS

/*
** Here when we are just interested in the definitions here.
** First define the components of an opcode, and a few macros.
*/

/* fields of opcode number */
#define OPCODE  000377			/* op without skips and such */
#define ISSKIP  000400			/* op is a skip (to tell from JUMP) */
#define OPSKIP  007000			/* bits saying which skip */
#define BOTH	010000			/* send result to mem as well as reg */

/* meanings of values of OPSKIP field */
#define SKPA    01000			/* always skip (0 is never skip) */
#define SKPE    02000			/* skip if equal (to zero) */
#define SKPN	03000			/* skip if not equal */
#define SKPL	04000			/* skip if less than */
#define SKPGE	05000			/* skip if greater than or equal */
#define SKPG	06000			/* skip if greater than */
#define SKPLE	07000			/* skip if less than or equal */

/* meanings of OPSKIP bits (derived from value meanings or vice versa) */
#define INVSKIP 01000			/* bit to flip to invert skip parity */
#define SWPSKIP 02000			/* flip to swap comparison arguments */
#define EQSKIP  03000			/* add E to arithmetic skips */
#define CMPSKIP 04000			/* on if skip is a comparison */

#define revop(op) ((op) ^ INVSKIP)
#define isskip(op) ((op) & ISSKIP)
#define swapop(op) ((op) & CMPSKIP? ((op) ^ SWPSKIP) : (op))


/*
** Now we set things up so that the opcode listing below
** defines constants for each opcode defined.
**
** This is done by defining code() to expand to its argument,
** and by enclosing the list of codes in an enum { }.
** We also need to declare dec20op[] since we don't define it.
*/

#define code(name) ,name		/* define name as an enum */
extern char *dec20op[];			/* defined elsewhere using this file */
enum { NOP, CVALUE			/* again, start with nop and cvalue */


#else

/*
** Here when CODESTRINGS is defined, meaning ccgdat.c is including
** this file to get the definition for dec20op[].
**
** This time code() expands to the string containing its argument.
** The list is enclosed as part of the initialization for dec20op[].
*/

#define code(name) ,"name"		/* use name to make opcode string */
char *dec20op[] = { "--", ""		/* start with nop and cvalue */

#endif

/*
** We use the same code to define the opcode constants and to initialize
** the strings in dec20op[].
**
** The opcodes are in approximately alphabetical order.
** But note that any newly added ones should go at the end
** of the list unless everything using these definitions
** is to be recompiled.
*/

code(ADD)
code(ADJBP)
code(ADJSP)
code(AND)
code(AOJ)
code(AOS)
code(ASH)
code(ASHC)
code(BPCNT)
code(CAI)
code(CAM)
code(DFAD)
code(DFDV)
code(DFIX)
code(DFLOT)
code(DFMP)
code(DFSB)
code(DMOVE)
code(DMOVEM)
code(DMOVN)
code(DPB)
code(EQV)
code(FADR)
code(FDVR)
code(FIXR)
code(FLTR)
code(FMPR)
code(FSBR)
code(FSC)
code(HLRZ)
code(HRLM)
code(HRRM)
code(HRRZ)
code(IBP)
code(IDIV)
code(IDPB)
code(IFIW)
code(ILDB)
code(IMUL)
code(IOR)
code(JRST)
code(JUMP)
code(LDB)
code(LSH)
code(MOVE)
code(MOVEM)
code(MOVM)
code(MOVN)
code(MOVS)
code(ORCM)
code(POP)
code(POPJ)
code(PUSH)
code(PUSHJ)
code(ROT)
code(SETCM)
code(SETO)
code(SETZ)
code(SKIP)
code(SOJ)
code(SOS)
code(SPOP)
code(SPUSH)
code(SUB)
code(SUBBP)
code(TDC)
code(TDN)
code(TDO)
code(TDZ)
code(TLC)
code(TLN)
code(TLO)
code(TLZ)
code(TRC)
code(TRN)
code(TRO)
code(TRZ)
code(XMOVEI)
code(XOR)

};					/* end enum or dec20op[] */
#undef code
