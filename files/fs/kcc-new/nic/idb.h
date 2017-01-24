/* IDB.H - NIC Ifpack Data Base definitions.
**
**	See nicprog/cifp.doc and nicprog/idb.doc for complete documentation.
**
**	This file contains definitions and declarations for using
** IDB, the C version of the NIC IFPACK DataBase package.  These definitions
** should be the only things that a user program needs or has access to.
**	Internal package defs are kept in "idbdef.h" in the source directory.
**	CIFP compatibility defs are in <nic/ifpack.h>.
*/

#ifndef _IDB_INCLUDED	/* Only include once */
#define _IDB_INCLUDED 1

/* System-dependent stuff. */

#include <c-env.h>		/* Find what system is in use */

#if CPU_PDP10

/* Ensure 6-char symbol uniqueness for PDP-10.
**	Note that all function symbols are compressed without a "_", but
**	all variable symbols retain it.
*/

/* External variables */
#define id_abortf id_abf /* Set non-zero by user to abort IFPACK loops */
#define id_updflg id_upf /* Non-zero if database being modified */
#define id_errflg id_erf /* Non-zero if database has internal errors */
#define id_errstr id_ers /* Error msg, for some failing routines. */
#define id_recidx id_rix /* Rec idx for current rec (-1 if new, 0 if none) */

/* User-visible routines */
#define id_pkginit	idinit	/* Init package */
#define id_pkgreset	idrese	/* Reset package */

#define id_dbselect	idbsel	/* Select database */
#define id_dbget	idbget	/* Load database for reading */
#define id_dbgetupd	idbgtu	/* Allow updates to database */
#define id_dbcreate	idbcre	/* Create new empty database */
#define id_dbput	idbput	/* Make all updates permanent */
#define id_dbstat	idbsta	/* Return full status of current DB */
#define id_dbvstat	idbvst	/* Return variable-only status of DB */
#define id_dblock	idblok	/* Lock DB (indep of write-lock) */
#define id_dbunlock	idbulk	/* Unlock DB */
#define id_dbgc		idbgc	/* GC and compact database */
#define id_dbtrim	idbtrm	/* Trim DB files */
#define id_dbbackup	idbbak	/* Create duplicate backup database */
#define id_dbcopy	idbcop	/* Copy DB to new specified name. */

#define id_ddfget	iddfgt	/* Get database definition info */
#define id_ddfput	iddfpt	/* Modify database definition */

#define id_idfget	ididgt	/* Get item definition */
#define id_idfput	ididpt	/* Change/add item definition */
#define id_idfdel	ididdl	/* Delete item definition */
#define	id_idfnamechg	ididnc	/* Change item names in an IDF */
#define id_idftostr	ididts	/* Represent IDF as string */
#define id_idffrstr	ididfs	/* Build IDF from string */
#define	id_idfupdget	ididug	/* Get the update-item number if any */
#define	id_idflnkget	ididlg	/* Get the link-item numbers if any */
#define id_idfsynnext	ididsn	/* Get next item synonym */

#define id_idfnum	ididno	/* Get item # for item name */
#define id_idfname	ididna	/* Get official name item # */
#define id_idfflags	ididfl	/* Get flags for item */
#define id_idfissort	ididst	/* True if item has sort-table */
#define id_idfissi	ididsi	/* True if item has subitems */

#define id_ifltostr	idiflt	/* Convert flags into flagchars string */
#define id_iflfrstr	idiflf	/* Convert flagchars string into flags */
	/* id_iflis*() are all macros */

#define id_stefind	idstef	/* Find STE (sort-table entry) */
#define id_stenext	idsten	/* Get next STE */
#define id_steins	idstei	/* Insert STE (maintenance) */
#define id_stedel	idsted	/* Delete STE (maintenance) */
#define	id_rixnext	idrixn	/* Get next record index */
#define	id_rixprev	idrixp	/* Get previous record index */

