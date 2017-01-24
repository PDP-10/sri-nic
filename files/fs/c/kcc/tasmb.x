#include "ccsite.h"
#include "cc.h"
#include "ccchar.h"
#include <sys/types.h>	/* For stat(), for symval stuff */
#include <sys/stat.h>
#include <sys/file.h>	/* For open() */
#include <errno.h>	/* For strerror */
#include <string.h>	/* For strchr etc */

#if 0

#if SYS_T20
#undef NODE		/* Avoid conflict with JSYS name */
#define _JSYS_NOEXTRADEFS	/* Don't need all the old stuff */
#include <jsys.h>	/* For PRARG%, sigh */
#define PRGBLEN 0200	/* TOPS-20 limit on size of PRARG block */
#else
#define PRGBLEN 0
#endif

#ifndef RH
#define RH 0777777	/* Mask for RH of a word */
#endif
#ifndef LH
#define LH (-1<<18)	/* Mask for LH */
#endif
#ifndef XWD		/* Put halves together */
#define XWD(a,b) ((unsigned)(a)<<18|((unsigned)(b)&RH))
#endif

/* Exported routines */
int asmb();
void runlink();
int fnxisrel();
char *fnparse(), *estrcpy(), *fstrcpy();
int sixbit();

/* Imported routines */
extern char *malloc(), *realloc();

/* Internal routines */
static int *stmpfile();
static char *gtmpfile();
static int hackfork();

static int tdebug = 0;		/* Set non-zero to print out tmpcor args */

/* BP7 - macro to convert a char ptr into a 7-bit byte pointer */
#define bp7(cp) ((char *)(int)(_KCCtype_char7 *)(cp))
#endif /* 0 */

int
asmb(m, f1, f2)
char *m, *f1, *f2;
{
    int res = 0;
    char str[200*4];
    if (tgasm == 3) {
	sprintf(str, "midas %s_%s\n", m, (f2 ? f2 : f1));
    }
}
