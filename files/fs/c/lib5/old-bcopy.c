/*
**	BCOPY - KCC "BSTRING" library routines BZERO, BCOPY, BCMP
**	(C) Copyright 1985 Ken Harrenstien / SRI International / 22-Apr-1985
**
** BZERO(cp1, cnt)	- Zero byte string
** BCOPY(from, to, cnt)	- Copy byte string
** BCMP(cp1, cp2, cnt)	- Compare byte strings
** char *cp1, *cp2, *from, *to;
** int cnt;
**
**	These routines serve the same function as the BSD library routines
**	of the same names.  They are written in assembler to take advantage
**	of special machine instructions such as BLT which greatly speed up
**	execution.
*/

#include "c-env.h"

#if CPU_PDP10
static void
tablestuff()
{
#asm

A=1	; KCC functions return value in this AC
B=2
C=3
D=C+1
E=D+1
T=E+1
TT=7
P=17

;;;;;;;;;;;;;;;;;; MIDAS test stuff
;fsklcv:	popj p,
;fscopy:
;fscpy:	ibp -2(p)
;	ibp -1(p)
;	jrst bcopy
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%brkev==17	; Use 15. chars as breakeven point.  This length or less
		; uses default byte-by-byte loop; more than this invokes
		; hairy optimization code.

; Assumption is that all code in PDP-10 C will never use any char ptrs
; for other than 7, 8, or 9 bytes.  We further assume that while
; running non-extended, only local-format BPs will be seen, and when
; running extended (in non-zero section), only global one-word BPs
; will be seen.  There are a few places where the code checks on this
; (when convenient) but recovery from abuse cannot always be relied
; on.  Note that the KL string instructions such as MOVSLJ are not
; generally used, first because they do not exist on KA/KI processors,
; and second because they are not much faster than a regular byte
; loop!

; Global-format P&S table
;	<Local LH>,,<#bytes/wd><00><offset>	; <Global P&S>
$$gbpt:	0		; 44 - really a local-fmt ptr!

	; 6-bit bytes: size 06	
	440600,,060000	; 45
	360600,,060000	; 46
	300600,,060005	; 47
	220600,,060004	; 50
	140600,,060003	; 51
	060600,,060002	; 52
	000600,,060001	; 53
	; 8-bit bytes (size 10)
	441000,,040000	; 54
	341000,,040000	; 55
	241000,,040003	; 56
	141000,,040002	; 57
	041000,,040001	; 60
	; 7-bit bytes (size 07)
	440700,,050000	; 61
	350700,,050000	; 62
	260700,,050004	; 63
	170700,,050003	; 64
	100700,,050002	; 65
	010700,,050001	; 66
	; 9-bit bytes (size 11)
	441100,,040000	; 67
	331100,,040000	; 70
	221100,,040003	; 71
	111100,,040002	; 72
	001100,,040001	; 73
	; 18-bit bytes (size 22)
	442200,,020000	; 74
	222200,,020000	; 75
	002200,,020001	; 76

	000000,,0	; 77	; Illegal
#endasm		/* End of table and declaration stuff */
}
#endif /* CPU_PDP10 */

/*************************  BZERO  *******************************/

