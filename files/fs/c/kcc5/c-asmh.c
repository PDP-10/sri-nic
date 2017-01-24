/*
** C-ASMH.C   - KCC Assembler Header file
**
**	Copyright (c) 1986 by Ken Harrenstien, SRI International
**
** This file is run through KCC's preprocessor using the -E switch
** in order to generate the standard C-ASMH file which KCC inserts
** automatically, without parsing, at the beginning of all its assembler
** output files (just after any ENTRY or TITLE statements).
**
** Any independent assembler modules intended for use with KCC programs
** should also .INSERT C-ASMH; however, such modules are very, very strongly
** discouraged in favor of using #asm or asm().
**
** Keep all comments herein in slash-star form (rather than semicolons)
** so the assembler doesn't have to look at them again.
*/

#include "c-env.h"		/* Insert our configuration parameters! */

/* The header for both FAIL and MACRO is identical.
** However, MIDAS requires a radically different syntax, and for this reason
** the following switches exist to produce a MIDAS-suitable header if defined.
*/
#ifndef ASM_FAIL
#define ASM_FAIL 0
#endif
#ifndef ASM_MACRO
#define ASM_MACRO 0
#endif
#ifndef ASM_MIDAS
#define ASM_MIDAS 0
#endif
#if !(ASM_FAIL+ASM_MACRO+ASM_MIDAS)
#undef ASM_FAIL
#define ASM_FAIL 1			/* Default is FAIL (and MACRO) */
#endif

/* Define various preprocessor macros to help compensate for differences
** in the way the assemblers define THEIR macros.
*/
#if ASM_FAIL+ASM_MACRO
#define macdef0(name)		DEFINE name
#define macdef1(name,arg1)	DEFINE name(arg1)
#define macdef2(name,arg1,arg2)	DEFINE name(arg1,arg2)
#define macbeg <
#define macend >
#elif ASM_MIDAS
#define macdef0(name)		DEFINE name
#define macdef1(name,arg1)	DEFINE name arg1
#define macdef2(name,arg1,arg2)	DEFINE name arg1,arg2
#define macbeg
#define macend TERMIN
#endif

/* Define assembler macros to help compensate for CPU differences. */
#if CENV_DMOVX==0
macdef2(DMOVE, AC,MEM)
macbeg	MOVE	AC,MEM
	MOVE	AC+1,1+MEM
macend

macdef2(DMOVEM, AC,MEM)
macbeg	MOVEM	AC,MEM
	MOVEM	AC+1,1+MEM
macend
#endif /* No DMOVx */

#if CENV_ADJSP==0
macdef2(ADJSP, AC,CONST)
macbeg	ADD	AC,[<CONST,,CONST>+<CONST&<-1,,0>>]
macend
#endif /* No ADJSP */

/*
** Byte pointer operations.
**
** ADJBP AC,MEM
**    takes a byte count in AC and a byte pointer of any shape in MEM,
**    and leaves a byte pointer shifted over by the count in the AC.
**
** %CHRBP AC,MEM
**    backs up the byte pointer in MEM by one and leaves it in AC.
**    This is often used in the runtimes to produce an ILDB pointer
**    from an IBP pointer.  The result may not quite be the same as
**    corresponding AC -- it may be a (POINT n,X) pointer i.e. have
**    a P field of 44 -- so only do this once and don't index through
**    the result or compare it to another non-%CHRBP pointer.
*/

#if CENV_ADJBP
#if ASM_FAIL+ASM_MACRO
	OPDEF	ADJBP	[IBP]	/* ADJBP is same as IBP on KS/KL */
#endif
macdef2(%CHRBP, TOAC,FRMBP)	/* Turn LDB pointer into ILDB pointer: */
macbeg	SETO	TOAC,		/* Get -1 */
	ADJBP	TOAC,FRMBP	/* And do ADJBP */
macend

#else	/* No ADJBP */

