/***************************************************************************

 LDPLOT.C
   
   Todd Koumrian
   SRI International
   26-Nov-1988

   Written: Sep-Oct 1988
      Uses: COMND.REL (must be linked with - see LDPLOT.CMD)

   This program produces graphs of the Average 5 minute load average on
   the SRI-NIC TOPS-20 machine.  The original source of the 5 Minute load
   average is the SYSTAT table.  The background process, LOADED, wakes
   up every 5 minutes and samples the TOPS-20 SYSTAT table's 5 Min. load
   average number.  Then every half hour, it averages the 6 LA's, and
   stores the number in the current day's record in the file
   <OPERATOR>LOADED.DAT.

   The actual format of the datafile is pretty gross.  Each day's record
   consists of a 36-bit word containing the internal format day in the
   the RH, and with the LH (the time) stupidly zero'ed.  Following that
   are 8 36-bit words, each containing 6 6-bit integers representing
   int(round(load_average * 2)).  The Load Average was multiplied by 2
   before it was converted to an integer, in order to give 0.5 graph
   resolution using only integers.  The maximum 6-bit value of 63 (or -1)
   is used to indicate no info available for that time-slice (system was
   down).  Maximum representable LA is 62/2 = 31.0

   LDPLOT is a C program making extensive use of TOPS-20 JSYS's and
   user interface, which is very appropriate for this never-to-be-ported
   program.  The COMND C interface and JSYS was used to give the user
   the full TOPS-20 command-line parsing interface.  Recognition, fill-out,
   ^U, ^W, and ^H-redo-line all work.

   For more usage sort of details, see the file HLP:LDPLOT.HLP.
 ****************************************************************************/

#include <stdio.h>              /* Usual stuff */
#include <nic/comnd.h>          /* TOPS-20 COMND JSYS Command parsing */
#include <signal.h>             /* Signals for suspend/continue */
#include <jsys.h>               /* JSYS's and defines */
#include <string.h>             /* String functions */
#include <stdlib.h>             /* malloc() */
#include <time.h>		/* Time functions */
#include <sys/time.h>           /* Machine dependent time TAD stuff */

#define CM_IDA monsym("CM%IDA") /* define missing from COMND and JSYS */
#define IC_UTZ 1<<33            /* ODCNV IC%UTZ bit: B2 */
#define MAX(a,b) (a>b ? (a) : (b))  /* Handy macros */
#define MIN(a,b) (a<b ? (a) : (b))

#define char6 _KCCtype_char6    /* Tricky pointer type for 6-bit bytes */
#define MAX_DATA_VALUE 63       /* Maximum value represented in 6 bits */
#define RES_FACTOR 2            /* 6 bit ints are mult by 2 for 0.5 res */

#define CSB cm_state_block              /* Shorthand for object */
#define CTD cm_data.cmR_table_data      /* Shorthand for keyword */
#define CTE cm_table_entry              /* Shorthand for macro */
#define NOISE(s) cm_noise(cm_str(s))    /* Noise words macro */

#define DEF_SOURCE "PS:<OPERATOR>LOADED.DAT"    /* Default data file */
#define PS_PLOT    "PS:<SYSTEM>LDPLOT-DRIVER.PS"/* PS Plot program */
#define HELP_FILE  "HLP:LDPLOT.HLP"             /* Help file */
#define MAX_FILENAME 39*4                       /* Max filename length */
#define LINE_LENGTH   100                       /* Max help file line */
#define RECORD_SIZE     9                       /* LOADED is 9 word entry */
#define DATE_LEN       25                       /* Length of Date strings */

#define MAX_PLOTS      20               /* Max # of MULTI PS plots */
#define PLOT_LINE_LEN 320               /* MULTI PS plot string length */
#define NO_LEGENDS      0               /* Value to use to mean no legends */

#define MAX_HEIGHT 20                   /* Max LA plot height */
#define TEXTWIDTH  80                   /* Width of Text plots */
#define HRES       48                   /* Horz data points */
#define LMARGIN    10                   /* Text graph Left margin */

#define FALSE 0
#define TRUE  1
typedef int bool;

/* ----------------------------
   Enumerations for FDB's and vars
   ---------------------------- */

typedef enum {                              /* Top-level commands */
    CMD_Exit, CMD_Get, CMD_Help, CMD_Multi,
    CMD_Plot, CMD_Quit, CMD_Set, CMD_Status
 } com_types;

typedef enum {                              /* Plot types */
    PLT_Daily, PLT_Monthly, PLT_Single, PLT_Weekly
} plt_types;

typedef enum {                              /* Graph types */
    GPH_Postscript, GPH_Text
} gph_types;

typedef enum {                              /* SET keywords */
    SET_Grid, SET_Legend, SET_Maximum, SET_No, SET_Reduction, SET_Smoothing,
    SET_Weekends
} set_types;

#define BOO_No  FALSE   /* BOO keywords are the familiar boolean values */
#define BOO_Yes TRUE

typedef enum {                              /* MLT (MULTI) Terminators */
    MLT_Abort, MLT_Quit
} mlt_types;

/* ---------------------
   Global vars and flags
   --------------------- */

bool have_file = FALSE;     /* Data file loaded */
bool ps_available = FALSE;  /* PS plot program driver is available */
FILE *ps_prog;              /* Global ps grapher file handle */
char in_buf[MAX_FILENAME];  /* Data filename string buffer (for user) */
char *in_name;              /* Data filename string */

bool grid = TRUE;          /* PS graphs have grid lines */
bool smoothing = TRUE;      /* Text graph smoothing flag */
bool weekends = FALSE;      /* Weekends included flag - default is FALSE */
bool legend = TRUE;         /* Want Multi PS do_multiegends */
bool reduction = FALSE;     /* Full size PS graphs by default */

int  s_date, e_date;        /* Internal format starting & ending dates */
char s_buf[DATE_LEN];       /* Buffers for date strings of available dates */
char e_buf[DATE_LEN];

int  count = 0;             /* Counter of # of MULTI PS plots performed. */
struct {
    char *pstr;             /* Plot string - the numbers */
    char *rstr;             /* Range string - the english label */
} mplots[MAX_PLOTS];        /* Array of structs of MULTI PS plot strings */

int in_jfn = -1;            /* JFN for data file */
int tmp_jfn = -1;           /* Temp jfn, used in reparse cleanup */
int in_len;                 /* Length of input file */
gtjfn_block jgb;            /* COMND's GETJFN state block */
FILE *fout = NULL;          /* Output file stream */

int max_height = MAX_HEIGHT;    /* Set default Max graph-height */
char *side = "LOAD AVERAGE";    /* Text graph strings */
char *top =  "Average 5 Min. Load Average on SRI-NIC";
char *hbar[] = {
    "+---+---+---+---+---+---+---+---+---+---+---+---",
    "0   2   4   6   8  10  12  14  16  18  20  22",
    "",
    "                  TIME OF DAY",
    NULL
};

