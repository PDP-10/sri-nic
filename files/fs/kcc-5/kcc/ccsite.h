/*	CCSITE.H - Site-dependent declarations for KCC
**
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/

#include "c-env.h"

/* Separator char for prefix/postfix filename components in KCC switches */
#ifndef FILE_FIX_SEP_CHAR
#define FILE_FIX_SEP_CHAR '+'
#endif

/* Define KCC standard Header File location (prefix and postfix strings)
** This is used to search for all <> include files, as well
** as the Assembler Header file.
*/
#ifdef HF_PREFIX		/* Define prefix for standard header dir */
#elif SYS_10X
#define HF_PREFIX "<C>"
#elif SYS_ITS
#define HF_PREFIX "KC;"
#elif SYS_T20
#define HF_PREFIX "C:"
#endif

#ifdef HF_POSTFIX		/* Define postfix for standard header dir */
#elif SYS_T20+SYS_10X+SYS_ITS
#define HF_POSTFIX ""
#elif SYS_WAITS
#define HF_POSTFIX "[SYS,KCC]"
#endif

/* KCC Library file prefix and postfix definitions
**	Same principle as for the header file location.
*/
#ifdef LIB_PREFIX		/* Define library file prefix */
#elif SYS_10X
#define LIB_PREFIX "<C>LIB"
#elif SYS_ITS
#define LIB_PREFIX "KC;LIB"
#elif SYS_T20
#define LIB_PREFIX "C:LIB"
#endif

#ifdef LIB_POSTFIX		/* Define library file postfix */
#elif SYS_T20+SYS_10X
#define LIB_POSTFIX ".REL"
#elif SYS_WAITS
#define LIB_POSTFIX ".REL[SYS,KCC]"
#elif SYS_ITS
#define LIB_POSTFIX " REL"
#endif

#ifndef SWI_TGASM		/* Specify default assembler */
#define SWI_TGASM TGASM_FAIL	/* Default: FAIL */
#endif

#ifndef SWI_CLEV		/* Specify default C implem level */
#define SWI_CLEV CLEV_ANSI	/* Default: ANSI insofar as implemented */
#endif
