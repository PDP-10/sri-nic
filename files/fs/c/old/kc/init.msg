  ------------------------------------------------------------------
  A mailing list has been created for those interested in CC. If you
  are not interested in receiving junk mail concerning the compiler,
  please drop me a note.			k.c.
  ------------------------------------------------------------------

First some conventions:

C:  refers to [SU-SCORE]PS:<CSL.SP.KCHEN.C>
    and contains the runtime library for C, stdio, and so on.

CC: refers to [SU-SCORE]PS:<CSL.SP.KCHEN.CC>
    and contains the compiler sources and EXE file.

On a #include  "file", the compiler  will look for  "file" in the  usual
nooks and corners.  On a  #include <file>,  the compiler  will look  for
"file" in the  logical directory  C:. It is  advisable to  define C:  as
PS:<csl.sp.kchen.c> to the EXEC. "stdio.h" can be found in C:.

To compile a program, say

	@CC:CC foo

The compiler will  first look  for foo.C  if you  haven't specified  any
extension. It will create  foo.FAI. Assemble that  in the usual  manner.
The compiler stops emitting  code once it encounters  an error. Code  is
emitted independently for each C  function.  [ The intermediate code  is
generated in-core and  given to  the code  generator one  function at  a
time.  Therefore, there is a limit to the size of any one function (2000
syntax tree nodes), but none to the source file itself.]

A couple of compiler switches exist, the more useful ones are,

	-s	put out symbol table in foo.SYM
	-n	turn off optimizer

Other switches are more obscure and even I don't remember what they are.
You would  have to  look into  the sources  for them.  The switches  are
placed in the compile command line in a UNIX(*)-like fashion.

To load your files, you will need  C:CLIB (thanks to JQ, who wrote  it).
If you have  included stdio calls,  then you will  have to load  C:STDIO
with your other modules too.

For you DDT lovers, all  compiler-generated symbols are half-killed,  so
that DDT  will  show  offsets  from  from  symbols  that  are  names  of
functions or extern variables.

Auto and macro identifiers are unique  to 9 characters. Upper and  lower
cases are distinguished. Extern and  static identifiers are unique  only
to 6 characters and upper and  lower cases are not distinguished  (don't
blame me, FAIL limits me to that).

Float, double, short, long and all those goodies are not implemeted yet.
Struct, union, typedef are, so all is  not lost.  Int is a 36-bit  word.
Scalar char is  like an  int. Char  arrays (and  pointers) are  standard
DEC-10/20 7-bit byte (and byte pointers). [Don't worry, char pointers to
scalar char, i.e. &foo, works the way it should...] Char pointers  point
exactly at the byte, and not at the previous byte, as in JSYS. So, those
who wish to interface with JSYS will have to do a ADJBP -1 first. By the
way, ADJBP is  generated as IBP  by the compiler,  to make the  compiler
compatible with WAITS (the  FAIL assembler at  SU-AI does not  implement
the mnemonic ADJBP).

(This paragraph  is  intended for  those  who wish  to  write  assembler
program  interfaces.   All  20-th  century  programmers  may  skip  this
paragraph.)  Arguments to functions are passed by pushing them onto  the
stack.  The stack pointer is the usual P (AC017).  AC0 and AC1 are  used
as function returns.  AC0 is used for int and char and pointer  returns.
AC1 is reserved for used also by float and long and what nots. I haven't
the foggiest what I would do with double. Perhaps use AC0,1,2,3 in  some
way.  Arguments are pushed onto the stack so that the first argument  is
closest to the return address after a PUSHJ. The number of arguments  is
not pushed.  Variable-argument calls are possible if everything  depends
on the information  contained in  the first argument.   Upon entering  a
function, the  first argument  is addressed  via -1(P),  the second  via
-2(P) and so on. No frame pointer  is used, nor is one neccessary.   The
compiler updates its internal representation of the stack pointer  along
the way. The caller  is responsible for popping  the arguments back  off
the stack,  since it  is the  only one  who has  knowledge of  how  many
arguments a  function is  called  with. The  callee is  responsible  for
allocating stack  space for  auto variables  and releasing  them  before
executing a return.

Goto is implemented, although the compiler does not use a single one  of
them. Some type  coercions (casting) are  implemented. These are  (char)
<-> (int), (char *) <-> (int *) and (char *) <-> (struct *). (int *)  ->
(char *) and (struct *)  -> (char *) make good  sense.  But, if you  are
demented enough to try (char *) -> (int *) or (char *) -> (struct *), it
will try its best, which may still not be what you want. It simply zeros
out the left half of the byte pointer  and hands that to you as the  int
or struct pointed.  I.e., it works  fine if the char pointer is  pointed
at a word boundary. Int  and struct pointers are  assured to be zero  on
the left half-word so that you can do meaningful comparisons.

Because of the weird DEC-10/20 bytes, C programs may not be as portable
as you wish them to be. For example, sizeof of an int yields 5 (because
there are 5 chars in an int when they are packed).

The only claim I am willing to  make at the moment is that the  compiler
will compile itself. Since I write pretty straightforward code, this may
mean little. It also has been used to write two moderate-sized  programs
(one is  a  wire-wrap  compiler  and  the other  is  one  that  takes  a
description of a logic diagram and produces a schematic diagram in PRESS
format), so  the  compiler does  work  in  some fashion.   The  code  it
generates is  not super,  nor is  it rotten.  It runs  moderately  fast,
15,000 to 30,000  source lines per  minute, depending on  the amount  of
white-space in your program.  (Well, at  least I can claim that it  runs
faster than the other known C compiler on SCORE, ha ha.)

It may take many  hours before I  find time to fix  a reported bug.  You
will have to code around the bugs in the meantime. Those who are weak of
heart are advised to stick to PASCAL.

-------------------------------------------------------------------------
Finally, the only real piece of new news:

	#ifdef and #endif have been implemented. Enjoy (or, suffer,
as the case may be).

-------------------------------------------------------------------------
(*) UNIX is a Trademark of Bell Laboratories.
