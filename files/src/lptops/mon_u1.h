/* 
 * monsym.h - C version of monsym [mon_files.h extract]
 * 
 * Based on DEC's monsym.mac
 * Author:	Jay Lepreau
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	23 October 1981
 * Copyright (c) 1981,1982 Jay Lepreau for C version
 */

/*	Created from monsym.mac, 10/23/81  J.Lepreau, Univ of Utah 	*/
/*	Improved 1/82 */

/***		most recent edit history	***/
/* add .goatc for attach */
/* ps:<4.subsys-sources>monsym.mac.3  7-oct-80 20:22:30, edit by frank */
/* add smap jsys to monitor */
/* <4.utilities>monsym.mac.262,  3-jan-80 15:26:07, edit by r.ace */
/* update copyright date */
/* <4.utilities>monsym.mac.261, 24-oct-79 18:17:04, edit by murphy */
/* smapx1 */



/* ***************************************** */
/* jsys specific arguments */
/* the following are ordered alphabetically by jsys name */
/* ***************************************** */

/* acces - access a directory (e.g., by connecting) */

#define AC_con 01:35-0			/* connect to the specified directory */
#define AC_own 01:35-1			/* gain ownership */
#define AC_rem 01:35-2			/* remove ownership */

/* offsets in argument block */

#define ACdir 00	 		/* directory designator */
#define ACpsw 01	 		/* pointer to password string */
#define ACjob 02	 		/* job number (-1 for self) */

/* dvchr, devunt and dvch1 bit definitions */

#define DV_out 01:35-0			/* device can do output */
#define DV_in 01:35-1			/* device can do input */
#define DV_dir 01:35-2			/* device has a directory */
#define DV_as 01:35-3			/* device is assignable */
#define DV_mdd 01:35-4			/* device is a multiple directory device */
#define DV_av 01:35-5			/* device is available to this job */
#define DV_asn 01:35-6			/* device is assigned by asnd */
#define DV_mdv 01:35-7			/* reserved (historical) */
#define DV_mnt 01:35-8			/* device is mounted */
#define DV_typ 0777:35-17			/* device type field */
#define DV_psd 01:35-18			/* pseudo device */
#define DV_unt 077777	 		/* unit mask */
#define DV_mod 0177777:35-35		/* device data mode */
#define DV_m0 01:35-35			/* device can be opened in mode 0 */
#define DV_m1 01:35-34			/* device can be opened in mode 1 */
#define DV_m2 01:35-33			/* device can be opened in mode 2 */
#define DV_m3 01:35-32			/* device can be opened in mode 3 */
#define DV_m4 01:35-31			/* device can be opened in mode 4 */
#define DV_m5 01:35-30			/* device can be opened in mode 5 */
#define DV_m6 01:35-29			/* device can be opened in mode 6 */
#define DV_m7 01:35-28			/* device can be opened in mode 7 */
#define DV_m10 01:35-27			/* device can be opened in mode 10 */
#define DV_m11 01:35-26			/* device can be opened in mode 11 */
#define DV_m12 01:35-25			/* device can be opened in mode 12 */
#define DV_m13 01:35-24			/* device can be opened in mode 13 */
#define DV_m14 01:35-23			/* device can be opened in mode 14 */
#define DV_m15 01:35-22			/* device can be opened in mode 15 */
#define DV_m16 01:35-21			/* device can be opened in mode 16 */
#define DV_m17 01:35-20			/* device can be opened in mode 17 */
#define D1_spl 01:35-0			/* device is spooled */
#define D1_alc 01:35-1			/* device is under control of allocator */
#define D1_vvl 01:35-2			/* volume valid */
#define D1_niu 01:35-3			/* device slot is not in use (for structures  */
				/*  not yet mounted) */
#define D1_ini 01:35-4			/* device is being initialized (structure */
				/*  is available only to the fork whose number */
				/*  is stored in sdbsts) */



/* device type definitions */

#define DVdsk 00	 		/* disk */
#define DVmta 02	 		/* magtape */
#define DVdta 03	 		/* dectape */
#define DVptr 04	 		/* paper tape reader */
#define DVptp 05	 		/* paper tape punch */
#define DVdsp 06	 		/* display */
#define DVlpt 07	 		/* line printer */
#define DVcdr 010	 		/* card reader */
#define DVfe 011	 		/* front end device */
#define DVtty 012	 		/* terminal */
#define DVpty 013	 		/* pty */
#define DVnul 015	 		/* null device */
#define DVnet 016	 		/* arpa network */
#define DVplt 017	 		/* plotter */
#define DVcdp 021	 		/* card punch */
#define DVdcn 022	 		/* decnet active component */
#define DVsrv 023	 		/* decent passive component */
#define DVats 024	 		/* applications terminal service */
#define DVads 025	 		/* aydin display */

