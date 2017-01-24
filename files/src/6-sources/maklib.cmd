!COMMAND FILE FOR OBTAINING ALL FILES RELEVANT TO BUILDING MAKLIB
INFORMATION LOGICAL FROM*:
INFORMATION LOGICAL TO*:

;Files required to build product
COPY FROM-SOURCE:MAKLIB.CMD TO-SOURCE:*.*.-1
COPY FROM-SOURCE:MAKLIB.CTL TO-SOURCE:*.*.-1
COPY FROM-SOURCE:MAKLIB.MAC TO-SOURCE:*.*.-1

;Documentation for product
COPY FROM-SOURCE:MAKLIB.HLP TO-SUBSYS:*.*.-1
COPY FROM-SOURCE:MAKLIB.DOC TO-DOC:*.*.-1

;Final product
COPY FROM-SOURCE:MAKLIB.EXE TO-SUBSYS:*.*.-1
