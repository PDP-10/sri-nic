/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/*
 * CMFIL.h:
 * symbols for filename parsing
 */


#define REGEXCHARS "$^.*+?[]\\"
#define REGEXQCHARS "|()123456789"

/*
 * internal filename parser symbols
 */
#define FIL_MAT 0x0001			/* a match so far */
#define FIL_NOR 0x0002			/* ignorable */
#define FIL_INV 0x0004			/* invisible */
#define FIL_EXA 0x0008			/* exact match (for wilds) */
#define FIL_ARD	0x0010			/* readable file */
#define FIL_AWR 0x0020			/* writable file */
#define FIL_AEX 0x0040			/* executable file */
#define FIL_ADR 0x0080			/* directory */
#define FIL_ASY 0x0100			/* system file */
#define FIL_AHD 0x0200			/* hidden file */
#define FIL_ALK 0x0400			/* symbolic link */
#define FIL_ASRCH 0x0800		/* directory execute */
#define FIL_STAT 0x1000			/* internal.  already stat'ed */