int ld_buf[RECORD_SIZE];    /* Single record data buffer */
int s_avail, e_avail;       /* Internal format begin and end dates in file */
struct {                    /* Averaging struct */
    double accum;               /* Accumulator array for averaging */
    int hits;                   /* Counter for number of valid data points */
    int sm_low;                 /* Col lower limit when smoothed */
    int sm_high;                /* Col upper limit when smoothed */
} ave_array[HRES];

/* ------------------------------------------------------------------------
   FDB's 
   ------------------------------------------------------------------------*/

/* Top-level command stuff */

cm_table_begin(_ldc0)
    CTE("Exit",   0, CMD_Exit)
    CTE("Get",    0, CMD_Get)
    CTE("Help",   0, CMD_Help)
    CTE("Multi",  0, CMD_Multi)
    CTE("Plot",   0, CMD_Plot)
    CTE("Quit",   0, CMD_Quit)
    CTE("Set",    0, CMD_Set)
    CTE("Status", 0, CMD_Status)
cm_table_end(cmd_table, _ldc0, _ldc1)
cm_def_fdb(command_fdb, _CMKEY, CM_HPP, &cmd_table, cm_str("A Command,"),
           NULL, NULL, NULL)

/* PLOT type stuff */

cm_table_begin(_ldp0)
    CTE("Daily",  0, PLT_Daily)
    CTE("Monthly", 0, PLT_Monthly)
    CTE("Single", 0, PLT_Single)
    CTE("Weekly", 0, PLT_Weekly)
cm_table_end(plt_table, _ldp0, _ldp1)
cm_def_fdb(plot_fdb, _CMKEY, CM_HPP, &plt_table, cm_str("A Plot type,"),
            NULL, NULL, NULL)

/* Graph type stuff - Text or PS */

cm_table_begin(_lga0)
    CTE("Postscript", 0, GPH_Postscript)
    CTE("Text",       0, GPH_Text)
cm_table_end(gpha_table, _lga0, _lga1)
cm_def_fdb(gpha_fdb, _CMKEY, CM_HPP | CM_DPP, &gpha_table,
           cm_str("An output format,"), cm_str("TEXT"), NULL, NULL)

cm_table_begin(_lgb0)
    CTE("Text", 0, GPH_Text)
cm_table_end(gphb_table, _lgb0, _lgb1)
cm_def_fdb(gphb_fdb, _CMKEY, CM_HPP | CM_DPP, &gphb_table, 
           cm_str("An output format,"), cm_str("Text"), NULL, NULL)

/* GET File stuff */

cm_def_fdb(in_fdb, _CMFIL, CM_HPP | CM_SDH, NULL,
        cm_str("Input data file or RETURN to load default"), NULL, NULL, NULL)

/* Plot to file stuff */

cm_def_fdb(out_fdb, _CMFIL, CM_HPP | CM_SDH, NULL, NULL, NULL, NULL, NULL)
      /* Put help in at runtime */

cm_def_fdb(cfm_fdb, _CMCFM, CM_HPP | CM_SDH, 0,
           cm_str("confirm for TTY:"), 0, 0, 0)

/* Date parsing stuff */

cm_def_fdb(date_fdb, _CMTAD, 0, CM_IDA, NULL, NULL, NULL, NULL)

cm_table_begin(_mmn0)
    CTE("April",     0,  3)
    CTE("August",    0,  7)
    CTE("December",  0, 11)
    CTE("February",  0,  1)
    CTE("January",   0,  0)
    CTE("July",      0,  6)
    CTE("June",      0,  5)
    CTE("March",     0,  2)
    CTE("May",       0,  4)
    CTE("November",  0, 10)
    CTE("October",   0,  9)
    CTE("September", 0,  8)
cm_table_end(mmn_tab, _mmn0, _mmn1)
cm_def_fdb(mmn_fdb, _CMKEY, 0, &mmn_tab, NULL, NULL, NULL, NULL)
cm_def_fdb(yr_fdb, _CMNUM, 0, 10, NULL, NULL, NULL, NULL)

/* SET stuff */

cm_table_begin(_lds0)
    CTE("Grid",      0, SET_Grid)
    CTE("Legends",   0, SET_Legend)
    CTE("Maximum",   0, SET_Maximum)
    CTE("No",        0, SET_No)
    CTE("Reduction",     0, SET_Reduction)
    CTE("Smoothing", 0, SET_Smoothing)
    CTE("Weekends",  0, SET_Weekends)
cm_table_end(set_table, _lds0, _lds1)
cm_def_fdb(set_fdb, _CMKEY, 0, &set_table, NULL, NULL, NULL, NULL)

cm_table_begin(_ldw0)
    CTE("Grid",        0, SET_Grid)
    CTE("Legends",     0, SET_Legend)
    CTE("Reduction",   0, SET_Reduction)
    CTE("Smoothing",   0, SET_Smoothing)
    CTE("Weekends",    0, SET_Weekends)
cm_table_end(no_table, _ldw0, _ldw1)
cm_def_fdb(no_fdb, _CMKEY, 0, &no_table, NULL, NULL, NULL, NULL)

cm_def_fdb(ht_fdb, _CMNUM, 0, 10, NULL, NULL, NULL, NULL)

/* Boolean yes/no question fdb */

cm_table_begin(_ldb0)
    CTE("No",  0, BOO_No)
    CTE("YES", 0, BOO_Yes)
cm_table_end(boo_table, _ldb0, _ldb1)

cm_def_fdb(boo_fdb, _CMKEY, CM_HPP | CM_DPP, &boo_table, 
           cm_str("A boolean value,"), cm_str("YES"), NULL, NULL)

/* MULTI Terminator stuff */

cm_table_begin(_ldm0)
    CTE("Abort", 0, MLT_Abort)
    CTE("Quit",  0, MLT_Quit)
cm_table_end(mlt_table, _ldm0, _ldm1)

cm_def_fdb(mlt_fdb, _CMKEY, CM_HPP, &mlt_table, cm_str("A terminator,"),
           NULL, NULL, NULL)

/* Da master list, see! */

cm_fdb *fdbs[] = {
    &command_fdb,
    &plot_fdb,
    &gpha_fdb,
    &gphb_fdb,
    &in_fdb,
    &out_fdb,
    &cfm_fdb,
    &date_fdb,
    &set_fdb,
    &no_fdb,
    &ht_fdb,
    &boo_fdb,
    &mlt_fdb,
    &mmn_fdb,
    &yr_fdb,
    NULL
};

/* -------------------------------------------------
   Function list (alphabetical order.  Yeah, right!)
   ------------------------------------------------- */

