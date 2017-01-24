!COMMAND FILE FOR OBTAINING ALL FILES RELEVANT TO SYSJOB
INFORMATION LOGICAL-NAMES FROM*:
INFORMATION LOGICAL-NAMES TO*:

;Files required to build product
COPY FROM-SOURCE:SYSJOB.CMD TO-SOURCE:*.*.-1
COPY FROM-SOURCE:SYSJOB.CTL TO-SOURCE:*.*.-1
COPY FROM-SOURCE:SYSJOB.MAC TO-SOURCE:*.*.-1

;Documentation for product
COPY FROM-SOURCE:SYSJOB.HLP TO-SUBSYS:*.*.-1
COPY FROM-SOURCE:SYSJOB.DOC TO-DOC:*.*.-1

;Final product
COPY FROM-SOURCE:6-SYSJOB.EXE TO-SYSTEM:*.*.-1
COPY FROM-SOURCE:6-SYSJOB.RUN TO-SYSTEM:*.*.-1