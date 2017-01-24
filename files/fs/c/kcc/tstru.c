/* This program sets up various structure-structure copies so
** as to test out the P_SMOVE pseudo-op expansion done by CCOUT.
*/
struct s1 { int a;} s1a, s1b;
struct s2 {int a,b;} s2a, s2b;
struct s3 {int a,b,c;} s3a, s3b;
struct s4 {int a,b,c,d;} s4a, s4b;
struct s5 {int a,b,c,d,e;} s5a, s5b;
struct s6 {int a,b,c,d,e,f;} s6a, s6b;
struct s7 {int a[7];} s7a, s7b;
struct s8 {int a[8];} s8a, s8b;
struct s9 {int a[9];} s9a, s9b;
struct s10 {int a[10];} s10a, s10b;
struct s11 {int a[11];} s11a, s11b;
struct s12 {int a[12];} s12a, s12b;

/* Copy struct to and from local */
void coploc()
{
    struct s5 loc5;
    struct s10 loc10;

    loc5 = s5a;
    s10a = loc10;
}

/* Push various length structs on stack as args */
void pushcall()
{
	dummy(s1a, s2a, s3a, s4a, s5a, s12a);
}

/* Return various length structs */
struct s1 ret1() { return s1a; }
struct s2 ret2() { return s2a; }
struct s3 ret3() { return s3a; }
struct s5 ret5() { return s5a; }

int stoint;		/* Extract member of non-lvalue struct */
int extr()
{
	stoint = ret1().a;
	stoint = ret2().a;
	stoint = ret2().b;
	stoint = ret3().b;
	stoint = ret5().b;
}


int retint(arg)		/* Take 4-elem structure as arg, return a member */
struct s5 arg;
{	return s5a.b;
}


/* Check out expansion of P_SMOVE for various lengths of structs */
tstlen()
{
	s1a = s1b;
	s2a = s2b;
	s3a = s3b;
	s4a = s4b;
	s5a = s5b;
	s6a = s6b;
	s7a = s7b;
	s8a = s8b;
	s9a = s9b;
	s10a = s10b;
	s11a = s11b;
	s12a = s12b;
}
