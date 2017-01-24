;[MIT-XX]SSY:<EXEC.TEST>XXMAKE.CTL.2, 16-Jun-83 18:15:07, Edit by TEK
;713 reorganized batch control files
;build MIT-XX EXEC

@noerror			
@enable
@define exec: dsk:
@define unv: dsk:,unv:			!get local definitions first
@define sys: dsk:,sys:			!ditto
@delete xxexgl.rel			!force reassmbly of universals
@execute @xxassm
*y
@rename exec.exe xxexec.exe
;%err::					!we never get here anyway
;@delete xxexec.exe			!don't keep a losing exec
;%fin::					!here on all cases
@delete *lnk.exe			!get rid of temporary exe file