void initialization();      /* One-time startup initialization */
void do_get();              /* GET (data file) command */
void do_help();             /* HELP (me) command */
bool do_plot();             /* PLOT (average type) (format) command */
void do_set();              /* SET command */
void do_status();           /* STATUS (parameters) command */
int  my_parse();            /* Special version of cm_parse() */
void release_file();        /* Close and release file jfn */
int  tty_jfn();             /* Assign and return JFN for TTY: */
bool get_date();            /* Read in the a record date header */
bool read_record();         /* Read in rest of record (after get_date()) */
bool convert_date();        /* Convert internal format date to string */
void fatal();               /* Fatal error message typer */
void error();               /* Not so fatal error message typer */
bool get_file();            /* File info-getter, access-checker */
void show_file();           /* Show file status */
void show_weekends();       /* Show weekend status */
void show_plots();          /* Show plots available */
void show_grid();           /* Show PS grid-lines paramter */
void show_height();         /* Show max-height parameter */
void show_reduction();      /* Show current scale setting */
void show_smoothing();      /* Show text graph smoothing parameter */
void show_legend();         /* Show Multi PS legend param */
void init_average();        /* Clear accumulators */
void enter_record();        /* Add in one days worth of info */
void txt_plot();            /* Text graph plotter */
void ps_plot();             /* Postscript graph plotter */
int  date_loc();            /* Returns RIN filepos for given date's record */
bool plot_driver();         /* Set up to do the plot, either text or ps */
void cleanup();             /* Close and release files after reparse */
void calc_average();        /* Calculate average of current data */
void vmark();               /* For text graphs, does left side */
int  round();               /* Round a DOUBLE */
void calc_smoothing();      /* Calculate smoothed graph vert bars */
bool is_target();           /* When doing smoothe graphs, check for hit */
void center_print();        /* Print string centered (using TEXTWIDTH) */
void do_multi();            /* MULTI-PS mode workhorse */
void ps_header();           /* Writes an Adobe style header + prolog */
void ps_trailer();          /* Writes an Adobe style trailer (no big deal) */
void multi_plot();          /* Writes current plot numbers to a string */
bool ask_dates();           /* Query for date(s) of plot range */
void rid_plots();           /* Reclaim mallocated multi_plot() strings */

extern int  kill();
extern int  getpid();
extern char *getlogin();

/*************************************************************************
 MAIN() - Main driver

    Top-level command parsing is controlled from here.  Some commands are
    handled within the giant switch() statement, while most are relegated
    to being dealt with inside their own function.
 *************************************************************************/

void main (argc, argv)
int argc;
char *argv[];
{
    initialization();                   /* One-time initialization */
    show_file();                        /* Alert user to what's available */
    cm_init(fdbs, cm_str("LDPLOT>"));   /* Setup the prompt */
    for (;;) {
        cm_set_error_trap();        /* Errors will come to here */
        fflush(stdout);
        cm_start_parse();           /* Do it! */
        cm_set_reparse();           /* And so will reparses (^U) */

        cleanup();              /* Remove jfns still around after reparse */
        cm_parse(&command_fdb);     /* Get a command */

        switch (CTD) {              /* Handle the command */

            case CMD_Exit:          /* EXIT and QUIT are the same */
            case CMD_Quit:
                NOISE("program");
                cm_confirm();
                kill (getpid(), SIGTSTP);   /* Suspend w/option to continue */
                show_file();                /* On continue, show loaded file */
                break;

            case CMD_Get:           /* GET (data file) */
                do_get();
                break;

            case CMD_Help:          /* HELP (me) */
                do_help();
                break;

            case CMD_Multi:         /* MULTI (PS plots to file) */
                do_multi();
                cm_prompt_set(cm_str("LDPLOT>"));
                break;

            case CMD_Plot:          /* PLOT (average type) */
                (void) do_plot();
                break;

            case CMD_Set:           /* SET (program parameter) */
                do_set();
                break;

            case CMD_Status:        /* STATUS (of program) */
                do_status();
                break;
        }
    }
}

/* INITIALIZATION() - One time initial setup
 */

void initialization()
{
    int ablock[5];

    ablock[1] = GJ_SHT | GJ_OLD;            /* Get def datafile jfn */
    ablock[2] = (int) (DEF_SOURCE - 1);

    if (jsys(GTJFN, ablock)) {
        in_jfn = RH & ablock[1];            /* Save jfn */
        if (get_file(in_jfn))               /* Attempt to access file */
            have_file = TRUE;               /* Indicate success! */
        else have_file = FALSE;             /* Uh oh.. problem with file */
    }
    else have_file = FALSE;                 /* Couldn't find default file */

    if ((ps_prog = fopen(PS_PLOT, "r")) != NULL)    /* Look for grapher */
        ps_available = TRUE;                        /* Indicate status */
}

/*--------------------------------------------
  Top-Level Command procedures (DO_ functions)
  --------------------------------------------*/

/* DO_GET() - Get specified input datafile

    Do_get() uses my_parse() to parse the name of the input datafile,
    with PS:<OPERATOR>LOADED.DAT as the default.  My_parse() is used
    because cm_parse() is not setup to get and return the jfn of the
    selected file.  It returns a file-descriptor instead, which is
    no good for my purpose.

    tmp_jfn is used to prevent leaving jfns around if the user decides
    to reparse anywhere on the line (most programs don't do this).
    On confirmation, in_jfn is assigned and tmp_jfn is deassigned
    and get_file() "loads" the file.
 */

void do_get()
{
    NOISE("data file");
    cm_setup_gtjfn(&jgb, GJ_OLD, cm_str("PS"), cm_str("OPERATOR"),
                   cm_str("LOADED"), cm_str("DAT"));
    
    tmp_jfn = my_parse(&in_fdb);    /* Parse filename, save jfn */
    cm_confirm();

    if (in_jfn != -1)
        release_file(in_jfn);       /* Release any old in_jfn file */
    in_jfn = tmp_jfn;               /* Recall new jfn, make it current */
    tmp_jfn = -1;                   /* Clear reparse protection jfn */

    if (get_file(in_jfn)) {
        in_name = in_buf;
        have_file = TRUE;           /* Input file is selected */
    }
    else have_file = FALSE;         /* Or could not be properly accessed */

}

/* DO_HELP() - Print out contents of .HLP file.
 */

void do_help()
{
    FILE *hlp;
    char buf[LINE_LENGTH];
    char *l;

    NOISE("me");
    cm_confirm();
    if ((hlp = fopen(HELP_FILE, "r")) == NULL)
        error("Couldn't find help file");
    else {
        do {
            if ((l = fgets(buf, LINE_LENGTH, hlp)) != NULL)
                fputs(buf, stdout);
        } while (l != NULL);
        fclose(hlp);
    }
}

/* DO_MULTI() - Multiple-plot PS plot driver

    DO_MULTI() prompts for the output filespec of the destination for the
    subsequent Postscript graph.  This routine sets up a "MULTI" mode,
    by changing the prompt to "MULTI>" and setting the error and reparse
    addresses to within this routine.  These are reset to the main command
    parser after exitting.

    DO_MULTI() parses the plot_fdb right away, calling ask_dates() and
    plot_driver() to set up for a requested plot.  Plot_driver() will
    call multi_plot() instead of the usual ps_plot().  Two terminators
    are also offered, to allow the user to signal when they are through
    with MULTI mode.  The user is asked to confirm whether to output the
    resulting postscript file when they are done, if they correctly specified
    any plots.
 */

