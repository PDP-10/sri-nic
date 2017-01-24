/*
** malloc - memory allocation for C programs (first fit with rover)
** David Eppstein / Stanford University / 26-Jul-84
**
** Blocks are an integral number of words long, and there is at least
** one data word (to ensure room for the freelist pointer).  It is
** assumed that integer pointers are the same size as integers, and
** that sizeof(char) is 1.
**
** Block in use, pointed to by a byte pointer:
**		     ________________________________________________
**		    |						     |
**		    | Number of words in block (including this word) |
**		    |________________________________________________|
** Byte ptr-------->|						     |
**		    | Block data				     |
**		    \						     \
**		    \						     \
**		    |________________________________________________|
**
**
** Block on free list:
**		     ________________________________________________
** Ptr from prev--->|						     |
**		    | Number of words in block (including this word) |
**		    |________________________________________________|
**		    |						     |
**		    | Pointer to next block in circular freelist     |
**		    |________________________________________________|
**		    |						     |
**		    | Rest of block unused			     |
**		    \						     \
**		    \						     \
**		    |________________________________________________|
**
**
** The variable rover points to some block in the circular free block list;
** when the freelist is empty rover contains NULL.
*/

entry malloc, free, realloc;

#define NBYTES	(sizeof (int))
#define NULL	0

char *sbrk();

/* --------------------------------------------- */
/*	initialization for freelist pointer	 */
/* --------------------------------------------- */

static int *zblock()
{
    return NULL;			/* fn not val so re-init on re-start */
}

static int *rover = zblock();

/* ------------------------------------ */
/*	allocate a block of memory	*/
/* ------------------------------------ */

char *malloc(nbytes)
{
    int *p, *q, nwords;
    char *align;

    /* calculate number of words needed in block */
    if (nbytes < 0) return NULL;	/* range check argument */
    nwords = (nbytes + (2 * NBYTES - 1)) / NBYTES; /* calc num words needed */

    /* look through the free list for a fit */
    p = rover;				/* remember where we started */
    q = NULL;				/* don't do at all if nothing there */
    while (q != p) {
	q = rover;			/* remember old pointer */
	rover = (int *) rover[1];	/* and move over one */
	if (rover[0] >= nwords) {	/* found a big enough block? */
	    p = rover;			/* yes, remember where it is */
	    if (q == rover) rover = NULL; /* take last one from list */
	    else rover = (int *) (q[1] = rover[1]); /* or splice out of list */
	    split (p, nwords);		/* maybe break off unneeded portion */
	    return (char *) (p + 1);	/* return what we found */
	}
    }

    /* no large enough blocks were found, make a new one */
    align = sbrk (0);			/* see how we're aligned */
    if (align != (char *) (int *) align) brk ((char *) (1 + (int *) align));
    align = sbrk (nwords * NBYTES);	/* allocate a new memory block */
    if ((int) align == -1) return NULL;	/* no more room at the inn */
    p = (int *) align;			/* got one, make word pointer */
    p[0] = nwords;			/* remember how many words in it */
    return (char *) (p + 1);		/* return next word as char pointer */
}


/* --------------------------------------------------------------- */
/*	break up a block into two smaller blocks if necessary	   */
/* --------------------------------------------------------------- */

static split(block,newsize)
int *block;
{
    if (block[0] >= newsize+2) {	/* enough room for another block? */
	block[newsize] = block[0] - newsize;	/* yes, make sizes of the */
	block[0] = newsize;			/* new smaller blocks */
	free((char *) (block+newsize+1));	/* throw back unused part */
    }
}

/* ------------------------------------------- */
/*      free a previously allocated block      */
/* ------------------------------------------- */

free(cp)
char *cp;
{
    int *p;

    p = ((int *) cp) - 1;		/* point back to start of block */
    if (rover == NULL) rover = (int *) (p[1] = (int) p); /* make circle list */
    else {
	p[1] = rover[1];		/* already have one, link to it */
	rover[1] = (int) p;		/* and splice in. */
    }
}


/* ------------------------------------------------------------- */
/*      make new block with old contents but different size      */
/* ------------------------------------------------------------- */

char *
realloc(cp, nbytes)
char *cp;
{
    char *op;
    int *p, *q, nwords;

    p = ((int *) cp) - 1;		/* point back to start of block */
    if (nbytes < 0) return NULL;	/* range check argument */
    nwords = (nbytes + (2 * NBYTES - 1)) / NBYTES; /* calc num words needed */
    if (nwords <= p[0]) split(p, nwords); /* shrinking, break up block */
    else {				/* bigger, so reallocate and copy */
	op = cp;			/* first save the original pointer */
	cp = malloc(nbytes);		/* make a bigger block */
	if (cp != NULL) {
	    q = ((int *) cp) - 1;	/* get word pointer */
	    while (--nwords > 1) *++q = *++p; /* copy data portion */
	}
	free(op);			/* get rid of old block */
    }
    return cp;				/* return updated char pointer */
}
