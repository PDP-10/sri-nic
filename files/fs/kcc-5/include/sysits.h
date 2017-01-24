/* This file is meant to be included by programs
 * using ITS system calls.
 */

#define SC_NAM(a) ato6(a)		/* Ugh bletch */

#define SC_ARG(a) (a)			/* 0 - argument (for completeness) */
#define SC_IMM(a) ((1<<26)|(int)a)	/* 1 - immediate argument */
#define SC_VAL(a) ((2<<26)|(int)a)	/* 2 - value */
#define SC_ERR(a) ((3<<26)|(int)a)	/* 3 - error return code */
#define SC_CTL(a) ((4<<26)|(int)a)	/* 4 - control */
#define SC_IMC(a) ((5<<26)|(int)a)	/* 5 - immediate control */
#define SC_LAST(a) ((1<<35)|(int)a)	/* Turns on SETZ bit */
#define SC_LERR(a) (SC_LAST(SC_ERR(a)))		/* Common combo */

#define SYSCAL(name) _scall(3,SC_LERR(1),\
			name,SC_LAST(0))
#define SYSCAL1(name,a) _scall(4,SC_LERR(1),a,\
			name,SC_LAST(0))
#define SYSCAL2(name,a,b) _scall(5,SC_LERR(1),b,a,\
			name,SC_LAST(0))
#define SYSCAL3(name,a,b,c) _scall(6,SC_LERR(1),c,b,a,\
			name,SC_LAST(0))
#define SYSCAL4(name,a,b,c,d) _scall(7,SC_LERR(1),d,c,b,a,\
			name,SC_LAST(0))
#define SYSCAL5(name,a,b,c,d,e) _scall(8,SC_LERR(1),e,d,c,b,a,\
			name,SC_LAST(0))
#define SYSCAL6(name,a,b,c,d,e,f) _scall(9,SC_LERR(1),f,e,d,c,b,a,\
			name,SC_LAST(0))
#define SYSCAL7(name,a,b,c,d,e,f,g) _scall(10,SC_LERR(1),g,f,e,d,c,b,a,\
			name,SC_LAST(0))
#define SYSCAL8(name,a,b,c,d,e,f,g,h) _scall(11,SC_LERR(1),h,g,f,e,d,c,b,a,\
			name,SC_LAST(0))

