/***************************************************************************
 PAGEIO.C -

     Todd Koumrian
     SRI International
     3 January 1988

         This file contains all of the definitions and code for the
     PageIO module used by the REVER page reversal program.

    All file input/output operations are implemented by code in this
    file.  The I/O implementations and the mechanisms for marking page
    off pages as well as for copying them are hidden from sight of the
    the user program.

 16-Jul-1988: Reorganized order of functions in file (.h file too).  Added
              REWIND_SOURCE() function for use in rever auto-identify.
 22-Jul-1988: Changed function REWIND_SOURCE() to WINDBACK_SOURCE().  KCC
              won't compile properly otherwise.
 14-Sep-1988: Added UNIX variant defs for portage.
  1-Nov-1988: Changed Init & Wrap_Up to allow stdin and stdout.
  2-Nov-1988: Rewrote all routines to use a memory buffering method instead
               of the previous all file method.  Added separate end_page()
               and start_page() routines for proper Postscript marking.
  4-Nov-1988: Added record/playback modes, to allow regurgitating the
               source ala windback_source() used to provide.
  3-Jan-1989: Changed function names to new convention.  Added pg_seek_page()
               to allow playback starting at any page, and added pgmrecord
               flag (and pg_init() param) to select whether source is to
               be recorded or not.
 **************************************************************************/

#include <c-env.h>
#include <stdio.h>
#include "pageio.h"

#if (SYS_T20)
#include <stdlib.h>
#endif

#if (SYS_UNIX)
#define SEEK_SET 0       /* BSD doesn't define the fseek() names */
#endif

#define CP_RANGE_ERR    1     /* Error code for Copy_Page param range error */
#define CP_SEEK_ERR     2     /* Error code for Copy_Page infile seek error */
#define SOURCE_OPEN_ERR 3     /* Error code for Source file openning error */
#define DEST_OPEN_ERR   4     /* Error code for Dest file openning error */
#define INS_OPEN_ERR    5     /* Error code for Insert file openning error */
#define NO_MEMORY       6     /* Malloc() failed error */
#define NO_LINES        7     /* No room left in line_array[] */
#define PAGE_OVERFLOW   8     /* No room left in pgmparr[] */

#define LINE_ARRAY_SIZE 100000/* Pointers for this many lines */

#define FALSE 0
#define TRUE  1

typedef int bool;

void pg_err();          /* Print err message, prepare for exit */
char *malloc();         /* Our friend, the mallocator. */

int    pgmchr;
char   pgmbuffer[MAX_PLINE];
char   *pgmlarr[LINE_ARRAY_SIZE];  /* Array of pointers to line strings */
u_long pgmlines;              /* Index counter to above array */
u_long pgmparr[MAX_PAGES];    /* Array of start-of-page positions. */
u_int  pgmpages;              /* module counter for number of pages. */

int    pgmsource;             /* module mode - record/playback */
bool   pgmrecord;             /* If source is record, do we really record? */
u_int  pgmrppage;             /* playback mode current page index */
u_long pgmrpline;             /* playback mode line index */
char   *pgmrpchar;            /* playback mode char placekeeper */

FILE   *pgmin, *out;          /* File handles */

/***************************************************************************
  PG_INIT() - 

           Arguments:  SOURCE and DEST - source and destination file
                         filename strings.
                       RECORD - non-zero indicates input will be recorded.
             Returns:  nothing.

     This procedure initializes the PageIO module by clearing the line,
     page and line char counters, and attempts to open files with the names
     passed as parameters *source and *dest.  A null argument specifies
     the use of stdin or stdout streams.  The module mode is set according
     to the value of record.
 **************************************************************************/

void pg_init (source, dest, record)
char *source, *dest;
bool  record;
{
    pgmsource = FALSE;
    pgmrecord = record;
    pgmchr = 0;
    pgmlines = 0;
    pgmpages = 0;

    if (source != NULL) {
        if ((pgmin = fopen(source, "r")) == NULL)
            pg_err (SOURCE_OPEN_ERR);
    }
    else pgmin = stdin;    /* Use specified input or else stdin */

    if (dest != NULL) {
        if ((out = fopen(dest, "w")) == NULL)
            pg_err (DEST_OPEN_ERR);
    }
    else out = stdout;  /* Use specified output else stdout */
}

