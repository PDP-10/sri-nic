/* STRERR - Implementation of strerror().
**	Copyright (c) 1987 by Ken Harrenstien, SRI International
*/

#include "c-env.h"
#include "stddef.h"	/* For NULL */
#include "errno.h"

/* Array of error messages indexed by UN*X error number. */

char *sys_errlist[] = {
	"No error",					/* errno = 0 */
	"Not owner",					/* EPERM */
	"No such file or directory",			/* ENOENT */
	"No such process",				/* ESRCH */
	"Interrupted system call",			/* EINTR */
	"I/O error",					/* EIO */
	"No such device or address",			/* ENXIO */
	"Arg list too long",				/* E2BIG */
	"Exec format error",				/* ENOEXEC */
	"Bad file number",				/* EBADF */
	"No children",					/* ECHILD */
	"No more processes",				/* EAGAIN */
	"Not enough core",				/* ENOMEM */
	"Permission denied",				/* EACCES */
	"Bad address",					/* EFAULT */
	"Block device required",			/* ENOTBLK */
	"Mount device busy",				/* EBUSY */
	"File exists",					/* EEXIST */
	"Cross-device link",				/* EXDEV */
	"No such device",				/* ENODEV */
	"Not a director",				/* ENOTDIR */
	"Is a directory",				/* EISDIR */
	"Invalid argument",				/* EINVAL */
	"File table overflow",				/* ENFILE */
	"Too many open files",				/* EMFILE */
	"Not a typewriter",				/* ENOTTY */
	"Text file busy",				/* ETXTBSY */
	"File too large",				/* EFBIG */
	"No space left on device",			/* ENOSPC */
	"Illegal seek",					/* ESPIPE */
	"Read-only file system",			/* EROFS */
	"Too many links",				/* EMLINK */
	"Broken pipe",					/* EPIPE */
	"Argument too large",				/* EDOM */
	"Result too large",				/* ERANGE */
	"Operation would block",			/* EWOULDBLOCK */
	"Operation now in progress",			/* EINPROGRESS */
	"Operation already in progress",		/* EALREADY */
	"Socket operation on non-socket",		/* ENOTSOCK */
	"Destination address required",			/* EDESTADDRREQ */
	"Message too long",				/* EMSGSIZE */
	"Protocol wrong type for socket",		/* EPROTOTYPE */
	"Protocol not available",			/* ENOPROTOOPT */
	"Protocol not supported",			/* EPROTONOSUPPORT */
	"Socket type not supported",			/* ESOCKTNOSUPPORT */
	"Operation not supported on socket",		/* EOPNOTSUPP */
	"Protocol family not supported",		/* EPFNOSUPPORT */
	"Address family not supported by protocol family",/* EAFNOSUPPORT */
	"Address already in use",			/* EADDRINUSE */
	"Can't assign requested address",		/* EADDRNOTAVAIL */
	"Network is down",				/* ENETDOWN */
	"Network is unreachable",			/* ENETUNREACH */
	"Network dropped connection on reset",		/* ENETRESET */
	"Software caused connection abort",		/* ECONNABORTED */
	"Connection reset by peer",			/* ECONNRESET */
	"No buffer space available",			/* ENOBUFS */
	"Socket is already connected",			/* EISCONN */
	"Socket is not connected",			/* ENOTCONN */
	"Can't send after socket shutdown",		/* ESHUTDOWN */
	"Too many references: can't splice",		/* ETOOMANYREFS */
	"Connection timed out",				/* ETIMEDOUT */
	"Connection refused",				/* ECONNREFUSED */
	"Too many levels of symbolic links",		/* ELOOP */
	"File name too long",				/* ENAMETOOLONG */
	"Host is down",					/* EHOSTDOWN */
	"No route to host",				/* EHOSTUNREACH */
	"Directory not empty",				/* ENOTEMPTY */
	"Too many processes",				/* EPROCLIM */
	"Too many users",				/* EUSERS */
	"Disc quota exceeded"				/* EDQUOT */
};

/* SYS_NERR holds max integer # that can be used to index SYS_ERRLIST. */
int sys_nerr = (sizeof sys_errlist)/(sizeof sys_errlist[0]) - 1;

static char *t20_errlist[] = {
	"No TOPS-20 error",				/* not used */
	"Must truncate on write-only open",		/* ETRUNC */
	"Can't close file"  	    	    	    	/* ECLOSE */
};
static int t20_nerr = (sizeof t20_errlist)/(sizeof t20_errlist[0]) - 1;

char *
strerror(errnum)
int errnum;
{
#if SYS_T20+SYS_10X
    if (errnum > _T20_ERR_FIRST && (errnum - _T20_ERR_FIRST) <= t20_nerr)
	return t20_errlist[errnum - _T20_ERR_FIRST];
#endif
    if (errnum > 0 && errnum <= sys_nerr)
	return sys_errlist[errnum];
    else return NULL;
}
