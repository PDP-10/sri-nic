/*==================================================
 *  jsys.h
 *	#include file for jsys support for the V7 C library
 *
 *	Copyright (C) New Mexico Tech 1984, 1985.  All rights reserved.
 *
 *	Author: Greg Titus
 */

/*
 *  Has this file already been #included?  Matching #endif is at bottom of file
 */
#ifndef JSYS_INCLUDED

#define JSYS_INCLUDED

/*
 *  the last Jsys error code (from clfdat.c)
 */
extern int	    errno;

/*
 *  some handy constants
 */
#ifndef NULL
#   define  NULL	0
#endif

#ifndef TRUE
#   define  TRUE	1
#endif

#ifndef FALSE
#   define  FALSE	0
#endif

#ifndef PAGE_SIZE
#   define PAGE_SIZE	01000
#endif

/*
 *  macros to set and extract bit fields
 */
#ifndef putbits
#   define putbits(m,p,x)	(((x) & (m)) << (p))
#endif

#ifndef getbits
#   define getbits(m,p,x)	(((x) >> (p)) & (m))
#endif

/*
 *  primary input and output designators
 */
#ifndef _PRIIN
#   define _PRIIN	0100
#endif
#ifndef _PRIOU
#   define _PRIOU	0101
#endif

/*
 *  null device
 */
#ifndef _NULIO
#   define _NULIO	0377777
#endif

/*
 *  process designators
 */
#define _FHSLF		0400000
#define	_FHJOB		-5

/*
 *	jsys codes
 */
