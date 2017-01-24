{This program is a demonstration of the SORT interface.  It creates a
file with random numbers in it, calls SORT to sort the file, and prints
the first 20 lines in the sorted file.  To run this program, load it
with PASSRT.MAC.  
    exec srttst.pas,passrt.mac
This program does things in a more complex way than it needs to, just to
show you how to do certain common things.  It reads the file names from
the user and constructs the command string to SORT dynamically.  Since
Pascal does not have strings, this can be a bit tricky.  We construct
the command string by openning the file OUTPUT on the command string and
writing the command to OUTPUT.  The ability to do this is a special
feature of Tops-20 Pascal.  It allows you to redirect a file, in this
case OUTPUT, to a string (i.e. a packed array of char.  Anything you
write to OUTPUT shows up in that string.  This turns out to be a simple
and powerful way to build up strings, since the Pascal write procedure
lets you format things fairly easily.}

program test;

const
    namelength=200;
    commandlength=2500;

var 
    numrecords,i:integer;
    unsortedfile,sortedfile:text;
    unsortedname,sortedname:packed array[1:namelength]of char;
    unsortedlength,sortedlength:integer;
    sortcommand:packed array[1:commandlength]of char;
    thing:real;

   {string is a built-in data type usable only in declarations of
    external procedures.  It allows a packed array of char of
    any length.  It passes both the address of the array and its
    length.}
procedure sort(s:string);extern;

begin
for i := 1 to commandlength do sortcommand[i] := chr(0);
   {We clear the command buffer in case you decide to restart the
    program, and the new command is shorter than the old one.
    Blanks would be as good as nulls.}

write(tty,'How many records would you like in the file? ');
readln(tty); read(tty,numrecords);

write(tty,'File name for unsorted random numbers: ');
readln(tty); read(tty,unsortedname:unsortedlength);
    {Note that READ into a packed array of char reads until you
     fill the array or you get to the end of the line in the input,
     whichever happens first.  UNSORTEDLENGTH gets set to the number
     of characters read.}
     

write(tty,'File name for sorted random numbers: ');
readln(tty); read(tty,sortedname:sortedlength);

strwrite(output,sortcommand);
    {strwrite is like encode in Fortran - it directs a file to
     a packed array of char.  That is, any writes done on
     OUTPUT will now go into that array}
write('s/r:20/k:1,20 ',unsortedname:unsortedlength,' ',
		       sortedname:sortedlength);

rewrite(output,unsortedname);
     {this form of rewrite opens the file OUTPUT with whatever filename
      is in the variable UNSORTEDNAME}

for i := 1 to numrecords do
   writeln(random(0):20:7);
     {this writes out a random number in format F20.7.  Random(0)
      calls RAN(0) from the Fortran library.  Fortran requires that
      the random number generator take an argument.  However it
      completely ignores the argument.}
      

close(output);
     {We must close the file, or SORT won't be able to see it.  New
      files do not show up on disk until they are closed}

sort(sortcommand);

reset(input,sortedname);
     {Open the file INPUT for input, with filename stored in the
      variable SORTEDNAME}

if numrecords > 20   {output only a reasonable number}
  then numrecords := 20;

writeln(tty,'Please verify that these are in order');

for i := 1 to numrecords do
  begin
  read(thing);
  writeln(tty,thing:20:7)
  end;
  
end.
