/*	title	sigsys - catch or ignore signals
	subttl	David Eppstein / Stanford University / 13-Aug-84
*/
#include "c-env.h"
#if SYS_T20+SYS_10X
	entry	signal,sigsys,kill;

#asm
	SEARCH	MONSYM

ilev==1		; Basic interrupt level
ilevx==1B5	; Ilev in right place for int vectoring.

; make initialization routine to set up pseudo-interrupt system

iint:	block	1		;space for linkage
	pushj	17,chnini	; Initialize channel table
	xmovei	1,lev1pc	;get pc storage
	movem	1,levtab+ilev-1	;for pc save on one level (the only one we use)
	tlne	1,-1
	 jrst iint2

	; Running extended, must use XSIR%
	movei	1,.fhslf	;on ourself
	movei	2,3		;arg block in acs
	movei	3,3		;three words in arg block
	xmovei	4,levtab	;level table
	xmovei	5,chntab	;and channel table
	xsir%			;set up interrupt system
	jrst iint3

	; Non-extended, use old RIR%
iint2:	movei	1,.fhslf	;on ourself
	move	2,[levtab,,chntab]
	rir%

iint3:	movei	1,.fhslf	;on ourself
	eir%			;enable interrupts
	movsi	1,.ticrf	;channel 0 is hang up
	ati%			;assign terminal interrupt
#if SYS_10X
	movei	1,.fhslf
	move	2,[1B<.ICILI>+1B<.ICEOF>+1B<.ICDAE>]
	aic%			; Always keep these on for 10X
#endif
	skipe	16,iint		;more inits?
	 jrst	1(16)		;yes
	popj	17,		;no


define chnset(x)
<	xmovei 1,x	; Get addr of int handler
	tlne 2,-1
	 tlo 1,(ilevx)	; If extended, we use "general" format
	tlnn 2,-1
	 hrli 1,ilev	; Else use "old" non-extended format
	movem 1,(2)
	addi 2,1
>

	; Initialize channel table.  Needed because we dont know which format
	; we will be using until runtime.
chnini:	xmovei 2,chntab

	chnset(hup)		;0 assigned to terminal hang up
	repeat 5,<chnset(unk)>	;1-5 assignable
	chnset(aov)		;6 .ICAOV arithmetic overflow
	chnset(fov)		;7 .ICFOV floating overflow
	chnset(unk)		;8 reserved for DEC
	chnset(pov)		;9 .ICPOV pushdown overflow
	chnset(eof)		;10 .ICEOF end of file condition
	chnset(dae)		;11 .ICDAE data error file condition
	repeat 3,<chnset(unk)>	;12-14 reserved for DEC
	chnset(ili)		;15 .ICILI illegal instruction
	chnset(ird)		;16 .ICIRD illegal memory read
	chnset(iwr)		;17 .ICIWR illegal memory write
	chnset(unk)		;18 reserved for DEC
	chnset(ift)		;19 .ICIFT inferior process termination
	chnset(mse)		;20 .ICMSE system resources exhausted
	chnset(unk)		;21 reserved for DEC
	chnset(nxp)		;22 .ICNXP nonexistent page reference
	repeat 13,<chnset(unk)>	;23-35 assignable
	popj 17,

	$$DATA
chntab:	block 44		; 36. words for channel table

levtab:	block	3		;three words in level table of return PC locs
lev1pc:	block	2		;the return pc from the interrupt
				; (1 word if single section, 2 if extended)
	$$CODE			;back to pure

; table of signal handlers

sigdfl==0			;default
sigign==1			;ignore
sigcat==2			;held signal caught
sighld==3			;hold signal

sigtbl:	repeat 25,<sigdfl>	;nothing there yet

