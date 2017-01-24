/*
 *	TMPFILE - create a temporary binary file
 *
 *	This code conforms with the description of the tmpfile()
 *	function in the ANSI X3J11 C language standard, section
 *	4.9.4.3
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "stdio.h"
#include "jsys.h"

FILE *tmpfile()
{
    return fopen(tmpnam(NULL), "w+b");
}
