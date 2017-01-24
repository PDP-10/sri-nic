REDIT 1(104) COMPARE by user CLYNN, 10-Feb-86 18:54:59
File 1: CWL:<DEC-6-1-BASE>APRSRV.MAC.1
File 2: CWL:<5-4-NET>APRSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
;[BBNF.ARPA]CWL:<5-4-NET>APRSRV.MAC.1, 24-Apr-85 19:27:22, Edit by CLYNN
; Move CONOPG and CASHF to RSDAT so symbol table isn't trashed at startup

***** CHANGE #2; PAGE 87, LINE 4; PAGE 87, LINE 4
   >				;END IFN SMFLG


;CACHE CONTROL

RS CASHF,1			;STATE OF THE CACHE (-1 FOR ON, 0 FOR OFF)
   IFN KLFLG,<
 ---------------------------------
   >				;END IFN SMFLG


;CACHE CONTROL

RSI CASHF,0			;STATE OF THE CACHE (-1 FOR ON, 0 FOR OFF)
   IFN KLFLG,<

