;[MIT-XX]SSY:<EXEC.TEST>XXASSM.CMD.2, 16-Jun-83 18:49:31, Edit by TEK
;716 add CE module
;715 move symbol table and patch area to high segment
;   add	PC,PI,PM,PS,PU,PX modules
;   if PCL is not to be included remove PI,PC,PX,PU modules by hand
;714 add MI,MP modules
;713 reorganize assembly files
;   source: EXEC??.MAC => object: XXEX??.REL

%"/SET:.HIGH.:150000/SYMSEG:HIGH"
EXEC:XXCONF.MAC+EXEC:EXECDE.MAC+EXEC:EXECGL.MAC XXEXGL
EXEC:EXECPR.MAC XXEXPR		
EXEC:EXECVR.MAC XXEXVR		
EXEC:EXEC0.MAC  XXEX0
EXEC:EXECCA.MAC XXEXCA		
EXEC:EXEC1.MAC  XXEX1		
EXEC:EXECSE.MAC XXEXSE		
EXEC:EXECMT.MAC XXEXMT		
EXEC:EXECP.MAC  XXEXP		
EXEC:EXECIN.MAC XXEXIN		
EXEC:EXEC2.MAC  XXEX2
EXEC:EXEC3.MAC  XXEX3
EXEC:EXEC4.MAC  XXEX4	
EXEC:EXECED.MAC XXEXED		
EXEC:EXECCS.MAC XXEXCS		
EXEC:EXECQU.MAC XXEXQU		
EXEC:EXECMP.MAC XXEXMP
EXEC:EXECMI.MAC XXEXMI
EXEC:EXECCE.MAC XXEXCE
EXEC:EXECPM.MAC XXEXPM
EXEC:EXECPS.MAC XXEXPS
EXEC:EXECPI.REL
EXEC:EXECPC.REL
EXEC:EXECPX.REL
EXEC:EXECPU.REL
EXEC:EXECSU.MAC XXEXSU		
SYS:MACREL.REL
