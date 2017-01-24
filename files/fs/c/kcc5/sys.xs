
<C.KCC.CC>CC.H.199

( 2.65) {SYS_}
				/*	n different, some TGSYS_ value */

<C.KCC.CC>CCPARM.H.30

( 1.17) {SYS_}
**	Note that for TGSYS_NULL, the source system can be found using c-env.h.

( 1.21) {SYS_}
	TGSYS_NULL,	/* Target system same as source system */

( 1.22) {SYS_}
	TGSYS_TOPS20,	/* Target: DEC TOPS-20 system */

( 1.23) {SYS_}
	TGSYS_WAITS,	/* Target: SU WAITS system */

( 1.24) {SYS_}
	TGSYS_TENEX,	/* Target: BBN TENEX / TYMSHARE AUGUST system */

( 1.25) {SYS_}
	TGSYS_TOPS10,	/* Target: DEC TOPS-10 system */

( 1.26) {SYS_}
	TGSYS_ITS	/* Target: MIT ITS system */

<C.KCC.CC>CCSITE.H.14

( 1.18) {SYS_}
#elif SYS_10X

( 1.20) {SYS_}
#elif SYS_ITS

( 1.22) {SYS_}
#elif SYS_T20

( 1.27) {SYS_,SYS_,SYS_}
#elif SYS_T20+SYS_10X+SYS_ITS

( 1.29) {SYS_}
#elif SYS_WAITS

( 1.37) {SYS_}
#elif SYS_10X

( 1.39) {SYS_}
#elif SYS_ITS

( 1.41) {SYS_}
#elif SYS_T20

( 1.46) {SYS_,SYS_}
#elif SYS_T20+SYS_10X

( 1.48) {SYS_}
#elif SYS_WAITS

( 1.50) {SYS_}
#elif SYS_ITS

<C.KCC.CC>CC.C.187

( 7.9) {SYS_}
	"tops20", &tgsys, TGSYS_TOPS20,	/* 5 choices of system */

( 7.10) {SYS_}
	"tops10", &tgsys, TGSYS_TOPS10,

( 7.11) {SYS_}
 	"waits",  &tgsys, TGSYS_WAITS,

( 7.12) {SYS_}
	"tenex",  &tgsys, TGSYS_TENEX,

( 7.13) {SYS_}
	"its",    &tgsys, TGSYS_ITS,

( 7.61) {SYS_}
#if SYS_WAITS

( 7.62) {SYS_}
		(tgsys != TGSYS_WAITS);

( 7.64) {SYS_}
		(tgsys == TGSYS_WAITS);

( 15.12) {SYS_}
#if SYS_WAITS	/* Running on WAITS */

( 15.13) {SYS_}
    if (tgsys != TGSYS_WAITS) {	/* Convert from WAITS ASCII to USASCII */

( 15.21) {SYS_}
    if (tgsys == TGSYS_WAITS) {	/* Convert from USASCII to WAITS ASCII */

<C.KCC.CC>CCASMB.C.97

( 1.57) {SYS_,SYS_,SYS_}
#if SYS_T20+SYS_10X+SYS_T10

( 3.11) {SYS_,SYS_}
#if SYS_T20+SYS_10X		/* Run assembler, don't chain */

( 4.2) {SYS_}
#if SYS_T20

( 4.19) {SYS_,SYS_,SYS_}
#if SYS_T20+SYS_10X+SYS_T10

( 5.14) {SYS_}
#if SYS_T20	/* Check for PRARG */

( 5.37) {SYS_}
#endif /* SYS_T20 */

( 5.39) {SYS_}
#if SYS_T10

( 5.46) {SYS_,SYS_}
#if SYS_T20+SYS_10X

( 5.74) {SYS_}
#if SYS_T20

( 5.95) {SYS_}
#endif	/* SYS_T20 */

( 6.5) {SYS_,SYS_,SYS_}
#if SYS_T20+SYS_10X+SYS_T10

( 6.14) {SYS_}
#if SYS_T20

( 6.29) {SYS_}
#if SYS_T10

( 6.35) {SYS_,SYS_}
#if SYS_T20+SYS_10X

( 7.13) {SYS_,SYS_,SYS_}
#if SYS_T20+SYS_10X+SYS_T10

( 8.18) {SYS_}
#if SYS_T20

( 8.37) {SYS_}
#if SYS_T20

( 9.2) {SYS_}
#if SYS_T20

( 10.25) {SYS_}
#if SYS_ITS

( 10.36) {SYS_}
#if SYS_ITS

( 10.49) {SYS_}
#if SYS_ITS

( 11.33) {SYS_}
#if SYS_T20

<C.KCC.CC>CCDATA.C.166

( 2.27) {SYS_}
#if SYS_T20

( 2.28) {SYS_}
	TGSYS_TOPS20;

( 2.29) {SYS_}
#elif SYS_T10

( 2.30) {SYS_}
	TGSYS_TOPS10;

( 2.31) {SYS_}
#elif SYS_TENEX

( 2.32) {SYS_}
	TGSYS_TENEX;

( 2.33) {SYS_}
#elif SYS_WAITS

( 2.34) {SYS_}
	TGSYS_WAITS;

( 2.35) {SYS_}
#elif SYS_ITS

( 2.36) {SYS_}
	TGSYS_ITS;

( 2.38) {SYS_}
	TGSYS_NULL;

( 2.60) {SYS_,SYS_}
#if SYS_T10+SYS_WAITS

( 2.62) {SYS_,SYS_}
#elif SYS_T20+SYS_10X

( 2.64) {SYS_}
#elif SYS_ITS

<C.KCC.CC>CCOUT.C.259

( 29.90) {SYS_,SYS_}
	if (tgsys == TGSYS_TOPS20 || tgsys == TGSYS_TENEX) {

( 29.92) {SYS_}
	    if (tgsys == TGSYS_TOPS20)

( 29.98) {SYS_}
	cp = estrcpy(cp, (tgsys == TGSYS_TOPS20


	Lines recognized = 69
   String    Matches  Unrecognized Matches
1) "SYS_"	86	0
Exact match search performed.

Files with no matches: 	<C.KCC.CC>CCCHAR.H.11	<C.KCC.CC>CCCODE.H.46
<C.KCC.CC>CCERR.H.40	<C.KCC.CC>CCGEN.H.76	<C.KCC.CC>CCLEX.H.21
<C.KCC.CC>CCNODE.H.25	<C.KCC.CC>CCREG.H.17	<C.KCC.CC>CCSYM.H.126
<C.KCC.CC>CCTOKS.H.75	<C.KCC.CC>CCCODE.C.315	<C.KCC.CC>CCCREG.C.63
<C.KCC.CC>CCCSE.C.136	<C.KCC.CC>CCDECL.C.371	<C.KCC.CC>CCDUMP.C.142
<C.KCC.CC>CCERR.C.57	<C.KCC.CC>CCEVAL.C.24	<C.KCC.CC>CCFOLD.C.116
<C.KCC.CC>CCGEN.C.221	<C.KCC.CC>CCGEN1.C.198	<C.KCC.CC>CCGEN2.C.294
<C.KCC.CC>CCGSWI.C.52	<C.KCC.CC>CCINP.C.266	<C.KCC.CC>CCJSKP.C.101
<C.KCC.CC>CCLEX.C.150	<C.KCC.CC>CCOPT.C.328	<C.KCC.CC>CCREG.C.45
<C.KCC.CC>CCSTMT.C.340	<C.KCC.CC>CCSYM.C.165	<C.KCC.CC>CCTYPE.C.123
36 files searched, 29 without matches.
