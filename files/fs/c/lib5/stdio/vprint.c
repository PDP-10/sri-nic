/*
 *	VFPRINTF, VPRINTF, VSPRINTF
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "varargs.h"

extern int _doprnt();			/* from printf.c */

int vfprintf(stream, format, arg)
FILE *stream;
char *format;
va_list arg;
{
    return _doprnt(format, &arg, stream);
}

int vprintf(format, arg)
char *format;
va_list arg;
{
    return _doprnt(format, &arg, stdout);
}

int vsprintf(s, format, arg)
char *s, *format;
va_list arg;
{
    FILE *f;
    int result;

    f = sopen(s, "w", 32767);		/* open string for output */
    result = _doprnt(format, &arg, f);	/* write to the string... */
    putc('\0', f);			/* null-terminate result */
    fclose(f);				/* "close" the string */
    return result;
}