#define id_recget	idrecg	/* Get record for index */
#define id_recgethdl	idrech	/* Get record for handle */
#define id_recgetitm	idreci	/* Get record for (shd be unique) item */
#define id_recfind	idrecf	/* Find and get record (stefind+recget) */
#define id_recnext	idrecn	/* Get next record (stenext+recget) */
#define id_recprev	idrecm	/* Get prev record (steprev+recget) */
#define id_reccreate	idrecc	/* Create empty current record */
#define id_recput	idrecp	/* Write out current record */
#define	id_recputraw	idrecr	/* ditto w/o checking "update" or "links" */
#define	id_recdel	idrecd	/* Delete current record */
#define	id_recvfy	idrecv	/* Verify links for current record */

#define id_itmget	iditmg	/* Get item in current record */
#define id_itmnext	iditmn	/* Get next item */
#define id_itmput	iditmp	/* Put item into current record */
#define id_itmdel	iditmd	/* Delete item from current rec */
#define id_itmapp	iditma	/* Append to item */
#define id_itmismod	iditmw	/* TRUE if item modified in cur rec */
#define id_itmcmp	iditmc	/* Compare item with string, per item flags */
#define	id_itmmatch	iditmm	/* TRUE if string completely matches item */
#define	id_itmsimatch	idisim	/* TRUE if find subitem in item */
#define	id_itmsiapp	idisia	/* Append "str" subitem to item */
#define	id_itmsiadd	idisib	/* Add "str" subitem to item if not there */
#define	id_itmsidel	idisid	/* Delete "str" subitem from itm if there */
#define	id_itmwsiadd	idwsia	/* Add "str" wd-subitm to item if not there */
#define	id_itmwsidel	idwsid	/* Delete "str" wd-subitm from itm if there */

#define	id_ssiget	idssig	/* Get next String-subitem */
#define	id_lsiget	idlsig	/* Get next Line-subitem */
#define	id_wsiget	idwsig	/* Get next Word-subitem */
#define	id_siget	idsigt	/* Get next subitem from str */

#define id_fndstrget	idfsgt	/* Get canonicalized target string */
#define	id_radget	idradg	/* Get record address */
#define	id_stafprint	idstap	/* Outputs status info to stream */
#define	id_stfmake	idstfm	/* Remake sort-table file */
#define id_stfname	idstfn	/* Get sort-table filename */
#define	id_stfgc	idstfg	/* GC the sort-table for specified item */
#define	id_rcfgc	idrcfg	/* GC the record file only */

#define	id_stfxck	idstfx	/* Cross-chk STF */
#define	id_rcfxck	idrcfx	/* Cross-chk all records vs STFs */
#define	id_recxck	idrecx	/* Cross-chk current record vs STFs */
#define	id_itmxck	iditmx	/* Cross-chk current record item vs STF */
#define	id_ptfprint	idptfp	/* Output dump of specified PT */

/* String-handling functions */
#define id_fputs	idsfpt	/* Output idstr_t to stream */
#define id_strfs	idsfs	/* Create idstr_t from C str */
#define id_strfmem	idsfme	/* Create idstr_t from char ptr & cnt */
#define	id_strscat	idssct	/* Append to C-string from str, null-term */
#define	id_strscpy	idsscy	/* Copy   to C-string from str, null-term */
#define	id_strmcat	idsmct	/* Append to cp from str */
#define	id_strmcpy	idsmcy	/* Copy   to cp from str */
#define	id_strcats	idscts	/* Append to str from C-string */
#define	id_strcpys	idscys	/* Copy   to str from C-string */
#define	id_strsncat	idssnt	/* App <= N chs to C-str from str, null-term */
#define	id_strsncpy	idssny	/* Copy N chars to C-str from str, null-pads */
#define	id_strncats	idsnts	/* App <= N chars to str from C-string */
#define	id_strncpys	idsnys	/* Copy N chars to str from C-string */
#define	id_strbcpy	idsbcp	/* Copies to buffer from str, updates buf. */
#define	id_strbcpys	idsbcs	/* Ditto, from C-string. */
#define	id_strbprintf	idsbpf	/* Printf-style output to buf */
#define	id_strscmp	idsscm	/* Compares C-string with str */
#define	id_strsncmp	idssnc	/* Compares C-string with str, up to N chars */
#define	id_strmatch	idsmtc	/* TRUE if s1 matches s2 exactly */

