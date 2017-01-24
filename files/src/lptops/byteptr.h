/* 
 * byteptr.h - Stuff for byte pointers.
 * 
 * Author:	Jay Lepreau
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Feb 1981
 * Copyright (c) 1981 Jay Lepreau
 */

# define SZCHAR 36
# define POSLOC	30		/* location of position field in bp */
# define SIZLOC 24		/*    "		size	  "	"   */
# define ADDRFLD 0777777	/* address field of instr and bp    */

#ifdef CROSSCOMP
  static int posloc = POSLOC;
  static int sizloc = SIZLOC;
# define POINT(pos, addr)  ((SZCHAR << sizloc) | (int)(addr)-1)/*ready for ildb*/ 
# define MKBPTR(pos, addr) ((SZCHAR << sizloc) | (int)(addr))/* the real bp */

#else
# define POINT(addr)  ((SZCHAR << SIZLOC) | (int)(addr)-1) /* ready for ildb*/ 
# define MKBPTR(addr) ((SZCHAR << SIZLOC) | (int)(addr))   /* the real bp */
# define UNBPTR(addr) ((int)(addr) & ADDRFLD)

#endif

/*
 * GETJUSLEN - get jsys updated string length
 * Next macro finds the length of a string returned by a jsys, where
 * 'before' is the C (char *) variable prior to POINT, and 'after' is
 * the "updated string ptr" exactly as returned by the jsys.
 * NB: The length does not include the trailing null (like strlen).
 */
#define GETJUSLEN(before, after) (getright(after) - (int) (before) + 1)