void do_multi()
{
    plt_types p_type;
    int i;
    int legs;
    double base, color;
    int yorn;

    NOISE("PS graph mode using file");
                                    /* PS - file only, LDPLOT.PS default */
    cm_setup_gtjfn(&jgb, GJ_FOU, NULL, NULL,
                   cm_str("LDPLOT"), cm_str("PS"));
    out_fdb.cmF_help = cm_str("Output file (.PS) or RETURN for default");
    cm_parse(&out_fdb);
    fout = fdopen(cm_data.cmR_fd, "w");
    cm_confirm();

    count = 0;                      /* Reset plots count */
    cm_prompt_set(cm_str("MULTI>"));
    for (;;) {                      /* Not forever, but until ready */
        cm_set_error_trap();        /* Errors will come to here */
        fflush(stdout);
        cm_start_parse();           /* Set-up prompt, parameters */
        cm_set_reparse();           /* And so will reparses (^U) */

        cm_link_fdb(&plot_fdb, &mlt_fdb);   /* Add terminators to plots */
        cm_parse(&plot_fdb);
        p_type = CTD;                       /* Get response */
        if (cm_used == &mlt_fdb) {          /* A terminator was given */
            switch (p_type) {
                case MLT_Abort: 
                    NOISE("MULTI mode immediately");
                    break;
                case MLT_Quit:
                    NOISE("MULTI mode with option to plot");
                    break;
            }
            cm_confirm();
            break;                    /* Break for(;;) loop */
        }
        if (ask_dates(TRUE, p_type))  /* Otherwise, a plot was given */
            (void) plot_driver(TRUE, p_type, GPH_Postscript);
    }  /* End for */

    if (count > 0 && p_type == MLT_Quit) {
        cm_prompt_set(cm_str("Output Plot? ")); /* Ask if want to plot */
        cm_set_error_trap();
        fflush(stdout);
        cm_start_parse();           /* Reset COMND parameters again */
        cm_set_reparse();

        cm_parse(&boo_fdb);         /* Get a boolean answer */
        yorn = CTD;
        cm_confirm();
            
        if (yorn) {                     /* Answer was yes */
            base = 1.0 / count;         /* Compute color increment */
            ps_header("Multiple days"); /* Non-specific Title: */
            fputs("%%Page: ? ?\n", fout);
            if (reduction)                  /* Shrink this to 90% ? */
                fputs("Shrink\n", fout);
            fputs("LA TD TI\n", fout);
            if (legend) {               /* Produce legends if selected */
                legs = count;
                fputs("VSP\n", fout);
                for (i = 0; i < count; i++) {
                    color = base * (i);
                    fprintf(fout, "(%s) %.2f Leg\n", mplots[i].rstr, color);
                }
            }
            else legs = 0;

            fprintf(fout, "%d %d %d TR\n", max_height, max_height, legs);
            fputs("Axes\n", fout);
            if (grid)
                fputs("Grid\n", fout);
            for (i = 0; i < count; i++) {
                color = base * (i);
                fprintf(fout, "%% %s\n", mplots[i].rstr);
                fprintf(fout, "%.2f BPLT\n", color);
                fprintf(fout, "%sPlot\n", mplots[i].pstr);
                fputs("EPLT\n", fout);
            }
            fputs("showpage\n", fout);
            ps_trailer();
            rid_plots();                /* Deallocate plot strings */
        }
    }

    fclose(fout);                   /* Maybe delete an empty file later? */
    cm_link_fdb(&plot_fdb, NULL);   /* Remove terminators from PLOT fdb */
}

/* DO_PLOT() - PLOT command handler/parser

    Do_plot() parses the necessary keywords and destinations for plotting
    a graph.  Average type, dates, graph type, and destination are all
    parsed, and if no errors occur and confirmation is given, plot_driver()
    is called to setup the plot.
 */

bool do_plot()
{
    cm_fdb *tmp_fdb;
    gph_types g_type;
    plt_types p_type;

    if (!have_file) {               /* Check for screw-up */
        error("No data file");
        return (FALSE);
    }

    NOISE("average type");          /* Get plot type */
    cm_parse(&plot_fdb);
    p_type = CTD;

    if (!ask_dates(FALSE, p_type))
        return (FALSE);

    NOISE("format");                /* Graph format - text or ps */
    if (ps_available)               /* Offer both if ps-prolog found */
        cm_parse(&gpha_fdb);
    else cm_parse(&gphb_fdb);       /* Else just offer text */
    g_type = CTD;

    NOISE("to file");               /* Get destination */
    switch (g_type) {               /* Text - file or TTY: by default */
        case GPH_Text:
            cm_setup_gtjfn(&jgb, GJ_FOU, NULL, NULL, NULL, NULL);
            out_fdb.cmF_help = cm_str("Output file (.TXT)");
            cm_link_fdb(&cfm_fdb, &out_fdb);
            tmp_fdb = &cfm_fdb;
            break;
                                    /* PS - file only, LDPLOT.PS default */
        case GPH_Postscript:
            cm_setup_gtjfn(&jgb, GJ_FOU, NULL, NULL,
                   cm_str("LDPLOT"), cm_str("PS"));
            out_fdb.cmF_help = cm_str("Output file (.PS) or RETURN for default");
            tmp_fdb = &out_fdb;
            break;
    }
    cm_parse(tmp_fdb);
    switch (cm_used->cmF_function) {    /* Only applies to Text */
        case _CMCFM: fout = stdout;	/* Use stdout if user just confirmed */
                     break;
        case _CMFIL: fout = fdopen(cm_data.cmR_fd, "w"); /* Else use file */
                     cm_confirm();
                     break;
        default: fout = NULL;
                 error("Wierd parse case");
                 break;
    }

    if (fout == NULL)
        error("Do_plot: Couldn't open dest file");
    else {                                             /* Set it up */
        (void) plot_driver(FALSE, p_type, g_type);
        if (fout != stdout)
            fclose(fout);
    }
}

/* DO_SET() - SET parameter command

    Grid - Grid lines on Postscript graphs.
    Legends - Legend marks on MULTI Postscript graphs.
    Maximum - global Max height (LA) of graph.
    Weekends - global boolean of whether weekends are included.
    Smoothing - global boolean of whether text graphs are smoothed.
    NO - allows setting boolean parameters to FALSE.
*/

void do_set()
{
    set_types s;

    cm_parse(&set_fdb);

    switch (CTD) {
        case SET_Grid:
            NOISE("lines");
            cm_confirm();
            grid = TRUE;
            break;
        case SET_Legend:
            NOISE("on Multi PS graphs");
            cm_confirm();
            legend = TRUE;
            break;
        case SET_Maximum:
            NOISE("graph height");
            cm_parse(&ht_fdb);
            max_height = cm_data.cmR_number;
            cm_confirm();
            break;
        case SET_No:
            cm_parse(&no_fdb);
            s = CTD;
            cm_confirm();
            switch (s) {
                case SET_Grid:
                    grid = FALSE;
                    break;
                case SET_Legend:
                    legend = FALSE;
                    break;
                case SET_Reduction:
                    reduction = FALSE;
                    break;
                case SET_Smoothing:
                    smoothing = FALSE;
                    break;
                case SET_Weekends:
                    weekends = FALSE;
                    break;
            }
            break;
        case SET_Reduction:
            NOISE("of PS graphs");
            cm_confirm();
            reduction = TRUE;
            break;
        case SET_Smoothing:
            NOISE("of text graphs");
            cm_confirm();
            smoothing = TRUE;
            break;
        case SET_Weekends:
            NOISE("included");
            cm_confirm();
            weekends = TRUE;
            break;
    }
 }

