!COMMAND FILE FOR OBTAINING ALL COMPONENTS OF DLUSER
INFORMATION LOGICAL-NAME FROM*:
INFORMATION LOGICAL-NAME TO*:

;Files required to build product
COPY FROM-SOURCE:DLUSER.CMD TO-SOURCE:*.*.-1
COPY FROM-SOURCE:DLUSER.CTL TO-SOURCE:*.*.-1
COPY FROM-SOURCE:DLUSER.MAC TO-SOURCE:*.*.-1

;Documentation for product
COPY FROM-SOURCE:DLUSER.HLP TO-SUBSYS:*.*.-1
COPY FROM-SOURCE:DLUSER.DOC TO-DOC:*.*.-1

;Final product
COPY FROM-SOURCE:DLUSER.EXE TO-SUBSYS:*.*.-1