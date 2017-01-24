/*
 *	FPUTS - put a string to a stream
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "string.h"

int fputs(s, f)
register char *s;
register FILE *f;
{
    register int c;

    if (!_writeable(f))
	return EOF;
    if (f->sioflgs & _SIOF_BUF) {
	while ((c = *s++) && (c = putc(c, f)) != EOF) ;
	return c;
    } else
	return (write(f->siofd, s, strlen(s)) > 0) ? 0 : EOF;
}
