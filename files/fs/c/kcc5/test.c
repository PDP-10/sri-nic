#include <monsym.h>
#define FLDGET(wd,mask) (((unsigned)(wd)&(mask))/((mask)&(-(mask))))

int fdb[monsym(".FBSIZ")];
int a, b;
foo()
{
	a = FLDGET(07700,b);
	b = FLDGET(a, 070);
	return FLDGET(fdb[monsym(".FBBYV")],monsym("FB%BSZ"));
}
