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
*/

#ifdef ERRSTRINGS

#define err(id,str) ,str
char *errstr[] = { 0

#else

#define err(id,str) ,id
enum { NO_ERROR

#endif

err(EIDENT, "Identifier expected")
err(ECONST, "Constant expected")
err(EMACRO, "Unsupported preprocessor command -- %s")
err(EFILE, "Can't open file -- %s")
err(EEOF, "Unexpected end of file")
err(ECHAR, "Unclosed character constant")
err(EXFILE, "Filename expected")
err(EDSYMB, "Symbol previously defined -- %s")
err(EARG, "Identifier needed as formal argument")
err(EARGEND, "End of formal argument list expected")
err(ENARGS, "Number of arguments mismatches -- %d expected, %d seen")
err(EUNDEF, "Undefined symbol -- %s")
err(ENOTMAC, "Symbol is not a macro")
err(EXPECT, "Expected token not found -- %s")
err(ETYPE, "Type table overflow")
err(SYNTAX, "Syntax")
err(ENOTARG, "Identifier not in parameter list -- %s")
err(ELABEL, "Identifier not a label -- %s")
err(EPRIM, "Primary expected")
err(EMEMBER, "Name of a structure member expected")
err(EBREAK, "Break not allowed here")
err(EPTR, "Pointer type needed as operand of *()")
err(EDSTRUC, "Structure previously defined")
err(EDUNION, "Union previously defined")
err(EDMEM, "Member not found in struct, has multiple possible offsets -- %s")
err(ESIZE, "Initializer too large")
err(ELVALUE, "Lvalue required -- %s")
err(EARRAY, "Array or pointer type required")
err(EINT, "Int type needed -- %s")
err(ESTRUCT, "Structure type needed -- ().%s")
err(EPSTRUCT, "Pointer to struct needed -- ()->%s")
err(EPARITH, "Illegal pointer arithmetic")
err(ECOER, "Unsupported type coercion")
err(EASMNEST, "Already in #asm, can't nest")
err(ECONTINUE, "Continue not allowed here")
err(EFNCALL, "Call to non-function")
err(EBINIT, "Illegal initialization")
err(ETYPCOMB, "Invalid combination of data types")
err(EBFSIZE, "Bit field longer than word")
err(EASMEND, "Not in #asm, can't #endasm")
err(EBIGFN, "Function too large")
err(EAUTOFULL, "Symbol table discovered full in flush of local vars")
err(ECUNIMP, "Unimplemented constant type -- %d")
err(EUTCCL, "Unterminated compiler control line seen")
err(EUNKSWI, "Unknown switch -- %c")
err(ENFILE, "No filename given -- %s")
err(EFILOPN, "Could not open %s file -- %s")
err(EASM, "Could not fork to assembler")
err(NERRORS, "%d error(s) detected")
err(EENUM, "Can not get address for enum tag -- %s")
err(EADDR, "Attempt to get address for unknown op -- %d")
err(EASGN, "Unknown assignment lvalue op -- %d")
err(EASOP, "Unknown assignment op -- %d")
err(EUNARY, "Unknown unary op -- %d")
err(ECASE, "Case label outside switch statement")
err(ECGEN, "Unimplemented op in code generation -- %d")
err(EUNKCOE, "Unknown coercion -- %d")
err(EPTRPTR, "Pointer/pointer arithmetic op -- %d")
err(EUNKPTR, "Unknown pointer op -- %d")
err(EFLOAT, "Unknown floating point op -- %d")
err(ERETVOID, "Function is of type void, can't have a return value")
err(ETWOTYP, "Symbol previously declared with different type -- %s")
err(EIMPCOER, "Implicit coercion assumed -- %s")
err(EDUPCASE, "Duplicate case labels within switch -- %d")
err(EDUPDEF, "Multiple defaults within switch")
err(EUTASM, "Unterminated #asm at end of file")
err(EENTRY, "Entry statement after start of code")
err(EOVCASE, "Too many case statements; need to remake CC -- %d")
err(ESTRSIZE, "Structure not defined, unable to calculate size -- %s")
err(EULABEL, "Goto label used but never defined -- %s")
err(EINIT, "Initializer mismatched with variable type -- %s")
err(EDUPLAB, "Label defined twice -- %s")
err(EENDIF, "Unmatching #endif encountered")
err(EBBLOCK, "Bad BLOCK pseudo op -- %d")
err(EOUTMEM, "Out of memory")
err(ERELREG, "Register allocation error: %s")
err(EVOID, "Invalid use of void -- %s")
err(EUNKMEM, "Unrecognized structure member -- %s")
err(ENPDEF, "Too many predefined macros -- %s")
err(EPREDEF, "Bad syntax for predefined macro -- %s")
err(ECASERNG, "Case label outside range of AND in switch -- %d")
err(EBIGMAC, "Too many %s in macro definition -- %s")
err(ETERNPREC, "Comma inside (?:) has lower precedence -- use parentheses")
err(EFNARITH, "Can't do anything to function other than calling it")
err(EGMEM, "Member not found in struct -- %s")
err(EBITPREC, "Bitwise operation on result of comparison -- use parentheses")

};					/* end enum or array initializer */
#undef err
