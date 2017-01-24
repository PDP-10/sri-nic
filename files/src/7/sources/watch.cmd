!COMMAND FILE FOR OBTAINING ALL FILES RELEVANT TO WATCH
INFORMATION LOGICAL-NAMES FROM*:
INFORMATION LOGICAL-NAMES TO*:

;Files required to build product
COPY FROM-SOURCE:WATCH.CMD TO-SOURCE:*.*.-1
COPY FROM-SOURCE:WATCH.CTL TO-SOURCE:*.*.-1
COPY FROM-SOURCE:SYMSUB.MAC TO-SOURCE:*.*.-1
COPY FROM-SOURCE:WATMAI.MAC TO-SOURCE:*.*.-1
COPY FROM-SOURCE:WATDAT.MAC TO-SOURCE:*.*.-1
COPY FROM-SOURCE:WATCH.MAC TO-SOURCE:*.*.-1

;Documentation for product
COPY FROM-HLP:WATCH.HLP TO-HLP:*.*.-1
COPY FROM-DOC:WATCH.DOC TO-DOC:*.*.-1

;Final product
COPY FROM-SOURCE:WATCH.EXE TO-SUBSYS:*.*.-1
