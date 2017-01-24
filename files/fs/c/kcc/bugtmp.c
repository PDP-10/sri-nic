10-Apr-90 17:09:05-PDT,35302;000000000005
Return-Path: <BEEBE@science.utah.edu>
Received: from science.utah.edu by NIC.DDN.MIL with TCP; Tue, 10 Apr 90 17:07:19 PDT
Date: Mon 9 Apr 90 17:35:13-MDT
From: "Nelson H.F. Beebe" <Beebe@science.utah.edu>
Subject: Another KCC-6 bug
To: KLH@NIC.DDN.MIL
cc: Beebe@science.utah.edu
In-Reply-To: <12580492201.19.KLH@NIC.DDN.MIL>
X-US-Mail: "Center for Scientific Computing, South Physics, University of Utah, Salt Lake City, UT 84112"
X-Telephone: (801) 581-5254
Message-ID: <12580548193.39.BEEBE@SCIENCE.utah.edu>

The attached file (produced by "kccx -DDVI -E option.c
>foo.c) produces these compile errors:

$kcc -c foo.c
KCC:    foo
<BEEBE.TEX.NDVI>FOO.PRE.2
<BEEBE.TEX.NDVI>FOO.FAI.2
FAIL:  foo
   $89  UNDEF   000026'
   $88  UNDEF   000023'
   $87  UNDEF   000020'
   $86  UNDEF   000015'
   $85  UNDEF   000012'
   $84  UNDEF   000007'
   $83  UNDEF   000004'
   $82  UNDEF   000001'
   $99  UNDEF   000064'
   $98  UNDEF   000061'
   $97  UNDEF   000056'
   $96  UNDEF   000053'
   $95  UNDEF   000050'
   $94  UNDEF   000045'
   $93  UNDEF   000042'
   $92  UNDEF   000037'
   $91  UNDEF   000034'
   $90  UNDEF   000031'

These symbols are referenced in the .fai file, but never
defined.  Here is foo.c:


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef char * caddr_t;
typedef int daddr_t;
typedef int dev_t;
typedef int ino_t;
typedef int off_t;
typedef int size_t;
typedef long time_t;
typedef struct _siostream FILE;
typedef long fpos_t;
extern FILE _sios[];
int remove(const char*);
int rename(const char*, const char*);
FILE *tmpfile(void);
char *tmpnam(char*);
int fclose(FILE*);
int fflush(FILE*);
FILE *fopen(const char*, const char*);
FILE *freopen(const char*, const char*, FILE*);
void setbuf(FILE*, char*);
int setvbuf(FILE*, char*, int, size_t);
int fprintf(FILE*, const char*, ...);
int fscanf(FILE*, const char*, ...);
int printf(const char*, ...);
int scanf(const char*, ...);
int sprintf(char *, const char*, ...);
int sscanf(const char*, const char*, ...);
int vfprintf(FILE*, const char*, int*);
int vprintf(const char *, int*);
int vsprintf(char*, const char*, int*);
int fgetc(FILE*);
char *fgets(char*, int, FILE*);
int fputc(int, FILE*);
int fputs(const char*, FILE*);
int getc(FILE*);
int getchar(void);
char *gets(char*);
int putc(int, FILE*);
int putchar(int);
int puts(const char*);
int ungetc(int, FILE*);
size_t fread(void*, size_t, size_t, FILE*);
size_t fwrite(const void*, size_t, size_t, FILE*);
int fgetpos(FILE*, fpos_t*);
int fseek(FILE*, long, int);
int fsetpos(FILE*, const fpos_t*);
long ftell(FILE*);
void rewind(FILE*);
void clearerr(FILE *);
int feof(FILE*);
int ferror(FILE*);
void perror(const char*);
FILE *fdopen(int, const char*);
FILE *sopen(const char*, const char*, size_t);
void _setbuf(FILE*, char*, size_t);
void setlinebuf(FILE*);
char *mktemp(char*);
int getw(FILE*);
int putw(int, FILE*);
int fileno(FILE*);
extern char *ctermid();
extern char *cuserid();
extern FILE *_FILE_head, *_makeFILE();
extern int _filbuf(), _readable(), _writeable(), _prime(), _sioflags();
extern void _cleanup(), _freeFILE(), _setFILE();
struct _siostream {
    int siocheck;
    int siocnt;
    char *siocp;
    char *siopbuf;
    int siolbuf;
    int sioflgs;
    int siofd;
    long siofdoff;
    int sioocnt;
    int sioerr;
    int sio2cnt;
    char *sio2cp;
    char siopbc[2];
    FILE *sionFILE;
    FILE *siopFILE;
    int siospare[4];
};
typedef int ptrdiff_t;
typedef char wchar_t;
extern int errno;
typedef int BOOLEAN;
typedef int BYTE;
typedef int COORDINATE;
typedef int INT16;
typedef long INT32;
typedef int INT8;
typedef unsigned int UNSIGN16;
typedef unsigned long UNSIGN32;
typedef void VOID;
typedef struct
{
    const char *name;
    int minmatch;
    int value;
} match_table;
extern int _ctyp1[];
extern int
	(isalnum)(int), (isalpha)(int), (isascii)(int), (iscntrl)(int),
	(iscsym)(int),
	(isdigit)(int), (isgraph)(int),
	(islower)(int), (isodigit)(int),(isprint)(int), (ispunct)(int),
	(isspace)(int), (isupper)(int), (isxdigit)(int),
	(toascii)(int), (toint)(int), (tolower)(int), (toupper)(int),
	(_tolower)(int), (_toupper)(int);