/***************************************************************************
  PG_CLOSE - Wrap up PageIO module

        This procedure should be called by the user program
          after it finishes with the PageIO module, normally just       
          before exit.  Pg_close closes the input and output files.   
 **************************************************************************/

void pg_close()
{
    u_long i;

    for (i = 0; i < pgmlines; i++) {
        free(pgmlarr[i]);
        pgmlarr[i] = NULL;
    }

    if (pgmin != stdin)
        (void) fclose(pgmin);
    if (out != stdout)
        (void) fclose(out);
}

/****************************************************************************
 PG_GET_CHR()  -  Get a character.

         Returns:  an integer (includes EOF char).

pgmsource == FALSE:
    A char is read from the source stream and returned.  If pgmrecord
    is set (as initialized by pg_init()), then the character is stored.

pgmsource == TRUE:
    The char at the current position in the previously stored input is
    returned, and the pointers updated for the next call.  See pg_playback()
    and pg_seek_page() for setting the current position.
 ***************************************************************************/

int pg_get_chr()
{
    int c;

    if (!pgmsource) {           /* function returns from input source */
        c = getc(pgmin);
        if (pgmchr < MAX_PLINE && c != EOF)
            pgmbuffer[pgmchr++] = c;
        return (c);
    }
    else {                      /* function returns from stored input */
        if (pgmrpline < pgmlines && pgmrpline <= pgmparr[pgmrppage] &&
               pgmrpchar != NULL) {
            if (*pgmrpchar != NUL)
                return (*pgmrpchar++);
            else {
               pgmrpline++;
               if (pgmrpline < pgmlines && pgmrpline <= pgmparr[pgmrppage]) {
                    if ((pgmrpchar = pgmlarr[pgmrpline]) != NULL)
                        return (*pgmrpchar++);
                    else return (EOF);
               }
               else return (EOF);
            }
        }
        else return (EOF);
    }
}

/****************************************************************************
 PG_GET_LINE()  -  

              Arguments:  BUF, SIZE - Buffer and size to return line in.
                Returns:  pointer to string containing line or NULL if EOF.

pgmsource == FALSE:
     GET_LINE attempts to read a NEWLINE terminated line from the current
     position of stream pgmin.  NULL is returned if EOF is read, else the arg
     string pointer is returned.  The newline is preserved.  This routine
     will also complete the reading of a line, even if pg_get_chr() has
     previously been called on this line.  This input is recorded if
     pgmrecord is set (set by call to pg_init()).

pgmsource == TRUE:
     Returns the recorded source in sequence by lines.  Pg_get_line() may also
     be called on a line which has already been partially read by
     pg_get_char(), and will then return the rest of the line.
 ***************************************************************************/

char *pg_get_line(buf, size)
char buf[];
int size;
{
    int c;

    if (!pgmsource) {       /* Read from stream mode */
        if (fgets(&pgmbuffer[pgmchr], MAX_PLINE - pgmchr, pgmin)) {
            pgmchr = strlen(pgmbuffer) + 1;
            pg_end_line();
            strncpy(buf, pgmbuffer, size);
            return(buf);
        }
        else return(NULL);

/*        do {
            c = get_chr();
        } while (c != EOF && c!= NEWLINE);
        if (c == NEWLINE) {
            pg_end_line();
            strncpy(buf, pgmbuffer, size);
            return (buf);
        } 
        else return (NULL);
*/
    }
    else {      /* Read from memory (playback) mode */
        if (pgmrpline < pgmlines && pgmrpline <= pgmparr[pgmrppage] &&
                        pgmrpchar != NULL) {
            strncpy(buf, pgmrpchar, size);
            pgmrpline++;
            if (pgmrpline < pgmlines && pgmrpline <= pgmparr[pgmrppage])
                pgmrpchar = pgmlarr[pgmrpline];
            else pgmrpchar = NULL;
            return (buf);
        }
        else return (NULL);
    }
}

