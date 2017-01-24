/*
** cclab - Manage internal labels for kcc assembly output
** David Eppstein / Stanford University / 29 May 1985
*/

#include "ccsym.h"
#include "ccgen.h"

typedef struct SYMBOL *label;
char *malloc();

/*
** Get a new label to play with.
**
** The argument should be nonzero if the label will be emitted after all
** uses of it, rather than before.  If it is zero, the label is emitted.
*/

label newlabel (forward)
{
    label lab;

    /* find a free label */
    if (fllist == NULL) lab = (label) malloc (sizeof (*lab)); /* make new */
    else {				/* else have something in freelist */
	lab = fllist;			/* so use head for this label */
	fllist = fllist->snext;		/* and remove it from freelist */
    }

    /* fill it out */
    lab->sclass = SILABEL;		/* this is an internal label */
    sprintf (lab->sname, "$%d", ++maxlabel); /* give it a name */
    lab->svalue = 0;			/* no uses yet */

    /* emit if necessary and return */
    if (!forward) glabel (lab->sname);
    return lab;
}

/*
** Reference or dereference a label.
**
** The second argument is how much to add to the reference count.
** The label may be NULL or not a SILABEL; in that case nothing happens.
*/

reflabel (lab, count)
label lab;
{
    if (lab != NULL && lab->sclass == SILABEL) lab->svalue += count;
}

/*
** Release a label.
**
** This should be called after the last possible reference to the label.
** It will be called automatically on emission of forward labels.
** Note sprev must not be changed (see emitlabel).
*/

static
realfreelabel (lab)
label lab;
{
    lab->snext = fllist;		/* chain old freelist onto it */
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

freelabel (lab)
label lab;
{
    lab->sprev = flprev;		/* chain old freelist onto it */
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

emitlabel (lab)
label lab;
{
    int after = hackstack (lab);	/* pull ADJSP across POPJ */
    optlab (lab);			/* call peephole optimizer */
    if (lab->svalue > 0) {
	glabel (lab->sname);		/* emit if necessary */
	while (flprev != NULL) {	/* peephole buffer is now empty */
	    realfreelabel (flprev);	/* so free the list of labels queued */
	    flprev = flprev->sprev;	/* by freelabel() */
	}				/* (nb sprev unchanged by realfree) */
    }
    if (after) code8 (ADJSP, SP, after); /* fix up stack */
    realfreelabel (lab);
}

/*
** Emit a GOTO label
** These are not as well behaved as loop and if labels so we can't do as much.
*/

emitgoto (lab)
label lab;
{
    optlab (lab);			/* optimize */
    glabel (lab->sname);		/* and unconditionally send out */
}
