/*
 *	This is T20/10X stuff: JSYSes, flags, process handles, etc.
 */

#ifndef _JSYS_INCLUDED
#define _JSYS_INCLUDED 1

#include "c-env.h"

/* Handy macros, not part of MONSYM.
**
**	FLD is as per MACSYM's FLD macro.
**	T20_BIT(x) is like MACRO 1Bx.
*/
#define FLD(bits,mask) ((((mask)&(-(mask)))*(bits))&(mask))
#define T20_BIT(n)	(1 << (35 - n))

#define RH		0000000777777
#define LH		0777777000000

#define FILEPART_SIZE	40		/* maximum size of a filespec part */
#define FILENAME_SIZE	(FILEPART_SIZE*4)

#define	T20_OWNER_MASK		0770000		/* masks for prot field */
#define T20_OWNER_OFFSET	12		/* shift right this much */
#define T20_GROUP_MASK		0007700
#define T20_GROUP_OFFSET	6
#define T20_WORLD_MASK		0000077
#define T20_WORLD_OFFSET	0

#define	FP_READ		040	/* file protection bits.  readable, */
#define	FP_WRITE	020	/* writeable, */
#define	FP_EXECUTE	010	/* executable. */

/* General-purpose definitions from MONSYM. */

/* I/O designators */
#define _PRIIN		0100		/* primary input */
#define _PRIOU		0101		/* primary output */
#define _CTTRM		0777777		/* controlling terminal */

/* Fork Handles */
#define _FHSLF		0400000		/* "self" process handle */
#define _FHSUP		0777777		/* Superior process handle */
#define _FHTOP		0777776		/* Top process in structure */
#define _FHINF		0777774		/* All inferiors of current process */

/*
 *	JFN mode word stuff
 */

#define TT_TAB		T20_BIT(2)
#define TT_LEN		037600000000		/* length */
#define TT_LEN_S	25			/* shift right this much */
#define TT_WID		000177000000		/* width */
#define TT_WID_S	18			/* likewise... */
#define TT_WAK		0170000
#define	TT_ECO		T20_BIT(24)
#define	TT_DAM		0300
#define	  _TTBIN	0000
#define	  _TTASC	0100
#define	  _TTATO	0200
#define	  _TTATE	0300
#define	TT_PGM		T20_BIT(34)

/*
 *	FDB stuff
 */

#define	_FBCTL		1
#define   FB_NXF	T20_BIT(4)
#define	  FB_DIR	T20_BIT(7)
#define	_FBBYV		011
#define	  FBBSZ_S	-24	/* shift left this much then */
#define	  FBBSZ_M	077	/* mask by this to get byte-size */
#define	_FBADR		3
#define	_FBPRT		4
#define	_FBCRE		5
#define	_FBUSE		6	/* 10X */ 
#define	_FBGEN		7
#define	_FBSIZ		012
#define	_FBCRV		013
#define	_FBWRT		014
#define	_FBREF		015
#define	_FBBBT		022
#define	_FBLEN		037

/* JSYS definitions, plus related bits, fields, etc.
 *	Each JSYS is defined by its JSYS# (in the RH) plus a class code,
 *	stored in the JSYS_CLASS field in the LH.
 *	JSYS_OKINT is a flag which can be added in a user call
 *	to jsys() which says the JSYS call is allowed to be interrupted
 *	by signals.
 *	For faster finding of the jsys you want,
 *	look for comments like *100*, *200*, etc.
 *
 *	The JSYS classes are:
 *
 *	class 0		jsys returns +1 always, generating an illegal
 *			instruction on error
 *	class 1		jsys returns +1 on error, +2 on win
 *	class 2		special class for ERSTR%
 *	class 3		special class for SIBE% and SOBE%
 *	class 4		special class for SOBF%
 */

/* DO NOT CHANGE THESE 3 DEFS WITHOUT CHANGING THE JSYS() ROUTINE! */
#define JSYS_CLASS		0070000000000
#define JSYS_OKINT		0400000000000
#define _DEFJS(num, class)	(FLD(num, RH) | FLD(class, JSYS_CLASS))

