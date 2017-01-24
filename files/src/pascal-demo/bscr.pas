program bscribe;

include '<pascal>pascmd.pas','<pascal>string.pas','<pascal>quasar.pas';

const
	noswitch=0;
	filex=1;
	la36=2;
	gigi=3;
	lpt=4;
	diablo=5;
	gsi=6;
	pagedfile=7;
	dover=8;
	penguin=9;
	device=10;
	hyphen=11;
	vocab=12;
	words=13;
	quiet=14;
	terse=15;
	draft=16;
	noindex=17;
	x9700=18;
	time=19;
	lastswitch=19;

var
	privs:set of 0..35;
	onechar:packed array[1..1]of char;
	devicename,draftname:packed array[1..40]of char;
	devicelength,draftlength:integer;
	tad:tadrec;
	i,ret:integer;
	infile:text;
	swdone:Boolean;
	switchtable:table;
	switchnames:array[1:lastswitch]of alfa;
	switchlengths:array[1:lastswitch]of integer;
	switches:set of 1..lastswitch;
	buf:packed array[1:100]of char;
	sbuf:packed array[1:39]of char;
	xwd:packed record case Boolean of
		true:(full:integer);
		false:(lh:0..777777B;rh:0..777777B)
		end;

initprocedure;
	begin
	switchnames[filex] := 'FILE      ';
	switchnames[la36] := 'LA36      ';
	switchnames[gigi] := 'GIGI      ';
	switchnames[lpt]  := 'LPT       ';
	switchnames[diablo] := 'DIABLO    ';
	switchnames[gsi] := 'GSI       ';
	switchnames[pagedfile] := 'PAGEDFILE ';
	switchnames[dover] := 'DOVER     ';
	switchnames[penguin] := 'PENGUIN   ';
	switchnames[device] := 'DEVICE    ';
	switchnames[hyphen] := 'HYPHEN    ';
	switchnames[vocab] := 'VOCAB     ';
	switchnames[words] := 'WORDS     ';
	switchnames[quiet] := 'QUIET     ';
	switchnames[terse] := 'TERSE     ';
	switchnames[draft] := 'DRAFT     ';
	switchnames[noindex] := 'NOINDEX   ';
	switchnames[x9700] := 'X9700     ';
	switchnames[time] := 'TIME      ';
	end;

initprocedure;
	begin
	switchlengths[filex] := 4;
	switchlengths[la36] := 4;
	switchlengths[gigi] := 4;
	switchlengths[lpt]  := 3;
	switchlengths[diablo] := 6;
	switchlengths[gsi] := 3;
	switchlengths[pagedfile] := 9;
	switchlengths[dover] := 5;
	switchlengths[penguin] := 7;
	switchlengths[device] := 6;
	switchlengths[hyphen] := 6;
	switchlengths[vocab] := 5;
	switchlengths[words] := 5;
	switchlengths[quiet] := 5;
	switchlengths[terse] := 5;
	switchlengths[draft] := 5;
	switchlengths[noindex] := 7;
	switchlengths[x9700] := 5;
	switchlengths[time] := 4;
	end;

procedure quit; extern;

procedure dodevice;
  begin
  devicelength := cmfld(devicename);
  if devicelength = 0
    then cmuerr('Null device specification')
  end;

procedure dodraft;
  begin
  draftlength := cmfld(draftname);
  if draftlength = 0
    then cmuerr('Null draft specification')
  end;

begin
devicelength := 0;
draftlength := 0;
tad.seconds := 300;

switchtable := tbmak(lastswitch+1+11);
tbadd(switchtable,filex,'FILE',0);
onechar[1] := 'F';
tbadd(switchtable,filex,onechar,1);
tbadd(switchtable,la36,'LA36',0);
onechar[1] := 'A';
tbadd(switchtable,la36,onechar,1);
tbadd(switchtable,gigi,'GIGI',0);
tbadd(switchtable,gigi,'GG',1);
tbadd(switchtable,lpt,'LPT',0);
onechar[1] := 'L';
tbadd(switchtable,lpt,onechar,1);
tbadd(switchtable,diablo,'DIABLO',0);
onechar[1] := 'D';
tbadd(switchtable,diablo,onechar,1);
tbadd(switchtable,gsi,'GSI',0);
onechar[1] := 'G';
tbadd(switchtable,gsi,onechar,1);
tbadd(switchtable,pagedfile,'PAGEDFILE',0);
tbadd(switchtable,dover,'DOVER',0);
tbadd(switchtable,penguin,'PENGUIN',0);
tbadd(switchtable,device,'DEVICE:',0);
tbadd(switchtable,hyphen,'HYPHEN',0);
onechar[1] := 'H';
tbadd(switchtable,hyphen,onechar,1);
tbadd(switchtable,vocab,'VOCAB',0);
onechar[1] := 'V';
tbadd(switchtable,vocab,onechar,1);
tbadd(switchtable,words,'WORDS',0);
onechar[1] := 'W';
tbadd(switchtable,words,onechar,1);
tbadd(switchtable,quiet,'QUIET',0);
onechar[1] := 'Q';
tbadd(switchtable,quiet,onechar,1);
tbadd(switchtable,terse,'TERSE',0);
onechar[1] := 'T';
tbadd(switchtable,terse,onechar,1);
tbadd(switchtable,draft,'DRAFT',0);
tbadd(switchtable,noindex,'NOINDEX',0);
tbadd(switchtable,x9700,'X9700',0);
tbadd(switchtable,time,'TIME:',0);

cminir('BSCRIBE>');

gjgen(100000000000B);	{an input file}
gjext('MSS');
cmfil(infile);		{This is the main part of the command}

swdone := false;
switches := [];
loop
    cmmult;	{multiple mode}
    cmcfm;	{CRLF}
    i := cmswi(switchtable);  {or switch - i is dummy return}
    i := cmdo;  {now actually do it}
    exit if i = 1;  {done if CRLF}    
    i := cmint;
    if i > 0
      then switches := switches + [i]
      else case i of
	-device: dodevice;
	-draft:  dodraft;
	-time:	 cmtn(tad);
	others:  cmuerr('This switch does not take an argument')
	end
    end;

rewrite(output,'BSCRIBE.CTL;T','/m:1');
jsys(150B{rpcap};400000B;i,i,privs);
if (18 in privs) or (19 in privs)
  then writeln('ENABLE');
jsys(20B{gtjfn},2,ret;100001B:0,-1:'SYS:SCRIBE.EXE';i);
if ret <> 2
  then begin
  writeln(tty,'? Can''t find SYS:SCRIBE.EXE');
  quit
  end;
jsys(30B{jfns};0:output,0:i,111100B:1);
writeln;
jsys(23B{rljfn},2;0:i);
jsys(30B{jfns};0:output,0:infile,111110B:1);
for i := 1 to lastswitch do
  if i in switches
    then write('/',switchnames[i]:switchlengths[i]);
if draftlength > 0
  then write('/DRAFT:',draftname:draftlength);
if devicelength > 0
  then write('/DEVICE:',devicename:devicelength);
writeln;
jsys(30B{jfns};-1:buf,0:output,111110B:1);
close(output);
close(infile);

qsrinit;
startqueue(batch,'BSCRIBE   ',buf);
qoutput(lognever);
qnotify(notifytty);
qbatchlog(logsupercede);
qlimit(tad.seconds);
doqueue(true);

end.