extern float _fltmax, _flteps, _fltmin;
extern double _dblmax, _dbleps, _dblmin;
extern long double _ldbmax, _ldbeps, _ldbmin;
extern double
	acos(double), asin(double), atan(double), atan2(double, double),
	ceil(double), cos(double), cosh(double), exp(double),
	fabs(double), floor(double), fmod(double, double),
	frexp(double, int *), ldexp(double, int), log(double),
	log10(double), modf(double, double *), pow(double, double),
	sin(double), sinh(double), sqrt(double), tan(double), tanh(double);
typedef long clock_t;
struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
	int tm_dummy[3];
};
clock_t clock(void);
double difftime(time_t, time_t);
time_t mktime(struct tm *);
time_t time(time_t *);
char *asctime(const struct tm *);
char *ctime(const time_t *);
struct tm *gmtime(const time_t *);
struct tm *localtime(const time_t *);
size_t strftime(char*, size_t,
		const char*, const struct tm *);
typedef struct { int quot; int rem; } div_t;
typedef struct { long quot; long rem; } ldiv_t;
double atof(const char *);
int atoi(const char *);
long atol(const char *);
double strtod(const char *, char **);
long strtol(const char *, char **, int);
unsigned long strtoul(const char *, char **, int);
int rand(void);
void srand(unsigned int);
void *calloc(size_t, size_t);
void free(void *);
void *malloc(size_t);
void *realloc(void *, size_t);
void abort(void);
int atexit(void (*)(void));
void exit(int);
char *getenv(const char *);
int system(const char *);
void *bsearch(const void *, const void *,
	size_t, size_t,
	int (*)(const void *, const void *));
void qsort(void *, size_t, size_t,
	int (*)(const void *, const void *));
