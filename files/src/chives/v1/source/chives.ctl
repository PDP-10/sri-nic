;XX:<CHIVES.V1.SOURCE>CHIVES.CTL.121, 14-Sep-88 12:05:22, Edit by SRA
; Build CHIVES system.
;
; Force restart from begining of file, get logical name frobs.
%fin::
@set default take echo
@take -dirs-.cmd
;
; Compile everything.
;
; Make RSVCTL.EXE
@compile @domsym.cmd
@compile gtdom.usr+gtdom.mac ugtdom.rel
@compile gtdtst.mac
@link
*@gtdtst.ccl
@load rsvctl.mac
;
; Compile everything with appropriate debugging and local configuration.
; The -D options here are for a vanilla version that should run
; on any version of TOPS-20 and which has all the debugging code present
; but disabled.
@cc -q -c -x=macro -DDEBUG -DDUMP_P=1 -DUDP_PACKET_LOGGING -
@   bug20x crit fil20x gc gtable gtoken inaddr insert ipc20x -
@   load lookup makqry match memory names net20x resolv rpkt rrfmt -
@   rrtoa sio20x skipws tables tblook tim20x trees udp20x usr20x zt zzz20x
;
; Below this line should be linking only, no compiling.
;----------------------------------------------------------------
;
; Link the resolver, several forms
;
; Extended addressing version for big configurations with
; big zone files, etcetera.
@cc -q -o=RESOLV -i -
@   bug20x crit fil20x gc gtable gtoken inaddr insert ipc20x load lookup -
@   makqry match memory names resolv rpkt rrfmt rrtoa sio20x skipws -
@   tables tblook tim20x trees udp20x usr20x zzz20x
;
; Non-extended version for little configurations.  Beware
; of the -L command here, it is dependent on the exact behavior
; of KCC at link time....
@cc -q -o=RES0LV -L=/SET:.HIGH.:#200000 -
@   bug20x crit fil20x gc gtable gtoken inaddr insert ipc20x load lookup -
@   makqry match memory names resolv rpkt rrfmt rrtoa sio20x skipws -
@   tables tblook tim20x trees udp20x usr20x zzz20x
;
; Link the zone transfer client
;
@cc -q -o=ZT -
@   bug20x crit fil20x gtable gtoken inaddr insert ipc20x load makqry -
@   match memory names net20x rpkt rrfmt rrtoa sio20x skipws tables -
@   tblook tim20x trees zt
;
; That's all, folks.