#define LOGIN		_DEFJS(1, 1)
#define CRJOB		_DEFJS(2, 1)
#define LGOUT		_DEFJS(3, 1)
#define CACCT		_DEFJS(4, 1)
/* JSYS 5, EFACT, not in monitor */
#define SMON		_DEFJS(6, 0)
#define TMON		_DEFJS(7, 0)
#define GETAB		_DEFJS(010, 1)
#define ERSTR		_DEFJS(011, 2)
#define GETER		_DEFJS(012, 0)
#define GJINF		_DEFJS(013, 0)
#define TIME		_DEFJS(014, 0)
#define RUNTM		_DEFJS(015, 0)
#define SYSGT		_DEFJS(016, 0)
#define	GNJFN		_DEFJS(017, 1)
#define GTJFN		_DEFJS(020, 1)
#define OPENF		_DEFJS(021, 1)
#define CLOSF		_DEFJS(022, 1)
#define RLJFN		_DEFJS(023, 1)
#define GTSTS		_DEFJS(024, 0)
#define STSTS		_DEFJS(025, 1)
#define DELF		_DEFJS(026, 1)
#define SFPTR		_DEFJS(027, 1)
#define	JFNS		_DEFJS(030, 0)
#define FFFFP		_DEFJS(031, 0)
/* JSYS 32, RDDIR, obsolete
   JSYS 33, CPRTF, not in monitor */
#define CLZFF		_DEFJS(034, 0)
#define RNAMF		_DEFJS(035, 1)
#define SIZEF		_DEFJS(036, 1)
#define GACTF		_DEFJS(037, 1)
/* JSYS 40, STDIR, obsolete */
#define DIRST		_DEFJS(041, 1)
#define BKJFN		_DEFJS(042, 1)
#define RFPTR		_DEFJS(043, 1)
/* JSYS 44, CNDIR, not in monitor */
#define RFBDZ		_DEFJS(045, 1)
#define SFBSZ		_DEFJS(046, 1)
#define SWJFN		_DEFJS(047, 0)
#define BIN		_DEFJS(050, 0)
#define BOUT		_DEFJS(051, 0)
#define SIN		_DEFJS(052, 0)
#define SOUT		_DEFJS(053, 0)
#define RIN		_DEFJS(054, 0)
#define ROUT		_DEFJS(055, 0)
#define PMAP		_DEFJS(056, 0)
#define RPACS		_DEFJS(057, 0)
#define SPACS		_DEFJS(060, 0)
#define RMAP		_DEFJS(061, 0)
#define SACTF		_DEFJS(062, 1)
#define GTFDB		_DEFJS(063, 0)
#define CHFDB		_DEFJS(064, 0)	/* change FDB */
#define DUMPI		_DEFJS(065, 1)
#define DUMPO		_DEFJS(066, 1)
#define DELDF		_DEFJS(067, 0)
#define ASND		_DEFJS(070, 1)
#define RELD		_DEFJS(071, 1)
/* JSYS 72, CSYNO, not in monitor */
#define PBIN		_DEFJS(073, 0)
#define PBOUT		_DEFJS(074, 0)
/* JSYS 75, PSIN, not in monitor */
#define PSOUT		_DEFJS(076, 0)
#define MTOPR		_DEFJS(077, 0)

/*   *100*   */

#define CFIBF		_DEFJS(0100, 0)
#define CFOBF		_DEFJS(0101, 0)
#define SIBE		_DEFJS(0102, 3)
#define SOBE		_DEFJS(0103, 3)
#define DOBE		_DEFJS(0104, 0)
/* JSYS 105, GTABS, obsolete
   JSYS 106, STABS, obsolete */
#define RFMOD		_DEFJS(0107, 0)
#define SFMOD		_DEFJS(0110, 0)
#define RFPOS		_DEFJS(0111, 0)
#define	RFCOC 		_DEFJS(0112, 0)
#define	SFCOC 		_DEFJS(0113, 0)
#define STI		_DEFJS(0114, 0)
#define DTACH		_DEFJS(0115, 0)
#define ATACH		_DEFJS(0116, 1)
#define DVCHR		_DEFJS(0117, 0)
#define STDEV		_DEFJS(0120, 1)
#define DEVST		_DEFJS(0121, 1)
/* JSYS 122, MOUNT, obsolete
   JSYS 123, DSMNT, obsolete
   JSYS 124, INIDR, obsolete */
