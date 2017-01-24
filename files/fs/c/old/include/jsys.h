/*
 *	This is commonly-used JSYS stuff; jsys numbers and flags
 */

#include "c-env.h"

#define _PRIIN		0100		/* primary input */
#define _PRIOU		0101		/* primary output */
#define _CTTRM		0777777		/* controlling terminal */
#define _FHSLF		0400000		/* "self" process handle */
#define _FHTOP		0777776

#define FILEPART_SIZE	40		/* maximum size of a filespec part */
#define FILENAME_SIZE	(FILEPART_SIZE*4)

#define	OWNER_MASK	0770000			/* masks for prot field */
#define OWNER_OFFSET	12			/* shift right this much */
#define GROUP_MASK	0007700
#define GROUP_OFFSET	6
#define WORLD_MASK	0000077
#define WORLD_OFFSET	0

#define	FP_READ		040
#define	FP_WRITE	020
#define	FP_EXECUTE	010

#define RH		0000000777777
#define LH		0777777000000

#define T20_BIT(n)	(1 << (35 - n))

#define GJINF		013
#if SYS_10X
#define TIME		014
#endif
#define	GNJFN		017
#define GTJFN		020
#define	  GJ_FOU	T20_BIT(0)
#define	  GJ_NEW	T20_BIT(1)
#define	  GJ_OLD	T20_BIT(2)
#define   GJ_IFG	T20_BIT(11)
#define	  GJ_SHT	T20_BIT(17)
#define OPENF		021
#define	  OF_RD		T20_BIT(19)
#define	  OF_WR		T20_BIT(20)
#define	  OF_APP	T20_BIT(22)
#define   OF_PLN	T20_BIT(30)
#define CLOSF		022
#define RLJFN		023
#define DELF		026
#define   DF_EXP	T20_BIT(1)
#define SFPTR		027
#define	JFNS		030
#define RNAMF		035
#define SIZEF		036
#define RFPTR		043
#define GTFDB		063
#define   _FBCTL	1
#define     FB_NXF	T20_BIT(4)
#define	    FB_DIR	T20_BIT(7)
#define	  _FBBYV	011
#define	    FBBSZ_S	-24	/* shift left this much then */
#define	    FBBSZ_M	077	/* mask by this to get byte-size */
#define	  _FBADR	3
#define   _FBPRT	4
#define   _FBCRE	5
#define	  _FBUSE	6	/* 10X */ 
#define	  _FBSIZ	012
#define	  _FBCRV	013
#define	  _FBWRT	014
#define	  _FBREF	015
#define	  _FBLEN	037
#define	RFCOC 		0112
#define	SFCOC 		0113 
#define DVCHR		0117	/* get device characteristics */
#define   DV_TYP_S	-18	/* for device type, left-shift by this */
#define   DV_TYP_M	0777	/* and mask by this */
#define     _DVTTY	012
#define RPCAP		0150
#define GFRKS		0166
#define HALTF		0170
#define	ODTIM		0220
#define	  OT_DAY	T20_BIT(1)
#define	  OT_4YR	T20_BIT(5)
#define	  OT_DAM	T20_BIT(6)
#define	  OT_SPA	T20_BIT(7)
#define ODCNV		0222
#define   ICTMZ		077000000
#define   ICADS		T20_BIT(1)
#define   ICJUD		T20_BIT(3)
#define GTAD		0227	/* get time and date */
#define RSCAN		0500
#define   _RSINI	0
#define   _RSCNT	1
#define HPTIM		0501
#define   _HPELP	0
#define LNMST		0504	/* get definition of a logical name */
#define   _LNSJB	0
#define   _LNSSY	1
#define CHKAC		0521
#define   CK_JFN	T20_BIT(0)
#define	  _CKARD	0		/* Check read access */
#define	  _CKAWR	1		/* Check write access */
#define	  _CKAEX	2		/* Check execute access */
#define	  _CKAAC	0		/* Code of desired access to files */
#define	  _CKALD	1		/* User number */
#define	  _CKACD	2		/* Conn dir number */
#define	  _CKAEC	3		/* Enabled caps */
#define	  _CKAUD	4		/* JFN of file being accessed */
#define	  _CKAPR	5		/* File protection (not used) */
#define PRARG		0545
#define   _PRAST	2
#define     PRA_CCL	0	/* re-do last CCL command function */
#define     PRA_KEEP	1	/* keep fork */
#define	    PRA_KILL	2	/* kill fork */
#define     PRA_BACK	3	/* continue fork in the background */
#define GFUST		0550
#define   GFAUT		0
#define RCUSR		0554
#define   RC_NOM	T20_BIT(3)
#define	  RC_EMO	T20_BIT(17)