int abs(int);
div_t div(int, int);
long labs(long);
ldiv_t ldiv(long, long);
int mblen(const char *, size_t);
int mbtowc(wchar_t *, const char *, size_t);
int wctomb(char *, wchar_t);
size_t mbstowcs(wchar_t *, const char *, size_t);
size_t wcstombs(char *, const wchar_t *, size_t);
extern int _n_exit_func;
extern void (*_exit_func[])(void);
void *memcpy(void*, const void*, size_t);
void *memmove(void*, const void*, size_t);
char *strcpy(char*, const char*);
char *strncpy(char*, const char*, size_t);
char *strcat(char*, const char*);
char *strncat(char*, const char*, size_t);
int memcmp(const void*, const void*, size_t);
int strcmp(const char*, const char*);
int strcoll(const char*, const char*);
int strncmp(const char*, const char*, size_t);
size_t strxfrm(char*, const char*, size_t);
void *memchr(const void*, int, size_t);
char *strchr(const char*, int);
size_t strcspn(const char*, const char*);
char *strpbrk(const char*, const char*);
char *strrchr(const char*, int);
size_t strspn(const char*, const char*);
char *strstr(const char*, const char*);
char *strtok(char *, const char*);
void *memset(void*, int, size_t);
char *strerror(int);
size_t strlen(const char*);
char *index(const char*, int);
char *rindex(const char*, int);
int strpos(const char*, int);
int strrpos(const char*, int);
char *strrpbrk(const char*, const char*);
void *memccpy(void*, const void*, int, size_t);
struct stat
{
    dev_t st_dev;
    ino_t st_ino;
    unsigned st_mode;
    int st_nlink;
    int st_uid;
    int st_gid;
    dev_t st_rdev;
    off_t st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
    long st_blksize;
    long st_blocks;
    long st_spares[4];
};
struct xstat {
    struct stat st;
    union {
	struct {
	    int state;
	    int fhost;
	    int fport;
	} tcp;
	struct {
	    int version;
	    int pagcnt;
	    int bytsiz;
	} disk;
    } dev_dep;
};
struct char_entry
{
    long fontrp;
    INT32 dx;
    INT32 dy;
    UNSIGN32 *rasters;
    UNSIGN32 tfmw;
    int devfont;
    int devchar;
    COORDINATE hp;
    COORDINATE wp;
    COORDINATE xoffp;
    COORDINATE yoffp;
    UNSIGN16 pxlw;
    INT16 refcount;
    BOOLEAN isknown;
    BOOLEAN isloaded;
    BOOLEAN istoobig;
};
struct font_entry
{
    struct font_entry *next;
    char *fontname;
    char *filename;
    FILE *font_file_id;
    int (*charxx)();
    UNSIGN32 checksum;
    UNSIGN32 design_size;
    UNSIGN32 designsize;
    UNSIGN32 font_mag;
    INT32 font_space;
    UNSIGN32 hppp;
    UNSIGN32 magnification;
    INT32 max_m;
    INT32 max_n;
    INT32 min_m;
    INT32 min_n;
    UNSIGN32 scale_factor;
    UNSIGN32 storage;
    INT32 tex_font_number;
    INT32 ticks;
    UNSIGN32 vppp;
    struct char_entry ch[(255 + 1 - 0)];
    COORDINATE cell_d;
    COORDINATE cell_h;
    COORDINATE cell_w;
    UNSIGN16 font_number;
    UNSIGN16 nloaded;
    BOOLEAN substitute;
    BYTE len_fontarea;
    BYTE len_fontname;
};
extern int maxstdmag;
extern INT32 offset_stdmag[];
extern UNSIGN32 stdmag[];
extern BOOLEAN backwards;
extern double bb_llx;
extern double bb_lly;
extern double bb_urx;
extern double bb_ury;
extern UNSIGN32 **bitmap;
extern long bytelimit;
extern int byte_size;
extern INT32 cache_size;
extern UNSIGN32 cmdmag;
extern double conv;
extern UNSIGN16 copies;
extern char curext[512+1];
extern char curname[512+1];
extern char curpath[512+1];
extern INT16 cur_page_number;
extern char dbgname[512+1];
extern UNSIGN16 debug_code;
extern UNSIGN32 den;
extern UNSIGN32 devmag;
extern int dev_bitmap;
extern const char *dev_id;
extern int dev_model;
extern int dev_postscript;
extern char dvifonts[257];
extern FILE *dvifp;
extern char dvihelp[257];
extern char dviinputs[257];
extern UNSIGN32 dvimag;
extern char dviname[512+1];
extern const char *dviprefix;
extern char dvoname[512+1];
extern int eop_status;
extern const char *errsuffix;
extern char fontfmt[257];
extern FILE *fontfp;
extern char fontmags[257];
extern struct font_entry *fontptr;
extern UNSIGN16 font_count;
extern struct font_entry *font_table[32];
extern BOOLEAN font_switched;
extern char *funstr[64];
extern UNSIGN32 gpower[33];
extern char **g_argv;
extern int g_argc;
extern int g_errenc;
extern BOOLEAN g_dolog;
extern FILE *g_logfp;
extern char g_logname[257];
extern char g_progname[257];
extern INT32 g_ticks;
extern INT32 h;
extern BOOLEAN have_outname;
extern BOOLEAN heavyduty;
extern struct font_entry *hfontptr;
extern COORDINATE hh;
extern UNSIGN16 img_words;
extern UNSIGN32 img_mask[32];
extern UNSIGN32 img_row[(1500 - -500 + 1 + 31) >> 5];
extern int interactive;
extern int keybinding[256];
extern BOOLEAN landscape;
extern double leftmargin;
extern COORDINATE lmargin;
extern double magstep;
extern UNSIGN32 mag;
extern BOOLEAN manfeed;
extern UNSIGN16 maxdrift;
extern int maxofn;
extern int maxpage;
extern INT16 max_m;
extern INT16 max_n;
extern int max_stacksize;
extern UNSIGN16 merit_fact;
extern char message[1024];
extern match_table model_table[];
extern INT16 min_m;
extern INT16 min_n;
extern INT16 nopen;
extern INT16 npage;
extern UNSIGN32 num;
extern const char *outext;
extern const char *outsuffix;
extern INT16 page_count;
extern INT16 page_fonts;
extern INT16 page_begin[256+1];
extern INT16 page_end[256+1];
extern long *page_loc;
extern INT32 *page_tex;
extern char *page_option[256+1];
extern long *page_ptr;
extern INT16 page_step[256+1];
extern int paperfeeding;
extern int paint_mode;
extern struct font_entry *pfontptr;
extern double pixround_temp;
extern FILE *plotfp;
extern UNSIGN32 power[32];
extern BOOLEAN preload;
extern UNSIGN32 ps_reload;
extern int p_index;
extern COORDINATE p_xdots;
extern double p_xin;
extern COORDINATE p_xleft;
extern COORDINATE p_xright;
extern COORDINATE p_xwords;
extern COORDINATE p_ybottom;
extern COORDINATE p_ydots;
extern double p_yin;
extern COORDINATE p_ytop;
extern BOOLEAN quiet;
extern INT16 refpt;
extern BOOLEAN reversevideo;
extern UNSIGN32 rightones[36];
extern int rnd_xmult;
extern int rnd_ymult;
extern COORDINATE rule_height;
extern COORDINATE rule_width;
extern BOOLEAN runlengthcode;
extern UNSIGN32 runmag;
extern UNSIGN16 size_limit;
extern BOOLEAN spool_output;
extern FILE *stddbg;
extern const char *stdform[]
;
extern int maxfrm;
extern UNSIGN32 stor_fonts;
extern UNSIGN32 stor_limit;
extern COORDINATE str_ycp;
extern char subname[512+1];
extern char subext[512+1];
extern char subfile[512+1];
extern char syspath[512+1];
extern COORDINATE s_bot;
extern COORDINATE s_height;
extern COORDINATE s_top;
extern char tempout[257+1];
extern char texfonts[512+1];
extern char texinputs[512+1];
extern INT32 tex_counter[10];
extern COORDINATE tmargin;
extern double topmargin;
extern INT32 v;
extern const char *const version_no;
extern int vert_step;
extern COORDINATE vv;
extern BOOLEAN xclip;
extern COORDINATE xcp;
extern int xdpi;
extern COORDINATE xscreen;
extern COORDINATE xsize;
extern int xwords;
extern BOOLEAN yclip;
extern COORDINATE ycp;
extern int ydpi;
extern COORDINATE ysize;
extern COORDINATE yscreen;
typedef struct
{
    const char* s_name;
    int s_length;
} const_string;
typedef struct
{
    char* s_name;
    int s_length;
} string;
typedef union
{
    double d;
    string s;
} symbol_value;
typedef struct
{
    const_string se_name;
    symbol_value* se_value;
    int se_type;
} symbol_entry;
typedef struct
{
	double output_order;
	double x_size;
	double y_size;
	double x_origin;
	double y_origin;
	double x_left;
	double x_right;
	double y_top;
	double y_bottom;
	double x_clip;
	double y_clip;
	string paper_name;
	string use_name;
	string page_initialization;
	string page_termination;
	string device_initialization;
	string device_termination;
} form_record;
extern int max_paper_table;
extern form_record* paper_table;
void initstab (symbol_entry *st,int nst);
int paper (symbol_entry *st,int nst,const char *pgm);
void prtsent (FILE *fp,symbol_entry *se,
 BOOLEAN showunset);
