REDIT 1(104) COMPARE by user CLYNN, 10-Feb-86 18:59:45
File 1: CWL:<DEC-6-1-BASE.AP7>POSTLD.MAC.1
File 2: CWL:<5-4-NET>POSTLD.MAC.1
***** CHANGE #1; PAGE 7, LINE 11; PAGE 7, LINE 11
	MOVE 2,[JRST EVGO]	;MAIN START LOCATION
	HRRM 2,.JBSA
	SEVEC
	; ..


;DUMP BUG STRINGS TO FILE, SETUP BUGTABLE IN SWPMON
 ---------------------------------
	MOVE 2,[JRST EVGO]	;MAIN START LOCATION
	HRRM 2,.JBSA
	SEVEC
	; ..



;Sort network symbol table, STSTAB, and parallel count, STATCT, and
;pointer, STATPT, tables.  First & last entries are + & - infinity.

STSORT:	HLLZ T4,STSTAB-2		; -Length, including + & - infinity
	ADD T4,[3,,1]			; Ends ok, start with second entry
	IFL. T4
	  DO.				; Find next smallest value
	    MOVE T3,T4			; Begin inner loop from
	    ADDI T3,1			; Next entry
	    MOVE T2,STSTAB(T4)		; Get current minimum
	    DO.
	      CAMG T2,STSTAB(T3)	; Current entry out of order?
	      IFSKP.
	        EXCH T2,STSTAB(T3)	; Yes, exchange
	        MOVE T1,STATCT(T4)	; Also exchange corresponding
	        EXCH T1,STATCT(T3)	; Entries in count table
	        MOVEM T1,STATCT(T4)
	        MOVE T1,STATPT(T4)	; And entries in pointer table
	        EXCH T1,STATPT(T3)
	        MOVEM T1,STATPT(T4)
	      ENDIF.
	      AOBJN T3,TOP.		; Back for rest of inner loop
	    ENDDO.
	    MOVEM T2,STSTAB(T4)		; Save minimum value
	  AOBJN T4,TOP.			; Back for rest of outer loop
	  ENDDO.
; Now look for duplicates
	  MOVX T1,.PRIOU
	  SETZ T3,
	  MOVSI T4,<-<STATCT-STSTAB>+1>	; Loop over all but 1 entry
	  DO.
	    MOVE T2,STSTAB(T4)		; Get entry
	    CAMGE T2,STSTAB+1(T4)	; Any duplicates?
	    IFSKP.
	      HRROI T2,[ASCIZ /% Duplicate name in network symbol table "/]
	      SOUT%
	      PUSH P,STSTAB(T4)
	      PUSH P,T3
	      HRROI T2,-1(P)
	      SOUT%
	      ADJSP P,-2
	      HRROI T2,[ASCIZ /".
/]
	      SOUT%
	    ENDIF.
	  AOBJN T4,TOP.			; Check whole table
	  ENDDO.
	ENDIF.

;DUMP BUG STRINGS TO FILE, SETUP BUGTABLE IN SWPMON

