!COMMAND FILE FOR OBTAINING ALL FILES RELEVANT TO BUILDING RSXFMT
INFORMATION LOGICAL-NAMES

;Files required to build product
COPY FROM-SOURCE:RSXFMT.CMD TO-SOURCE:*.*.-1
COPY FROM-SOURCE:RSXFMT.CTL TO-SOURCE:*.*.-1
COPY FROM-SOURCE:RSXFMT.MAC TO-SOURCE:*.*.-1

;Documentation for product
COPY FROM-SOURCE:RSXFMT.TCO TO-DOC:*.*.-1
COPY FROM-SOURCE:RSXFMT.HLP TO-SUBSYS:*.*.-1
COPY FROM-SOURCE:RSXFMT.DOC TO-DOC:*.*.-1

;Library files used for symbols

;Final product
COPY FROM-SOURCE:RSXFMT.EXE TO-SUBSYS:*.*.-1
