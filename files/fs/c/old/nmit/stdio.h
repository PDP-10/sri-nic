/*==================================================
 *  stdio.h
 *	#include file for stream I/O support for the V7 C library
 *
 *	Copyright (C) New Mexico Tech 1984, 1985.  All rights reserved.
 *
 *	Author: Greg Titus
 */

/*
 *  Has this file already been #included?  Matching #endif is at bottom of file
 */
#ifndef STDIO_INCLUDED

#define STDIO_INCLUDED

#ifndef TRUE
#   define TRUE	    1
#endif

#ifndef FALSE
#   define FALSE    0
#endif

#ifndef NULL
#   define NULL	    0
#endif

/*
 *  size of a page (in words and characters) and a standard buffer
 */
#ifndef PAGE_SIZE
#   define PAGE_SIZE   01000
#endif
#define PAGE_CSIZE  (PAGE_SIZE * sizeof(int))
#define BUFSIZ	    PAGE_CSIZE

/*
 *  the type of an I/O descriptor entry
 */
typedef struct
{
    int		    iob_count;		/* number of characters in buffer */
    char	    *iob_ptr;		/* current buffer pointer */
    char	    *iob_base;		/* base of buffer */
    int		    iob_flag;		/* flags word */
    int		    iob_fildes;		/* file descriptor for this stream */
} FILE;

/*
 *  maximum number of files
 */
#define	_NFILE	    20

/*
 *  the stream descriptor table (in clfdat)
 */
extern FILE	    _iob[];

/*
 *  the CRLF-handling buffer (in clfdat)
 */
extern int	    _____c;

/*
 *  the guaranteed-pushback buffers (in clfdat)
 */
extern char	    _pb_b[];

/*==================================================
 *  IOBF_xxx
 *	bits in iob_flag, meanings are:
 *
 *  name		meaning
 *  ----		-------
 *  IOBF_READ	    read access for file
 *  IOBF_WRITE	    write access for file
 *  IOBF_RW	    file is read/write
 *  IOBF_STRING	    file is really a string (sprintf, sscanf)
 *
 *  IOBF_EOF	    file was at end-of-file when last read
 *  IOBF_ERROR	    some error occurred on last i/o
 *  IOBF_PUSHED	    a character is in the guaranteed-pushback buffer
 *
 *  IOBF_UNBUF	    unbuffered (single-char buffering)
 *  IOBF_BIGBUF	    large (malloc'ed) buffer is present
 *  IOBF_LINEBUF    file is line-buffered
 *  IOBF_PMAP	    file is PMAP'ped
 *
 *  IOBF_ACCMASK    mask for access type
 *  IOBF_STTMASK    mask for status
 *  IOBF_BUFMASK    mask for buffering type
 */
#define	IOBF_READ	0000001
#define	IOBF_WRITE	0000002
#define	IOBF_RW		0000004
#define	IOBF_STRING	0000010

#define	IOBF_EOF	0000100
#define	IOBF_ERROR	0000200
#define IOBF_PUSHED	0000400

#define	IOBF_UNBUF	0010000
#define	IOBF_BIGBUF	0020000
#define	IOBF_LINEBUF	0040000
#define IOBF_PMAP	0100000

#define IOBF_ACCMASK	0000077
#define IOBF_STTMASK	0007700
#define IOBF_BUFMASK	0770000

#define	EOF	    (-1)

#define	stdin	    (&_iob[0])
#define	stdout	    (&_iob[1])
#define	stderr	    (&_iob[2])
#define	getc(p)	    ( (  (_____c= ((p)->iob_flag&IOBF_PUSHED)	\
				 ?( (p)->iob_flag&=~IOBF_PUSHED	\
				   ,_pb_b[(p)-_iob])		\
				 :((--(p)->iob_count>=0)	\
				   ?(*(p)->iob_ptr++&0177)	\
				   :_filbuf(p)))		\
		       =='\r')					\
		     ?( ((_____c=_fgetc(p))=='\n')		\
		       ?('\n')					\
		       :(_ungetc(_____c,(p)),'\r'))		\
		     :_____c)
#define	getchar()   getc(stdin)
#define putc(x,p)   ( (((x)=='\n')?fputc('\r',(p)):0)			\
		     ,( ((p)->iob_flag&IOBF_STRING||--(p)->iob_count>=0)\
		       ?((int)(*(p)->iob_ptr++=(int)(x)))		\
		       :_flsbuf((int)(x),(p))))
#define	putchar(x)  putc(x,stdout)
#define	feof(p)	    (((p)->iob_flag&IOBF_EOF)!=0)
#define	ferror(p)   (((p)->iob_flag&IOBF_ERROR)!=0)
#define	fileno(p)   ((p)->iob_fildes)

#endif	/* STDIO_INCLUDED */