/* gtjfn definitions */

/* flags provided to gtjfn on call */

#define GJ_fou 01:35-0			/* file is for output use */
#define GJ_new 01:35-1			/* new file only */
#define GJ_old 01:35-2			/* old file only */
#define GJ_msg 01:35-3			/* print an appropriate message */
#define GJ_cfm 01:35-4			/* confirmation is required */
#define GJ_tmp 01:35-5			/* temporary */
#define GJ_ns 01:35-6			/* dont search search lists */
#define GJ_acc 01:35-7			/* no access by other forks */
#define GJ_del 01:35-8			/* ignore "deleted" bit */
#define GJ_jfn 03:35-10			/* jfn use field */
#define GJdnu 00	 		/* do not use jfn provided */
#define GJerr 02	 		/* error if cannot use jfn provided */
#define GJalt 03	 		/* use alternate if cannot use given jfn */
#define GJ_ifg 01:35-11			/* accept input file group descriptors */
#define GJ_ofg 01:35-12			/* accept output file group descriptors */
#define GJ_flg 01:35-13			/* return flags */
#define GJ_phy 01:35-14			/* physical device only */
#define GJ_xtn 01:35-15			/* extended format (e+11 exists) */
#define GJ_fns 01:35-16			/* accumulator 2 contains job file numbers */
#define GJ_sht 01:35-17			/* short call format */

/* flags provided to gtjfn (in second flag word) */

#define G1_rnd 01:35-0			/* return on null(in alternate flag word) */
#define G1_rbf 01:35-1			/* ^r buffer is disjoint (obsolete) */
#define G1_nln 01:35-2			/* no long names */
#define G1_rcm 01:35-3			/* return confirm message */
#define G1_rie 01:35-4			/* return when main string is empty */
#define G1_iin 01:35-5			/*  ignore invisible status */
#define G1_sln 01:35-6			/* suppress expansion of logical names */



/* flags returned by gtjfn */

#define GJ_dev 01:35-0			/* asterisk was given for device */
#define GJ_unt 01:35-1			/* asterisk was given for unit */
#define GJ_dir 01:35-2			/* asterisk was given for directory */
#define GJ_nam 01:35-3			/* asterisk was given for name */
#define GJ_ext 01:35-4			/* asterisk was given for extension */
#define GJ_ver 01:35-5			/* asterisk was given for generation */
#define GJ_uhv 01:35-6			/* use highest generation */
#define GJ_nhv 01:35-7			/* use next higher generation */
#define GJ_ulv 01:35-8			/* use lowest generation */
#define GJ_pro 01:35-9			/* protection given */
#define GJ_act 01:35-10			/* account given */
#define GJ_tfs 01:35-11			/* temporary file specified (;t) */
#define GJ_gnd 01:35-12			/* complement of gj%del on call */
#define GJ_giv 01:35-17			/*  comp of g1%iiv */

/* gtjfn table offsets */

#define GJgen 00	 		/* flags ,, generation */
#define GJdef <z 0>			/* default generation */
#define GJnhg <z -1>			/* next higher generation */
#define GJleg <z -2>			/* lowest existing generation */
#define GJall <z -3>			/* all generations (i.e., ;*) */
#define GJsrc 01	 		/* source jfn ,, output jfn */
#define GJdev 02	 		/* default device */
#define GJdir 03	 		/* default directory */
#define GJnam 04	 		/* default name */
#define GJext 05	 		/* default extenstion */
#define GJpro 06	 		/* default protection */
#define GJact 07	 		/* default account */
#define GJjfn 010	 		/* desired jfn */
#define GJf2 011	 		/* second group flags,,count */
#define GJcpp 012	 		/* copy buffer pointer */
#define GJcpc 013	 		/* copy buffer count */
#define GJrty 014	 		/* retype (^r) pointer */
#define GJbfp 015	 		/* top of buffer pointer */
#define GJatr 016	 		/* pointer to arbitrary attribute block */

/* gnjfn - flags returned */

#define GN_str 01:35-13			/* structure changed */
#define GN_dir 01:35-14			/* directory changed */
#define GN_nam 01:35-15			/* name changed */
#define GN_ext 01:35-16			/* extension changed */


/* openf */

