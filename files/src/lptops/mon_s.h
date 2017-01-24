/* sctty */

#define SCret 00		/* return designator (ctty) for fork */
#define SCset 01		/* set sctty for fork */
#define SCrst 02		/* clear fork ctty (restore job ctty) */

/* scvec */

#define SVead 00		/* entry address */
#define SVine 01		/* initial entry for setup */
#define SVget 02		/* entry address for get share file routine */
#define SV40 03	 		/* address to get location 40 */
#define SVrpc 04		/* address to get return pc */
#define SVmak 05		/* entry for make share file routine */
#define SVcst 06		/* 2 word block for control-c/start processing */


/* sdvec */

#define SDead 00		/* entry address */
#define SDine 01		/* initial entry */
#define SDver 02		/* dms version */
#define SDdms 03		/* address to store dms jsys */
#define SDrpc 04		/* address to store return pc */



/* setjb function codes */

#define SJden 00		/* set default magtape density */
#define SJddn 00		/* system default density */
#define SJdn2 01		/* 200 bpi */
#define SJdn5 02		/* 556 bpi */
#define SJdn8 03		/* 800 bpi */
#define SJd16 04		/* 1600 bpi */
#define SJd62 05		/* 6250 bpi */
#define SJpar 01		/* set default magtape parity */
#define SJpro 00		/* odd parity */
#define SJpre 01		/* even parity */
#define SJdm 02	 		/* set default magtape data mode */
#define SJddm 00		/* system default data mode */
#define SJdmc 01		/* core dump mode */
#define SJdm6 02		/* six bit byte mode (for 7-track drives) */
#define SJdma 03		/* ansi ascii mode (7 bits in 8 bit byte) */
#define SJdm8 04		/* industry compatible mode */
#define SJdmh 05		/* hi-density mode (9 eight bit */
				/* bytes in 2 words) */
#define SJrs 03	 		/* set default magtape record size */
#define SJdfs 04		/* set deferred spooling */
#define SJspi 00		/* immediate mode spooling */
#define SJspd 01		/* deferred mode spooling */
#define SJsrm 05		/* set job session remark */
#define SJt20 06		/* declare whether tops20 command level or not */
#define SJdfr 07		/* set default job retrieval mode */
#define SJrfa 00		/* openf should always fail */
#define SJrwa 01		/* openf should always request & wait */
#define SJbat 010		/* set batch flags and stream */
				/* see .jibch for field definitions */
#define SJllo 011		/* set job location */


/* sfork */

#define SF_con 01:35-0		/* continue process, ignore pc in ac2 */
#define SF_prh 0777777:35-35	/* process handle */


/* sfust */

#define SFaut 00		/* set author string */
#define SFlwr 01		/* set last writer string */



/* smon function codes and bit definitions (system flags) */

#define SFfac 00		/* allow fact entries */
#define SFcde 01		/* checkdisk found errors */
#define SFcdr 02		/* checkdisk running */
#define SFmst 03		/* manual start in progress */
#define SFrmt 04		/* remote logins allowed */
#define SFpty 05		/* pty logins allowed */
#define SFcty 06		/* cty login allowed */
#define SFopr 07		/* operator in attendance */
#define SFlcl 010		/* local logins allowed */
#define SFbte 011		/* bit table errors found on startup */
#define SFcrd 012		/* user can change directory characteristics */
#define SFnvt 013		/* tops20an	;nvt login allowed */
#define SFwct 014		/* wheel login on cty allowed */
#define SFwlc 015		/* wheel login on local terminals allowed */
#define SFwrm 016		/* wheel login on remote terminals allowed */
#define SFwpt 017		/* wheel login on pty's allowed */
#define SFwnv 020		/* tops20an	;wheel login on nvt's allowed */
#define SFusg 021		/* usage file in use */
#define SFflo 022		/* full latency optimization */
				/* caution: setting this requires that the */
				/* system be at revision level 10, and */
				/* that rh20 board m8555 be at revision level d. */
				/* otherwise, the file-system may be damaged. */

#define SFmta 023		/* magtape allocation enabled */
/* below are function codes that do not map directly into bits */


#define SFntn 044		/* tops20an	;network on/off control */
#define SFndu 045		/* tops20an	;net down/up request */
#define SFnhi 046		/* tops20an	;net host table initialize */
#define SFtmz 047		/* set time zone this system is in */
#define SFlhn 050		/* tops20an	;set local host number of this net site */
#define SFavr 051		/* account validation on/off */
#define SFsts 052		/* enable/disable status reporting */
#define SFsok 053		/* getok/givok default setting */
#define SFmcy 054		/* set max ordinary offline exp period */
#define SFrdu 055		/* read date update function */
#define SFacy 056		/* set max archive exp period */
#define SFrtw 057		/* set [no] retrieval waits non-0 => no wait */
#define SFtdf 060		/* tape mount controls */
#define MT_uut 01:35-0		/* unload unreadable tapes */
#define SFwsp 061		/* working set preloading */

#define SF_fac 01:35-<.sffac>	/* fact entries allowed */
#define SF_cde 01:35-<.sfcde>	/* checkdisk found errors */
#define SF_cdr 01:35-<.sfcdr>	/* checkdisk running */
#define SF_mst 01:35-<.sfmst>	/* manual start in progress */
#define SF_rmt 01:35-<.sfrmt>	/* remote logins allowed */
#define SF_pty 01:35-<.sfpty>	/* pty logins allowed */
#define SF_cty 01:35-<.sfcty>	/* cty login allowed */
#define SF_opr 01:35-<.sfopr>	/* operator in attendance */
#define SF_lcl 01:35-<.sflcl>	/* local logins allowed */
#define SF_bte 01:35-<.sfbte>	/* bit table errors found on startup */
#define SF_crd 01:35-<.sfcrd>	/* user can change directory characteristics */
#define SF_nvt 01:35-<.sfnvt>   /* tops20an	;nvt logins allowed */
#define SF_usg 01:35-<.sfusg>	/* usage file in use */
#define SF_flo 01:35-<.sfflo>	/* full latency optimization in use */
				/* caution: setting this requires that the */
				/* system be at revision level 10, and */
				/* that rh20 board m8555 be at revision level d. */
				/* otherwise, the file-system may be damaged. */