#define SIR		_DEFJS(0125, 0)
#define EIR		_DEFJS(0126, 0)
#define SKPIR		_DEFJS(0127, 1)
#define DIR		_DEFJS(0130, 0)
#define AIC		_DEFJS(0131, 0)
#define IIC		_DEFJS(0132, 0)
#define DIC		_DEFJS(0133, 0)
#define RCM		_DEFJS(0134, 0)
#define RWM		_DEFJS(0135, 0)
#define DEBRK		_DEFJS(0136, 0)
#define ATI		_DEFJS(0137, 0)
#define DTI		_DEFJS(0140, 0)
#define CIS		_DEFJS(0141, 0)
#define SIRCM		_DEFJS(0142, 0)
#define RIRCM		_DEFJS(0143, 0)
#define RIR		_DEFJS(0144, 0)
#define GDSTS		_DEFJS(0145, 0)
#define SDSTS		_DEFJS(0146, 0)
#define RESET		_DEFJS(0147, 0)
#define RPCAP		_DEFJS(0150, 0)
#define EPCAP		_DEFJS(0151, 0)
#define CFORK		_DEFJS(0152, 1)
#define KFORK		_DEFJS(0153, 0)
#define FFORK		_DEFJS(0154, 0)
#define RFORK		_DEFJS(0155, 0)
#define RFSTS		_DEFJS(0156, 0)
#define SFORK		_DEFJS(0157, 0)
#define SFACS		_DEFJS(0160, 0)
#define RFACS		_DEFJS(0161, 0)
#define HFORK		_DEFJS(0162, 0)
#define WFORK		_DEFJS(0163, 0)
#define GFRKH		_DEFJS(0164, 1)
#define RFRKH		_DEFJS(0165, 1)
#define GFRKS		_DEFJS(0166, 1)
#define DISMS		_DEFJS(0167, 0)
#define HALTF		_DEFJS(0170, 0)
#define GTRPW		_DEFJS(0171, 0)
#define GTRPI		_DEFJS(0172, 0)
#define RTIW		_DEFJS(0173, 0)
#define STIW		_DEFJS(0174, 0)
#define SOBF		_DEFJS(0175, 4)
#define RSWET		_DEFJS(0176, 0)
#define GETNM		_DEFJS(0177, 0)

/*   *200*   */

#define GET		_DEFJS(0200, 0)
#define SFRKV		_DEFJS(0201, 0)
#define SAVE		_DEFJS(0202, 0)
#define SSAVE		_DEFJS(0203, 0)
#define SEVEC		_DEFJS(0204, 0)
#define GEVEC		_DEFJS(0205, 0)
#define GPJFN		_DEFJS(0206, 0)
#define SPJFN		_DEFJS(0207, 0)
#define SETNM		_DEFJS(0210, 0)
#define FFUFP		_DEFJS(0211, 1)
#define DIBE		_DEFJS(0212, 0)
/* JSYS 213, FDFRE, obsolete */
#define GDSKC		_DEFJS(0214, 0)
/* JSYS 215, LITES, not in monitor */
#define TLINK		_DEFJS(0216, 1)
#define STPAR		_DEFJS(0217, 0)
#define	ODTIM		_DEFJS(0220, 0)
#define IDTIM		_DEFJS(0221, 1)
#define ODCNV		_DEFJS(0222, 0)
#define IDCNV		_DEFJS(0223, 1)
#define NOUT		_DEFJS(0224, 1)
#define NIN		_DEFJS(0225, 1)
#define STAD		_DEFJS(0226, 1)
#define GTAD		_DEFJS(0227, 0)
#define ODTNC		_DEFJS(0230, 0)
#define IDTNC		_DEFJS(0231, 1)
#define FLIN		_DEFJS(0232, 1)
#define FLOUT		_DEFJS(0233, 1)
#define DFIN		_DEFJS(0234, 1)
#define DFOUT		_DEFJS(0235, 1)
/* no jsys' 236, 237 */
#define CRDIR		_DEFJS(0240, 0)
#define GTDIR		_DEFJS(0241, 0)
#define DSKOP		_DEFJS(0242, 0)
#define SPRIW		_DEFJS(0243, 0)
#define DSKAS		_DEFJS(0244, 1)
#define SJPRI		_DEFJS(0245, 0)
#define STO		_DEFJS(0246, 0)
#define ARCF		_DEFJS(0247, 0)
/* no jsys' 250-257
   JSYS 260, ASNDP, not in monitor
   JSYS 261, RELDP, not in monitor
   JSYS 262, ASNDC, not in monitor
   JSYS 263, RELDC, not in monitor
   JSYS 264, STRDP, not in monitor
   JSYS 265, STPDP, not in monitor
   JSYS 266, STSDP, not in monitor
   JSYS 267, RDSDP, not in monitor
   JSYS 270, WATDP, not in monitor
   no jsys 271
   JSYS 272, GTNCP, not in monitor */
