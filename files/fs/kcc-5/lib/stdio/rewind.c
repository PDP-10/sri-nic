/*
 *	REWIND - rewind a stream back to the beginning
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "sys/file.h"

void rewind(f)
FILE *f;
{
    fseek(f, 0L, L_SET);
}
