/* cc.g - code generation header file */

#define	    sc extern
#include    "cc.h"

#define	FAIL    1	/* emit FAIL-compatible code     */
#define MACRO	0	/* emit MACRO-10 compatible code */

#define	MAXCODE	256	/* peephole size 2^n */
#define	MAXCASE 256	/* max number of cases per switch */
#define	MAXLIT  128	/* number of literals per function */

struct PSEUDO {			/* pseudo code structure*/
	int    ptype;		    /*	ptype GLOBAL/GADDR */
	int    pop;		    /*  operator        */
	int    preg;		    /*  pseudo register */
	struct SYMBOL *pptr;	    /*  identifier      */
        int    poffset;
	int    pindex;
};

struct PS2 {			/* pseudo code structure*/
	int    ptype;		    /*  ptype IMMED     */
	int    pop;		    /*  operator        */
	int    preg;		    /*  pseudo register */
	int    _p1;
	int    pvalue;		    /*  immediate value = poffset! */
};

struct PS3 {			/* pseudo code structure*/
	int    ptype;		    /*  ptype REGIS/INDIRECT */
	int    pop;		    /*  operator        */
	int    pr1;		    /*  pseudo register */
	int    pr2;		    /*  immediate value */
};

struct PS4 {			/* pseudo code structure*/
	int    ptype;		    /*  ptype arbitrary */
	int    pop;		    /*  = GLABEL	*/
	char   plabel[10];	    /*  label string    */
};

struct LITERAL {
	int    llabel;
	char   *lptr;
};


struct CASES {
	int	csval;		/* case value */
	int	cslab;		/* case label */
};

extern	  char		*dec20op[];

sd struct PSEUDO 	codes[MAXCODE],
			*previous;
sd struct LITERAL	literal[MAXLIT];

sd	  int		maxcode,
			mincode,
			maxlit,
			stackoffset,
			regis[32],
			brklabel,
			looplabel;

#define	GLOBAL	    1		/* op code types */
#define LOCAL       2
#define IMMED	    3
#define REGIS       4
#define GADDR	    5
#define	LADDR	    6
#define INDIRECT    7
#define MINDIRECT   8
#define ONEREG	    9
#define LAB	   10
#define	BR	   11
#define IMMED2	   12
#define INDEXED	   13
#define MINDEXED   14
#define	BOTH	   15
#define RCONST	   16
#define BYTEPOINT  17
#define ISINGLE	   18
#define SINGLE	   19
#define IINDEXED   20
#define BLABEL     21

#define RELEASE  1000
#define GLABEL	 1001
#define	CODED	 1002
#define NOP	 1003
#define CLABEL	 1004
#define CVALUE	 1005
#define DLABEL	 1006
#define CINIT	 1007

/* aliases for pseudo codes */

#define	JUMPN   ELSE
#define	JUMPG   FOR
#define JUMPL   DO
#define JUMPE   WHILE
#define	JUMPLE  SWITCH
#define	JUMPGE  CASE
#define CAIN	UNSIGNED
#define CAIG	AUTO
#define	CAIL	EXTERN
#define CAIE	REGISTER
#define CAILE	TYPEDEF
#define CAIGE	STATIC
#define	SETZ	SPACE
#define	SETF	CONST		/* setz that is not optimized */
#define	CALL	FUNCTION
#define	FNARG	FNDEF
#define POP	ARRAY
#define ADJSP	DQUOTE
#define HRL	POUND
#define HRRZ	SQUOTE
#define	HRLZ	UNDEF
#define DPB	INT
#define LDB	CHAR
#define ADJBP	FLOAT
#define IBP	DOUBLE
#define IDPB	STRUCT
#define ILDB	UNION
#define ADDM	88
#define SUBM	89
#define ABS	99
#define	ROT	COERCE
	
#define SP	017		/* stack pointer */

#define	LSBYTE	01100		/* byte pointer to LS byte */
#define MSBYTE  0331100 	/* byte pointer to MS byte */

