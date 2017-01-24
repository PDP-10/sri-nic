/*
 *	FREOPEN - reopen a file on a given stream
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "sys/file.h"


FILE *freopen(pathname, type, f)
char *pathname, *type;
register FILE *f;
{
    int fd, flags, uio_flags;

    if (f->siocheck == _SIOF_OPEN) {
	flags = f->sioflgs & _SIOF_DYNAMFILE;	/* extract dynamfile flag */
	f->sioflgs &= ~_SIOF_DYNAMFILE;		/* then remove the flag */
	fclose(f);				/* close the old stream */
	f->sioflgs |= flags;			/* maybe add back in flag */
    } else if (f->siocheck != _SIOF_FILE)
	return NULL;			/* Bad FILE pointer */

    if (!(flags = _sioflags(type, &uio_flags)))
	return NULL;			/* Bad type string */

    if ((fd = open(pathname, uio_flags, 0644)) < 0)
	return NULL;			/* Bad pathname or sys error */

    _setFILE(f, fd, flags);		/* Won, set up FILE block */
    return f;
}

/* _SIOFLAGS - Parse and check the stream-type specification string.
 *	Used by fdopen() and freopen().
 *	Returns the _SIOF flags as value (0 if error), and stores
 *	the UNIX open() call flags in given location as 2nd result.
 *
 *	"r"	open an existing file for reading
 *	"w"	open a new file, or truncate an existing one, for writing
 *	"a"	create a new file, or append to an existing one, for writing
 *	"r+"	open an existing file for update (both reading and writing),
 *		starting at the beginning of the file
 *	"w+"	create a new file, or truncate an existing one, for update
 *	"a+"	create a new file, or append to an existing one, for update
 *
 *	additional flags allowed:
 *
 *	"x"	auto eXpand string buffer
 *	"b"	select binary mode stream (as opposed to text stream)
 *	"C"	force newline-CRLF conversion (on text stream)
 *	"C-"	force NO newline-CRLF conversion
 *	"7"	force 7-bit byte-size
 *	"8"	force 8-bit byte-size
 *	"9"	force 9-bit byte-size
 */

int _sioflags(type, ufaddr)
char *type;
int *ufaddr;
{
    int c, flags = 0, byte_size = 0, uio_flags = 0;

    while (c = *type++) switch (c) {
	case 'r':
	    flags |= _SIOF_READ; break;
	case 'w':
	    flags |= _SIOF_WRITE; break;
	case 'a':
	    flags |= _SIOF_APPEND; break;
	case 'b':
	    flags |= _SIOF_BINARY; break;
	case '+':
	    flags |= _SIOF_UPDATE; break;
	case 'x':
	    flags |= _SIOF_GROWBUF; break;
	case '7': case '8': case '9':
	    byte_size = c - '0'; break;
	case 'C':
	    if (*type == '-') {
		flags |= _SIOF_UNCONVERTED;
		type++;
	    } else flags |= _SIOF_CONVERTED;
	    break;
	default:
	    return 0;			/* Bad char in type specification */
    }

    switch (flags & (_SIOF_READ | _SIOF_WRITE | _SIOF_APPEND | _SIOF_UPDATE)) {
	case _SIOF_READ:			/* "r" */
	    uio_flags = O_RDONLY; break;
	case _SIOF_WRITE:			/* "w" */
	    uio_flags = O_WRONLY | O_CREAT | O_TRUNC; break;
	case _SIOF_APPEND:			/* "a" */
	    uio_flags = O_WRONLY | O_CREAT | O_APPEND; break;
	case (_SIOF_READ | _SIOF_UPDATE):	/* "r+" */
	    uio_flags = O_RDWR; break;
	case (_SIOF_WRITE | _SIOF_UPDATE):	/* "w+" */
	    uio_flags = O_RDWR | O_CREAT | O_TRUNC; break;
	case (_SIOF_APPEND | _SIOF_UPDATE):	/* "a+" */
	    uio_flags = O_RDWR | O_CREAT | O_APPEND; break;
	default:
	    return 0;			/* Bad specification string */
    }

    if (flags & _SIOF_BINARY) uio_flags |= O_BINARY;
    if (flags & _SIOF_CONVERTED) uio_flags |= O_CONVERTED;
    else if (flags & _SIOF_UNCONVERTED) uio_flags |= O_UNCONVERTED;
    switch (byte_size) {
	case 7:	uio_flags |= O_BSIZE_7; break;
	case 8:	uio_flags |= O_BSIZE_8; break;
	case 9:	uio_flags |= O_BSIZE_9; break;
    }
				/* Done with Unix open() flags, return them */
    if (ufaddr) *ufaddr = uio_flags;

    if (flags & _SIOF_APPEND) flags |= _SIOF_WRITE;
    if (flags & _SIOF_UPDATE) flags &= ~(_SIOF_READ | _SIOF_WRITE);
    return flags;
}

/* _setFILE - Initialize a FILE block, given a FD and flags.
**	Only thing that is assumed to already exist in the block is the
**	_SIOF_DYNAMFILE flag.
*/
void
_setFILE(f, fd, flags)
FILE *f;
int fd, flags;
{
    f->siofd = fd;
    f->siofdoff = lseek(fd, 0, L_INCR);		/* Set starting position */
    f->sioflgs = (f->sioflgs & _SIOF_DYNAMFILE) | flags | _SIOF_AUTOBUF;
    f->siocp = f->siopbuf = NULL;
    f->siolbuf = f->sioocnt = f->siocnt = f->sioerr = 0;
    f->siocheck = _SIOF_OPEN;			/* Done, validate block */
}
