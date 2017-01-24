/*
 *	MATH.H - math library definitions
 */

extern double acos(), asin(), atan(), atan2(), ceil(), cos(), cosh();
extern double exp(), fabs(), floor(), fmod(), frexp(), ldexp(), log();
extern double log10(), modf(), pow(), sin(), sinh(), sqrt(), tan(), tanh();
extern long int labs();
extern int abs(), rand();
extern srand();

/* Additional routines used by some of the above */

extern double sign(), xmant(), poly();
extern int xexp();

/* Define some commonly used constants */

#define TWOPI		6.2831853071795864769
#define PI		3.1415926535897932384
#define HALFPI		1.5707963267948966192
#define FOURTHPI	0.7853981633974483096
#define SIXTHPI		0.523598776
#define LOG2E		1.4426950408889634073
#define LOG10E		0.4342944819032518276
#define SQRT2		1.4142135623730950488
#define SQRT3		1.7320508075688772935
#define LN2		0.6931471805599453094
#define LNSQRT2		0.3465735902799726547

/* Some hardware-dependant constants */

#define MAX_POS_DBLF 1.7014118e38	/* Max positive double float	*/
#define MIN_POS_DBLF 1.4693680e-39	/* Min positive double float	*/
#define MAX_NEG_DBLF -1.7014118e38	/* Max negative double float	*/
#define MIN_NEG_DBLF -1.4693680e-39	/* Min negative double float	*/
#define MAX_EXPONENT 127		/* Max exponent allowed		*/
#define RECIP_MIN 5.877471e-39		/* MAX_POS_DBLF >= 1/RECIP_MIN	*/
#define RECIP_MAX 1.7014118e38		/* MIN_POS_DBLF <= 1/RECIP_MAX	*/
#define LN_MAXPOSDBL 88.0		/* LN(MAX_POS_DBLF)		*/
#define LN_MINPOSDBL -89.4		/* LN(MIN_POS_DBLF)		*/
#define TANH_MAXARG 16			/* |TANH(maxarg)| = 1.0		*/
#define DSQRT_MPDF   1.304380e19	/* DSQRT(MAX_POS_DBLF)		*/
#define X6_UNDERFLOWS 3.37174e-7	/* X**6 almost underflows	*/
#define X16_UNDERFLOWS 3.74063e-3	/* X**16 almost underflows	*/
