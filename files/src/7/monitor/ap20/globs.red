REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:24:21
File 1: SRC:<7.MONITOR>GLOBS.MAC.1
File 2: SRC:<7.MONITOR.AP20>GLOBS.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8977 to GLOBS.MAC on 4-Oct-88 by JROSSELL, for SPR #21689
;Add support for optionally not flushing NULs when a terminal is in ASCII
;mode. 
; Edit= 8974 to GLOBS.MAC on 30-Sep-88 by GSCOTT
;Prevent LAPRBF BUGCHKs on systems without an NIA20. Add routines SMFLTS and
;TMFLTS which implement SMON and TMON function 74. 
; Edit= 8964 to GLOBS.MAC on 8-Sep-88 by RASPUZZI
;Add CLUDGR joining sequence to occur after CFSJYN has finished.
;Add CLUJYN.

***** CHANGE #2; PAGE 3, LINE 359; PAGE 3, LINE 359
QEXT <NETDAL,NETDED,NETDTB,NETDWN,NETFLD,NETFRE,NETFRK,NETHDN,NETHLK>
QEXT <NETHST,NETINI,NETKFK,NETLGO,NETMOV,NETON,NETRAL>
QEXT <NETSTS,NETTCH,NETTCS,NETTIM,NETWK1,NETWKF,NEVNTS>
QEXT <NEWIB,NFACHK,NFBSZ,NFKS,NFKSPP,NFTDTB,NFTIDX,NGCCOR,NGOJOB>
QEXT <NHIPG,NHOME,NHOSTS,NHQFK,NHSTN,NHSTST,NIBUGX,NIC>
QEXT <NIDXPG,NIJFLG,NIJJIF,NILDTB,NIMSTK,NINIDV,NINIPG,NJOBS>
QEXT <NLFKS,NLHOST,NLINES,NLOWPG,NLQFK,NMAPLK,NMAXPG,NNAMES,NNSBE,NNTBFS>
 ---------------------------------
QEXT <NETDAL,NETDED,NETDTB,NETDWN,NETFLD,NETFRE,NETFRK,NETHDN,NETHLK>
QEXT <NETHST,NETINI,NETKFK,NETLGO,NETMOV,NETON,NETRAL>
QEXT <NETSTS,NETTCH,NETTCS,NETTIM,NETWK1,NETWKF,NEVNTS>
QEXT <NEWIB,NFACHK,NFBSZ,NFKS,NFKSPP,NFTDTB,NFTIDX,NGCCOR,NGOJOB>
QEXT <NHIPG,NHOME,NHOSTS,NHQFK,NHSTN,NHSTST,NIBUGX,NIC>
QEXT <NIDXPG,NIHERE,NIJFLG,NIJJIF,NILDTB,NIMSTK,NINIDV,NINIPG,NJOBS>;[8974]
QEXT <NLFKS,NLHOST,NLINES,NLOWPG,NLQFK,NMAPLK,NMAXPG,NNAMES,NNSBE,NNTBFS>

***** CHANGE #3; PAGE 3, LINE 489; PAGE 3, LINE 489
QEXT <SFTADN,SGCCWR,SHLTW,SHRPTR,SHTNCZ,SHTNRL,SIONXT,SIQIBI,SIQIBO,SIQSPC>
QEXT <SIQTIM,SIXASC,SJBFRK,SK2DWN,SK2RET,SK3RET,SKDFIL,SKDFL,SKDIDL>
QEXT <SKDLOG,SKDLRT,SKDLST,SKDOUT,SKDOVH,SKDPC,SKDPDL,SKDSWP>
QEXT <SKDTHS,SKDTM0,SKDTM1,SKDTM2,SKEDF1,SKEDF3,SKEDFC>
QEXT <SKFLGV,SKHWPT,SKIIF,SKIIFA,SKISF,SKMRTN,SKPOPR,SKPPAG,SKPUNL>  ;[7247]
QEXT <SLNINI,SLOWF,SMADNX,SMFILE,SMFLAG,SMLKF,SMTEPT>
QEXT <SMTEPT,SNAMES,SNBLKS,SNDALL,SNDDG,SNDMAI,SNDMSG,SNDSIZ,SNDTIM> ;[7.1190]
 ---------------------------------
QEXT <SFTADN,SGCCWR,SHLTW,SHRPTR,SHTNCZ,SHTNRL,SIONXT,SIQIBI,SIQIBO,SIQSPC>
QEXT <SIQTIM,SIXASC,SJBFRK,SK2DWN,SK2RET,SK3RET,SKDFIL,SKDFL,SKDIDL>
QEXT <SKDLOG,SKDLRT,SKDLST,SKDOUT,SKDOVH,SKDPC,SKDPDL,SKDSWP>
QEXT <SKDTHS,SKDTM0,SKDTM1,SKDTM2,SKEDF1,SKEDF3,SKEDFC>
QEXT <SKFLGV,SKHWPT,SKIIF,SKIIFA,SKISF,SKMRTN,SKPOPR,SKPPAG,SKPUNL>  ;[7247]
QEXT <SLNINI,SLOWF,SMADNX,SMFILE,SMFLAG,SMFLTS,SMLKF,SMTEPT> ;[8974]
QEXT <SMTEPT,SNAMES,SNBLKS,SNDALL,SNDDG,SNDMAI,SNDMSG,SNDSIZ,SNDTIM> ;[7.1190]