#define id_strcat	idscat	/* Appends s2 to s1, returns new idstr_t */
#define	id_strncat	idsnca	/* Ditto but only up to N chars. */
#define	id_strcpy	idscpy	/* Copies str2 to str1 (ignores dest cnt). */
#define	id_strncpy	idsncp	/* Writes N chars from str2 to str1. */
#define	id_strcmp	idscmp	/* Compares two idstr_ts like strcmp. */
#define	id_strncmp	idsncm	/* Compares two idstr_ts like strncmp. */
#define	id_strchr	idschr	/* Finds c, returns rem of str (c 1st char) */
#define	id_strchr1	idsch1	/* Same but returns 1st-part (not rem). */
#define	id_strpos	idspos	/* Finds c, returns char pos. */
#define	id_strrchr	idsrch	/* Like id_strchr but finds last c. */
#define	id_strrchr1	idsrc1	/* Same but returns 1st-part (not rem). */
#define	id_strrpos	idsrpo	/* Like id_strpos but finds last c. */
#define	id_strspn	idsspn	/* Skips setstr chars, returns char pos. */
#define	id_strcspn	idscsp	/* Like id_strpos for any chars in setstr. */
#define	id_strpbrk	idspbr	/* Like id_strspn, returns rem of str. */
#define	id_strpbrk1	idspb1	/* Same but returns 1st-part (not rem). */
#define	id_strrpbrk	idsrpb	/* Like id_strcspn, returns rem of str. */
#define	id_strrpbrk1	idsrp1	/* Same but returns 1st-part (not rem). */
#define	id_strstr	idsstr	/* Should act like id_strchr. */
#define	id_strtok	idstok	/* Returns idstr_t describing token. */

		/* Case-independent versions of above */
#define	id_strCMP	idsucm
#define	id_strnCMP	idsunc
#define	id_strCHR	idsuch
#define	id_strCHR1	idsuc1
#define	id_strPOS	idsups
#define	id_strrCHR	idsurc
#define	id_strrCHR1	idsur1
#define	id_strrPOS	idsurp
#define	id_strSTR	idsust
#define	id_strsCMP	idsusc
#define	id_strsnCMP	idsusd
#define	id_strMATCH	idsumt

#else /* not CPU_PDP10 */
	/* Assume using 8-char case-dependent symbols. */

#define id_dbgetupd	id_dbgtu	/* vs id_dbget */
#define id_idfissort	id_idfst
#define id_idfissi	id_idfsi
#define id_idfnamechg	id_idfnc	/* vs id_idfname */
#define id_recget	id_recgt
#define id_recgethdl	id_recgh
#define id_recgetitm	id_recgi
#define id_recput	id_recpt
#define	id_recputraw	id_recpr
#define	id_itmsimatch	id_itmsm
#define	id_itmsiapp	id_itmsa
#define	id_itmsiadd	id_itmsb
#define	id_itmsidel	id_itmsd
#define	id_itmwsiadd	id_itmwa
#define	id_itmwsidel	id_itmwd

/* String-handling functions */
#define id_strfs	id_sfs
#define id_strfmem	id_sfmem
#define	id_strscat	id_sscat
#define	id_strscpy	id_sscpy
#define	id_strmcat	id_smcat
#define	id_strmcpy	id_smcpy
#define	id_strcats	id_scats
#define	id_strcpys	id_scpys
#define	id_strsncat	id_ssnct
#define	id_strsncpy	id_ssncy
#define	id_strncats	id_sncts
#define	id_strncpys	id_sncys
#define	id_strbcpy	id_sbcpy
#define	id_strbcpys	id_sbcps
#define	id_strbprintf	id_sbprf
#define	id_strscmp	id_sscmp
#define	id_strsncmp	id_ssncm
#define	id_strmatch	id_smtch

