/**************************************************************************
  PAGEIO.H - Header file for Pageio ADT

    Todd Koumrian
    SRI International
    3 November 1988

    The PageIO Abstract Data Type (ADT) allows I/O operations on two
    streams to be handled in with the abstraction that the streams are
    divided into sections called pages.  This ADT is specifically
    designed for use with REVER, the page-reversal utility.  (REVER.C)

    For further details of the arguments and usage, see PAGEIO.C

    To use the pageio ADT, #include "pageio.h" and link with pageio.rel.
**************************************************************************/

                         /* Symbolic names for special characters */
#define NUL      '\0'       /* NUL character - string terminator */
#define NEWLINE  '\n'       /* Some combination of <CR><LF> */
#define FF       '\f'       /* ASCII <FF> char */

#define MAX_PLINE  512      /* Maximum length for a line of Postscript */
#define MAX_PAGES  1024     /* Guarenteed bookkeeping for this many pages */

typedef unsigned int  u_int;
typedef unsigned long u_long;

extern void init();              /* Init ADT, open files or err if can't.  */
extern int  get_chr();           /* Get char from source file or EOF */
extern char *get_line();     /* Get a line of source and strip '\n' or NULL. */
extern void put_string();        /* Output an arbitrary string to dest. */
extern void insert_file();       /* Insert file at current place in dest. */
extern void end_line();          /* Terminate and log current line */
extern void end_page();          /* Mark end of page */
extern void start_page();        /* Have count start of new page */
extern void copy_page();         /* Copy a page to dest file. */
extern void wrap_up();           /* Close ADT transaction */
extern int  page_count();        /* Returns current page count */
extern void replay();            /* Switch from record to playback-only */
