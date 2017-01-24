	title	ccb
	search	monsym
	entry	fork

P==17
;
;  ************************************************************
;	fork(program_name,command_block,start_offset);
;
;	create inferior fork, runs a program on it, and
;	  wait for the program to terminate.
;
;	command_block is the argument block for the
;	  inferior process,
;	entry_offset is an offset that can be added to the
;	  usual starting location.
;  ************************************************************
;
fork:	movsi	1,(gj%old+gj%sht)
	seto	2,
	adjbp	2,-1(P)			; get pointer to program name
	gtjfn%				; get JFN of program file
	erjmp	error			; cannot open exe file
	hrrzm	1,jfn			; save JFN

	hrli	1,200000
	cfork%				; create inferior with same capability
	erjmp	error			; cannot create fork
	movem	1,frk			; save fork handle
	
	hrl	1,frk			; fork handle
	hrr	1,jfn			; JFN of program file
	get%				; load program image into inferior fork
	erjmp	error

	hrrz	1,frk
	gevec%				; get entry vector
	erjmp	error			; oops??
	add	2,-3(P)			; get offset
	hrrz	2,2
	movem	2,pc

	hrli	1,2			; set argument
	hrr	1,frk			; fork handle
	move	2,-2(P)			; address of command block
	movei	3,40			; length of block
	prarg%
	erjmp	error
	
	hrrz	1,frk
	move	2,pc
	sfork%				; start fork
	wfork%				; wait for fork to terminate
	kfork%				; then kill fork

ok:	movei	0,1			; non-error return
	popj	P,

error:	setz	0,			; error return
	popj	P,

jfn:	0				; program file JFN
frk:	0				; fork handle
pc:	0				; starting pc of process
arg:	1
	20
	40
	block	100			; process argument block

	end