void prtstab (FILE *fp,symbol_entry *st,int n,
 BOOLEAN showunset);
void prtstr (FILE *fp,const_string *s);
void abortrun (int);
double actfact (UNSIGN32);
void alldone (void);
void bopact (void);
int chargf (int,void(*) (int,UNSIGN16));
int charpk (int,void(*) (int,UNSIGN16));
int charpxl (int,void(*) (int,UNSIGN16));
void clrbmap (void);
void clrrow (void);
char *cuserid (char *);
void dbgbmap (void);
void dbgclose (FILE*,const char*);
void dbgopen (FILE*, const char*, const char*);
void devinit (void);
void devterm (void);
void dispchar (int);
void disploop (void);
void dobbox (const string *s);
void dographics (const string *s);
void doinclude (const string *s);
void doliteral (const string *s);
void domessage (const string *s);
void dooptions (const string *s);
void dooverlay (const string *s);
void doposition (const string *s);
void dvifile (const char *);
void dviinit (const char *);
void dviterm (void);
void eopact (void);
void eopeach (void);
void eopinit (void);
void eopterm (void);
FILE *f20open (const char *filename,const char *mode);
void fatal (const char *);
int fileargs (const char *filename,int *pargc,char*** pargv);
void fileloop (void);
void fillrect (COORDINATE, COORDINATE, COORDINATE,
 COORDINATE);
char *findfile (const char *pathlist,const char *name);
void findpost (void);
COORDINATE fixpos (COORDINATE, INT32, double);
char *fontfile (const char *,const char *,UNSIGN32);
BOOLEAN fontsub (char *,UNSIGN32 *,const char *,UNSIGN32);
void getbmap (void);
void getbytes (FILE *, char *, int);
void getfntdf (void);
void getpgtab (long);
void help (void);
double inch (const char *);
void initpost (void);
void initfile (const char *filename);
void initglob (void);
void initpre (void);
void initterm (void);
int kbstring (char *s,int n);
void loadchar (int);
int main (int,char *[]);
int match (const char *user_string,const char *keyword);
void movedown (INT32);
void moveover (INT32);
void moveto (COORDINATE, COORDINATE);
void newfont (void);
UNSIGN32 nosignex (FILE *,int);
void openfont (const char *);
void option (const char *);
void outrow (int,UNSIGN16);
void outraster (int,UNSIGN16);
void prtbmap (void);
int prtcomm (void);
void prtinit (long bytepos);
void prtpage (long);
void readfont (INT32);
int readgf (void);
int readpk (void);
void readpost (void);
int readpxl (void);
void reldfont (struct font_entry *);
void rsetterm (void);
COORDINATE rulepxl (UNSIGN32,double);
BOOLEAN search (long bytepos,const char* s);
BOOLEAN selpage (INT16 page_number);
void setchar (int,BOOLEAN);
void setfntnm (INT32);
void setkeys (const char *s);
void setrule (UNSIGN32, UNSIGN32, BOOLEAN);
void setstr (int);
void showname (int key);
INT32 signex (FILE *,int);
void skipfont (INT32);
void skgfspec (void);
void skpkspec (void);
void special (const char *s,INT32 n);
char *strdup (const char * s);
int stricmp (const char *,const char *);
char *stristr (const char *,const char *);
int strnicmp (const char *s1,const char *s2,size_t n);
char *tctos (void);
INT16 trimstrip (int);
void unloadfonts (void);
void usage (FILE *);
void warning (const char *);
void* xcalloc (size_t nmemb, size_t size);
void xfree (void* p);
void* xmalloc (size_t size);
unsigned long xmemused (void);
void* xrealloc (void* block, size_t size);
int findform (const char *formname);
void initform (form_record *ppt, const char *name);
void prtform (FILE *fp,form_record *spt);
int saveform (const char *program);
static void doinifile (const char *the_filename,
 int nest_level);