#define GTHST		_DEFJS(0273, 1)
#define ATNVT		_DEFJS(0274, 1)
/* JSYS 275, CVSKT, pup, not in monitor */
#define CVHST		_DEFJS(0276, 1)
/* JSYS 277, FLHST, not in monitor */

/*   *300*   */

#define GCVEC		_DEFJS(0300, 0)
#define SCVEC		_DEFJS(0301, 0)
#define STTYP		_DEFJS(0302, 0)
#define GTTYP		_DEFJS(0303, 0)
/* JSYS 304, BPT, obsolete */
#define GTDAL		_DEFJS(0305, 0)
#define WAIT		_DEFJS(0306, 0)
#define HSYS		_DEFJS(0307, 1)
#define USRIO		_DEFJS(0310, 1)
#define PEEK		_DEFJS(0311, 1)
#define MSFRK		_DEFJS(0312, 0)
#define ESOUT		_DEFJS(0313, 0)
#define SPLFK		_DEFJS(0314, 1)
/* JSYS 315, ADVIS, not in monitor
   JSYS 316, JOBTM, not in monitor */
#define DELNF		_DEFJS(0317, 1)
/* JSYS 320, SWTCH, obsolete */
#define TFORK		_DEFJS(0321, 0)
#define RTFRK		_DEFJS(0322, 0)
#define UTFRK		_DEFJS(0323, 0)
#define SCTTY		_DEFJS(0324, 0)
/* no jsys 325
   JSYS 326, OPRFN, obsolete
   no JSYS' 327-335
   JSYS 336, SETR, obsolete
   no JSYS' 337-377 */

/*   *400*   */

#define PUPI		_DEFJS(0441, 0)	/* pup only.  class? */
#define PUPO		_DEFJS(0442, 0)
#define PUPNM		_DEFJS(0443, 0)

/*   *500*   */

