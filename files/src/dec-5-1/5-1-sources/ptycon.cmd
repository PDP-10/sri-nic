!COMMAND FILE FOR OBTAINING ALL FILES RELEVANT TO BUILDING PTYCON
INFORMATION LOGICAL-NAMES

;Files required to build product
COPY FROM-SOURCE:PTYCON.CMD TO-SOURCE:*.*.-1
COPY FROM-SOURCE:PTYCON.CTL TO-SOURCE:*.*.-1
COPY FROM-SOURCE:PTYCON.MAC TO-SOURCE:*.*.-1

;Documentation for product
COPY FROM-SOURCE:PTYCON.TCO TO-DOC:*.*.-1
COPY FROM-SOURCE:PTYCON.HLP TO-SUBSYS:*.*.-1
COPY FROM-SOURCE:PTYCON.DOC TO-DOC:*.*.-1

;Library files used for symbols
;COPY FROM-SUBSYS:MACREL.REL TO-SUBSYS:*.*.-1
;COPY FROM-SUBSYS:MACSYM.UNV TO-SUBSYS:*.*.-1
;COPY FROM-SUBSYS:MONSYM.UNV TO-SUBSYS:*.*.-1

;Final product
COPY FROM-SOURCE:PTYCON.EXE TO-SUBSYS:*.*.-1
COPY FROM-SOURCE:PTYCON.ATO TO-SYSTEM:*.*.-1
