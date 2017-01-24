/* cc0.c   (C) 1981 K. Chen */

/* --------------------------------- */
/*	system dependent module      */
/*	  (TOPS-20 & WAITS)	     */
/* --------------------------------- */

#define	  sc	extern
#include  "cc.h"

files(n)
string n;
{
  int    e;
  string s, org;
  FILE   *fopen();

  if (n[0] == '\0') {
      fprintf(stderr, "No filename given.\n");
      exit(1);
  }

  e = 0;
  org = n;
  for (s=module ; *s = *n ; s++) 
      if (*n++ == '.') {
          *s = '\0';
	  e = 1;
	  break;
      }

  if (!e) {				/* filename has no extension */
      strcpy(input, module);
      strapp(input, ".c");		/* try filename.c first */
      in = fopen(input, "r");
      if (in == NULL) {
	  strcpy(input, module);	/* then filename */
	  in = fopen(input, "r");
      }
  }
  else {
      strcpy(input, org);		/* filename has extension */
      in = fopen(input, "r");
  }

  if (in == NULL) {
      fprintf(stderr, "Cannot open file %s for input.\n", input);
      exit(1);
  }

  strcpy(output, module);
  strapp(output, ".fai");
  if (!eflag) {
      if ((out = fopen(output, "w")) == NULL) {
          fprintf(stderr, "Cannot open output file %s.\n", output);
          exit(1);
      }
  }
  strcpy(symfname, module);
  strapp(symfname, ".sym");
  if (dump) {
      if ((fsym = fopen(symfname, "w")) == NULL) {
          fprintf(stderr, "Cannot open symbol table file %s.\n", symfname);
          exit(1);
      }
  }
}


/* ----------------------- */
/*	size of type  	   */
/* ----------------------- */

tsize(t)
struct TY *t;
{
  struct TY *u;
  int    s;

  if (t == NULL) return 0;
  if (t->ttype == ARRAY) {
      s = 1;
      u = t;
      while (u->ttype == ARRAY) {
          s = s * u->tsize;
	  u = u->tptr;
      }
      s = s * tsize(u);
      if (u->ttype == CHAR) s = (s+NBYTES-1)/NBYTES;
      return s;
  }
  return t->tsize;
}

