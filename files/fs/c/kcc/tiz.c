#define NULL 0
int optpar, optgen, optobj;
struct flagent {		/* Structure of an entry in flag table */
	char *name;		/* Flag name */
	int *fladdr;		/* Address of runtime flag value */
	int flval;		/* Value to set it to if user gives flag */
};
struct flagent copttab[] = {
	"all",	NULL,	   0,	/* First element is special */
	"parse", &optpar,  1,	/* Parse tree optimization */
	"gen",	&optgen,   1,	/* Code generator optimizations */
	"object", &optobj, 1,	/* Object code (peephole) optimizations */
	NULL,	NULL,	NULL
};
