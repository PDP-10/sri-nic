/***************************************************************************
  REENAM.C - File renamer

  Todd Koumrian
  SRI International
  19-March-1989

  Usage:
    reenam  -format source_file [dest_dir]
         (where format is one of: d, m, y,  or  yd, ym, yy.

  Description:
    Basically, this program renames the indicated source_file according
    to the format switch provided.  The -d would rename a file foo.ext
    or just foo to foo.YYMMDD.  -m would rename to foo.YYMM, and -y
    renames to foo.YY

    The -yx switches provide the yester-? renamings, which are perhaps
    more useful (especially in those midnight batch jobs).  The TOPS-20
    internal clock is used, as well as Time and Date related JSYS's
    so the yester functions do always give the correct dates.

    A dest_dir is optional, and if provided, will be taken as the directory
    in which the renamed file is to written to.  If omitted, the currently
    connected directory will be the destination directory.
 ***************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <jsys.h>
#include <sys/param.h>

typedef enum {
   Day, Month, Year, YestDay, YestMonth, YestYear
} format_types;

void makename();

main(argc, argv)
int  argc;
char *argv[];
{
    format_types format;
    char *source, *dest;
    char newpath[MAXPATHLEN], newname[MAXPATHLEN];

    if (argc < 3 || argc >= 5 || argv[1][0] != '-') {
        fputs("Usage: reenam  -{y | m | d | yy | ym | yd} source [dest_dir]\n", stderr);
        exit(1);
    }

    switch (argv[1][1]) {
        case 'd': format = Day; break;
        case 'm': format = Month; break;
        case 'y':
            switch (argv[1][2]) {
                case '\0': format = Year; break;
                case 'd':  format = YestDay; break;
                case 'm':  format = YestMonth; break;
                case 'y': format = YestYear; break;
              default: fprintf(stderr, "Unknown format - \"%s\"\n", argv[1]);
                       exit(1);
                       break;
              }
              break;
                
      default: fprintf(stderr, "Unknown format - \"%s\"\n", argv[1]);
               exit(1);
               break;
    }

    source = argv[2];
    if (argc >= 4)
        strcpy(newpath, argv[3]);
    else newpath[0] = '\0';

    makename(format, source, newname);
    strcat(newpath, newname);
    printf("%s -> %s\n", source, newpath);
    if (rename(source, newpath) < 0) {
        perror(NULL);
        exit(1);
    }
}

void makename(form, src, new)
format_types form;
char *src;
char *new;
{
    char *c;
    int off;
    int ablock[5];
    char scratch[10];
    int yr, mth, dy;

    c = src;
    if ((off = strpos(c, ':')) >= 0)    /* Look for TOPS-20 device */
        c += (off + 1);                 /* Skip if present */
    if ((off = strpos(c, '<')) >= 0) {  /* Look for TOPS-20 directory */
        c += (off + 1);
        if ((off = strpos(c, '>')) < 0) {
            fprintf(stderr, "?Bad source pathname - %s\n", src);
            exit(1);
        }
        else c += (off + 1);            /* And skip it */
    }

    off = 0;
    while (c[off] != '\0' && c[off] != '.') {
        new[off] = c[off];
        off++;
    }
    new[off++] = '.';
    new[off] = '\0';

    ablock[2] = -1;
    ablock[4] = 0;
    if (!jsys(ODCNV, ablock)) {
        fputs("?ODCNV failed\n", stderr);
        exit(1);
    }

    if (form == YestDay) {
        if (!jsys(IDCNV, ablock)) {
           fputs("?IDCNV failed\n", stderr);
           exit(1);
        }
        ablock[2] -= (1<<18);
        if (!jsys(ODCNV, ablock)) {
            fputs("?ODCNV2 failed\n", stderr);
            exit(1);
        }
    }

    yr = (LH & ablock[2]) >>18;
    mth = RH & ablock[2] + 1;
    dy = ((LH & ablock[3]) >>18) + 1;

    if (form == YestMonth) {
        if (--mth <= 0) {
            mth = 12;
            yr--;
        }
    }

    if (form == YestYear)
        yr--;

    sprintf(scratch, "%d", yr);                 /* Append 2-digit year */
    strcat(new, scratch+2);

    if (form == Month || form == YestMonth ||
          form == Day || form == YestDay) {     /* Append 2-digit month */
        sprintf(scratch, "%02d", mth);
        strcat(new, scratch);
    }

    if (form == Day || form == YestDay) {       /* Append 2-digit date */
        sprintf(scratch, "%02d", dy);
        strcat(new, scratch);
    }
}
