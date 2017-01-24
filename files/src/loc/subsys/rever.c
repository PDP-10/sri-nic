/***************************************************************************
  REVER.C  -  Page reversal program

      Todd Koumrian
      SRI International
      18 April 1990

      This program is designed as a page-reverser for use with laser
      printers that do not have page-reversal capability.

      Arguments:
          rever [ -switch [parameter]]*  [in_filename [out_filename]]

         Allowable switches are:

           %  b   Turns on "/BOOK"-style page organization.
	   %  B   Select Book style treatment of Scribed landscaped pages.
           %  c   Selects source file page-counting only.
           %  l   Turns on Landscaped output
           %  q   Turns on 4-Up vertical (Quad) printing.
           %  Q   Turns on 4-Up horizontal (Quad) printing.
	   %  w   Turns on Quad style treatment of Scribed landscaped pages.
              f   Allows selection of one of the Postscript built-in fonts.
              p   Allows selection of font point-size.
              s   Allows selection of line-spacing.
           +  v   Print pages and ranges exactly as specified, and in
                   the order specified.
           +  u   Print pages in ascending order, regardless of the
                   the order in request list.  Even if a page is
                   specified more than once, it will only be printed
                   once.

      Note that switches marked with '%' or '+' are mutually exclusive
      among themselves.  Only one from each group may be specified.

      If the output filespec is omitted, output will be sent to stdout.
      Additionally, if both the input and output filespecs are omitted,
      the input will bet taken from stdin and the output will go to stdout.

      REVER.C uses the PageIO ADT for all Page-related file operations
      as well as for page marking and bookkeeping functions.  The
      PageIO ADT is implemented in PAGEIO.C.

      REVER was constructed in a modular fashion, in order to allow
      for easy expansion of capabilities.  Version 2.0 was the premier
      of this new structure, and as an example, the implementation of
      LATEX took only a few minutes.

  9-Jun-88: 'g' text mode option (the "Guilty-conscience" reversal),
              added.  Implemented as mode but advertised as option.

 17-Jun-88: 'b' text option added (/BOOK) option to print 2 pages
               landscaped on each output page.

 21-Jun-88:  Changing put_formfeed() to pg_put_string(), so as to allow
               outputting FF's and NL's, both needed to work with the
               current /BOOK prepend file.  Adding pg_insert_file to
               prepend BOOK.PS, and make BOOK mode output a PS file.

 10-Jul-88:  Reformatting program as suggested by KLH.  Created a 
               header file for use with pageio ADT.

 12-Jul-88:  Found non-portable strcmp() in do_scribe and do_latex.
               rearranged loop to avoid comparisons with NULL pointers.
               Added machine variant define's to program.  A setbuf()
               will be needed for the SUN, since it defaults to line
               buffered tty output (can't see ... entertainment.

 16-Jul-88:  Continued splitting of scanning and reversing functions.
               Prepared things for auto-identify.

 23-Jul-88: Auto-id, count, font, point-size, and linespacing features
              fully implemented.  Special prepend file for plaintext
              written especially for REVER.  PARSE_ARGS() needs to
              updated to parse the new features.

 24-Jul-88: Updated PARSE_ARGS() with new switches and simple parsing
              technique.  Added/Changed err messages.  Fixed font selection
              for text and book.  Need to make fontnames caps in right places
              and set maxlines for different point sizes.

 31-Jul-88: Added 'F' option to prompted input - query_args().

  6-Aug-88: Added 'l' landscaped option and updated prepend files.

 31-Aug-88: Fixed problem with do_book(), which was not doing scheme NONE
              properly.  Adding -q Quad output format.

  1-Sep-88: Implemented reverse_quad() with page selection schemes.

  4-Sep-88: Implemented Scribe hacks for QUAD and BOOK.  Added reverse_scribe
              _book and reverse_scribe_quad, with scribe_prelude().  Updated
              main() and do_scribe().

  8-Sep-88: Relaxed restriction on "sp" switches with COUNT mode, more info
              is printed.

 10-Sep-88: Changed parse_args() and query_args() to insist on ".ps" file
              extension for user supplied destinations.  Main() alerts
              user of any changes that had to be made.

 13-Sep-88: Added conditionals to use stolen string libraries on Unixes.

 15-Sep-88: Changed verify_font() to use strCMP.

 16-Sep-88: Added fattempt flag to screen out -f Courier with SCRIBE
              (slipped between the cracks since default = none for scribe.

 21-Sep-88: Made provision for two kinds of QUADs, QUADH & QUADV.
              Renamed files to use system directories.

 5-Oct-88: Renamed insert files for directory change on Suns.
 5-Nov-88: Removed prompted input to make way for piping and redirection.
            Paging package rewritten, and all reversals rewritten to take
            advantage.  Much code condensation.  General Adobe format reading
            and reversing added, Scribe made just a variant.  All routines
            touched; this was a major rewrite.  REVID module incorporated
            into REVER, broken into id_source() and get_prolog().
16-Nov-88: Minor changes - changed "Scanning" to "Reading", "Copying" to
            "Writing".
19-Nov-88: Fixed pnums_parse() to return when encountering filenames that
            start with numbers.
17-Dec-88: Added global var first_char to be the first character of the
            input which is sampled by id_source().  Get_text() needs to
            know if it was a NL or FF.  Added more useful page count output.
 3-Jan-89: Updated PAGEIO functions to use the new naming convention.
 7-Feb-89: Switched over text file processing to encapsulate text in a
            Postscript program, rather than use the previous prepend method.
            Gives better control, performance.  Allows backspace processing.
 9-Feb-89: Add '-t' tabspace setting option.
11-Feb-89: Removed defines for obsolete prepend files.
17-Dec-89: Added -B bookland mode for scribe.
18-Apr-90: Added -w "wierd" quadland mode for scribe.
 **************************************************************************/

#include <c-env.h>              /* NIC machine dependent defs */
#include <stdio.h>              /* Usual stuff */
#include <ctype.h>              /* Need various character functions */
#include <math.h>               /* Need floor */
#include "pageio.h"             /* Use PageIO ADT */
#include "textps.h"             /* Use text encapsulation routines */

#if (SYS_UNIX)
#include "string.h"             /* Need string functions + strstr() */
#include "strung.h"             /* Need strSTR() */
#else                        /* BSD doesn't have it */
#include <string.h>             /* KCC has string stuff */
#include <strung.h>             /* KCC has it, has been installed on suns */
#endif

typedef int bool;     /* Type for Boolean only variables */
#define FALSE 0                     /* Boolean False value */
#define TRUE  1                     /* Boolean Truth value */

#define MAXFILENAME (39 * 4)    /* Maximum filename string length */
#define MAXFONTNAME     40      /* Max font name length */
#define MAX_POINT_SIZE 200      /* Maximum font point size ??? */
#define MAX_TABSPACE   140      /* Maximum tabspace substitution value */

#define DEF_POINTSIZE  11       /* Default font point size used by prepend */
#define DEF_LINESPACE (DEF_POINTSIZE + 1)
#define DEF_LINES      60       /* Default lines per page with def font/size */
#define DEF_FORMLENGTH 10       /* Default form length in inches */

#if SYS_T20
#define TEXT_PRELUDE "SYSTEM:REVER-PRELUDE.PS"
#define BOOK_SCRIBE  "SYSTEM:REVER-SCRIBE-BOOK.PS"
#define BOOKLAND_SCRIBE "SYSTEM:REVER-SCRIBE-BOOKLAND.PS"
#define QUADLAND_SCRIBE "SYSTEM:REVER-SCRIBE-QUADLAND.PS"
#define QDV_SCRIBE   "SYSTEM:REVER-SCRIBE-QUADV.PS"
#define QDH_SCRIBE   "SYSTEM:REVER-SCRIBE-QUADH.PS"
#else
#define TEXT_PRELUDE "/usr/local/lib/rever/rever-prelude.ps"
#define BOOK_SCRIBE  "/usr/local/lib/rever/rever-scribe-book.ps"
#define BOOKLAND_SCRIBE "/usr/local/lib/rever/rever-scribe-bookland.ps"
#define QUADLAND_SCRIBE "/usr/local/lib/rever/rever-scribe-quadland.ps"
#define QDH_SCRIBE   "/usr/local/lib/rever/rever-scribe-quadh.ps"
#define QDV_SCRIBE   "/usr/local/lib/rever/rever-scribe-quadv.ps"
#endif

