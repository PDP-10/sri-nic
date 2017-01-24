21-Mar-89 08:18:57-PST,47457;000000000000
Return-Path: <BEEBE@SCIENCE.UTAH.EDU>
Received: from SCIENCE.UTAH.EDU by SRI-NIC.ARPA with TCP; Tue, 21 Mar 89 08:14:24 PST
Return-Path: <TURBOC-L@UCF1VM.BITNET>
Received: from CC.UTAH.EDU by SCIENCE.UTAH.EDU with TCP; Sun 19 Mar 89 23:25:06-MST
Received: from ADMIN.BYU.EDU by CC.UTAH.EDU; Sun, 19 Mar 89 20:04 MST
Received: by BYUADMIN (Mailer R2.01A) id 4352; Sun, 19 Mar 89 04:49:18 MST
Date: Tue, 14 Mar 89 13:55:53 EST
From: MTSJMC@GSUVM1.BITNET
Sender: "TURBO C Discussion group." <TURBOC-L@UCF1VM.BITNET>
To: Nelson Beebe <Beebe@SCIENCE.UTAH.EDU>
Reply-to: "TURBO C Discussion group." <TURBOC-L@UCF1VM.BITNET>
ReSent-Date: Tue 21 Mar 89 09:14:08-MST
ReSent-From: "Nelson H.F. Beebe" <Beebe@SCIENCE.UTAH.EDU>
ReSent-To: klh@SRI-NIC.ARPA
ReSent-Message-ID: <12479804600.36.BEEBE@SCIENCE.UTAH.EDU>

ANSI C standard test code
     
This (I hope) is the code that Tamura requested this morning. It's
really big, but I figured alot of people will want to try it out.
     
---------- Start of CLEANUP.BAT ----------
del %1.obj
del %1.exe
del %1.c
---------- End of CLEANUP.BAT ----------
---------- Start of CLEANUP.SH ----------
rm $1.c $1.o $1
---------- End of CLEANUP.SH ----------
---------- Start of EXAMPLE.BAT ----------
runtest "COMPILE -FLAGS %%s" "LNK %%s" "cleanup %%s" <sampler.88 >EXAMPLE.out
summtest EXAMPLE
---------- End of EXAMPLE.BAT ----------
---------- Start of EXAMPLE.SH ----------
runtest "cc -c %s" "cc -o %s %s.o" "cleanup.sh %s" <sampler.88 >xxx.out
summtest EXAMPLE
---------- End of EXAMPLE.SH ----------
---------- Start of README ----------
THE PLUM HALL VALIDATION SAMPLER
     
On September 29, 1988, the committee X3J11 unanimously voted
to forward the current proposed Standard for C to X3 for final
approval.  We expect that there will be no further changes to
this standard before its eventual publication in early 1989.
This vote completes five years of work by several hundred
members of X3J11.
     
Now that we know exactly what Standard C looks like, many people
will want to know "How close are compilers X, Y, and Z to the
ANSI Standard?"
     
Certain facts must be noted:  The ANSI Standard for C has no subsets.
There is no "partial conformance" to this Standard, and no "degrees
of conformance".  Vendors have been requested not to specify or claim
conformance to any of the various working drafts, even including the
current proposed Standard, which becomes a full
American Standard only upon its official publication by ANSI.
Even at that point, its acceptance as an International Standard
depends upon further decisions by the appropriate bodies of ISO.
     
Nonetheless, until most compilers achieve official certification
of conformance to the Standard, it is useful to have an unofficial
estimator of "how close" the compilers are.
     
The file  sampler.88  contains the C source for 79 compilable
tests that "sample" the closeness of a compiler to the soon-to-be
ANSI standard for C.
This "sampler" in no way attempts to measure conformance
to the (current or future) Standard.  It simply estimates the number
of areas in which a current compiler will need to be modified in
order to achieve eventual conformance to the Standard.  Compared to
the megabytes of source code in a full Validation Suite for C, it is
relatively tiny.
     
So much for caveats.  On the positive side, any compiler which
embodied all the features of the May 1988 draft would pass all
79 of the tests in this file.  Most of the tests in the "sampler"
reflect aspects of ANSI C which have been unchanged since mid-1986;
13 tests reflect 1987 decisions, and 3 tests reflect 1988 decisions.
     
WHEN THE 1989 GENERATION OF COMPILERS START TO SATISFY ALL OR MOST
OF THESE TESTS, WE WILL TRULY HAVE SYNTACTIC PORTABILITY IN C.
     
A word about the vendors:  As of the date these tests were first
run (October 1988), several vendors already had beta-test versions
of their products which scored substantially better.  Because of
editorial lead times, some of these beta versions will reach the
market by the time this article appears.  Some vendors have chosen
to wait until the Standard is announced officially before releasing
compilers that are tracking that Standard.  Both observations lead
to this conclusion: You should inquire about the CURRENT situation
of each compiler that you are considering.
     
I wish to express my gratitude to the authors of these compilers for
the contribution that they have made to the field of programming.
Each had unique personal reasons for undertaking a project of this
magnitude.  Comparisons based on this "sampler" are in no way meant
as determinations of the "quality" of their compiler; they are simply
a snapshot of one aspect of the situation at this point in time.
Each "score" in the table could, in fact, be off by one or two points,
because some tests were changed slightly during the review process.
     
A thorough validation of a compiler requires much more work than
this sampler provides. Plum Hall is proud to announce that the British
Standards Institution's recent competitive evaluation has selected
the Plum Hall Validation Suite for C.  It will be used by BSI,
IMQ (of Italy), and AFNOR (France) in the European C Validation Service.
(Contact John Souter of BSI for information at UK 908-220-908,
FAX UK 908-320-856.) In the USA, the National Bureau of Standards
(recently re-named National Institute of Science and Technology)
is planning the American validation service. (Contact
L Arnold Johnson at USA 301-975-3247.)
     
For information about the Plum Hall Validation Suite, contact
Joan Hall at Plum Hall Inc, USA 609-927-3770, FAX USA 609-653-1903.
     