#define RSCAN		_DEFJS(0500, 1)
#define HPTIM		_DEFJS(0501, 1)
#define CRLNM		_DEFJS(0502, 1)
#define INLNM		_DEFJS(0503, 1)
#define LNMST		_DEFJS(0504, 1)
#define RDTXT		_DEFJS(0505, 1)
#define SETSN		_DEFJS(0506, 1)
#define GETJI		_DEFJS(0507, 1)
#define MSEND		_DEFJS(0510, 1)
#define MRECV		_DEFJS(0511, 1)
#define MUTIL		_DEFJS(0512, 1)
#define ENQ		_DEFJS(0513, 1)
#define DEQ		_DEFJS(0514, 1)
#define ENQC		_DEFJS(0515, 1)
#define SNOOP		_DEFJS(0516, 1)
#define SPOOL		_DEFJS(0517, 1)
#define ALLOC		_DEFJS(0520, 1)
#define CHKAC		_DEFJS(0521, 1)
#define TIMER		_DEFJS(0522, 1)
#define RDTTY		_DEFJS(0523, 1)
#define TEXTI		_DEFJS(0524, 1)
#define UFPGS		_DEFJS(0525, 1)
#define SFPOS		_DEFJS(0526, 0)
#define SYERR		_DEFJS(0527, 0)
#define DIAG		_DEFJS(0530, 1)
#define SINR		_DEFJS(0531, 0)
#define SOUTR		_DEFJS(0532, 0)
#define RFTAD		_DEFJS(0533, 0)
#define SFTAD		_DEFJS(0534, 0)
#define TBDEL		_DEFJS(0535, 0)
#define TBADD		_DEFJS(0536, 0)
#define TBLUK		_DEFJS(0537, 0)
#define STCMP		_DEFJS(0540, 0)
#define SETJB		_DEFJS(0541, 0)
#define GDVEC		_DEFJS(0542, 0)
#define SDVEC		_DEFJS(0543, 0)
#define COMND		_DEFJS(0544, 0)
#define   _CMKEY	0
#define   _CMNUM	1
#define   _CMNOI	2
#define   _CMSWI	3
#define   _CMIFI	4
#define   _CMOFI	5
#define   _CMFIL	6
#define   _CMFLD	7
#define   _CMCFM	010
#define   _CMDIR	011
#define   _CMUSE	012
#define   _CMCMA	013
#define   _CMINI	014
#define   _CMFLT	015
#define   _CMDEV	016
#define   _CMTXT	017
#define   _CMTAD	020
#define   _CMQST	021
#define   _CMUQS	022
#define   _CMTOK	023
#define   _CMNUX	024
#define   _CMACT	025
#define   _CMNOD	026
#define   CM_NSF	T20_BIT(12)
#define   CM_BRK	T20_BIT(13)
#define   CM_PO		T20_BIT(14)
#define   CM_HPP	T20_BIT(15)
#define   CM_DPP	T20_BIT(16)
#define   CM_SDH	T20_BIT(17)
#define   CM_ESC	T20_BIT(0)
#define   CM_NOP	T20_BIT(1)
#define   CM_EOC	T20_BIT(2)
#define   CM_RPT	T20_BIT(3)
#define   CM_SWT	T20_BIT(4)
#define   CM_PFE	T20_BIT(5)
#define PRARG		_DEFJS(0545, 0)
#define GACCT		_DEFJS(0546, 0)
#define LPINI		_DEFJS(0547, 0)
#define GFUST		_DEFJS(0550, 0)
#define SFUST		_DEFJS(0551, 0)
#define ACCES		_DEFJS(0552, 0)
#define RCDIR		_DEFJS(0553, 0)
#define RCUSR		_DEFJS(0554, 0)
#define MSTR		_DEFJS(0555, 0)
#define STPPN		_DEFJS(0556, 0)
#define PPNST		_DEFJS(0557, 0)
#define PMCTL		_DEFJS(0560, 0)
#define PLOCK		_DEFJS(0561, 0)
#define BOOT		_DEFJS(0562, 0)
#define UTEST		_DEFJS(0563, 0)
#define USAGE		_DEFJS(0564, 0)
#define WILD		_DEFJS(0565, 0)
#define VACCT		_DEFJS(0566, 0)
#define NODE		_DEFJS(0567, 0)
#define ADBRK		_DEFJS(0570, 0)
/* JSYS 571, SINM, undefined if no ATS support
   JSYS 572, SOUTM, ditto */
#define SWTRP		_DEFJS(0573, 0)
#define GETOK		_DEFJS(0574, 0)
#define RCVOK		_DEFJS(0575, 0)
#define GIVOK		_DEFJS(0576, 0)
#define SKED		_DEFJS(0577, 0)

/*   *600*    */

#define MTU		_DEFJS(0600, 0)
#define XRIR		_DEFJS(0601, 0)
#define XSIR		_DEFJS(0602, 0)
#define PDVOP		_DEFJS(0603, 0)
#define NTMAN		_DEFJS(0604, 0)
#define XSFRK		_DEFJS(0605, 0)
#define XGVEC		_DEFJS(0606, 0)
#define XSVEC		_DEFJS(0607, 0)
#define RSMAP		_DEFJS(0610, 0)
#define XRMAP		_DEFJS(0611, 0)
#define XGTPW		_DEFJS(0612, 0)
#define XSSEV		_DEFJS(0613, 0)
#define XGSEV		_DEFJS(0614, 0)
#define QUEUE		_DEFJS(0615, 0)
/* JSYS 616, DYNLB DYNamic LiBrary.  Huh?
   JSYS 617, CTSOP, Canonical Terminal Support OPeration.  Huh?
   JSYS 620, DAP, 6.0 Data Access Protocol.  Huh?
   no JSYS 621 */