#define ADBE      "PS-Adobe"
#define CREATOR   "%%Creator:"
#define ENDPROLOG "%%EndProlog"
#define PGHEAD    "%%Page:"
#define TRAILER   "%%Trailer"
#define SCRB      "Scribe"
#define DVAPP     "DVIAPPLE"
#define DV2PS     "DVI2PS"

#define SCRB_BPAGE "/BS {"      /* Start of Scribe's start-page routine */
#define SCRB_EPAGE "/ES {"      /* Start of Scribe's end-page routine */

                    /* Source id types */
#define BADFILE   0     /* Problem with source file - cannot id */
#define TEXTFILE  1     /* Plain old ascii textfile */
#define ADOBE     2     /* Adobe formatted Postscript - none of the others */
#define SCRIBE    3     /* A Scribe generated PS file */
#define DVI2PS    4     /* A La/TeX DVI2PS generated PS file */
#define DVIAPPLE  5     /* A La/TeX DVIAPPLE generated PS file */
#define UNKNOWNPS 6     /* An unknown type of PS file */

                         /* Processing modes */
#define PLAIN  0            /* ASCII textfile mode */
#define BOOK   1            /* "Book" style double landscaped ordering */
#define COUNT  2            /* Count pages in source file only */
#define LAND   3            /* Landscaped output */
#define QUADH  4            /* 4-Up Quad (Horz) output */
#define QUADV  5            /* 4-Up Quad (Vert) output */
#define BOOKLAND 6   	    /* Special booked scribe landscape */
#define QUADLAND 7          /* Special quad'ed scribe landscape */

                         /* Page specification schemes */
#define NONE   0            /* Print entire document reversed */
#define PAGED  1            /* PAGED mode - print pages exactly as specified */
#define UNIQUE 2            /* UNIQUE mode - print specified pages ascending */

                         /* ERR_HANDLER error codes */
#define BAD_SWITCH      1      /* Unknown or bad option requested */
#define NO_DATA         2      /* Parse_args(): no data following switch */
#define BAD_PAGE_NUM    4      /* Missing or badly formed page number */
#define BAD_SPEC        5      /* Badly formed page range (delimiter bad) */
#define BAD_RANGE       6      /* Invalid page range */
#define NO_PAGE         7      /* No pages specified when using '-v' or 'u' */
#define TABSPACE_ERR    8      /* Tabspace param in error */
#define MULT_MODES      9      /* More than one processing mode specified */
#define MULT_SCHEMES   10      /* More than one page select scheme specified */
#define OPT_COMB_ERR   11      /* Option combination not allowed */
#define OPT_FILE_ERR   12      /* Option not allowed with current file type */
#define OPT_SCHEME_ERR 13      /* Option not allowed with current scheme */
#define FILE_BAD       14      /* Source file is bad */
#define UNKNOWN_PS_SOURCE 15   /* Source file is an unknown PS format */
#define FONT_ERR       16      /* Selected font not found */
#define POINT_SIZE_ERR 17      /* Selected point size is invalid */
#define LINE_SPACE_ERR 18      /* Selected linespacing is invalid */
#define PRELUDE_PARAM  19      /* Bad Scribe hack requested */
#define NO_PLAYBACK    20      /* PAGEIO says playback unavailable - err */
#define OPT_NOT_ALLOWED 21     /* Format option not available for input type */

/*-------------
  Function list
  -------------*/

#define reverse_adobe_book  revr01
#define reverse_adobe_plain revr02
#define reverse_adobe_quad  revr03
#define reverse_book        revr04
#define reverse_plain       revr05
#define reverse_quad        revr06

#define lines_from_space lin001

int  do_adobe();
int  do_text();

void get_text();        /* Read and mark a text file */
int  get_prolog();      /* Read and log a PS prolog and check type */
bool get_script();      /* Read and mark a PS script - was trailer found? */
void get_trailer();     /* Read and log PS trailer */

int  reverse_adobe_book();
int  reverse_adobe_plain();
int  reverse_adobe_quad();
int  reverse_book();
int  reverse_plain();

void err_handler();
bool is_even();
int  lines_from_space();
void parse_args();
int  pnums_parse();
void scribe_prelude();
int  search_pagelist();
char *verify_font();

/*-----------------------
  Global vars and structs
  -----------------------*/

int  page_list[MAX_PAGES];          /* List for individual page requests */
int  id;                            /* Type of source stream */
int  first_char;                    /* 1st char of input (see id_source()) */
int  option =   PLAIN;              /* Reversal option (BOOK, COUNT, etc) */
int  scheme =   NONE;               /* Page numbering scheme */
int  maxlines = DEF_LINES;          /* set up lines/page to default */

int  requests;                      /* Index to top of PAGED scheme queue */
char *source;                       /* Source filename */
char *dest;                         /* Pointers to dest filename */
char dbuf[MAXFILENAME];             /* Buffer for dest filename */
bool dchanged = FALSE;              /* Flag indicating dest name was altered */

char *font;                         /* TEXT font */
bool fattempt = FALSE;              /* Flag indicating -f was attempted */

int  point_size = DEF_POINTSIZE;    /* TEXT point size - 11p is default */
int  linespace = DEF_LINESPACE;     /* TEXT linespacing - point_size + 1 */
int  tabspace = TABSPACES;          /* Set # of spaces to substitute for tab */
bool text_options = FALSE;          /* Flag for text options set */

char *font_list[] = { "Courier", "Courier-Bold", "Courier-Oblique",
           "Courier-BoldOblique", "Helvetica", "Helvetica-Bold",
           "Helvetica-Oblique", "Helvetica-BoldOblique", "Times-Roman",
           "Times-Bold", "Times-Italic", "Times-BoldItalic", "Symbol", "" };

struct {                /* Structure of %%Creator: identifiers */
    char *ps_str;           /* string */
    int  ps_val;            /* corresponding value to return */
} ps_types[] = {
    SCRB,  SCRIBE,
    DVAPP, DVIAPPLE,
    DV2PS, DVI2PS,
    NULL,    0
};


/**************************************************************************
  Main - Main program
***************************************************************************/


