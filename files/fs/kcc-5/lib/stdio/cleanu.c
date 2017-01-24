/*
 *	_CLEANUP - final cleanup for stdio upon exit
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"

void _cleanup()
{
    int i;
    FILE *fp;

    for (i = 0; i < SYS_OPEN; i++)		/* close static FILEs */
	if (_sios[i].sioflgs & _SIOF_OPEN)
	    fclose(&_sios[i]);

    while (_FILE_head) {			/* close dynamic FILEs */
	fp = _FILE_head->sionFILE;		/* next FILE in chain */
	fclose(_FILE_head);
	_FILE_head = fp;
    }	
}
