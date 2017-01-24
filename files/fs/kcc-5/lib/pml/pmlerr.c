/************************************************************************
 *									*
 *		PORTABLE MATH LIBRARY -- ERROR HANDLER			*
 *									*
 *	This is a sample PML library error handler.			*
 *	It may be used as is, or another of the user's choice		*
 *	substituted.							*
 *									*
 *	In any case, the global "pmlerr" must be defined somewhere	*
 *	in the user task, since many Portable Math Library routines	*
 *	reference it.  The other routines in this file are not called	*
 *	by any library routines and may be omitted.			*
 *									*
 ************************************************************************
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

static struct pml_err {
    int flags;			/* Flag word; bits defined in pml.h	*/
    char *message;		/* Error message			*/
};

static struct pml_err pml_errs[] = {
    CONTINUE | COUNT | LOG, "DEXP overflow",
    CONTINUE | COUNT | LOG, "DEXP underflow",
    CONTINUE | COUNT | LOG, "DSCALE exponent overflow",
    CONTINUE | COUNT | LOG, "DSQRT of negative argument",
    CONTINUE | COUNT | LOG, "DLN of zero",
    CONTINUE | COUNT | LOG, "DLN of negative argument",
    CONTINUE | COUNT | LOG, "DACOS argument magnitude greater than 1.0",
    CONTINUE | COUNT | LOG, "DASIN argument magnitude greater than 1.0",
    CONTINUE | COUNT | LOG, "DTAN overflow",
    CONTINUE | COUNT | LOG, "DCOSH overflow",
    CONTINUE | COUNT | LOG, "DCOSH underflow",
    CONTINUE | COUNT | LOG, "DSINH overflow",
    CONTINUE | COUNT | LOG, "DSINH underflow",
    CONTINUE | COUNT | LOG, "DASINH overflow",
    CONTINUE | COUNT | LOG, "DACOSH argument less than 1.0",
    CONTINUE | COUNT | LOG, "DACOSH overflow",
    CONTINUE | COUNT | LOG, "DATANH argument magnitude greater than or = 1.0",
    CONTINUE | COUNT | LOG, "DATAN underflow",
    CONTINUE | COUNT | LOG, "Complex division by zero",
    CONTINUE | COUNT | LOG, "Complex reciprocal of zero",
    CONTINUE | COUNT | LOG, "DSCALE exponent underflow",
    CONTINUE | COUNT | LOG, "DINT argument has no fractional part"
};

static int err_count = 0;	 /* Counter for PML errors */
static int err_limit =  MAX_ERRORS; /* PML error limit */

/*
 *	+++ NAME +++
 *
 *	 PMLCFS   Clear specified PML error handler flags
 *
 *	+++ INDEX +++
 *
 *	 PMLCFS
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Clear the specified PML error handler flags for the
 *	specified error.  Two or more flags may be cleared simultaneously
 *	by "or-ing" them in the call, for example "LOG | CONTINUE".
 *	The manifest constants for the flags and error codes are
 *	defined in "c:pmluse.h".
 *
 *	+++ USAGE +++
 *
 *	 pmlcfs(err_code,flags)
 *	 int err_code;
 *	 int flags;
 *
 *	+++ PROGRAMMER +++
 *
 *	 Fred Fish
 *	 Goodyear Aerospace Corp, Arizona Div.
 *	 (602) 932-7000 work
 *	 (602) 894-6881 home
 *
 *	---
 */

pmlcfs(err_code,flag_word)
register int err_code;
register int flag_word;
{
    if (err_code < 0 || err_code > (sizeof(pml_errs)/sizeof(struct pml_err))) {
	fprintf(stderr,"? PMLCFS --- invalid error code %d.\n",err_code);
    } else {
	pml_errs[err_code].flags &= ~flag_word;
    }
}


/*
 *	+++ NAME +++
 *
 *	 PMLCNT   Get PML error count and reset it to zero
 *
 *	+++ INDEX +++
 *
 *	 PMLCNT
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns the total number of PML errors seen
 *	prior to the call, and resets the error count to zero.
 *
 *	+++ USAGE +++
 *
 *	int pmlcnt()
 *
 *	+++ PROGRAMMER +++
 *
 *	 Fred Fish
 *	 Goodyear Aerospace Corp, Arizona Div.
 *	 (602) 932-7000 work
 *	 (602) 894-6881 home
 *
 *	---
 */

