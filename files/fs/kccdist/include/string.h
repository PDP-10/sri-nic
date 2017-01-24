/* STRING.H - predeclarations for string routines.
**
**	All of these functions are documented in
**	CARM II (H&S v2), chap 15 and 16.
*/

#ifndef NULL
#define NULL 0
#endif

/* CARM functions, most also exist for ANSI and BSD */
extern	char *strcat();
extern	char *strchr();
extern	char *index();		/* V7/BSD name for strchr */
extern	int strcmp();
extern	char *strcpy();
extern	int strcspn();
extern	int strlen();
extern	char *strncat();
extern	int strncmp();
extern	char *strncpy();
extern	char *strpbrk();
extern	int strpos();		/* CARM only! */
extern	char *strrchr();
extern	char *rindex();		/* V7/BSD name for strrchr */
extern	char *strrpbrk();	/* CARM only! */
extern	int strrpos();		/* CARM only! */
extern	int strspn();
extern	char *strtok();		/* not in V7 */
extern	char *strstr();		/* New ANSI/CARM */
extern	char *strerror();	/* New ANSI/CARM */

/* These functions used to be declared in <memory.h> but ANSI has
** moved them here, so <memory.h> now just includes this file.
*/
extern	char *memchr();
extern	int memcmp();
extern	char *memcpy();
extern	char *memset();
extern	char *memmove();	/* New ANSI/CARM */
extern	char *memccpy();	/* S5/BSD/CARM, not in ANSI */
