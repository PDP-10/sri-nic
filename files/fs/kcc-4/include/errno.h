/* Error codes from 4.2BSD UPM.
** Most of these are irrelevant or unimplemented on TOPS-20.
** Nevertheless, they are defined here so that code which wants to
** use them can win.
*/

extern int errno;		/* Error variable */
#if 0
extern int sys_nerr;		/* BSD (but not ANSI), defined in STRERR.C */
extern char *sys_errlist[];
#endif

#define EPERM		1	/* Not owner */
#define ENOENT		2	/* No such file or directory */
#define ESRCH		3	/* No such process */
#define EINTR		4	/* Interrupted system call */
#define EIO		5	/* I/O error */
#define ENXIO		6	/* No such device or address */
#define E2BIG		7	/* Arg list too long */
#define ENOEXEC		8	/* Exec format error */
#define EBADF		9	/* Bad file number */
#define ECHILD		10	/* No children */
#define EAGAIN		11	/* No more processes */
#define ENOMEM		12	/* Not enough core */
#define EACCES		13	/* Permission denied */
#define EFAULT		14	/* Bad address */
#define ENOTBLK		15	/* Block device required */
#define EBUSY		16	/* Mount device busy */
#define EEXIST		17	/* File exists */
#define EXDEV		18	/* Cross-device link */
#define ENODEV		19	/* No such device */
#define ENOTDIR		20	/* Not a directory*/
#define EISDIR		21	/* Is a directory */
#define EINVAL		22	/* Invalid argument */
#define ENFILE		23	/* File table overflow */
#define EMFILE		24	/* Too many open files */
#define ENOTTY		25	/* Not a typewriter */
#define ETXTBSY		26	/* Text file busy */
#define EFBIG		27	/* File too large */
#define ENOSPC		28	/* No space left on device */
#define ESPIPE		29	/* Illegal seek */
#define EROFS		30	/* Read-only file system */
#define EMLINK		31	/* Too many links */
#define EPIPE		32	/* Broken pipe */
#define EDOM		33	/* Math package: Math argument */
#define ERANGE		34	/* Math package: Result too large */
#define EWOULDBLOCK	35	/* Operation would block */
#define EINPROGRESS	36	/* Operation now in progress */
#define EALREADY	37	/* Operation already in progress */
#define ENOTSOCK	38	/* Socket operation on non-socket */
#define EDESTADDRREQ	39	/* Destination address required */
#define EMSGSIZE	40	/* Message too long */
#define EPROTOTYPE	41	/* Protocol wrong type for socket */
#define ENOPROTOOPT	42	/* Bad protocol option */
#define EPROTONOSUPPORT	43	/* Protocol not supported */
#define ESOCKTNOSUPPORT	44	/* Socket type not supported */
#define EOPNOTSUPP	45	/* Operation not supported on socket */
#define EPFNOSUPPORT	46	/* Protocol family not supported */
#define EAFNOSUPPORT	47	/* Address family not supported by protocol family */
#define EADDRINUSE	48	/* Address already in use */
#define EADDRNOTAVAIL	49	/* Can't assign requested address */
#define ENETDOWN	50	/* Network is down */
#define ENETUNREACH	51	/* Network is unreachable */
#define ENETRESET	52	/* Network dropped connection on reset */
#define ECONNABORTED	53	/* Software caused connection abort */
#define ECONNRESET	54	/* Connection reset by peer */
#define ENOBUFS		55	/* No buffer space available */
#define EISCONN		56	/* Socket is already connected */
#define ENOTCONN	57	/* Socket is not connected */
#define ESHUTDOWN	58	/* Can't send after socket shutdown */
/* #define unused	59 */
#define ETIMEDOUT	60	/* Connection timed out */
#define ECONNREFUSED	61	/* Connection refused */
#define ELOOP		62	/* Too many levels of symbolic links */
#define ENAMETOOLONG	63	/* File name too long */
#define EHOSTDOWN	64	/* Host is down */
#define EHOSTUNREACH	65	/* Host is unreachable */
#define ENOTEMPTY	66	/* Directory not empty */
/* #define unused	67 */
/* #define unused	68 */
#define EDQUOT		69	/* Disc quota exceeded */

#define _ERR_LAST	EDQUOT	/* last defined error */

#define _T20_ERR_FIRST	1000	/* T20 error codes start at this value! */
#define ETRUNC		1001	/* Must truncate on write-only open */
#define ECLOSE    	1002	/* Can't close file */

#define _T20_ERR_LAST	ETRUNC	/* last defined T20 error */