The sampler was written by Thomas Plum, Plum Hall Inc,
USA 609-927-3770, uunet!plumhall!plum,
and Ralph Ryan, Chiron Systems Inc,
with assistance from Joan Hall, Don Gallagher, and Scott Erlichman.
     
FROM THE AUTHOR: I started working in C in 1976, and began teaching
it professionally in 1978.  Out of this teaching came five textbooks,
one in collaboration with Jim Brodie, the Chair of X3J11.
     
Joan Hall and I have worked in the X3J11 project
since 1983.  We believe the committee has truly performed a service
to the computing community.  I was chosen to be Vice-Chair in 1983,
largely because I had no bias toward any particular vendor of C.
     
In 1985, Ralph Ryan invited us to become partners in the project
that became the Plum Hall Validation Suite for C.  It has been the
major focus of Plum Hall for three years now.
     
All of this work is our contribution toward programmers being able
to write in C with certainty that, if portably written, their
programs will in fact work in each new environment.
     
--------------------
How to execute the Sampler:
     
The program  runtest.c  will, when compiled, produce a program that
splits the file  sampler.88  into 79 individual C source files, such as
a001.c .  It will compile, link, and execute each of them.  Each
of the resulting executable files will produce an output file such as
a001.out .  The command-line arguments to  runtest  are as follows:
     
    "compile-cmd"  -- how to compile (e.g., "cc -c %s")
    "link-cmd"     -- how to link  (e.g., "cc -o %s %s.o")
    "cleanup-cmd"  -- one or more commands that remove source (%s.c),
                      object, executable, map, etc.  BUT NOT .out !
    <sampler.88    -- take standard input from the sampler file
    >xxx.out       -- capture the compiler diagnostics for future
                      reference.
     
Some of these samples will throw some compilers into infinite loops
or "hard hang-ups".  If this happens to you, determine which sample
caused the problem, edit the sampler.88 file, delete the cases up to
and including the fatal case, and run  runtest  with the remainder
of the file.
     
When all cases have been compiled, and the successfully-compiled cases
have been executed, the other program  summtest  will examine all the
a???.out files and produce a one-column summary of the results.  These
one-column summaries can be pasted together into a table of results.
     
-----------------------
Permissions:
     
 Permission is granted for reproduction and use of this "Sampler"
 provided that its enclosed authorship notice is reproduced entirely.
     
     
 Tom Plum, Plum Hall Inc, 609-927-3770,  uunet!plumhall!plum .
 Ralph Ryan, Chiron Systems Inc.
 With assistance from Joan Hall, Don Gallagher and Scott Erlichman.
---------- End of README ----------
---------- Start of RUNTEST.C ----------
/* runtest - run the Plum Hall Validation Sampler */
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#define NO_OUTPUT 0
#define NO_SUCCESS 1
#define SUCCESS 2
main(argc, argv)
    int argc;
    char *argv[];
    {
    FILE *outfp;
    FILE *pgmfp;
    char buf[BUFSIZ];
    char cmd[BUFSIZ];
    char out_buf[BUFSIZ];
    char out_name[10];
    char pgm_name[10];
    char pgm_c_name[10];
    int i;
    int pgm_num;
    static char comp_fmt[130] = "cc -c %s.c\n";
    static char link_fmt[130] = "cc -o %s %s.o\n";
     
    if (argc > 2)
        {
        strcpy(comp_fmt, argv[1]);
        strcpy(link_fmt, argv[2]);
        }
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    fgets(buf, BUFSIZ, stdin);
    while (!feof(stdin))
        {     /* for each input program */
        pgm_num = atoi(buf+3);
        sprintf(pgm_name, "a%03d", pgm_num);
        sprintf(pgm_c_name, "a%03d.c", pgm_num);
        pgmfp = fopen(pgm_c_name, "w");
        if (pgmfp == NULL)
            {
            fprintf(stderr, "cannot create %s\n", pgm_c_name);
            exit(2);
            }
        do {    /* for each line of this program */
            fprintf(pgmfp, "%s", buf);
            fgets(buf, BUFSIZ, stdin);
            } while (!feof(stdin) && strncmp(buf, "/*", 2) != 0);
        /* now program is complete; test it */
        fclose(pgmfp);
        sprintf(cmd, comp_fmt, pgm_name, pgm_name);
        printf("COMPILE: %s\n", cmd);
        system(cmd);
        sprintf(cmd, link_fmt, pgm_name, pgm_name);
        printf("LINK: %s\n", cmd);
        system(cmd);
        sprintf(out_name, "%s.out", pgm_name);
        sprintf(cmd, "%s >%s", pgm_name, out_name);
        printf("EXECUTE: %s\n", cmd);
        system(cmd);
        for (i = 3; i < argc; ++i)
            {
            sprintf(cmd, argv[i], pgm_name, pgm_name);
            printf("CLEANUP: %s\n", cmd);
            system(cmd);
            }
        }    /* end loop over each program */
    exit(0);
    }
---------- End of RUNTEST.C ----------
---------- Start of SAMPLER.88 ----------
/*   1: 2.1.1.2  Backslash-splicing */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    i\
n\
t\
    i = 3;
    if (i == 3)
        printf("success\n");
    return 0;
    }
     
/*   2: 2.1.1.2  Phases of translation */
    /* #define NOTDEFINED 1 */
??=include/* SPACE */<stddef.h>
#\
i\
f\
 !\
defined(NOTDEFINED)
#define UI unsigned in??/
t
     
#define/* SPACE */PCHAR(/*SPACE*/x/*SPACE*/)/* SPACE */char x/*SPACE*/??(??)
     
#endif
UI/* SPACE */Ui/* SPACE */=/* SPACE */'\
\n' ;
PCHAR(Pc) = "string1??/
#\
"\
"string2" ;
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (Ui == '\n' && Pc[8] == 's')
        printf("success\n");
    return 0;
    }
     
