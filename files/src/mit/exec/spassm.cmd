;[MIT-XX]SSY:<EXEC.TEST>SPASSM.CMD.2, 16-Jun-83 18:57:07, Edit by TEK
;716 add CE module
;715 move symbol table and patch area to high segment
;   add	PC,PI,PM,PS,PU,PX modules
;   if PCL is not to be included remove PI,PC,PX,PU modules by hand
;714 add MI,MP modules
;713 reorganize assembly files
;   source: EXEC??.MAC => object: XXEX??.REL

%"/SET:.HIGH.:150000/SYMSEG:HIGH"
EXEC:SPCONF.MAC+EXEC:EXECDE.MAC+EXEC:EXECGL.MAC SPEXGL
EXEC:EXECPR.MAC SPEXPR		
EXEC:EXECVR.MAC SPEXVR		
EXEC:EXEC0.MAC  SPEX0
EXEC:EXECCA.MAC SPEXCA		
EXEC:EXEC1.MAC  SPEX1		
EXEC:EXECSE.MAC SPEXSE		
EXEC:EXECMT.MAC SPEXMT		
EXEC:EXECP.MAC  SPEXP		
EXEC:EXECIN.MAC SPEXIN		
EXEC:EXEC2.MAC  SPEX2
EXEC:EXEC3.MAC  SPEX3
EXEC:EXEC4.MAC  SPEX4	
EXEC:EXECED.MAC SPEXED		
EXEC:EXECCS.MAC SPEXCS		
EXEC:EXECQU.MAC SPEXQU		
EXEC:EXECMP.MAC SPEXMP
EXEC:EXECMI.MAC SPEXMI
EXEC:EXECCE.MAC SPEXCE
EXEC:EXECPM.MAC SPEXPM
EXEC:EXECPS.MAC SPEXPS
EXEC:EXECPI.REL
EXEC:EXECPC.REL
EXEC:EXECPX.REL
EXEC:EXECPU.REL
EXEC:EXECSU.MAC SPEXSU		
SYS:MACREL.REL