int pmlcnt()
{
    register int rtn_val;

    rtn_val = err_count;
    err_count = 0;
    return(rtn_val);
}


/*
 *	+++ NAME +++
 *
 *	 PMLERR   Portable Math Library error handler
 *
 *	+++ INDEX +++
 *
 *	 PMLERR
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Provides a sample PML error handler.  Does
 *	not use any available hardware "traps" so is machine
 *	independent.  Generally only called internally by the
 *	other PML routines.
 *
 *	There are currently three flags which control the
 *	response for specific errors:
 *
 *	 (1)  LOG      When set an error message is sent
 *	               to the user terminal.
 *
 *	 (2)  COUNT    When set the error is counted
 *	               against the PML error limit.
 *
 *	 (3) CONTINUE  When set the task continues
 *	               providing the error count has not
 *	               exceeded the PML error limit.
 *
 *	Each of these flags can be set or reset independently
 *	by "pmlsfs" or "pmlcfs" respectively.
 *
 *	+++ USAGE +++
 *
 *	 pmlerr(err_code)
 *	 int err_code;
 *
 *	+++ PROGRAMMER +++
 *
 *	 Fred Fish
 *	 Goodyear Aerospace Corp, Arizona Div.
 *	 (602) 932-7000 work
 *	 (602) 894-6881 home
 *
 *	---
 */

pmlerr(err_code)
register int err_code;
{
    register struct pml_err *err;

    if (err_code < 0 || err_code > (sizeof(pml_errs)/sizeof(struct pml_err))) {
	fprintf(stderr,"? PMLERR --- invalid error code %d.\n",err_code);
    } else {
	err = &pml_errs[err_code];
	if (err->flags & LOG) {
	    fprintf(stderr,"? WARNING --- %s.\n",err->message);
	}
	if (err->flags & COUNT) {
	    err_count++;
	}
	if ((err->flags & CONTINUE) && (err_count <= err_limit)) {
	    return;
	} else {
	    fprintf(stderr,"? FATAL --- PML error limit exceeded.\n");
	    fprintf(stderr, "? FATAL --- Task aborted with %d error(s).\n",err_count);
	    exit();
	}
    }
}


/*
 *	+++ NAME +++
 *
 *	 PMLLIM   Set Portable Math Library error limit
 *
 *	+++ INDEX +++
 *
 *	 PMLLIM
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Sets the PML error limit to the specified value
 *	and returns it previous value.
 *	Does not affect the current error count (which may be reset
 *	to zero by a call to "pmlcnt").  Note that the default error
 *	limit is set at compile time by the value in "pml.h".
 *
 *	+++ USAGE +++
 *
 *	 int pmllim(limit)
 *	 int limit;
 *
 *	+++ PROGRAMMER +++
 *
 *	 Fred Fish
 *	 Goodyear Aerospace Corp, Arizona Div.
 *	 (602) 932-7000 work
 *	 (602) 894-6881 home
 *
 *	---
 */

int pmllim(limit)
register int limit;
{
    register int rtn_val;

    rtn_val = err_limit;
    err_limit = limit;
    return(rtn_val);
}


/*
 *	+++ NAME +++
 *
 *	 PMLSFS   Set specified PML error handler flags
 *
 *	+++ INDEX +++
 *
 *	 PMLSFS
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Set the specified PML error handler flags for the
 *	specified error.  Two or more flags may be set simultaneously
 *	by "or-ing" them in the call, for example "LOG | CONTINUE".
 *	The manifest constants for the flags and error codes are
 *	defined in "c:pmluse.h".
 *
 *	+++ USAGE +++
 *
 *	 pmlsfs(err_code,flags)
 *	 int err_code;
 *	 int flags;
 *
 *	+++ PROGRAMMER +++
 *
 *	 Fred Fish
 *	 Goodyear Aerospace Corp, Arizona Div.
 *	 (602) 932-7000 work
 *	 (602) 894-6881 home
 *
 *	---
 */

pmlsfs(err_code,flag_word)
register int err_code;
register int flag_word;
{
    if (err_code < 0 || err_code > (sizeof(pml_errs)/sizeof(struct pml_err))) {
	fprintf(stderr,"? ERRSET --- invalid error code %d.\n",err_code);
    } else {
	pml_errs[err_code].flags |= flag_word;
    }
}
