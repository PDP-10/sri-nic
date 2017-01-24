/*
** C-HDR.C   - Assembler header file
**
** This file is run through KCC's preprocessor using the -E switch
** in order to generate the standard C-HDR.FAI file which KCC inserts
** automatically, without parsing, at the beginning of all its assembler
** output files (just after any ENTRY or TITLE statements).
**
** Any independent assembler modules intended for use with KCC programs
** should also .INSERT C-HDR.FAI; however, such modules are strongly
** discouraged in favor of using #asm.
**
** Keep all comments herein in slash-star form (rather than semicolons)
** so FAIL and MACRO don't have to look at them again.
*/

#include "c-env.h"		/* Insert our configuration parameters! */

/* Define various macros to help compensate for cpu/sys differences. */
#if CENV_DMOVX==0
DEFINE DMOVE(AC,MEM)
<	MOVE	AC,MEM
	MOVE	AC+1,1+MEM
>
DEFINE DMOVEM(AC,MEM)
<	MOVEM	AC,MEM
	MOVEM	AC+1,1+MEM
>
#endif /* No DMOVx */

#if CENV_ADJSP==0
DEFINE ADJSP(AC,CONST)
<	ADD	AC,[<CONST,,CONST>+<CONST&<-1,,0>>]
>
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
	OPDEF	ADJBP	[IBP]	/* ADJBP is same as IBP on KS/KL */
DEFINE %CHRBP(TOAC,FRMBP)	/* Turn LDB pointer into ILDB pointer: */
<	SETO	TOAC,		/* Get -1 */
	ADJBP	TOAC,FRMBP	/* And do ADJBP */
>
#else	/* No ADJBP */
DEFINE ADJBP(AC,MEM)		/* No ADJBP, simulate: */
<	EXTERN	$ADJBP		/* Declare extern routine to simulate it */
	MOVE	16,MEM		/* Do this before EXCH.  Get byte pointer */
	EXCH	15,AC		/* Put count into expected reg, save AC15 */
	PUSHJ	17,$ADJBP	/* Call $ADJBP to simulate byte ptr add */
	EXCH	15,AC		/* restore AC15 and get adjusted bp in reg */
>
DEFINE %CHRBP(TOAC,FRMBP)	/* Expand call to $ADJBP for special case: */
<	MOVE	TOAC,FRMBP	/* Get byte pointer */
	ADD	TOAC,[110000,,0] /* Adjust assuming 9-bit bytes */
	TLNN	TOAC,007000	/* Was it 7-bit byte pointer? */
	 SUB	TOAC,[020000,,0] /* Yes, fix up for that case */
>
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

DEFINE %DSAVE(A,M)		/* Set up A+2/free, M into -2(17)/-1(17): */
<	MOVE	16,1+M		/* Get low word (push here would mess up M) */
	PUSH	17,M		/* Now safe to push high word */
	PUSH	17,16		/* Followed by low word (careful if A+2==16) */
	PUSH	17,A+2		/* Followed by AC to be freed */
>
DEFINE %DREST			/* Restore at end of double op after %DSAVE: */
<	POP	17,A+2		/* Get saved register back */
	ADJSP	17,-2		/* Forget about pushed copy of doubleword */
>
DEFINE DMOVN(A,M)		/* Get double from memory and negate: */
<	DMOVE	A,M		/* Get double using previously defined macro */
	DFN	A,A+1		/* Do double register negate */
>
DEFINE DMOVNM(A,M)		/* Save negated doubleword into memory: */
<	DFN	A,A+1		/* Negate register doubleword */
	MOVEM	A,M		/* Save high part of negated word into mem */
	MOVEM	A+1,1+M		/* Save low part */
	DFN	A,A+1		/* Negate reg again to leave the way it was */
>
DEFINE DFAD(A,M)		/* Add two doubles: */
<	%DSAVE	A,M		/* Set up for doubleword op */
	UFA	A+1,-1(17)	/* Sum of low parts to A+2 */
	FADL	A,-2(17)	/* Sum of high parts to A,A+1 */
	UFA	A+1,A+2		/* Add low part of high sum to A+2 */
	FADL	A,A+2		/* Add low sum to high sum */
	%DREST			/* Fix up after doubleword op */