main(argc, argv)
int   argc;
char *argv[];
{
    int pages;                  /* Source/Dest file page counter */
    int j;
    double factor;

#if SYS_UNIX                    /* For Sun Unix, need to set stderr */
    setbuf(stderr, NULL);       /*   buffering to no buffering */
#endif

    font = font_list[0];            /* First entry in font_list is default */

    for (j = 0; j < MAX_PAGES; j++) /* Clear request list */
        page_list[j] = FALSE;

    parse_args(argc, argv);         /* Try to parse the args */
                                    /* Global vars may change */
    tp_set_tabspace(tabspace);      /* Explicitly set the tabspacing */

    if (dchanged)                   /* Alert user if dest needed alterations */
        fprintf(stderr, "Destination changed to \"%s\"\n", dest);

    if (option == COUNT)                 /* Init PAGEIO module */
        pg_init(source, dest, FALSE);   /* Do not record source for COUNT */
    else pg_init(source, dest, TRUE);   /* Record all other options though */

    id = id_source();                   /* Identify source file type */

    fputs("Source file type = ", stderr);
    switch (id) {
        case BADFILE:
            fputs("BAD\n", stderr);
            err_handler(FILE_BAD);
            break;

        case UNKNOWNPS:
            fputs("UNKNOWN\n", stderr);
            err_handler(UNKNOWN_PS_SOURCE);
            break;

        case TEXTFILE:
            fputs("TEXT\n", stderr);
            fprintf(stderr, "Lines/panel = %d\n", maxlines);
            pages = do_text();
            break;

        case ADOBE:
            id = get_prolog();  /* Read entire prolog and further identify */

            switch (id) {       /* Now handle appropriately */
                case BADFILE:
                    fputs("BAD\n", stderr);
                    err_handler(FILE_BAD);
                    break;

                case ADOBE:
                    fputs("ADOBE\n", stderr);
                    if ((option == PLAIN || option == COUNT) && !text_options)
                        pages = do_adobe();
                    else err_handler(OPT_FILE_ERR);
                    break;

                case SCRIBE:
                    fputs("SCRIBE\n", stderr);
                    if (option != LAND && !text_options && !fattempt)
                        pages = do_adobe();
                    else err_handler(OPT_FILE_ERR);
                    break;

/* This is really wishful thinking.  I've left the TeX modes in here,
   just in case I'm able to fix DVI2PS soon, but it doesn't look good.
   Incidentally, since DVI2PS will fail the previous Adobe test, this code
   will never execute, so I haven't bothered to comment it out. */

                case DVIAPPLE:
                    fputs("DVIAPPLE\n", stderr);
                    if ((option == PLAIN || option == COUNT) && !text_options)
                        pages = do_adobe();
                    else err_handler(OPT_FILE_ERR);
                    break;

                case DVI2PS:
                    fputs("DVI2PS\n", stderr);
                    if ((option == PLAIN || option == COUNT) && !text_options)
                        pages = do_adobe();
                    else err_handler(OPT_FILE_ERR);
                    break;
            }
            break;
    }

    pg_close();         /* Alert PageIO ADT that transaction is concluded */

    switch (option) {
        case PLAIN:
        case LAND:
            if (dest == NULL)
                dest = "standard output";
            fprintf(stderr, "%s contains %d pages.\n", dest, pages);
            break;
        case COUNT:
            if (source == NULL)
                source = "standard input";
            fprintf(stderr, "%s contains %d pages.\n", source, pages);
            break;
        default:
            if (dest == NULL)
                dest = "standard output";
            if (option == BOOK || option == BOOKLAND)
                factor = 2.0;
            else factor = 4.0;
            fprintf(stderr, "%s contains %d panels on %.0f pages.\n", dest,
                    pages, ceil((double) (pages / factor)));
            break;
    }
    exit(0);
}                     /* end MAIN */

/******************************************************************************
  PARSE_ARGS

               Arguments:  ARGC, ARGV  from MAIN
                 Returns:  nothing.
            Side Effects:  Sets OPTION, SCHEME, SOURCE and DEST.
                   Calls:  PNUMS_PARSE which alters PAGE_LIST[] and
                             may set REQUESTS if scheme is PAGED.

       This function parses the command-line arguments to obtain such
       information as the file to process, output filename, and mode switches
       like BOOK or COUNT mode or page output orders.

       If some, but not all of the necessary arguments were supplied, an
       error will occur, the user will be notified, and the program will
       terminate.  Note that finding a second instance of either a mode
       or a page scheme results in an error.

       Font, fontsize, linespacing, and dest filename arguments are verified
       within parse_args.  Parse_args() insists on destination filename
       extension ".ps".
 *****************************************************************************/

void parse_args(argc, argv)
int argc;
char *argv[];
{
    int  i = 1;                    /* Current argv argument counter */
    char *spot;                    /* Used in checking dest file extension */
    bool iss = FALSE;              /* Linespacing set flag */

    while ((i < argc) && (argv[i][0] == '-')) {  /* Step through switches */
        if (strlen(argv[i]) > 2)                 /* Only 1 char switches */
            err_handler(BAD_SWITCH);

        switch (argv[i][1]) {                    /* Check out switch */

            case 'b':                            /* BOOK-style */
	        if (option == PLAIN) {
		    option = BOOK;
		    i++;
		}
		else err_handler(OPT_COMB_ERR);  /* Can't have BOOK + other */
		break;

	    case 'B':
		if (option == PLAIN) {
		    option = BOOKLAND;
		    i++;
		}
		else err_handler(OPT_COMB_ERR);
                break;

            case 'c':
                if (option == PLAIN) {
                    if (scheme == NONE) {
                        option = COUNT;        /* Set count mode */
                        i++;                   /* Skip to next arg */
                    }
                    else err_handler(OPT_SCHEME_ERR);  /* No schemes allowed */
                }
                else err_handler(OPT_COMB_ERR);   /* Can't have COUNT + BOOK */
                break;

            case 'f':                          /* font selection request */
                if (option != COUNT) {         /* Not allowed in COUNT */
                    i++;
                    fattempt = TRUE;           /* Indicate flag was used */
                    if (i >= argc)             /* Check for fontname */
                        err_handler(NO_DATA);
                    if ((font = verify_font(argv[i])) == NULL)
                        err_handler(FONT_ERR);
                    if (font != font_list[0])     /* Check for default echo */
                        text_options = TRUE;      /* Indicate using options */
                }
                else err_handler(OPT_COMB_ERR);   /* Can't do font and count */
                i++;
                break;

            case 'l':                             /* LANDSCAPE-style */
                if (option == PLAIN) {
                    option = LAND;
                    i++;
                }
                else err_handler(OPT_COMB_ERR);   /* Can't have LAND + other */
                break;

            case 'p':
                i++;
                if (i >= argc)                  /* Check for point size */
                    err_handler(NO_DATA);
                point_size = atoi(argv[i++]);   /* Get and convert */
                if ((point_size < 1) || (point_size > MAX_POINT_SIZE))
                    err_handler(POINT_SIZE_ERR);
                if (!iss)                       /* If no previous setting */
                    linespace = point_size + 1; /* Set linespacing */
                if (point_size != DEF_POINTSIZE)
                    text_options = TRUE;
                break;

            case 'q':                           /* QuadV 4-Up output */
                if (option == PLAIN) {
                    option = QUADV;
                    i++;
                }
                else err_handler(OPT_COMB_ERR); /* Can't have QUAD + other */
                break;

            case 'Q':                           /* QuadH 4-Up output */
                if (option == PLAIN) {
                    option = QUADH;
                    i++;
                }
                else err_handler(OPT_COMB_ERR); /* Can't have QUAD + other */
                break;

            case 's':
                i++;
                if (i >= argc)
                    err_handler(NO_DATA);
                linespace = atoi(argv[i++]);
                if ((linespace < 1) || (linespace > MAX_POINT_SIZE))
                    err_handler(LINE_SPACE_ERR);
                if (linespace != (point_size + 1))
                    text_options = TRUE;
                iss = TRUE;             /* Indicate that l has been set */
                break;

            case 't':                   /* User-settable tab stop widths */
                if (++i >= argc)
                    err_handler(NO_DATA);
                tabspace = atoi(argv[i++]);
                if (tabspace < 0 || tabspace > MAX_TABSPACE)
                    err_handler(TABSPACE_ERR);
                text_options = TRUE;
                break;

            case 'v':                                  /* VERBATIM scheme */
                if (scheme == NONE) {
                    if (option == COUNT)
                        err_handler(OPT_SCHEME_ERR);
                    else {
                        scheme = PAGED;
                        i = pnums_parse(argc, argv, ++i);
                    }
                }
                else err_handler(MULT_SCHEMES);
                break;

	      case 'u':                              /* UNIQUE scheme */
		if (scheme == NONE) {
		    if (option == COUNT)
		      err_handler(OPT_SCHEME_ERR);
		    else {
			scheme = UNIQUE;
			i = pnums_parse(argc, argv, ++i);
		    }
		}
		else err_handler(MULT_SCHEMES);
		break;

	      case 'w':
		if (option == PLAIN) {
		    option = QUADLAND;
		    i++;
		}
		else err_handler(OPT_COMB_ERR);
                break;		

	      default: err_handler(BAD_SWITCH);       /* Illegal options */
                          break;
        }
    }

    if (linespace != DEF_LINESPACE)               /* If not default lnspc */
        maxlines = lines_from_space(linespace);   /* Set lines per page */

    if (i < argc)               /* Another arg? - Must be source name */
        source = argv[i++];
    else source = NULL;

    if (i < argc && option != COUNT) {  /* Still another? - Dest name */
        dest = argv[i];                 /* ...it must be output filename */
        if ((spot = strchr(dest, '.')) != NULL) {
            if (strSTR(spot, ".PS") != spot || (spot[3] != NUL
                       && spot[3] != '.')) {
                strcpy(dbuf, dest);
                spot = strchr(dbuf, '.');
                strcpy(spot, ".ps");
                dest = dbuf;
                dchanged = TRUE;        /* .ps extension was substituted */
            }
        }
        else {
            strcpy(dbuf, dest);
            strcat(dbuf, ".ps");
            dest = dbuf;
            dchanged = TRUE;            /* .ps extension was concatenated */
        }
    }
    else dest = NULL;             /* Just COUNT'ing, so no dest filename */
}

