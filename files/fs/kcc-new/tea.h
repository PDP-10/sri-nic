/* TEA.H - Table Entry Allocator header file
*/

#ifndef _TEA_INCLUDED	/* Include only once */
#define _TEA_INCLUDED 1

#include <limits.h>
#include <stddef.h>

#if INT_MAX > 32767
#define teawd_t int	/* Plain int is big enough */
#else
#define teawd_t long	/* Otherwise use long */
#endif

extern size_t	tea_size();	/* (int n) */
extern teawd_t *tea_init(); 	/* (teawd_t *ptr, int n)		*/
extern teawd_t *tea_table();	/* (teawd_t *ptr)			*/
extern int	tea_maxfree();	/* (teawd_t *ptr)			*/
extern int	tea_alloc();	/* (teawd_t *ptr,        int n, teawd_t val) */
extern int	tea_get();	/* (teawd_t *ptr, int i, int n, teawd_t val) */
extern int	tea_exp();	/* (teawd_t *ptr, int i, int n, teawd_t val) */
extern /*bool*/	tea_set();	/* (teawd_t *ptr, int i, int n, teawd_t val) */
extern /*bool*/	tea_free();	/* (teawd_t *ptr, int i, int n)		*/
extern /*bool*/	tea_show();	/* (teawd_t *ptr, FILE *f)		*/
extern /*bool*/	tea_verify();	/* (teawd_t *ptr) */

#endif /* ifndef _TEA_INCLUDED */