/* DO_Status() - Show current parameters
 */

void do_status()
{
    NOISE("of program");
    cm_confirm();

    show_file();
    show_weekends();
    show_height();
    show_smoothing();
    show_reduction();
    show_grid();
    show_legend();
    show_plots();
}

/*------------------------------
  Day and Time related functions
  ------------------------------*/

/* ASK_DATES() - Parse date(s) given plot type and whether MULTI
 */

bool ask_dates(mult, p_type)
bool mult;
plt_types p_type;
{
    int mon, year;
    int ablock[5];

    switch (p_type) {               /* Prompt appropriately */
        case PLT_Daily:
            NOISE("starting");
            break;
        case PLT_Monthly:
            NOISE("month");
            break;
        case PLT_Single:
            NOISE("for");
            break;
        case PLT_Weekly:
            NOISE("for week of");
            break;
    }

    switch (p_type) {
        case PLT_Monthly:
            cm_parse(&mmn_fdb);     /* Parse month (keyword) */
            mon = CTD;
            NOISE("year");
            cm_parse(&yr_fdb);      /* Parse year (number) */
            year = CTD;
            
            ablock[2] = (year<<18) + mon;   /* Convert 1-MMM-YY to internal */
            ablock[3] = 0;
            ablock[4] = 0;
            if (!jsys(IDCNV, ablock)) {
                fprintf(stderr, "ask_dates(): IDCNV\n");
                return (FALSE);
            }
            s_date = LH & ablock[2];         /* Make this the starting date */

            mon++;                  
            if (mon >= 12) {
                mon = 0;
                year++;
            }

            ablock[2] = (year<<18) + mon;
            ablock[3] = 0;
            ablock[4] = 0;
            if (!jsys(IDCNV, ablock)) {
                fprintf(stderr, "ask_dates(): IDCNV\n");
                return (FALSE);
            }
            e_date = (LH & ablock[2]) - (1<<18);/* Ending date is last day o mon */
            break;

        default:
            s_date = LH & my_parse(&date_fdb);          /* Starting date */
            if (s_date < s_avail || s_date > e_avail) { /* Check validity */
                error("Date not available in file");
                return (FALSE);
            }

            if (p_type == PLT_Daily) {              /* Ending date if Daily */
                NOISE("ending");
                e_date = LH & my_parse(&date_fdb);
            }
            if (mult)               /* MULTI needs to confirm here */
              cm_confirm();         /* since it doesn't parse anything else */
            break;
    }

    if (p_type == PLT_Daily ||
        p_type == PLT_Monthly) {
        if (e_date > e_avail) {             /* Check validity of ending date */
            error("Date not available in file");
            return (FALSE);
        }
        if (e_date < s_date) {      /* And check for increasing dates */
            error("Ending date must be after starting date");
            return (FALSE);
        }
    }
    return (TRUE);
}

/* DAYTIME() - Return current date and time in char buf[]
 */

bool daytime(buf)
char *buf;
{
    int ablock[5];

    ablock[1] = (int) (buf - 1);
    ablock[2] = -1;                 /* Specify current date & time */
    ablock[3] = OT_4YR;             /* Usual with 4 digit years */
    return (jsys(ODTIM, ablock));
}

/* CONVERT_DATE() - Return ascii string in buf of date

    ODTIM could not directly be used because of the stupidity of LOADED's
    writers.  When they created a day's record, they took the current internal
    format date and time, and cleared the time part.  Clearing the time part
    makes the time Midnight (what they thought) true, but it is midnight
    GMT.  So whenever we try to convert the timestamp using ODTIM, we
    usually get the day before around 17:00 (GMT is 8 hours ahead, depending
    on daylights savings time).

    Solution - pull hair out, then stumble upon using ODCNV to disect the
    time into various pieces, converting to GMT in the process, and then
    using ODTNC (which takes disected time pieces and prints out ascii
    strings).
 */

bool convert_date(buf, date, mon)
char *buf;
int date;
bool mon;
{
    struct tm *gmtstr;
    time_t t;

    t = tadl_to_utime(date);

    if ((gmtstr = (struct tm *) gmtime(&t)) == NULL) {
        fprintf(stderr, "Convert_date: ?gmtstr()\n");
        return (FALSE);
    }

    if (mon)
        strftime(buf, DATE_LEN, "%B %Y", gmtstr);
    else strftime(buf, DATE_LEN, "%a %d-%b-%Y", gmtstr);

    return (TRUE);

/*    int block[5];

    block[2] = date;
    block[4] = IC_UTZ;
    jsys(ODCNV, block);
    
    block[1] = (int) (buf - 1);
    block[4] = 0;
    if (mon)
	block[5] = (1<<31) | (1<<30) | (1<<26) | (1<<18);
    else block[5] = (1<<34) | (1<<26) | (1<<18);

    return (jsys(ODTNC, block));
*/
}

/* MY_PARSE() - Special hack of Ian Macky's cm_parse()

    My_parse() provides date parsing needed by LDPLOT which is absent
    from cm_parse().  Also, cm_parse() opens the parsed filename
    if it is sucessful and returns an integer file descriptor which
    is not particularly useful for further JSYS calls.  Instead,
    my_parse() returns the JFN of the parsed filename, and does not
    open it.
 */

int my_parse(fdb)
cm_fdb *fdb;
{
    int ablock[5];

    ablock[1] = (int) &CSB;
    ablock[2] = (int) fdb;
    if ((jsys(COMND, ablock) < 1) || (ablock[1] & CM_NOP)) {
	cm_terpri();				/* get to new line */
	printf("?%s\n", strerror(-1));		/* output system error */
	longjmp(cm_error_jmpbuf,1);		/* jump to user error */
    }						/* restart point. */
    CSB.r_flags = ablock[1];			/* save returned flags, */
    CSB.r_used = (cm_fdb *) cm_global_ptr(ablock[3]);

    switch (CSB.r_used->cmF_function) {
        case _CMCFM: return (-1);
                     break;
        case _CMFIL: return (ablock[2]);
                     break;
        case _CMTAD: return (ablock[2]);
                break;
    }
}

/*---------------------
  I/O related functions
  ---------------------*/

/* GET_FILE() - Make datafile current

    Get_file makes a datafile current when given a just-parsed JFN.
    First thing to do is get the whole pathname string, for use
    in showing to the user.  Then, we get the file length (in 36-bit words)
    and make that global. It had better be a multiple of 9! (9 words to
    a record).

    Finally, the first word both the first and last records are read in,
    and deciphered as datestamps using convert_date().  These are made
    global to apprise the user at any time of the dates contained within
    the currently selected datafile.
 */