/*   3: 2.2.1.1  Trigraphs */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    ??<
    int i = 3;
    if (i == 3)
        printf("success\n");
    ??>
    return 0;
    }
     
/*   4: 2.2.4.2  <float.h> -- e.g., DBL_DIG >= 10  [87/03]   */
#include <float.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (DBL_DIG >= 10)
        printf("success\n");
    return 0;
    }
     
/*   5: 2.2.4.2  <limits.h> -- e.g., has MB_LEN_MAX  [87/12]   */
#include <limits.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (CHAR_BIT >= 8 && INT_MIN <= -32767 && MB_LEN_MAX >= 1)
        printf("success\n");
    return 0;
    }
     
/*   6: 3.1.2    Internal identifier significance at least 31 characters */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    int a23456789012345678901234567890a = 3;
    int a23456789012345678901234567890b = 4;
    if (a23456789012345678901234567890a == 3 &&
        a23456789012345678901234567890b == 4)
        printf("success\n");
    return 0;
    }
/*   7: 3.1.2.2  Scope rules: file, function, prototype, and block */
#include <stdio.h>
int x = 2;          /* this x has file scope */
main()
    {
    int start = (printf("started\n"));
    int f();
     
    if (f() == 1)
        printf("success\n");
    return 0;
    }
int f()
    {
    int c(int x);   /* this x has prototype scope */
    if (x != 2)
        exit(2);
    else
        {
        char x = 3; /* this x has block scope */
        if (x != 3)
            exit(3);
        goto x;
        exit(4);
        }
x:                  /* this x has function scope */
    return 1;
    }
/*   8: 3.1.2.3  Name space rules: variables, labels, tags, and members */
#include <stdio.h>
#include <stdlib.h>
main()
    {
    int start = (printf("started\n"));
    static struct x { int x; } x = {6};
    goto x;
    exit(0);
x:
    if (x.x == 6) printf("success\n");
    return 0;
    }
/*   9: 3.1.2.3  Unique member name spaces */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    static struct a { char x; } a = {7};
    static struct b { double x; } b = {8};
    if (a.x == 7 && b.x == 8)
        printf("success\n");
    return 0;
    }
/*  10: 3.1.2.5  long double  (even if same size as  double  ) */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    long double x = 4.L;
    if (x == 4L)
        printf("success\n");
    return 0;
    }
/*  11: 3.1.2.5  all unsigned types */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    unsigned char uc = 1;
    unsigned short us = 2;
    unsigned int ui = 4;
    unsigned long ul = 7;
    if (ul - ui - us - uc == 0)
        printf("success\n");
    return 0;
    }
/*  12: 3.1.2.5  signed char */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    signed char sc = -1;
    if (8 * sc == -8)
        printf("success\n");
    return 0;
    }
/*  13: 3.1.2.6  Type-compatibility rules  [87/12] */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    const int *f();
    if (*f(&start) == start)
        printf("success\n");
    return 0;
    }
int signed const *f();
signed int const *f(int *);
int const *f(int *pi)
    {
    return pi;
    }
/*  14: 3.1.3.2  constants:  U,  L,  unsignedness rules */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (-1 < 1u)
        ;
    else if (1UL != 1ul)
        ;
    else
        printf("success\n");
    return 0;
    }
/*  15: 3.1.3.4  character constants with more than one char */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    int c = 'abcd';
    printf("success\n");
    return 0;
    }
/*  16: 3.1.3.4  '\xFF' */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if ('\xFF' == (char)0xFF)
        printf("success\n");
    return 0;
    }
/*  17: 3.1.3.4  '\a'  '\v'   */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if ('\a' != 'a' && '\v' != 'v')
        printf("success\n");
    return 0;
    }
/*  18: 3.1.3.4  Wide-characters: L'x', L"x", wc*, mb*, wchar_t  [87/12] */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    int f();
    if (f() == 2)
        printf("success\n");
    return 0;
    }
#include <stdlib.h>
int f()
    {
    wchar_t wc = L'\0';
    wchar_t wcs[] = L"";
    char s[2];
    if (wc == 0 && wctomb(s, wc) == 1)
        return 2;
    return 0;
    }
/*  19: 3.1.5    "Old-style" assignment-operators are gone */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    short n = 3;
    short m = 2;
    n=-m;
    if (n == -2)
        printf("success\n");
    return 0;
    }
/*  20: 3.2.1.1  "Value-preserving" integer conversion rules */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (sizeof(int) > sizeof(char))
        {
        unsigned char uc1 = 1;
        unsigned char uc2 = 2;
        if (uc1 - uc2 < 0)
            printf("success\n");
        }
    else
        printf("success\n");    /* conforming, but weird */
    return 0;
    }
/*  21: 3.2.1.5  Expressions with  float  operands have  float  type */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    float x, y;
    if (sizeof(x + y) == sizeof(float))
        printf("success\n");
    return 0;
    }
/*  22: 3.2.2.1  Address-of on array and function */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    char a[10];
    if (sizeof(*&a) == 10 && main == &main)
        printf("success\n");
    return 0;
    }
/*  23: 3.2.2.1  Call-through-pointer  (*pkg.fn)()  may be written  pkg.fn() */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    int f();
    int (*pf)() = f;
    if (pf() == 5)
        printf("success\n");
    return 0;
    }
int f()
    {
    return 5;
    }
/*  24: 3.2.2.2  OK to cast void to void  [87/09] */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    void f();
    (void)f();
    return 0;
    }
void f()
    {
    printf("success\n");
    }
/*  25: 3.2.2.3  Generic pointers:  void * */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    void *gp;
    float x, y;
    gp = &x;
    if (gp == &x && gp != &y)
        printf("success\n");
    return 0;
    }
/*  26: 3.3.2.2  prototype-with-default-sizes is compatible with no-prototype */
#include <stdio.h>
int f();
int f(int);
main()
    {
    int start = (printf("started\n"));
    if (f(0) == 2)
        printf("success\n");
    return 0;
    }
int f(int i)
    {
    return i + 2;
    }
