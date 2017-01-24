/* TPROTO.C - Test KCC function prototype code
*/
static struct s1 { int a,b,c; } sloc;

/* These should all compile without complaint */
static rvoid(void);
static rchar(char);
static rshort(short);
static rint(int);
static rfloat(float);
static rdouble(double);
static rstruct(struct s1);
static rprintf(int, ...);
static rptr(char *);
static rvptr(void *);

/* Redeclarations, which should also not complain */
static rchar(char c);
static rshort(short s);
static rint(const int);
static rfloat(volatile float);
static rdouble(const volatile double);
static rstruct(struct s1);
static rprintf(int i,...);
static rptr(char *const);
static rvptr(void *const v);

/* Definitions, to see what code is produced */
static rvoid(void) {		return 0;}
static rchar(char c) {		return c;}
static rshort(short s) {	return s;}
static rint(const int i) {	return i;}
static rfloat(volatile float f) {	return f;}
static rdouble(const volatile double d) {	return d;}
static rstruct(struct s1 s) {	return s.c;}
static rprintf(int i,...) {	return i;}
static rptr(char *cp) {		return *cp;}
static rvptr(void *vp) {	return *(char *)vp;}

/* Now attempt calls -- again, shouldn't complain */
static char *cptr;
static int *iptr;
main()
{
	rvoid();
	rchar(1);
	rshort(2);
	rint(3);
	rfloat(4);
	rdouble(5);
	rstruct(sloc);
	rptr(0);
	rptr(cptr);
	rvptr(0);
	rvptr(cptr);
	rvptr(iptr);
	rprintf(1);
	rprintf(1,2,3,4.0,5.0,sloc);
}

/* Now try some things that SHOULD complain */
#if TEST
foosh()
{
	rvoid(0);
	rint(1,2);
	rprintf();
	rstruct(6.0);
	rint((char *)0);
	rptr(iptr);
}
#endif
