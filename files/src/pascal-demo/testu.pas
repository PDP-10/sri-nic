program testserver;
include 'ps:<hedrick.431>user.hdr';

type
  message = record
     length:integer;
     contents:array[1:2000]of char
     end;

var
  m:messpt;  {messpt is declared in server.hdr}

begin
m := user('CLH-TEST  ');
while true do
  begin
  writeln(tty);
  writeln(tty,'Your request:');
  readln(tty); read(tty,m^.contents:m^.length);
  sendrequest;
  getreply;
  writeln(tty);
  writeln(tty,'Reply:');
  writeln(tty,m^.contents:m^.length)
  end
end.

  


  
  