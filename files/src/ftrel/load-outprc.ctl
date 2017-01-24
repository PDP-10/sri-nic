@; Output (to) Assembler File <FtRel,Load-OutPrc.ctl,> 
@Connect PS:<FtRel>
@PS:<FtRel>ArcLdr
*/; switches
*/M
*/A
*/W
*/; low writable data
*/140o
*Rt-Data.l10
*bdata.l10
*/.bLDataArea._140
*mdata.l10
*madata.l10
*msg3data.l10
*sdata.l10
*uodata.l10
*/.eLDataArea.f
*/; low constants
*/.bLConstArea.a
*bconst.l10
*error.l10
*maconst.l10
*brecords.l10
*srecords.l10
*Syms-OS.l10
*/.eLConstArea.f
*/; low code
*/.bLCodeArea.a
*auxcod.l10
*colsrt.l10
*coresupport.l10
*filmnp.l10
*ioexec.l10
*isp.l10
*middle.l10
*psedt1.l10
*psedt2.l10
*seqfil.l10
*seqgen.l10
*utility.l10
*/.eLCodeArea.f
*/; system zone
*/p
*/.sysZone.a
*/276000o
*/.eSysZone.f
*/; pmap pages
*/276000o
*/.p1MapPgAdr.a
*/277000o
*/.pMapPgAdr.a
*/; shared pages
*/300000o
*/.bSpArea.a
*spdata.l10
*/p
*/.eSpArea.f
*Rt-Main.l10
*Rt-LRP.l10
*stgmgt.l10
*/; high non-map-out code
*/.bHCodeArea.a
*adrmnp.l10
*delimiters.l10
*/.eHCodeArea.f
*/J
*/; processor area
*/.bPrcArea._366000
*/.ePrcArea._523777
*/366000o  ; if change this, must change other things
*OpData.l10  ; MUST be first
*DVal.opdinit
*input.l10
*LibeMOP.l10
*LibeDOP.l10
*things.l10
*DrecEx.meta
*docfmt.l10
*stfmt.l10
*lsgcol.l10
*pstprc.l10
*hdrfmt.l10
*pgnate.l10
*oputil.l10
*number.l10
*opexec.l10
*fastop.l10
*/; final stuff
*/.eOutPrc.f  ; symbol table NOT included
*/B
*/366000_.opexec.
*/366001_.stkint.
*/366002_[116
*/.symtabptr._[116
*/.startup._.initll.

@DDT
(Load-OutPrc-switches)
/M
/A
(Load-OutPrc-switches-T20)
*/M
*/A
(Load-OutPrc-modules)
OpData.l10  ; MUST be first
DVal.opdinit
input.l10
LibeMOP.l10
LibeDOP.l10
things.l10
DrecEx.meta
docfmt.l10
stfmt.l10
lsgcol.l10
pstprc.l10
hdrfmt.l10
pgnate.l10
oputil.l10
number.l10
opexec.l10
fastop.l10
(Load-OutPrc-modules-T20)
*OpData.l10  ; MUST be first
*DVal.opdinit
*input.l10
*LibeMOP.l10
*LibeDOP.l10
*things.l10
*DrecEx.meta
*docfmt.l10
*stfmt.l10
*lsgcol.l10
*pstprc.l10
*hdrfmt.l10
*pgnate.l10
*oputil.l10
*number.l10
*opexec.l10
*fastop.l10
(Load-OutPrc-ddt)
l10startgDVal.txt
(Load-OutPrc-ddt-T20)
*l10startgDVal.txt
(Load-OutPrc-XRel)
; Output (to) Assembler File <Lib10,Load-OutPrc.lib,> 
Connect XObj 
<XRel>ArcLdr
/; switches
/M
/A
/S
/; low writable data
/140o
Rt-Data.l10
bdata.l10
/.bLDataArea._140
mdata.l10
madata.l10
msg3data.l10
sdata.l10
uodata.l10
/.eLDataArea.f
/; low constants
/.bLConstArea.a
bconst.l10
error.l10
maconst.l10
brecords.l10
srecords.l10
Syms-OS.l10
/.eLConstArea.f
/; low code
/.bLCodeArea.a
auxcod.l10
colsrt.l10
coresupport.l10
filmnp.l10
ioexec.l10
isp.l10
middle.l10
psedt1.l10
psedt2.l10
seqfil.l10
seqgen.l10
utility.l10
stgmgt.l10
/.eLCodeArea.f
/; system zone
/p
/.sysZone.a
/276000o
/.eSysZone.f
/; pmap pages
/276000o
/.p1MapPgAdr.a
/277000o
/.pMapPgAdr.a
/; shared pages
/300000o
/.bSpArea.a
spdata.l10
/p
/.eSpArea.f
Rt-Main.l10
Rt-LRP.l10
/; high non-map-out code
/.bHCodeArea.a
adrmnp.l10
delimiters.l10
/.eHCodeArea.f
/J
/; processor area
/.bPrcArea._366000
/.ePrcArea._556777
/366000o  ; if change this, must change other things
OpData.l10  ; MUST be first
DVal.opdinit
input.l10
LibeMOP.l10
LibeDOP.l10
things.l10
DrecEx.meta
docfmt.l10
stfmt.l10
lsgcol.l10
pstprc.l10
hdrfmt.l10
pgnate.l10
oputil.l10
number.l10
opexec.l10
fastop.l10
/; final stuff
/B
/.eOutPrc.f  ; symbol table IS included
/366000_.opexec.
/366001_.stkint.
/366002_[116
/.symtabptr._[116
/.startup._.initll.

Connect XRel 
DDT
l10startgDVal.txt
SSave 366 556 OutPrc.sav;
Connect Lib10 
Quit
(Load-OutPrc-XRel-T20)
@; Output (to) Assembler File <Lib10,Load-OutPrc.ctl,> 
@Connect user:<XRel>
@user:<XRel>ArcLdr
*/; switches
*/M
*/A
*/S
*/; low writable data
*/140o
*Rt-Data.l10
*bdata.l10
*/.bLDataArea._140
*mdata.l10
*madata.l10
*msg3data.l10
*sdata.l10
*uodata.l10
*/.eLDataArea.f
*/; low constants
*/.bLConstArea.a
*bconst.l10
*error.l10
*maconst.l10
*brecords.l10
*srecords.l10
*Syms-OS.l10
*/.eLConstArea.f
*/; low code
*/.bLCodeArea.a
*auxcod.l10
*colsrt.l10
*coresupport.l10
*filmnp.l10
*ioexec.l10
*isp.l10
*middle.l10
*psedt1.l10
*psedt2.l10
*seqfil.l10
*seqgen.l10
*utility.l10
*/.eLCodeArea.f
*/; system zone
*/p
*/.sysZone.a
*/276000o
*/.eSysZone.f
*/; pmap pages
*/276000o
*/.p1MapPgAdr.a
*/277000o
*/.pMapPgAdr.a
*/; shared pages
*/300000o
*/.bSpArea.a
*spdata.l10
*/p
*/.eSpArea.f
*Rt-Main.l10
*Rt-LRP.l10
*stgmgt.l10
*/; high non-map-out code
*/.bHCodeArea.a
*adrmnp.l10
*delimiters.l10
*/.eHCodeArea.f
*/J
*/; processor area
*/.bPrcArea._366000
*/.ePrcArea._556777
*/366000o  ; if change this, must change other things
*OpData.l10  ; MUST be first
*DVal.opdinit
*input.l10
*LibeMOP.l10
*LibeDOP.l10
*things.l10
*DrecEx.meta
*docfmt.l10
*stfmt.l10
*lsgcol.l10
*pstprc.l10
*hdrfmt.l10
*pgnate.l10
*oputil.l10
*number.l10
*opexec.l10
*fastop.l10
*/; final stuff
*/B
*/.eOutPrc.f  ; symbol table IS included
*/366000_.opexec.
*/366001_.stkint.
*/366002_[116
*/.symtabptr._[116
*/.startup._.initll.
*
Connect XRel 
@DDT
*l10startgDVal.txt
@Save OutPrc 366 556
(Load-OutPrc-FtRel)
; Output (to) Assembler File <FtRel,Load-OutPrc.run,> 
Connect FtObj 
<FtRel>ArcLdr
/; switches
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-switches.d>
/W
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-modules-low.d>
/J
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-OutPrc-FtRel.d>
/366000o  ; if change this, must change other things
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-modules.d>
/; final stuff
/.eOutPrc.f  ; symbol table NOT included
/B
/366000_.opexec.
/366001_.stkint.
/366002_[116
/.symtabptr._[116
/.startup._.initll.

DDT
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-ddt.d>
Connect FtRel 
SSave 366 523 OutPrc.sav;
(Load-OutPrc-urel)
; Output (to) Assembler File <FtRel,Load-OutPrc-urel.run,> 
Connect FtObj 
<FtRel>ArcLdr
/; switches
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-switches.d>
/W
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-modules-low.d>
/J
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-OutPrc-FtRel.d>
/366000o  ; if change this, must change other things
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-modules.d>
/; final stuff
/.eOutPrc.f  ; symbol table NOT included
/B
/366000_.opexec.
/366001_.stkint.
/366002_[116
/.symtabptr._[116
/.startup._.initll.

DDT
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-ddt.d>
Connect urel 
SSave 366 523 OutPrc.sav;
(FtRel-OutPrc-Load)
Connect FtObj 
<FtRel>ArcLdr
/; switches
/M
/A
/W
/; low writable data
/140o
Rt-Data.l10
bdata.l10
/.bLDataArea._140
mdata.l10
madata.l10
msg3data.l10
sdata.l10
uodata.l10
/.eLDataArea.f
/; low constants
/.bLConstArea.a
bconst.l10
error.l10
maconst.l10
brecords.l10
srecords.l10
Syms-OS.l10
/.eLConstArea.f
/; low code
/.bLCodeArea.a
auxcod.l10
colsrt.l10
coresupport.l10
filmnp.l10
ioexec.l10
isp.l10
middle.l10
psedt1.l10
psedt2.l10
seqfil.l10
seqgen.l10
utility.l10
stgmgt.l10
/.eLCodeArea.f
/; system zone
/p
/.sysZone.a
/276000o
/.eSysZone.f
/; pmap pages
/276000o
/.p1MapPgAdr.a
/277000o
/.pMapPgAdr.a
/; shared pages
/300000o
/.bSpArea.a
spdata.l10
/p
/.eSpArea.f
Rt-Main.l10
Rt-LRP.l10
/; high non-map-out code
/.bHCodeArea.a
adrmnp.l10
delimiters.l10
/.eHCodeArea.f
/J
/; processor area
/.bPrcArea._366000
/.ePrcArea._523777
/366000o  ; if change this, must change other things
OpData.l10  ; MUST be first
DVal.opdinit
input.l10
LibeMOP.l10
LibeDOP.l10
things.l10
DrecEx.meta
docfmt.l10
stfmt.l10
lsgcol.l10
pstprc.l10
hdrfmt.l10
pgnate.l10
oputil.l10
number.l10
opexec.l10
fastop.l10
/; final stuff
/.eOutPrc.f  ; symbol table NOT included
/B
/366000_.opexec.
/366001_.stkint.
/366002_[116
/.symtabptr._[116
/.startup._.initll.

DDT
l10startgDVal.txt
(Load-OutPrc-FtRel-T20)
@; Output (to) Assembler File <FtRel,Load-OutPrc.ctl,> 
@Connect PS:<FtRel>
@PS:<FtRel>ArcLdr
*/; switches
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-switches-T20.d>
*/W
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-modules-low-T20.d>
*/J
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-OutPrc-FtRel-T20.d>
*/366000o  ; if change this, must change other things
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-modules-T20.d>
*/; final stuff
*/.eOutPrc.f  ; symbol table NOT included
*/B
*/366000_.opexec.
*/366001_.stkint.
*/366002_[116
*/.symtabptr._[116
*/.startup._.initll.

@DDT
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-ddt-T20>
@Save OutPrc 366 523
(Load-OutPrc-urel-T20)
@; Output (to) Assembler File <FtRel,Load-OutPrc-urel.ctl,> 
@Connect PS:<FtRel>
@PS:<FtRel>ArcLdr
*/; switches
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-switches-T20.d>
*/W
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-modules-low-T20.d>
*/J
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-OutPrc-FtRel-T20.d>
*/366000o  ; if change this, must change other things
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-modules-T20.d>
*/; final stuff
*/.eOutPrc.f  ; symbol table NOT included
*/B
*/366000_.opexec.
*/366001_.stkint.
*/366002_[116
*/.symtabptr._[116
*/.startup._.initll.

@DDT
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-ddt-T20>
@Connect PS:<urel>
@Save OutPrc 366 523
(FtRel-OutPrc-Load-T20)
@Connect PS:<FtRel>
@PS:<FtRel>ArcLdr
*/; switches
*/M
*/A
*/W
*/; low writable data
*/140o
*Rt-Data.l10
*bdata.l10
*/.bLDataArea._140
*mdata.l10
*madata.l10
*msg3data.l10
*sdata.l10
*uodata.l10
*/.eLDataArea.f
*/; low constants
*/.bLConstArea.a
*bconst.l10
*error.l10
*maconst.l10
*brecords.l10
*srecords.l10
*Syms-OS.l10
*/.eLConstArea.f
*/; low code
*/.bLCodeArea.a
*auxcod.l10
*colsrt.l10
*coresupport.l10
*filmnp.l10
*ioexec.l10
*isp.l10
*middle.l10
*psedt1.l10
*psedt2.l10
*seqfil.l10
*seqgen.l10
*utility.l10
*/.eLCodeArea.f
*/; system zone
*/p
*/.sysZone.a
*/276000o
*/.eSysZone.f
*/; pmap pages
*/276000o
*/.p1MapPgAdr.a
*/277000o
*/.pMapPgAdr.a
*/; shared pages
*/300000o
*/.bSpArea.a
*spdata.l10
*/p
*/.eSpArea.f
*Rt-Main.l10
*Rt-LRP.l10
*stgmgt.l10
*/; high non-map-out code
*/.bHCodeArea.a
*adrmnp.l10
*delimiters.l10
*/.eHCodeArea.f
*/J
*/; processor area
*/.bPrcArea._366000
*/.ePrcArea._523777
*/366000o  ; if change this, must change other things
*OpData.l10  ; MUST be first
*DVal.opdinit
*input.l10
*LibeMOP.l10
*LibeDOP.l10
*things.l10
*DrecEx.meta
*docfmt.l10
*stfmt.l10
*lsgcol.l10
*pstprc.l10
*hdrfmt.l10
*pgnate.l10
*oputil.l10
*number.l10
*opexec.l10
*fastop.l10
*/; final stuff
*/.eOutPrc.f  ; symbol table NOT included
*/B
*/366000_.opexec.
*/366001_.stkint.
*/366002_[116
*/.symtabptr._[116
*/.startup._.initll.

@DDT
(Load-OutPrc-switches)
/M
/A
(Load-OutPrc-switches-T20)
*/M
*/A
(Load-OutPrc-modules)
OpData.l10  ; MUST be first
DVal.opdinit
input.l10
LibeMOP.l10
LibeDOP.l10
things.l10
DrecEx.meta
docfmt.l10
stfmt.l10
lsgcol.l10
pstprc.l10
hdrfmt.l10
pgnate.l10
oputil.l10
number.l10
opexec.l10
fastop.l10
(Load-OutPrc-modules-T20)
*OpData.l10  ; MUST be first
*DVal.opdinit
*input.l10
*LibeMOP.l10
*LibeDOP.l10
*things.l10
*DrecEx.meta
*docfmt.l10
*stfmt.l10
*lsgcol.l10
*pstprc.l10
*hdrfmt.l10
*pgnate.l10
*oputil.l10
*number.l10
*opexec.l10
*fastop.l10
(Load-OutPrc-ddt)
l10startgDVal.txt
(Load-OutPrc-ddt-T20)
*l10startgDVal.txt
(Load-OutPrc-XRel)
; Output (to) Assembler File <Lib10,Load-OutPrc.lib,> 
Connect XObj 
<XRel>ArcLdr
/; switches
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-switches.d>
/S
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-modules-low.d>
/J
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-OutPrc-XRel.d>
/366000o  ; if change this, must change other things
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-modules.d>
/; final stuff
/B
/.eOutPrc.f  ; symbol table IS included
/366000_.opexec.
/366001_.stkint.
/366002_[116
/.symtabptr._[116
/.startup._.initll.

Connect XRel 
DDT
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-ddt.d>
SSave 366 556 OutPrc.sav;
Connect Lib10 
Quit
(Load-OutPrc-XRel-T20)
@; Output (to) Assembler File <Lib10,Load-OutPrc.ctl,> 
@Connect user:<XRel>
@user:<XRel>ArcLdr
*/; switches
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-switches-T20.d>
*/S
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-modules-low-T20.d>
*/J
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-OutPrc-XRel-T20.d>
*/366000o  ; if change this, must change other things
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-modules-T20.d>
*/; final stuff
*/B
*/.eOutPrc.f  ; symbol table IS included
*/366000_.opexec.
*/366001_.stkint.
*/366002_[116
*/.symtabptr._[116
*/.startup._.initll.
*
Connect XRel 
@DDT
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-ddt-T20.d>
@Save OutPrc 366 556
(Load-OutPrc-FtRel)
; Output (to) Assembler File <FtRel,Load-OutPrc.run,> 
INCLUDE PLEX <Misc10,Run-Ctl,FtRel-OutPrc-Load.d>
Connect FtRel 
SSave 366 523 OutPrc.sav;
(Load-OutPrc-urel)
; Output (to) Assembler File <FtRel,Load-OutPrc-urel.run,> 
INCLUDE PLEX <Misc10,Run-Ctl,FtRel-OutPrc-Load.d>
Connect urel 
SSave 366 523 OutPrc.sav;
(FtRel-OutPrc-Load)
Connect FtObj 
<FtRel>ArcLdr
/; switches
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-switches.d>
/W
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-modules-low.d>
/J
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-OutPrc-FtRel.d>
/366000o  ; if change this, must change other things
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-modules.d>
/; final stuff
/.eOutPrc.f  ; symbol table NOT included
/B
/366000_.opexec.
/366001_.stkint.
/366002_[116
/.symtabptr._[116
/.startup._.initll.

DDT
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-ddt.d>
(Load-OutPrc-FtRel-T20)
@; Output (to) Assembler File <FtRel,Load-OutPrc.ctl,> 
INCLUDE PLEX <Misc10,Run-Ctl,FtRel-OutPrc-Load-T20.d>
@Save OutPrc 366 523
(Load-OutPrc-urel-T20)
@; Output (to) Assembler File <FtRel,Load-OutPrc-urel.ctl,> 
INCLUDE PLEX <Misc10,Run-Ctl,FtRel-OutPrc-Load-T20.d>
@Connect PS:<urel>
@Save OutPrc 366 523
(FtRel-OutPrc-Load-T20)
@Connect PS:<FtRel>
@PS:<FtRel>ArcLdr
*/; switches
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-switches-T20.d>
*/W
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-modules-low-T20.d>
*/J
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-OutPrc-FtRel-T20.d>
*/366000o  ; if change this, must change other things
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-modules-T20.d>
*/; final stuff
*/.eOutPrc.f  ; symbol table NOT included
*/B
*/366000_.opexec.
*/366001_.stkint.
*/366002_[116
*/.symtabptr._[116
*/.startup._.initll.

@DDT
INCLUDE PLEX <Misc10,Run-Ctl,Load-OutPrc-ddt-T20>
(Load-OpDInit-XRel)
; Output (to) Assembler File <Lib10,Load-OpDInit.lib,> 
Connect XObj 
<XRel>ArcLdr
/400010o
Libe.l10  ; must be on page boundary
OpDInit.meta
LibeOpDinit.l10
Rt-MiniData.l10
Rt-MiniMain.l10
Syms-OS.l10

DDT
move 1,116xmovem 400000x
startup/initll
l10startgOpDInit.meta
OpDInit.opdinit
Delete OpDInit.opdinit;*
Connect XRel 
SSave 400 554 <XRel>OpDInit.sav;
Connect Lib10 
Quit
(Load-OpDInit-XRel)
; Output (to) Assembler File <Lib10,Load-OpDInit.lib,> 
Connect XObj 
<XRel>ArcLdr
/400010o
Libe.l10  ; must be on page boundary
OpDInit.meta
LibeOpDinit.l10
Rt-MiniData.l10
Rt-MiniMain.l10
Syms-OS.l10

DDT
move 1,116xmovem 400000x
startup/initll
l10startgOpDInit.meta
OpDInit.opdinit
Delete OpDInit.opdinit;*
Connect XRel 
SSave 400 554 <XRel>OpDInit.sav;
Connect Lib10 
Quit
@Save OutPrc 366 523