static void error (const char *errmsg,const char *optstr);
static int lookup (const match_table *table,
 const char *value);
static void opt_a (const char *optstr);
static void opt_b (const char *optstr);
static void opt_c (const char *optstr);
static void opt_d (const char *optstr);
static void opt_e (const char *optstr);
static void opt_f (const char *optstr);
static void opt_h (const char *optstr);
static void opt_i (const char *optstr);
static void opt_k (const char *optstr);
static void opt_l (const char *optstr);
static void opt_m (const char *optstr);
static void opt_o (const char *optstr);
static void opt_p (const char *optstr);
static void opt_q (const char *optstr);
static void opt_query (const char *optstr);
static void opt_r (const char *optstr);
static void opt_s (const char *optstr);
static void opt_v (const char *optstr);
static void opt_x (const char *optstr);
static void opt_y (const char *optstr);
static const char *skipsep (const char *s);
static int init_nest_level = 0;
static int m;
static const char *org_optstr;
void
option(optstr)
const char *optstr;
{
    if (*optstr == '@')
    {
	doinifile(optstr+1,init_nest_level);
	return;
    }
    else if (*optstr != '-')
	return;
    org_optstr = optstr;
    switch (optstr[1])
    {
    case '?':
	opt_query(optstr);
	break;
    case 'a':
    case 'A':
	opt_a(optstr);
	break;
    case 'b':
    case 'B':
	opt_b(optstr);
	break;
    case 'c':
    case 'C':
	opt_c(optstr);
	break;
    case 'd':
    case 'D':
	opt_d(optstr);
	break;
    case 'e':
    case 'E':
	opt_e(optstr);
	break;
    case 'f':
    case 'F':
	opt_f(optstr);
	break;
    case 'h':
    case 'H':
	opt_h(optstr);
	break;
    case 'i':
    case 'I':
	opt_i(optstr);
	break;
    case 'k':
    case 'K':
	opt_k(optstr);
	break;
    case 'l':
    case 'L':
	opt_l(optstr);
	break;
    case 'm':
    case 'M':
	opt_m(optstr);
	break;
    case 'o':
    case 'O':
	opt_o(optstr);
	break;
    case 'p':
    case 'P':
	opt_p(optstr);
	break;
    case 'q':
    case 'Q':
	opt_q(optstr);
	break;
    case 'r':
    case 'R':
	opt_r(optstr);
	break;
    case 's':
    case 'S':
	opt_s(optstr);
	break;
    case 'v':
    case 'V':
	opt_v(optstr);
	break;
    case 'x':
    case 'X':
	opt_x(optstr);
	break;
    case 'y':
    case 'Y':
	opt_y(optstr);
	break;
    default:
	error((char*)0,optstr);
    }
}
static void
doinifile(the_filename,nest_level)
const char *the_filename;
int nest_level;
{
    char *tcp;
    tcp = findfile((char*)0,the_filename);
    if (tcp == (char*)0)
	tcp = findfile(dviinputs,the_filename);
    if (tcp == (char*)0)
	tcp = findfile(texinputs,the_filename);
    if (tcp != (char*)0)
    {
	nest_level++;
	if (nest_level < 10)
	    initfile(tcp);
	else
	{
	    (void)sprintf(message,
		"option():  More than %d nested option files.  File [%s] will not be processed.",10,tcp);
	    warning(message);
	}
	nest_level--;
    }
    else
    {
	(void)sprintf(message,"option():  Cannot find option file [%s]",
	    the_filename);
	fatal(message);
    }
}
static void
error(errmsg,optstr)
const char *errmsg;
const char *optstr;
{
    usage((&_sios[2]));
    (void)sprintf(message,"option():  [%s] is not a valid switch.",optstr);
    if (errmsg != (char*)0)
    {
	(void)strcat(message,"\n");
	(void)strcat(message,errmsg);
    }
    fatal(message);
}
static int
lookup(table,the_name)
const match_table *table;
const char *the_name;
{
    register int k;
    for (k = 0; table[k].name != (const char*)0; ++k)
    {
	if (match(table[k].name,the_name) >= table[k].minmatch)
	    return (k);
    }
    usage((&_sios[2]));
    (void)sprintf(message,
	"option():  Bad switch value in [%s]---expected one of these:",
	org_optstr);
    for (k = 0; table[k].name != (const char*)0; ++k)
    {
	(void)strcat(message,"\n\t");
	(void)strcat(message,table[k].name);
    }
    fatal(message);
    return (0);
}
static void
opt_a(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"author")) > 0)
    {
	(void)fprintf((&_sios[2]),"Author:\n");
	(void)fprintf((&_sios[2]),"\tNelson H. F. Beebe\n");
	(void)fprintf((&_sios[2]),"\tCenter for Scientific Computing\n");
	(void)fprintf((&_sios[2]),"\tand Department of Mathematics\n");
	(void)fprintf((&_sios[2]),"\tSouth Physics Building\n");
	(void)fprintf((&_sios[2]),"\tUniversity of Utah\n");
	(void)fprintf((&_sios[2]),"\tSalt Lake City, UT 84112\n");
	(void)fprintf((&_sios[2]),"\tUSA\n");
	(void)fprintf((&_sios[2]),"\tTel: (801) 581-5254\n");
	(void)fprintf((&_sios[2]),"\tFAX: (801) 581-4801\n");
	(void)fprintf((&_sios[2]),"\tEmail: <Beebe@science.utah.edu>\n");
    }
    else
	error("Expected -author",optstr);
}
static void
opt_b(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"backwards")) > 1)
	backwards = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!backwards));
    else if ((m = match(optstr+1,"bytelimit")) > 1)
	bytelimit = strtol(skipsep(optstr+1+m),(char**)0,0);
    else
	error("Expected -backwards or -bytelimit:#",optstr);
}
static void
opt_c(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"copies")) > 0)
    {
	copies = (UNSIGN16)strtol(skipsep(optstr+1+m),(char**)0,0);
	copies = ((1) > (((copies) < (256) ? (copies) : (256))) ? (1) : (((copies) < (256) ? (copies) : (256))));
    }
    else
	error("Expected -copies:#",optstr);
}
static void
opt_d(optstr)
const char *optstr;
{
    static match_table debug_table[] =
    {
	{"bitmap", 1, 0x0200},
	{"char-dump", 6, 0x0002},
	{"char-off-page", 6, 0x0020},
	{"char-position", 6, 0x0004},
	{"font-cache", 6, 0x0040},
	{"font-name", 6, 0x1000},
	{"fonts", 3, 0x1000},
	{"form-default", 6, 0x0400},
	{"forms", 5, 0x0800},
	{"malloc", 2, 0x0100},
	{"memory-allocation", 2, 0x0100},
	{"open", 1, 0x0008 | 0x0010},
	{"open-fail", 6, 0x0010},
	{"open-okay", 6, 0x0008},
	{"page-dump", 1, 0x0200},
	{"set-text", 2, 0x0080},
	{"stdout", 2, 0x0001},
        {"sun", 2, 0x2000},
	{(char*)0, 0, 0},
    };
    const char *the_value;
    if ((m = match(optstr+1,"debugfile")) > 1)
	(void)strcpy(dbgname,skipsep(optstr+1+m));
    else if ((m = match(optstr+1,"dviprefix")) > 1)
	dviprefix = strdup(skipsep(optstr+1+m));
    else if ((m = match(optstr+1,"d")) > 0)
    {
	the_value = skipsep(optstr+1+m);
	if (((_ctyp1+1)[*the_value]&(04)))
	    debug_code |= (UNSIGN16)strtol(the_value,(char**)0,0);
	else
	    debug_code |=
		(UNSIGN16)debug_table[lookup(debug_table,the_value)].value;
    }
    else
	error("Expected -d:# or -d:selection or -debugfile:filename or -dviprefix:name",optstr);
}
static void
opt_e(optstr)
const char *optstr;
{
    register int k;
    size_t len;
    register const char *p;
    register const char *q;
    typedef struct
    {
	const char *envname;
	char *envvar;
    } envstruct;
    static envstruct envlist[] =
    {
	"DVIFONTS", dvifonts,
	"DVIHELP", dvihelp,
	"DVIINPUTS", dviinputs,
	"FONTFMT", fontfmt,
	"FONTMAGS", fontmags,
	"TEXFONTS", texfonts,
	"TEXINPUTS", texinputs,
    };
    p = strrchr(optstr,'=');
    if (p == (char*)0)
    {
	if ((m = match(optstr+1,"epsf")) > 1)
	    ps_reload = (UNSIGN32)1L;
	else if ((m = match(optstr+1,"errsuffix")) > 3)
	    errsuffix = strdup(skipsep(optstr+1+m));
	else
	    error("Expected -epsf or -errsuffix:name or -eENVVAR=value",optstr);
    }
    else
    {
	m = 1;
	q = skipsep(optstr+1+m);
	len = (size_t)(p - q);
	for (k = 0; k < sizeof(envlist)/sizeof(envlist[0]); ++k)
	{
	    if (strnicmp(q,envlist[k].envname,len) == 0)
	    {
		(void)strncpy(envlist[k].envvar,p+1,257-1);
		envlist[k].envvar[257-1] = '\0';
		return;
	    }
	}
	usage((&_sios[2]));
	(void)sprintf(message,
	    "option():  Bad switch [%s]---expected one of:",optstr);
	for (k = 0; k < sizeof(envlist)/sizeof(envlist[0]); ++k)
	{
	    (void)strcat(message,"\n\t");
	    (void)strcat(message,envlist[k].envname);
	}
	fatal(message);
    }
}
static void
opt_f(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"fontsubfile")) > 0)
	(void)strcpy(subfile,skipsep(optstr+1+m));
    else
	error("Expected -fontsubfile:filename",optstr);
}
static void
opt_h(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"heavyduty")) > 2)
	heavyduty = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!heavyduty));
    else if ((m = match(optstr+1,"help")) > 2)
    {
	usage((&_sios[2]));
	alldone();
    }
    else
	error("Expected -heavyduty or -help",optstr);
}
static void
opt_i(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"inifile")) > 0)
	doinifile(skipsep(optstr+1+m),init_nest_level);
    else
	error("Expected -inifile:filename",optstr);
}
static void
opt_k(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"keybindings")) > 0)
	setkeys(skipsep(optstr+1+m));
    else
	error("Expected -keybindings:{...}",optstr);
}
static void
opt_l(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"landscape")) > 1)
	landscape = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!landscape));
    else if ((m = match(optstr+1,"logging")) > 1)
	g_dolog = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!g_dolog));
    else
	error("Expected -logging or -landscape",optstr);
}
static void
opt_m(optstr)
const char *optstr;
{
    if (((m = match(optstr+1,"m")) > 0) || ((m = match(optstr+1,"magnification")) > 3))
    {
	cmdmag = (UNSIGN32)(0.5 + (double)atof(skipsep(optstr+1+m)));
	dvimag = cmdmag;
	runmag = ((magstep == 0.0) ? dvimag : (UNSIGN32)(0.5 + pow(1.2,magstep)*(double)dvimag));
    }
    else if ((m = match(optstr+1,"magstep")) > 3)
    {
	magstep = (double)atof(skipsep(optstr+1+m));
	if ((-15.0 <= magstep) && (magstep <= 15.0))
	    runmag = ((magstep == 0.0) ? dvimag : (UNSIGN32)(0.5 + pow(1.2,magstep)*(double)dvimag));
	else
	    error("-magstep value must be in range -15..+15",optstr);
    }
    else if ((m = match(optstr+1,"manualfeed")) > 2)
	manfeed = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!manfeed));
    else if ((m = match(optstr+1,"maxdownloadedfonts")) > 3)
	stor_fonts = (UNSIGN32)strtol(skipsep(optstr+1+m),(char**)0,0);
    else if ((m = match(optstr+1,"maxdrift")) > 3)
	maxdrift = (UNSIGN16)strtol(skipsep(optstr+1+m),(char**)0,0);
    else if ((m = match(optstr+1,"maxfontstorage")) > 3)
	stor_limit = (UNSIGN32)strtol(skipsep(optstr+1+m),(char**)0,0);
    else if ((m = match(optstr+1,"maxopenfonts")) > 3)
    {
	maxofn = (UNSIGN16)strtol(skipsep(optstr+1+m),(char**)0,0);
	maxofn = ((maxofn) < (32) ? (maxofn) : (32));
	maxofn = ((1) > (maxofn) ? (1) : (maxofn));
    }
    else if ((m = match(optstr+1,"meritfactor")) > 1)
	merit_fact = (UNSIGN16)strtol(skipsep(optstr+1+m),(char**)0,0);
    else if ((m = match(optstr+1,"model")) > 1)
	dev_model = model_table[lookup(model_table,skipsep(optstr+1+m))].value;
    else
	error("Expected -m:# or -magnification:# or -magstep or\n\t-manualfeed or -maxdownloadedfonts:# or -maxdrift:# or\n\t-maxfontstorage:# or -maxopenfonts:# or -model:name",optstr);
}
static void
opt_o(optstr)
const char *optstr;
{
    register const char *p;
    long p1;
    long p2;
    long p3;
    long p4;
    int value;
    if ((m = match(optstr+1,"outfile")) > 3)
    {
	(void)strcpy(dvoname,skipsep(optstr+1+m));
	have_outname = ((BOOLEAN)1);
    }
    else if ((m = match(optstr+1,"outsuffix")) > 3)
	outsuffix = strdup(skipsep(optstr+1+m));
    else if ((m = match(optstr+1,"o")) > 0)
    {
	p1 = p2 = p3 = p4 = 0L;
	p = skipsep(optstr+1+m);
	if (strchr((const char*)p,'.') != (char*)0)
	{
	    page_begin[npage] = 1;
	    page_end[npage] = 32767;
	    page_option[npage] = strdup(p);
	}
	else
	{
	    value = (int)sscanf(p,"%ld:%ld:%ld:%ld",&p1,&p2,&p3,&p4);
	    p1 = ((-32767L) > (((p1) < (32767L) ? (p1) : (32767L))) ? (-32767L) : (((p1) < (32767L) ? (p1) : (32767L))));
	    p2 = ((-32767L) > (((p2) < (32767L) ? (p2) : (32767L))) ? (-32767L) : (((p2) < (32767L) ? (p2) : (32767L))));
	    p3 = ((-32767L) > (((p3) < (32767L) ? (p3) : (32767L))) ? (-32767L) : (((p3) < (32767L) ? (p3) : (32767L))));
	    p4 = ((-32767L) > (((p4) < (32767L) ? (p4) : (32767L))) ? (-32767L) : (((p4) < (32767L) ? (p4) : (32767L))));
	    switch (value)
	    {
	    case 1:
		page_begin[npage] = page_end[npage] = (INT16)p1;
		page_step[npage] = 1;
		break;
	    case 2:
		page_begin[npage] = (INT16)p1;
		page_end[npage] = (INT16)p2;
		page_step[npage] = 1;
		break;
	    case 3:
		page_begin[npage] = (INT16)p1;
		page_end[npage] = (INT16)p2;
		page_step[npage] = (((INT16)p3) < 0 ? -((INT16)p3) : ((INT16)p3));
		if (page_step[npage] == 0)
		    page_step[npage] = 1;
		break;
	    case 4:
		page_begin[npage] = (INT16)p1 + (INT16)p4;
		page_end[npage] = (INT16)p2 + (INT16)p4;
		page_step[npage] = (((INT16)p3) < 0 ? -((INT16)p3) : ((INT16)p3));
		if (page_step[npage] == 0)
		    page_step[npage] = 1;
		break;
	    default:
		error("Expected -o#, -o#:#, -o#:#:#, or -o#:#:#:#",optstr);
	    }
	    page_option[npage] = (char*)0;
	}
	npage = ((npage+1) < (256-1) ? (npage+1) : (256-1));
    }
    else
	error("Expected -o or -outfile:filename or -outsuffix:name",optstr);
}
static void
opt_p(optstr)
const char *optstr;
{
    register const char *p;
    if ((m = match(optstr+1,"paintmode")) > 2)
    {
	stor_limit -= 970 * 1024L;
	stor_fonts = stor_limit;
	paint_mode = 2;
    }
    else if ((m = match(optstr+1,"paper")) > 2)
    {
	p = skipsep(optstr+1+m);
	if (*p == '{')
	{
	    if (saveform(p) != 0)
		error("Error parsing paper specifications",optstr);
	}
	else
	{
	    p_index = findform(p);
	    if ( (paper_table[p_index].x_size < 0.01) ||
		(paper_table[p_index].y_size < 0.01) )
		error("Unknown paper form name specified",optstr);
	}
    }
    else if ((m = match(optstr+1,"preload")) > 1)
	preload = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!preload));
    else
	error("Expected -paintmode or -paper:name or -paper:{program} or -preload",optstr);
}
static void
opt_q(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"quiet")) > 0)
	quiet = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!quiet));
    else
	error("Expected -quiet",optstr);
}
static void
opt_query(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"?")) > 0)
    {
	usage((&_sios[2]));
	alldone();
    }
    else
	error("Expected -?",optstr);
}
static void
opt_r(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"reloadafter")) > 2)
	ps_reload = (UNSIGN32)strtol(skipsep(optstr+1+m),(char**)0,0);
    else if ((m = match(optstr+1,"reversevideo")) > 2)
	reversevideo = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!reversevideo));
    else if ((m = match(optstr+1,"runlengthcode")) > 1)
	runlengthcode = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!runlengthcode));
    else
	error("Expected -reloadafter:# or -reversevideo or -runlengthcode",optstr);
}
static void
opt_s(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"sizelimit")) > 1)
	size_limit = (UNSIGN16)strtol(skipsep(optstr+1+m),(char**)0,0);
    else if ((m = match(optstr+1,"spooloutput")) > 1)
	spool_output = (optstr[1+m] ? (BOOLEAN)(strtol(skipsep(optstr+1+m),(char**)0,0) != 0) : (BOOLEAN)(!spool_output));
    else
	error("Expected -sizelimit:# or -spooloutput",optstr);
}
static void
opt_v(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"version")) > 0)
    {
	(void)fprintf((&_sios[2]),"[TeX DVI Translator Version %s]",version_no);
	{(void)(--((&_sios[2]))->siocnt >= 0 ? (*++((&_sios[2]))->siocp = ((char)'\r')) : fputc(((char)'\r'), ((&_sios[2]))));(void)(--((&_sios[2]))->siocnt >= 0 ? (*++((&_sios[2]))->siocp = ((char)'\n')) : fputc(((char)'\n'), ((&_sios[2]))));};
	(void)fprintf((&_sios[2]),"[%s]",dev_id);
	{(void)(--((&_sios[2]))->siocnt >= 0 ? (*++((&_sios[2]))->siocp = ((char)'\r')) : fputc(((char)'\r'), ((&_sios[2]))));(void)(--((&_sios[2]))->siocnt >= 0 ? (*++((&_sios[2]))->siocp = ((char)'\n')) : fputc(((char)'\n'), ((&_sios[2]))));};
    }
    else if ((m = match(optstr+1,"virtualfonts")) > 1)
    {
    }
    else
	error("Expected -version or -virtualfonts",optstr);
}
static void
opt_x(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"x")) > 0)
	leftmargin = inch(skipsep(optstr+1+m));
    else
	error("Expected -x:#",optstr);
}
static void
opt_y(optstr)
const char *optstr;
{
    if ((m = match(optstr+1,"y")) > 0)
	topmargin = inch(skipsep(optstr+1+m));
    else
	error("Expected -y:#",optstr);
}
static const char*
skipsep(s)
register const char *s;
{
    while (*s && ((_ctyp1+1)[*s]&(0100)))
	++s;
    if ((*s == ':') || (*s == '='))
	++s;
    while (*s && ((_ctyp1+1)[*s]&(0100)))
	++s;
    return (s);
}
-------