/****************************************************************************
  PNUMS_PARSE

               Arguments:  ARGC, ARGV - The familiar command line argument
                                        variables handed down through the
                                        various scopes to here.  

                           N - Index into ARGV indicating place to start
                               looking for the page numbers from.

                 Returns:  the ARGV index (of last page number) + 1.

            Side Effects:  Sets global variable REQUESTS to the number
                           of page requests entered into PAGE_LIST.

        If the SCHEME is PAGED, PNUMS_PARSE assembles the specified pages
        in a simple order, exactly in the order input.  In this case,
        the first N locations in the PAGE_LIST array will contain the page
        numbers in exactly the order specified.  The global variable REQUESTS
        will be set to the number of requests in the queue.

        If the SCHEME is UNIQUE, PNUMS_PARSE decodes the numbers and marks
        the corresponding page numbers index in the array as 'TRUE' (should
        be printed).  For example, if page 44 is specified for printing,
        PAGE_LIST[44] = TRUE.  In this scheme, all pages are printed in
        ascending order, and can only be printed once.
 *****************************************************************************/

int pnums_parse(argc, argv, n)
int argc;
char *argv[];
int n;
{
    int arg;                /* Index into argv array */
    int vindex;             /* Index into individual argument's characters */
    int tindex;             /* Index into temp */

    char temp[10];          /* String for assembling page numbers */
    int page1, page2;  /* Value holders for translated page numbers (ranges) */
    int count = 0;         /* Index to end of request queue for PAGED scheme */

    arg = n;           /* Get ARGV index of first argument to look at */

    while ((arg < argc) && (isdigit(argv[arg][vindex = 0]))) {
        tindex = 0;         /* Build page number (or first part of range) */
        do {
            temp[tindex++] = argv[arg][vindex++];
        } while (isdigit(argv[arg][vindex]));

    /* Gross kludge to break out of outer while loop, and then do stuff
       at the end of the routine before exitting.  Couldn't break from
       inside switch, sigh. */

        if (!isdigit(argv[arg][vindex]) && argv[arg][vindex] != NUL
                && argv[arg][vindex] != ':')
            break;

        switch (argv[arg][vindex]) {   /* Find out why number building ended */

            case NUL:                        /* End of the argument */
                   temp[vindex] = NUL;       /* Terminate built string */
                   page1 = atoi(temp);       /* Convert to integer */

                   if (page1 < 1 || page1 >= MAX_PAGES)  /* Check validity */
                       err_handler(BAD_PAGE_NUM);

     /* Insert requested page into request list.  */

                    switch (scheme) {
                        case PAGED:
                            page_list[count++] = page1;
                            break;

                        case UNIQUE:
                            page_list[page1] = TRUE;  /* Mark page */
                            count++;
                            break;
                    }
                    break;

         case ':':                       /* Page range delimiter */
                   temp[tindex] = NUL;       /* Terminate built string */
                   page1 = atoi(temp);       /* Convert start to integer */

                   if (page1 < 1 || page1 >= MAX_PAGES)  /* Check validity */
                       err_handler(BAD_RANGE);

                   vindex++;                            /* Skip over ':' */
                   tindex = 0;                          /* Build range end */
                   while (isdigit(argv[arg][vindex]))
                       temp[tindex++] = argv[arg][vindex++];

                   if (argv[arg][vindex] != NUL)    /* Check for bad chars */
                       err_handler(BAD_PAGE_NUM);

                   page2 = atoi(temp);              /* Convert end */

                   if ((page2 < 1) || (page2 >= MAX_PAGES) /* Check range */
                                   || (page2 < page1))
                       err_handler(BAD_RANGE);

                   switch (scheme) {
                        case PAGED:                           /* Add request */
                                    while (page1 <= page2)
                                        page_list[count++] = page1++;
                                    break;

                        case UNIQUE:                         /* Mark request */
                                    while (page1 <= page2) {
                                        page_list[page1++] = TRUE;
                                        count++;
                                    }
                                    break;
                     }
                  break;

         default: err_handler(BAD_SPEC);    /* All other chars illegal */
                  break;
          }                    /* end switch */

       arg++;            /* Do next argument (check for number & process) */
    }                    /* end of while */

    if (arg == n)              /* Check for absense of page requests */
       err_handler(NO_PAGE);

    requests = count;          /* Set number of requests */

    return (arg);  /* Return index of first argument that was not a page num */
}

/*********************************************
  ID_SOURCE() - Partially identify source type
 *********************************************/

int id_source()
{
    char buf[100];

    if ((first_char = pg_get_chr()) == EOF)
        return (BADFILE);
    else {
        if (first_char == '%' && (first_char = pg_get_chr()) == '!') {
            if (pg_get_line(buf, 100) != NULL) {
                if (strstr(buf, ADBE))
                    return (ADOBE);
                else return (UNKNOWNPS);
            }
            else return (BADFILE);
        }
        else {
            switch (first_char) {
                case NEWLINE:
                    pg_end_line();
                    break;
                case FF:
                    pg_end_line();
                    pg_end_page();
                    break;
                default:
                    break;      /* Do nothing for EOF and others */
            }
            return (TEXTFILE);
        }
    }
}

/*****************************************************************************

      These functions process ASCII text files.  Lines of text may be
      of any length, and are assumed to be terminated by the system
      dependent NEWLINE character, or either FF or the end of the
      file.

      The text is broken into pages by three possibilities.  FF
      characters in the text clearly define page boundries.  If the
      current page contains global var maxlines (default 60) lines
      and neither a FF nor EOF have been encountered, than a page
      boundry has been found.  The current page is also ended when EOF
      is encountered.

      Reversal of pages is accomplished with extreme ease once the ends
      of the pages have been marked with calls to pg_end_page().  It is a
      simple matter of copying all pages (or only those requested) in the
      reverse order that they were found in.  The global var scheme determines
      what type of reversal is to take place.

      get_text() and reverse_plain() do not tamper with the files'
      formatting, in that the actual NEWLINEs and FFs in the source file
      are copied to the destination file to format the text in exactly the
      same manner.  This means that any user errors are preserved exactly the
      way they were made, only the pages will be reversed.  Bad control codes,
      malformed or misplaced NEWLINEs or FFs will be passed through to
      the output file and cause whatever errors they might cause in an unREV'd
      file.

      There is one exception, though.  When the last page of the source
      file contains less than maxlines lines, and does not terminate with a FF
      reverse_plain() must place a FF immediately after this page to prevent
      the entire destination file from getting out of synch with regards to
      page ends.  Not placing the FF causes the next page to be printed
      (next to last page in a full reversal) to start immediately after the
      last page.  This will happen somewhere still on the last page,
      and from there on after, none of the measured page boundries will
      fall correctly on the actual pages.

      If get_text() finds the above case to be true, it returns TRUE
      indicating that the last page will need a FF.  This fact is passed on
      to reverse_plain() so that it will output the needed FF.
***************************************************************************/


/*************************************************************************
 DO_TEXT()

    Arguments:  Global - option
      Returns:  Number of pages copied to destination if option != COUNT,
                 or the number of pages in the source file option == COUNT.
***************************************************************************/

