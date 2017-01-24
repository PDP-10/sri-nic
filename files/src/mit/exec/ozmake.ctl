;[MIT-XX]SSY:<EXEC.TEST>OZMAKE.CTL.2, 16-Jun-83 18:40:21, Edit by TEK
;713 reorganized batch control files
;build MIT-OZ EXEC

@noerror	
@enable
@define exec: dsk:
@define unv: dsk:,ssy:<xx.monrel>, unv:	!get new definitions
@define sys: dsk:,ssy:<xx.monrel>, sys:
@delete ozexgl.rel			!force reassmbly of universals
@execute @ozassm
*y
@rename exec.exe ozexec.exe
@delete *lnk.exe
@logout
%error:
@delete ozexec.exe, *lnk.exe
@logout