/*  27: 3.3.2.2  Calling a prototyped function causes conversion (as if by ass't
) */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    double f(double);
    if (f(2) == -2.0)
        printf("success\n");
    return 0;
    }
double f(double x)
    {
    return -x;
    }
/*  28: 3.3.3.3  Unary plus */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    int i = 3;
    if (+i == 3)
        printf("success\n");
    return 0;
    }
/*  29: 3.3.3.4  sizeof applies to any rvalue expression */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    int i;
    if (sizeof(i+1) == sizeof(int))
        printf("success\n");
    return 0;
    }
/*  30: 3.3.16.1 Structure assignment, return, and argument-passing */
#include <stdio.h>
static struct x { int i; } a = {1};
main()
    {
    int start = (printf("started\n"));
    struct x b;
    struct x c;
    struct x f();
    b = a;
    c = f(b);
    if (c.i == 1)
        printf("success\n");
    return 0;
    }
struct x f(v)
struct x v;
    {
    return v;
    }
/*  31: 3.5.2.2  enum and tag-scope rules  [87/03] */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    enum b { bm2=-2, b0=bm2+2, b1, b2 } b = b2;
    switch (b)
        {
    case b2:
        printf("success\n");
    default:
        ;
        }
    return 0;
    }
/*  32: 3.5.2.3  struct sb;  introduces a new scope  */
#include <stdio.h>
struct sb { double x; } bx = { 9.9 };
main()
    {
    int start = (printf("started\n"));
	struct sb;
	struct sa { struct sb *pb; int i; } a;
	struct sb { struct sa *pa; int i; } b;
	b.i = 13;
	a.pb = &b;
	if (a.pb->i == 13)
        printf("success\n");
    return 0;
    }
/*  33: 3.5.3    const is independent qualifier of e.g. struct type  */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    const struct sa { int i; } csa;
    const struct sa *pcsa = &csa;
    struct sa sa2;
    sa2.i = 9;
    if (sa2.i == 9 && pcsa == &csa)
        printf("success\n");
    return 0;
    }
/*  34: 3.5.3    volatile preserves auto values modified after setjmp  */
#include <stdio.h>
#include <setjmp.h>
jmp_buf env;
main()
    {
    int start = (printf("started\n"));
    volatile int n = 2;
    if (setjmp(env) == 0)
        {
        n = 6;
        longjmp(env, 1);
        }
    else
        {
        if (n == 6)
            printf("success\n");
        }
    return 0;
    }
/*  35: 3.5.4    ellipsis -- e.g. int printf(const char *, ...); */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    int printf(const char *fmt, ...);
    printf("success\n");
    return 0;
    }
/*  36: 3.5.4    prototypes (for declarations) */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    int f(int);
    if (f(0) == 4)
        printf("success\n");
    return 0;
    }
int f(i)
int i;
    {
    return 4;
    }
/*  37: 3.5.4    prototypes (for "new-style" definitions) */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    short f(short);
    if (f(0) == 4)
        printf("success\n");
    return 0;
    }
short f(short i)
    {
    return 4;
    }
/*  38: 3.5.7    Elided-braces rules (as in K&R, not in PCC) */
#include <stdio.h>
struct sa { int a[3]; int b; } asa[2] = { {1}, 2 }; /* from Std */
main()
    {
    int start = (printf("started\n"));
    if (asa[0].a[0] == 1 && asa[1].a[0] == 2)
        printf("success\n");
    return 0;
    }
/*  39: 3.5.7    auto aggregate initializers */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    char msg[20] = {"success\n"};
    printf(msg);
    return 0;
    }
/*  40: 3.5.7    union initializers (via first member) */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    union { char *msg; double d; } md = {"success\n"};
    printf(md.msg);
    return 0;
    }
/*  41: 3.6.4.2  long-size switch labels */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    long lnum = 2;
    switch (lnum)
        {
    default:
        exit(3);
    case 2L:
        printf("success\n");
        }
    return 0;
    }
/*  42: 3.7.2    Tentative definition for static */
#include <stdio.h>
static struct sa { struct sb *pb; int i; } a;
static struct sb { struct sa *pa; int i; } b = { &a, 13 };
static struct sa  a = { &b, 11 };
main()
    {
    int start = (printf("started\n"));
    if (a.i == 11 && b.pa->pb->i == 13)
        printf("success\n");
    return 0;
    }
/*  43: 3.8.1    #if defined, #elif */
#define DEF 1
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
#ifdef NODEF
    exit(3);
#elif defined(DEF)
    printf("success\n");
#endif
    return 0;
    }
/*  44: 3.8.1    no syntax-checking of skipped groups */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
#if NODEF
    L'x' 1.2.3.4.E--xx @$*** JUNK (((
#else
    printf("success\n");
#endif
    return 0;
    }
/*  45: 3.8.2    #include macro-name */
#define STDIO <stdio.h>
#include STDIO
main()
    {
    int start = (printf("started\n"));
    char *p = NULL;
    printf("success\n");
    return 0;
    }
/*  46: 3.8.3    "hiding" of macro names */
#include <stdio.h>
int f(a, b, c)
int a, b, c;
    {
    return c+4;
    }
#define f(a, b) f(a, b, 0)
main()
    {
    int start = (printf("started\n"));
    int a=1, b=2;
    if (f(a, b) == 4)
        printf("success\n");
    return 0;
    }
/*  47: 3.8.3    benign re-definition allowed */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
#define A 1 + 1
#define A   1  +  1
#define A 1 /**/ + /**/ 1
    if (A == 2)
        printf("success\n");
    return 0;
    }
/*  48: 3.8.3    Preprocessor catenation and string-izing */
#include <stdio.h>
#define STR(a) NXSTR(a)
#define NXSTR(a) #a
#define CAT(a, b) NXCAT(a, b)
#define NXCAT(a, b) a ## b
#define A 1
#define B 2
#define AB 5
main()
    {
    int start = (printf("started\n"));
    if (STR(A)[0] == '1' &&
        NXSTR(B)[0] == 'B' &&
        CAT(A, B) == 12 &&
        NXCAT(A, B) == 5)
        printf("success\n");
    return 0;
    }
/*  49: 3.8.5    New preprocessor directive #pragma */
#pragma IGNORE THIS NON-EXISTENT PRAGMA
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    printf("success\n");
    return 0;
    }
