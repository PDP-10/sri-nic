/* <SUBSYS.PCC>STDIO.H.19, 23-Nov-85 13:40:20, Edit by BEEBE */
/* Add definition of void type, since many C programs require it */

/* typedef int void;*/

#define	BUFSIZ	512
#define	_NFILE	20
#define FNMAX	100	/* max length of filename, for conversion routines */

# ifndef FILE
extern	struct	_iobuf {
	char	*_ptr;		/* current char */
	int	_cnt;		/* offset in file */
	char	*_base;		/* buffer start */
	int	_flag;		/* flags */
	int	_file;		/* jfn */
} _iob[_NFILE];

#define NJFN 96				/* max allowed by tops20 ??97 */

extern struct _fot {			/* indexed by jfn */
    int	_flg;	/* in this compiler, must be diff name from _iob._flag */
    int _tiw;				/* saved terminal intr word */
    int _unused2;			/* reserved */
}  _fotab [NJFN];

# endif

#define	FILE	struct _iobuf

#define	_IOREAD	01
#define	_IOWRT	02
#define	_IONBF	04
#define	_IOMYBUF 010
#define	_IOEOF	020
#define	_IOERR	040
#define	_IOSTRG	0100
#define _IOLBF  0200
#define	_IORW	0400

#define	_IOTTY	01000
#define _IOBINARY 02000

#define	NULL	0
#define	EOF	(-1)
#define EOS	'\0'	/* end of string */
#define OPENERR (-1)
#define TTYEOF	('\032')	/* ctrl-Z */

#define	stdin	(&_iob[0])
#define	stdout	(&_iob[1])
#define	stderr	(&_iob[2])

#define getc(p)		(--(p)->_cnt>=0? *(p)->_ptr++:_filbuf(p))
#define	getchar()	getc(stdin)
#define putc(x,p)     (--(p)->_cnt>=0?((int)(*(p)->_ptr++=(x))):_flsbuf((x),p))
#define	putchar(x)	putc(x,stdout)
#define	feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	fileno(p)	((p)->_file)

FILE	*fopen();
FILE	*freopen();
FILE	*fdopen();
long	ftell();
char	*fgets();

#define atol(p)	atoi(p)

	/* Try keep same flags as in _iob */
#ifdef COMMENT		/* these are defined elsewhere; must leave room */
# define ECHO	010
# define CRMOD	020
# define RAW	040
#endif

#define _FOEOF		001
#define _FODIR		002		/* is a directory */
#define _FOTTY	      01000		/* is a tty, use texti to read */
#define _FOBINARY     02000

#ifndef jfnof
# define jfnof(fd)	( (fd) <= 2 ? fileno(&_iob[fd]) : fd )
#endif

#define isatty(fd)	(_fotab[jfnof(fd)]._flg & _FOTTY)
