;[MIT-XX]SSY:<EXEC.TEST>OZASSM.CMD.2, 16-Jun-83 18:54:48, Edit by TEK
;716 add CE module
;715 move symbol table and patch area to high segment
;   add	PC,PI,PM,PS,PU,PX modules
;   if PCL is not to be included remove PI,PC,PX,PU modules by hand
;714 add MI,MP modules
;713 reorganize assembly files
;   source: EXEC??.MAC => object: XXEX??.REL

%"/SET:.HIGH.:150000/SYMSEG:HIGH"
EXEC:OZCONF.MAC+EXEC:EXECDE.MAC+EXEC:EXECGL.MAC OZEXGL
EXEC:EXECPR.MAC OZEXPR		
EXEC:EXECVR.MAC OZEXVR		
EXEC:EXEC0.MAC  OZEX0
EXEC:EXECCA.MAC OZEXCA		
EXEC:EXEC1.MAC  OZEX1		
EXEC:EXECSE.MAC OZEXSE		
EXEC:EXECMT.MAC OZEXMT		
EXEC:EXECP.MAC  OZEXP		
EXEC:EXECIN.MAC OZEXIN		
EXEC:EXEC2.MAC  OZEX2
EXEC:EXEC3.MAC  OZEX3
EXEC:EXEC4.MAC  OZEX4	
EXEC:EXECED.MAC OZEXED		
EXEC:EXECCS.MAC OZEXCS		
EXEC:EXECQU.MAC OZEXQU		
EXEC:EXECMP.MAC OZEXMP
EXEC:EXECMI.MAC OZEXMI
EXEC:EXECCE.MAC OZEXCE
EXEC:EXECPM.MAC OZEXPM
EXEC:EXECPS.MAC OZEXPS
EXEC:EXECPI.REL
EXEC:EXECPC.REL
EXEC:EXECPX.REL
EXEC:EXECPU.REL
EXEC:EXECSU.MAC OZEXSU		
SYS:MACREL.REL