int do_text()
{
    int outp;
    char sprintbuf[MAXFONTNAME + 20];

    if (option != PLAIN && option != COUNT && option != BOOK &&
	option != LAND && option != QUADV && option != QUADH)
      err_handler(OPT_NOT_ALLOWED);

    fputs("Reading  [", stderr);          /* Open  "Reading  [...]" display */
    get_text();                           /* Read */
    fputs("]\n", stderr);                 /* Close "Reading  [...]" display */

    if (option == COUNT)
        return (pg_page_count());

    if (source)
        tp_head(source, font);
    else tp_head("standard input", font);

    pg_insert_file(TEXT_PRELUDE);
    pg_put_string("%%EndProlog\n");

    pg_put_string("%%BeginSetup\n");
    sprintf(sprintbuf, "%d /%s SF\n", point_size, font);
    pg_put_string(sprintbuf);
    sprintf(sprintbuf, "%d SL\n", linespace);
    pg_put_string(sprintbuf);

    switch (option) {
        case BOOK:
            pg_put_string("MDB\n");
            break;

        case LAND:
            pg_put_string("MDL\n");
            break;

        case PLAIN:
            pg_put_string("MDP\n");
            break;

        case QUADH:
            pg_put_string("MDQH\n");
            break;

        case QUADV:
            pg_put_string("MDQV\n");
            break;
    }

    pg_put_string("%%EndSetup\n");
    fputs("Writing [", stderr);
    if (!pg_playback())
        err_handler(NO_PLAYBACK);

    switch (option) {
        case BOOK:
            fputc('b', stderr);
            outp = reverse_book();
            break;

        case LAND:
            fputc('l', stderr);
            outp = reverse_plain();  /* Reverse same as PLAIN */
            break;

        case PLAIN:
            fputc('P', stderr);            /* Indicate prepend insertion */
            outp = reverse_plain();        /* Reverse */
            break;

        case QUADH:
            fputc('Q', stderr);
            outp = reverse_quad();
            break;

        case QUADV:
            fputc('q', stderr);
            outp = reverse_quad();
            break;
    }
    fputs("]\n", stderr);          /* Close "Writing  [...]" display */

    pg_put_string("%%Trailer\n");
    sprintf(sprintbuf, "%%%%Pages: %d\n", outp);
    pg_put_string(sprintbuf);

    return(outp);
}

/****************************************************************************
  GET_TEXT() - Completely scan input text stream.
 ****************************************************************************/

void get_text()
{
    int c;
    int line;
    int ccount = 0;

    switch (first_char) {
        case NEWLINE:
            line = 2;
            break;
        case FF:
            line = 1;
            break;
        default:
            line = 1;
            break;
    }

    while ((c = pg_get_chr()) != EOF) {
        ccount++;
        switch (c) {
            case NEWLINE:
                pg_end_line();
                line++;
                ccount = 0;
                if (line > maxlines) {
                    pg_end_page();
                    line = 1;
                    fputc('.', stderr);
                }
                break;

            case FF:
                pg_end_line();
                pg_end_page();
                line = 1;
                ccount = 0;
                fputc('.', stderr);
                break;

            default:
                break;
        }
    }
    if (ccount > 0)
        pg_end_line();
    pg_end_page();
    fputc('.', stderr);
}

/**************************************************************************
 REVERSE_PLAIN()

         Returns:  Number of pages copied to output.

    Globals option, scheme and requests[] are also parameters.
 **************************************************************************/

int reverse_plain()
{
    int outpages = 1;              /* Dest file page counter */
    int i;                         /* Misc counter */
    int inpages;

    inpages = pg_page_count();

    switch (scheme) {                     /* Copy all pages in reverse order */
         case NONE:
            for (i = inpages - 1; i >= 0; i--) {
                tp_start_page(outpages);
                tp_encap_panel(i);       /* Do remaining pages */
                fputc('.', stderr);      /* Indicate copy on TTY */
                outpages++;              /* ...and increment count */
            }
            break;

         case PAGED:
            for (i = requests - 1; i >= 0; i--) {
                tp_start_page(outpages);
                tp_encap_panel(page_list[i] - 1);
                fputc('.', stderr);   /* Indicate copy on TTY */
                outpages++;           /* ...and increment count */
            }
            break;

         case UNIQUE:
            for (i = inpages; i >= 1; i--)      /* Search through list */
                if (page_list[i] == TRUE) {     /* If page i requested */
                    tp_start_page(outpages);
                    tp_encap_panel(i - 1);      /* Copy logical page i */
                    fputc('.', stderr);         /* Indicate on TTY */
                    outpages++;                 /* ...and increment count */
                }
            break;
     }

   return (outpages - 1);       /* Return number of pages copied to output */
}

/************************************************************************
 REVERSE_BOOK()  -  Reverse Textfile for BOOK ordering

       Returns:  The number of panels (logical pages) copied to output.
         Calls:  is_even() and search_pagelist()
          Uses:  globals scheme and requests.

     REVERSE_BOOK() is a variant of REVERSE_PLAIN() that provides the
     necessary logic to reverse pages in such an order that they will print
     correctly when printed in "/BOOK" style.

     /BOOK prints text landscaped and condensed, so that two pages
     fit on the page in a form similar to how a book looks when you
     open it and see a page on either side of the binding.

     The general idea is to start by outputting the next-to-last page
     first, followed by the last page.  When printed using the BOOK
     prepend, this will result in a page with the next-to-last page on
     the left side, and the last page on the right.

     However, if there are an odd number of pages to be output, the last
     page must be output first, followed by a blank page to blank the
     right side panel.  That is necessary to keep the output in synch,
     lest the first page to be output appear on the wrong side of the
     first /BOOK page.
 ************************************************************************/

int reverse_book()
{
    int outpanels = 0;             /* Dest log. page (panel) counter */
    int outpages = 1;              /* True destination page counter */
    int inpages;
    int i;                         /* Misc counter */
    int req, right;                /* Page request holders */
    int rem;                       /* Used to determine odd/even count */
    int entries = 0;               /* Counter for # of entries is page_list */

    inpages = pg_page_count();

    switch (scheme) {              /* Copy all pages in reverse order */
        case NONE:
            rem = inpages % 2;
            if (rem > 0) {              /* Odd number of panels */
                tp_start_page(outpages);
                tp_encap_panel(inpages - 1);
                fputc('.', stderr);
                outpanels++;

                tp_cut_page();
                outpages++;
            }

            for (i = ((inpages - rem) / 2); i > 0; i--) {
                req = 2 * (i - 1);
                tp_start_page(outpages);
                tp_encap_panel(req);    /* Copy left-side page */
                fputc('.', stderr);     /* Indicate copy on TTY */
                outpanels++;

                req = (2 * i) - 1;      /* Copy right-side page */
                tp_encap_panel(req);   
                fputc('.', stderr);
                outpanels++;
                outpages++;
            }
            break;
        
        case PAGED:
            rem = requests % 2;
            if (rem > 1) {
                tp_start_page(outpages);
                req = requests - 1;
                tp_encap_panel(page_list[req] - 1);
                fputc('.', stderr);
                outpanels++;
                tp_cut_page();
                outpages++;
            }

            for (i = ((requests - rem) / 2); i > 0; i--) {
                tp_start_page(outpages);
                req = (i - 1) * 2;                      /* Left side */
                tp_encap_panel(page_list[req] - 1);
                fputc('.', stderr);
                outpanels++;

                req = (i * 2) - 1;                      /* Right side */
                tp_encap_panel(page_list[req] - 1);
                fputc('.', stderr);
                outpanels++;
                outpages++;
            }
            break;

        case UNIQUE:
            for (i = 0; i < MAX_PAGES; i++)
                if (page_list[i] == TRUE)      /* Count requests */
                    entries++;

            rem = entries % 2;
            if (rem > 0) {
                tp_start_page(outpages);
                req = search_pagelist();       /* Get last req'ed page */
                tp_encap_panel(req - 1);
                entries--;                 /* Decrement # of entries */
                fputc('.', stderr);
                outpanels++;
                tp_cut_page();
                outpages++;
            }

            for (i = (entries - rem) / 2; i > 0; i--) {
                right = search_pagelist();      /* Right side */
                req = search_pagelist();        /* Left side */

                tp_start_page(outpages);
                tp_encap_panel(req - 1);         /* Output left page */
                fputc('.', stderr);
                outpanels++;

                tp_encap_panel(right - 1);       /* Output right page */
                fputc('.', stderr);
                outpanels++;
                outpages++;
            }
            break;
    }

    return (outpanels);           /* Return number of pages copied to output */
}

