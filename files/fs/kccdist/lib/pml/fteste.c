/*)BUILD	$(PROGRAM)	=	fteste
		$(INCLUDE)	=	pml.h
		$(DTOA)		=	1
		$(UTLIB)	=	C:PML
		$(TKBOPTIONS)	= {
			STACK	=	2000
			TASK	=	...FTE
		}
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

char *test1[] = {
    "\n******  TEST #1 --- Generate all errors  ******",
    "",
    "(1) LOG bits, COUNT bits, and CONTINUE bits set.",
    "(2) Error limit set high enough to avoid aborting.",
    "",
    "This test should generate an error message for each",
    "error tested, and the final error count should be printed",
    "when done.",
    "\n",
    0
};

char *test2[] = {
    "\n******  TEST #2 --- Turn off logging  ******",
    "",
    "(1) LOG bits reset.",
    "(2) COUNT bits and CONTINUE bits set.",
    "(3) Error limit set high enough to avoid aborting.",
    "",
    "This test should not log any error messages but the error",
    "count should be the same as test 1.",
    "\n",
    0
};

char *test3[] = {
    "\n******  TEST #3 --- Turn off error counting ******",
    "",
    "(1) LOG and COUNT bits reset.",
    "(2) CONTINUE bits set.",
    "(3) Error limit set to 0.",
    "",
    "This test should not log or count any errors.  The final",
    "error count should be zero.",
    "\n",
    0
};

char *test4[] = {
    "\n******  TEST #4 --- Enable error limit and abort ******",
    "",
    "(1) LOG bits, COUNT bits, and CONTINUE bits set.",
    "(2) Error limit set to 5.",
    "",
    "This test should abort after logging the sixth error.",
    "\n",
    0
};


main()
{
    initialize();
    pmllim(40);
    prtdoc(test1);
    gen_errors();
    printf("\nTotal errors counted = %d\n",pmlcnt());

    log_off();
    pmllim(40);
    prtdoc(test2);
    gen_errors();
    printf("\nTotal errors counted = %d\n",pmlcnt());

    count_off();
    pmllim(0);
    prtdoc(test3);
    gen_errors();
    printf("\nTotal errors counted = %d\n",pmlcnt());

    initialize();
    pmllim(5);
    prtdoc(test4);
    gen_errors();
}


prtdoc(dp)
register char **dp;
{
    while (*dp) {
	printf("%s\n",*dp++);
    }
}


initialize()
{
    pmlsfs(DEXP_OVERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DEXP_UNDERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DSCALE_OVERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DSCALE_UNDERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(NEG_DSQRT,LOG|COUNT|CONTINUE);
    pmlsfs(LOG_OF_ZERO,LOG|COUNT|CONTINUE);
    pmlsfs(LOG_OF_NEGATIVE,LOG|COUNT|CONTINUE);
    pmlsfs(DACOS_BADARG,LOG|COUNT|CONTINUE);
    pmlsfs(DASIN_BADARG,LOG|COUNT|CONTINUE);
    pmlsfs(DTAN_OVERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DCOSH_OVERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DCOSH_UNDERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DSINH_OVERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DSINH_UNDERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DASINH_OVERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DACOSH_BADARG,LOG|COUNT|CONTINUE);
    pmlsfs(DACOSH_OVERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(DATANH_BADARG,LOG|COUNT|CONTINUE);
    pmlsfs(DATAN_UNDERFLOW,LOG|COUNT|CONTINUE);
    pmlsfs(C_DIV_ZERO,LOG|COUNT|CONTINUE);
    pmlsfs(CRCP_OF_ZERO,LOG|COUNT|CONTINUE);
    pmlsfs(DINT_2BIG,LOG|COUNT|CONTINUE);
}

log_off()
{
    pmlcfs(DEXP_OVERFLOW,LOG);
    pmlcfs(DEXP_UNDERFLOW,LOG);
    pmlcfs(DSCALE_OVERFLOW,LOG);
    pmlcfs(DSCALE_UNDERFLOW,LOG);
    pmlcfs(NEG_DSQRT,LOG);
    pmlcfs(LOG_OF_ZERO,LOG);
    pmlcfs(LOG_OF_NEGATIVE,LOG);
    pmlcfs(DACOS_BADARG,LOG);
    pmlcfs(DASIN_BADARG,LOG);
    pmlcfs(DTAN_OVERFLOW,LOG);
    pmlcfs(DCOSH_OVERFLOW,LOG);
    pmlcfs(DCOSH_UNDERFLOW,LOG);
    pmlcfs(DSINH_OVERFLOW,LOG);
    pmlcfs(DSINH_UNDERFLOW,LOG);
    pmlcfs(DASINH_OVERFLOW,LOG);
    pmlcfs(DACOSH_BADARG,LOG);
    pmlcfs(DACOSH_OVERFLOW,LOG);
    pmlcfs(DATANH_BADARG,LOG);
    pmlcfs(DATAN_UNDERFLOW,LOG);
    pmlcfs(C_DIV_ZERO,LOG);
    pmlcfs(CRCP_OF_ZERO,LOG);
    pmlcfs(DINT_2BIG,LOG);
}

count_off()
{
    pmlcfs(DEXP_OVERFLOW,COUNT);
    pmlcfs(DEXP_UNDERFLOW,COUNT);
    pmlcfs(DSCALE_OVERFLOW,COUNT);
    pmlcfs(DSCALE_UNDERFLOW,COUNT);
    pmlcfs(NEG_DSQRT,COUNT);
    pmlcfs(LOG_OF_ZERO,COUNT);
    pmlcfs(LOG_OF_NEGATIVE,COUNT);
    pmlcfs(DACOS_BADARG,COUNT);
    pmlcfs(DASIN_BADARG,COUNT);
    pmlcfs(DTAN_OVERFLOW,COUNT);
    pmlcfs(DCOSH_OVERFLOW,COUNT);
    pmlcfs(DCOSH_UNDERFLOW,COUNT);
    pmlcfs(DSINH_OVERFLOW,COUNT);
    pmlcfs(DSINH_UNDERFLOW,COUNT);
    pmlcfs(DASINH_OVERFLOW,COUNT);
    pmlcfs(DACOSH_BADARG,COUNT);
    pmlcfs(DACOSH_OVERFLOW,COUNT);
    pmlcfs(DATANH_BADARG,COUNT);
    pmlcfs(DATAN_UNDERFLOW,COUNT);
    pmlcfs(C_DIV_ZERO,COUNT);
    pmlcfs(CRCP_OF_ZERO,COUNT);
    pmlcfs(DINT_2BIG,COUNT);
}

gen_errors()
{
    COMPLEX z1, z2;

    z1.real = 1.0;
    z1.imag = 0.0;
    z2.real = 0.0;
    z2.imag = 0.0;
    printf("Testing for DEXP overflow.\n");
    dexp(100.0);
    printf("Testing for DEXP underflow.\n");
    dexp(-100.0);
    printf("Testing for DSCALE exponent overflow.\n");
    dscale(1.0,500);
    printf("Testing for DSCALE exponent underflow.\n");
    dscale(1.0,-500);
    printf("Testing for DSQRT of negative argument.\n");
    dsqrt(-1.0);
    printf("Testing for DLN of zero.\n");
    dln(0.0);
    printf("Testing for DLN of negative argument.\n");
    dln(-1.0);
    printf("Testing for DACOS argument magnitude greater than 1.0\n");
    dacos(2.0);
    printf("Testing for DASIN argument magnitude greater than 1.0\n");
    dasin(-2.0);
    printf("Testing for DTAN overflow\n");
    dtan(HALFPI);
    printf("Testing for DCOSH overflow\n");
    dcosh(LN_MAXPOSDBL+1.0);
    printf("Testing for DCOSH underflow\n");
    dcosh(LN_MINPOSDBL-1.0);
    printf("Testing for DSINH overflow\n");
    dsinh(LN_MAXPOSDBL+1.0);
    printf("Testing for DSINH underflow\n");
    dsinh(LN_MINPOSDBL-1.0);
    printf("Testing for DASINH overflow\n");
    dasinh(2.0 * DSQRT_MPDF);
    printf("Testing for DACOSH argument less than 1.0\n");
    dacosh(0.0);
    printf("Testing for DACOSH overflow\n");
    dacosh(2.0 * DSQRT_MPDF);
    printf("Testing for DATANH argument magnitude >= 1.0\n");
    datanh(-1.0);
    printf("Testing for DATAN underflow\n");
    datan(RECIP_MAX);
    printf("Testing for complex division by zero\n");
    cdiv(&z1,&z2);
    printf("Testing for complex reciprocal of zero\n");
    crcp(&z2);
    printf("Testing for DINT argument has no fractional part\n");
    dint(MAX_POS_DBLF);
}
