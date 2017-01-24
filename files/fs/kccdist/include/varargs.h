/*
** varargs.h - support for variable numbers of arguments
** David Eppstein / Stanford University / 24-Aug-85
**
** Uses PDP-10 convention of arguments on word boundaries,
** with stack growing towards larger numbers and arguments
** inversely ordered on stack.
**
** Sample use:
**
** int sum(va_alist)
** va_dcl             <<=== NB no semicolon
** {
**     int s = 0, x; va_list pvar;
**     va_start (pvar);
**     while ((x = va_arg (pvar, int)) != 0) s += x;
**     va_end (pvar);
**     return s;
** }
*/

#define va_dcl int va_alist;		/* declare args for fn */
typedef int *va_list;			/* use int pointers for lists */
#define va_start(pvar) (pvar = 1+&va_alist) /* point past first arg */
#define va_arg(pvar,type) (*(type *)(pvar-=(sizeof(type)+sizeof(int)-1) \
					    / sizeof(int))) /* get arg */
#define va_end(pvar)			/* finished with arglist */