/*  50: 3.8.8    Predefined macro names */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (__DATE__[7] == '1' &&
        __TIME__[2] == ':' &&
        __FILE__[0] != '\0' &&
        __LINE__ > 6 &&
        __STDC__ == 1)
        printf("success\n");
    return 0;
    }
/*  51: 4.1.3    <errno.h>  [87/03]  */
#include <errno.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (errno == 0)
        printf("success\n");
    return 0;
    }
     
/*  52: 4.1.5    <stddef.h> -- e.g. offsetof on nested struct member  [88/04] */
#include <stddef.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    struct x { struct y { char c; } y; int z; } x;
    if (offsetof(struct x, y.c) == 0)
        printf("success\n");
    return 0;
    }
     
/*  53: 4.2      <assert.h> is a void expression  [87/12] */
#include <assert.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    0, assert(1 == 1);
    printf("success\n");
    return 0;
    }
     
/*  54: 4.3      <ctype.h>  */
#include <ctype.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (isalpha('a'))
        printf("success\n");
    return 0;
    }
     
/*  55: 4.4      <locale.h> -- e.g., initial locale is "C" */
#include <locale.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    char initial_locale[100];
    strncpy(initial_locale, setlocale(LC_ALL, NULL), 100);
    setlocale(LC_ALL, "C");
    if (LC_ALL != LC_COLLATE &&
        strncmp(setlocale(LC_ALL, NULL), initial_locale, 100) == 0)
        printf("success\n");
    return 0;
    }
     
/*  56: 4.4      localeconv, LC_CURRENCY, negative_sign  [88/04] */
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    struct lconv *plc;
    plc = localeconv();
    if (plc->negative_sign[0] == '\0') /* default "C" value is "" */
        printf("success\n");
    return 0;
    }
     
/*  57: 4.5      <math.h>   */
#include <math.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (HUGE_VAL != 0.0)
        printf("success\n");
    return 0;
    }
     
/*  58: 4.5.1    Math library sets errno when required */
#include <math.h>
#include <errno.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    asin(10.);
    if (errno == EDOM)
        printf("success\n");
    return 0;
    }
     
/*  59: 4.6      <setjmp.h> */
#include <setjmp.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    jmp_buf buf;
    if (setjmp(buf) == 0)
        printf("success\n");
    return 0;
    }
     
/*  60: 4.7      <signal.h> */
#include <signal.h>
#include <stdio.h>
void func(int sig)
    {
    if (sig == SIGINT)
        printf("success\n");
    }
main()
    {
    int start = (printf("started\n"));
    signal(SIGINT, func);
    raise(SIGINT);
    return 0;
    }
     
/*  61: 4.8      <stdarg.h> */
#include <stdarg.h>
#include <stdio.h>
func(int nargs, ...)
    {
    va_list ap;
    double d;
    int i;
    va_start(ap, nargs);
    d = va_arg(ap, double);
    if (d == 1.1 && va_arg(ap, int) == 7)
        printf("success\n");
    va_end(ap);
    }
main()
    {
    int start = (printf("started\n"));
    func(2, 1.1, 7);
    return 0;
    }
     
/*  62: 4.9      <stdio.h> -- e.g., OK to include more than once  */
#include <stdio.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (BUFSIZ >= 256 && TMP_MAX >= 25)
        printf("success\n");
    return 0;
    }
     
/*  63: 4.9.1    In <stdio.h>: FOPEN_MAX  and  FILENAME_MAX  [87/12] */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (FOPEN_MAX > 0 && FILENAME_MAX > 0)
        printf("success\n");
    return 0;
    }
     
/*  64: 4.9.2    Stream and file semantics: opening, seeking, text-binary */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    char name[1000];
    FILE *f;
    tmpnam(name);
    f = fopen(name, "w+b");
    fputc(1, f);
    fputc(2, f);
    rewind(f);
    if (fgetc(f) == 1 &&  fgetc(f) == 2)
        printf("success\n");
    return 0;
    }
     
/*  65: 4.9.4    remove, rename  */
#include <stdio.h>
char old[1000];
char new[1000];
main()
    {
    int start = (printf("started\n"));
    FILE *f;
    tmpnam(old);
    tmpnam(new);
    f = fopen(old, "w");
    fputc('a', f);
    fclose(f);
    rename(old, new);
    if (fopen(old, "r") == NULL)
        printf("success\n");
    remove(new);
    return 0;
    }
     
/*  66: 4.9.6    full printf/scanf  to new exact spec */
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    char buf[10];
    union { struct { float f; char c; } s; double d; } u;
    u.s.c = 'a';
    sscanf("123.", "%E", &u.s.f);
    sprintf(buf, "%.0d", 0);
    if (u.s.f == 123. && u.s.c == 'a' && buf[0] == '\0')
        printf("success\n");
    return 0;
    }
     
/*  67: 4.9.6    scanf and ungetc push-back independently  [87/12] */
#include <stdio.h>
char name[1000];
main()
    {
    int start = (printf("started\n"));
    FILE *f;
    int i, j;
    tmpnam(name);
    f = fopen(name, "w+");
    fprintf(f, "123 456\n");
    rewind(f);
    fscanf(f, "%d", &i);
    ungetc('9', f);
    fscanf(f, "%d", &j);
    if (i == 123 && j == 9)
        printf("success\n");
    fclose(f);
    remove(name);
    return 0;
    }
     
/*  68: 4.9.6    v*printf, v*scanf  */
#include <stdarg.h>
#include <stdio.h>
void print(char *s, char *format, ...)
    {
    va_list args;
    va_start(args, format);
    vsprintf(s, format, args);
    va_end(args);
    }