macdef2(ADJBP, AC,MEM)		/* No ADJBP, simulate: */
macbeg	EXTERN	$ADJBP		/* Declare extern routine to simulate it */
	MOVE	16,MEM		/* Do this before EXCH.  Get byte pointer */
	EXCH	15,AC		/* Put count into expected reg, save AC15 */
	PUSHJ	17,$ADJBP	/* Call $ADJBP to simulate byte ptr add */
	EXCH	15,AC		/* restore AC15 and get adjusted bp in reg */
macend
macdef2(%CHRBP, TOAC,FRMBP)	/* Expand call to $ADJBP for special case: */
macbeg	MOVE	TOAC,FRMBP	/* Get byte pointer */
	ADD	TOAC,[110000,,0] /* Adjust assuming 9-bit bytes */
	TLNN	TOAC,007000	/* Was it 7-bit byte pointer? */
	 SUB	TOAC,[020000,,0] /* Yes, fix up for that case */
macend
#endif /* No ADJBP */

#if CENV_DFL_S		/* Software double-precision instructions */

/*
** KA-10 Double-Precision Floating Point (software format)
**
** Copied from:
**    DEC Assembly Language Handbook, 1st edition, 1972
**    PDP-10 System Reference Manual  DEC-10-HGAC-D
**	3rd printing August 1969
**	Page 2-67, "Programming Examples".
**
** NOTE!  These macros require 3 registers, not just 2!
** We can save things in AC16 and be sure that A+2 is free
** (KCC reserves AC16 as a temporary for just such purposes).
** We never use doublewords beyond 14/15 so A+2 always exists.
**
** Some of these sequences are less than efficient, but they
** have to be sufficiently general to cover all possible abuses
** (e.g. unexpected registers used as indices in addresses).
*/

macdef2(%DSAVE, A,M)		/* Set up A+2/free, M into -2(17)/-1(17): */
macbeg	MOVE	16,1+M		/* Get low word (push here would mess up M) */
	PUSH	17,M		/* Now safe to push high word */
	PUSH	17,16		/* Followed by low word (careful if A+2==16) */
	PUSH	17,A+2		/* Followed by AC to be freed */
macend
macdef0(%DREST)			/* Restore at end of double op after %DSAVE: */
macbeg	POP	17,A+2		/* Get saved register back */
	ADJSP	17,-2		/* Forget about pushed copy of doubleword */
macend
macdef2(DMOVN, A,M)		/* Get double from memory and negate: */
macbeg	DMOVE	A,M		/* Get double using previously defined macro */
	DFN	A,A+1		/* Do double register negate */
macend
macdef2(DMOVNM, A,M)		/* Save negated doubleword into memory: */
macbeg	DFN	A,A+1		/* Negate register doubleword */
	MOVEM	A,M		/* Save high part of negated word into mem */
	MOVEM	A+1,1+M		/* Save low part */
	DFN	A,A+1		/* Negate reg again to leave the way it was */
macend
macdef2(DFAD, A,M)		/* Add two doubles: */
macbeg	%DSAVE	A,M		/* Set up for doubleword op */
	UFA	A+1,-1(17)	/* Sum of low parts to A+2 */
	FADL	A,-2(17)	/* Sum of high parts to A,A+1 */
	UFA	A+1,A+2		/* Add low part of high sum to A+2 */
	FADL	A,A+2		/* Add low sum to high sum */
	%DREST			/* Fix up after doubleword op */
macend
macdef2(DFSB, A,M)		/* Subtract double in mem from one in acs: */
macbeg	DFN	A,A+1		/* Negate double length operand */
	DFAD	A,M		/* Perform normal add */
	DFN	A,A+1		/* -(M - AC) = AC - M */
macend
macdef2(DFMP, A,M)		/* Multiply two doubles: */
macbeg	%DSAVE	A,M		/* Set up for doubleword op */
	MOVEM	A,A+2		/* Copy high AC operand in A+2 */
	FMPR	A+2,-1(17)	/* High reg times low mem to A+2 */
	FMPR	A+1,-2(17)	/* Low reg times high mem to A+1 */
	UFA	A+1,A+2		/* Add cross products into A+2 */
	FMPL	A,-2(17)	/* High product to A,A+1 */
	UFA	A+1,A+2		/* Add low part to cross sum in A+2 */
	FADL	A,A+2		/* Add low sum to high part of product */
	%DREST			/* Fix up after doubleword op */
