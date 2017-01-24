/* 
 * tops20.h - Top-level include file for system routines
 * 
 * Author:	Jay Lepreau
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	19 April 1981
 * Copyright (c) 1981,1982 Jay Lepreau
 *
 *	Extended 1/82 by fjl.
 */

#ifndef	_TOPS20H
#define	_TOPS20H

#define	RELEASE5
#undef	RELEASE4

#include <jsys.h>
#include <byteptr.h>

#define	PAGESIZE 512
#define PAGEMAX  511

#define MAXUSERL (39 * 5)	/* max reasonable length for a username */

#define MAXFORK	 30

#define SYSERR	 999

union _word {
    unsigned full;
    struct {
	unsigned left  :18;
	unsigned right :18; 
    } half;
};

typedef union _word WORD;
typedef int bool;

typedef unsigned BYTEPTR7;		/* 7 bit bp. for now */

#define CTRL(c)	('c' & 037)
#define ESCAPE	033
#define RUBOUT 0177

#define setbit(x)	(x)
#define makeword(l, r)	( ((l) << 18) | (r) )
#define makeleft(b)	( (b) << 18 )
#define getright(b)	( (b) & 0777777 )
#define getleft(b)	( (b) >> 18 )	/* get left half.		     */
#define getpleft(b)	( (b) >> 18 )	/* get positive left half.	     */
#define getnleft(b)	( ((b)>>18) | ~0777777 )/* neg lhs. not arith shift! */

/* Macros to handle monsym like field defines of form mask:shift */

/* Don't expect to use these at source level */
#define Getmask(name) 	 ( 1?name )
#define Getshift(name)	 ( 0?name )
#define Absmask(name) ( (1?name) << (0?name) )    /* maybe use this one */
#define Value(name)   ( (1?name) << (0?name) )    /* maybe use this one */

/* These are for real use */ 
#define makefield(name, value)	( ((value) & Getmask(name)) << Getshift(name) )
#define getfield(var, name)	( (var) >> Getshift(name) & Getmask(name) )

#define setfield(var, name, value) ( (var) = ((var) & ~Absmask(name)) | makefield(name, value) )

#define pagetoloc(page)	( (page) << 9 )
#define jfnof(fd)	( (fd) <= 2 ? fileno(&_iob[fd]) : fd )

extern int  _errno;
extern int  _JSacs[5];		/* should go in jsys */		/* 4 later */

#define acs _JSacs
#define ac1 _JSacs[1]		/* 0-3 later */
#define ac2 _JSacs[2]
#define ac3 _JSacs[3]
#define ac4 _JSacs[4]

#ifndef min
# define min(a,b) ( (a) <= (b) ? (a) : (b) )
# define max(a,b) ( (a) >= (b) ? (a) : (b) )
#endif

char *_numtodir();

#endif _TOPS20H
