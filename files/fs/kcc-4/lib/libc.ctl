@take logicals.cmd
@!
@!	compile stdio routines
@!
@connect cs:
@take stdio.cmd
@!
@!	compile math routines
@!
@connect cm:
@take math.cmd
@!
@!	compile unix syscall routines
@!
@connect cu:
@take usys.cmd
@!
@!	compile network stuff
@!
@connect cn:
@take network.cmd
@!
@!	compile everything else
@!
@connect libc:
@take libc.cmd
@!
@!	now merge them
@!
@maklib
*libc=@maklib.cmd
*libc.poi= libc/points
*
@pop
@logout
