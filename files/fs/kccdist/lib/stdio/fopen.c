/*
 *	FOPEN - open a file
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"

/*
 *	this is initial FILE-block storage for std streams (stdin, stdout,
 *	and stderr, the first three slots respectively), plus extra storage
 *	for user files.  this is a static array so that storage for the
 *	first n FILE blocks is pre-allocated, so there is no chance of
 *	not having enough memory to allocate space for one.
 */
#include "sys/usysio.h"		/* Get defs for STDIN_CH etc */

FILE _sios[SYS_OPEN] = {
    {_SIOF_OPEN,0,0,0,0,(_SIOF_READ |_SIOF_AUTOBUF),		   STDIN_CH},
    {_SIOF_OPEN,0,0,0,0,(_SIOF_WRITE|_SIOF_AUTOBUF|_SIOF_LINEBUF), STDOUT_CH},
    {_SIOF_OPEN,0,0,0,0,(_SIOF_WRITE),				   STDERR_CH}
};

/*
 *	this is the head of the linked list of FILE blocks beyond those
 *	in the previous static array.  when that array is filled, new
 *	blocks are allocated dynamically and added to this linklist.
 */

FILE *_FILE_head = NULL;

FILE *fopen(pathname, type)
char *pathname, *type;
{
    FILE *f;

    if (f = _makeFILE()) {				/* get a FILE block */
	if (freopen(pathname, type, f))			/* if can open file */
	    return f;					/* return FILE ptr */
	_freeFILE(f);					/* lose, release it */
    }
    return NULL;					/* barf return */
}

/*
 *	allocate a FILE block.  first check to see if there are any free
 *	blocks in the static array.  if there are none, then make a new
 *	block, flag it as dynamically allocated, and add it to the linked
 *	list.
 */

FILE *_makeFILE()
{
    int i;
    FILE *f;
    extern char *calloc();
/*
 *	look for a free spot in the static _sios[] array of FILE blocks.
 *	DON'T check the first three slots, which are reserved for stdin,
 *	stdout, and stderr respectively.
 */
    for (f = stderr+1; f < &_sios[SYS_OPEN]; f++)
	if (!(f->sioflgs & _SIOF_OPEN)) {
	    f->siocheck = _SIOF_FILE;		/* mark as valid FILE block */
	    return f;
	}
/*
 *	no free slots, so allocate a new FILE block
 */
    if ((f = (FILE *) calloc(1, sizeof(FILE))) == NULL)
	return NULL;			/* lost, couldn't make one */
    else {
	f->sioflgs |= _SIOF_DYNAMFILE;	/* success, splice this new block */
	f->siopFILE = NULL;		/* into the linklist of dynamically */
	f->sionFILE = _FILE_head;	/* allocated blocks, to keep track */
	f->siocheck = _SIOF_FILE;	/* mark as valid FILE block */
	return _FILE_head = f;		/* of them. */
    }
}

/*
 *	release a FILE block.  if it was dynamically allocated, then
 *	remove it from the linked list of FILE blocks and release it,
 *	else it was a static block, so zero the flag word so we will
 *	see it as available in _makeFILE()
 */

void _freeFILE(f)
FILE *f;
{
    if (f->sioflgs & _SIOF_DYNAMFILE) {
/*
 *	if there's no previous pointer then this is the first block in the
 *	linklist, so "previous" is the head pointer.
 */
	if (f->siopFILE)
	    f->siopFILE->sionFILE = f->sionFILE;
	else _FILE_head = f->sionFILE;
	if (f->sionFILE)
	    f->sionFILE->siopFILE = f->siopFILE;
	free((char *) f);
    } else {
	f->siocheck = f->sioflgs = 0;	/* nuke the open flag! */
	f->siofd = -1;			/* is this really necessary? */
    }
}