#define OF_bsz 077:35-5			/* byte size */
#define OF_mod 017:35-9			/* mode */
#define OF_her 01:35-18			/* halt on io error */
#define OF_rd 01:35-19			/* read */
#define OF_wr 01:35-20			/* write */
#define OF_ex 01:35-21			/* execute (reserved for the future) */
#define OF_app 01:35-22			/* append */
#define OF_thw 01:35-25			/* thawed */
#define OF_awt 01:35-26			/* always wait */
#define OF_pdt 01:35-27			/* preserve dates */
#define OF_nwt 01:35-28			/* never wait */
#define OF_rtd 01:35-29			/* restricted */
#define OF_pln 01:35-30			/* set to disable line number checking for */
				/*  non-line number files */
#define OF_dud 01:35-31			/* don't update to disk by ddmp */
#define OF_ofl 01:35-32			/* allow opening the device even if offline */
#define OF_fdt 01:35-33			/* force date update */
#define OF_rar 01:35-34			/*  wait if file is off-line */


/* input and output identifiers */

#define PRiin 0100	 		/* primary input */
#define PRiou 0101	 		/* primary output */
#define NUlio 0377777	 		/* null designator */
#define CTtrm 0777777	 		/* job's controlling terminal */
#define DVdes 0600000	 		/* universal device code */
#define TTdes 0400000	 		/* universal terminal code */

/* rdtty and texti */

#define RD_brk 01:35-0			/* break on regular break set */
#define RD_top 01:35-1			/* break on tops10 break set */
#define RD_pun 01:35-2			/* break on punctuation */
#define RD_bel 01:35-3			/* break on end of line */
#define RD_crf 01:35-4			/* suppress cr (returns lf only) */
#define RD_rnd 01:35-5			/* return if nothing to delete */
#define RD_jfn 01:35-6			/* jfns given for source */
#define RD_rie 01:35-7			/* return on input (buffer) empty */
#define RD_bbg 01:35-8			/* beginning of (dest) buffer given */
#define RD_beg 01:35-9			/* return immediately when typist edits to .rdbkl */
#define RD_rai 01:35-10			/* raise lowercase input */
#define RD_sui 01:35-11			/* suppress ^u indication */
#define RD_btm 01:35-12			/* break character terminated input */
#define RD_bfe 01:35-13			/* returned because buffer empty */
#define RD_blr 01:35-14			/* backup limit reached */

#if 0	/* Never include this, cause done with a struct in the source. */
/* texti arg block */

#define RDcwb 00	 		/* count of words in block */
#define RDflg 01	 		/* flags */
#define RDioj 02	 		/* io jfns */
#define RDdbp 03	 		/* dest byte pointer */
#define RDdbc 04	 		/* dest byte count */
#define RDbfp 05	 		/* top of buffer pointer */
#define RDrty 06	 		/* retype (^r) pointer */
#define RDbrk 07	 		/* break set mask pointer */
#define RDbkl 010	 		/* backup limit pointer */

#endif 0

/* Errors */

#define ERb 0600000		/* "Error base" */

#define ERopnx1 (ERb|0120) /* File is already open */
#define ERopnx2 (ERb|0121) /* File does not exist */
#define ERopnx3 (ERb|0122) /* Read access required */
#define ERopnx4 (ERb|0123) /* Write access required */
#define ERopnx5 (ERb|0124) /* Execute access required */
#define ERopnx6 (ERb|0125) /* Append access required */
#define ERopnx7 (ERb|0126) /* Device already assigned to another job */
#define ERopnx8 (ERb|0127) /* Device is not on line */
#define ERopnx9 (ERb|0130) /* Invalid simultaneous access */
#define ERopnx10 (ERb|0131) /* Entire file structure full */
#define ERopnx12 (ERb|0133) /* List access required */
#define ERopnx13 (ERb|0134) /* Invalid access requested */
#define ERopnx14 (ERb|0135) /* Invalid mode requested */
#define ERopnx15 (ERb|0136) /* Read/write access required */
#define ERopnx16 (ERb|0137) /* File has bad index block */
#define ERopnx17 (ERb|0140) /* No room in job for long file page table */
#define ERopnx18 (ERb|0141) /* Unit Record Devices are not available */
#define ERopnx19 (ERb|0142) /* IMP is not up */ ;TOPS20AN
#define ERopnx20 (ERb|0143) /* Host is not up */ ;TOPS20AN
#define ERopnx21 (ERb|0144) /* Connection refused */ ;TOPS20AN
#define ERopnx22 (ERb|0145) /* Connection byte size does not match */ ;TOPS20AN