#define	LOGIN_		01
#define	CRJOB_		02
#define	LGOUT_		03
#define	CACCT_		04
#define	EFACT_		05
#define	SMON_		06
#define	TMON_		07
#define	GETAB_		010
#define	ERSTR_		011
#define	GETER_		012
#define	GJINF_		013
#define	TIME_		014
#define	RUNTM_		015
#define	SYSGT_		016
#define	GNJFN_		017
#define	GTJFN_		020
#	define GJ_FOU	0400000000000
#	define GJ_NEW	0200000000000
#	define GJ_OLD	0100000000000
#	define GJ_TMP	0010000000000
#	define GJ_FLG	0000020000000
#	define GJ_FNS	0000002000000
#	define GJ_SHT	0000001000000
#define	OPENF_		021
#define	CLOSF_		022
#define	RLJFN_		023
#define	GTSTS_		024
#define	STSTS_		025
#define	DELF_		026
#define	SFPTR_		027
#define	JFNS_		030
#define	FFFFP_		031
#define	RDDIR_		032
#define	CPRTF_		033
#define	CLZFF_		034
#define	RNAMF_		035
#define	SIZEF_		036
#define	GACTF_		037
#define	STDIR_		040
#define	DIRST_		041
#define	BKJFN_		042
#define	RFPTR_		043
#define	CNDIR_		044
#define	RFBSZ_		045
#define	SFBSZ_		046
#define	SWJFN_		047
#define	BIN_		050
#define	BOUT_		051
#define	SIN_		052
#define	SOUT_		053
#define	RIN_		054
#define	ROUT_		055
#define	PMAP_		056
#define	RPACS_		057
#define	SPACS_		060
#define	RMAP_		061
#define	SACTF_		062
#define	GTFDB_		063
#define	CHFDB_		064
#define	DUMPI_		065
#define	DUMPO_		066
#define	DELDF_		067
#define	ASND_		070
#define	RELD_		071
#define	CSYNO_		072
#define	PBIN_		073
#define	PBOUT_		074
#define	PSIN_		075
#define	PSOUT_		076
#define	MTOPR_		077
#define	CFIBF_		0100
#define	CFOBF_		0101
#define	SIBE_		0102
#define	SOBE_		0103
#define	DOBE_		0104
#define	GTABS_		0105
#define	STABS_		0106
#define	RFMOD_		0107
#define	SFMOD_		0110
#define	RFPOS_		0111
#define	RFCOC_		0112
#define	SFCOC_		0113
#define	STI_		0114
#define	DTACH_		0115
#define	ATACH_		0116
#define	DVCHR_		0117
#define	STDEV_		0120
#define	DEVST_		0121
#define	MOUNT_		0122
#define	DSMNT_		0123
#define	INIDR_		0124
#define	SIR_		0125
#define	EIR_		0126
#define	SKPIR_		0127
#define	DIR_		0130
#define	AIC_		0131
#define	IIC_		0132
#define	DIC_		0133
#define	RCM_		0134
#define	RWM_		0135
#define	DEBRK_		0136
#define	ATI_		0137
#define	DTI_		0140
#define	CIS_		0141
#define	SIRCM_		0142
#define	RIRCM_		0143
#define	RIR_		0144
#define	GDSTS_		0145
#define	SDSTS_		0146
#define	RESET_		0147
#define	RPCAP_		0150
#define	EPCAP_		0151
#define	CFORK_		0152
#	define CR_CAP	0200000000000
#define	KFORK_		0153
#define	FFORK_		0154
#define	RFORK_		0155
#define	RFSTS_		0156
#define	SFORK_		0157
#define	SFACS_		0160
#define	RFACS_		0161
#define	HFORK_		0162
#define	WFORK_		0163
#define	GFRKH_		0164
#define	RFRKH_		0165
#define	GFRKS_		0166
#define	DISMS_		0167
#define	HALTF_		0170
#define	GTRPW_		0171
#define	GTRPI_		0172
#define	RTIW_		0173
#define	STIW_		0174
#define	SOBF_		0175
#define	RWSET_		0176
#define	GETNM_		0177
#define	GET_		0200
#define	SFRKV_		0201
#define	SAVE_		0202
#define	SSAVE_		0203
#define	SEVEC_		0204
#define	GEVEC_		0205
#define	GPJFN_		0206
#define	SPJFN_		0207
#define	SETNM_		0210
#define	FFUFP_		0211
#define	DIBE_		0212
#define	FDFRE_		0213
#define	GDSKC_		0214
#define	LITES_		0215
#define	TLINK_		0216
#define	STPAR_		0217
#define	ODTIM_		0220
#define	IDTIM_		0221
#define	ODCNV_		0222
#define	IDCNV_		0223
#define	NOUT_		0224
#define	NIN_		0225
#define	STAD_		0226
#define	GTAD_		0227
#define	ODTNC_		0230
#define	IDTNC_		0231
#define	FLIN_		0232
#define	FLOUT_		0233
#define	DFIN_		0234
#define	DFOUT_		0235
#define	CRDIR_		0240
#define	GTDIR_		0241
#define	DSKOP_		0242
#define	SPRIW_		0243
#define	DSKAS_		0244
#define	SJPRI_		0245
#define	STO_		0246
#define	ARCF_		0247
#define	ASNDP_		0260
#define	RELDP_		0261
#define	ASNDC_		0262
#define	RELDC_		0263
#define	STRDP_		0264
#define	STPDP_		0265
#define	STSDP_		0266
#define	RDSDP_		0267
#define	WATDP_		0270
#define	GTNCP_		0272
#define	GTHST_		0273
#define	ATNVT_		0274
#define	CVSKT_		0275
#define	CVHST_		0276
#define	FLHST_		0277
#define	GCVEC_		0300
#define	SCVEC_		0301
#define	STTYP_		0302
#define	GTTYP_		0303
#define	BPT_		0304
#define	GTDAL_		0305
#define	WAIT_		0306
#define	HSYS_		0307
#define	USRIO_		0310
#define	PEEK_		0311
#define	MSFRK_		0312
#define	ESOUT_		0313
#define	SPLFK_		0314
#define	ADVIS_		0315
#define	JOBTM_		0316
#define	DELNF_		0317
#define	SWTCH_		0320
#define	TFORK_		0321
#define	RTFRK_		0322
#define	UTFRK_		0323
#define	SCTTY_		0324
#define	SETER_		0336
#define	RSCAN_		0500
#define	HPTIM_		0501
#define	CRLNM_		0502
#define	INLNM_		0503
#define	LNMST_		0504
#define	RDTXT_		0505
#define	SETSN_		0506
#define	GETJI_		0507
#define	MSEND_		0510
#define	MRECV_		0511
#define	MUTIL_		0512
#define	ENQ_		0513
#define	DEQ_		0514
#define	ENQC_		0515
#define	SNOOP_		0516
#define	SPOOL_		0517
#define	ALLOC_		0520
#define	CHKAC_		0521
#define	TIMER_		0522
#define	RDTTY_		0523
#define	TEXTI_		0524
#define	UFPGS_		0525
#define	SFPOS_		0526
#define	SYERR_		0527
#define	DIAG_		0530
#define	SINR_		0531
#define	SOUTR_		0532
#define	RFTAD_		0533
#define	SFTAD_		0534
#define	TBDEL_		0535
#define	TBADD_		0536
#define	TBLUK_		0537
#define	STCMP_		0540
#define	SETJB_		0541
#define	GDVEC_		0542
#define	SDVEC_		0543
#define	COMND_		0544
#define	PRARG_		0545
#define	GACCT_		0546
#define	LPINI_		0547
#define	GFUST_		0550
#define	SFUST_		0551
#define	ACCES_		0552
#define	RCDIR_		0553
#define	RCUSR_		0554
#define	MSTR_		0555
#define	STPPN_		0556
#define	PPNST_		0557
#define	PMCTL_		0560
#define	PLOCK_		0561
#define	BOOT_		0562
#define	UTEST_		0563
#define	USAGE_		0564
#define	WILD_		0565
#define	VACCT_		0566
#define	NODE_		0567
#define	ADBRK_		0570
#define	SINM_		0571
#define	SOUTM_		0572
#define	SWTRP_		0573
#define	GETOK_		0574
#define	RCVOK_		0575
#define	GIVOK_		0576
#define	SKED_		0577
#define	MTU_		0600
#define	XRIR_		0601
#define	XSIR_		0602
#define	PDVOP_		0603
#define	NTMAN_		0604
#define	XSFRK_		0605
#define	XGVEC_		0606
#define	XSVEC_		0607
#define	RSMAP_		0610
#define	XRMAP_		0611
#define	XGTPW_		0612
#define	XSSEV_		0613
#define	XGSEV_		0614
#define	SNDIM_		0750
#define	RCVIM_		0751
#define	ASNSQ_		0752
#define	RELSQ_		0753
#define	METER_		0766
#define	SMAP_		0767
#define	THIBR_		0770
#define	TWAKE_		0771
#define	MRPAC_		0772
#define	SETPV_		0773
#define	MTALN_		0774
#define	TTMSG_		0775
#define	MDDT_		0777

/*
 *	a few handy error codes
 */
#ifndef _ERR_IOX4
    /*
     *	end-of-file reached
     */
#   define _ERR_IOX4	0600220
#endif _ERR_IOX4

#endif	/* JSYS_INCLUDED */
