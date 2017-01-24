/* SYS/PARAM.H - system-dependent definitions.
**	This file on UN*X is a kernel parameter file.
**	It is used for KCC only because certain library functions direct
**	the user to include <sys/param.h> for some definitions.
*/

#ifndef MAXPATHLEN
#define MAXPATHLEN	512	/* Max size of a complete filename string, */
#endif				/* 	used by getwd(). */
