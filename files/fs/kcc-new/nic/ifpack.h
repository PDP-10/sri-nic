/* IFPACK.H - NIC IFPACK definitions.
**
**	See nicprog/cifp.doc and nicprog/idb.doc for complete documentation.
**
**	This file provides definitions for backwards compatibility with
** CIFP, the C interface to the assembly-language version of IFPACK.
**	It includes the definitions for IDB, the C version of IFPACK.
*/

#ifndef _IFPACK_INCLUDED	/* Only include once */
#define _IFPACK_INCLUDED 1

#include <nic/idb.h>		/* Include defs for NIC Ifpack DataBase */

/* CIFP compatibility definitions.
**	All definitions for external consumption are CAPITALIZED.
** The CIFP user should never refer to the lower-case definitions.
*/

/* Typedefs for basic objects */
#define ID_ITM iditm_t	/* Integer for holding item # (includes neg #s) */
#define ID_RIX idrix_t	/* Integer big enough to hold record index */
#define ID_SLE idstx_t	/* Integer for SLE (Slot-Entry val) */
#define ID_STR idstr_t	/* Count & pointer to string */
#define ID_FNDBLK struct id_fndblk_s	/* arg block for ID_FNDIDX etc */
struct id_fndblk_s {
	ID_ITM idf_item;
	ID_STR idf_str;
	ID_SLE idf_SLE;
	ID_RIX idf_rix;
};

#undef ID_SCPSET	/* String handling incompatibility!!! */
#define ID_SCPSET(a,cp) ((a)->s_cp = (cp))	/* Set char ptr */
#define ID_SLENSET(a,i) ((a)->s_len = (i))	/* Set length */
#define ID_SLEN(a) ID_SLN(a)			/* For compatibility */

/* Locations visible to user */
#define ID_EMSGPT id_errcp  /* Char ptr to err msg, for failing routines.  */
#define ID_RECIDX id_recidx /* Rec idx for current rec */
#define ID_ABORTF id_abortf /* Set non-zero by user to abort IFPACK loops */
/* ID_UPDFLG flushed */ /* Non-zero if database being modified */
/* ID_ITMBGP flushed */ /* Non-zero if there is a current record. */
/* ID_LNKITM flushed */	/* <#A>,,<#B> if two link items A and B exist */

/*---------------------------------------------------------
**			CIFP routine definitions
**	See nicprog/cifp.doc for details of args and return values.
**---------------------------------------------------------
*/

/* Package initialization and reset */
#define ID_INIT		idcinit		/* Init package */
#define ID_IDRES	idcreset

/* Database manipulation routines */
#define ID_DBSELECT	id_dbselect
#define ID_LOAD		idclod		/* Load database for reading */
#define ID_DBLOAD	id_dbget
#define ID_UPDOPN	idcupopn	/* Allow updates to database */
#define ID_UPDCLS	id_dbput
#define ID_DBCREATE	id_dbcreate
#define ID_GCFILS	id_dbgc
#define ID_BACKUP	id_dbbackup

/* Database definition routines */
#define ID_DIDGET	idcgdd	/* Get database ID */
#define ID_DIDSET	idcsdd	/* Set database ID */

#define ID_IDFCHG	idcidf	/* Chg/add/del item def */
#define ID_ISYCHG	idcsyc
#define ID_ISTP		id_idfissort
#define ID_IFLP		id_idfissi
#define ID_ITMSTR	id_idfname
#define ID_ITMNUM	id_idfnum
#define ID_ILTNXT	idcsyn
#define ID_ITMFLA	idcgfc	/* Get flag chars for item */
#define ID_ITMFLG	idcgfb	/* Get item flags (-1 if error) */
#define ID_CVTFLA	idcb2c	/* Convert flag bits to flagchars */
#define ID_CVTFLG	id_iflfrstr

/* Search routines.  See ID_FNDBLK for definition of argument block. */

#define ID_FNDIDX idcfidx	/* Search for record */
#define ID_FNDSBX idcfsbx	/* ditto, substr match */
#define ID_FNDPFX idcfpfx	/* ditto, prefix match */
#define ID_NXTIDX idcnxtx	/* Get next ST entry */

/* Record manipulation routines */

#define ID_GETRCZ idcgrcz	/* Get record for handle */
#define ID_GETREC id_recgetitm
#define ID_GETCUR id_recget
#define ID_CRFCUR id_reccreate
#define ID_UPDREC id_recput
#define ID_UPDREZ id_recputraw
#define ID_DELCUR id_recdel

/* Item manipulation routines (all operate on current record) */

#define ID_ITMFND id_itmget
#define ID_ITMNXT id_itmnext
#define ID_ITMPUT id_itmput
#define ID_ITMDEL id_itmdel
#define ID_ITMAPP id_itmapp
#define ID_ITMWP  id_itmismod

/* Field manipulation routines. */

#define ID_IFLGET id_siget	/* Get next field */
#define ID_SFGET id_ssiget	/* Get next Subfield */
#define ID_LFGET id_lsiget	/* Get next Linefield */
#define ID_WFGET id_wsiget	/* Get next Wordfield */
	/* Obsolete definitions just in case */
#define ID_SFLGET ID_SFGET	/* Get next Subfield */
#define ID_LFLGET ID_LFGET	/* Get next Linefield */
#define ID_WFLGET ID_WFGET	/* Get next Wordfield */

/* Miscellaneous utility routines */

#define ID_WRDDEL id_itmwsidel
#define ID_WRDADD id_itmwsiadd
#define ID_CVSTR id_strfs
#define ID_PUTS  id_fputs

/* Declarations for simulated routines (not declared by idb.h) */

extern char *ID_DIDGET();
extern idstr_t ID_ITMFLA(), ID_CVTFLA();
extern int
	ID_INIT(),
	ID_IDRES(),
	ID_LOAD(),
	ID_UPDOPN(),
	ID_DIDSET(),
	ID_IDFCHG(),
	ID_ISYCHG(),
	ID_ILTNXT(),
	ID_ITMFLG(),
	ID_FNDIDX(),
	ID_FNDSBX(),
	ID_FNDPFX(),
	ID_NXTIDX(),
	ID_GETRCZ();

#endif /* ifndef _IFPACK_INCLUDED */
