/*
** <assert.h> - Diagnostic macro facility.
**
**	Partially implements the assert() macro as described by the
** ANSI C draft.  A full implementation would require ANSI-style preprocessing
** which most compilers don't know about yet.  Sigh.  --KLH 9/26/86
** Note stderr had better not be buffered!
*/

#ifndef NDEBUG
#define assert(e) ((e) ? 0 :\
	(fprintf(stderr,"Assertion failed: file %s, line %d\n",\
	__FILE__, __LINE__),abort(),0))
#else
#define assert(e)
#endif