/*************************************************************************
 REVERSE_QUAD() - Reverse textfile for QUAD ordering

      Returns:  The number of logical pages (panels) copied to output.

    Reverses textfile previously scanned with get_text(), formatting
    the pages for 4-Up Quad output.  Both page selection strategies are
    implemented.

    The method for quad reversal is much the same as book, but twice as
    much fun!  The first step is to find out how many pages are to be
    output.  This number is mod'ed by 4 to figure out how many logical pages
    will be on the last physical page.  These logical pages are output,
    and the rest of the page is blanked.

    Once these "problem" pages are handled, and if there were more than
    4 logical pages to output, the rest are handled easily by just Writing
    them in the appropriate order.  This order means groups of 4's, like
    5,6,7, 1,2,3,4.

    However, with either of the two pageing schemes, certain gyrations are
    needed to get the page numbers, but the idea is the same.  PAGED means
    pglst[4], pglst[5], pglst[6],  pglst[0], pglst[1], pglst[2], pglst[3]
    so to speak using the previous example.

    UNIQUE requires fetching requested pages out of page_list[] in bunches
    of 4's (or less).  Since those can only be retrieved in decending order
    by search_pagelist(), they are loaded into the p[] array and used in
    the appropriate order.
 *************************************************************************/

int reverse_quad()
{
    int outpages  = 1;
    int outpanels = 0;
    int inpages;
    int i, j;
    int p[3];
    int rem;
    int req;
    int entries = 0;

    inpages = pg_page_count();

    switch (scheme) {
        case NONE:

            rem = inpages % 4;
            if (rem > 0) {
                tp_start_page(outpages);
                for (i = rem; i > 0; i--) {     /* Do remaining 1,2,or 3 */
                    tp_encap_panel(inpages - i);
                    fputc('.', stderr);
                    outpanels++;
                }

                tp_cut_page();
                outpages++;
            }

            for (i = (inpages - rem) / 4; i > 0; i--) { /* Do full pages */
                tp_start_page(outpages);
                for (j = 0; j < 4; j++) {
                    req = (4*(i-1)) + j;
                    tp_encap_panel(req);
                    fputc('.', stderr);
                    outpanels++;
                }
                outpages++;
            }
            break;

        case PAGED:
            rem = requests % 4;         /* Do last 1,2, or 3 panels */
            if (rem > 0) {
                tp_start_page(outpages);
                for (i = rem; i > 0; i--) {
                    req = page_list[requests - i];
                    tp_encap_panel(req - 1);
                    fputc('.', stderr);
                    outpanels++;
                }
                tp_cut_page();
                outpages++;
            }

            for (i = (requests - rem) / 4; i > 0; i--) {
                tp_start_page(outpages);
                for (j = 0; j < 4; j++) {
                    req = page_list[(4*(i-1)) + j];
                    tp_encap_panel(req - 1);
                    fputc('.', stderr);
                    outpanels++;
                }
                outpages++;
            }
            break;

        case UNIQUE:
            for (i = 0; i < MAX_PAGES; i++)
                if (page_list[i] != FALSE)
                    entries++;

            rem = entries % 4;
            if (rem > 0) {
                for (i = 0; i < rem; i++)
                    p[i] = search_pagelist();

                tp_start_page(outpages);
                for (i = rem - 1; i >= 0; i--) {
                    tp_encap_panel(p[i] - 1);
                    fputc('.', stderr);
                    outpanels++;
                }

                tp_cut_page();
                outpages++;
            }

            for (i = (entries - rem) / 4; i > 0; i--) {
                for (j = 0; j < 4; j++)
                    p[j] = search_pagelist();

                tp_start_page(outpages);
                for (j = 3; j >= 0; j--) {
                    tp_encap_panel(p[j] - 1);
                    fputc('.', stderr);
                    outpanels++;
                }
                outpages++;
            }
            break;
    }  /* end switch */

    return (outpanels);
}

/****************************************************************************
 DO_ADOBE() - Handle all types of Adobe files.

    Arguments:  Global Option to determine action to take, and what
                scribe reverser to call.
      Returns:  if Option == COUNT: returns number of body pages in source.
                else the number of body pages reversed in dest.

    This is the driving routine for reversing any sort of Postscript file
    which conforms to Adobe's document structuring conventions.
    Previous to calling do_adobe(), the global option and id vars must be
    correctly set, and the Adobe prolog (if present), must have been handled
    previously by get_prolog().
 ****************************************************************************/

int do_adobe()
{
    int outp;

    fputs("Reading  [", stderr);        /* Open in-progress display */
    (void) get_script();
    get_trailer();
    fputs("]\n", stderr);               /* Close "Reading  [...]" display */


    switch (option) {
        case COUNT:
            return (pg_page_count() - 2);  /* Just return # of body pages */
            break;

        case BOOK:
	case BOOKLAND:
            fputs("Writing [", stderr);    /* Open "Writing [...]" display */
            outp = reverse_adobe_book();   /* Reverse pages */
            fputs("]\n", stderr);          /* Close display */
            return (outp);                 /* Return # body pages rev */
            break;

        case PLAIN:
            fputs("Writing  [", stderr);
            outp = reverse_adobe_plain();
            fputs("]\n", stderr);
            return (outp);
            break;

        case QUADH:
        case QUADV:
	case QUADLAND:
            fputs("Writing [", stderr);
            outp = reverse_adobe_quad();
            fputs("]\n", stderr);
            return (outp);
            break;

	  default:
	    err_handler(OPT_NOT_ALLOWED);
	    break;
    }
}

/***************************************************************************
  GET_PROLOG - Read Adobe Header and Prolog section, check %%Creator:
               field for known formats.

     A Postscript Prolog is read in by this routine, or if not present,
     everything up to the first Page: label.  If a %%Creator label is
     found, it is compared with known types and the appropriate id code
     is returned after the "prolog" has been completely read.
 ***************************************************************************/

int get_prolog()
{
    char line_buf[MAX_PLINE];
    char *line, *pstr;
    int  p;
    int retval = ADOBE;

    do {
        line = pg_get_line(line_buf, MAX_PLINE);
    } while (line != NULL && strstr(line_buf, CREATOR) != line_buf &&
             strstr(line_buf, PGHEAD) != line_buf);

    if (line == NULL)
        return (BADFILE);

    if (strstr(line_buf, CREATOR) == line_buf) {
        p = 0;
        while ((pstr = ps_types[p].ps_str) != NULL && !strstr(line_buf, pstr))
            p++;
        if (pstr != NULL)
            retval = ps_types[p].ps_val;

        do {
            line = pg_get_line(line_buf, MAX_PLINE);
        } while (line != NULL && strstr(line_buf, PGHEAD) != line_buf);
        if (line == NULL)
            return (BADFILE);
    }

    pg_start_page();
    return (retval);
}

/*************************************************************************
  GET_SCRIPT() - Read an Adobe formatted script
        Allows skipping over inserted documents using the Adobe 1.0 convention
        of mandatory %%EndProlog and %%Trailer comments in all documents.
        Get_script marks pages as usual, but if %%EndProlog is found,
        all page labels are ignored until %%Trailer is encountered.

        Returns: bool value - whether or not a start-of-trailer label was
                              found.
 *************************************************************************/

bool get_script()
{
    char line_buf[MAX_PLINE];
    char *line;
    bool for_sure;

    do {        /* Document loop */

        for_sure = TRUE;                /* Finding Trailer means end */

        do {        /* Page loop */
            line = pg_get_line(line_buf, MAX_PLINE);
        } while (line != NULL && strstr(line_buf, PGHEAD) != line_buf &&
                 strstr(line_buf, ENDPROLOG) != line_buf &&
                 strstr(line_buf, TRAILER) != line_buf);

/* Check if ENDPROLOG was found.  If so, need to skip over inserted document.
   Set for_sure to FALSE, so that the document loop won't end yet. */

        if (strstr(line_buf, ENDPROLOG) == line_buf) {
            for_sure = FALSE;
            do {
                line = pg_get_line(line_buf, MAX_PLINE);
            } while (line != NULL && strstr(line_buf, TRAILER) != line_buf);
        }

/* It wasn't ENDPROLOG, so check and see if it was a %%Page: label */

        else
            if (strstr(line_buf, PGHEAD) == line_buf) {
                pg_start_page();
                fputc('.', stderr);
            }

    } while (line != NULL && !(strstr(line_buf, TRAILER) == line_buf &&
                             for_sure));

    fputc('.', stderr);
    if (line == NULL) {     /* EOF, no trailer - */
        pg_end_page();          /* Mark end */
        return (FALSE);         /* Indicate no trailer */
    }
    else {                  /* Trailer label was found */
        pg_start_page();        /* Mark start of trailer */
        return (TRUE);          /* And indicate it */
    }
}