#define SF_mta 01:35-<.sfmta>	/* magtape allocation enabled */


#define SF_eok 01:35-0		/* enable access checking */
#define SF_dok 01:35-1		/* allow access if checking disabled */


/* sinm jsys definitions */

#define SI_tmg 01:35-0		/* truncate message */
#define SI_eom 01:35-1		/* end-of-message found */

/* sir jsys (new form) */

#define SI_ver 07:35-17		/* version of sir in t1 */
#define SI_lev 077:35-5		/* level field in chntab */
#define SI_adr 7777,,-1		/* address of interrupt routine in chntab */

/* sked jsys */

#define SAcnt 00	 	/* argument block offset for count */

	/* function codes */

#define SKrbc 01		/* read bias control knob */
#define SAknb 01	 	/* offset for knob value */
#define SKsbc 02		/* set bias control knob */
#define SKrcs 03		/* read share of a class */
#define SAcls 01	 	/* class */
#define SAshr 02	 	/* share */
#define SAuse 03	 	/* use */
#define SA1ml 04	 	/* 1 minute load average */
#define SA5ml 05	 	/* 5 minute load average */
#define SA15l 06	 	/* 15 minute load */
#define SKscs 04		/* set share of a class */
#define SKics 05		/* start or stop class scheduling */
#define SActl 01	 	/* word for control bits */
#define SKscj 06		/* set class of a job */
#define SAjob 01	 	/* job */
#define SAjcl 02	 	/* class of job */
#define SAwa 03			/* wa on/off switch */
#define SKrjp 07		/* read class parameters for a job */
#define SAjsh 03	 	/* job's share allotment */
#define SAjus 04	 	/* job's current use */
#define SKbcr 010		/* read class setting for batch jobs */
#define SAbcl 01	 	/* batch class */
#define SKbcs 011		/* set class for batch jobs */
#define SKbbg 012		/* run batch jobs on dregs queue */
#define SAdrg 01	 	/* word to specify dregs or not */
#define SKddc 013		/* set system class default */
#define SAdcl 01	 	/* default class word */
#define SKrcv 014		/* read status */
#define SK_act 01:35-0		/* class by accounts */
#define SK_wdf 01:35-1		/* withhold windfall */
#define SK_stp 01:35-2		/* class scheduler off */
#define SK_drg 01:35-3		/* batch jobs are being run on dregs queue */

/* snoop jsys definitions */

/* snoop function codes */

#define SNplc 00		/* lock code into monitor virt memory */
#define SNpls 01		/* lock down the swappable monitor */
#define SNpdb 02		/* define a break point */
#define SNpib 03		/* insert the break points */
#define SNprb 04		/* remove the break points */
#define SNpul 05		/* unlock and release all snoop resources */
#define SNpsy 06		/* look up a monitor symbol */
#define SNpad 07		/* look up address in symbol table */

/* soutm jsys definitions */

#define SO_wmg 01:35-0		/* write end-of-message */

/* spool jsys function codes */

#define SPldi 00		/* define an input spooling device */
#define SPlsd 01		/* set directory of spooled device */
#define SPlrd 02		/* read directory of spooled device */

/* flags in spool message on logout and spooled file close */

#define SP_bat 01:35-0		/* job is a batch job */
#define SP_dfs 01:35-1		/* spooling is deferred */
#define SP_elo 01:35-2		/* job executed lgout jsys itself */
#define SP_flo 01:35-3		/* job forced to log out by trap in top fk */
#define SP_olo 01:35-4		/* other job aimed lgout at this one */

/* spool argument block */

#define SPldv 00		/* device designator */
#define SPlna 01		/* name string */
#define SPldr 01		/* directory number */
#define SPlgn 02		/* generation number */


/* ssave */

#define SS_nnp 0777777:35-17	/* negative number of pages */
#define SS_cpy 01:35-18		/* allow copy-on-write */
#define SS_uca 01:35-19		/* use current access */
#define SS_rd 01:35-20		/* allow read access */
#define SS_wr 01:35-21		/* allow write access */
#define SS_exe 01:35-22		/* allow execute access */
#define SS_fpn 0777:35-35	/* first page number */


/* stcmp */

#define SC_lss 01:35-0		/* t1 less than t2 */
#define SC_sub 01:35-1		/* t1 substring of t2 */
#define SC_gtr 01:35-2		/* t1 greater than t2 */



/* stdir */

#define ST_dir 01:35-0		/* files only directory */
#define ST_ana 01:35-1		/* alphanumeric accounts */
#define ST_rlm 01:35-2		/* repeat login message */


/* stiw */

#define ST_dim 01:35-0		/* set deferred interrupt mask */
#define ST_prh 0777777:35-35	/* process handle */

/* swtrp definitions */

#define SWart 00		/* set arithmetic trap */
#define SWrat 01		/* read arithmetic trap */
#define SWlut 02		/* set luuo address */
#define SWrlt 03		/* read luuo address */
#define ARpfl 00	 	/* offset in trap block for pc flags */
#define ARopc 01	 	/* offset for old pc value */
#define ARefa 02	 	/* offset for e */
#define ARnpc 03	 	/* offset for new pc word */
