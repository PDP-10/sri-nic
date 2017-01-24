/*
 *	make [-f makefile] [-dimnpqrst] [target(s) ...]
 *
 *	(Better than EON mk but not quite as good as UNIX make)
 *
 *	-d debug output on stderr
 *	-f makefile name
 *	-i ignore exit status
 *	-m change memory requirements (EON only)
 *	-n pretend to make
 *	-p print all macros & targets
 *	-q question up-to-dateness of target.  Return exit status 1 if not
 *	-r don't not use inbuilt rules
 *	-s make silently
 *	-t touch files instead of making them
 */

#include "h.h"

#ifdef EON
#include <sys/err.h>
#endif

#ifdef KCC_20
#include <errno.h>
#endif

#ifdef MSC
#include <errno.h>
#endif

#ifdef OS9
#include <errno.h>
#endif

#ifdef UNIX
#include <sys/errno.h>
#endif

#ifdef VMS
#include <errno.h>
#endif

#ifdef EON
#define MEMSPACE	(16384)
#endif


char *			myname;
char *			makefile;	/*  The make file  */

#ifdef EON
unsigned		memspace = MEMSPACE;
#endif

FILE *			ifd;		/*  Input file desciptor  */
bool			debug = FALSE;	/*  debug printout on stderr */
bool			domake = TRUE;	/*  Go through the motions option  */
bool			ignore = FALSE;	/*  Ignore exit status option  */
bool			silent = FALSE;	/*  Silent option  */
bool			print = FALSE;	/*  Print debuging information  */
bool			rules = TRUE;	/*  Use inbuilt rules  */
bool			dotouch = FALSE;/*  Touch files instead of making  */
bool			quest = FALSE;	/*  Question up-to-dateness of file  */


void
main(argc, argv)
int			argc;
char **			argv;
{
	register char *		p;		/*  For argument processing  */
	int			estat = 0;	/*  For question  */
	register NAMEP	np;

	myname = (argc-- < 1) ? "make" : *argv++;

#ifdef VMS
	myname = "make";	/* otherwise get ugly dev:[dir]make.exe.## */
#endif

	while ((argc > 0) && (**argv == '-'))
	{
		argc--;		/*  One less to process  */
		p = *argv++;	/*  Now processing this one  */

		while (*++p != '\0')
		{
			switch(*p)
			{
			case 'd':
				debug = TRUE;
				break;

			case 'f':	/*  Alternate file name  */
				if (*++p == '\0')
				{
					if (argc-- <= 0)
						usage();
					p = *argv++;
				}
				makefile = p;
				goto end_of_args;
#ifdef EON
			case 'm':	/*  Change space requirements  */
				if (*++p == '\0')
				{
					if (argc-- <= 0)
						usage();
					p = *argv++;
				}
				memspace = atoi(p);
				goto end_of_args;
#endif
			case 'n':	/*  Pretend mode  */
				domake = FALSE;
				break;
			case 'i':	/*  Ignore fault mode  */
				ignore = TRUE;
				break;
			case 's':	/*  Silent about commands  */
				silent = TRUE;
				break;
			case 'p':
				print = TRUE;
				break;
			case 'r':
				rules = FALSE;
				break;
			case 't':
				dotouch = TRUE;
				break;
			case 'q':
				quest = TRUE;
				break;
			default:	/*  Wrong option  */
				usage();
			}
		}
	end_of_args:;
	}

#ifdef EON
	if (initalloc(memspace) == 0xffff)  /*  Must get memory for alloc  */
		fatal("Cannot initalloc memory");
#endif
	/* NB: Must check for non-NULL pointer--Sun core dumps if NULL is passed to strcmp! */
	if (makefile && strcmp(makefile, "-") == 0)/*  Can use stdin as makefile */
		ifd = stdin;
	else
		if (!makefile)		/*  If no file, then use default */
		{
			if ((ifd = fopen(DEFN1, "r")) == NULL_FILEP)
#ifdef EON
				if (errno != ER_NOTF)
					fatal("Can't open %s; error %02x", DEFN1, errno);
#endif

#ifdef KCC_20
				fatal("Can't open %s", DEFN1);
#endif

#ifdef MSC
				fatal("Can't open %s", DEFN1);
#endif

#ifdef OS9
				fatal("Can't open %s", DEFN1);
#endif

#ifdef UNIX
				if (errno != ENOENT)
					fatal("Can't open %s; error %02x", DEFN1, errno);
#endif

#ifdef VMS
				fatal("Can't open %s", DEFN1);
#endif

			if ((ifd == NULL_FILEP)
				  && ((ifd = fopen(DEFN2, "r")) == NULL_FILEP))
				fatal("Can't open %s", DEFN2);


		}
		else
			if ((ifd = fopen(makefile, "r")) == NULL_FILEP)
				fatal("Can't open %s", makefile);

	makerules();

	(void)setmacro("$", "$");

	while (argc && (p = index(*argv, '=')))
	{
		char		c;

		c = *p;
		*p = '\0';
		(void)setmacro(*argv, p+1);
		*p = c;

		argv++;
		argc--;
	}

	input(ifd);	/*  Input all the gunga  */
	fclose(ifd);	/*  Finished with makefile  */
	lineno = 0;	/*  Any calls to error now print no line number */

	if (print)
		prt();	/*  Print out structures  */

	np = newname(".SILENT");
	if (np->n_flag & N_TARG)
		silent = TRUE;

	np = newname(".IGNORE");
	if (np->n_flag & N_TARG)
		ignore = TRUE;

	precious();

	if (!firstname)
		fatal("No targets defined");

	circh();	/*  Check circles in target definitions  */

	if (!argc)
		estat = make(firstname, 0);
	else while (argc--)
	{
		if (!print && !silent && strcmp(*argv, "love") == 0)
			printf("Not war!\n");
		estat |= make(newname(*argv++), 0);
	}

	if (quest)
		EXIT(estat);
	else
		EXIT(0);
}

void
usage()
{
	fprintf(stderr,
#ifdef EON
	"Usage: %s [-f makefile] [-dimnpqrst] [macro=val ...] [target(s) ...]\n",
#else
	"Usage: %s [-f makefile] [-dinpqrst] [macro=val ...] [target(s) ...]\n",
#endif
	myname);
	EXIT(1);
}


void
fatal(msg, a1, a2, a3, a4, a5, a6)
char	*msg;
{

#ifdef KCC_20
	fprintf(stderr, "?");		/* error flag for batch use */
#endif

	fprintf(stderr, "%s: ", myname);
	fprintf(stderr, msg, a1, a2, a3, a4, a5, a6);
	fputc('\n', stderr);
	if (errno)
	    perror("");
	EXIT(1);
}
