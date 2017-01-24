/*
 *	This file should be included in any user routine
 *	which accesses the Portable Math Library (PML).
 *
 */

#define PDP10

#ifdef PDP10
#define dasinh		qzzz8		/* rename to avoid name clash	*/
#define dacosh		qzzz9		/* rename to avoid name clash	*/
#define datanh		qzzy0		/* rename to avoid name clash	*/
#define datan2		qzzy1		/* rename to avoid name clash	*/
#endif

/* And some data structures */

typedef struct complex {
    double real;
    double imag;
} COMPLEX;

/*	Define the error codes used internally in library routines	*/

#define DEXP_OVERFLOW	0
#define DEXP_UNDERFLOW	1
#define DSCALE_OVERFLOW	2
#define NEG_DSQRT	3
#define LOG_OF_ZERO	4
#define LOG_OF_NEGATIVE	5
#define DACOS_BADARG	6
#define DASIN_BADARG	7
#define DTAN_OVERFLOW	8
#define DCOSH_OVERFLOW	9
#define DCOSH_UNDERFLOW 10
#define DSINH_OVERFLOW	11
#define DSINH_UNDERFLOW	12
#define DASINH_OVERFLOW	13
#define DACOSH_BADARG	14
#define DACOSH_OVERFLOW	15
#define DATANH_BADARG	16
#define DATAN_UNDERFLOW 17
#define C_DIV_ZERO	18
#define CRCP_OF_ZERO	19
#define DSCALE_UNDERFLOW 20
#define DINT_2BIG	21
#define DPOW_0_TO_NON_POS 22
#define DPOW_NEG_TO_FLOAT 23
/*
 *	Error control values
 */

#define CONTINUE	01	/* Bit to continue processing on error	*/
#define LOG		02	/* Bit to log error (print message)	*/
#define COUNT		04	/* Bit to count error against limit	*/