#define	id_strcat	id_scat	
#define	id_strncat	id_sncat
#define	id_strcpy	id_scpy
#define	id_strncpy	id_sncpy
#define	id_strcmp	id_scmp
#define	id_strncmp	id_sncmp
#define	id_strchr	id_schr
#define	id_strchr1	id_schr1
#define	id_strpos	id_spos
#define	id_strrchr	id_srchr
#define	id_strrchr1	id_srch1
#define	id_strrpos	id_srpos
#define	id_strspn	id_sspn
#define	id_strcspn	id_scspn
#define	id_strpbrk	id_spbrk
#define	id_strpbrk1	id_spbr1
#define	id_strrpbrk	id_srpbr
#define	id_strrpbrk1	id_srpb1
#define	id_strstr	id_sstr
#define	id_strtok	id_stok

#define	id_strCMP	id_sCMP
#define	id_strnCMP	id_snCMP
#define	id_strCHR	id_sCHR
#define	id_strCHR1	id_sCHR1
#define	id_strPOS	id_sPOS
#define	id_strrCHR	id_srCHR
#define	id_strrCHR1	id_srCH1
#define	id_strrPOS	id_srPOS
#define	id_strSTR	id_sSTR
#define	id_strsCMP	id_ssCMP
#define	id_strsnCMP	id_ssnCM
#define	id_strMATCH	id_sMTCH

#endif /* not T20 */

/* Typedefs and Constants  */

#include <time.h>		/* For time_t definition, in iddst_t */

#define ID_NITEMS (128)		/* # of items (power of 2) (cannot change) */
#define ID_ITM_MAX (126)	/* Highest valid item # (cannot change) */
#define ID_ITM_HANDLE 0		/* Item value for "handle" */
#define ID_ITM_RAT (-1)		/* Special pseudo-item value for RAT */
#define ID_RIX_NEW (-1)		/* RIX value for new (unwritten) record */
#define ID_DB_LATEST ((char *)0)  /* DB selection value to get latest ver */
#define ID_DB_SAME   ((char *)-1) /* DB selection val to keep current ver */
#define ID_DB_NONE   ((char *)-2) /* DB selection val to flush current sel */

/* Internal constants, user should not refer to these */
#define ID_NSYNS 10		/* # synonyms allowed for an item def */
#define ID_MAXSTECHS 2047	/* Max # chars in a sort-table entry str */

/* Typedefs for basic objects */
typedef int iditm_t;	/* Integer for holding item # (includes neg #s) */
typedef int idifl_t;	/* Integer for item flag bits */
typedef int idrix_t;	/* Integer for record index */
typedef long idrad_t;	/* Integer for record address */
typedef int idstx_t;	/* (internal) STE index value */

typedef struct { char *s_cp; int s_len; } idstr_t;	/* String */

typedef struct {
	idstr_t ddf_id;		/* Database ID string */
	} idddf_t;

typedef struct {
	char *dst_dbsel;	/* Selected DB (NULL if none selected) */
	int dst_ver;		/* Selected DB version # (0 if none spec'd) */
	int dst_ldflg;		/* TRUE if DB is loaded */
	int dst_lkflg;		/* TRUE if DB is locked (by us) */
	int dst_updflg;		/* TRUE if DB is being updated */
	int dst_obsflg;		/* TRUE if DB is obsolete (not most recent) */
	int dst_muid;		/* Last modifier User ID (-1 if unknown) */
	time_t dst_mtime;	/* Last modified time (0 if unknown) */
	int dst_lkuid;		/* User ID with write-lock (-1 if unknown) */
	time_t dst_lktime;	/* Write-locked time (0 if unknown) */
	} iddst_t;

