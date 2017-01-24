/*
** atoi - convert ascii to integer and floating point
** David Eppstein / Stanford University / 11-Jul-84
*/

entry atoi, atof;

/* ---------------------------------------------- */
/*	convert string of decimal to integer      */
/* ---------------------------------------------- */

atoi(nptr)
char *nptr;
{
    int n, sign;
    n = 0;
    sign = 0;
    while (*nptr == ' ' || *nptr == '\t') nptr++; /* skip spaces */
    if (*nptr == '+') nptr++;		/* skip pos sign */
    else if (*nptr == '-') {
	sign = 1;
	nptr++;
    }
    while (*nptr >= '0' && *nptr <= '9') n = 10 * n + *nptr++ - '0';
    if (sign) n = - n;
    return n;
}

/* ------------------------------------------ */
/*      convert string to floating point      */
/* ------------------------------------------ */

double atof(nptr)
char *nptr;
{
    int sign, exp;
    double n;

    n = 0;
    sign = 0;
    exp = 0;
    while (*nptr == ' ' || *nptr == '\t') nptr++; /* skip spaces */
    if (*nptr == '+') nptr++;		/* skip pos sign */
    else if (*nptr == '-') {
	sign = 1;
	nptr++;
    }
    while (*nptr >= '0' && *nptr <= '9')
	n = 10.0 * n + (double) (*nptr++ - '0');
    if (*nptr == '.') while (*++nptr >= '0' && *nptr <= '9') {
	n = 10.0 * n + (double) (*nptr - '0');
	exp--;
    }
    if (*nptr == 'e' || *nptr == 'E') exp += atoi(++nptr);
    if (sign) n = - n;
    while (exp-- > 0) n *= 10.0; 
    while (++exp < 0) n /= 10.0;
    return n;
}
