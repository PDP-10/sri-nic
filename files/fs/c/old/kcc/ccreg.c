/*
** ccreg - register management for KCC
** David Eppstein / Stanford University / 8 Mar 1985
*/

#include "ccgen.h"
#include "ccerr.h"

#define REGLIST(x) static struct vreg x = {0, 0, 0, &x, &x};

REGLIST(freelist);			/* virtual regs not now in use */
REGLIST(reglist);			/* regs associated with real regs */
REGLIST(spillist);			/* regs spilled onto the stack */

static struct vreg *regis[NREGS];	/* who is using which registers */
static int regfree[NREGS];		/* what regs are used in code */

extern struct PSEUDO *before();		/* routine to poke through pbuf */

/*
** Initialize for start of new code
** Called at the start of each function or code initializer by inicode()
*/

void
iniregs()
{
    int i;

    if (!empty(&reglist) || !empty(&spillist))
	warn (ERELREG, "unreleased registers left over from previous code");
    for (i = 0; i < NREGS; i++) regis[i] = NULL;
}

/*
** Determine whether a list is empty
** Call with &freelist, &reglist, or &spillist.
*/

static int
empty(list)
struct vreg *list;
{
    return (list->next == list);
}


/*
** Remove a register from whatever list it's on
** This is the first half of changing from one list to another
*/

static void
unlink(reg)
struct vreg *reg;
{
    if (empty(reg)) {
	warn(ERELREG, "attempt to unlink list head");
	return;
    }

    reg->next->prev = reg->prev;
    reg->prev->next = reg->next;
}


/*
** Add a register to a list
** Used when first create a new reg and when moving from one list to another
*/

static void
linkreg(reg, list)
struct vreg *reg, *list;
{
    reg->next = list->next;
    list->next->prev = reg;
    reg->prev = list;
    list->next = reg;
}

/*
** Get an unused pseudo register
*/

static struct vreg *
newreg()
{
    char *cp, *malloc();
    struct vreg *rp;

    if (empty (&freelist)) {
	cp = malloc (sizeof (struct vreg));
	if (cp == NULL) fatal (EOUTMEM);
	rp = (struct vreg *) cp;
    } else {
	rp = freelist.next;
	unlink (rp);
    }
    rp->flags = 0;
    return rp;
}

/*
** Spill one real register
** This takes care of stacking it and deassigning it.
*/

static void
spillreg (reg)
{
    code0 (PUSH, SP, reg);		/* push onto stack */
    regis[reg] = NULL;			/* no longer here */
    ++stackoffset;			/* stack is bigger now */
}

/*
** Spill a register.
** Either we needed to reallocate it or we are calling a function.
** In either case the register moves onto the stack.
*/

static void
spillone(reg)
struct vreg *reg;
{
    if (reg->flags & SPILLED)
	warn (ERELREG, "attempt to spill already spilled reg");
    unlink (reg);			/* remove from assigned list */
    spillist.next->prevloc = stackoffset; /* remember where we are */
    spillreg (reg->location);
    if (reg->flags & REGPAIR) spillreg (reg->location + 1);
    reg->location = stackoffset;
    reg->flags |= SPILLED;
    linkreg (reg, &spillist);		/* it's now spilled */
}

/*
** Spill all registers
** This is needed to save values over subr calls
*/

void
spill()
{
    while (!empty (&reglist)) spillone (reglist.next);
}

/*
** See which registers are in use in the peephole buffer.
** We try to avoid assigning these so that common subexpression
** elimination will have the greatest opportunity to work.
**
** Since this is merely a heuristic and since it is called very often
** per compilation, we care more about speed than accuracy.
** In particular, we don't even bother looking at the opcode or
** addressing mode of each instruction.
*/

static void
updrfree()
{
    int r;
    struct PSEUDO *p;

    for (r = 0; r < NREGS; r++) regfree[r] = (regis[r] == NULL);
    for (p = previous; p != NULL && p->pop != PUSHJ; p = before(p))
	regfree[p->preg] = 0;
}

/*
** Find an unused scratch register.
** If none exist, we spill what is likely to be the
** earliest allocated register (since our register allocation
** will tend to act like a stack this is a win).
*/

static int
findreg(reg)
struct vreg *reg;
{
    int rnum;

    updrfree();				/* update regfree[] to pbuf contents */
    for (rnum = MINREG; rnum <= MAXREG; rnum++)	/* try for a register not */
	if (regfree[rnum]) return rnum;	/* ..used at all in this local block */
    for (rnum = MINREG; rnum <= MAXREG; rnum++)	/* no, try for a free one */
	if (regis[rnum] == NULL) return rnum;
    spillone (regis[MINREG]);		/* spill something. better random */
    return MINREG;			/* ..but this never happens anyway */
}

/*
** Same, but this time we return the first of a pair of registers.
** We have to be careful not to return the very last register.
*/

