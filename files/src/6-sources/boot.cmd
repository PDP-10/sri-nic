; This .CMD file is used by Release eng. and is NOT to be shipped
; with release software.  It is for inhouse building purposes only.
; RBOOT.CMD (ReleaseBOOT) lists files to be shipped.

;Files required to build product
COPY FROM-SOURCE:BOOT.CMD TO-SOURCE:*.*.-1
COPY FROM-SOURCE:RBOOT.CMD TO-SOURCE:*.*.-1
COPY FROM-SOURCE:BOOT.CTL TO-SOURCE:*.*.-1
COPY FROM-SOURCE:BOOT.MAC TO-SOURCE:*.*.-1
COPY FROM-SOURCE:SYSFLG.MAC TO-SOURCE:*.*.-1
COPY FROM-SOURCE:PMT.MAC TO-SOURCE:*.*.-1
COPY FROM-SOURCE:RP2.MAC TO-SOURCE:*.*.-1
COPY FROM-SOURCE:DXMCA.* TO-SOURCE:*.*.-1
COPY FROM-SOURCE:DXMCE.* TO-SOURCE:*.*.-1

;Documentation for product
COPY FROM-SOURCE:BOOT.DOC TO-DOC:*.*.-1
COPY FROM-SOURCE:BOOT.TCO TO-DOC:*.*.-1

