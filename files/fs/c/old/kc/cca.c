/* cca.c -- assembly tasks   (C) 1981  K. Chen */

#define	  sc	extern
#include  "cc.h"

asmb(m,o)
char *m,*o;
{
  int  *arg();
  char str[64],*s;

  s = str;
  while (*m) *s++ = *m++;
  *s++ = ',';
  *s++ = '=';
  while (*o) *s++ = *o++;
  *s++ = '\n';
  *s = '\0';

  fprintf(stderr,"\n");
  if (!fork("sys:fail.exe",arg(str),1)) {
      fprintf(stderr,"Fatal Error: Cannot fork to Assembler.\n");
  }
}

expunge(f)
char *f;
{
  delet(f);
}

static int block[50];
static int *arg(s)
char *s;
{
  int  i;
  char *t,*u;

  i = &block[17];
  t = u = i | 0350700000000;	/* convert to 7-bit byte pointer */
  while (*t++ = *s++);
  i = ((t-u)+3)/4;
  block[0] = 1;
  block[1] = 16;
  block[2] = 16 + i;
  block[16] = 0464151000000 + i;
  return block;
}

