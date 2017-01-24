;XX:<CHIVES.V1.SOURCE>-DIRS-.CMD.3,  9-Sep-88 02:50:57, Edit by SRA
; Logical name definitions to compile CHIVES.
;----------------------------------------------------------------
; This file has to be customized to your particular site.  The various
; .CTL files that build the CHIVES code all look for this file to
; figure out where you are keeping things.
;
; This file is provided by way of an example.  The values used on XX
; are provided in commented out form to help you figure out what the
; logical names refer to.
;----------------------------------------------------------------
; Where the sources currently live
;define CHIVES_SOURCE:	XX:<CHIVES.V1.SOURCE>
;
; Where monitor universal files live (for GTDOM/UGTDOM)
;define MONITOR_UNIVERSALS: foo:<bar>
;
; Search paths built out of the preceeding
define DSK: DSK:,CHIVES_SOURCE:
define SYS: DSK:,SYS:
;define UNV: DSK:,MONITOR_UNIVERSALS:,UNV:
