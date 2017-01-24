/* STDIO.H for TOPS-20 implementation */

# define BUFSIZ 512		/* this number is irrelevant */
# define FILE int		/* the actual structure is irrelevant */
# define NULL 0			/* null file pointer for error return */
# define EOF (-1)		/* returned on end of file */

extern stdin,stdout,stderr,fprintf;
