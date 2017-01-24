/***************************************************************************
  TEXTPS.C  - Rever Postscript header functions for text files

  Todd Koumrian
  SRI International
  12-Feb-1989

  Description:
    This file contains the functions to create Adobe format Header
    and Trailer comments and Postscript code as part of the Postscript
    encapsulation of a text file by the REVER program.

  Linking:
    Must be linked with PAGEIO.REL, and REVER of course.
 ****************************************************************************/

#include <stdio.h>
#include <time.h>
#include "pageio.h"
#include "textps.h"

char *getlogin();
int tabspc = TABSPACES;

void tp_head(title, font)
char *title;
char *font;
{
    char buf[100];
    long timeval;

    pg_put_string("%!PS-Adobe-\n");
    sprintf(buf, "%%%%Title: %s\n", title);
    pg_put_string(buf);
    pg_put_string("%%Creator: REVER by Todd Koumrian, SRI International 1989\n");
    sprintf(buf, "%%%%For: %s\n", getlogin());
    pg_put_string(buf);
    timeval = time((long *) NULL);
    sprintf(buf, "%%%%CreationDate: %s", ctime(&timeval));
    pg_put_string(buf);
    sprintf(buf, "%%%%DocumentFonts: /%s\n", font);
    pg_put_string(buf);
    pg_put_string("%%Pages: (atend)\n");
    pg_put_string("%%EndComments\n");
}

void tp_encap_panel(logpage)
int logpage;
{
    char lbuf[MAX_PLINE];
    char prbuf[MAX_PLINE];
    int c, nextc;
    int bkcnt, len, curstart;
    int spaces;
    int i;

    pg_put_string("STPN\n");

    pg_seek_page(logpage);
    len = curstart = 0;

    while ((c = pg_get_chr()) != EOF) {
crud:   switch (c) {
            case NEWLINE:
                if (len < MAX_PLINE)
                    lbuf[len] = NUL;
                else lbuf[MAX_PLINE - 1] = NUL;

                sprintf(prbuf, "(%s) PL\n", lbuf+curstart);
                pg_put_string(prbuf);
                len = curstart = 0;
                break;

            case '\t':
                spaces = tabspc - (len % tabspc);
                if ((len + spaces) < MAX_PLINE)
                    for (i = 1; i <= spaces; i++)
                        lbuf[len++] = ' ';
                break;

            case '(':
                if ((len + 2) < MAX_PLINE) {
                    lbuf[len++] = '\\';
                    lbuf[len++] = '(';
                }
                break;

            case ')':
                if ((len + 2) < MAX_PLINE) {
                    lbuf[len++] = '\\';
                    lbuf[len++] = ')';
                }
                break;

            case '\\':
                if ((len + 2) < MAX_PLINE) {
                    lbuf[len++] = '\\';
                    lbuf[len++] = '\\';
                }
                break;

            case BKSP:
                bkcnt = 1;
                while ((nextc = pg_get_chr()) == BKSP)
                    bkcnt++;
                lbuf[len] = NUL;
                sprintf(prbuf, "(%s) PR\n", lbuf+curstart);
                pg_put_string(prbuf);

                if (bkcnt > len)
                    bkcnt = len;
                curstart = len - bkcnt;
                sprintf(prbuf, "(%s) BK\n", lbuf+curstart);
                pg_put_string(prbuf);

                len = curstart;
                if (nextc != EOF) {
                    c = nextc;
                    goto crud;
                }
                break;

            default:
                if ((len + 1) < MAX_PLINE)
                    lbuf[len++] = c;
                break;
        }
    }

    pg_put_string("EOPN\n");
}

void tp_cut_page()
{
    pg_put_string("EOPG\n");
}

void tp_start_page(page)
int page;
{
    char buf[100];

    sprintf(buf, "%%%%Page: ? %d\n", page);
    pg_put_string(buf);
    pg_put_string("STPG\n");
}

void tp_set_tabspace(tbsp)
int tbsp;
{
    tabspc = tbsp;
}
