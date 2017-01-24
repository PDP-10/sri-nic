/* -*-C-*- inch.h */
/*-->inch*/
/**********************************************************************/
/******************************** inch ********************************/
/**********************************************************************/

float
inch(s)
char *s;

/***********************************************************************
Convert a value field in s[] of the form

	------		------------------------------
	value			units implied
	------		------------------------------
	#.##bp		big point (1in = 72bp)
	#.##cc		cicero (1cc = 12dd)
	#.##cm		centimeter
	#.##dd		didot point (1157dd = 1238pt)
	#.##in		inch
	#.##mm		millimeter (10mm = 1cm)
	#.##pc		pica (1pc = 12pt)
	#.##pt		point (72.27pt = 1in)
	#.##sp		scaled point (65536sp = 1pt)
	------		------------------------------

to inches, returning it as the function value.  A leading plus or  minus
sign is optional.  The letter case of the dimension name is ignored.  No
space is permitted between the number and the dimension.
***********************************************************************/

{
    BOOLEAN neg;
    register char *tc;
    register double flnum, pten;

    flnum = 0.0;
    tc = s;
    neg = FALSE;

    if (*tc == '-')
    {
	++tc;
	neg = TRUE;
    }
    else if (*tc == '+')
    {
	++tc;
	neg = FALSE;
    }

    /* we do not use sprintf() here because we want to be able to easily
    find the dimension string */

    for (; isdigit(*tc); ++tc)	/* collect integer part */
	flnum = flnum*10.0 + (double)((*tc) - '0');

    if (*tc == '.')			/* collect fractional part */
	for ((pten = 10.0, ++tc); isdigit(*tc); (pten *= 10.0,++tc))
	    flnum += ((double)((*tc) - '0'))/pten;

    if (strcm2(tc,"in") == 0)		/* inches */
	;
    else if (strcm2(tc,"cm") == 0)	/* centimeters */
	flnum /= 2.54;
    else if (strcm2(tc,"pt") == 0)	/* points */
	flnum /= 72.27;
    else if (strcm2(tc,"bp") == 0)	/* big points */
	flnum /= 72.0;
    else if (strcm2(tc,"cc") == 0)	/* cicero */
	flnum *= 12.0 * (1238.0 / 1157.0) / 72.27;
    else if (strcm2(tc,"dd") == 0)	/* didot points */
	flnum *= (1238.0 / 1157.0) / 72.27;
    else if (strcm2(tc,"mm") == 0)	/* millimeters */
	flnum /= 25.4;
    else if (strcm2(tc,"pc") == 0)	/* pica */
	flnum *= 12.0 / 72.27;
    else if (strcm2(tc,"sp") == 0)	/* scaled points */
	flnum /= (65536.0 * 72.27);
    else				/* unknown units -- error */

#if    PXLID                            /* must be in DVI driver */
    {
	(void)sprintf(message,
	    "inch():  Unrecognized dimension string [%s];\nexpected one of \
bp, cc, cm, dd, in, mm, pc, pt or sp\n",s);
        (void)fatal(message);
    }
#else                                   /* some other program then */
    {
        (void)fprintf(stderr,"inch():  Unrecognized dimension string [%s];",s);
	NEWLINE(stderr);
	(void)fprintf(stderr,
	    "expected one of bp, cc, cm, dd, in, mm, pc, pt or sp");
	NEWLINE(stderr);
        EXIT(1);
    }
#endif

    if (neg)
	flnum = -flnum;
    return((float)flnum);
}