main()
    {
    int start = (printf("started\n"));
    char buff[10];
    print(buff, "%.2d %.2d %.2d", 1, 2, 3);
    if (strcmp(buff, "01 02 03") == 0)
        printf("success\n");
    return 0;
    }
     
/*  69: 4.9.9    fsetpos  fgetpos  (and  fpos_t  ) */
#include <stdio.h>
char name[1000];
main()
    {
    int start = (printf("started\n"));
    FILE *f;
    fpos_t pos;
    char buff[16];
    tmpnam(name);
    f = fopen(name, "w+");
    fputs("first line\n", f);
    fgetpos(f, &pos);
    fputs("second line\n", f);
    fputs("third line\n", f);
    fsetpos(f, &pos);
    fgets(buff, 16, f);
    if (strcmp(buff, "second line\n") == 0)
        printf("success\n");
    fclose(f);
    remove(name);
    return 0;
    }
     
     
/*  70: 4.10     <stdlib.h> -- e.g. has size_t  [87/12] */
#include <stdlib.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    size_t n = 2;
    if (n == 2)
        printf("success\n");
    return 0;
    }
     
/*  71: 4.10.1   strtol, strtoul, strtod  [87/12] */
#include <stdlib.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    unsigned long ul;
    char *p;
    ul = strtoul("+0x010Y", &p, 0);
    if (ul == 16L && *p == 'Y')
        printf("success\n");
    return 0;
    }
     
/*  72: 4.10.4   system, atexit, getenv, EXIT_SUCCESS, EXIT_FAILURE  [86/12] */
#include <stdlib.h>
#include <stdio.h>
void func()
    {
    if (EXIT_SUCCESS != EXIT_FAILURE)
        printf("success\n");
    }
main()
    {
    int start = (printf("started\n"));
    char *p = 0;
    system("echo spawned >junk");
    atexit(func);
    p = getenv("");
    return 0;
    }
     
/*  73: 4.10.6   div  ldiv */
#include <stdlib.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    div_t val;
    ldiv_t lval;
    val = div(-5, 3);
    lval = ldiv(-5L, 3L);
    if (val.quot == -1 && val.rem == -2 &&
        lval.quot == -1L && lval.rem == -2L)
        printf("success\n");
    return 0;
    }
     
/*  74: 4.11     <string.h> -- e.g. memset and strcmp */
#include <string.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    static char a[5] = "0123";
    memset(a, 'x', 4);
    if (strcmp(a, "xxxx") == 0)
        printf("success\n");
    return 0;
    }
     
/*  75: 4.11.2   memmove, strstr  [87/03] */
#include <string.h>
char *s = "0123456789";
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    char *p;
    char q[11];
    p = strstr("testXYZtest", "XYZ");
    if (*p == 'X' && strstr(p, "") == p)
        {
        strcpy(q, s);
        memmove(q, q+1, 9);
        if (strcmp(q, "1234567899") ==0)
            {
            strcpy(q, s);
            memmove(q+1, q, 9);
            if (strcmp(q, "0012345678") == 0)
                printf("success\n");
            }
        }
    return 0;
    }
     
/*  76: 4.11.6.2 strerror */
#include <string.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    if (strerror(0) != 0)
        printf("success\n");
    return 0;
    }
     
/*  77: 4.12     <time.h>   */
#include <time.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    char *p = NULL;
    int i = (sizeof(clock_t) + sizeof(time_t));
    if (i > 0)
        printf("success\n");
    return 0;
    }
     
/*  78: 4.12.2.2 difftime */
#include <time.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    struct tm time1, time2;
    time1.tm_isdst = time2.tm_isdst = 0;
    time1.tm_year = time2.tm_year = 88;
    time1.tm_mon = time2.tm_mon = 12;
    time1.tm_mday = time2.tm_mday = 25;
    time1.tm_hour = 7;
    time2.tm_hour = 8;
    time1.tm_min = time2.tm_min = 31;
    time1.tm_sec = 13;
    time2.tm_sec = 17;
    if (difftime(mktime(&time2), mktime(&time1)) == 3604.0)
        printf("success\n");
    return 0;
    }
     