intchn:	1B0			;SIGHUP, channel 0
	0			;SIGINT?
	0			;SIGQUIT?
	1B<.ICILI>		;SIGILL, illegal instruction
	0			;SIGTRAP?
	0			;SIGIOT?
	0			;SIGEMT?
	1B<.ICFOV>		;SIGFPE, floating point exception
	0			;SIGKILL, simulated with HALTF%/HFORK%
	0			;SIGBUS?
	1B<.ICNXP>		;SIGSEGV, segmentation violation
	0			;SIGSYS?
	0			;SIGPIPE?
	0			;SIGALRM?
	0			;SIGTERM?
	0			;16 unassigned
	0			;SIGSTOP?
	0			;SIGTSTP?
	0			;SIGCONT?
	1B<.ICIFT>		;SIGCHLD, child status has changed
	0			;SIGTTIN?
	0			;SIGTTOU?
	0			;SIGTINT?
	0			;SIGXCPU?
	0			;SIGXFSZ?

; entry routines

signal:
sigsys:	move	5,-1(17)	;get signal
	skipl	6,-2(17)	;and function
	 caile	6,^d25		;in range?
	  jrst	badsig		;no
	jumpe	6,drpsig	;SIG_DFL, deassigning
	exch	6,sigtbl-1(5)	;put new sig, get old
	movei	1,.fhslf	;get self
	move	2,intchn-1(5)	;and interrupt channel
	cain	6,sigcat	;was it an old caught one?
	 iic%			;yes, re-initiate
	cain	6,sigdfl	;not there at all before?
	 aic%			;yes, activate interrupt channel
	move	1,6		;return the old value
	popj	17,

badsig:	seto	1,		;unknown signal, return -1
	popj	17,

drpsig:	move	1,.fhslf	;dropping signal, on self
	move	2,intchn-1(5)	;with signal mask for channel
#if SYS_10X
	ANDCM 2,[1B<.ICILI>+1B<.ICEOF>+1B<.ICDAE>]
#endif
				; Never deactivate these on 10X.
	dic%			;deactivate interrupt channel
	setz	6,		;get nothing
	exch	6,sigtbl-1(5)	;exchange with what was there
	cain	6,sigcat	;caught?
	 iic%			;yes, unhold
	setz	1,		;return SIG_DFL as old value
	popj	17,

kill:	move	1,-1(17)	;get fork
	move	2,-2(17)	;signal
	cain	2,^d9		;SIGKILL?
	 jrst	kill0		;yes
	move	2,intchn-1(2)	;channel mask for it
	iic%			;initiate interrupt condition
	popj	17,		;done

kill0:	caie	1,.fhslf	;on self?
	 jrst	kill1		;no
	haltf%			;yes, just stop self
	popj	17,		;done

kill1:	hfork%			;stop inferior
	popj	17,		;done

; individual interrupt handlers

	$$DATA			;data section
acsave:	block	20		;place to save accumulators
	$$CODE			;code again

define	inthan(s) <
	movem	0,acsave	;;first save register zero
	move	0,[1,,acsave+1]	;;get BLT pointer
	blt	0,acsave+17	;;save rest of registers
	movei	5,^d<s>		;;get signal channel to use
	jrst	runint		;go run interrupt
>

unk:
aov:
pov:
ird:
iwr:
mse:
#if SYS_T20
eof:
dae:
#endif
	debrk%			;unknwn interrupt, merely return from it

hup:	inthan(1)		;SIGHUP
fov:	inthan(8)		;SIGFPE
nxp:	inthan(11)		;SIGSEGV
ift:	inthan(20)		;SIGCHLD
#if SYS_T20
ili: inthan(4)		; SIGILL
#endif

#if SYS_10X
	; On TENEX these can result from a failing JSYS.
eof:
dae:
ili:
	movem	0,acsave	;;first save register zero
	move	0,[1,,acsave+1]	;;get BLT pointer
	blt	0,acsave+17	;;save rest of registers
	jrst	int.il
#endif

#if SYS_10X
; This code lifted from MIDAS --KLH
A=1
B=2
INTPC1==LEV1PC

