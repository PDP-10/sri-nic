/*
 *	GETW - get an int from a stream
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "c-env.h"

int getw(f)
FILE *f;
{
    int c, i, value;

    if (!_readable(f)) return EOF;
#if SYS_T20+SYS_10X
    value = 0;					/* return value */
    for (i = 0; i < 4; i++) {
	if ((c = getc(f)) == EOF) return EOF;
	else value = (value << 9) | c;
    }
    return value;
#else
    return EOF;
#endif
}