#define SCS		_DEFJS(0622, 0)
#define WSMGR		_DEFJS(0623, 0)
#define LLMOP		_DEFJS(0624, 0)
/* JSYS 625, APCON, not in monitor */
#define XPEEK		_DEFJS(0626, 0)
/* JSYS 627, CNFIG, huh? */
#define NI		_DEFJS(0630, 0)
#define LATOP		_DEFJS(0631, 0)
#define NTINF		_DEFJS(0632, 0)
/* no JSYS 633
   JSYS 634, GTBLT, MIT multiple GETAB
   no JSYS' 635-677 */

/*   *700*   */

#define HANDS		_DEFJS(0700, 0)	/* class? */
#define IDLE		_DEFJS(0701, 0)	/* class? */
/* JSYS 702, GTWAA, LOTS thing
   JSYS 703, PKOPR, more stanford
   no JSYS' 704-716 */
#define MONRD		_DEFJS(0717, 0)	/* class? */
#define DBGRD		_DEFJS(0720, 0)	/* more MONRD.  class? */
/* no JSYS' 721-737 */
#define SEND		_DEFJS(0740, 1)
#define RECV		_DEFJS(0741, 1)
#define OPEN		_DEFJS(0742, 1)
#define CLOSE		_DEFJS(0743, 1)
#define SCSLV		_DEFJS(0744, 1)
#define STAT		_DEFJS(0745, 1)
#define CHANL		_DEFJS(0746, 1)
#define ABORT		_DEFJS(0747, 1)
#define SNDIM		_DEFJS(0750, 1)
#define RCVIM		_DEFJS(0751, 1)
#define ASNSQ		_DEFJS(0752, 1)
#define RELSQ		_DEFJS(0753, 0)
#define SNDIN		_DEFJS(0754, 1)
#define RCVIN		_DEFJS(0755, 1)
#define ASNIQ		_DEFJS(0756, 1)
#define RELIQ		_DEFJS(0757, 1)
#define IPOPR		_DEFJS(0760, 0)
#define TCOPR		_DEFJS(0761, 0)
#define   _TCPSH	2		/* push data */
/* no JSYS' 762-764 */
#define GTDOM		_DEFJS(0765, 1)
#define METER		_DEFJS(0766, 0)
#define SMAP		_DEFJS(0767, 0)
#define THIBR		_DEFJS(0770, 1)
#define TWAKE		_DEFJS(0771, 1)
/* JSYS 772, MRPAC, huh?
   JSYS 773, SETPV, not in monitor */
#define MTALN		_DEFJS(0774, 0)
#define TTMSG		_DEFJS(0775, 0)
/* no JSYS 776 */
#define MDDT		_DEFJS(0777, 0)

/*
 *	per-JSYS bits and syms, in (approx) alpha order of JSYS
 */

/*    ACCES    */

#define	AC_CON		T20_BIT(0)

/* CFORK */

#define CR_MAP	T20_BIT(0)
#define CR_CAP	T20_BIT(1)
#define CR_ACS	T20_BIT(3)
#define CR_ST	T20_BIT(4)

/*    CHFDB   */

#define _CFNUD		T20_BIT(0)

/*    CHKAC    */

#define	CK_JFN		T20_BIT(0)
#define	  _CKARD	0		/* Check read access */
#define	  _CKAWR	1		/* Check write access */
#define	  _CKAEX	2		/* Check execute access */
#define	  _CKAAC	0		/* Code of desired access to files */
#define	  _CKALD	1		/* User number */
#define	  _CKACD	2		/* Conn dir number */
#define	  _CKAEC	3		/* Enabled caps */
#define	  _CKAUD	4		/* JFN of file being accessed */
#define	  _CKAPR	5		/* File protection (not used) */

