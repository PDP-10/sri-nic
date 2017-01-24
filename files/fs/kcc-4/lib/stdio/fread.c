/*
 *	FREAD - binary read
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"

int fread(ptr, size_of_ptr, count, f)
char *ptr;
unsigned size_of_ptr;
int count;
FILE *f;
{
    int number_read, size, c;

    if (!_readable(f) || size_of_ptr++ < 1 || count < 1)
	return 0;
    else {
	number_read = 0;
	while (count--) {
	    size = size_of_ptr;
	    while (--size && ((c = getc(f)) != EOF))
		*ptr++ = c;
	    if (!size) number_read++;
	    else break;
	}
	return number_read;
    }
}
