;[MIT-XX]SSY:<EXEC.TEST>SPMAKE.CTL.2, 16-Jun-83 18:47:19, Edit by TEK
;713 reorganize batch control files
;build MIT-SPEECH EXEC

@noerror
@enable
@define exec: dsk:
@define unv: dsk:,ssy:<xx.monrel>, unv:	!get new definitions
@define sys: dsk:,ssy:<xx.monrel>, sys:
@delete spexgl.rel			!force reassmbly of universals
@execute @spassm
*y
@rename exec.exe spexec.exe
@delete *lnk.exe
@logout
%error:
@delete spexec.exe, *lnk.exe
@logout
