/*
 *	This file gets included with all of the floating point math
 *	library routines when they are compiled.  Note that
 *	this is the proper place to put machine dependencies
 *	whenever possible.
 *
 *	It should be pointed out that for simplicity's sake, the
 *	environment parameters are defined as floating point constants,
 *	rather than octal or hexadecimal initializations of allocated
 *	storage areas.  This means that the range of allowed numbers
 *	may not exactly match the hardware's capabilities.  For example,
 *	if the maximum positive double precision floating point number
 *	is EXACTLY 1.11...E100 and the constant "MAX_POS_DBLF is
 *	defined to be 1.11E100 then the numbers between 1.11E100 and
 *	1.11...E100 are considered to be undefined.  For most
 *	applications, this will cause no problems.
 *
 *	An alternate method is to allocate a global static "double" variable,
 *	say "max_pos_dblf", and use a union declaration and initialization
 *	to initialize it with the proper bits for the EXACT maximum value.
 *	This was not done because the only compilers available to the
 *	author did not fully support union initialization features.
 *
 */

#define PDP10

/*
 *	PDP10  (DECSYSTEM 20) HARDWARE DEPENDENCIES
 *
 *	********  W A R N I N G      W A R N I N G    ********
 *	The current DEC20 implementation treats double precision
 *	floats as single precision floats.  When true double
 *	precision is implemented, the flowing floats may have to
 *	change.
 *
 *	Since the PDP10 compiler has no static variables, and only
 *	first 5 letters are significant, must use preprocessor
 *	to avoid name clashes.
 */

#ifdef PDP10
#define MAX_POS_DBLF 1.7014118e38	/* Max positive double float	*/
#define MIN_POS_DBLF 1.4693680e-39	/* Min positive double float	*/
#define MAX_NEG_DBLF -1.7014118e38	/* Max negative double float	*/
#define MIN_NEG_DBLF -1.4693680e-39	/* Min negative double float	*/
#define MAX_EXPONENT 127		/* Max exponent allowed		*/
#define RECIP_MIN 5.877471e-39		/* MAX_POS_DBLF >= 1/RECIP_MIN	*/
#define RECIP_MAX 1.7014118e38		/* MIN_POS_DBLF <= 1/RECIP_MAX	*/
#define LN_MAXPOSDBL 88.0		/* LN(MAX_POS_DBLF)		*/
#define LN_MINPOSDBL -89.4		/* LN(MIN_POS_DBLF)		*/
#define DTANH_MAXARG 16			/* |DTANH(maxarg)| = 1.0	*/
#define DSQRT_MPDF   1.304380e19	/* DSQRT(MAX_POS_DBLF)		*/
#define X6_UNDERFLOWS 3.37174e-7	/* X**6 almost underflows	*/
#define X16_UNDERFLOWS 3.74063e-3	/* X**16 almost underflows	*/

#define datan_coeffs	qzzz1		/* rename to avoid name clash	*/
#define dcos_pcoeffs	qzzz2		/* rename to avoid name clash	*/
#define dcos_qcoeffs	qzzz3		/* rename to avoid name clash	*/
#define dln_pcoeffs	qzzz4		/* rename to avoid name clash	*/
#define dln_qcoeffs	qzzz5		/* rename to avoid name clash	*/
#define dsin_pcoeffs	qzzz6		/* rename to avoid name clash	*/
#define dsin_qcoeffs	qzzz7		/* rename to avoid name clash	*/
#endif


/*
 *	PDP11 HARDWARE DEPENDENCIES
 *
 */

#ifdef pdp11
#define MAX_POS_DBLF 1.7014118e38	/* Max positive double float	*/
#define MIN_POS_DBLF 1.4693680e-39	/* Min positive double float	*/
#define MAX_NEG_DBLF -1.7014118e38	/* Max negative double float	*/
#define MIN_NEG_DBLF -1.4693680e-39	/* Min negative double float	*/
#define MAX_EXPONENT 127		/* Max exponent allowed		*/
#define RECIP_MIN 5.877471e-39		/* MAX_POS_DBLF >= 1/RECIP_MIN	*/
#define RECIP_MAX 1.7014118e38		/* MIN_POS_DBLF <= 1/RECIP_MAX	*/
#define LN_MAXPOSDBL 88.0		/* LN(MAX_POS_DBLF)		*/
#define LN_MINPOSDBL -89.4		/* LN(MIN_POS_DBLF)		*/
#define DTANH_MAXARG 16			/* |DTANH(maxarg)| = 1.0	*/
#define DSQRT_MPDF   1.304380e19	/* DSQRT(MAX_POS_DBLF)		*/
#define X6_UNDERFLOWS 3.37174e-7	/* X**6 almost underflows	*/
#define X16_UNDERFLOWS 3.74063e-3	/* X**16 almost underflows	*/
#endif


/*
 *	VAX11 HARDWARE DEPENDENCIES
 *
 * Temp: same as pdp11
 */

#ifdef vax
#define MAX_POS_DBLF 1.7014118e38	/* Max positive double float	*/
#define MIN_POS_DBLF 1.4693680e-39	/* Min positive double float	*/
#define MAX_NEG_DBLF -1.7014118e38	/* Max negative double float	*/
#define MIN_NEG_DBLF -1.4693680e-39	/* Min negative double float	*/
#define MAX_EXPONENT 127		/* Max exponent allowed		*/
#define RECIP_MIN 5.877471e-39		/* MAX_POS_DBLF >= 1/RECIP_MIN	*/
#define RECIP_MAX 1.7014118e38		/* MIN_POS_DBLF <= 1/RECIP_MAX	*/
#define LN_MAXPOSDBL 88.0		/* LN(MAX_POS_DBLF)		*/
#define LN_MINPOSDBL -89.4		/* LN(MIN_POS_DBLF)		*/
#define DTANH_MAXARG 16			/* |DTANH(maxarg)| = 1.0	*/
#define DSQRT_MPDF   1.304380e19	/* DSQRT(MAX_POS_DBLF)		*/
#define X6_UNDERFLOWS 3.37174e-7	/* X**6 almost underflows	*/
#define X16_UNDERFLOWS 3.74063e-3	/* X**16 almost underflows	*/
#endif

/*	Define some commonly used constants				*/

#define TWOPI		6.2831853071795864769
#define PI		3.1415926535897932384
#define HALFPI		1.5707963267948966192
#define FOURTHPI	0.7853981633974483096
#define SIXTHPI		0.523598776
#define LOG2E		1.4426950408889634073
#define LOG10E		0.4342944819032518276
#define DSQRT2		1.4142135623730950488
#define DSQRT3		1.7320508075688772935
#define LN2		0.6931471805599453094
#define LNSQRT2		0.3465735902799726547

/* Configuration options */

#define MAX_ERRORS	10	/* Limit on number of errors before abort */

/* Function declarations */

extern double dacos();
extern double dacosh(), dasin(), dasinh(), datan(), datan2(), datanh();
extern double dcos(), dcosh(), dexp(), dln(), dlog();
extern double dmax(), dmin(), dpoly(), dscale(), dsign(), dsin();
extern double dsinh(), dsqrt(), dtan(), dtanh(), dxmant();
extern int dxexp();
