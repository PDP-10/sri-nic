/* TTSPEC.C - Check out type-specifier handling
**	Compile with -d=sym -S to see if the resulting TTSPEC.CYM is correct.
**		use -DBADSYN to check error detection of bad syntax.
*/

/* First verify that ordering of typespecs can be random */
static unsigned char uc1;
char unsigned static uc2;
char static unsigned uc3;

const i1;
int const i2;
extern const i3;
const extern signed i4;
int extern signed const i5;

volatile long l1;
long extern volatile l2;
signed volatile long l3;
long volatile int extern l4;
int long volatile signed extern l5;

struct sfoo {int a;};			/* Following decls do differ */
struct sfoo static s1;
struct sfoo volatile s2;
volatile struct sfoo const extern s3;

/* Check structure containing const and/or volatile objects */

struct sfc { int a; const b; int c;} sfc1, *psfc;
struct sfv { int a; volatile b; int c;} sfv1, *psfv;
struct sfvc {int a; volatile b; const c;} sfvc1, *psfvc;

volatile struct sfc sfc2;
const struct sfv sfv2;
const volatile struct sfc sfc3;

/* Enum stuff */
enum efoo { zero, one, two, three };
enum ef2 { aone=1, athree=3, asix=aone+two+zero+athree, asix2 = asix };

#ifdef BADSYN
auto int a;
i;
const int int ic;
static const int const ic2;
const unsigned float uf;
enum eferr { barf=uc1, ugh=i2, bleah,};
static int ic3 const;

#endif
