!COMMAND FILE FOR OBTAINING ALL FILES RELEVANT TO BUILDING MACRO

INFORMATION LOGICAL-NAMES

!SOURCES
COPY FROM-SOURCE:MACRO.MAC TO-LANG-SRC:*.*.-1

!COMMAND FILES
COPY FROM-SOURCE:MACRO.CMD TO-LANG-SRC:*.*.-1
COPY FROM-SOURCE:MACRO.CTL TO-LANG-SRC:*.*.-1

!OUTPUT FILES
COPY FROM-SOURCE:MACRO.EXE TO-SUBSYS:*.*.-1

!DOCUMENTATION

COPY FROM-DOC:MACRO.BWR TO-DOC:*.*.-1
COPY FROM-DOC:MACRO.DOC TO-DOC:*.*.-1
COPY FROM-DOC:MACRO.HLP TO-SUBSYS:*.*.-1

!DONE
TAKE