typedef struct {
	iditm_t idf_num;	/* Item number */
	idifl_t idf_flags;	/* Item flags */
	idstr_t idf_name;	/* Official name */
	int     idf_nsyn;	/* # of synonyms */
	idstr_t idf_syns[ID_NSYNS];	/* Synonyms */
	} ididf_t;

typedef struct {
	int pkg_flags;		/* ID_PKGF_xxx */
	int pkg_mmupgs;		/* Max mem usage in # pages. */
	int pkg_mmupct;		/* Max mem usage as % of available pages. */
	int pkg_npgs;		/* Return: # pages actually allocated */
	int pkg_fpag;		/* Return: Page # of first allocated page */
	} idpkg_t;

typedef struct {
    int fnd_init;	/* >= 0 to do string init (set up context) */
			/* 	This uses fnd_str, fnd_itm, fnd_rix */
			/*	and sets up everything else below. */
			/* Flag definitions: */
#define ID_FNDF_NOREC	0100	/* Don't retrieve record unless necessary */
#define ID_FNDF_TRIX	0200	/* Target record index is in fnd_rix */
#define ID_FNDF_STX	010000	/* (internal) Current STX is already set */
#define ID_FNDF_CSD	020000	/* (internal) Case-dependent item */
#define ID_FNDF_SRT	040000	/* (internal) Sort-table item */
#define ID_FNDF_INITED WD_SIGN	/* (internal) Context initialized. */

#define ID_FNDF_TYPMASK (07)	/* Mask for search-type value */
			/* Search type, specified in fnd_ftyp */
#define ID_FNDT_FULL 0	/*	full match */
#define ID_FNDT_PREF 1	/*	prefix match */
#define ID_FNDT_SUBS 2	/*	substring match */
#define ID_FNDT_EXPR 3	/*	(not yet) pattern expression match */

    iditm_t fnd_itm;	/* Item # to search for */
    idstr_t fnd_str;	/* String to search for, & returned entry str val */
    idrix_t fnd_rix;	/* Recidx to search for, & returned entry recidx */
    idstx_t fnd_stx;	/* STE index to continue srch from (-1 for beg) */
    int     fnd_err;	/* Indicates failure reason for losing srch/step */

    iditm_t fnd_order;	/* Ordering to use, either RAT or ST item # */
    int fnd_typ;	/* Match type for easier dispatch */
    int fnd_pfmsk;	/* Last-word mask for prefix matching */
    int fnd_uprf;	/* 0 = case-dep match, else uppercase (case-indep) */
    int fnd_wds;	/* <# words> used in fnd_sbuf */
    int fnd_slen;	/* # chars in target string */

    /* Target-string buffer length must be large enough to hold largest
    ** possible item.  It contains bytes of type "char7".
    ** To access the target string, use the function
    **		idstr_t id_fndstrget(idfnd_t *);
    */
#define iduword_t unsigned int
    unsigned int fnd_sbuf[
	((ID_MAXSTECHS+sizeof(iduword_t)-1)/(sizeof(iduword_t)))
		+1];		/* Plus one for "fence" wd */
} idfnd_t;


/* Argument types for id_stafprint() */
enum {
	ID_STA_ALL=0,		/* This one should always be 0 */
	ID_STA_GENERAL,
	ID_STA_DATABASE,
	ID_STA_FILES,
	ID_STA_RAT,
	ID_STA_RECFILE,
	ID_STA_SORTTABLE,
	ID_STA_MEMORY
};

/* Argument types for id_stfname() */
enum {
	ID_STF_ACCESS=0,
	ID_STF_NEW_ACCESS,
	ID_STF_RECORDS,
	ID_STF_SORTTABLE
};

/* Flags for id_pkginit() */
#define ID_PKG_MMUPGS	01	/* Specified max # pages to use */
	/* ... */

