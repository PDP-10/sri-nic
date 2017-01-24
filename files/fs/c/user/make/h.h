/*
 *	Include header for make
 */

/* Include the most-needed system files */
#include <stdio.h>
#include <ctype.h>

#ifdef EON
typedef long	time_t;
#endif

#ifdef KCC_20
#include <sys/types.h>			/* need for time_t */
#endif

#ifdef MSC
#include <sys/types.h>			/* need for time_t */
#include <sys/utime.h>
#include <process.h>			/* for library type declarations */
#include <stdlib.h>			/* for library type declarations */
#include <string.h>			/* for library type declarations */
#include <time.h>			/* for library type declarations */
#define index strchr
#define rindex strrchr
#endif

#ifdef OS9
typedef long	time_t;
#endif

#ifdef UNIX
#include <sys/types.h>			/* need for time_t */
#endif

#ifdef VMS
#include <types.h>			/* need for time_t */
#define index strchr
#define rindex strrchr
#endif

#ifdef VMS
#define EXIT vms_exit
#else
#define EXIT exit
#endif

#ifndef uchar
#ifdef OS9
typedef char	uchar;
typedef int	void;
#define fputc		putc
#else
typedef unsigned char	uchar;
#endif
#endif

typedef uchar	bool;


#define TRUE		((bool)(1))
#define FALSE		((bool)(0))
#define max(a,b)	((a)>(b)?(a):(b))

#define DEFN1		"makefile"		/*  Default names  */

#ifdef EON
#define DEFN2		"Makefile"
#define MORE		"\\\n"		/* how to continue shell command lines */
#endif

#ifdef KCC_20
#define DEFN2		"Makefile"
#define MORE		"-\n"		/* how to continue shell command lines */
#endif

#ifdef MSC
#define DEFN2		"Makefile"
#define MORE		"\\\n"		/* how to continue shell command lines */
#endif

#ifdef OS9
#define DEFN2		"Makefile"
#define MORE		"\\\n"		/* how to continue shell command lines */
#endif

#ifdef UNIX
#define DEFN2		"Makefile"
#define MORE		"\\\n"		/* how to continue shell command lines */
#endif

#ifdef VMS
#define DEFN2		"Makefile"
#define MORE		"-\n"		/* how to continue shell command lines */
#endif

#ifdef MSC
#define LZ	(4096)	/*  Line size -- BIG for long dependency lists */
#else
#define LZ	(20480)	/*  Line size -- BIG for long dependency lists */
#endif

/* Make structure pointer types so we can use them in struct's */
typedef struct cmd * 		CMDP;
typedef struct depend *		DEPENDP;
typedef struct line * 		LINEP;
typedef struct macro * 		MACROP;
typedef struct name * 		NAMEP;

/*
 *	A name.  This represents a file, either to be made, or existant
 */
typedef struct name
{
	NAMEP		n_next;		/* Next in the list of names */
	char *		n_name;		/* Called */
	LINEP		n_line;		/* Dependencies */
	time_t		n_time;		/* Modify time of this name */
	uchar		n_flag;		/* Info about the name */
} NAME;

#define N_MARK		0x01			/* For cycle check */
#define N_DONE		0x02			/* Name looked at */
#define N_TARG		0x04			/* Name is a target */
#define N_PREC		0x08			/* Target is precious */
#define N_DOUBLE	0x10			/* Double colon target */

/*
 *	Definition of a target line.
 */

typedef struct line
{
	LINEP		l_next;		/* Next line (for ::) */
	DEPENDP		l_dep;		/* Dependents for this line */
	CMDP		l_cmd;		/* Commands for this line */
} LINE;

/*
 *	List of dependents for a line
 */
typedef struct depend
{
	DEPENDP		d_next;		/* Next dependent */
	NAMEP		d_name;		/* Name of dependent */
} DEPEND;


/*
 *	Commands for a line
 */
typedef struct cmd
{
	CMDP		c_next;		/* Next command line */
	char *		c_cmd;		/* Command line */
} CMD;


/*
 *	Macro storage
 */
typedef struct macro
{
	MACROP		m_next;		/* Next variable */
	char *		m_name;		/* Called ... */
	char *		m_val;		/* Its value */
	uchar		m_flag;		/* Infinite loop check */
} MACRO;


extern bool		debug;
extern bool		domake;
extern bool		dotouch;
extern NAMEP		firstname;
extern bool		ignore;
extern int		lineno;
extern MACROP		macrohead;
extern char *		myname;
extern struct name	namehead;
extern bool		quest;
extern bool		rules;
extern bool		silent;
extern char		str1[];
extern char		str2[];

#ifndef VMS
extern int		errno;
#endif

#ifdef ANSI	/* ANSI function and argument type declarations */
void           check(NAMEP);
void           circh(void);
void           docmd2(NAMEP,LINEP);
void           docmds(NAMEP);
void           doexp(char **,char *,int *,char *);
int            dosh(char *,char *);
bool           dyndep(NAMEP);
void           error(char *,int ,int ,int);
void           expand(char *);
void           fatal(char *,int ,int ,int ,int ,int ,int);
bool           getline(char *,FILE*);
char *         getmacro(char *);
MACROP         getmp(char *);
char *         gettok(char **);
char *         index(char *,char);
void           input(FILE*);
void           main(int ,char **);
int            make(NAMEP,int);
void           make1(NAMEP,LINEP,DEPENDP);
void           makerules(void);

#ifndef MSC
char *         malloc(int);
#endif

void           modtime(NAMEP);
CMDP           newcmd(char *,CMDP);
DEPENDP        newdep(NAMEP,DEPENDP);
void           newline(NAMEP,DEPENDP,CMDP,bool);
NAMEP          newname(char *);
void           newsuffix(char *);
void           precious(void);
void           prt(void);
char *         rindex(char *,char);
MACROP         setmacro(char *,char *);
char *         suffix(char *);
void           touch(NAMEP);
void           usage(void);
#else		/* old-style function declarations */
void           check();
void           circh();
void           docmd2();
void           docmds();
void           doexp();
int            dosh();
bool           dyndep();
void           error();
void           expand();
void           fatal();
bool           getline();
char *         getmacro();
MACROP         getmp();
char *         gettok();
char *         index();
void           input();
void           main();
int            make();
void           make1();
void           makerules();

#ifndef MSC
char *         malloc();
#endif

void           modtime();
CMDP           newcmd();
DEPENDP        newdep();
void           newline();
NAMEP          newname();
void           newsuffix();
void           precious();
void           prt();
char *         rindex();
MACROP         setmacro();
char *         suffix();
void           touch();
void           usage();
#endif

/* Define some convenient shorthands for null typecasts */
#define NULL_CHARP	((char *)NULL)
#define NULL_CMDP	((CMDP)NULL)
#define NULL_DEPENDP	((DEPENDP)NULL)
#define NULL_FILEP	((FILE *)NULL)
#define NULL_LINEP	((LINEP)NULL)
#define NULL_MACROP	((MACROP)NULL)
#define NULL_NAMEP	((NAMEP)NULL)
