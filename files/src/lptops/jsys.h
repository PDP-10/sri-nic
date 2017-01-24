/* <SUBSYS.PCC>JSYS.H.4, 13-Jan-86 11:30:10, Edit by BEEBE */
/* added missing JSppnst and JSstppn */
/* Jsys defines for pcc-20.  This file is jsys.h  J.Lepreau 2/81 */

#define JSerr 0
#define	JSok  1

/* Created file from MONSYM.MAC.  6-Jan-77.  Dan Halbert. (for bcpl) */

#define JSjsys	00
#define JSlogin	01
#define JScrjob	02
#define JSlgout	03
#define JScacct	04
#define JSefact	05
#define JSsmon	06
#define JStmon	07
#define JSgetab	010
#define JSerstr	011
#define JSgeter	012
#define JSgjinf	013
#define JStime	014
#define JSruntm	015
#define JSsysgt	016
#define JSgnjfn	017
#define JSgtjfn	020
#define JSopenf	021
#define JSclosf	022
#define JSrljfn	023
#define JSgtsts	024
#define JSststs	025
#define JSdelf	026
#define JSsfptr	027
#define JSjfns	030
#define JSffffp	031
#define JSrddir	032
#define JScprtf	033			/* not implemented */
#define JSclzff	034
#define JSrnamf	035
#define JSsizef	036
#define JSgactf	037
#define JSstdir	040
#define JSdirst	041
#define JSbkjfn	042
#define JSrfptr	043
#define JScndir	044
#define JSrfbsz	045
#define JSsfbsz	046
#define JSswjfn	047
#define JSbin	050
#define JSbout	051
#define JSsin	052
#define JSsout	053
#define JSrin	054
#define JSrout	055
#define JSpmap	056
#define JSrpacs	057
#define JSspacs	060
#define JSrmap	061
#define JSsactf	062
#define JSgtfdb	063
#define JSchfdb	064
#define JSdumpi	065
#define JSdumpo	066
#define JSdeldf	067
#define JSasnd	070
#define JSreld	071
#define JScsyno	072			/* not implemented */
#define JSpbin	073
#define JSpbout	074
#define JSpsin	075			/* not implemented */
#define JSpsout	076
#define JSmtopr	077
#define JScfibf	0100
#define JScfobf	0101
#define JSsibe	0102
#define JSsobe	0103
#define JSdobe	0104
#define JSgtabs	0105
#define JSstabs	0106
#define JSrfmod	0107
#define JSsfmod	0110
#define JSrfpos	0111
#define JSrfcoc	0112
#define JSsfcoc	0113
#define JSsti	0114
#define JSdtach	0115
#define JSatach	0116
#define JSdvchr	0117
#define JSstdev	0120
#define JSdevst	0121
#define JSmount	0122
#define JSdsmnt	0123
#define JSinidr	0124
#define JSsir	0125
#define JSeir	0126
#define JSskpir	0127
#define JSdir	0130
#define JSaic	0131
#define JSiic	0132
#define JSdic	0133
#define JSrcm	0134
#define JSrwm	0135
#define JSdebrk	0136
#define JSati	0137
#define JSdti	0140
#define JScis	0141
#define JSsircm	0142
#define JSrircm	0143
#define JSrir	0144
#define JSgdsts	0145
#define JSsdsts	0146
#define JSreset	0147
#define JSrpcap	0150
#define JSepcap	0151
#define JScfork	0152
#define JSkfork	0153
#define JSffork	0154
#define JSrfork	0155
#define JSrfsts	0156
#define JSsfork	0157
#define JSsfacs	0160
#define JSrfacs	0161
#define JShfork	0162
#define JSwfork	0163
#define JSgfrkh	0164			/* not implemented */
#define JSrfrkh	0165
#define JSgfrks	0166
#define JSdisms	0167
#define JShaltf	0170
#define JSgtrpw	0171
#define JSgtrpi	0172
#define JSrtiw	0173
#define JSstiw	0174
#define JSsobf	0175
#define JSrwset	0176
#define JSgetnm	0177
#define JSget	0200
#define JSsfrkv	0201
#define JSsave	0202
#define JSssave	0203
#define JSsevec	0204
#define JSgevec	0205
#define JSgpjfn	0206
#define JSspjfn	0207
#define JSsetnm	0210
#define JSffufp	0211
#define JSdibe	0212
#define JSfdfre	0213			/* obsoleted by new directory format */
#define JSgdskc	0214
#define JSlites	0215
#define JStlink	0216
#define JSstpar	0217
#define JSodtim	0220
#define JSidtim	0221
#define JSodcnv	0222
#define JSidcnv	0223
#define JSnout	0224
#define JSnin	0225
#define JSstad	0226
#define JSgtad	0227
#define JSodtnc	0230
#define JSidtnc	0231
#define JSflin	0232
#define JSflout	0233
#define JSdfin	0234
#define JSdfout	0235
#define JScrdir	0240
#define JSgtdir	0241
#define JSdskop	0242
#define JSspriw	0243
#define JSdskas	0244
#define JSsjpri	0245
#define JSsto	0246
#define JSbbniit 0247
#define JSarcf	0247	/* tops-20 only */
#define JSasndp	0260			/* not implemented */
#define JSreldp	0261			/* not implemented */
#define JSasndc	0262			/* not implemented */
#define JSreldc	0263			/* not implemented */
#define JSstrdp	0264			/* not implemented */
#define JSstpdp	0265			/* not implemented */
#define JSstsdp	0266			/* not implemented */
#define JSrdsdp	0267			/* not implemented */
#define JSwatdp	0270			/* not implemented */
#define JSatnvt	0274
#define JScvskt	0275
#define JScvhst	0276
#define JSflhst	0277
#define JSgcvec	0300
#define JSscvec	0301
#define JSsttyp	0302
#define JSgttyp	0303
#define JSbpt	0304
#define JSgtdal	0305
#define JSwait	0306
#define JShsys	0307
#define JSusrio	0310
#define JSpeek	0311
#define JSmsfrk	0312
#define JSesout	0313
#define JSsplfk	0314
#define JSadvis	0315			/* not implemented */
#define JSjobtm	0316			/* not implemented */
#define JSdelnf	0317
#define JSswtch	0320
#define JStfork	0321
#define JSrtfrk	0322
#define JSutfrk	0323
#define JSoprfn	0326
#define JScgrp	0327
#define JSvacct	0330
#define JSgdacc	0331
#define JSatgrp	0332
#define JSgactj	0333
#define JSgpsgn	0334
#define JSrscan	0500
#define JShptim	0501
#define JScrlnm	0502
#define JSinlnm	0503
#define JSlnmst	0504
#define JSrdtxt	0505	/* obsoleted by rdtty and texti */
#define JSsetsn	0506
#define JSgetji	0507
#define JSmsend	0510
#define JSmrecv	0511
#define JSmutil	0512
#define JSenq	0513
#define JSdeq	0514
#define JSenqc	0515
#define JSsnoop	0516
#define JSspool	0517
#define JSalloc	0520
#define JSchkac	0521
#define JStimer	0522
#define JSrdtty	0523
#define JStexti	0524
#define JSufpgs	0525
#define JSsfpos	0526
#define JSsyerr	0527
#define JSdiag	0530
#define JSsinr	0531
#define JSsoutr	0532
#define JSrftad	0533
#define JSsftad	0534
#define JStbdel	0535
#define JStbadd	0536
#define JStbluk	0537
#define JSstcmp	0540
#define JSsetjb	0541
#define JSgdvec	0542
#define JSsdvec	0543
#define JScomnd	0544
#define JSprarg	0545
#define JSgacct	0546
#define JSlpini	0547
#define JSgfust	0550
#define JSsfust	0551
#define JSacces	0552
#define JSrcdir	0553
#define JSrcusr	0554
#define JSstppn 0556			/* added [13-Jan-85] */
#define JSppnst 0557			/* added [13-Jan-85] */
#define JSsndim	0750
#define JSrcvim	0751
#define JSasnsq	0752
#define JSrelsq	0753
#define JSthibr	0770
#define JStwake	0771
#define JSmrpac	0772
#define JSsetpv	0773
#define JSmtaln	0774
#define JSttmsg	0775