/**************************************************************************
  GET_TRAILER() - Read in Adobe Trailer

      Very anticlimatic - If there's anything there, read until no more.
      %%Trailer label has already been found, or we've run off the end
      of the world.  Either way, call pg_end_page() when we're sure.
 **************************************************************************/

void get_trailer()
{
    char line_buf[MAX_PLINE];

    while (pg_get_line(line_buf, MAX_PLINE) != NULL)   /* Read to end */
        ;
    pg_end_page();                                     /* And mark it */
}

/****************************************************************************
 REVERSE_ADOBE_PLAIN()

       Returns:  The number of pages copied to the destination file.

   REVERSE_ADOBE() will reverse any PostScript file previously scanned
   which contains a Header, body pages, and a trailer.  The Header is copied
   first, then the body pages in reverse order, then the trailer last.
   No gyrations, no machinations... it doesn't get any easier than this!
 ****************************************************************************/

int reverse_adobe_plain()
{
    int outpages = 0;              /* Dest file page counter */
    int inpages;
    int i;                         /* misc counter */

    inpages = pg_page_count();

     /* Copy pages to output, with the header page still first,
        the trailer page still last, and the body pages in reverse order. */

    pg_copy_page(0);                       /* Copy Header page */

    switch (scheme) {                      /* Reverse body pages */
        case NONE:
                    for (i = inpages - 2; i >= 1; i--) {
                        pg_copy_page(i);
                        fputc('.', stderr);
                        outpages++;            /* Keep track of pages out */
                    }
                    break;

        case PAGED:
                    for (i = requests - 1; i >= 0; i--) {
                        pg_copy_page(page_list[i]);
                        fputc('.', stderr);
                        outpages++;
                    }
                    break;

       case UNIQUE:
                    for (i = inpages - 2; i >= 1; i--)
                        if (page_list[i] == TRUE) {
                            pg_copy_page(i);
                            fputc('.', stderr);
                            outpages++;
                        }
                        break;
    }

    pg_copy_page(inpages - 1);            /* Copy trailer page */
    return (outpages);                    /* Return number of pages copied */
}

/**************************************************************************
 REVERSE_ADOBE_BOOK()

    Returns:  the number of pages copied to destination.

    Reverses an Adobe-formatted Postscript file in the corrrect order for
    book-style output, inserting various PS hacks (SCRIBE only at present).

    The method here is basically the same as in reverse_book().  An odd
    number of pages to be output means that the right-side of the last
    page will have to blanked in order to keep the file in synch.

    The first step is to copy the prolog, performing the required hack
    to allow book-style output.  The pages are then copied as described
    above, and a special hack is ouput at the end of the file and
    when processing an "orphan" page.  Other hacks will be incorporated
    here soon, I hope.
 **************************************************************************/

int reverse_adobe_book()
{
    int outpages = 0;
    int inpages;
    int i;
    int req, right;
    int rem;
    int entries = 0;

    inpages = pg_page_count();

    switch (id) {               /* Prepare for others besides scribe */
        case SCRIBE:
            scribe_prelude(option); /* Copy/Hack the Prolog */
            break;
        default:
            break;
    }

    switch (scheme) {
        case NONE:
            rem = (inpages - 2) % 2;    /* Find out odd/even # pages */
            if (!is_even(rem)) {        /* Odd - do last, then blank rt */
                pg_copy_page(inpages - 2);
                switch (id) {
                    case SCRIBE:
                        pg_put_string("EF\n");
                        break;
                }
                fputc('.', stderr);
                outpages++;
            }

/* Handle even remaining even number of pages */

            for (i = ((inpages - 2) - rem) / 2; i > 0; i--) {
                pg_copy_page((2*i)-1);
                fputc('.', stderr);
                outpages++;
                pg_copy_page(2*i);
                fputc('.', stderr);
                outpages++;
            }
            break;

        case PAGED:
            rem = requests % 2;
            if (!is_even(rem)) {
                pg_copy_page(page_list[requests - 1]);
                switch (id) {
                    case SCRIBE:
                        pg_put_string("EF\n");
                        break;
                }
                fputc('.', stderr);
                outpages++;
            }
            for (i = (requests - rem) / 2; i > 0; i--) {
                pg_copy_page(page_list[2*(i-1)]);
                fputc('.', stderr);
                outpages++;
                pg_copy_page(page_list[(2*i)-1]);
                fputc('.', stderr);
                outpages++;
            }
            break;

        case UNIQUE:
            for (i = 0; i < MAX_PAGES; i++)
                if (page_list[i])
                    entries++;

            rem = entries % 2;
            if (!is_even(rem)) {
                req = search_pagelist();
                pg_copy_page(req);
                switch (id) {
                    case SCRIBE:
                        pg_put_string("EF\n");
                        break;
                }
                fputc('.', stderr);
                outpages++;
            }
            for (i = (entries - rem) / 2; i > 0; i--) {
                right = search_pagelist();
                req = search_pagelist();
                pg_copy_page(req);
                fputc('.', stderr);
                outpages++;
                pg_copy_page(right);
                fputc('.', stderr);
                outpages++;
            }
            break;
    }

    pg_copy_page(inpages - 1);      /* Copy the Trailer page */
    return (outpages);              /* Return number of output pages */
}

/************************************************************************ 
  REVERSE_ADOBE_QUAD() - Postscript...in quads!

    Returns: number of pages output

    This procedure works much like reverse_quad() does for text.  The only
    differences are that first the Postscript Prolog must be output
    and hacked to allow the quad style output.  Currently only a SCRIBE
    hack is available.  Of course the Trailer is always output last.

    The pages are bunched in groups of 4's, and special treatment is
    in order for the last physical page which has fewer than 4 logical
    pages on it.
 ************************************************************************/

int reverse_adobe_quad()
{
    int outpages = 0;
    int inpages;
    int i, j;
    int rem;
    int entries = 0;
    int p[4];

    inpages = pg_page_count();

    switch (id) {
        case SCRIBE:
            scribe_prelude(option);
            break;
        default:
            break;
    }

    switch (scheme) {
        case NONE:
            rem = (inpages - 2) % 4;    /* Check for orphans */
            if (rem > 0) {
                for (i = rem; i > 0; i--) {
                    pg_copy_page(inpages - (i + 1));
                    fputc('.', stderr);
                    outpages++;
                }
                switch (id) {       /* Slip in hack to blank rt side */
                    case SCRIBE:
                        pg_put_string("EF\n");
                        break;
                }
            }

/* Any orphans have been handled, now any remaining bunches of 4's */

            for (i = ((inpages - 2) - rem) / 4; i > 0; i--)
                for (j = 1; j <= 4; j++) {
                    pg_copy_page(4*(i-1) + j);
                    fputc('.', stderr);
                    outpages++;
                }
            break;

        case PAGED:             /* Follows the same pattern as NONE */
            rem = requests % 4;
            if (rem > 0) {
                for (i = rem; i > 0; i--) {
                    pg_copy_page(page_list[requests - i]);
                    fputc('.', stderr);
                    outpages++;
                }
                switch (id) {       /* Slip in hack to blank rt side */
                    case SCRIBE:
                        pg_put_string("EF\n");
                        break;
                }
            }
            for (i = (requests - rem) / 4; i > 0; i--)
                for (j = 0; j < 4; j++) {
                    pg_copy_page(page_list[(4*(i-1)) + j]);
                    fputc('.', stderr);
                    outpages++;
                }
            break;

        case UNIQUE:                        /* Same pattern as NONE */
            for (i = 0; i < MAX_PAGES; i++)
                if (page_list[i])
                    entries++;

            rem = entries % 4;
            if (rem > 0) {
                for (i = rem - 1; i >= 0; i--)
                    p[i] = search_pagelist();
                for (i = 0; i < rem; i++) {
                    pg_copy_page(p[i]);
                    fputc('.', stderr);
                    outpages++;
                }
                switch (id) {       /* Slip in hack to blank rt side */
                    case SCRIBE:
                        pg_put_string("EF\n");
                        break;
                }
            }
            for (i = (entries - rem) / 4; i > 0; i--) {
                for (j = 3; j >= 0; j--)
                    p[j] = search_pagelist();
                for (j = 0; j < 4; j++) {
                    pg_copy_page(p[j]);
                    fputc('.', stderr);
                    outpages++;
                }
            }
            break;
    }

    pg_copy_page(inpages - 1);  /* Copy Trailer */
    return (outpages);          /* Return number of pages output */
}

