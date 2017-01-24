@; Output (to) Assembler File <FtRel,Load-NlsSpFE.ctl,> 
@Connect PS:<FtRel>
@PS:<FtRel>ArcLdr
*/; switches
*/117e
*/120l
*/M
*/277000s
*/S
*/; modules
*Rt-Data.l10
*feosidata.l10
*dpydata.l10
*fedata.l10
*/P ;load termpage on page boundary
*termpage.l10
*/P ;load termpage on page boundary
*fertns.l10
*feosi.l10
*up.l10
*Rt-Main.l10
*Rt-LRP.l10
*stgmgt.l10
*cli.l10
*cgramldr.l10
*dpypkg.l10
*dpy-10.l10
*fecomm.l10
*pcpl10.l10
*wsprinter.l10
*Syms-OS.l10
*spi.l10
*/300000o
*spidata.l10
*
@Connect PS:<urel>
@DDT
*startup/god
*recover/restart
*debug/0
*presav/1
*l10startg
@Save NlsSpFE 0 767
