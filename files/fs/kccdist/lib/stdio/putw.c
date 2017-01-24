/*
 *	PUTW - write an int to a stream
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "c-env.h"

int putw(w, f)
int w;
FILE *f;
{
    if (!_writeable(f)) return EOF;
#if SYS_T20+SYS_10X
    putc((w & 0777000000000) >> (9 * 3), f);
    putc((w & 0777000000) >> (9 * 2), f);
    putc((w & 0777000) >> 9, f);
    putc(w & 0777, f);
    return w;
#else
    return EOF;
#endif
}