/***************************************************************************
 PG_PUT_STRING() - Send an arbitrary string to the output stream.

                Arguments:  STRNG - String to be output to dest file.
                  Returns:  nothing.

         This procedure outputs the specified string to the destination
         file at its current position.  This ability to send arbitrary
         text is mainly useful for sending the FFs and NEWLINES
         needed to properly format the reversed pages in the various
         textfile modes (TEXT, BOOK).

         This is necessary since when reversing text files, the last page
         of the input file rarely is MAXLINES long.  Without the FF,
         any following text (which is supposed to start at the top of a
         page) will be placed directly after the last page (not a the top)
         and throw the whole file out of synch.

         In addition, when doing a BOOK reversal, it is necessary to create
         a blank last page when the total number of requested pages is odd,
         and a FF is needed there.  It is possible to encounter the
         situation where two FFs need to be output, when the last
         source page is less than 60 lines long, and (in BOOK) if there
         is an odd number of pages.
 **************************************************************************/

void pg_put_string(strng)
char *strng;
{
    fputs(strng, out);
}

/***************************************************************************
  PG_INSERT_FILE() - 

             Arguments:  FILSTR - string containing path/filename of
                          string to insert.
               Returns:  nothing.

       Pg_insert_file() opens the specified file and writes it to the
       reversal destination file (dest/out) at the current position
       within the file.
 *************************************************************************/

void pg_insert_file(filstr)
char *filstr;
{
    FILE *insrt;
    int c;

    if ((insrt = fopen(filstr, "r")) == NULL)
        pg_err(INS_OPEN_ERR);

    while ((c = getc(insrt)) != EOF)
        putc(c, out);

   (void) fclose(insrt);
}

/***************************************************************************
  PG_END_LINE() - Terminate building of line

    The currently built-up line in pgmbuffer is terminated with a NUL,
    and if pgmrecord is TRUE, (and resources are willing and able), a string
    is allocated into which the buffer is to be copied and logged in
    the pgmlarr[].
 ***************************************************************************/

void pg_end_line()
{
    char *str;

    if (pgmchr < MAX_PLINE) {
        if (pgmbuffer[pgmchr] != NUL)
            pgmbuffer[pgmchr++] = NUL;
    }
    else pgmbuffer[MAX_PLINE - 1] = NUL;

    if (pgmrecord) {
        if (pgmlines < LINE_ARRAY_SIZE) {
            if ((str = (char *) malloc(pgmchr + 1)) != NULL) {
                strcpy(str, pgmbuffer);
                pgmlarr[pgmlines++] = str;
            }
            else pg_err(NO_MEMORY);
        }
        else pg_err(NO_LINES);
    }
    pgmchr = 0;
}

/***************************************************************************
  PG_END_PAGE() - 

             Arguments:  none.
               Returns:  nothing.                            
                                                                         
          This procedure should be called after reading the last      
          character of the current page, and just before reading the  
          first character of the next page.  Pg_end_page() marks the end
          of the current page by inserting the current line number
          pgmlines into pgmparr[] and bumping pgmpages.
 **************************************************************************/

void pg_end_page()
{
    if (pgmrecord) {
        if (pgmpages < MAX_PAGES)
            pgmparr[pgmpages] = pgmlines - 1;
        else pg_err(PAGE_OVERFLOW);
    }
    pgmpages++;
}

/***************************************************************************
  PG_START_PAGE() - Mark the start of a new page (variant of pg_end_page())

      Pg_start_page() should be called after reading the last character of
      the *first* line of a new page.  This is in contrast to pg_end_page()
      which is called after reading the _last_ line of a page.
      Pg_start_page() is needed to properly handle the paging of Postscript
      files, where often it can't be determined that a page boundry has
      appeared until after it has been crossed.
 ***************************************************************************/

