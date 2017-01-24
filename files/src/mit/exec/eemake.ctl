;[MIT-XX]SSY:<EXEC.TEST>EEMAKE.CTL.2, 16-Jun-83 18:42:19, Edit by TEK
;713 reorganize batch control files
;build MIT-EECS EXEC

@noerror	
@enable
@define exec: dsk:
@define unv: dsk:,ssy:<xx.monrel>, unv:	!get new definitions
@define sys: dsk:,ssy:<xx.monrel>, sys:
@delete eeexgl.rel			!force reassmbly of universals
@execute @eeassm
*y
@rename exec.exe eeexec.exe
@delete *lnk.exe
@logout
%error:
@delete eeexec.exe, *lnk.exe
@logout
