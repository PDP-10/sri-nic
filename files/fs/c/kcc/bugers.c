int foo(), ff();
main25()
{
    /* M31003.C */
    /* short and int are different, even if same size */
    { short *p; int *q = 0; p = q; }    /* Should generate error */
/* OK: [Warning] Implicit conversion - pointer to pointer */
    
    /* M31004.C */
    /* long and int are different, even if same size */
    { long  *a; int *b = 0; a = b; }    /* Should generate error */
/* OK: [Warning] Implicit conversion - pointer to pointer */

    /* M31008.C */
    { /* enum constant must fit the (int) storage */    
    #include <limits.h>
    enum { A = INT_MAX, B };            /* Should generate error */
    }
/* No error -- need one? */

    /* M31015.C */
    /* violates hexadecimal-constant syntax */
    { int i = 0x ; foo(i);}
/* No error -- need one? */

    /* M31019.C */
    /* value of hex escape sequence shall fit its storage */
    { int i = sizeof('\x1234567890123456789012345678901234567890'); foo(i);}
/* OK: [Warning] Hex constant overflow */

    /* M31030.C */
    /* matching of {} punctuator is constraint */
    { { }
/* Hmmmmm??? Nothing reported */

    /* M31032.C */
    { /* header-name syntax */
    #include <stdio.h
    }
/* Hmmmmm??? Nothing reported */
}

main26()
{
    struct x { char a[4]; } f();
    int i;
    i = f().a[0];
#if 0
"bug26.c", line 25: [Internal error] gaddress: bad op (node 8: 40=N_FNCALL)
"bug26.c", line 25: [Internal error] vrtoreal: bad vr 0
"bug26.c", line 25: [Warning][Internal error] vrstoreal: bad vr 0
"bug26.c", line 25: [Internal error] vrfree: bad vr 0
#endif
}

main27()
{  /* Should generate error */
   {  void f2int(int, int); f2int(1);  }
/* OK: [Warning] Number of args doesn't match function prototype */

   /* Should generate error */
   { int i; void f2int(int, int); f2int(1,&i); }
/* OK: [Warning] Implicit conversion - pointer to int */
   
   /* Should generate error -- not internal error */
   { int (*pf)(); pf++; }
/* Hmmm... Nothing reported? */

   /* Should generate error -- not internal error */
   { int (*pf)(); ++pf; }
/* Hmmm... Nothing reported? */

   /* Should generate error */   /* &bit-field is invalid */
   { static struct bitf {unsigned a : 1; } bitf = {0}; ff(&bitf.a); }
/* Hmmm... Nothing reported? */

   /* Should generate error */ /* &register is invalid */
   { register int r1, r2, r3; ff(&r1); }
/* Typo, try again */

   /* Should generate error */ /* operand of sizeof isn't lvalue for bit-field *
/
   { static struct bitf {unsigned a:1;} bitf = {0}; ff(sizeof(bitf.a)); }
/* OK: [Warning] Operand of sizeof has bitfield type */
   
   /* Should generate error */ /* left operand of asst shall be modifiable lvalu
e */
   { typedef const struct ci { int i; } CI; CI a = {0}; a.i = 1; }
/* Hmmm... Nothing reported? */   
   
  /* Should generate error */ /* member of const aggregate is const and ass't */
  /* needs modifiable lvalue */
  { static const struct { int a; } cs = {9}; cs.a = 1; }
/* Hmmm... Nothing reported? */

  /* Should generate error */  /* result type and const char isn't modifiable */
  { char c; char *p = &c; * (1 ? (char *)p : (const char *)p) = 0; }
/* Hmmm... Nothing reported? */

  /* Should generate error */   /* Incompatible asst operands */
  { int i = 3; int *pi = &i; long int li = 4; long *pli = &li; pli = pi; }
/* OK: [Warning] Implicit conversion - pointer to pointer */
}

main28()
{
    /* Should generate an error */ /* no comma in const expression */
    { static int d = (1,2); }
/* Nothing reported */

    /* Should generate an error */ /* each const expr shall fit its type */
    {
#include <limits.h>
    int i = 0;
    switch (i)
        {
        case LONG_MAX * 4 : i = 1;
        }    
    }
/* Nothing reported */

}