/*******************************************************************
 SCRIBE_PRELUDE() - Copy Scribe's Adobe header and prolog & hack it!

    Arguments: hack - the hack to use (really just option echoed.
      Returns: nothing.
 *******************************************************************/

void scribe_prelude(hack)
int hack;
{
    char linebuf[MAX_PLINE];
    
    if (!pg_playback())      /* Set playback mode, rewind to first page */
        err_handler(NO_PLAYBACK);

    (void) pg_get_line(linebuf, MAX_PLINE);

    while (strSTR(linebuf, SCRB_BPAGE) != linebuf) {
        pg_put_string(linebuf);
        (void) pg_get_line(linebuf, MAX_PLINE);
    }
    switch (hack) {
        case BOOK:  pg_insert_file(BOOK_SCRIBE);
                    fputc('b', stderr);
                    break;

	case BOOKLAND:
	  pg_insert_file(BOOKLAND_SCRIBE);
	  fputc('B', stderr);
	  break;

        case QUADH:
	  pg_insert_file(QDH_SCRIBE);
	  fputc('Q', stderr);
	  break;

        case QUADV: pg_insert_file(QDV_SCRIBE);
                    fputc('q', stderr);
                    break;

	case QUADLAND:
	  pg_insert_file(QUADLAND_SCRIBE);
	  fputc('w', stderr);
	  break;

        default:  err_handler(PRELUDE_PARAM);
                  break;
    }
    (void) pg_get_line(linebuf, MAX_PLINE);
    while (strSTR(linebuf, SCRB_EPAGE) != linebuf) {
        pg_put_string(linebuf);
        (void) pg_get_line(linebuf, MAX_PLINE);
    }
    (void) pg_get_line(linebuf, MAX_PLINE);
    while (strSTR(linebuf, ENDPROLOG) != linebuf) {
        pg_put_string(linebuf);
        (void) pg_get_line(linebuf, MAX_PLINE);
    }
}

/****************************************************************************
  IS_EVEN - Test argument to find if it is even

             Arguments:  VALUE - an integer value.
               Returns:  TRUE or FALSE.

         IS_EVEN tests the given argument to determine if it is even.
         This is accomplished by evaluating (VALUE mod 2), and returning
         TRUE if the result is 0.  Otherwise FALSE is returned.
 ****************************************************************************/

bool is_even(value)
int value;
{
    if (value % 2 == 0)
        return (TRUE);
    else return (FALSE);
}



/****************************************************************************
  SEARCH_PAGELIST - Search PAGE_LIST[] downward to find next UNIQUE entry.

          Arguments:  None.
            Returns:  Index of next found entry, else if no entry found,
                         returns -1.
          Allocates:  INDEX as a static variable to keep track of where
                        search left off.

      SEARCH_PAGELIST() initializes INDEX on its initial call, and
      subsequently uses it on future calls as the placemarker from
      where to continue the search from.  If the entire PAGE_LIST[]
      array has been searched, but no UNIQUE-scheme TRUE entry found,
      a -1 value is returned (since that is not a valid array index).
      If an entry was found, the index into PAGE_LIST[] is returned.
 ***************************************************************************/

int search_pagelist()
{
    static int index = (MAX_PAGES - 1);
    int result;

    while ((index >= 0) && (page_list[index] == FALSE))   /* Search array */
        index--;

    if (index < 0)             /* If have searched entire array unsucessfuly */
        result = -1;           /* ...return -1 */
    else {
        result = index;        /* Otherwise: return index of entry */ 
        index--;               /* ...and skip over entry */
    }
    return (result);
}

/***************************************************************************
 ERR_HANDLER -  Main program Error handler

                Arguments:  error code.
                  Returns:  nothing.

        Prints indicated error message to stderr, and if necessary,
        prepares process for termination and then exits.
**************************************************************************/

void err_handler(code)
int code;
{
   switch (code) {
        case BAD_SWITCH:
            fprintf(stderr, "?Unknown switch\n");
            break;

        case BAD_PAGE_NUM:
            fprintf(stderr, "?Bad page number\n");
            break;

        case BAD_SPEC:
            fprintf(stderr, "?Bad page number or page range delimiter\n");
            break;

        case BAD_RANGE:
            fprintf(stderr, "?Illegal page range\n");
            break;

        case NO_DATA:
            fprintf(stderr, "?Switch requires parameters\n");
            break;

        case NO_PAGE:
            fprintf(stderr, "?No pages specified\n");
            break;

        case MULT_MODES:
            fprintf(stderr, "?Too many processing modes specified\n");
            break;

        case MULT_SCHEMES:
            fprintf(stderr, "?Too many page schemes specified\n");
            break;

        case OPT_COMB_ERR:
            fprintf(stderr, "?Option combination not allowed");
            break;

        case OPT_FILE_ERR:
            fprintf(stderr, "?Option not allowed with this file type\n");
            pg_close();
            break;

        case OPT_SCHEME_ERR:
            fprintf(stderr, "?Cannot have page schemes with this option\n");
            break;

        case FILE_BAD:
            fprintf(stderr, "?Unreadable Source file\n");
            perror(source);
            pg_close();
            break;

        case UNKNOWN_PS_SOURCE:
            fprintf(stderr, "?Source file is an unknown Postscript format\n");
            pg_close();
            break;

        case FONT_ERR:
            fprintf(stderr, "?Unknown font requested\n");
            break;

        case POINT_SIZE_ERR:
             fprintf(stderr, "?Bad font pointsize\n");
             break;

        case LINE_SPACE_ERR:
            fprintf(stderr, "?Bad linespacing value\n");
            break;

        case TABSPACE_ERR:
             fprintf(stderr, "?Bad tabspace parameter\n");
             break;
        
        case PRELUDE_PARAM:
             fprintf(stderr, "?Scribe_prelude: Unknown param\n");
             break;

        case NO_PLAYBACK:
             fprintf(stderr, "?Pg_playback() failed\n");
             pg_close();
             break;

	case OPT_NOT_ALLOWED:
	    fprintf(stderr, "?Option not allowed for this input type\n");
	    break;
    }
    exit(1);
}

/****************************************************************************
  LINES_PER_PAGE() - Return number of lines/page given fontsize & linespace
 ****************************************************************************/

int lines_per_page(fntsz, lnspc)
int fntsz;
int lnspc;
{
    int count = 0;
    int end;

    end = DEF_FORMLENGTH * 72;

    if (fntsz < end) {
        count = 1;
        while (((count * (fntsz + lnspc)) + fntsz) < end)
            count++;
    }
    return (count);
}


/**************************************************************
 VERIFY_FONT()

    Arguments: FNT - font string to compare regardless of case
      Returns: pointer to found font in FONT_LIST[],
                else returns NULL.
 ***************************************************************/

char *verify_font(fnt)
char *fnt;
{
    int findex = 0;

    while (*font_list[findex] != NUL && strCMP(fnt, font_list[findex]))
        findex++;

    if (*font_list[findex] == NUL)
        return (NULL);
    else return (font_list[findex]);
}

/*******************************************************
 LINES_FROM_SPACE()

     Arguments: SPC - selected linespacing in points
       Returns: Number of lines per page that will fit.
 ********************************************************/

int lines_from_space(spc)
int spc;
{
    int home = DEF_FORMLENGTH * 72;
    int count = 0;

    while (home > 0) {
        count ++;
        home -= spc;
    }
    return (count);
}