***** CHANGE #4; PAGE 3, LINE 520; PAGE 3, LINE 520
QEXT <TAB81,TAB82,TAD11,TADIDT,TATTVT,TCI,TCIB>
QEXT <TCITST,TCO,TCOB,TCOERR,TCOTST,TCOUM,TESTKP,TIM0>
QEXT <TIM1,TIM2,TIMALC,TIMDTQ,TIMLCK,TIMMLQ>
QEXT <TIMSCD,TIMSCM,TIMZDT,TIMZON,TLABBF,TLABBP,TLABL0>
QEXT <TLABL1,TLABL2,TLABL3,TLABL4,TLABL5,TLABL6>
QEXT <TLABL7,TLABL8,TLABL9,TLABR0,TM2DSP,TM8DSP,TMDTTM>
QEXT <TMLSTL,TMMLTM,TMMUSE,TMNSS,TMPSMM,TMRINT,TMSNTR,TMZONE> ;[7.1063]
 ---------------------------------
QEXT <TAB81,TAB82,TAD11,TADIDT,TATTVT,TCI,TCIB>
QEXT <TCITST,TCO,TCOB,TCOERR,TCOTST,TCOUM,TESTKP,TIM0>
QEXT <TIM1,TIM2,TIMALC,TIMDTQ,TIMLCK,TIMMLQ>
QEXT <TIMSCD,TIMSCM,TIMZDT,TIMZON,TLABBF,TLABBP,TLABL0>
QEXT <TLABL1,TLABL2,TLABL3,TLABL4,TLABL5,TLABL6>
QEXT <TLABL7,TLABL8,TLABL9,TLABR0,TM2DSP,TM8DSP,TMDTTM,TMFLTS> ;[8974]
QEXT <TMLSTL,TMMLTM,TMMUSE,TMNSS,TMPSMM,TMRINT,TMSNTR,TMZONE> ;[7.1063]

***** CHANGE #5; PAGE 3, LINE 527; PAGE 3, LINE 527
QEXT <TPFBYN,TPFCNT,TPFLEN,TPFLNX,TPRCYC,TPSBYT>
QEXT <TODCLK,TODDT,TODPWL,TOTRC,TRAPAP,TRAPC,TRAPFL> ;[7.1200] 
QEXT <TRAPPC,TRAPS0,TRAPSK,TRAPSW,TRECPP,TRIMER,TRMBLK>
QEXT <TRMLST,TRMTS1,TRMTST,TRP0,TRPDSP,TRPID,TRPPTR>
QEXT <TSACT1,TSACT3,TSKED,TSTGCC,TSVN,TTABRT,TTACTL>
QEXT <TTBBUF,TTBIGC,TTBIGI,TTBIGO,TTBKPT,TTBSIZ>
 ---------------------------------
QEXT <TPFBYN,TPFCNT,TPFLEN,TPFLNX,TPRCYC,TPSBYT>
QEXT <TODCLK,TODDT,TODPWL,TOTRC,TRAPAP,TRAPC,TRAPFL> ;[7.1200] 
QEXT <TRAPPC,TRAPS0,TRAPSK,TRAPSW,TRECPP,TRIMER,TRMBLK>
QEXT <TRMLST,TRMTS1,TRMTST,TRP0,TRPDSP,TRPID,TRPPTR>
QEXT <TSACT1,TSACT3,TSKED,TSTGCC,TSVN,TTABRT,TTACTL,TTANUL> ;[8977]
QEXT <TTBBUF,TTBIGC,TTBIGI,TTBIGO,TTBKPT,TTBSIZ>

***** CHANGE #6; PAGE 4, LINE 6; PAGE 4, LINE 6

;[7.1074]
;CLUDGR globulars

QEXT <CFSNOD,CLASSF,CLFORK,CLRCLU,CLRTMG,CLRCVQ,CLREQQ,CLSFRK,CLTMSG>
QEXT <CLUBUF,CLUFLG,CLUHST,CLUINI,CLUSET,CLUSTS,CLUTMG>
QEXT <.CNFIG,CPYFU6,.DEVST,.DIRST,.DVCHR>
 ---------------------------------

;[7.1074]
;CLUDGR globulars

QEXT <CFSNOD,CLASSF,CLFORK,CLRCLU,CLRTMG,CLRCVQ,CLREQQ,CLSFRK,CLTMSG>
QEXT <CLUBUF,CLUFLG,CLUHST,CLUINI,CLUJYN,CLUSET,CLUSTS,CLUTMG> ;[8964]
QEXT <.CNFIG,CPYFU6,.DEVST,.DIRST,.DVCHR>

