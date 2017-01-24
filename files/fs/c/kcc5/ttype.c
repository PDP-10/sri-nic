/* Test program to check out various combinations of types
** and see what gets assembled for them.
** By doing assignment amongst almost every type we can check out
** most of the type conversions.
*/

/* Declare a couple of vars (A and B) for each arithmetic type,
** plus a pointer to it.
*/

char	ca, cb,		*cp;
signed char sca, scb,	*scp;
signed short ssa, ssb,	*ssp;
signed int sia, sib,	*sip;
signed long sla, slb,	*slp;
unsigned char uca, ucb,	*ucp;
unsigned short usa, usb,*usp;
unsigned int uia, uib,	*uip;
unsigned long ula, ulb,	*ulp;

signed _KCCtype_char6 c6a, c6b,	*c6p;	/* KCC specific frobs */
signed _KCCtype_char7 c7a, c7b,	*c7p;
signed _KCCtype_char8 c8a, c8b,	*c8p;
signed _KCCtype_char9 c9a, c9b,	*c9p;
signed _KCCtype_char18 c18a, c18b,*c18p;
unsigned _KCCtype_char6 uc6a, uc6b,	*uc6p;	/* KCC specific frobs */
unsigned _KCCtype_char7 uc7a, uc7b,	*uc7p;
unsigned _KCCtype_char8 uc8a, uc8b,	*uc8p;
unsigned _KCCtype_char9 uc9a, uc9b,	*uc9p;
unsigned _KCCtype_char18 uc18a, uc18b,	*uc18p;

float flta, fltb,	*fltp;
double dbla, dblb,	*dblp;
long double ldbla, ldblb,*ldblp;

main()
{}

/* Now check out assignments.  First assign everything to each other. */

selfass()
{
	ca = cb;
	sca = scb;	ssa = ssb;	sia = sib;	sla = slb;
	uca = ucb;	usa = usb;	uia = uib;	ula = ulb;
	c6a = c6b;   c7a = c7b;   c8a = c8b;   c9a = c9b;   c18a = c18b;
	uc6a = uc6b; uc7a = uc7b; uc8a = uc8b; uc9a = uc9b; uc18a = uc18b;
	flta = fltb;	dbla = dblb;	ldbla = ldblb;
}

/* Now define routines to assign every type to every other type. */
#define assmac(name, lval) name(){\
	lval = cb;\
	lval = scb;  lval = ssb;  lval = sib;  lval = slb;\
	lval = ucb;  lval = usb;  lval = uib;  lval = ulb;\
	lval = c6b;  lval = c7b;  lval = c8b;  lval = c9b;  lval = c18b;\
	lval = uc6b; lval = uc7b; lval = uc8b; lval = uc9b; lval = uc18b;\
	lval = fltb; lval = dblb; lval = ldblb;\
	}

assmac(cass, ca)
assmac(scass, sca)
assmac(ssass, ssa)
assmac(siass, sia)
assmac(slass, sla)
assmac(ucass, uca)
assmac(usass, usa)
assmac(uiass, uia)
assmac(ulass, ula)

assmac(c6ass, c6a)
assmac(c7ass, c7a)
assmac(c8ass, c8a)
assmac(c9ass, c9a)
assmac(c18ass, c18a)
assmac(uc6ass, uc6a)
assmac(uc7ass, uc7a)
assmac(uc8ass, uc8a)
assmac(uc9ass, uc9a)
assmac(uc18ass, uc18a)

assmac(fltass, flta)
assmac(dblass, dbla)
assmac(ldblass, ldbla)

/* Pointer conversions and arithmetic.
** (1) Convert a pointer to every type to some ptr to every other type.
** (2) Check out pointer comparison.
** (3) Check out pointer subtraction, both with and without a constant
**		subtrahend (to invoke optimization).
*/

#define ptrmac(name, t, v, obj) name(larg) t *larg; {\
	v=(t *)cp;\
	v=(t *)scp;  v=(t *)ssp;  v=(t *)sip;  v=(t *)slp;\
	v=(t *)ucp;  v=(t *)usp;  v=(t *)uip;  v=(t *)ulp;\
	v=(t *)c6p;  v=(t *)c7p;  v=(t *)c8p;  v=(t *)c9p;  v=(t *)c18p;\
	v=(t *)uc6p; v=(t *)uc7p; v=(t *)uc8p; v=(t *)uc9p; v=(t *)uc18p;\
	v=(t *)fltp; v=(t *)dblp; v=(t *)ldblp;\
	return larg < v ? v - larg : v - &obj; \
	}

ptrmac(pcass,	char, cp, ca)
ptrmac(pscass, signed char, scp, sca)
ptrmac(pssass, signed short, ssp, ssa)
ptrmac(psiass, signed int, sip, sia)
ptrmac(pslass, signed long, slp, sla)
ptrmac(pucass, unsigned char, ucp, uca)
ptrmac(pusass, unsigned short, usp, usa)
ptrmac(puiass, unsigned int, uip, uia)
ptrmac(pulass, unsigned long, ulp, ula)

ptrmac(pc6ass, signed _KCCtype_char6, c6p, c6a)
ptrmac(pc7ass, signed _KCCtype_char7, c7p, c7a)
ptrmac(pc8ass, signed _KCCtype_char8, c8p, c8a)
ptrmac(pc9ass, signed _KCCtype_char9, c9p, c9a)
ptrmac(pc18ass, signed _KCCtype_char18, c18p, c18a)
ptrmac(puc6ass, unsigned _KCCtype_char6, uc6p, uc6a)
ptrmac(puc7ass, unsigned _KCCtype_char7, uc7p, uc7a)
ptrmac(puc8ass, unsigned _KCCtype_char8, uc8p, uc8a)
ptrmac(puc9ass, unsigned _KCCtype_char9, uc9p, uc9a)
ptrmac(puc18ass, unsigned _KCCtype_char18, uc18p, uc18a)

ptrmac(pfltass,	float,	fltp, flta)
ptrmac(pdblass,	double,	dblp, dbla)
ptrmac(pldblass, long double, ldblp, ldbla)

/* Comparisons.  Compare all kinds of objects, except pointers (already done)
*/
/* First check out objects */
#define defcmp(nam, o1, o2) int nam(){ return o1 < o2; }

defcmp(cmpc, ca, cb)
defcmp(cmpsc, sca, scb)
defcmp(cmpss, ssa, ssb)
defcmp(cmpsi, sia, sib)
defcmp(cmpsl, sla, slb)
defcmp(cmpuc, uca, ucb)
defcmp(cmpus, usa, usb)
defcmp(cmpui, uia, uib)
defcmp(cmpul, ula, ulb)

defcmp(cmpc6, c6a, c6b)
defcmp(cmpc7, c7a, c7b)
defcmp(cmpc8, c8a, c8b)
defcmp(cmpc9, c9a, c9b)
defcmp(cmpc18, c18a, c18b)
defcmp(cmpuc6, uc6a, uc6b)
defcmp(cmpuc7, uc7a, uc7b)
defcmp(cmpuc8, uc8a, uc8b)
defcmp(cmpuc9, uc9a, uc9b)
defcmp(cmpuc18, uc18a, uc18b)

defcmp(cmpflt, flta, fltb)
defcmp(cmpdbl, dbla, dblb)
defcmp(cmpldbl, ldbla, ldblb)
