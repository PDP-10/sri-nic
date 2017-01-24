#if 0
	This file consists of various small pieces of code which
in the past would tickle some kind of KCC bug or another.  By including
them all in one source file, and seeing if new KCC versions produce any
differences in the resulting .FAI file, it is possible to quickly verify
that no old bugs have been re-introduced.
	To repeat: as far as is known, all the code here DOES now compile
correctly.  Code sequences which do NOT compile correctly are left as
separate BUG*.C files until their respective bugs are fixed, whereupon the
code is then moved into here.
#endif

/* 14-May-89  Bug in CCGEN2's relflush(), which was flushing P_MOVE ops
** without checking to see if they were skipped over.  Astonishing this
** didn't show up before.
*/
bugrlf()
{
    static struct blk {int arr[027];} *a, *b;
    static int c;
    a = b;
    if (c < 0) {
    }
    return rtn(++a);
}

/* 02-Feb-89  Bug in CCOPT's adjstack(), it wasn't checking for things
** like ADD 1,17 and thus flushed ADJSPs without any correction.  Since
** hard to do corrections of this sort, code was fixed to check for
** unusual refs to 17 and punts the optimization if so.
*/
bugastk()
{
    float f[11];
    int x, patch;  
    float prob, fst[5];

    prob =  fst[patch] * f[fun(x,0,10)] + f[fun(x,0,10)];
}

/* 31-Aug-88 Bug in CCOPT's killstack(), it neglected to take doubleword
** instruction stack refs into account.
*/
 
bugkst(i)
{
    struct str { char *cp; int len; } sbuf;
    extern struct str idistr();

    sbuf = idistr(i);
    return (sbuf.len != 0);
}

/* 04-Aug-88 Bug in CCSTMT's primary(), wasn't applying array/funct convs
** to expression before use by -> operator.
*/
cvaptr()
{
    static struct foo { int a,b; } arr[2];
    arr->a = 1;		/* Unconventional syntax, but legal */
}

/* 03-Aug-88 This exposed a bug in CCOPT's hackstack().  It wasn't checking
** the "stackrefs" global or otherwise doing anything about refs which
** generated auto var addresses.
*/
static hakstk(a)
{
    extern barfun();
    char *b[128];
    if (a) barfun(b+128);
    return 1;
}

/* 29-Jul-88 This triggered a bug in CCJSKP's foldjump() which wasn't checking
** before it skipped over a skip "instruction".  CCOUT sometimes expands
** immediate addr refs like CAIL R,(X)Y into XMOVEI 16,(X)Y ? CAML R,16 
** which is why everything needs to call oneinstr() to check before skipping
** over something.
*/
bugfjmp()
{
    static struct { int a; } i[20], *p;
    static int d[10];
    int n;
    while (1) {
	    if (n >= d[p->a] && p>i)
		    continue;
    }
}

/* This bug caused CCTYPE's convbinary() to generate a double cast for the
** lefthand operand (from char to int to long) which confused the code
** generator, which can only handle single implicit casts in assignment ops.
*/
bugasc()
{	char c = 5;
	long l = 2;
	c += l;
}

/* This uncovered a bug in CCOUT's outptr() where it was just outputting
** the symbol part of a pointer initializer without checking for SC_ISTATIC
** indirection.
*/

bugsdf()
{
	static int x[] = {1};
	static int *ip = x;
}


/* The code in FINDCSE combines the first two statements, which otherwise
** come out as IBP and LDB, into an ILDB.  This would be OK except that
** the pointer is still in a register rather than in "cp".  For the
** time being, FINDCSE has been "fixed" to not even bother trying to
** optimize this, but perhaps there is a better way to improve it.
** See comments inside FINDCSE in CCCSE.C about what is going on.
*/
char *csebar;
char *csecp;
bugcse()
{
    int c;

    csecp = (csebar + 1);
    if ((c = *csecp) == 'y')
	c = *++csecp;
}

