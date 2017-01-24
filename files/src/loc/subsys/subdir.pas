(* SRC:<LOC.SUBSYS>SUBDIR.PAS.2, 13-Nov-84 21:06:46, Edit by STACIA *)
(*  Create subdirectories from connected directory, not login *)
(*  directory.  Under NicSw. *)
(* <SU-UTILITIES>SUBDIR.PAS.5,  3-Aug-84 18:26:46, Edit by LOUGHEED *)
(* Don't set up unique usergroups at Stanford.  We use ACJ to establish *)
(*   access to subdirectories, so we don't need this proliferation of groups *)
(* <SU-UTILITIES>SUBDIR.PAS.5,  9-Mar-84 15:38:00, Edit by EJ *)
(* GETNAME bombed if only <return> typed.  Now just reprompt for name. *)
(* <SU-UTILITIES>SUBDIR.PAS.4, 21-Jul-83 13:29:37, Edit by STELLA *)
(*  Grammatical corrections *)
(* <UTILITIES>SUBDIR.PAS.3, 10-May-83 18:01:18, Edit by MRC *)
(* Changes for Stanford operation *)

{Program for creating subdirectories.  It is sufficiently hard to get
  all the crazy parameters right that our users need to be led through
  by the hand this way.  This has some dependencies on our accounting
  structure, so it may not be appropriate for you.}

{this is an old program. I would now use PASCMD for the command scanning.
  Also the string handling code is grungy.  I would now use the string
  routines in STRING, though nothing will make string handling pleasant
  in Pascal.  Have you ever thought of using SAIL?}

    const
	NicSw = True;	(* Yes, we're running this on SRI-NIC *)

TYPE
  STR = PACKED ARRAY[1:40]OF CHAR;
  array14 = packed array[1:14]of char;
  GRP = ARRAY[0:30]OF INTEGER;
  STRPT = ^ STR;
  GRPPT = ^ GRP;
VAR
DIRBLK:PACKED RECORD   (* argument block for CRDIR  *)
  LEN:INTEGER;	        (* i.e. directory attributes *)
  passpt:integer;	(* password - not used here *)
  WORK:INTEGER;		(* working quota *)
  CAP:INTEGER;		(* capabilities *)
  MODE:INTEGER;		(* files-only or not *)
  PERM:INTEGER;		(* perm. quota *)
  DIRNO:INTEGER;	(* direct. no to use (unused) *)
  DEFFILE:INTEGER;  	(* default file prot. *)
  DIRPROT:INTEGER;   	(* protection of direcotry *)
  GENS:INTEGER;		(* no. of generations to keep *)
  DATE:INTEGER;		(* date last logged in *)
  dum1:0..777777B;USERL:GRPPT;
  DUM2:0..777777B;DIRL:GRPPT;	(* list of user groups allowed to access *)
  SUBDIRS:INTEGER;	(* how many subdirectories he can have *)
  dum3:0..777777B;SUBUSL:GRPPT;	(* list of user groups he can put his sfd's in *)
  DEFACCT:integer;	(* defalt acct. for login *)
  END;
BITS:SET OF 0..35;	(* standard word to put bits from jsys in *)
CURDIR,CURUSER,I,J:INTEGER;
   (* curdir, curuser are index of current user and dir in dirl, userl *)
dirno36,uniquegroup:integer;
   (* dirno36 is 36-bit dir. no. of this user.  uniquegroup is group
	no. unique to each user, to give him access to his SFD's,
	etc.  It is a group that looks the same in decimal as his
	directory number in octal *)
S,DIRNAME:PACKED ARRAY[1:80]OF CHAR;  (* the guy we're worrying about *)
DIRLEN: INTEGER;  (* length of directory name *)

PROCEDURE QUIT; EXTERN;

function getyn:Boolean;
   var ch:char;
  begin
  loop
  write(TTY,'[Y or N] :');
  readln(tty); read(tty,ch);
  exit if ch in ['Y','N'];
  writeln(tty,'You must type Y or N');
  end;
  getyn := ch = 'Y'
  end;

function getnum(ub:integer):integer;
   var n:integer;
  begin
  loop
    write(tty,'Type ');
    for n := 1 to ub-1 do
      write(tty,n:0,', ');
    write(tty,'or ',ub:0,' :');
    readln(tty); read(tty,n);
   exit if (not eof(tty)) and (n >= 1) and (n <= ub);
    reset(tty,'',true,0,0,5);
    writeln(tty,'You must type a number between 1 and ',ub:0);
  end;
  getnum := n
  end;

function getint(ub:integer):integer;
   var n:integer;
  begin
  loop
    write(tty,'[1:',ub:0,']  :');
    readln(tty); read(tty,n);
   exit if (not eof(tty)) and (n >= 1) and (n <= ub);
    reset(tty,'',true,0,0,5);
    writeln(tty,'You must type a number between 1 and ',ub:0);
  end;
  getint := n
  end;

procedure getOwnDir;  (* gets data from main directory *)

    var
	Place: array [1:1] of integer;
	Ret: integer;

    begin   (* getOwnDir *)
	if NicSw then
	    jsys (507B (* GETJI% *), 2, Ret; -1, -1:Place, 3B (* .JIDNO *) )
	else
	    jsys(507B%getji\,2,ret;-1,-1:place,17B%logged in dir\);
	if Ret = 1 then begin
	    if NicSw then
		writeln (tty, '?Can''t find connected directory')
	    else
	        writeln (tty, '?Can''t find logged in directory');
	    quit;
	end;	(* if Ret *)
	DirNo36 := Place[1];
	jsys (241B%gtdir\;place[1],dirblk,0);
	if DirBlk.SubDirs <= 0 then begin
	    writeln(tty,'? Your quota for subdirectories is exhausted');
	    quit
	end;	(* if DirBlk.SubDirs *)
    end;    (* getOwnDir *)

PROCEDURE GETNAME;  (* reads directory name and makes sure it 
        		exists.  If not,create him with default attr's.
			This is because we need the dir. no. in order to
			make the unique group no., so he must exist *)
    VAR ret,i,slen:INTEGER; S:PACKED ARRAY[1:6]OF CHAR; sdir:packed array[1:40]of char;
  BEGIN
  jsys(41B%dirst\,2,ret;dirname,dirno36);
  if ret = 1
    then begin
    writeln(tty,'? Can''t translate directory number - lose big');
    quit
    end;
  for i := 1 to 45 do
    if dirname[i] = chr(0)
      then goto 2;
2:dirlen := i-1;
  dirname[dirlen] := '.';
  writeln(tty);
1:WRITE(TTY,'Subdirectory to create:  ',dirname:dirlen);
  READLN(TTY);READ(TTY,sdir:slen);
  if slen = 0 then goto 1;
  if slen > 39
    then begin writeln(tty,'Name is too long'); goto 1 end;
  if sdir[slen] # '>'
    then begin slen := slen+1; sdir[slen] := '>' end;
  if dirlen+slen+1 > 80
    then begin writeln(tty,'Name is too long'); goto 1 end;
  for i := 1 to slen do
    dirname[dirlen+i] := sdir[i];
  DIRNAME[DIRLEN+slen+1] := chr(0);
  JSYS(553B,-3,I;1:0,DIRNAME,0;BITS,dirno36,dirno36);
     (* rcdir - see if he exists.  If so get the direct. no. *)
  if i > 2
    then begin
    jsys(11B,3;101B,400000B:-1,0);  (* print error msg *)
    writeln(tty);
    writeln(tty,'Please fill in the subdirectory name after the dot');
    goto 1
    end;
  IF NOT (3 IN BITS)     (* non-existent *)
    then begin
	 write(tty,'Directory already exists.  Do you want to redefine its parameters? ');
         if not getyn
           then begin
           writeln(tty,'OK, then we won''t do that one.');
	   goto 1
	   end
	 end
  end;

procedure setunique;
  begin
  STRWRITE(OUTPUT,S); WRITE(dirblk.dirno:6:O);  (* get right 6 digits *)
  STRSET(INPUT,S); READ(uniquegroup);  (* and make into decimal no. *)
{Begin Stanford addition}
  uniquegroup := uniquegroup + 10000
{End Stanford addition}
     (*that, then is the "unique group" to identify just him *)
  END;

procedure setgroups;
  begin
  writeln(tty);
  writeln(tty,'What group of users do you want to have special rights over');
  writeln(tty,'files in the subdirectory?');
  writeln(tty,'  1 - you');
  writeln(tty,'  2 - other people in your group (as defined for the directory');
  writeln(tty,'      you are currently logged into)');
  writeln(tty,'  3 - both of the above');
  with dirblk do
  case getnum(3) of
    1: begin dirl^[0] := 2; dirl^[1] := uniquegroup end;
    2: if dirl^[0] >= 31
         then writeln(tty,'% Warning: too many people in your group - first 30 used');
    3: begin
       if dirl^[0] >= 30
	 then begin
         writeln(tty,'% Warning: too many people in your group - first 29 used');
	 dirl^[0] := 30;
         end;
       dirl^[dirl^[0]] := uniquegroup;
       dirl^[0] := dirl^[0] +1
       end;
   end;
  end;

procedure setrights;
  begin
  writeln(tty);
  writeln(tty,'What rights do you want this group to have?');
  writeln(tty,'  1 - to be treated completely as owners');
  writeln(tty,'  2 - to have complete rights over most existing files');
  writeln(tty,'  3 - to have read access to most files');
  writeln(tty,'By "most files" I means files that you have not ');
  writeln(tty,'specifically assigned a non-default protection to');
  with dirblk do
  case getnum(3) of
    1: begin dirprot := 777740B; deffile := 777700B end;
    2: begin dirprot := 774040B; deffile := 777700B end;
    3: begin dirprot := 774040B; deffile := 775200B end;
    end;
  end;

procedure setotherrights;
  begin
  writeln(tty);
  writeln(tty,'What access permissions do you want all other users');
  writeln(tty,'to have to files in your subdirectory?');
  writeln(tty,'  1 - read access to most files');
  writeln(tty,'  2 - no access to most files');
  writeln(tty,'By "most files" I means files to which you have');
  writeln(tty,'not specifically assigned a non-default protection.');
  with dirblk do
  case getnum(2) of
    1: deffile := deffile + 52B;
    2: ;
    end;
  end;

procedure setquotas;
  begin
  with dirblk do
    begin
    writeln(tty);
    writeln(tty,'Your main directory has the following disk space quotas:');
    writeln(tty,'  Permanent: ',perm:0);
    writeln(tty,'  Working: ',work:0);
    writeln(tty,'How much of this do you want to suballocate to the');
    writeln(tty,'subdirectory you are creating?');
{Begin Stanford deletion
    write(tty,'  Permanent '); perm := getint(perm);
    write(tty,'  Working '); work := getint(work);
 End Stanford deletion}
{Begin Stanford addition}
    write(tty,'  Disk allocation '); perm := getint(perm); work := perm;
{End Stanford addition}
    end;
  end;

BEGIN
reset(tty,'',true,0,0,5);  (* map lower case, handle data errors *)
writeln(tty,'Program to create subdirectories.  If you don''t understand');
writeln(tty,'about assigning disk space and directory groups, please type');
writeln(tty,'^C to this program and type HELP SUBDIR.');
writeln(tty);
writeln(tty,'If none of the alternatives provided seems quite right,');
writeln(tty,'choose the closest to what you want.  Details can be adjusted');
writeln(tty,'with the BUILD command.');
WITH DIRBLK DO
 BEGIN
 dum1 := 0; DUM2 := 0; dum3 := 0; passpt := 0; defacct := 0;
 LEN := 20B;
 NEW(DIRL); dirl^[0] := 30;
 new(userl); userl^[0] := 0;
 new(subusl); subusl^[0] := 0;
 getowndir;
 setunique;
 getname;
 CAP := 0;  (* capabilities irrelevant on files-only *)
 MODE := 402000000000B;  (* files-only bit *)
 DATE := 0;  (* irrelevant *)
 SUBDIRS := 0;  (* no nested sfd's *)
 passpt := 0; (* null password *)
 dirno := 0;  (* don't try to specify dir. no *)
 userl^[0] := 1;  (* not a user *)
 subusl^[0] := 1;
 defacct := 0;
(*	At Stanford we don't  need to set up usergroups for access to
        subdirectories.  We use an ACJ hook instead.
 setgroups;
 setrights;
*)
 setotherrights;
 setquotas;
 JSYS(240B%crdir\,-3,i; DIRNAME,773774B:DIRBLK,0);
 if i > 2
   then begin
   writeln(tty,'? Couldn''t create the directory.  There is probably');
   writeln(tty,'  something odd about the name you typed, but here is');
   writeln(tty,'  what the monitor complained about:');
   jsys(11B,3;101B,400000B:-1,0);  (* print error msg *)
   quit
   end;
 writeln(tty,'[Done]');
 writeln(tty);
 END
END.
