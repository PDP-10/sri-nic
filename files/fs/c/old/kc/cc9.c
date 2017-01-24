/* cc9.c -- debug module   (C) 1981  K. Chen */

#define	sc extern
#include "cc.h"

/* ------------------------------------------------ */
/*	dump symbol table to symbol table file      */
/* ------------------------------------------------ */

symdump()
{
  int    i, u;
  char   *str, *c;
  struct TY     *t;
  struct SYMBOL *s;

  fputs("\n-- Global Symbols --\n\n", fsym);
  i = minsym;
  while (i < maxsym) {
      s = &symbol[i];
      switch (u=s->sclass) {
      case STAG:
	   str = "struct tag";
	   break;
      case SMEMBER:
	   str = "member";
	   break;
      case STYPEDEF:
	   str = "typedef";
	   break;
      case SEXTERN:
	   str = "extern";
	   break;
      case SSTATIC:
	   str = "static";
	   break;
      default:
	   str = "unknown";
      }
      if (u != SMACRO) {
  	  c = s->sname;
	  if (*c == '%' || *c == '#') c++;
          fprintf(fsym, "%10s", c);
          fprintf(fsym, "%12s  type:  %4d", str, s->sptr);
          if (u == SMEMBER) {
	      fprintf(fsym, "  offset = %4d", s->svalue);
	  }
          putc ('\n', fsym);
      }
      i++;
  }
  fputs("\n\n-- Types --\n\n", fsym);
  i = 0;
  while (i < maxtype) {
      t = &types[i];
      switch (u = t->ttype) {
      case INT:
	   str = "int";
	   break;
      case CHAR:
	   str = "char";
	   break;
      case FLOAT:
	   str = "float";
	   break;
      case DOUBLE:
	   str = "double";
	   break;
      case LONG:
	   str = "long";
	   break;
      case SHORT:
	   str = "short";
	   break;
      case STRUCT:
	   str = "struct";
	   break;
      case UNSIGNED:
	   str = "unsigned";
	   break;
      case LABEL:
	   str = "Label";
	   break;
      case ARRAY:
	   str = "Array";
	   break;
      case FUNCTION:
	   str = "Function";
	   break;
      case FNDEF:
	   str = "Func-def";
	   break;
      case PTR:
	   str = "Pointer";
	   break;
      default:
	   str = "unknown";
      }
      if (u != PTR || t->tptr != NULL) {
          fprintf(fsym, " %5d %9s ", t-types, str);

          switch (u) {
          case ARRAY:
	       fprintf(fsym, "size: %d of ", t->tsize);
	       break;
          case STRUCT:
	       fprintf(fsym, "size: %d", t->tsize);
	       break;
          case PTR:
               fprint(fsym, "of    ");
          }
          if (t->tptr != NULL) 
	      fprintf(fsym, "%d\n", t->tptr-types);
          else
	      putc('\n', fsym);
      }
      i++;
  }
}
