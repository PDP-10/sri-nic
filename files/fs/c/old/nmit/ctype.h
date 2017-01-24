/*==================================================
 *  ctype.h
 *	#include file for character typing for the V7 C library
 *
 *	Copyright (C) New Mexico Tech 1984, 1985.  All rights reserved.
 *
 *	Author: Greg Titus
 */

/*
 *  Has this file already been #included?  Matching #endif is at bottom of file
 */
#ifndef CTYPE_INCLUDED

#define CTYPE_INCLUDED

#define	_UC_	    0001
#define	_LC_	    0002
#define	_NUM_	    0004
#define	_WS_	    0010
#define _PUN_	    0020
#define _CTRL_	    0040
#define _OCT_	    0100
#define _HEX_	    0200
#define _SPACE_	    0400

extern	int	    _ctype_[];

#define	isalpha(c)  ((_ctype_+1)[c]&(_UC_|_LC_))
#define	isupper(c)  ((_ctype_+1)[c]&_UC_)
#define	islower(c)  ((_ctype_+1)[c]&_LC_)
#define	isdigit(c)  ((_ctype_+1)[c]&_NUM_)
#define	isxdigit(c) ((_ctype_+1)[c]&(_NUM_|_HEX_))
#define	isodigit(c) ((_ctype_+1)[c]&_OCT_)
#define	isspace(c)  ((_ctype_+1)[c]&_WS_)
#define ispunct(c)  ((_ctype_+1)[c]&_PUN_)
#define isalnum(c)  ((_ctype_+1)[c]&(_UC_|_LC_|_NUM_))
#define isprint(c)  ((_ctype_+1)[c]&(_SPACE_|_PUN_|_UC_|_LC_|_NUM_))
#define iscntrl(c)  ((_ctype_+1)[c]&_CTRL_)
#define isascii(c)  ((unsigned)(c)<=0177)
#define toupper(c)  ((c)-'a'+'A')
#define tolower(c)  ((c)-'A'+'a')
#define toascii(c)  ((c)&0177)

#endif	/* CTYPE_INCLUDED */
