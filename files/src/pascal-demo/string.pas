procedure putstr(source:string;count:integer;dest:string;offset:integer); extern;
{PUTSTR copies a piece of a string into another string.
  source - copies this string from its beginning
  count - for this many characters
  dest - to this string
  offset - starting this many characters from its beginning
}

function findnull(source:string):integer; extern;
 {FINDNULL looks in SOURCE for a null and returns the offset}

procedure byteset(var f:file;bytepointer:integer); extern;
procedure bytewrite(var f:file;bytepointer:integer); extern;
{
BYTESET and BYTEWRITE work just like STRSET and STRWRITE, but take as
arguments byte pointers for where to start reading or writing.
They are designed to play with strings where JSYS's are also being
used.  So READ will return EOF at a null and WRITE will put null at
end of file.  No check for end of string is done, since the JSYS's don't.

ARRSET and ARRWRITE are the same, but are used at the beginning of an
array - they take an array as arguments instead of a byte pointer, and
start I/O at the beginning of the array.  They differ from STRSET and
STRWRITE in that nulls are treated as in BYTESET and BYTEWRITE.
}

procedure arrset(var f:file;st:string); extern;
procedure arrwrite(var f:file;st:string); extern;

function bytepos(var f:file):integer; extern.
{BYTEPOS returns a byte pointer which when IBP'ed will give the next
character to be read or written for the file.  The file is assumed
to be open on a string (with STRSET, STRWRITE, BYTESET, BYTEWRITE,
ARRSET, or ARRWRITE).
