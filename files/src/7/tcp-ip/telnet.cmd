;
!COMMAND FILE FOR OBTAINING ALL FILES RELEVANT TO BUILDING TELNET
;INFORMATION LOGICAL-NAMES FROM-*:
;INFORMATION LOGICAL-NAMES TO-*:
;
;Files required to build product
COPY FROM-source:TELNET.CMD TO-SOURCE:
COPY FROM-source:TELNET.CTL TO-SOURCE:
COPY FROM-ARPA:TELNET.MAC TO-SOURCE:
COPY FROM-ARPA:HSTNAM.MAC TO-SOURCE:
COPY FROM-ARPA:TELNET.DOC TO-HLP:
COPY FROM-ARPA:TELNET.HLP TO-HLP:
;
;Final product
COPY FROM-ARPA:TELNET.EXE TO-SUBSYS:
