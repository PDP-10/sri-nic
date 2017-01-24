; This .CMD file is used by Release eng. as a final list of
; files related to BOOT which get shipped on the Inst/Dist tape

;Documentation for product
COPY FROM-SOURCE:BOOT.DOC TO-DOC:*.*.-1
COPY FROM-SOURCE:BOOT.TCO TO-DOC:*.*.-1

;Final product
COPY FROM-SOURCE:DXMCA.ADX TO-SUBSYS:*.*.-1