/* Flags for id_stfxck() cross-check options, can be OR'd together */
#define ID_STFX_PGT	01	/* PGT internal consistency */
#define ID_STFX_SRT	02	/* STF  "  */
#define ID_STFX_REC	04	/* STF contents vs record contents */ 

/* Item flags (contained within an idifl_t) */
#define ID_ITMF_EXIST	01	/* Item exists */
#define ID_ITMF_SORT	02	/* Item has sort-table */
#define ID_ITMF_CASE	04	/* Item is case-dependent */
#define ID_ITMF_NUM	010	/* Item is numerical */
#define ID_ITMF_LINK	020	/* Item is one of two "link" items */
#define ID_ITMF_UPD	040	/* Item is the "updated" item */
#define ID_ITMF_SSI	0100	/* Item has string subitems */
#define ID_ITMF_WSI	0200	/* Item has word subitems */
#define ID_ITMF_LSI	0400	/* Item has line subitems */
#define ID_ITMF_SI (ID_ITMF_SSI|ID_ITMF_WSI|ID_ITMF_LSI) /* Has subitems */
#define ID_ITMF_UNIQ	01000	/* Item contents must be unique */

/* Externally available locations */
extern int	id_abortf;	/* Set non-zero by user to abort IDB loops */
extern int	id_updflg;	/* Non-zero if database being modified */
extern int	id_errflg;	/* Non-zero if errors in current database */
extern idstr_t	id_errstr;	/* Error msg, for some failing routines. */
#define id_errcp ID_SCP(id_errstr)	/* Char ptr to err msg */
extern idrix_t	id_recidx;	/* Current record idx (-1 new, 0 none) */

/* IDB routine declarations */

extern int	id_pkginit();	/* Init package */
extern void	id_pkgreset();	/* Reset package */

extern int
	id_dbselect(),		/* Select database */
	id_dbget(),		/* Load database for reading */
	id_dbgetupd(),		/* Allow updates to database */
	id_dbcreate(),		/* Create new empty database */
	id_dbput(),		/* Make all updates permanent */
	id_dbstat(),		/* Return full status of current DB */
	id_dbvstat(),		/* Return variable-only status of DB (fast) */
	id_dblock(),		/* Lock DB (independent of updating) */
	id_dbunlock(),		/* Release write lock on DB */
	id_dbgc(),		/* GC and compact database */
	id_dbtrim(),		/* Trim DB files */
	id_dbbackup(),		/* Create duplicate backup database */
	id_dbcopy();		/* Copy DB to new specified name. */

extern int
	id_ddfget(),	/* Get database definition info */
	id_ddfput();	/* Modify database definition */

extern int	id_idfget();	/* Get item definition */
extern int	id_idfput();	/* Change/add item definition */
extern int	id_idfdel();	/* Delete item definition */
extern int	id_idfnamechg(); /* Chg/add/del synonym */
extern idstr_t	id_idftostr();	/* Represent IDF as string */
extern int	id_idffrstr();	/* Build IDF from string */
extern int	id_idfupdget();	/* Get the update-item number if any */
extern int	id_idflnkget();	/* Get the link-item numbers if any */
extern int	id_idfsynnext(); /* Get next item synonym */
extern iditm_t	id_idfnum();	/* Get item # for item name */
extern idstr_t	id_idfname();	/* Get official name item # */
extern idifl_t	id_idfflags();	/* Get flags for item */
extern int	id_idfissort();	/* True if item has sort-table */
extern int	id_idfissi();	/* True if item has subitems */

extern idstr_t	id_ifltostr();	/* Convert flags into flagchars string */
extern idifl_t	id_iflfrstr();	/* Convert flagchars string into flags */
#define		id_iflissort(ifl)	((ifl)&ID_ITMF_SORT)
#define		id_ifliscasedep(ifl)	((ifl)&ID_ITMF_CASE)
#define		id_iflisnum(ifl)	((ifl)&ID_ITMF_NUM)
#define		id_iflislink(ifl)	((ifl)&ID_ITMF_LINK)
#define		id_iflisupdate(ifl)	((ifl)&ID_ITMF_UPD)
#define		id_iflissi(ifl)		((ifl)&ID_ITMF_SI)
#define		id_iflisssi(ifl)	((ifl)&ID_ITMF_SSI)
#define		id_ifliswsi(ifl)	((ifl)&ID_ITMF_WSI)
#define		id_iflislsi(ifl)	((ifl)&ID_ITMF_LSI)
#define		id_iflisunique(ifl)	((ifl)&ID_ITMF_UNIQ)

