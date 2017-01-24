/* STDLIB.H - ANSI General Utility declarations and definitions.
**
**	Copyright (C) 1987 by Ken Harrenstien, SRI International
**
**	These declarations correspond to the ANSI X3J11 draft of July 1986.
*/

extern int _n_exit_func;		/* # of registered exit functions */
extern void (*_exit_func[])();		/* registered exit functions ptrs */

/*
 *	New type decls
 */

typedef int onexit_t;
typedef struct { int quot; int rem; } div_t;
typedef struct { long quot; long rem; } ldiv_t;

/*
 *	New constants
 */

#define RAND_MAX (((unsigned int)-1)>>1)
#include <math.h>	/* To get ERANGE and HUGE_VAL definitions */

#define MAX_EXIT_FUNCTIONS	32	/* max # of functions which can be
					   registered with onexit() */

#ifndef __STDC__			/* if this isn't defined yet */
#define __STDC__ 0			/* then we're certainly not ANSI C */
#endif

#if (__STDC__)				/* ANSI C? */

/*
 *	Full ANSI declarations
 */

#include <stddef.h>			/* Get size_t etc */

/* Conversions */
double	atof(const char *nptr);
int	atoi(const char *nptr);
long	atol(const char *nptr);
double	strtod(const char *nptr, char **endptr);
long	strtol(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);

/* Pseudo-random functions */
int rand(void);
void srand(unsigned int seed);

/* Memory management */
void *calloc(size_t nmemb, size_t size);
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

/* Environment */
void abort(void);
void exit(int status);
char *getenv(const char *name);
onexit_t onexit(onexit_t (*func)(void));
int system(const char *string);
char *bsearch(const void *key, const void *base,
	size_t nmemb, size_t size,
	int (*compar)(const void *, const void *));
void qsort(void *base, size_t nmemb, size_t size,
	int (*compar)(const void *, const void *));

/* Arith functions */
int abs(int j);
div_t div(int numer, int denom);
long labs(long j);
ldiv_t ldiv(long numer, long denom);

#else					/* Next page is old-style stuff */

/*
 *	 Old-style declarations
 */

/* Conversions */
double atof();
int atoi();
long atol();
double strtod();
long strtol();
unsigned long strtoul();

/* Pseudo-random functions */
int rand();
void srand();

/* Memory management */
char *calloc();
void free();
char *malloc();
char *realloc();
void cfree();					/* CARM and BSD function */
char *clalloc(), *mlalloc(), *relalloc();	/* CARM functions */


/* Environment */
void abort();
void exit();
char *getenv();
onexit_t onexit();
int system();
char *bsearch();
void qsort();

/* Arith functions */
int abs();
div_t div();
long labs();
ldiv_t ldiv();

#endif
