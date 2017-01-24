#if 0
 5-Dec-89 15:12:43-PST,1788;000000000005
Return-Path: <ben@csi.compuserve.com>
Received: from saqqara.cis.ohio-state.edu by NIC.DDN.MIL with TCP; Tue, 5 Dec 89 15:12:32 PST
Received: by saqqara.cis.ohio-state.edu (5.61/4.891204)
	id AA13911; Tue, 5 Dec 89 17:56:22 -0500
Date: 05 Dec 89 16:23:06 EST
From: <BEN@CSI.CompuServe.COM>
To: <KLH@NIC.DDN.MIL>
Subject: EBUG33.C
Message-Id: <"CSI 5679-21691"@CompuServe.COM>

#endif
/* EBUG33.C */

main ()
{  /* Should generate error */
   {  void f2int(int, int); f2int(1);  }
   
   /* Should generate error */
   { int i; void f2int(int, int); f2int(1,&i); }
   
   /* Should generate error -- not internal error */
   { int (*pf)(); pf++; }

   /* Should generate error -- not internal error */
   { int (*pf)(); ++pf; }

   /* Should generate error */   /* &bit-field is invalid */
   { static struct bitf {unsigned a : 1; } bitf = {0}; ff(&bitf.a); }

   /* Should generate error */ /* &register is invalid */
   { register int r1, r2, r3; ff(&r10); }

   /* Should generate error */ /* operand of sizeof isn't lvalue for bit-field */
   { static struct bitf {unsigned a:1;} bitf = {0}; ff(sizeof(bitf.a)); }
   
   /* Should generate error */ /* left operand of asst shall be modifiable lvalue */
   { typedef const struct ci { int i; } CI; CI a = {0}; a.i = 1; }
   
   
  /* Should generate error */ /* member of const aggregate is const and ass't */
  /* needs modifiable lvalue */
  { static const struct { int a; } cs = {9}; cs.a = 1; }

  /* Should generate error */  /* result type and const char isn't modifiable */
  { char c; char *p = &c; * (1 ? (char *)p : (const char *)p) = 0; }

  /* Should generate error */   /* Incompatible asst operands */
  { int i = 3; int *pi = &i; long int li = 4; long *pli = &li; pli = pi; }
}