extern int
	id_recfind(),		/* FIND and get record */
	id_recnext(),		/* Get next record in FIND order */
	id_recprev(),		/* Get prev record in FIND order */
	id_stefind(),		/* Find STE (sort-table entry) */
	id_stenext(),		/* Get next STE */
	id_steprev(),		/* Get previous STE */
	id_rixnext(),		/* Get next record index */
	id_rixprev(),		/* Get prev record index */
	id_steins(),		/* Insert STE (maintenance) */
	id_stedel();		/* Delete STE (maintenance) */

extern int
	id_recget(),		/* Get record for index */
	id_recgethdl(),		/* Get record for handle */
	id_recgetitm(),		/* Get record for (shd be unique) item */
	id_reccreate(),		/* Create empty current record */
	id_recput(),		/* Write out current record */
	id_recputraw(),		/* ditto w/o checking "update" or "links" */
	id_recdel(),		/* Delete current record */
	id_recvfy();		/* Verify links for current record */

extern int
	id_itmget(),		/* Get item in current record */
	id_itmnext(),		/* Get next item */
	id_itmput(),		/* Put item into current record */
	id_itmdel(),		/* Delete item from current rec */
	id_itmapp(),		/* Append to item */
	id_itmismod(),		/* TRUE if item modified in cur rec */
	id_itmcmp(),		/* Compare item with string, per item flags */
	id_itmmatch(),		/* TRUE if string completely matches item */
	id_itmsimatch(),	/* TRUE if find subitem in item */
	id_itmsiapp(),		/* Append "str" subitem to item */
	id_itmsiadd(),		/* Add "str" subitem to item if not there */
	id_itmsidel(),		/* Delete "str" subitem from itm if there */
	id_itmwsiadd(),		/* Add "str" wd-subitm to item if not there */
	id_itmwsidel();		/* Delete "str" wd-subitm from itm if there */

extern int
	id_ssiget(),		/* Get next String-subitem */
	id_lsiget(),		/* Get next Line-subitem */
	id_wsiget(),		/* Get next Word-subitem */
	id_siget();		/* Get next subitem from str */

extern idstr_t
	id_fndstrget();		/* Get canonicalized target string */
extern int
	id_radget(),		/* Get record address */
	id_stafprint(),		/* Outputs status info to stream */
	id_stfmake(),		/* Remake sort-table file */
	id_stfgc(),		/* GC the sort-table for specified item */
	id_rcfgc();		/* GC the record file only */

extern long
	id_stfname();		/* Get sort-table filename */

extern int
	id_stfxck(),	/* Cross-chk STF */
	id_rcfxck(),	/* Cross-chk all records vs STFs */
	id_recxck(),	/* Cross-chk current record vs STFs */
	id_itmxck(),	/* Cross-chk current record item vs STF */
	id_ptfprint();	/* Output dump of specified PT */

/* String-handling facilities */

/* "Functions" which operate on idstr_t variables */
#define ID_SLN(a)  ((a).s_len)		/* Get length */
#define ID_SCP(a)  ((a).s_cp)		/* Get char pointer */
#define ID_SBP(a)  ((a).s_cp-1)		/* Get BP (a hack) */
#define ID_SPTR(a) ((a).s_cp)		/* Get pointer (for compares only!) */