/* This code is not buggy, but it illustrates something that
** the optimizer could probably do better.  When there is a sequence
** of LDB+IBP instructions, the IBP of one pair could be combined with
** the LDB (or DPB) of the next pair.
*/
char *
optdpb(ext, s, source)
int ext;
char *s, *source;
{
	if (!ext) {
	    *s++ = *source++;		/* copy across */
	    *s = '\0';			/* null terminate */
	}
}

/* This code used to tickle a bug in CCOPT's hackstack()
** which wrongly moved the initial ADJSP 17,1 to produce
** the sequence:
**	PUSH 17,p ? ... ? ADJSP 17,1 ? PUSH 17,1 ? PUSHJ ...
*/
udp_recv(p, waitp)
{   int err;
    if( err = udp_jsys((waitp ? 0:1), p))
	logpkt();
}

/* This code tests the stuff that handles
** compound assignment, to verify that it only evaluates the left-hand
** expression ONCE.
*/
int i,arr[10];
bugass()
{
	while(i)
		arr[i++] += 2;
}

/* This code was used to demonstrate the old bug that a pointer to
** a single "char foo" declaration actually became an int pointer instead
** of a char pointer.
**	This also demonstrated a newer bug that a pointer to a char or
** short within a structure would generate local-format byte pointers (as
** for bit-fields) instead of true global byte pointers (as for non-bitfield
** byte objects).
*/
char bcpfoo;
char bcpbar[24];
struct ts {	int a;	char b;	int c;	char d[20]; } tsdef, *tsp;
bugcp()
{
	bcpfoo = 'A';
	call(&bcpfoo);
	call(&bcpbar[0]);
	call(&tsdef.b);
	call(&tsdef.d[0]);
	call(&tsp->b);
	call(&tsp->d[0]);
}

/* This used to produce bad code for the float-to-double conversion.
*/
bugflt(ugh)
int ugh;
{
	printf("Help!  %g\n", (float)ugh);
	putchar(ugh);
}

/* This used to be "optimized" into something that set "fext" to 0 instead of
** 1234.0!
*/
float fext;
bugu2f()
{
	int foo = 1234;
	float bar;

	bar = (float) foo;
	fext = bar;
}


/* This code used to illustrate that DFDV wasn't given a literal operand.
** Not bad, just suboptimal.
*/
optdbl(bar)
double	bar;
{
	bar /= 1000.0;
}

/* This used to trigger some kind of bug in the register spilling code
** such that there were leftover regs.
*/
bugdsp(Smx,E,incl,Rap,Prp)
double Smx,E,incl,*Rap,*Prp;
{
	extern double pow(), cos();
#define SQR(x) ((x)*(x))
#if 0
	/* This statement compiled OK */
  *Rap = 9.95*pow(6.1/Smx,3.5) * cos(incl)	/ SQR(1-SQR(E)) * (3.1/180);
#endif
	/* This one barfed */
  *Prp = 4.97*pow(6.1/Smx,3.5) * (5*SQR(cos(incl))-1)
						/ SQR(1-SQR(E)) * (3.1/180);
}

/* This bug came from SRA and illustrated an oversight in the code
** that adjusted stack references; it was missing refs in literal
** bytepointers.
*/
struct mumble {
    unsigned a : 2;
    unsigned b;
} stuff = { 1, 2 };

bugsra(h)
struct mumble *h;
{
    int c;
    if(!h->a)
	return 0;
    return 1;
}

/* This code used to compile a POP instr which made a following stack
** reference have a positive offset, i.e. to a non-ex part of stack!
*/
#include <time.h>

struct tm *
bugtim(clock)
long *clock;
{
    unsigned i,j, acs[5];
    struct tm *t;
    static struct tm ctmbuf;

    t = &ctmbuf;
    if (!jsys(0123,acs)) return(0);	/* something went wrong */
    t->tm_yday = (acs[2] & 0777777) - 1;	/* day of year */
    return t;
}

