/*	CCERR.H - KCC Error Codes
**
**	All changes after version 17 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/*
** ccerr.h - error codes for KCC
** Split out of cc.h / David Eppstein / 23 May 85
**
** This source is used both for generating the constant error numbers
** used to pass to the error handler, and for generating the array of
** error message strings indexed by those numbers.
**
** Either add errors to the end of the list and never delete them,
** or recompile all of KCC whenever the list is changed.
**
** Note EGEN is handled specially; the call always furnishes the complete
** error string and whatever args it needs.  This avoids having to
** make entries here for one-of-a-kind error messages.
*/

#ifdef ERRSTRINGS

#define err(id,str) ,str
char *errstr[] = { 0

#else

#define err(id,str) ,id
enum { NO_ERROR

#endif
err(EGEN,    "")			/* Handled specially */
err(EINT,    "Internal error")		/* Handled specially */

err(ECONST,  "Constant expected")
err(ECPOOL, "Internal char pool overflow, too many string constants or macros")
err(EDSYMB,  "Symbol \"%s\" previously defined")
err(EEOF,    "Unexpected end of file %s")
err(EFILOPN, "Could not open %s file -- %s")
err(EIDENT,  "Identifier expected")
err(EMACPOOL,"Internal macro arg char pool overflow")
err(ENOTINT, "Operation applies to integral types only -- %s")
err(EOUTMEM, "Out of memory")
err(ERELREG, "Internal error - Register allocation: %s")
err(ETWOTYP, "Symbol \"%s\" previously declared with different type")
err(ETYPCOMB,"Invalid combination of data types")
err(EVOID,   "Invalid use of void -- %s")

/* Move up later */
err(EINTNOD, "Internal error")		/* Internal node error */

};					/* end enum or array initializer */
#undef err
