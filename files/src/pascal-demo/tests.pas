program testserver;
include 'ps:<hedrick.431>server.hdr';

type
  message = record
     length:integer;
     contents:array[1:2000]of char
     end;

var
  m:messpt;  {messpt is declared in server.hdr}

begin
m := server('CLH-TEST  ');
while true do
  begin
  getrequest;
  writeln(tty);
  writeln(tty,'Request:');
  writeln(tty,m^.contents:m^.length);
  writeln(tty);
  writeln(tty,'Your reply:');
  readln(tty); read(tty,m^.contents:m^.length);
  sendreply
  end
end.

  


  
  