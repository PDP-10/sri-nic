/* -------------------------------------------- */
/*	stdio.h		(TOPS-20)		*/
/*						*/
/*	reference:				*/
/*	  Kernighan,B.W., D.M.Ritchie,		*/
/*	  The C Programming Language		*/
/*	  pp. 165				*/
/* -------------------------------------------- */

#define	_BUFSIZE	512
#define	_NFILE		10

typedef struct	_iobuf {
		char *_ptr ;	/* next character position   */
		int  _cnt ;	/* number of characters left */
		char *_base ;	/* location of buffer	     */
		int  _flag ;	/* file access mode	     */
		int  _fd ;	/* file descriptor	     */
	} FILE ;


extern	FILE	_iob[_NFILE],
		*stdin,
		*stdout,
		*stderr;

#define	_READ	01	/* file opened for reading */
#define	_WRITE	02	/* file opened for writing */
#define	_UNBUF	04	/* file is unbuffered	   */
#define	_BIGBUF	010	/* big buffer allocated	   */

#define	_EOF	020	/* EOF occured		   */
#define	_ERR	040	/* error occured	   */

#define	_EFILE	0400	/* E editor file */

#define	NULL	0
#define	EOF	(-1)