void pg_start_page()
{
    if (pgmrecord) {
        if (pgmpages < MAX_PAGES)
            pgmparr[pgmpages] = pgmlines - 2;
        else pg_err(PAGE_OVERFLOW);
    }
    pgmpages++;
}

/***************************************************************************
  PG_COPY_PAGE() - 

            Arguments:  PAGE - logical page to copy.
              Returns:  nothing.

              Pg_copy_page() copies the logical page specified by parameter   
              Page, to the output stream, first checking to see if Page    
              is within acceptable range.

              Since pages are only marked at their finish, the start is
              simply the last line of the previous page + 1.  For the case
              of the very first page, the very first line is used.
 **************************************************************************/

void pg_copy_page(page)
int page;
{
    u_long start, end, i;

    if (!pgmrecord)     /* No way Jose, if nothing's been recorded */
        return;

    if (page < 0 || page > (pgmpages - 1))       /* Check to see if in range */
        pg_err(CP_RANGE_ERR);
    else {
        end = pgmparr[page];
        if (page == 0)
            start = 0;
        else start = pgmparr[page-1] + 1;

        for (i = start; i <= end; i++)
            fputs(pgmlarr[i], out);
    }
}

/***************************************************************************
  PG_ERR() - PageIO module Error handler
 
            Arguments:  ERRCODE - error code.
              Returns:  nothing.                       
                                                                         
          This procedure prints an English-language error message on     
            the STDERR device, as dictated by the parameter Err.         
          If the error is a fatal one, pg_err() will prepare the process    
            for exitting and then exit.                                  
 **************************************************************************/

void pg_err(errcode)
int errcode;
{
    switch (errcode) {
        case CP_RANGE_ERR:
            fprintf(stderr, "\n?Nonexistant page\n");
            pg_close();
            break;

        case CP_SEEK_ERR:
            fprintf(stderr, "\n?Seek error during Pg_copy_page\n");
            pg_close();
            break;

        case SOURCE_OPEN_ERR:
            perror("\n?Error openning source file");
            break;

        case DEST_OPEN_ERR:
            perror("\n?Error openning output file");
            pg_close();
            break;

        case INS_OPEN_ERR:
            perror("\n?Error openning insert file");
            pg_close();
            break;

       case NO_MEMORY:
            fprintf(stderr, "\n?Pg_end_line: No memory\n");
            pg_close();
            break;

        case NO_LINES:
            fprintf(stderr, "\n?Pg_end_line: No room in pgmlarr[]\n");
            pg_close();
            break;

        case PAGE_OVERFLOW:
            fprintf(stderr, "\n?No room left in pgmparr[]\n");
            pg_close();
            break;
    }
    exit(1);
}

/***********************************************
  PG_PAGE_COUNT() - Returns the number of pages.
 ***********************************************/

int pg_page_count()
{
    return (pgmpages);
}


/****************************************************
  PG_PLAYBACK() - Set playback mode for recorded text
 ****************************************************/

int pg_playback()
{
    if (pgmrecord) {
        pgmsource = TRUE;
        return (pg_seek_page(0));
    }
    else return (FALSE);
}


/**********************************************************
  PG_SEEK_PAGE() - Seek to start of prerecorded page

      Arguments: PAGE - page number to seek to beginning of
        Returns: integer (boolean) success flag.

    This routine allows random access to prerecorded pages
    recorded by the module.
 **********************************************************/

int pg_seek_page(page)
int page;
{
    if (pgmsource && pgmrecord) {  /* may seek only in rec & plybck mode */
        if (page >= 0 && page < pgmpages) {
            pgmrppage = page;
            if (page == 0)
                pgmrpline = 0;
            else pgmrpline = pgmparr[page-1] + 1;

            if (pgmlines > 0)
                pgmrpchar = pgmlarr[pgmrpline];
            else pgmrpchar = NULL;
            return (TRUE);
        }
        else return (FALSE);
    }
    else return (FALSE);
}
