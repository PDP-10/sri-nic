/*==================================================
 *  math.h
 *	#include file for math library support for the V7 C library
 *
 *	Copyright (C) New Mexico Tech 1984, 1985.  All rights reserved.
 *
 *	Author: Greg Titus
 */

/*
 *  Has this file already been #included?  Matching #endif is at bottom of file
 */
#ifndef MATH_INCLUDED

#define MATH_INCLUDED

/*
 *  declare the FORTRAN math functions
 */
fortran float	    exp();
fortran float	    alog(), alog10();
fortran float	    sqrt();
fortran float	    sin(), sind(), cos(), cosd(), tan(), cotan();
fortran float	    asin(), acos(), atan(), atan2();
fortran float	    sinh(), cosh(), tanh();

#endif	/* MATH_INCLUDED */
