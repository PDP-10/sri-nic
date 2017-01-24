/*	CCPARM.H - Global parameters for KCC
**
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
**	Collected from various files, 15 Dec 85
**
** This file contains two conceptually distinct sets of parameters:
**	(1) Target Machine (System/CPU/ASM) definitions & parameters
**	(2) General KCC size parameters
** If the first category becomes much more complex it should be split
** off into its own file.
*/

/* Target machine definitions. */

/* Target System type.
**	The global "tgsys" is set to one of these values.
**	Note that for TGSYS_NULL, the source system can be found using c-env.h.
*/

enum systype {
	TGSYS_NULL,	/* Target system same as source system */
	TGSYS_TOPS20,	/* Target: DEC TOPS-20 system */
	TGSYS_WAITS,	/* Target: SU WAITS system */
	TGSYS_TENEX,	/* Target: BBN TENEX / TYMSHARE AUGUST system */
	TGSYS_TOPS10,	/* Target: DEC TOPS-10 system */
	TGSYS_ITS	/* Target: MIT ITS system */
};

/* Target CPU types.
**	 The global array "tgcpu[]" is indexed by these values.
** These all represent some variety of PDP-10.  Anything else (a non-PDP10)
** will require an overhaul of not only this typing scheme but a lot of KCC.
*/
enum cputype {
	TGCPU_KA,		/* Target: KA-10 */
	TGCPU_KI,		/* Target: KI-10 */
	TGCPU_KS,		/* Target: KS-10 and 1-section KL (KL-10A) */
	TGCPU_KL0,		/* Target: Extended KL-10, section 0 */
	TGCPU_KLX,		/* Target: Extended KL-10, non-zero section */
	TGCPU_N			/* # of target CPU types */
};

/* Target Machine data type sizes.
**	These are universal for all PDP-10s.  KCC may never be set up to
** compile for anything else.
**
** Note that the sizes are in terms of bits, rather than "char" bytes.
*/

#define TGSIZ_WORD 36		/* Size of a PDP-10 word, in bits */
#define TGSIZ_HALFWD 18		/* Size of a PDP-10 halfword in bits */
				/* This is sometimes handled specially. */

#define TGSIZ_CHAR	(TGSIZ_WORD/4)	/* Default size of a char byte */
#define TGSIZ_SHORT	(TGSIZ_WORD/2)
#define TGSIZ_INT	TGSIZ_WORD
#define TGSIZ_LONG	TGSIZ_WORD
#define TGSIZ_FLOAT	TGSIZ_WORD
#define TGSIZ_DOUBLE	(TGSIZ_WORD*2)
#define TGSIZ_LNGDBL	(TGSIZ_WORD*2)
#define TGSIZ_PTR	TGSIZ_WORD
#define TGSIZ_ENUM	TGSIZ_WORD

/* Target Assembler types.
**	The global "tgasm" is set to one of these values.
** This is actually more or less independent of the target system/cpu.
*/

enum asmtype {
	TGASM_FAIL,	/* SU  FAIL assembler code */
	TGASM_MACRO,	/* DEC MACRO assembler code */
	TGASM_MIDAS	/* MIT MIDAS assembler code */
};

/* Parameter definitions, mostly to do with sizes */
#ifndef _FILSIZ		/* Size of a filename string */
#define _FILSIZ (40*4+10)	/* TOPS-20 has biggest possible names */
#endif
#ifndef MAXINCLNEST	/* Max depth of nested include files */
#define MAXINCLNEST 10
#endif
#ifndef MAXNODE		/* number of nodes in static table */
#define MAXNODE 4000
#endif
#ifndef THASHSIZE	/* size of type hash table */
#define THASHSIZE 1021
#endif
#ifndef TYPES		/* number of types possible */
#define TYPES 1021
#endif
#ifndef MAXTSTACK	/* Size of token stack */
#define MAXTSTACK 16
#endif

#ifndef CPOOLSIZE	/* Size of character (string) pool */
#define CPOOLSIZE 16000
#endif
#ifndef MACPOOLSIZE	/* Size of macro arg char pool */
#define MACPOOLSIZE 4000
#endif
#ifndef ERRLSIZE	/* Size of error context line */
#define ERRLSIZE 256
#endif
#ifndef MAXMAC		/* size of immense macros */
#define MAXMAC 10000
#endif
#ifndef MAXMARG		/* Max # of macro args, cannot exceed max char val-1 */
#define MAXMARG 40
#endif
#ifndef MAXPREDEF	/* how many -D and -U predefs allowed  */
#define MAXPREDEF 20
#endif
#ifndef MAXINCDIR	/* how many -I include-file search paths allowed  */
#define MAXINCDIR 10
#endif
#ifndef MAXMACNEST	/* depth of macro nesting allowed */
#define MAXMACNEST 40
#endif
#ifndef MAXIFLEVEL	/* depth of #if nesting allowed */
#define MAXIFLEVEL 50
#endif

#ifndef MAXCASE		/* max number of cases per switch (CCGSWI) */
#define MAXCASE 512
#endif
#ifndef MAXCODE		/* Size of peephole buffer (CCGEN) */
#define MAXCODE 256
#endif			/*   Must be a power of 2?? */


/* Stuff used by CCSYM */
#ifndef _IDSIZE		/* Max identifier length in chars, plus 1 for null */
#define _IDSIZE 32
#endif
#ifndef MAXHSH		/* Symbol hashtable size (MUST be 2^N) */
#define MAXHSH 8192
#endif