#if 0
This one actually triggered two different bugs, one to do with
union member handling (didn't start each at offset 0) and one to do
with over-optimization of BP stack refs or something like that.

Date: Mon, 16 Mar 1987  20:57 EST
From: Rob Austein <SRA@XX.LCS.MIT.EDU>
To:   Bug-KCC@SRI-NIC.ARPA
Subject: Another bug for you (unions and structs, wrong code generated)

I must admit that this is one of the more amusing compiler errors I
can recall seeing.  It is clearly on drugs by the time it gets to the
return() statement....

Here's the C code:
#endif

unsigned a,b,c,d;

int
buguns()
{
    union {
	int number;
	struct {
	    unsigned   : 4;
	    unsigned a : 8;
	    unsigned b : 8;
	    unsigned c : 8;
	    unsigned d : 8;
	} address;
    } crock;
    crock.number = 0;
    crock.address.a = a;
    crock.address.b = b;
    crock.address.c = c;
    crock.address.d = d;
    return(crock.number);
}

/* This code compiled OK but was suboptimal, making ASH
** use an index register for its operand rather than an immediate constant.
*/
int bugz_i;
bugz()
{	bugz_i >>= 20;
}

#if 0
	This code loses for the case where an "IDIVI from,x" exists, and
to+1 (ie the new remainder-clobbered register) happens to be a register
that contains a value used farther on in the code buffer.  In this lossage
case, the register had been freed, but subexpression optimization had
found it still contained a useful value, and thus re-used that value.
Thus bug is demonstrated by the file BUGDIV.C.
Until this all gets figured out, simplest to avoid messing with IDIV
completely.  Sigh.  --KLH
	Problem is in CCCREG's "creg" routine.
#endif

typedef	struct TACtype {
	unsigned hostnum;   
	char	name[40];   
	int	ctime[0100];	 
	int	ports[0100];   
	struct TACtype *next;   
} tnode, *tptr;
 
bugdiv(curtac)
tptr	curtac;   
{
int	i,j;   
int	total;   
int	tmins;	 

	for (i=0;i<0100;++i) {
		total+=curtac->ports[i];
		tmins +=curtac->ctime[i];
	}
	tmins/=60;
	j=(tmins/total/10); 
	printf("foo",tmins,total,tmins/total,j);
}

#if 0
report:
	PUSH 17,[0]
	ADJSP 17,3
$1==.
	MOVE 5,-5(17)
	ADD 5,-3(17)
	MOVE 7,113(5)
	ADDB 7,-1(17)
	MOVE 6,13(5)
	ADDB 6,0(17)
	AOS 10,-3(17)
	CAIGE 10,100
	 JRST $1
	IDIVI 6,74		<=== BAD!! Clobbers 7 which is needed later!
	MOVEM 6,0(17)
	IDIV 6,7
	IDIVI 6,12
	MOVEM 6,-2(17)
	PUSH 17,6
	MOVE 4,-1(17)
	IDIV 4,-2(17)
	PUSH 17,4
	PUSH 17,-3(17)
	PUSH 17,-3(17)
	XMOVEI 3,$4
	IOR 3,$BYTE*1+4
	PUSH 17,3
	PUSHJ 17,printf
	ADJSP 17,-11
	POPJ 17,
$4==.
	BYTE	(7) 146,157,157,0
#endif

/* This demonstrated an optimizer bug with IDIV where a preceding
** MOVE R,S would be swapped with the IDIV; CCOPT's foldboth() was the
** culprit.
*/

struct tm *		/* tm struct already defined by previous include */
bugdv2(t, utim)
struct tm *t;
long utim;
{
    register int i, days, secs, yday, year;

    days = utim / (24*60*60);		/* Get # of days */
    secs = utim % (24*60*60);		/* and remaining # secs in day */
    t->tm_sec = secs % 60;
    t->tm_min = (secs / 60) % 60;
    t->tm_hour = (secs / 60) / 60;
    t->tm_wday = (days + 4) % 7;	/* epoch was a Thurs, must adjust. */
    t->tm_isdst = 0;			/* GMT, so DST never applies */
    
    return t;
}

/* Not sure what the problem with this one was, but including it anyway. */
findtorg(op)
{	for (; op; op = op*2)
	  {	if(op == 3)
		  {	if(op += 4) return(5);
		  }
		else if(op == 4) return(op);
	  }
	return(0);
}
/* Probably the above code led to the following case, which at one time
** produced broken code; the first break went to the
** else statement rather than leaving the while loop.
*/
int brkfoo;
bugbrk()
{
    while (1) {
	if (brkfoo == 5)
	    break;
	else if (brkfoo == 3)
	    break;
    }
}

/* This bug came from NIS (they sell ACCENT-R), was quickly fixed */
void CMCFEP()
{
   char modtyp[3];
   int  l18epl[5];

   l18epl[3] = (int) modtyp;
}
/*
Error at CMCFEP+6, line 7 of kccbug.c:
}
Internal error - evaling array or function.
*/

/* This produced something like a register allocation error because
** the number of temporary regs needed for the expression was greater
** than the number of PDP-10 acs available.
*/
double pow(), cos();
GetPrec(Smx,E,incl,Rap,Prp)
double Smx,E,incl,*Rap,*Prp;
{
  *Prp = 4.97	* pow(6378.16/Smx, 3.5)
		* (5 * ((cos(incl)) * (cos(incl))) -1)
		/ ((1-((E)*(E))) * (1-((E)*(E))))
		* (3.14159265/180);
}
GetBearings(){}

/* This code used to cause lossage because the compiler would interpret
** the ';' as starting a comment, and strip off the remainder of the line.
*/
bugasm()
{
#asm
	MOVE	2,[POINT 7,[ASCIZ ".-1;P770000"]]
	MOVEM 2,123
#endasm
}

/* This routine tests ADD/SUB series optimization.  The
** expression (a-(b+c+d)) is changed to (a-b-c-d), i.e. a series of SUBs
** instead of ADDs into a SUB.
*/
bugsub()
{	int a,b,c,d, i;

	return a - (b+c+d);
}

/* This code triggered a CCOPT hackstack() bug.  By moving the ADJSP
** when it shouldn't have, it made the store of sbp into sb go OK, but then
** it next assignment went to sb also instead of into sd!!!
*/
struct SBBUF {	char *sbiop; int a[4]; int flags; long sboff;
		struct sdblk *sbcur; };
struct sdblk {	int a[4]; struct smblk *sdmem; };
struct smblk {	int a[3]; char *smaddr; };

bugsb(sbp)
struct SBBUF *sbp;
{	register struct SBBUF *sb;
	register struct smblk *sm;
	register struct sdblk *sd;

	if((sd = (sb=sbp)->sbcur) == 0)
		return(0);
	if(sb->sbiop && (sm = sd->sdmem))
		return(1);
}

/* This code caused KCC to generate an ADJBPI instruction!
*/

bugbpi()
{
	char *rp;
	rp = 0;
	rp += 12;
	foo(rp);
}

/* This code didn't generate a bug of itself, but is handy as it
** tests all the current pathways and optimizations possible for the
** unsigned integer division code sequence, which is generated
** by CCOUT's simuidiv() routine.
*/

tudiv()	/* Test Unsigned Division */
{
	static unsigned pos = 01000000, neg = -2, pvar = 0123, pv1 = 1;

	printf("UIDIV results:\n\
pos is %o, neg is %o, pvar is %o.\n\
setz	pos/0 = %o ? %o\n\
setz	pos/1 = %o ? %o\n\
lsh	pos/01000 = %o ? %o\n\
divi	pos/01001 = %o ? %o\n\
div []	pos/1,,1 = %o ? %o\n\
case1	pos/pvar = %o ? %o\n\
case2	neg/pvar = %o ? %o\n\
case2.1	neg/[1]  = %o ? %o\n\
case3	pos/[-1] = %o ? %o\n\
case4.1 neg/[-1] = %o ? %o\n\
case4.2	neg/neg  = %o ? %o\n",
		pos, neg, pvar,
		pos/0, pos%0,
		pos/1, pos%1,
		pos/01000, pos%01000,
		pos/01001, pos%01001,
		pos/01000001, pos%01000001,
		pos/pvar, pos%pvar,	/* case 1 */
		neg/pvar, neg%pvar,	/* case 2 */
		neg/pv1, neg%pv1,	/* case 2.1 */
		pos/(-1), pos%(-1),	/* case 3 */
		neg/(-1), neg%(-1),	/* case 4.1 */
		neg/neg, neg%neg	/* case 4.2 */
	);
}

/* This code used to confuse KCC owing to the void cast of a constant.
** This also tests to ensure warnings aren't generated when discarding
** implicit casts (such as happen within the stdio putc macro).
*/
int bregfl;
bugreg(i)
{
    char *cp;
    (bregfl ? (void)foo() : (void)0);	/* Should ignore the 0 completely */
    (bregfl ? (*++cp = i) : fputc(i));	/* Discards implicit char->int cast */
}

/* This demonstrated an over-optimization bug which completely took
** out the call to "foo()", not realizing that control could jump
** into it.
*/
buglab(i)
{
	i += 4;
	if (0) {
 badlab:	foo();
	}
	if (i) goto badlab;
	return i;
}

/* This code tickled two bugs, one of which had to do with
** completely missing the "b = 0" operation, and the other which
** produced a SETZ 1,-1(17); it worked but was rather misleading.
** Both were fixed.
*/
bugstz()
{
	struct header {
		unsigned he_byt : 8;
		unsigned he_bit : 1;
	} *hptr;
	int a,b,c;
	if(b&040) b=1;
	b = 0;
	a = hptr->he_byt;
	return(a+b);
}

/* This produced wrong code -- it tickled a bug in foldskip()'s
** optimization.
*/

bugpem(x)
double x;
{	register int i;
	i = x * 42 + 0.5;
	if ((i += 42 + 666) < 1)
	    i = 1;
	return(i);
}

/* This one complained about non-constant in case stmt, because the
** cast to (char) wasn't being folded.
*/
bugcas(i)
{
	switch (i) {
		case 0:	return 1;
		case (char)-1: return 2;
		default: return 0;
	}
}

/* This bug caused an indexed byte pointer to be pushed on stack as
** a function arg (see enclosed sample of assembly output).
*/
struct bugshortfoo {
	int morejunk;
	struct {
		short count, type, npg[10];
	} data;
	int junk;
};
bugshort(ptr)
struct bugshortfoo *ptr;
{	char no[20];

	if (!scan(no, ptr->data.npg, &(ptr->data.count), &(ptr->data.type)))
		return (0);
	return 1;
}
#if 0
	MOVE 4,-1(17)
	PUSH 17,[2200,,152(4)]
	PUSH 17,[222200,,152(4)]
	ADDI 4,145
	TLO 4,$$BPH0
	PUSH 17,4
	PUSH 17,[$$BP90+$$SECT,,no]
	PUSHJ 17,scan.n
#endif

/* This one tickled a bug in sizeptobj() that didn't correctly compute the
** size of a multidimensional char array.  The same bug afflicted a similar
** computation in the generation of pointer constants.
*/
char bug55a [5][5];	/* 5x5 array is 7 words (25/4 rounded up) */
char *bug5p0 = (char *)(bug55a+0);
char *bug5p1 = (char *)(bug55a+1);
char *bug5p2 = (char *)(bug55a+2);
char *bug5p3 = (char *)(bug55a+3);
char *bug5p4 = (char *)(bug55a+4);
bug55()
{
	strcpy(bug55a,"bif");
	strcpy(bug55a+1,"foo");
	strcpy(bug55a+2,"bar");
	strcpy(bug55a+3,"baz");
	strcpy(bug55a+4,"bin"); 
}

/* This one tickled a bug in CCJSKP's crossjump() which did not check for
** AOS/SOS (modifying memory) when scanning back over skips prior to pulling
** a JUMPx down over them.
*/
bugfor()
{
	int i,w;

	for (w = 0, i = 0; i < 1; i++) {
	    if ((w = func()) > 0)
		break;
	}
	/* At this point, should have i == 0.
	** Instead we have i == 1 !! 
	*/
	return i;
}

/* This uncovered a bug in CCOPT's foldboth() where a sequence of
** MOVEM R,x / AOS S,x was turned into ADDI R,1 / MOVEM R,x / MOVE S,R
** and then the optimizer proceeded to flush R entirely, so that future
** references to R found garbage in the register.
*/
bugsqr()
{
	static int i;
	i = 5;
	return ++i*++i;
}

/* The following illustrates a preprocessor problem with recursive macros.
** KCC 580 generated incorrect code for this due to unsuccessful attempts
** to do ANSI-style preprocessing.
*/
#define updcrc(cp, crc) ( crctab[((crc >> 8) & 255)] ^ (crc << 8) ^ cp)
bugrmac()
{
	register unsigned short oldcrc;
	static unsigned short crctab[1] = {0x0000};

	oldcrc = updcrc(0,updcrc(0,oldcrc));
}

/* Optimization irritation:
** This code produced an IORI ac,0 which is meaningless.
**	Looks as if the constant folding code cannot deal with rearranging
** commutative ops very well, so it never discovers that it can fold the
** two OR operations.  Actually, it could also discover that an IOR of 0 is
** unnecessary and just flush that!
**	This has been fixed by some associative op evaluation.
*/

bugfld(i)
{
    return i + 0400 + 01000;		/* This barfed during development. */
}
bugior(ufx, arg)
int ufx, *arg;
{
    int ablock[5];
    if (arg[2] & 0x00000020 ) {			 
	ablock[2] = (ablock[2] & ~0300) | 0000 | 170000;		 
	ufx &= ~(01) ;	 
    }
    return 0;					 
}

/* This code uncovered a bug in the sizeof stuff.
*/
bugsiz()
{
	static struct {
		char f[5];
	} foo;
	return sizeof(foo.f);	/* Should be 5 */
}

/* This provoked somewhat spurious warnings about discarded operator,
** when it was just discarding the 0 that sorta had to be there.
*/
bugdisc()
{
	(getc() ? putc() : 0);
}

/* This exhibited a preprocessor problem where a macro definition would
** reduce all whitespace in the body into just one space, even if the
** whitespace was inside a string literal.
*/
#define BUGSTRMAC "This is two blanks:  : end"
bugstrmac()
{
	static char str[] = BUGSTRMAC;
}

/* This code tickled a bug in the old CCGSWI routine that generated switch
** statements; it assumed a null statement for a case label was the same
** thing as "break", and case 1 below never called bar()!!
*/
bugswi(i)
{
	switch(i) {
	case 1: ;
	case 2: bar();
	}
}

/* This tripped over yet another optimizer bug in CCJSKP where optlab() was
** removing a MOVEM without checking to see whether it was skipped over.
*/
bugmvm()
{
    static int maxbits = 16;
    if(maxbits < 9) maxbits = 9;
    if (maxbits > 16) maxbits = 16;
    return maxbits;
}

/* This demonstrated a preprocessor oversight, sort of.
** The old C definition was unclear about whether newline should be considered
** whitespace or not while parsing a macro invocation.  ANSI now says yes,
** so that macro invocation looks more like a function call.
*/
#define MACEOL(a,b) foo(a,b)
bugmeol()
{
    MACEOL
	(1,
	    2);
}

/*	This exposed a fatal weakness in gternary() and the peephole optimizer.
** While the bug is "fixed" in the sense that correctly working code should
** always be produced, it is still not being done the Right Way.  See the
** comments at gternary() for more information.
*/
bugter()
{
	static int x, y, z;
	static char *pp, *a;
	extern char *strrchr();

	/* Case 1: Screwed up on realizing AC 1 was in use */
	a = strrchr();
	pp = strrchr();
	a = (a == ((char *)0)) ? pp : a;
	foo();			/* break up flow */

	/* Case 2: Screwed up with plain AC too */
	pp = "foo";
	a = (a == ((char *)0)) ? pp : a;

	/* Case 3: OK but optimizer had trouble with this */
	x = (y ? z : 0);
}

/* This one (from ALAN) tickled an oversight in cregok() of CCCREG which
** thought it had finished changing regs when it hadn't.
*/
void
bugtr2(val, cnt)
  int val, cnt;
{
    extern void padcount();
    int nread;
    nread = ((val > 0) ? val - 1 : val + cnt);
    padcount(nread);
}
#if 0
    screw:
	    SKIPG 1,-1(17)	; SKIPG 5,-1(17) would fix it
	     SKIPA 5,-2(17)
	     SOSA 5,5		; SOSA 5,1 would fix it
	     ADD 5,-1(17)
	    PUSH 17,5
	    PUSH 17,5
	    PUSHJ 17,padcou
	    ADJSP 17,-2
	    POPJ 17,
#endif

/* This tickled a bug in CCOPT's foldinc() routine which pushed an AOS
** too far back up in the peephole buffer.
*/
bugfinc(rt, entry, data)
int *rt, *entry, data;
{
    int table_size, used, *ne;

    table_size = *rt & 0777777;			/* RH is size of table */
    used = ((unsigned) *rt) >> 18;		/* LH is # entries so far */
    if (++used > table_size) return 0;		/* no room for new entry */
    ne = rt + used;				/* new table entry here */
    *ne = ((int) entry << 18) | (data & 0777777);	/* here it is! */
    *rt = (used << 18) | table_size;		/* update table head word */
    return 1;					/* success!!! */
}

/* This tripped over a bug in the parser; nextoken() was always
** flushing whitespace between chars of multi-char tokens, so
** that "a + ++b" was seen as "a++ + b".  Sigh.
*/
buginc()
{
    int a, b;

    a = 1;
    b = 2;
    printf("a (1) + ++b(2+1=3) = %d, expecting 4\n", a + ++b);
}
#if 0
the printed result is 3
-------
#endif

/* This tripped over a situation in CCGEN2 where genexpr() was returning
** a NULL vreg pointer to gboolean, which blithely stuck it into a
** pseudocode instruction and thus caused a JUMPE 0, which caused other
** problems.
*/
bugnor()
{	static int a, b, c;
	a && (b = c);
}

/* This bug was introduced by the new constant optimization code,
** where (ptr[0]) became (ptr + 0) which became (ptr) without
** retaining the type conversion of the result (array auto-conversion
** strikes again).
*/
char *
bugmim(i)
{
    static char *foo[2][5];
    return foo[0][i];
}

/* This provoked an internal error while trying to evaluate an array.
** It should cause an "implicit conversion" warning.
*/
bugcar()
{
    static struct prncode {
	char *prncap[32][10];
    } printer;
    char *s2;
    s2=printer.prncap[25];
}

/* This illustrates a change to KCC such that it adopts the ANSI
** approach for undefined identifiers within an #if or #elif expression;
** "(0)" is substituted for those.  A warning is given, however.
*/
#if (123+(5/2)*barf)
#elif morebarf
#endif