bool get_file(jfn)
{
    int ablock[5];

    ablock[2] = jfn;                    /* Want full filename string */
    ablock[1] = (int) (in_buf - 1);
    ablock[3] = 0;
    ablock[4] = 0;
    if (!jsys(JFNS, ablock)) {
        error("Get file: JFNS failed");
        return (FALSE);
    }
    else {                              /* Here's how to get it */
        in_name = in_buf;
        have_file = TRUE;
    }

    ablock[1] = jfn;                    /* Now want length of file */
    ablock[2] = (1 << 18) + _FBSIZ;     /* Want 1 word, the .FBSIZ */
    ablock[3] = (int) &in_len;          /* Stuff it in global in_len */
    if (!jsys(GTFDB, ablock)) {
        error("Get_file: GTFDB failed");
        return (FALSE);
    }

    if (in_len % 9 != 0) {                  /* Should be complete records */
        error("Get_file: wierd file size");
        return (FALSE);
    }

    ablock[1] = jfn;
    ablock[2] = 36 | OF_RD;         /* Open file as 36-bit, for read-only */
    if (!jsys(OPENF, ablock)) {
        error("Get_File: OPENF");
        return (FALSE);
    }

    if (!get_date(0)) {                             /* Get start date */
        error("Get_file: Couldn't get start date");
        return (FALSE);
    }
    else {
        s_avail = ld_buf[0] & LH;       /* Mask any garbage on LH */
        convert_date(s_buf, s_avail, FALSE);   /* Convert screwy date */
    }

    if (!get_date(in_len - RECORD_SIZE)) {  /* Get last record's date */
        error("Get_file: Couldn't get ending date");
        return (FALSE);
    }
    else {
        e_avail = ld_buf[0] & LH;
        convert_date(e_buf, e_avail, FALSE);
    }
    return (TRUE);
}

/* GET_DATE() - Get date word at head of a record.

    Given a "byte" (word really) position within the datafile, read in one
    "byte" (word) which should be the datestamp word at the head of a record.
    The file position pointer is left pointing to the following word.
 */

bool get_date(pos)
int pos;
{
    int ablock[5];

    if (pos < in_len) {             /* Check for validity */
        ablock[1] = in_jfn;         /* Read from this JFN */
        ablock[3] = pos;            /* ..at this position */
        if (jsys(RIN, ablock)) {
            ld_buf[0] = ablock[2];  /* Place word in first buffer position */
            return (TRUE);
        }
    }
    return (FALSE);
}


/* READ_RECORD() - Read rest of record whose date is already read.

    The assumption is made here that the datestamp header word of a
    particular record has already been read in and accepted.  All we
    need to here is to read in the next sucessive 8 words to the buffer.
    Note that SIN would not work correctly using the C compiler.  Apparently
    the destination pointer couldn't be coerced to the needed size.
    8 sucessive BIN's were used instead (big deal).
 */

bool read_record()
{
    int ablock[5], i;
    
    for (i = 1; i < RECORD_SIZE; i++) {
        ablock[1] = in_jfn;
        if (!jsys(BIN, ablock))
            return (FALSE);
        else ld_buf[i] = ablock[2];
    }
    return (TRUE);
}

/* DATE_LOC() - Return file offset given requested date
 */

int date_loc(date)
int date;
{
    return (((date>>18) - (s_avail>>18)) * RECORD_SIZE);
}

/* RELEASE_FILE() - Close file and release JFN given file JFN
 */

void release_file(jfn)
int jfn;
{
    int ablock[5];

    ablock[1] = jfn;
    (void) jsys(CLOSF, ablock);    /* Close old jfn */
    ablock[1] = jfn;
    (void) jsys(RLJFN, ablock);    /* ..and release it */
}

/* TTY_JFN() - Assign and return a JFN to the TTY:
 */

int tty_jfn()
{
    int ablock[5];

    ablock[1] = GJ_SHT | GJ_FOU;
    ablock[2] = (int) cm_str("TTY:");
    if (jsys(GTJFN, ablock))
        return (RH & ablock[1]);
    else return (-1);
}

/*------------------------
  Status display functions
  ------------------------*/

/* SHOW_FILE() - Show info on currently selected datafile
 */

void show_file()
{
    fprintf(stderr, "File: ");
    if (have_file) {
        fprintf(stderr, "%s\n", in_name);
        fprintf(stderr, "Available: %s to %s\n", s_buf, e_buf);
    }
    else fprintf(stderr, "<no data file>\n");
}

/* SHOW_WEEKENDS - echo weekends (included) variable
 */

void show_weekends()
{
    fprintf(stderr, "Weekends: ");
    if (!weekends)
        fprintf(stderr, "NO\n");
    else fprintf(stderr, "Yes\n");
}

/* SHOW_PLOTS - List available plots
 */

void show_plots()
{
    fprintf(stderr, "Plots Available: Text");
    if (ps_available)
        fprintf(stderr, ", Postscript");
    fprintf(stderr, "\n");
}

/* SHOW_HEIGHT() - show current Maximum graph height setting
 */

void show_height()
{
    fprintf(stderr, "Max Height: %d\n", max_height);
}

/* SHOW_GRID() - Show PS grid-lines parameter
 */
void show_grid()
{
    fprintf(stderr, "Grid-lines: ");
    if (grid)
        fprintf(stderr, "YES\n");
    else fprintf(stderr, "No\n");
}

/* SHOW_REDUCTION() - show current ps graph scale setting
 */

void show_reduction()
{
    fprintf(stderr, "PS Graph reduction: ");
    if (reduction)
        fprintf(stderr, "Yes\n");
    else fprintf(stderr, "NO\n");
}


/* SHOW_SMOOTHING() - Show current graph smoothing parameter
 */

void show_smoothing()
{
    fprintf(stderr, "Graph smoothing: ");
    if (smoothing)
        fprintf(stderr, "YES\n");
    else fprintf(stderr, "No\n");
}

/* SHOW_LEGEND() - Show current Multi PS graph legend parameter
 */
void show_legend()
{
    fprintf(stderr, "Multi-mode Legends: ");
    if (legend)
        fprintf(stderr, "YES\n");
    else fprintf(stderr, "No\n");
}    

/*--------------------------------------------
  Average init, enter, and calculate functions
  --------------------------------------------*/

/* INIT_AVERAGE() - Clears Accumulators and hit counts
 */

void init_average()
{
    int i;

    for (i = 0; i < HRES; i++) {
        ave_array[i].accum = 0;
        ave_array[i].hits = 0;
    }
}

/* ENTER_RECORD() - Enter one day's worth of load data into ave struc.

    Enter_record slurps out the 48 data points from the 8 36-bit words
    contained in the global buffer ld_buf.  A tricky 6-bit char pointer
    is used to this end, along with a touch of coercion (friendly or not).
 */

void enter_record()
{
    int i;
    char6 *ptr;
    int val;

    ptr = (char6 *) &ld_buf[1];         /* Start with first data word */
    for (i = 0; i < HRES; i++) {
        val = (int) *ptr++;
        if (val < MAX_DATA_VALUE) {     /* Enter only if actual reading */
            ave_array[i].accum += val;  /* Accumulate */
            ave_array[i].hits++;        /* Point found for this slice */
        }
    }
}