/*    DELF    */

#define	DF_EXP		T20_BIT(1)

/*    DVCHR    */

#define	DV_TYP_S	18	/* for device type, right-shift by this */
#define	DV_TYP_M	0777	/* and mask by this */
#define   _DVDSK	0	/* disk */
#define   _DVMTA	02	/* magtape */
#define   _DVLPT	07	/* lineprinter */
#define   _DVCDR	010	/* card reader (<laughter!>) */
#define   _DVFE		011	/* front-end psuedo-device */
#define   _DVTTY	012	/* TTY */
#define   _DVPTY	013	/* psuedo-TTY */
#define   _DVNUL	015	/* null device */
#define   _DVNET	016	/* old ARPAnet device code */
#define   _DVDCN	022	/* DECnet active component */
#define   _DVSRV	023	/* DECnet passive component */
#define   _DVTCP	025	/* TCP */
#define   _DVPIP	0403	/* pipe device */

/* GFRKS */

#define GF_GFH	T20_BIT(0)
#define GF_GFS	T20_BIT(1)

/*    GFUST    */

#define	_GFAUT		0

/*    GTHST    */

#define _GTHIX		0
#define	_GTHNS		2
#define	_GTHSN		3

/*    GTJFN    */

#define	GJ_FOU		T20_BIT(0)
#define	GJ_NEW		T20_BIT(1)
#define	GJ_OLD		T20_BIT(2)
#define	GJ_IFG		T20_BIT(11)
#define	GJ_XTN		T20_BIT(15)
#define	GJ_SHT		T20_BIT(17)

/* GTSTS */

#define GS_EOF	T20_BIT(8)

/*    HPTIM    */

#define	_HPELP		0

/*    IDCNV    */

#define	IC_TMZ		077000000
#define	IC_ADS		T20_BIT(1)
#define	IC_JUD		T20_BIT(3)

/*    IDTIM    */

#define	IT_NDA		T20_BIT(0)	/* do not input date */
#define	IT_NTI		T20_BIT(6)	/* do not input time */

/*    LNMST    */

#define	_LNSJB		0
#define	_LNSSY		1

/*    MTOPR    */

#define	_MOSPD		026	/* set TTY line speed */
#define	_MORSP		027	/* read TTY line speed */
#define _MORLW		030	/* Read TTY line width */
#define _MOSLW		031	/* Set TTY line width */
#define _MORLL		032	/* Read TTY page height */
#define _MOSLL		033	/* Set TTY page height */



/*    MUTIL    */

#define	_MUCRE		6	/* Create a PID */

/*    ODTIM    */

#define	OT_DAY		T20_BIT(1)
#define OT_FDY		T20_BIT(2)
#define OT_NMN		T20_BIT(3)
#define OT_FMN		T20_BIT(4)
#define	OT_4YR		T20_BIT(5)
#define	OT_DAM		T20_BIT(6)
#define	OT_SPA		T20_BIT(7)
#define OT_SLA		T20_BIT(8)
#define	OT_NTM		T20_BIT(9)
#define OT_NSC		T20_BIT(10)
#define OT_12H		T20_BIT(11)
#define OT_NCO		T20_BIT(12)
#define OT_TMZ		T20_BIT(13)
#define	OT_SCL		T20_BIT(17)

/*    OPENF    */

#define	OF_RD		T20_BIT(19)
#define	OF_WR		T20_BIT(20)
#define	OF_APP		T20_BIT(22)
#define	OF_THW		T20_BIT(25)
#define	OF_PDT		T20_BIT(27)
#define	OF_PLN		T20_BIT(30)

/*    PMAP    */

#define	PM_CNT		T20_BIT(0)
#define	PM_RD		T20_BIT(2)
#define	PM_WR		T20_BIT(3)

/*    PRARG    */

#define	_PRAST		2
#define   PRA_CCL	0	/* re-do last CCL command function */
#define   PRA_KEEP	1	/* keep fork */
#define	  PRA_KILL	2	/* kill fork */
#define   PRA_BACK	3	/* continue fork in the background */

