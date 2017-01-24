/*
 *	FWRITE - binary write
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"

int fwrite(ptr, size_of_ptr, count, f)
char *ptr;
unsigned size_of_ptr;
int count;
FILE *f;
{
    int number_of_bytes;

    if (!_writeable(f) || size_of_ptr < 1 || count < 1)
	return 0;
    else {
	number_of_bytes = count * size_of_ptr;
	while (number_of_bytes--)
	    putc(*ptr++, f);
	return count;
    }
}