macend
macdef2(DFDV, A,M)		/* Divide double in acs by one in mem: */
macbeg	%DSAVE	A,M		/* Set up for complicated double op */
	FDVL	A,-2(17)	/* Get high part of quotient */
	MOVN	A+2,A		/* Copy negative of quotient in A+2 */
	FMPR	A+2,-1(17)	/* Multiply by low part of divisor */
	UFA	A+1,A+2		/* Add remainder */
	FDVR	A+2,-2(17)	/* Divide sum by high part of divisor */
	FADL	A,A+2		/* Add result to original quotient */
	%DREST			/* Fix up after doubleword op */
macend
#endif /* CENV_DFL_S */

/*
** Some more opcode definitions.
**
** ERJMP and ERJMPA are for TOPS-20 and TENEX sites only.
** ERJMPA should be used instead of ERJMP when the JSYS normally skips.
**
** XMOVEI, and IFIW are for KL-10B extended addressing.
** We might also define XHLLI here but I've yet to see a correct use.
*/

#if ASM_FAIL+ASM_MACRO
#if SYS_T20+SYS_10X
	IFNDEF	ERJMP,< OPDEF ERJMP [JUMP 16,] >
#if SYS_T20
	OPDEF	ERJMPA	[ERJMP]
#else
	OPDEF	ERJMPA	[JUMPA 16,]
#endif
#endif
	OPDEF	XMOVEI	[SETMI]
	DEFINE	IFIW	<SETZ >

#elif ASM_MIDAS

#if SYS_T20
	ERJMPA==<ERJMP>
#else
	ERJMPA==<JUMPA 16,>
#endif
	IFIW==<SETZ>
#endif

/*
** Data segmentation.
**
** KCC emits code in two sections: code and data.  This file sets things up
** so that code and data are in the high and low segments, which typically
** correspond to the high and low halves of their section but which can be
** rearranged by the appropriate LINK directives.
**
** Code and data are separated in the assembly output by the %%CODE and %%DATA
** macros.  %%CODE will never be called from any context other than the data
** segment, and vice versa.  KCC expects the header to initially be in code.
*/
#if ASM_FAIL+ASM_MACRO
	TWOSEG	400000	    	/* Set up for pure and impure segments */
	RELOC	0
	RELOC	400000		/* Begin in the pure segment */
	DEFINE %%CODE <RELOC>
	DEFINE %%DATA <RELOC>
#elif ASM_MIDAS
	.DECTWO 400000
	$$RLO==.
	.=.+400000	/* Go into code seg */
	$$RHI==.
DEFINE %%CODE
$$RLO==. ? .=$$RHI	/* Remember current lowseg loc, switch to hiseg */
TERMIN
DEFINE %%DATA
$$RHI==. ? .=$$RLO	/* Remember current hiseg loc, switch to lowseg */
TERMIN
#endif

/*
** Finally, we try to purge symbols which may conflict with user-defined syms.
** However, there is a bug in FAIL which causes SEARCH MONSYM to barf
** ("FAIL BUG IN SEARCH") if the SEARCH comes after the IF's are purged!
** But if we put the SEARCH here, before the PURGE, we are including it in ALL
** KCC output, which is not a good idea; there is more overhead, and
** otherwise undefined symbols might be unexpectedly satisfied from MONSYM!
** For the time being, since FAIL will output a .REL file anyway even after
** barfing about the SEARCH, we will continue doing this, and include a
** note in the doc to ignore those barfs, and hope someone fixes FAIL someday.
**
** Note that MACRO will not complain, and consequently MACRO is preferred
** when compiling modules with #asm code (such as the C library!)
*/
#if ASM_FAIL+ASM_MACRO
	PURGE	IFE,IFN,IFG,IFGE,IFL,IFLE,IFDEF,IFNDEF,IFIDN,IFDIF
#elif ASM_MIDAS
	EXPUNGE	IFE,IFN,IFG,IFGE,IFL,IFLE,IFDEF,IFNDEF,IFIDN,IFDIF
#endif

/* End of C-ASMH.C */

