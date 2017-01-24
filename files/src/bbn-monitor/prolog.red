REDIT 1(104) COMPARE by user CLYNN, 10-Feb-86 18:59:47
File 1: CWL:<DEC-6-1-BASE>PROLOG.MAC.1
File 2: CWL:<5-4-NET>PROLOG.MAC.2
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Removed KNIN

***** CHANGE #2; PAGE 46, LINE 32; PAGE 46, LINE 32
TT.MCB==1			;ON THE MCB NETWORK
TT.PTY==2			;PSEUDO-TERMINAL
TT.D10==3			;ON THE DC10
TT.NVT==4			;ON THE ARPANET
TT.DZ==5			;ON THE SM10
TT.TVT==6			;TCP VIRTUAL TERMINALS
NLTYPS==7			;NUMBER OF TYPES

 ---------------------------------
TT.MCB==1			;ON THE MCB NETWORK
TT.PTY==2			;PSEUDO-TERMINAL
TT.D10==3			;ON THE DC10
TT.NVT==4			;ON THE ARPANET
TT.DZ==5			;ON THE SM10
TT.TVT==6			;TCP VIRTUAL TERMINALS
;NLTYPS==7			;NUMBER OF TYPES
TT.CVT==7			;CHAOSNET VIRTUAL TERMINALS
NLTYPS==10			;NUMBER OF TYPES


***** CHANGE #3; PAGE 57, LINE 7; PAGE 57, LINE 7


;DECNET 36 Stuff

FTP4R==:0			;No DECNET Phase 4 stuff
FTRTST==:0			;No Router test stuff
KNIN==:1			;Number of KLNIs supported

 ---------------------------------


;DECNET 36 Stuff

FTP4R==:0			;No DECNET Phase 4 stuff
FTRTST==:0			;No Router test stuff
;;[BBN] KNIN shouldn't be defined here
;;KNIN==:1			;Number of KLNIs supported


