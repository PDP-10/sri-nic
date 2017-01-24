/*
 *	DIV - computer quotient & remainer simultaneously
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "stdlib.h"

div_t div(n, d)
int n, d;
{
#if !(CPU_PDP10)
    div_t result;

    result.quot = n / d;
    result.rem  = n % d;
    return result;
#else
#asm
    move 1,-1(17)		;get n
    idiv 1,-2(17)		;divide by d.  quot in AC1, rem in AC2
    popj 17,
#endasm
#endif
}

ldiv_t ldiv(n, d)
long n, d;
{
#if !(CPU_PDP10)
    ldiv_t result;

    result.quot = n / d;
    result.rem  = n % d;
    return result;
#else
#asm
    move 1,-1(17)			;n
    idiv 1,-2(17)			;ac1/ quot, ac2/ rem
    popj 17,
#endasm
#endif
}