/*  79: 4.12.3.5 strftime  */
#include <time.h>
#include <string.h>
#include <stdio.h>
main()
    {
    int start = (printf("started\n"));
    size_t nc;
    struct tm time1;
    char buff[500];
    time1.tm_isdst = 0;
    time1.tm_year = 88;
    time1.tm_mon = 11;  /* December */
    time1.tm_mday = 25;
    time1.tm_hour = 7;
    time1.tm_min = 31;
    time1.tm_sec = 13;
    nc = strftime(buff, 500, "%B %d, 19%y, %I:%M %p", &time1);
    if (nc == strlen(buff))
        printf("success\n");
    return 0;
    }
     
 /* THE REMAINING 22 ITEMS REQUIRE A FULL-SCALE VALIDATION SUITE ... */
 /*  80: 2.*      Full semantics: environment */
 /*  81: 2.1.2.3  sequence points */
 /*  82: 2.1.2.2  argv[0] is program-name or null-string */
 /*  83: 2.2.4.1  translation-time capacity limits */
 /*  84: 3.1.*    All required diagnostics: Lexical elements */
 /*  85: 3.1.*    Full syntax and semantics: Lexical elements */
 /*  86: 3.2.*    All required diagnostics: Conversions */
 /*  87: 3.2.*    Full syntax and semantics: Conversions */
 /*  88: 3.3.*    Full syntax and semantics: Expressions */
 /*  89: 3.3.*    All required diagnostics: Expressions */
 /*  90: 3.4.*    Full syntax and semantics: Constant expressions */
 /*  91: 3.4.*    All required diagnostics: Constant expressions */
 /*  92: 3.5.*    All required diagnostics: Declarations */
 /*  93: 3.5.*    Full syntax and semantics: Declarations */
 /*  94: 3.6.*    All required diagnostics: Statements */
 /*  95: 3.6.*    Full syntax and semantics: Statements */
 /*  96: 3.7.*    All required diagnostics: External definitions */
 /*  97: 3.7.*    Full syntax and semantics: External definitions */
 /*  98: 3.8.*    All required diagnostics: Preprocessing directives */
 /*  99: 3.8.*    Full syntax and semantics: Preprocessing directives */
 /* 100: 4.1.2    No reserved names in user name space */
 /* 101: 4.1.2    No user names in standard headers */
     
 /* THESE ITEMS ARE "THE 101 FEATURES" OF ANSI/ISO C.                    */
 /* Items 1-79 can be sampled with this Plum Hall Validation Sampler.    */
 /* Items 80-101 require a full-scale Validation Suite.                  */
 /* (Contact Plum Hall for information about the comprehensive tests     */
 /* embodied in the Plum Hall Validation Suite for C.)                   */
 /* This "Sampler" is not intended for any formal certification or       */
 /* validation work; it is intended only to provide an overall           */
 /* estimate of how close a compiler is to the soon-to-be ANSI Standard. */
 /* Copyright (c) 1988, Plum Hall Inc and Chiron Systems Inc.            */
 /* Permission is granted for reproduction and use of this "Sampler"     */
 /* provided that this notice is reproduced entirely.                    */
 /* Tom Plum, Plum Hall Inc, 609-927-3770,  uunet!plumhall!plum .        */
 /* Ralph Ryan, Chiron Systems Inc.                                      */
 /* With assistance from Don Gallagher and Scott Erlichman.              */
 /* REVISIONS: 88/12/22                                                  */
 /* Test 13: f(start) -> *f(&start) AND f(int) -> f(int *)               */
 /* Test 18: In function f(after line 249), add return 0;                */
 /* Test 72: On (now) line 988, change succeed to success                */
 /*          Also, on line 994 change to "echo spawned >junk"            */
 /* Test 79: On line 1111, change month to 11 (zero-origin ouch          */
 /* Tests 13 & 72 could have affected scoring; 18 & 79 are cosmetic      */
---------- End of SAMPLER.88 ----------
---------- Start of SUMMTE.C ----------
/* runtest - run the Plum Hall Validation Sampler */
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#define NO_OUTPUT 0
#define NO_SUCCESS 1
#define SUCCESS 2
#define NTESTS 79
main(argc, argv)
    int argc;
    char *argv[];
    {
    FILE *outfp;
    FILE *summfp;
    char buf[BUFSIZ];
    char out_buf[BUFSIZ];
    char out_name[20];
    char summ_name[20];
    int i;
    int nerrs = 0;
    int pgm_num;
    static int result[100] = {0};
     
    if (argc < 2 || strchr(argv[1], '.') != NULL)
        {
        fprintf(stderr, "usage: summtest <compiler-name-without-dot>\n");
        exit(3);
        }
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    for (pgm_num = 1; pgm_num <= NTESTS; ++pgm_num)
        {     /* for each input program */
        sprintf(out_name, "a%03d.out", pgm_num);
        outfp = fopen(out_name, "r");
        if (outfp == NULL  ||
            fgets(out_buf, BUFSIZ, outfp) == NULL ||
            strcmp(out_buf, "started\n") != 0)
            result[pgm_num] = NO_OUTPUT;
        else if (fgets(out_buf, BUFSIZ, outfp) == NULL ||
            strcmp(out_buf, "success\n") != 0)
            result[pgm_num] = NO_SUCCESS;
        else
            result[pgm_num] = SUCCESS;
        fclose(outfp);
        }    /* end loop over each program */
    /* now all programs have been scored; print the results */
    sprintf(summ_name, "%s.sum", argv[1]);
    summfp = fopen(summ_name, "w");
    if (summfp == NULL)
        {
        fprintf(stderr, "cannot create %s\n", summ_name);
        exit(2);
        }
    for (i = 0; i < 10; ++i)
        {
        if (i < strlen(argv[1]))
            fprintf(summfp, "%c\n", argv[1][i]);
        else
            fprintf(summfp, " \n");
        }
    for (i = 1; i <= NTESTS; ++i)
        {
        if (result[i] == NO_OUTPUT)
            {
            ++nerrs;
            fprintf(summfp, "-\n");
            }
        else if (result[i] == NO_SUCCESS)
            {
            ++nerrs;
            fprintf(summfp, "o\n");
            }
        else
            fprintf(summfp, "*\n");
        }
    fprintf(summfp, "\n%2d\n", nerrs);
    fclose(summfp);
    exit(0);
    }
---------- End of SUMMTE.C ----------
---------- Start of TOP101 ----------
 1: 2.1.1.2  Backslash-splicing
 2: 2.1.1.2  Phases of translation
 3: 2.2.1.1  Trigraphs
 4: 2.2.4.2  <float.h> -- e.g., DBL_DIG >= 10  [87/03]
 5: 2.2.4.2  <limits.h> -- e.g., has MB_LEN_MAX  [87/12]
 6: 3.1.2    Internal identifier significance at least 31 characters
 7: 3.1.2.2  Scope rules: file, function, prototype, and block
 8: 3.1.2.3  Name space rules: variables, labels, tags, and members
 9: 3.1.2.3  Unique member name spaces
