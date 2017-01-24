/* cc1.c   (C) 1981 K. Chen */

#define  sc extern
#include "cc.h"

/* ----------------------------------------- */
/*	support routines for C compiler      */
/* ----------------------------------------- */

/* ----------------------------------- */
/*	copy string t to string s      */
/* ----------------------------------- */

strcpy(s, t)
string s, t;
{
  while (*s++ = *t++);
}

/* ------------------------------------- */
/*	append string t to string s      */
/* ------------------------------------- */

strapp(s, t)
string s, t;
{
  while (*s) s++;
  strcpy(s, t);
}


/* ---------------------------------------- */
/*	compare string s with string t      */
/*	  return <0 if s < t		    */
/*		  0 if s == t		    */
/*		 >0 if s > t		    */
/* ---------------------------------------- */

strcmp(s, t)
string s, t;
{
  while (*s == *t) {
      if (*s++ == '\0') return(0);
      t++;
  }
  return (*s - *t);
}
