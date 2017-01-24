/* SYS/SITDEP.H - Site-dependent system parameters.
**	All of the entries herein are specifically intended to be modified
**	individually for each site.
**
**	Copyright (c) 1987 by Ken Harrenstien, SRI International
** This is an invention for the KCC implementation and is not intended
** to simulate any Un*x file.
*/

/* Define local time zone.  This value is in minutes westward of
** Greenwich Mean Time.  If site is eastward of Greenwich, the value should
** be negative.
** See the library TIME.C module for the place this is stored (and may be
** patched).
*/
#ifndef _SITE_TIMEZONE
#define _SITE_TIMEZONE (8*60)
#endif

/* Define local form of DST (Daylight Savings Time) correction.
**	The possible choices are defined in <sys/time.h>.
** See the library TIME.C module for the place this is stored (and may be
** patched).
*/
#ifndef _SITE_DSTTIME
#define _SITE_DSTTIME _DST_USA
#endif