/* Set component vals in an idstr_t (must provide address of the idstr_t) */
#define ID_SLNSET(a,i)  ((a).s_len = (i))	/* Set length */
#define ID_SCPSET(a,cp) ((a).s_cp = (cp))	/* Set ptr with char ptr */
#define ID_SBPSET(a,cp) ((a).s_cp = (cp)+1)	/* Set ptr with byte ptr */
#define ID_STRSET(a,cp,i) (ID_SCPSET(a,cp),ID_SLNSET(a,i),a)
#define ID_STRLIT(slit) id_strfmem(slit,sizeof(slit)-1)
#define ID_STRLIZ(slit) ID_STRIZR(slit,sizeof(slit)-1)
#define ID_STRIZR(cp,i)	{ cp, i }

extern int id_fputs();		/* Output idstr_t to stream */
extern idstr_t
	id_strfs(),	/* Create idstr_t from C str */
	id_strfmem(),	/* Create idstr_t from char ptr & cnt */
	id_strscat(),	/* Append to C-string from str, null-term */
	id_strscpy(),	/* Copy   to C-string from str, null-term */
	id_strmcat(),	/* Append to cp from str */
	id_strmcpy(),	/* Copy   to cp from str */
	id_strcats(),	/* Append to str from C-string */
	id_strcpys(),	/* Copy   to str from C-string */
	id_strsncat(),	/* App <= N chs to C-str from str, null-term */
	id_strsncpy(),	/* Copy N chars to C-str from str, null-pads */
	id_strncats(),	/* App <= N chars to str from C-string */
	id_strncpys(),	/* Copy N chars to str from C-string */
	id_strbcpy(),	/* Copies to buffer from str, updates buf. */
	id_strbcpys(),	/* Ditto, from C-string. */
	id_strbprintf();	/* Printf-style output to buf */
extern int
	id_strscmp(),	/* Compares C-string with str */
	id_strsncmp(),	/* Compares C-string with str, up to N chars */
	id_strmatch();	/* TRUE if s1 matches s2 exactly */

#define id_strlen(s) ID_SLN(s)

extern idstr_t
	id_strcat(),	/* Appends str2 to str1, returns new idstr_t */
	id_strncat(),	/* Ditto but doesn't append more than N chars. */
	id_strcpy(),	/* Copies str2 to str1 (ignores dest cnt). */
	id_strncpy();	/* Writes N chars from str2 to str1. */
extern int
	id_strcmp(),	/* Compares two idstr_ts like strcmp. */
	id_strncmp();	/* Compares two idstr_ts like strncmp. */
extern idstr_t
	id_strchr(),	/* Finds c, returns remainder of str (c 1st char) */
	id_strchr1(),	/* Same but returns 1st-part (not remainder). */
	id_strrchr(),	/* Like id_strchr but finds last c. */
	id_strrchr1();	/* Same but returns 1st-part (not remainder). */
extern int
	id_strpos(),	/* Finds c, returns char pos. */
	id_strrpos(),	/* Like id_strpos but finds last c. */
	id_strspn(),	/* Skips setstr chars, returns char pos. */
	id_strcspn();	/* Like id_strpos for any chars in setstr. */
extern idstr_t
	id_strpbrk(),	/* Like id_strspn, returns remainder of str. */
	id_strpbrk1(),	/* Same but returns 1st-part (not remainder). */
	id_strrpbrk(),	/* Like id_strcspn, returns remainder of str. */
	id_strrpbrk1(), /* Same but returns 1st-part (not remainder). */
	id_strstr(),	/* Should act like id_strchr. */
	id_strtok();	/* Returns idstr_t describing token. */

extern int		/* Case-independent versions of routines above */
	id_strCMP(),
	id_strnCMP(),
	id_strPOS(),
	id_strrPOS(),
	id_strsCMP(),
	id_strsnCMP(),
	id_strMATCH();
extern idstr_t
	id_strCHR(),
	id_strCHR1(),
	id_strrCHR(),
	id_strrCHR1(),
	id_strSTR();

#endif /* ifndef _IDB_INCLUDED */
