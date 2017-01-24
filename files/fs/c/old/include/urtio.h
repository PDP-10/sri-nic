/*
 *	URTIO.H - Unix RunTime definitions for I/O calls
 */

#include <c-env.h>
#ifndef FILE
#include <stdio.h>
#endif

#define UIO_FIRST_CH	3	/* first free channel (0, 1 & 2 predefined) */
#define UIO_MAXFILE 	32	/* Max FDs we can have open */
#define UIO_UNASS	-1	/* Value of unassigned FD slot */

extern _uioch[UIO_MAXFILE];	/* Storage for I/O chans or JFNs */
extern FILE _sios[UIO_FIRST_CH];

#if SYS_WAITS
struct	bufhead {			/* structure for buffer headers */
	int	system;			/*       used by stdio ?? */
	char	*user;			/*       deposit pointer */
	int	count;			/*       space remaining in buffer */
};

struct	filespec {			/* structure for opening files */
	int	name;			/*       filename */
	int	extension;		/*       extension */
	int	date;			/*       date ?? */
	int	PPN;			/*       project-programmer num */
};

extern struct bufhead _buffers[UIO_MAXFILE]; /* file I/O buffers */
extern int _uioeof;			/* end of file reached on terminal */
#endif /* WAITS */

#define O_RDONLY	01
#define O_WRONLY	02
#define O_RDWR		04
#define O_NDELAY	010
#define O_APPEND	020
#define O_CREAT		040
#define O_TRUNC		0100
#define O_EXCL		0200

#define L_SET		0
#define L_INCR		1
#define L_XTND		2

/*
 *	function declarations
 */

int open(), close(), read(), readln(), write();