>
DEFINE DFSB(A,M)		/* Subtract double in mem from one in acs: */
<	DFN	A,A+1		/* Negate double length operand */
	DFAD	A,M		/* Perform normal add */
	DFN	A,A+1		/* -(M - AC) = AC - M */
>
DEFINE DFMP(A,M)		/* Multiply two doubles: */
<	%DSAVE	A,M		/* Set up for doubleword op */
	MOVEM	A,A+2		/* Copy high AC operand in A+2 */
	FMPR	A+2,-1(17)	/* High reg times low mem to A+2 */
	FMPR	A+1,-2(17)	/* Low reg times high mem to A+1 */
	UFA	A+1,A+2		/* Add cross products into A+2 */
	FMPL	A,-2(17)	/* High product to A,A+1 */
	UFA	A+1,A+2		/* Add low part to cross sum in A+2 */
	FADL	A,A+2		/* Add low sum to high part of product */
	%DREST			/* Fix up after doubleword op */
>
DEFINE DFDV(A,M)		/* Divide double in acs by one in mem: */
<	%DSAVE	A,M		/* Set up for complicated double op */
	FDVL	A,-2(17)	/* Get high part of quotient */
	MOVN	A+2,A		/* Copy negative of quotient in A+2 */
	FMPR	A+2,-1(17)	/* Multiply by low part of divisor */
	UFA	A+1,A+2		/* Add remainder */
	FDVR	A+2,-2(17)	/* Divide sum by high part of divisor */
	FADL	A,A+2		/* Add result to original quotient */
	%DREST			/* Fix up after doubleword op */
>
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

/*
** Cause LINK to include C runtime library.
**
** This is found on TOPS-20 sites in C:CLIB.REL; anywhere else
** its location is less standard.
*/

#ifdef CLIB_NAME
	.REQUEST CLIB_NAME
#else	/* Name not explicitly specified */
#if SYS_ITS
	.REQUEST C;CLIB
#else
	.REQUEST C:CLIB
#endif
#endif

/*
** Data segmentation.
**
** KCC emits code in two sections: code and data.  This file sets things up
** so that code and data are in the high and low segments, which typically
** correspond to the high and low halves of their section but which can be
** rearranged by the appropriate LINK directives.
**
** Code and data are separated in the assembly output by the $CODE and $DATA
** macros.  $CODE will never be called from any context other than the data
** segment, and vice versa.  KCC expects C-HDR.FAI to initially be in code.
*/
	TWOSEG	400000	    	/* Set up for pure and impure segments */
	RELOC	0
	RELOC	400000		/* Begin in the pure segment */

	DEFINE	$$CODE	<RELOC>
	DEFINE	$$DATA	<RELOC>

/*
** Declaration of external symbols.
**
** $START is the entry point for the KCC runtimes.  It will be
** used in the entry vector set up  if main() is defined.
**
** $ZERO is a doubleword zero literal, used for double normalization.
**
** The other symbols return from the subroutine after setting
** the return value to some constant (0, 1, or -1).  They are
** currently only used for assembly inclusions, but might someday
** also be used in generated code.
**
** This stuff is IFNDEF'd solely to prevent assembler complaints when
** MACRO assembles the CRT.C library module, which is where these labels
** are specified in ENTRY statements.  There is no way to muzzle FAIL.
*/
IFNDEF $START,<		/* Skip this if "ENTRY $START" already seen */
	EXTERN	$START
	EXTERN	$RET,$RETZ,$RETN,$RETP,$RETF,$RETT
	EXTERN	$ZERO
>

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
** when compiling the C library.
*/
	PURGE	IFE,IFN,IFG,IFGE,IFL,IFLE,IFDEF,IFNDEF,IFIDN,IFDIF

/* End of C-HDR.C */