static int
findpair()
{
    int rnum;

    updrfree();				/* update regfree[] to pbuf contents */
    for (rnum = MINREG; rnum < MAXREG; rnum++) /* try for unused reg */
	if (regfree[rnum] && regfree[rnum+1]) return rnum;
    for (rnum = MINREG; rnum < MAXREG; rnum++)	/* no, try for a free one */
	if (regis[rnum] == NULL && regis[rnum+1] == NULL) return rnum;
    spillone (regis[MINREG]);		/* no, spill something */
    spillone (regis[MINREG+1]);		/* and another to make a pair */
    return MINREG;
}

/*
** Set a virtual register's location to be some real reg
*/

static void
setreg (reg, loc)
struct vreg *reg;
{
    reg->location = loc;
    regis[loc] = reg;
}

/*
** Same, but set it for a double register pair
*/

static void
setpair (reg, loc)
struct vreg *reg;
{
    reg->location = loc;
    regis[loc] = reg;
    regis[loc+1] = reg;
    reg->flags |= REGPAIR;
}

/*
** Assign a new register
*/

struct vreg *
getreg()
{
    struct vreg *reg;
    reg = newreg();
    setreg (reg, findreg());
    linkreg (reg, &reglist);
    return reg;
}

/*
** Assign a pair of registers
*/

struct vreg *
getpair()
{
    struct vreg *reg;
    reg = newreg();
    setpair (reg, findpair());
    linkreg (reg, &reglist);
    return reg;
}

/*
** Get a register for holding a return value
*/

struct vreg *
getret()
{
    struct vreg *reg = newreg();
    if (regis[RETVAL] != NULL) spillone(regis[RETVAL]);
    setreg (reg, RETVAL);
    linkreg (reg, &reglist);
    return reg;
}

/*
** Return a doubleword
*/

struct vreg *
getrpair()
{
    struct vreg *reg = newreg();
    if (regis[RETVAL] != NULL) spillone(regis[RETVAL]);
    if (regis[RETDBL] != NULL) spillone(regis[RETDBL]);
    setpair (reg, RETVAL);
    linkreg (reg, &reglist);
    return reg;
}

/*
** Forget about a no-longer-in-use register
** The reg can be a small integer, in which case we ignore it.
*/

void
release(reg)
struct vreg *reg;
{
    if ((int) reg < NREGS) return;
    if (reg->flags & SPILLED) warn (ERELREG, "release of a spilled register");
    unlink (reg);
    regis[reg->location] = NULL;
    if (reg->flags & REGPAIR) regis[reg->location + 1] = NULL;
    linkreg (reg, &freelist);
}

/*
** Make a singleword register into a doubleword
*/

void
widen(reg)
struct vreg *reg;
{
    int nreg;

    if (reg->flags & REGPAIR) warn (ERELREG, "doubly widened register");
    reg->flags |= REGPAIR;		/* make it wide */
    if (reg->flags & SPILLED) return;	/* on stack, don't need to find reg */
    if (reg->location < MAXREG && regis[reg->location+1] == NULL) {
	regis[reg->location+1] = reg;	/* space available after it */
	return;				/* take it and return */
    }
    nreg = findpair();			/* not available, get new pair */
    code0 (MOVE, nreg, reg->location);	/* move registers across */
    regis[reg->location] = NULL;	/* free old place */
    setpair (reg, nreg);		/* and set up new one instead */
}


/*
** Extract one word of a doubleword register
** If the second arg is one, we take the second word rather than the first
*/

void
narrow(reg, low)
struct vreg *reg;
{
    if (!(reg->flags & REGPAIR)) warn(ERELREG, "doubly narrowed register");
    if (low) regis[reg->location++] = NULL;
    else regis[reg->location + 1] = NULL;
    reg->flags &=~ REGPAIR;
}

/*
** Return a physical register number for some virtual register
** Pulls it back off the stack if necessary
*/

int
realreg(reg)
struct vreg *reg;
{
    if ((int) reg < NREGS) return (int) reg;

    /* check for spilled register now somewhere on stack */
    if (reg->flags & SPILLED) {
	int nreg;

	/* put it back into a register */
	reg->flags &=~ SPILLED;
	unlink (reg);
	if (reg->flags & REGPAIR) {
	    code12 (DMOVE, nreg = findpair(), reg->location - 1 - stackoffset);
	    setpair (reg, nreg);
	} else {
	    code12 (MOVE, nreg = findreg(), reg->location - stackoffset);
	    setreg (reg, nreg);
	}

	/* drop stack to top remaining spilled reg */
	if (reg->next == spillist.next) {
	    code8 (ADJSP, SP, spillist.next->prevloc - stackoffset);
	    stackoffset = spillist.next->prevloc;
	}
	linkreg (reg, &reglist);
    }

    /* in a reg now no matter what, so just return it */
    return reg->location;
}

/*
** Return whether register is still assigned
** This is necessary because the outside world can't see regis[]
*/

int
rfree(reg)
{
    return regis[reg] == NULL;
}

/*
** Hack to back up over MOVE R,S leaving resulting reg allocated.
** Used by switch case jump generation to avoid lossage from ufcreg().
*/

void
backreg (reg)
struct vreg *reg;
{
    if ((int) reg < NREGS) return;	/* only for virtual register */
    regis[realreg (reg)] = NULL;	/* swap in and deassign */
    regis[reg->location = ufcreg (reg->location)] = reg; /* reassign */
}
