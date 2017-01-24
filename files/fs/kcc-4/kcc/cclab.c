/*	CCLAB.C - Label management routines
**
**	All changes after version 6 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/*
** cclab - Manage internal labels for kcc assembly output
** David Eppstein / Stanford University / 29 May 1985
*/

#include "cc.h"
#include "ccgen.h"

/* Exported functions */
void initlab(), reflabel(), freelabel(), emitlabel(), emitgoto();
SYMBOL *newlabel();

/* Imported functions */
extern char *malloc();

/* Internal functions */
static void realfreelabel();

static int maxlabel;		/* Current highest-numbered label */
static SYMBOL *fllist = NULL;	/* Free label list */
static SYMBOL *flprev;		/* almost-free label list */
static int nlabels = 0;		/* # labels allocated */

/* INITLAB - Initialize label stuff.
**	Called by INITSYM at start of compilation for each file.
*/
void
initlab()
{
    maxlabel = 0;		/* Reset internal label numbering to 0 */
    flprev = NULL;		/* Ensure this list is empty */
	/* Note that "fllist" is left alone in case it contains free labels,
	** which will save us the bother of allocating them.
	*/
}

/*
** Get a new label to play with.
**
** The argument should be nonzero if the label will be emitted after all
** uses of it, rather than before.  If it is zero, the label is emitted.
*/

SYMBOL *
newlabel(forward)
{
    SYMBOL * lab;

    /* find a free label */
    if (lab = fllist)			/* If have one already free, */
	fllist = fllist->Snext;		/* remove it from freelist */
    else if ((lab = (SYMBOL *) malloc(sizeof (*lab))) == NULL)	/* make new */
	efatal(EOUTMEM);
    else nlabels++;			/* Bump # of labels allocated */

    /* fill it out */
    lab->Sclass = SC_ILABEL;		/* this is an internal label */
    sprintf (lab->Sname, "$%d", ++maxlabel); /* give it a name */
    lab->Svalue = 0;			/* no uses yet */

    /* emit if necessary and return */
    if (!forward) glabel (lab->Sname);
    return lab;
}

/*
** Reference or dereference a label.
**
** The second argument is how much to add to the reference count.
** The label may be NULL or not a SC_ILABEL; in that case nothing happens.
*/
void
reflabel (lab, count)
SYMBOL * lab;
{
    if (lab != NULL && lab->Sclass == SC_ILABEL) lab->Svalue += count;
}

/*
** Release a label.
**
** This should be called after the last possible reference to the label.
** It will be called automatically on emission of forward labels.
** Note sprev must not be changed (see emitlabel).
*/

static void
realfreelabel (lab)
SYMBOL * lab;
{
    lab->Snext = fllist;		/* chain old freelist onto it */
    fllist = lab;			/* it is now head of freelist */
}

/*
** Unfortunately we can't know when the last instance in the peephole
** buffer has been emitted, at least until we flush the whole thing out.
** So we keep explicitly freed labels on another list and only transfer
** them after we have emitted a new label (and thus called flushcode()).
**
** This list chains through sprev rather than snext to keep things simple.
*/
void
freelabel (lab)
SYMBOL * lab;
{
    lab->Sprev = flprev;		/* chain old freelist onto it */
    flprev = lab;			/* it is now head of freelist */
}

/*
** Emit a (forward) label.
**
** Optimizations are performed and if the label still has references
** to it, it is emitted.  Then realfreelabel() is called on it, and
** if we emitted it and thus cleared the peephole buffer we also free
** the list of labels queued by freelabel().
*/
void
emitlabel (lab)
SYMBOL * lab;
{
    int after = hackstack (lab);	/* pull ADJSP across POPJ */
    optlab (lab);			/* call peephole optimizer */
    if (lab->Svalue > 0) {
	glabel (lab->Sname);		/* emit if necessary */
	while (flprev != NULL) {	/* peephole buffer is now empty */
	    realfreelabel (flprev);	/* so free the list of labels queued */
	    flprev = flprev->Sprev;	/* by freelabel() */
	}				/* (nb sprev unchanged by realfree) */
    }
    if (after) code8 (P_ADJSP, R_SP, after); /* fix up stack */
    realfreelabel (lab);
}

/*
** Emit a GOTO label
** These are not as well behaved as loop and if labels so we can't do as much.
*/
void
emitgoto (lab)
SYMBOL * lab;
{
    optlab (lab);			/* optimize */
    glabel (lab->Sname);		/* and unconditionally send out */
}
