
struct fooalign { char ch; double doub; }; /*How much space does ch get? */

foo()
{     /* THIS COMPILES OK: */
  return (char *) & ((struct fooalign *) 0)->doub    /* offset of doub */
       - (char *)    (struct fooalign *) 0;       /* minus base of struct */
}
foo2()
{    /* THIS DOESN'T: */
  return (char *) & ((struct fooalign *) 0)->doub
       - (char *) 0;
}
/*
error message: Internal error - unknown adrmode for SUBBP: 6
*/