/*    RCUSR    */

#define	RC_NOM		T20_BIT(3)
#define	RC_EMO		T20_BIT(17)

/* RFSTS */

#define _RFRUN 0
#define _RFIO  1
#define _RFHLT 2
#define _RFFPT 3
#define _RFWAT 4
#define _RFSLP 5
#define _RFTRP 6
#define _RFABK 7

/*    RSCAN    */

#define	_RSINI		0
#define	_RSCNT		1

/*    STIW    */

#define	ST_DIM		T20_BIT(0)

/* TEXTI */

#define RD_BRK	T20_BIT(0)
#define RD_TOP	T20_BIT(1)
#define RD_PUN	T20_BIT(2)
#define RD_BEL	T20_BIT(3)
#define RD_CRF	T20_BIT(4)
#define RD_RND	T20_BIT(5)
#define RD_JFN	T20_BIT(6)
#define RD_RIE	T20_BIT(7)
#define RD_BBG	T20_BIT(8)
#define RD_RAI	T20_BIT(10)
#define RD_SUI	T20_BIT(11)
#define RD_BTM	T20_BIT(12)
#define RD_BFE	T20_BIT(13)
#define RD_BLR	T20_BIT(14)

/*    TIMER   */

#define _TIMRT	0
#define _TIMEL	1
#define _TIMDT	2
#define _TIMDD	3
#define _TIMBF	4
#define _TIMAL	5

/*
 *	Standard DEC terminal type codes
 */

#define _TT33	0		/* MODEL 33 */
#define _TT35	1		/* MODEL 35 */
#define _TT37	2		/* MODEL 37 */
#define _TTEXE	3		/* EXECUPORT */
#define _TTDEF	8		/* DEFAULT */
#define _TTIDL	9		/* IDEAL */
#define _TTV05	10		/* VT05 */
#define _TTV50	11		/* VT50 */
#define _TTL30	12		/* LA30 */
#define _TTG40	13		/* GT40 */
#define _TTL36	14		/* LA36 */
#define _TTV52	15		/* VT52 */
#define _TT100	16		/* VT100 */
#define _TTL38	17		/* LA38 */
#define _TT120	18		/* LA120 */
#define _TT125	35		/* VT125 */
#define _TTK10	36		/* VK100 - GIGI */
#define _TT102	37		/* VT102 */
#define _TTH19	38		/* H19 */ /* Conflicts with Stanford sym! */
#define _TT131	39		/* VT131 */
#define _TT200	40		/* VT200 */

/*
 *	Local definitions.  These are for Stanford and SRI-NIC
 */

#define _TTADM	4		/* LSI ADM-3 */
#define _TTDAM	5		/* DATAMEDIA 2500 */
#define _TTHP	6		/* HP2645 ETC. */
#define _TTHAZ	7		/* VIRGIN HAZELTINE 1500 */
#define _TT43	19		/* TTY MODEL 43 */
#define _TTSRC	20		/* SOROC 120 */
#define _TTGIL	21		/* GILLOTINE */
#define _TTTEL	22		/* TELERAY 1061 */
#define _TTTEK	23		/* TEKTRONIX 4025 */
#define _TTANN	24		/* ANN ARBOR */
#undef _TTH19		/* Flush DEC definition */
#define _TTH19	25		/* HEATH H19 */
#define _TTCPT	26		/* CONCEPT 100 */
#define _TTIBM	27		/* IBM 3101-20 */
#define _TTTVI	28		/* TELEVIDEO-912 */
#define _TTTK3	29		/* TEKTRONIX 4023 */
#define _TTDM2	30		/* DATAMEDIA 1520 */
#define _TTAMB	31		/* AMBASSADOR */
#define _TTESP	32		/* ESPRIT */
#define _TTFRD	33		/* FREEDOM-100 */
#define _TTFR2	34		/* FREEDOM-200 */
#define _TTANS	41		/* ANSI STANDARD */
#define _TTAVT	42		/* CONCEPT AVT */

#endif /* #ifndef _JSYS_INCLUDED */
