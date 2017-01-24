(* <SU-UTILITIES>QF.PAS.3,  7-Apr-85 11:09:07, Edit by WHP4 *)
(*  do print batch requests *)
(* <WHP4>QF.PAS.5, 14-May-84 17:31:28, Edit by WHP4 *)
(*  print card-punch requests, they're really laser-printer requests *)
(* ACCT:<SU-UTILITIES>QF.PAS.3, 29-Nov-83 14:59:57, Edit by R.RMK *)
(* Don't print batch requests *)
(* ACCT:<SU-UTILITIES>QF.PAS.2,  8-Aug-83 19:49:42, Edit by R.RMK *)
(*  Change for 4.2 GALAXY. *)
Program Qfile;
(* Performs various read-only operations on the galaxy system
by reading the master queue file.*)

LABEL 9999;
(* Structured programming, huh?  Tell P20 to have a construct
for going to the end of a loop from the middle, or multiple
EXIT IF statements, and I wouldn't need this label *)

CONST
(* from GLXMAC *)
    FDLEN=0;FDFIL=1;
    FPLEN=0;
    (* Object type codes *)
    OTLPT=3;
    OTBAT=4;
    OTCDP=5;				(* not card punch, Canon LGP *)
(* from QSRMAC *)
    RBBAT=1; (* code for normal queue entry *)
    EQRID=14B;
    EQLEN=4;
    EQROB=5;
    ROBTY=0;
    EQSPC=15B;
    EQJOB=11B;
    EQOWN=131B;

TYPE halfword=0..777777B;
     sixbitchar=0..77B;
     sixbit=packed array[1..6] of sixbitchar;
     asciichars=packed array[1..5] of char;
     word = packed record case integer of
        1: (full:integer);
        2: (right,left:halfword);
        3: (six:sixbit);
	4: (asc:asciichars);
     end; (* type word *)
     page = array[0..777B] of word;
     qentry = record 
	next:^qentry;
	eq:page;
     end;
     qptr=^qentry;

VAR queue:file of word;
(*    qpage:page; *)
    tries:integer;	(* Number of tries at getting consistant data*)
    request:integer;	(* Request number, to check consistancy *)
    indexpos:integer;	(* position we are at in index page *)
    indexword:word;	(* word read from index *)
    messagelen:integer; (* Length of message on current page *)
    success:boolean;
    cureq:^qentry;	(* Temp to point to EQ blocks *)
    head:^qentry;	(* Pointer to head of queue of EQ *)
    i:integer;		(* Loop counter*)

procedure sixtype(var f:file;val:word);
(* Types VAL to file in sixbit *)
var i:integer;
begin
for i:=1 to 6 do write(f,chr(val.six[i]+40B));
end;
    
PROCEDURE PASFIX(var f:file);extern;
(* This procedure sets the byte count of the file to a very large number, so
that files who's byte count is 0 can still be read by PMAP *)

Procedure asctype(var f:file;eq:page;index:integer;var i:integer);
(* Given an array with an EQ in it and and index, types that asciz string that starts at that address *)
var c:char;
begin
i:=1;
repeat
    c:=eq[index + ((i-1) div 5)].asc[((i-1) mod 5) + 1];
    if ord(c)<>0 then write(f,c); (*Stop on null*)
    i:=i+1;
until (ord(c)=0) or (i>300)
(* Stop on null, or if an unreasonable number of chars printed *)
end;

Function AscCompare(var one,two:page;index:integer):boolean;
(* Returns true if the asciz string starting at index is greater in page TWO
than in page one. *)
var c1,c2:char;
    i,j:integer;greater,less:boolean;
begin
i:=1;
(* write(tty,'Comparing "');
asctype(ttyoutput,one,index,j);
write(tty,'" with "');
asctype(ttyoutput,two,index,j);
writeln(tty,'"'); *)

greater:=false;
less:=false;
repeat				(* Repeat until we get an decision *)
    c1:=one[index + ((i-1) div 5)].asc[((i-1) mod 5) + 1];
    c2:=two[index + ((i-1) div 5)].asc[((i-1) mod 5) + 1];
    if c1>c2 then greater:=true;
    if c1<c2 then less:=true;
    i:=i+1;
until greater or less or (ord(c1)=0);
asccompare:=greater;
(* Stop if decision or strings run out [if c1=0 then c2=0 or else LESS
   would be set *)
end; (* Procedure Asccompare *)

PROCEDURE GETFIX(var f:file);
begin
GET(f);
end;

PROCEDURE SETPOSGET(var f:file;pos:integer);
(* This procedure sets the file to position POS and then does
a GET, so that f^ will be the datum at that position.  The GET
is documented to be done automatically by SETPOS, but it does
not happen that way on files of integer *)
BEGIN
SETPOS(f,pos);
(* GETFIX(f); *)
END;

Procedure TellAboutJob(eq:page);
(* Tell user about a job given EQ *)
Var i,j,k:integer;
    filecount:integer;	(* Files in this request *)
    fileptr:integer;	(* Used to point to fileblocks in request *)

Begin
(*  Old yucky output format
    case eq[eqrob+robty].full of
	otlpt: write(tty,'LPT: ');
	otbat: write(tty,'BATCH: ');  
	otcdp: write(tty,'Canon: ');
       others: write(tty,'Unknown request type: ');
    end;
    write(tty,'Request ',eq[eqrid].full:0,', job ');
    sixtype(ttyoutput,eq[eqjob]);
    write(tty,' for user ');
    asctype(ttyoutput,eq,eqown,j);
    writeln(tty);
    write(tty,'	Files:');
*)

(* New output format *)
    If eq[eqrob+robty].full = otbat then begin
	(* only print out batch requests *)
	sixtype(ttyoutput,eq[eqjob]);
    write(tty,'   ',eq[eqrid].full:4,'     ');
    j := 1;
    asctype(ttyoutput,eq,eqown,j);
    for k:= j to 12 do
	write(tty,' ');
    filecount:=eq[eqspc].right; (* Number of files in request *)
    fileptr:=eq[eqlen].right; (* Skip header to first FP *)
    for i:=1 to filecount do begin
	fileptr:=fileptr+eq[fileptr+FPLEN].left;
	    (* Skip over FP to FD *)
	if i > 1 then write(tty,'                                ');
	asctype(ttyoutput,eq,fileptr+FDFIL,j);
	If i<>filecount then writeln(tty,'              ');
	fileptr:=fileptr+eq[fileptr+FDLEN].left;
	    (* skip over FD to next FP *)
    end; (* loop over files *)
    writeln(tty);
    end;
End;

(* Procedures to do the QUICKSORT of queue entries *)

Function QCompare(one,two:page):boolean;
(* Returns true if two comes after one.  The comparison is done by looking
first at object type, then at username *)
Begin
    If one[eqrob+robty].full>two[eqrob+robty].full then qcompare:=false
    else if one[eqrob+robty].full<two[eqrob+robty].full then qcompare:=true
    else qcompare:= AscCompare(one,two,eqown);
(* Compare ascii strings starting at EQOWN, return result *)
end;
    
Function Append(first,last:qptr):qptr;
(* Appends the LAST list on to the end of the FIRST list, returns the result,
which is the same as FIRST unless FIRST is NIL *)
var cur:qptr;
Begin
If first=nil then append:=last
else begin
    cur:=first;
    while cur^.next<>nil do cur:=cur^.next;	(* Go up to the end *)
    cur^.next:=last;		(* Put ending list  *)
    append:=first;
end;
End; (* Procedure append *)

Function Qsort(list:qptr;function compare:boolean):qptr;
(* Perform a Quicksort on a Queue pointed to by LIST, by calling the
  function COMPARE on pairs of EQs.  Compare should return true if the
  second argument is greater than the first *)
var small,big,cur,next:qptr;
Begin
small:=nil; big:=nil;
If list=nil then qsort:=nil
else if list^.next=nil then qsort:=list	(* Zero or one is immediate *)
else begin
    cur:=list^.next;	(* Start out with second element *)
    while cur<>nil do begin
(*	writeln(tty,'Looking at ',ord(cur));*)
	next:=cur^.next;	(* Remember next element *)
	If compare(list^.eq,cur^.eq) then begin
	    cur^.next:=small;
	    small:=cur;	(* Smaller, goes on small list *)
	end else begin
	    cur^.next:=big;
	    big:=cur;	(* Big, goes on big list *)
	end;
        cur:=next;		(* Move to next in list *)
    end; (* while *)
big:=qsort(big,compare);
small:=qsort(small,compare);	(* Sort the sublists *)
list^.next:=big;	(* Put the dividing element on from of big list *)
qsort:=append(small,list);	(* Put the big list after the small list *)
end; (* if not trivial case *)
end; (* procedure qsort *)

BEGIN (* main program *)

RESET(queue,'PS:<SPOOL>PRIMARY-MASTER-QUEUE-FILE.QUASAR',TRUE);
(* RESET(queue,'PRIVATE-MASTER-QUEUE-FILE.QUASAR',TRUE); *)

PASFIX(queue);  (* Fix up byte count *)
head:=nil;		(* Set list of requests empty *)
For indexpos:=2 to 777B do begin (* go through valid indexes *)
    Tries:=0; (* Start couting tries at good data *)
    Repeat
	success:=false; (* Haven't succeeded yet *)
	Tries:=Tries+1; (* Count tries *)
	SETPOSGET(queue,1000B+indexpos); (* Read index word *)
	indexword:=queue^;
	if (indexword.full=0) or (indexword.left<>RBBAT)
	 then goto 9999;
	    (* Must be a valid request, and not a file delete *)
	SETPOSGET(queue,indexpos*1000B+eqrid);
	If EOF(queue) then begin
	    writeln(tty,'Page vanished while attempting to map, ignoring');
	    goto 9999
	end;
	request:=queue^.full;  (* Get request ID from page*)
	If request=0 then begin
	    Writeln(tty,'Request vanished, ignoring');
	    goto 9999;
	end;
	SETPOSGET(queue,indexpos*1000B); (* Go to start of page *)
	messagelen:=queue^.left; (* Get length for this page *)
	new(cureq);	(* Get an entry slot *)
	for i:=0 to messagelen do begin
	    cureq^.eq[i] := queue^;  GETFIX(queue);
	end;
        SETPOSGET(queue,indexpos*1000B+eqrid); (* Go to request again *)
        if request<>queue^.full then begin
	    writeln(tty,'Request changed while looking, trying again ');
	    dispose(cureq);
	end else begin (* Valid data *)
	    success:=true; (* say so *)
(*	    if cureq^.eq[eqrob+robty].full <> otbat then begin *)
		(* Not a batch entry... *)
	        cureq^.next:=head;
	        head:=cureq;	(* Insert this entry at front of list *)
(*	    end *)
	end (* If valid data *)
    until (tries>10) or success; (* Try this up to ten times *)
    If not success then writeln(tty,'Couldn''t get consistent data, giving up after 10 tries');

9999:
end; (* loop over index entries *)

(* We have read in the whole queue, now sort it *)
Head:=Qsort(head,qcompare);

(* Now print out queue in order *)
cureq:=head;
writeln(tty,'Job     Request   User       File');
writeln(tty,'------  -------   ------     -----');
while cureq<>nil do begin
    TellAboutJob(cureq^.eq);
    cureq:=cureq^.next;
end;

END.