/* CALC_AVERAGE() - Calculate average of accumulated data.
 */
void calc_average()
{
    int i;

    for (i = 0; i < HRES; i++) {
        if (ave_array[i].hits == 0)
            ave_array[i].accum = -1;
        else ave_array[i].accum /= ave_array[i].hits;
    }
}

/* CALC_SMOOTHING() - Calculate fill range to smooth the text graph vertically.
    This routine calculates the upper and lower limits of a vertical line
    of asterixes to be printed when producing a smoothed text graph.
    The y component (the average at time x) is used along with the y value
    of the point immediately to the left and right, to determine whether
    a vertical line is needed (and how long it should be) to produce a text
    graph that appears connected visually.
 */

void calc_smoothing()
{
    int l, c, r;
    int lx, rx;
    int dif, i;

    for (i = 0; i < HRES; i++) {
        if ((c = round(ave_array[i].accum)) != -1) {
            if (i == 0 || (l = round(ave_array[i-1].accum)) == -1)
                l = c;
            if (i == HRES-1 || (r = round(ave_array[i+1].accum)) == -1)
                r = c;

            if (l == c)
                lx = c;
            else {
                dif = l - c;
                if (abs(dif) > 1) {
                    lx = abs(dif) / 2;
                    if (abs(dif) % 2 == 0)
                        lx -= 1;
                    if (dif < 0)
                        lx = lx * -1;
                    lx += c;
                }
                else lx = c;
            }

            if (r == c)
                rx = c;
            else {
                dif = r - c;
                if (abs(dif) > 1) {
                    rx = (abs(dif) / 2) /* + (abs(dif) % 2) */ ;
                    if (dif < 0)
                        rx = rx * -1;
                    rx += c;
                }
                else rx = c;
            }

            if (c > lx && c > rx) {
                ave_array[i].sm_high = c;
                ave_array[i].sm_low = MIN(lx,rx);
            }
            else {
                if (c < lx && c < rx) {
                    ave_array[i].sm_high = MAX(lx,rx);
                    ave_array[i].sm_low = c;
                }
                else {
                    ave_array[i].sm_high = MAX(lx,rx);
                    ave_array[i].sm_low = MIN(lx,rx);
                }
            }
        }
    }
}

/*----------------------------------------------
  Plotting routines, setup and support functions
  ----------------------------------------------*/

/* PLOT_DRIVER() - Prepare parameters, gather data, and otherwise setup
                   to call one of the plot routines.
 */

bool plot_driver(mult, p_type, g_type)
bool mult;
plt_types p_type;
gph_types g_type;
{
    int i, start, end, ablock[5];
    int dow, d;
    bool has_week = FALSE;
    char st_buf[DATE_LEN];
    char end_buf[DATE_LEN];
    char sprbuf[80];

    if (p_type != PLT_Weekly &&
	p_type != PLT_Monthly)
        convert_date(st_buf, s_date, FALSE); /* Convert start date to string */

    switch (p_type) {
        case PLT_Daily:                     /* Setup for Daily */
            start = s_date;                 /* Start day in GMT*/
            end = e_date;                   /* End day in GMT*/
            convert_date(end_buf, end, FALSE);
            sprintf(sprbuf, "from %s to %s", st_buf+4, end_buf+4);/* Build title */
            break;

        case PLT_Monthly:
            start = s_date;
            end = e_date;
	    convert_date(st_buf, s_date, TRUE);
	    sprintf(sprbuf, "for %s", st_buf);
	    break;
	    
        case PLT_Single:                    /* Setup for Single */
            start = s_date;
            end = start;                    /* End is same as start */
            sprintf(sprbuf, "for %s", st_buf+4);
            break;

        case PLT_Weekly:                    /* Setup for Weekly */
            ablock[2] = s_date;
            ablock[4] = IC_UTZ;
            (void) jsys(ODCNV, ablock);     /* Break up (in GMT) */
            dow = ablock[3] & RH;
            start = s_date - (dow << 18);   /* Make sure start is Mon */
            convert_date(st_buf, start, FALSE);/* Now convert it to a string */
            if (weekends)
                end = start + (6<<18);    /* End is Sun if weekends included */
            else end = start + (4<<18);     /* ...Friday if not */
            convert_date(end_buf, end, FALSE);/* And convert */
            sprintf(sprbuf, "from %s to %s", st_buf+4, end_buf+4);
            break;
    }

/* Common to PLT_Weekly, PLT_Daily, and PLT_Single */

    init_average();
    i = start;

    while (i <= end) {                      /* Slurp up info */
        d = date_loc(i);
        if (!get_date(d)) {
            error("File is inconsistant");
            return (FALSE);
        }
        if ((LH & ld_buf[0]) != i) {
            error("File out of sequence");
            return (FALSE);
        }

        ablock[2] = i;      /* Find out what day of week i is */
        ablock[4] = IC_UTZ;
        (void) jsys(ODCNV, ablock);
        if (((dow = (ablock[3] & RH)) < 5) || weekends ||   /* If all ok... */
                     p_type == PLT_Single) {
            if (!read_record()) {                           /* Read record */
                error("Unable to finish reading record");
                return (FALSE);
            }
            enter_record();                                 /* Then enter it */
            if (dow >= 5)
                has_week = TRUE;
        }
        i += 1<<18;                                        /* Increment date */
    }
    calc_average();
    if (has_week && (p_type != PLT_Single))
        strcat(sprbuf, " (w/Weekends)");

    switch (g_type) {
        case GPH_Postscript: 
            if (mult)
                multi_plot(sprbuf);
            else ps_plot(sprbuf);
            break;
        case GPH_Text: 
            if (smoothing)
                calc_smoothing();
            txt_plot(sprbuf);
            break;
    }
}

/* TXT_PLOT() - Text Plot routine
 */

void txt_plot(range)
char *range;
{
    int x, y, i, j;

    fputc('\n', fout);
    center_print(top);          /* Print Banner */
    center_print(range);        /* Print range title */
    fputc('\n', fout);

    y = max_height * RES_FACTOR;
    vmark(y, TRUE);             /* Max y value must be ticked and numbered */
    for (x = 0; x < HRES; x++)  /* Max y is a special case, val may exceed */
        if (ave_array[x].accum > y)
            fputc('!', fout);
        else if (ave_array[x].accum >= y - 0.5)
                  fputc('*', fout);
             else fputc(' ', fout);
    fputc('\n', fout);

    for (y--; y >= 0; y--) {            /* All other rows handled here */
        if (y % (5 * RES_FACTOR) == 0)
            vmark(y, TRUE);             /* Tick on 5's */
        else vmark(y, FALSE);
        for (x = 0; x < HRES; x++)      /* Now scan row l-r and print */
            if (smoothing) {            /* Smooth graph logic */
                if (is_target(x, y))
                    fputc('*', fout);
                else fputc(' ', fout);
            }
            else {                      /* Scatter plot logic */
                if (round(ave_array[x].accum) == y)
                    fputc('*', fout);
                else fputc(' ', fout);
            }
        fputc('\n', fout);
    }

    i = 0;
    while (hbar[i] != NULL) {               /* Print Horz axis and legends */
        for (j = 1; j <= LMARGIN + 2; j++)  /* ...which handily are arrayed */
            fputc(' ', fout);
        fprintf(fout, "%s\n", hbar[i]);
        i++;
    }
}