void
bzero(array, n)
register char *array;	/* Address of byte array */
register int n;		/* number of bytes in array to zero */
{
#ifndef CPU_PDP10
	if(n > 0) do {*array++ = 0; } while(--n);

#else	/* CPU_PDP10 */
#asm
	xmovei a,0	; Get a zero (check for running extended)
	jumpn a,bzerox	; Jump if extended

	; Non-extended BZERO
	skipg d,-2(p)	; Get # of bytes
	 popj p,	; Count negative or zero
	skipn c,-1(p)	; Get char pointer (byte ptr)
	 popj p,	; Ignore if pointer zero.
;	setz a,
	caile d,%brkev	; If more bytes than breakeven,
	 jrst bzer30	; go do special check routine.

bzero1:	dpb a,c		; Set first byte zero
	sojle d,bzero9	; Then decrement count and fall into loop.
	idpb a,c
	sojg d,.-1
bzero9:	popj p,

	; Assume local-format byte pointer.  Zero bytes until at beg
	; of word.
bzer30:	dpb a,c
	sojle d,bzero9
bzero4:	idpb a,c
	tlne c,720000	; Stop when P=0 or 4 or 1.
	 sojg d,bzero4
	sojl d,bzero9

	; (c)+1 now points to words to clear.  Must find how many
	; words, plus remaining # bytes, to clear.  Requires determining
	; byte size, ugh.  Assumes one of 7,8,9.
	tlnn c,7000
	 jrst [	idivi d,5
		jrst bzero5]
	lshc d,-2	; Divide by 4
	lsh e,-<44-2>	; Get remainder right-justified.
bzero5:	setzm 1(c)	; Clear 1st word
	movsi b,1(c)	; <from>,,
	hrri b,2(c)	;	,,<to>
	addi c,(d)	; Get addr of last word (BP points to end)
	blt b,(c)	; Zero the words
	jumple e,bzero9	; Take care of any remaining bytes.
	idpb a,c
	sojg e,.-1
	popj p,

	; Running in non-zero section, so assume BP is global one-wd format.
bzerox:	skipg d,-2(p)	; Get # of bytes
	 popj p,	; Count negative or zero
	skipl c,-1(p)	; Get char pointer (byte ptr)
	 popj p,	; Ignore if pointer zero or non-global.
	setz a,
	caig d,%brkev	; More bytes to zero than breakeven?
	 jrst bzero1	; No, just zap them trivially.

	; Global format BP.
	ldb b,[360600,,c]	; Get the 6-bit P&S field
	skipg e,$$gbpt-44(b)	; Get table entry for it
	 jrst [	ibp c		; Odd - ensure points to 1st byte.
		jumpl c,.+1	; If not still global fmt, was a 44 local!
		jrst bzer30]	; Try to hack it as local-fmt ptr.
	andi e,77		; Get # bytes of roundup needed.
	jumple e,bzerx4(e)	; If 0, skip rounding.
	subi d,(e)		; Subtract # of bytes we will clear here
	dpb a,c
	sojle e,bzerx3
	idpb a,c
	sojg e,.-1

bzerx3:	ibp c		; -1 adjust BP then treat like 0
bzerx4:			; 0 BP is word-aligned.
	; C/ BP pointing to 1st byte in word
	; D/ # bytes left to clear
	ldb e,[140600,,$$gbpt-44(b)]	; Get # bytes per word
	idivi d,(e)		; Get # words to zap, remaining # bytes in e.
	jumple d,[skiple d,e
		  jrst bzero1	; If more bytes to zero, use byte loop.
		popj p,]
	move b,e		; Save remainder
	move e,c		; Get "source" BP
	tlz e,770000		; Clear the P&S field
	xmovei t,1(e)		; Get "dest" addr (1 greater)
	setzm (e)		; Zap 1st word
	sosle d
xblt==<020000,,0>
	 extend d,[xblt]	; Do extended BLT
	skipg d,b		; Now check for remaining bytes
	 popj p,		; None, all done.
	dpb t,[003600,,c]	; Put final dest addr +1 back in BP
	jrst bzero1		; More bytes to zero, use byte loop.
#endasm
#endif	/* CPU_PDP10 */
}

/************************  BCOPY  *********************************/