; Handle Illegal Instruction (normally a failing JSYS, bletch!)
; 10X ERJMP-handling interrupt routine.
;ERJMPA==:<JUMPA 16,>	; For use instead of ERJMP where JSYS normally skips.
;IFNDEF ERJMP,ERJMP==:<JUMP 16,>
;IFNDEF ERCAL,ERCAL==:<JUMP 17,>

ERXJMP==:<ERJMP_-27>	; For easier code writing
ERXCAL==:<ERCAL_-27>
ERXJPA==:<ERJMPA_-27>

INT.IL:	PUSH 17,A
	PUSH 17,B
	MOVE A,INTPC1		; Get PC we got interrupted from
	LDB B,[271500,,(A)]	; Get op-code and AC field of instr
	CAIN B,ERXJPA
	 JRST ERJFAK
	CAIE B,ERXJMP		; Is it a magic cookie?
	 CAIN B,ERXCAL
	  JRST ERJFAK
	AOJ A,
	LDB B,[271500,,(A)]	; Try next instr
	CAIE B,ERXJMP		; Any better luck?
	 CAIN B,ERXCAL
	  JRST ERJFAK
;	ETF [ASCIZ "Fatal interrupt encountered"]
	pop 17,b
	pop 17,a
	movei 5,4		; No ERJMPA/ERJMP/ERCAL so take real interrupt
	jrst runint

ERJFAK:	CAIN B,ERXCAL		; See which action to hack
	 JRST ERJFK2		; Go handle ERCAL, messy.
	MOVEI A,@(A)		; ERJMP, get the jump address desired
	MOVEM A,INTPC1		; Make it the new PC
	POP 17,B
	POP 17,A
	DEBRK%
ERJFK2:	MOVEI B,@(A)		; Get jump address
	MOVEM B,INTPC1		; Make it the new PC
	POP P,B
	AOJ A,			; old PC needs to be bumped for return
	EXCH A,(17)		; Restore old A, and save PC+1 on stack
	DEBRK%

; (Actually, since ERCAL is not special except after a JSYS, it would
; still work if the ERCAL-simulation didnt bump the PC; control would
; just drop through to the next instruction on return.  Might confuse
; people looking through the stack frames, though.)
#endif /* 10X */

; handler routine for interrupt
; call with 5/signal number
; returns with DEBRK% after restoring ACs

runint:	move	6,sigtbl-1(5)	;get handler
	cain	6,sighld	;holding?
	 jrst	hldsig		;yes
	caig	6,sighld	;real routine?
	 jrst	intret		;no
	movei	7,sigdfl	;assume default
	tlze	6,200000	;deferred?
	 movei	7,sighld	;yes
	push	17,[0]		;psl
	xmovei	1,. 	    	;get where we are
	tlnn	1,-1	    	;section zero?
	 push	17,lev1pc	;pc (non-extended)
	tlne	1,-1	    	;otherwise...
	 push	17,lev1pc+1	;pc (extended)
	push	17,[0]		;xx
	push	17,[0]		;param
	push	17,5		;signo
	pushj	17,(6)		;call the routine
	adjsp	17,-3		;drop some stuff
	xmovei	1,.
	tlnn	1,-1
	 pop	17,lev1pc	;fix return pc (non-extended)
	tlne	1,-1
	 pop	17,lev1pc+1	;fix return pc (extended)
	movem	7,0(17)		;new function
	push	17,5		;this signal
	pushj	17,sigsys	;reset it
	adjsp	17,-2		;fix stack
intret:	move	0,[acsave+1,,1]	;restoring reegistrs
	blt	0,17		;do it
	move	0,acsave	;get this reg back too
	debrk%			;return from interrupt

hldsig:	movei	6,sigcat	;caught signal
	movem	6,sigtbl-1(5)	;put in place
	jrst	intret		;return from interrupt

#endasm
#endif /* T20+10X */