/* PS_HEADER() - Write an Adobe Header and Prolog to fout
 */

void ps_header(title)
char *title;
{
    char datebuf[DATE_LEN];
    int c;

    fputs("%!PS-Adobe-\n", fout);
    fprintf(fout, "%%%%Title: Load Average Graph %s\n", title);
    fputs("%%Creator: LDPLOT by Todd Koumrian, SRI International 1988\n",
                    fout);
    fprintf(fout, "%%%%For: %s\n", getlogin());
    if (daytime(datebuf))
        fprintf(fout, "%%%%CreationDate: %s\n", datebuf);
    fputs("%%DocumentFonts: Helvetica\n", fout);
    fputs("%%Dimensions: 0 0 612 792\n", fout);
    fputs("%%Pages: 1\n", fout);
    fputs("%%EndComments\n", fout);

    rewind(ps_prog);                        /* Insert Prolog */
    while((c = getc(ps_prog)) != EOF)
        fputc(c, fout);
    fputs("%%EndProlog\n", fout);
}

/* PS_TRAILER() - Write an Adobe Trailer to fout
                  No big deal since the header was pretty complete.
 */

void ps_trailer()
{
    fputs("%%Trailer\n", fout);
}

/* PS_PLOT() - Straightforward Postscript Plotter (i.e. non-MULTI)
               Output is written to fout.
 */
void ps_plot(range)
char *range;
{
    int x;
    double tmp;

    ps_header(range);               /* Write the Header and Prolog */

    fputs("%%Page: ? ?\n", fout);   /* Indicate no page number */
    if (reduction)                  /* Shrink this to 90% ? */
        fputs("Shrink\n", fout);
    fputs("LA TD TI\n", fout);
    fprintf(fout, "(%s) STI\n", range);
    fprintf(fout, "%d %d %d TR\n", max_height, max_height, NO_LEGENDS);
    fputs("Axes\n", fout);
    if (grid)
        fputs("Grid\n", fout);
    fputs("0 BPLT\n", fout);    /* Black line for non-MULTI graph */
    for (x = 0; x < HRES; x++)
        if ((tmp = ave_array[x].accum) < 0)     /* Output averages */
            fprintf(fout, "%.0f ", tmp);
        else fprintf(fout, "%.2f ", tmp / 2);   /* 0.5 int res, remember? */
    fputs("Plot\n", fout);
    fputs("EPLT\n", fout);
    fputs("showpage\n", fout);                  /* Print it! */
    ps_trailer();                               /* Write trailer */
}

/* MULTI_PLOT() - MULTI-mode "plotter"

    MULTI_PLOT() doesn't actually write anything to fout, like the other
    plotters do.  Instead, it dynamically allocates string storage to
    hold the string that will be written to fout later, after the user
    has finished specifying plots in MULTI-mode.
    A string is also allocated to hold the Title string for the current
    plot.
 */
void multi_plot(range)
char *range;
{
    char small[10];
    char *p, *r;
    double y;
    int i;

    if (count < MAX_PLOTS) {
        if ((p = (char *) malloc(PLOT_LINE_LEN)) == NULL ||  /* Plot */
            (r = (char *) malloc(80)) == NULL)               /* Range string */
            error("Couldn't allocate string storage");
        else {
            mplots[count].rstr = r;
            strcpy(r, range);
            mplots[count].pstr = p;
            for (i = 0; i < HRES; i++) {
                if ((y = ave_array[i].accum) < 0)
                    sprintf(small, "%0.f ", y);
                else sprintf(small, "%.2f ", y/2);
                if (i > 0)                          /* Copy a[0], cat others */
                    strcat(p, small);
                else strcpy(p, small);
            }
            count++;
        }
    }
    else error("No more free entries in mplots[] for this plot");
}

/* RID_PLOTS() - Deallocate strings from MULTI_PLOT().
 */
void rid_plots()
{
    int i;

    for (i = 0; i < count; i++) {
        free(mplots[i].rstr);
        mplots[i].rstr = NULL;
        free(mplots[i].pstr);
        mplots[i].pstr = NULL;
    }
    count = 0;
}

/* VMARK()

    This is part of the text plotting routines.  Given the current row y,
    VMARK prints a letter of "LOAD AVERAGE", indents, ticks and numbers
    the y axis if TICK is TRUE.

    Note that max_height is used to calculate how the string is centered
    vertically.
 */

void vmark(y, tick)
int y;
bool tick;
{
    int slen;
    int width;
    char numbuf[5];
    int i;

    slen = ((max_height * RES_FACTOR) - strlen(side)) / 2;
    if (y > slen && y <= ((max_height * RES_FACTOR) - slen))
        fputc(side[((max_height * RES_FACTOR)-slen) - y], fout);
    else fputc(' ', fout);

    if (tick) {
        sprintf(numbuf, "%d", y / RES_FACTOR);
        width = strlen(numbuf);
        for (i = 1; i <= (LMARGIN - (width + 1)); i++)
            fputc(' ', fout);
        fprintf(fout, "%s +", numbuf);
    }
    else {
        for (i = 1; i <= LMARGIN; i++)
            fputc(' ', fout);
        fputc('|', fout);
    }
 }

/* IS_TARGET() - Used by TXT_PLOT() for smoothed graphs
    IS_TARGET() takes coordinates (x,y) and returns true if
    the load average is y at point x or if a smoothed line crosses
    (x,y).  The source of this data are the smoothing registers in
    ave_array, so CALC_SMOOTHING() must previously be performed.
 */

int is_target(x, y)
int x;
int y;
{
    if (ave_array[x].accum < 0)
        return (FALSE);
    else return (y >= ave_array[x].sm_low && y <= ave_array[x].sm_high);
}

/* ROUND() - Perform true rounding of a double to an int, return int.
 */

int round(num)
double num;
{
    int x;

    x = (int) num;
    if (num - x >= 0.5)
        return (x + 1);
    else return (x);
}

/* CENTER_PRINT() - Print a string horizontally centered to fout
     Used in TXT_PLOT()
 */

void center_print(outstr)
char *outstr;
{
    int indent, i;

    indent = (TEXTWIDTH - strlen(outstr)) / 2;
    for (i = 1; i <= indent; i++)
        fputc(' ', fout);
    fprintf(fout, "%s\n", outstr);
}

/*-----------------------
  Miscellaneous functions
  -----------------------*/

/* CLEANUP() - Cleanup JFN's and streams left hanging after a parse error
               or ^U (Lots of programs never do this.
*/

void cleanup()
{
    if (tmp_jfn != -1) {
        release_file(tmp_jfn);
        tmp_jfn = -1;
    }
    if (fout != NULL && fout != stdout) {
        fclose(fout);
        fout = NULL;
    }
}

/* ERROR() - Print error message string on tty
 */

void error(err)
char *err;
{
    fprintf(stderr, "?%s\n", err);
 }
