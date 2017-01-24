/* Produces suboptimal code for compares vs constant */
extern int sys_errlist[], sys_nerr, t20_nerr, t20_errlist[],
	strserr(), baderr(), errno;

strerror(errnum)
int errnum;
{
    if (1 <= errnum && errnum <= 50
      && (errnum - 1) < sys_nerr)
	return sys_errlist[(errnum - 1)+1];
    if (06000 <= errnum && errnum <= 070000
      && (errnum - 06000) <= t20_nerr)
	return t20_errlist[errnum - 06000];
    if (errnum == -1)			/* On T20/10X this means to use */
	return strserr(errnum);		/* "last system error" */
    if (06000 <= errnum && errnum <= 06000)
	return strserr(errnum);

    if (errnum == -1 && errnum != errno)
	return strerror(errno);

    return baderr(errnum);
}