10: 3.1.2.5  long double  (even if same size as  double  )
11: 3.1.2.5  all unsigned types
12: 3.1.2.5  signed char
13: 3.1.2.6  Type-compatibility rules  [87/12]
14: 3.1.3.2  constants:  U,  L,  unsignedness rules
15: 3.1.3.4  character constants with more than one char
16: 3.1.3.4  '\xFF'
17: 3.1.3.4  '\a'  '\v'
18: 3.1.3.4  Wide-characters: L'x', L"x", wc*, mb*, wchar_t  [87/12]
19: 3.1.5    "Old-style" assignment-operators are gone
20: 3.2.1.1  "Value-preserving" integer conversion rules
21: 3.2.1.5  Expressions with  float  operands have  float  type
22: 3.2.2.1  Address-of on array and function
23: 3.2.2.1  Call-through-pointer  (*pkg.fn)()  may be written  pkg.fn()
24: 3.2.2.2  OK to cast void to void  [87/09]
25: 3.2.2.3  Generic pointers:  void *
26: 3.3.2.2  prototype-with-default-sizes is compatible with no-prototype
27: 3.3.2.2  Calling a prototyped function causes conversion (as if by ass't)
28: 3.3.3.3  Unary plus
29: 3.3.3.4  sizeof applies to any rvalue expression
30: 3.3.16.1 Structure assignment, return, and argument-passing
31: 3.5.2.2  enum and tag-scope rules  [87/03]
32: 3.5.2.3  Tentative def for  struct sb b;
33: 3.5.3    const is independent qualifier of e.g. struct type
34: 3.5.3    volatile preserves auto values modified after setjmp
35: 3.5.4    ellipsis
36: 3.5.4    prototypes (for declarations)
37: 3.5.4    prototypes (for "new-style" definitions)
38: 3.5.7    Elided-braces rules (as in K&R, not in PCC)
39: 3.5.7    auto aggregate initializers
40: 3.5.7    union initializers (via first member)
41: 3.6.4.2  long-size switch labels
42: 3.7.2    Tentative definition for static
43: 3.8.1    #if defined, #elif
44: 3.8.1    no syntax-checking of skipped groups
45: 3.8.2    #include macro-name
46: 3.8.3    "hiding" of macro names
47: 3.8.3    benign re-definition allowed
48: 3.8.3    Preprocessor catenation and string-izing
49: 3.8.5    New preprocessor directive #pragma
50: 3.8.8    Predefined macro names
51: 4.1.3    <errno.h>  [87/03]
52: 4.1.5    <stddef.h> -- e.g. offsetof on nested struct member  [88/04]
53: 4.2      <assert.h>
54: 4.3      <ctype.h>
55: 4.4      <locale.h> -- e.g., initial locale is "C"
56: 4.4      localeconv, LC_CURRENCY, negative_sign  [88/04]
57: 4.5      <math.h>
58: 4.5.1    Math library sets errno when required
59: 4.6      <setjmp.h>
60: 4.7      <signal.h>
61: 4.8      <stdarg.h>
62: 4.9      <stdio.h> -- e.g., OK to include more than once
63: 4.9.1    In <stdio.h>: FOPEN_MAX  and  FILENAME_MAX  [87/12]
64: 4.9.2    Stream and file semantics: opening, seeking, text-binary
65: 4.9.4    remove, rename
66: 4.9.6    full printf/scanf  to new exact spec
67: 4.9.6    scanf and ungetc push-back independently  [87/12]
68: 4.9.6    v*printf, v*scanf
69: 4.9.9    fsetpos  fgetpos  (and  fpos_t  )
70: 4.10     <stdlib.h> -- e.g. has size_t  [87/12]
71: 4.10.1   strtol, strtoul, strtod  [87/12]
72: 4.10.4   system, atexit, getenv, EXIT_SUCCESS, EXIT_FAILURE  [86/12]
73: 4.10.6   div  ldiv
74: 4.11     <string.h> -- e.g. memset and strcmp
75: 4.11.2   memmove, strstr  [87/03]
76: 4.11.6.2 strerror
77: 4.12     <time.h>
78: 4.12.2.2 difftime
79: 4.12.3.5 strftime
     
THE REMAINING 22 ITEMS REQUIRE A FULL-SCALE VALIDATION SUITE ...
     
80: 2.*      Full semantics: environment
81: 2.1.2.3  sequence points
82: 2.1.2.2  argv[0] is program-name or null-string
83: 2.2.4.1  translation-time capacity limits
84: 3.1.*    All required diagnostics: Lexical elements
85: 3.1.*    Full syntax and semantics: Lexical elements
86: 3.2.*    All required diagnostics: Conversions
87: 3.2.*    Full syntax and semantics: Conversions
88: 3.3.*    Full syntax and semantics: Expressions
89: 3.3.*    All required diagnostics: Expressions
90: 3.4.*    Full syntax and semantics: Constant expressions
91: 3.4.*    All required diagnostics: Constant expressions
92: 3.5.*    All required diagnostics: Declarations
93: 3.5.*    Full syntax and semantics: Declarations
94: 3.6.*    All required diagnostics: Statements
95: 3.6.*    Full syntax and semantics: Statements
96: 3.7.*    All required diagnostics: External definitions
97: 3.7.*    Full syntax and semantics: External definitions
98: 3.8.*    All required diagnostics: Preprocessing directives
99: 3.8.*    Full syntax and semantics: Preprocessing directives
100: 4.1.2    No reserved names in user name space
101: 4.1.2    No user names in standard headers
     
 THESE ITEMS ARE "THE 101 FEATURES" OF ANSI/ISO C.
 Items 1-79 can be sampled with this Plum Hall Validation Sampler.
 Items 80-101 require a full-scale Validation Suite.
 (Contact Plum Hall for information about the comprehensive tests
 embodied in the Plum Hall Validation Suite for C.)
 This "Sampler" is not intended for any formal certification or
 validation work; it is intended only to provide an overall
 estimate of how close a compiler is to the soon-to-be ANSI Standard.
 Copyright (c) 1988, Plum Hall Inc and Chiron Systems Inc.
 Permission is granted for reproduction and use of this "Sampler"
 provided that this notice is reproduced entirely.
 Tom Plum, Plum Hall Inc, 609-927-3770,  uunet!plumhall!plum .
 Ralph Ryan, Chiron Systems Inc.
 With assistance from Don Gallagher and Scott Erlichman.
---------- End of TOP101 ----------
       _____ ____  ____  ____       ___        __          __
         /  /     /     /          /   '  /   /  \  /   / /  \  /   /
        /  /---  /---  /---       /      /   /   / /   / / __  /---/
    (__/  /___  /     /           \___, /___ \__/  \__/  \__/ /   /
     
Programmer
Georgia State University Computer Center
MTSJMC at GSUVM1                                        (404)651-4537
