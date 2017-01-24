@; Output (to) Assembler File <FtRel,Load-DirPrc.ctl,> 
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
INCLUDE PLEX <Misc10,Run-Ctl,Load-NlsSpBE-DirPrc-FtRel-T20.d>
*/366001o  ; if change this, must change other things
*dirprc.l10
*hash.l10
*verify.l10
*/; final stuff
*/B
*/366000_[116

@Save DirPrc 366 523