void
bcopy(from, to, n)
register char *from, *to;
register int n;			/* number of bytes to copy */
{
#ifndef CPU_PDP10
	if(n > 0) do { *to++ = *from++; } while(--n);

#else	/* CPU_PDP10 */
#asm
	; General AC usage:
	; a,b/ scratch
	; c/ # bytes
	; d/ # remainder
	; e/ Source BP
	; t/ Dest BP
BCOPY:	skipg c,-3(p)	; Get 3rd arg, # of bytes
	 popj p,
	caig c,%brkev	; Ensure worthwhile to do hairy stuff.
	 jrst bcpy40	; Naw, just do trivially.
	skipge e,-1(p)	; Get 1st arg, source BP
	 jrst bcpy50	; Sign bit set, assume global ptr.
	skipg t,-2(p)	; Assume local fmt, get dest BP
	 jrst bcpy42	; Not same fmt, for now default to trivial case.

	; Both BPs are local format; make final check for sameness
bcpy10:	move a,e
	xor a,t			; XOR the two byte pointers
	tlne a,-1		; Check for same LH
	 jrst [	tlnn a,007700	; Nope - check for same size...
		 tlnn a,770000	; Size same, check for alignment
		  jrst bcpy42	; Must use byte loop; size different, or
				;  using indexing/indirection (ugh!)
		jrst bcpy60]	; Same size, different alignment, go do hair.


	; Local-format BPs have same size and alignment, so hack
	; BLT word-move optimization!
	caml e,[331100,,0]	; Must be 331100, 341000, or 350700.
	 jrst bcpy33		; Points to beg of word!

	; Doesnt point to beg of word, must get things aligned.
	ldb a,e
	dpb a,t
	jrst bcpy32
bcpy31:	ildb a,e
	idpb a,t
bcpy32:	tlne e,720000	; Stop when P=00 or 04 or 01.
	 sojg c,bcpy31
	sojle c,bcpy90
	ibp e		; Then point to beg of next wd.
	ibp t

	; (E) and (T) now point to word locs for moving (from, to).
	; Find # words, plus remaining # bytes, to move.  Requires determining
	; byte size, ugh.  Assumes one of 7,8,9.
bcpy33:	tlnn t,7000	; Skip if 4 bytes per word (S=10 or 11, vs 07)
	 jrst [	idivi c,5
		jrst bcpy34]
	lshc c,-2	; Divide by 4
	lsh d,-<44-2>	; Get remainder right-justified.

	; c/ # wds
	; d/ # bytes remainder
bcpy34:	jumple c,bcpy35
	movsi b,(e)	; <from>,,
	hrri b,(t)	;	,,<to>
	addi t,(c)	; Get addr of last word + 1 (BP points to end)
	blt b,-1(t)	; Copy the words
bcpy35:	caig d,0	; Take care of any remaining bytes
	 popj p,
	addi e,(c)	; Sigh, must also point to last source word.
	move c,d
	jrst bcpy42	; Go copy the remainder by hand (start with LDB)

	; Trivial case.  Strings which dont have same alignment or size
	; all come here.
	; Note that the KL extended instruction MOVSLJ is
	; slightly faster than this byte loop, but tis painful to
	; conditionalize this.  MOVSLJ is done for global BPs however
	; since then we know the instruction will exist.
bcpy40:	skipe e,-1(p)		; Source BP
	 skipn t,-2(p)	; Dest BP
	  popj p,
	jumpl t,bcpy45		; If using global-format BPs, use MOVSLJ.
bcpy42:	ldb a,e			; Get byte
	dpb a,t			; Store it
	sojle c,bcpy90
bcpy43:	ildb a,e
	idpb a,t
	sojg c,bcpy43
bcpy90:	popj p,

	; Trivial case using MOVSLJ.
	; First need to get BPs into canonical shape.
bcpy45:	ldb a,e		; Get byte
	dpb a,t		; Store it
	sojle c,bcpy90
	move a,c	; AC+0: source string length
	move b,e	; AC+1,2: source byte ptr
	move d,c	; AC+3: dest string length
	move e,t	; AC+4,5: dest byte ptr
MOVSLJ==:<016000,,0>	; Subinstr of EXTEND
	EXTEND A,[MOVSLJ]	; DO IT!
	 trn
	popj p,

;;;;;;;;;;;;;;;;;;;;;;;   BCOPY (cont)   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; Global-format BP hacking.  Already have source BP in e.
bcpy50:	skipl t,-2(p)	; Get dest BP
	 jrst bcpy42	; Not same format, handle as trivial slow case.

	; c/ # bytes
	; e/ Source BP
	; t/ Dest BP
	ldb b,[360600,,e]	; Get P&S field for source
	ldb d,[360600,,t]	; Ditto for dest
	caie b,(d)
	 jrst [	move a,$$gbpt-44(b)	; Different size or alignment.  Check.
		xor a,$$gbpt-44(d)
		tlne a,007700		; See if have same byte size
		 jrst bcpy45		; No, must hack like trivial case.
		jrst bcpy56]		; Same size, do word-move hair.

	; P&S field is identical, so BPs are word-aligned and we can do
	; fast BLT!
	skipg d,$$gbpt-44(b)	; Get table entry for it
	 jrst [	ibp e		; Hmm, ensure points to 1st byte.
		ibp t
		jumpl e,bcpy55	; Ensure still global-fmt ptrs...
		jrst bcpy10]	; Ugh, must have been local-fmt with P=44!
	andi d,77		; Get # bytes of roundup needed.
	jumple d,bcpy55		; If 0, skip rounding.
	subi c,(d)		; Subtract # of bytes we will copy here
	ldb a,e
	dpb a,t
	sojle d,bcpy54
bcpy53:	ildb a,e
	idpb a,t
	sojg d,bcpy53
bcpy54:	ibp e
	ibp t
	ldb b,[360600,,t]	; Get P&S field again

       	; BPs are now word-aligned (point to 1st byte in word).
bcpy55:	ldb d,[140600,,$$gbpt-44(b)]	; Get # bytes per word
	idivi c,(d)		; Get # words (remainder in d)
	exch c,d
	jumple d,[jumpg c,bcpy42
		popj p,]

	; Set up to transfer at least 1 word.
	tlz e,770000		; Must flush P&S field from BPs
	tlz t,770000
	EXTEND D,[XBLT]		; Do it!
	jumple c,bcpy90		; If no remaining bytes, done.
	rot b,-6		; Must put P&S field back (alignment is OK)
	ior e,b
	ior t,b
	jrst bcpy42		; Then finish off with byte-loop copy.

	; Handle word-move hair for global-format BPs which have same
	; byte size but different alignment.
bcpy56:	cail d,70		; Only hack 9-bit bytes for now.
	 caile d,73
	  jrst bcpy45

	; Must get dest ptr to beg of word
bcpy57:	tlnn t,070000		; Check for value 70
	 jrst bcpy58
	ldb a,e
	dpb a,t
	ibp e
	ibp t
	sojg c,bcpy57
	popj p,

bcpy58:	ldb tt,[360600,,e]	; Save current source-BP P&S
	tlz e,770000	; Flush P&S so can do indexing
	tlz t,770000
	lshc c,-2	; Divide by 2
	lsh d,-<44-2>	; Get remainder in D
	move a,(e)	; Get 1st source word
	aoja e,@<.+1-70>(tt)	; Dispatch on P of 70-73
	setz bcpx90	; P=70=33, beg of word (shouldnt happen)
	setz bcpx91	; P=71=22, 3 bytes needed from source wd
	setz bcpx92	; P=72=11, 2 bytes needed from source wd
	setz bcpx93	; P=73=00, 1 byte needed from source wd

bcpy59:	skipg c,d
	 popj p,
	ior t,[700000,,0]	; Set dest P=70=33, beg of word
	rot tt,-6
	ior e,tt		; Set source P to original val
	soja e,bcpy42

bcpx90:	popj p,

	; Source P=71=22, 3 valid bytes in A
bcpx91:	move b,(e)
	lshc a,<11*1>
	movem a,(t)
	lshc a,<11*<4-1>>
	addi e,1		; Sigh... for extended addrs
	addi t,1
	sojg c,bcpx91
	jrst bcpy59

	; Source P=11, 2 valid bytes in A
bcpx92:	move b,(e)
	lshc a,<11*2>
	movem a,(t)
	lshc a,<11*<4-2>>
	addi e,1
	addi t,1
	sojg c,bcpx92
	jrst bcpy59

	; Source P=00, 1 valid byte in A
bcpx93:	move b,(e)
	lshc a,<11*3>
	movem a,(t)
	lshc a,<11*<4-3>>
	addi e,1
	addi t,1
	sojg c,bcpx93
	jrst bcpy59

;;;;;;;;;;;;;;;;;;;;;;;   BCOPY (cont)   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Special hair to handle case of byte strings which are same size
; but do not have same alignment (so BLT cannot be done).  We can still
; save a considerable amount of time by using word moves and shifts, which
; are 3 to 5 times faster than the alternatives (timed on a 2060):
;		Hairy stuff: .7-.8 usec/char
;		ILDB/IDPB:  3.5    usec/char
;		MOVSLJ:     2.1    usec/char

	; C/ count
	; E/ source BP (local fmt)
	; T/ Dest BP (local fmt)
	; First order of business is getting dest ptr word-aligned.
bcpy60:	caml t,[331100,,0]
	 jrst bcpy63	; Already word-aligned.
	ldb a,e
	dpb a,t
	jrst bcpy62
bcpy61:	ildb a,e
	idpb a,t
bcpy62:	tlne t,720000	; Stop when P=00 or 04 or 01.
	 sojg c,bcpy61
	sojle c,bcpy90
	ibp e
	ibp t

	; Dest is now word-aligned.  Further steps depend on byte size.
bcpy63:	tlne t,040000		; See if 9-bit bytes or not (P=33)
	 jrst [	tlne t,010000	; See if P=35 (vs =34)
		 jrst bcpy70	; Could be 35, assume 7-bit bytes.
		jrst bcpy80]	; Could be 34, assume 8-bit bytes.

	; Copy unaligned 9-bit bytes!
	lshc c,-2		; Divide by 4
	lsh d,-<44-2>		; Get 2 bits of remainder right-justified.
	ldb b,[360200,,e]	; Get low 2 bits of P from source BP
	move a,(e)		; Get 1st source word set up.
	aoja e,@.+1(b)		; Dispatch on value of 0,1,2,3.
	; Note sign bit (SETZ) must be set to prevent extended-format
	; indirection when in non-zero section.  Barf.
	setz bcpy66	; 0 = P=00, 1 byte needed from source wd
	setz bcpy65	; 1 = P=11, 2 bytes needed from source wd
	setz bcpy64	; 2 = P=22, 3 bytes needed from source wd
	setz bcpy34	; 3 = P=33, 4 bytes?  Is word-aligned, shouldnt happen
			;	but just in case, go hack BLT.

	; Source P=22, 3 valid bytes in A
bcpy64:	move b,(e)
	lshc a,<11*1>
	movem a,(t)
	lshc a,<11*<4-1>>
	addi e,1		; Sigh... for extended addr possibility.
	addi t,1
	sojg c,bcpy64
	skipg c,d
	 popj p,
	soja e,bcpy42

	; Source P=11, 2 valid bytes in A
bcpy65:	move b,(e)
	lshc a,<11*2>
	movem a,(t)
	lshc a,<11*<4-2>>
	addi e,1
	addi t,1
	sojg c,bcpy65
	skipg c,d
	 popj p,
	soja e,bcpy42

	; Source P=00, 1 valid byte in A
bcpy66:	move b,(e)
	lshc a,<11*3>
	movem a,(t)
	lshc a,<11*<4-3>>
	addi e,1
	addi t,1
	sojg c,bcpy66
	skipg c,d
	 popj p,
	soja e,bcpy42

; Copy non-aligned 7-bit bytes.
bcpy70:	idivi c,5		; Divide by 5, get rem in D
	ldb b,[360200,,e]	; Get low 2 bits of P from source BP
	move a,(e)		; Get 1st source word set up.
	lsh a,-1
	aoja e,@.+1(b)		; Dispatch on value of 0,1,2,3.
	; See note for previous table re SETZs.
	setz bcpy76	; 0 = P=10, 2 bytes needed from source wd
	setz bcpy78	; 1 = P=01, 1 bytes needed from source wd
	setz bcpy72	; 2 = P=26, 4 bytes needed from source wd
	setz bcpy74	; 3 = P=17, 3 bytes needed from source wd

	; Source P=26 (4 valid bytes in A)
bcpy72:	move b,(e)
	lshc a,<7*1>
	lsh a,1
	movem a,(t)
	lshc a,<7*<5-1>>
	addi e,1
	addi t,1
	sojg c,bcpy72
	skipg c,d
	 popj p,
	soja e,bcpy42

	; Source P=17 (3 valid bytes in A)
bcpy74:	move b,(e)
	lshc a,<7*2>
	lsh a,1
	movem a,(t)
	lshc a,<7*<5-2>>
	addi e,1
	addi t,1
	sojg c,bcpy74
	skipg c,d
	 popj p,
	soja e,bcpy42

	; Source P=10 (2 valid bytes in A)
bcpy76:	move b,(e)
	lshc a,<7*3>
	lsh a,1
	movem a,(t)
	lshc a,<7*<5-3>>
	addi e,1
	addi t,1
	sojg c,bcpy76
	skipg c,d
	 popj p,
	soja e,bcpy42

	; Source P=01 (1 valid byte in A)
bcpy78:	move b,(e)
	lshc a,<7*4>
	lsh a,1
	movem a,(t)
	lshc a,<7*<5-4>>
	addi e,1
	addi t,1
	sojg c,bcpy78
	skipg c,d
	 popj p,
	soja e,bcpy42

; Copy non-aligned 8-bit bytes.
bcpy80:	jrst bcpy42		; Also punt for now.

#endasm
#endif	/* CPU_PDP10 */
}	/* End of BCOPY */

/****************************  BCMP  ************************************
**
**	BCMP as defined by BSD returns 0 if equal, non-0 otherwise.
**	This version returns the difference of the 1st non-matching char
**	and can thus be used for < or > compares.
*/

int
bcmp(array1, array2, n)
register char *array1, *array2;
register int n;
{
#ifndef CPU_PDP10
	register int res;
	if(n > 0)
		do { if(res = (*array1++ - *array2++))
				return(res);
		  } while(--n);
	return(0);

#else	/* CPU_PDP10 */
#asm
	skipg e,-3(p)	; Get 3rd arg, # of bytes
	 jrst [	setz a,
		popj p,]
	move c,-2(p)
	move d,-1(p)
	ldb a,c
	ldb b,d
	came a,b
	 jrst bcmp9
	sojle e,bcmp9
bcmp5:	ildb a,c
	ildb b,d
	camn a,b
	 sojg e,bcmp5
bcmp9:	sub a,b		; Return difference byte1-byte2.
	popj p,
#endasm
#endif	/* CPU_PDP10 */
}	/* End of BCMP */
