{SCORE version of GROUPE}


{NOTE: WHEN BRINGING THIS UP ON YOUR SYSTEM, LOOK THROUGHOUT THE CODE
  FOR THE WORDS 'SYSTEM SPECIFIC' AND ALTER THE LINES SPECIFIED TO FIT
  YOUR SYSTEM. THIS CODE ALSO ASSUMES THAT CRDIR IGNORES THE REQUIRED
  'SUBDIRECTORY USER GROUPS ALLOWED' WHEN ADDING USER/DIR GROUPS WHEN
  THE REQUESTER IS WHL/OPR. (Groupe ALSO ASSUMES ALL USERS OF THIS WHO
  CAN >CHANGE< GROUPS ARE WHL/OPR). Groupe WILL NOT ALWAYS BE THIS WAY}

{* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}
{Groupe - user group / directory group program                }
{ ^---------pronounced "Group-ee"                             }
{                                                             }
{   Joel P. Bion for Stanford GSB - March, 1982               }
{   Copyright (c) 1982 GSB Computer Facility, Stanford Univ.  }
{     All rights reserved				      }
{                                                             }
{"Groupe", by the way, is not a cute variant of the word      }
{"groupie", which itself would be a cute variant of the       }
{sequence of words "user group". It instead comes from the    }
{ancient Floopgaliaziann word 'greupay', whose meaning has    }
{been lost in antiquity.                                      }
{                                                             }
{ David Eppstein of Stanford University wrote the program     }
{ called ESSEX. From this I learned how to use the COMND      }
{ jsys in Pascal, and much of the COMND jsys handling in      }
{ in this program (and also the structure of the HELPXXX      }
{ procedures) comes from ESSEX.                               }
{* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}

PROGRAM Groupe;

    include 'sys:pascmd.pas';       {change back to SYS: when installed}
    {file of definitions for COMND jsys}
    {functions and procedures of the form CMxxx, TBxxx, or GJxxx}
    {and the TABLE type are defined in this file.}

    include 'sys:jsys.pas'; {change back to SYS: when installed}
    {definitions for other jsyses}
    {this file contains constants for the JSYS procedure.}

    include 'sys:extern.pas';
    {definitions for some built-in pascal functions and}
    {procedures that aren't loaded by default}


    {* * * * * * * * * * * * * * * * * * * * * * * * * * *}
    {Global constants                                     }
    {                                                     }
    {Lengths of various data types,                       }
    {Names for certain numbers in certain representations,}
    {Codes for PASCMD keywords                            }
    {* * * * * * * * * * * * * * * * * * * * * * * * * * *}

CONST

    ctrla  = 01;    {control characters}
    ctrlb  = 02;    ctrlc  = 03;    ctrld  = 04;    ctrle  = 05;
    ctrlf  = 06;    ctrlg  = 07;    ctrlh  = 08;    ctrli  = 09;
    ctrlj  = 10;    ctrlk  = 11;    ctrll  = 12;    ctrlm  = 13;
    ctrln  = 14;    ctrlo  = 15;    ctrlp  = 16;    ctrlq  = 17;
    ctrlr  = 18;    ctrls  = 19;    ctrlt  = 20;    ctrlu  = 21;
    ctrlv  = 22;    ctrlw  = 23;    ctrlx  = 24;    ctrly  = 25;
    ctrlz  = 26;    deletekey = 177B;

    turnon = true;
    turnoff = false;

    ascizlen = 50;  {length of a short string (for filenames)}
			{for comparison, only the first (Ascizlen-1) chars}
			{are used; this is not a crock - this is efficiency}
    ugrparylen = 100; {Maximum number of user groups per person - silly limit}
    crdirblklen = 18; {Size of CRDIR argument block}
    errmesslen = 100; {Size of error message buffer}

    {Internal codes corresponding to each legal command}
    abortcmd = 1;   {ABORT without updating database}
    addcmd = 2;     {ADD a user to a group}
    changecmd = 3;  {CHANGE the name of a user group}
    createcmd = 4;  {CREATE a new user/directory group}
    destroycmd = 5; {DESTROY a user/directory group}
    exitcmd = 6;    {EXIT and update database}
    forgetcmd = 7;  {FORGET about all changes in current change list}
    genercmd = 8;   {GENERATE mailing list}
    helpcmd = 9;    {HELP with Groupe}
    infocmd = 10;    {INFORMATION on a dir/user group or a directory}
    initcmd = 11;   {INIT gets data from system to make new base}
    listcmd = 12;   {List information on lineprinter - same as INIT with
		     default file = LPT:}
    removecmd = 13; {REMOVE an existing group for a directory}
    updatecmd = 14; {UPDATE data base and directories without EXITing}
    cmdsize = 14;   {number of entries in command table}


    {Subcommands for many commands dealing with user/dir groups}
    dirgrp = 1;     {directory group}
    usergrp = 2;    {user group}
    udgsize = 2;    {number of entries in this table}

    {Subcommands for Yes-or-No questions}
    yeskey = 1;     {affirmative answer}
    nokey = 2;      {negative answer}
    yesnosize = 2;  {number of entries in Y-or-N table}

    {Subcommands for ALL information subcommand}
    allbothkey = 1;         {info on both groups/dirs}
    allgroupkey = 2;        {info on all groups}
    alldirectorykey = 3;    {info on all directories}
    allsubsize = 3;

    {Subcommands for NAMES information subcommand}
    alphasortkey = 1;       {Info Names sorted by alpha}
    numbersortkey = 2;      {Info Names sorted by number}
    namesubsize = 2;

    {Subcommands for GENERATE command}
    gencombine = 1;     {Combined user group}
    gendg = 2;          {Directory groups..}
    genug = 3;          {User groups}
    gensize = 3;

    {Subcommands for INFORMATION command}
    allkey = 1;     {Info on all things}
    changeskey = 2; {Info on the changes made in the session}
    directorykey = 3;   {info on directory}
    groupkey = 4;   {Info on a single directory}
    LowGroupKey = 5; {Info on the lowest group number}
    nameskey = 6;   {Information on the name of a group}
    infosize = 6;

    {* * * * * * * * * * * * * * * * * * * * * * * * * * * *}
    {Global data types                                      }
    {                                                       }
    {strings, forms for system calls, records, action links }
    {* * * * * * * * * * * * * * * * * * * * * * * * * * * *}

TYPE

    asciz = PACKED ARRAY [1..ascizlen] OF char; {Standard string}
    ascizpnt = ^Asciz;

    errmess = PACKED ARRAY [1..errmesslen] OF char; {Error message buffer}

    ugrpary =  ARRAY [1..ugrparylen] OF integer; {User groups for CRDIR}
    ugrparypnt = ^UGrpAry;

    HalfW = 0..777777B; {Simple enough concept - 18 bits out of 36}
    HalfNames = (Left, Right);		{Names of the two halfwords}
    HalfWords = PACKED ARRAY [HalfNames] of Halfw;

    datary = PACKED ARRAY [1..9] of char; {Date array}
    timary = PACKED ARRAY [1..10] of char; {Time of day array}
    flagset = PACKED SET OF 0..35;             {Boolean flags word}
    udlpnt = ^udl;              {Standard Dir/user grp. for a directory}
    udl = RECORD
	      num : integer;  {Group number}
	      next: udlpnt;
	  END;

    grpnt = ^grp;               {Entry for master user group}
    grp = RECORD
	      num : integer;  {Group number}
	      name: asciz;    {Group name}
	      next : grpnt;
	  END;

    WorkType = (WorkNone, WorkAdd, WorkRem);
    WorkGroupType = (WorkUser, WorkDir);
    workpnt = ^work;                    {Entry for work in CRDIR}
    work = RECORD
	       dirnum:integer;     {Directory to change}
	       addrem:WorkType;
	       ugrp : integer;     {Group to change}
	       useordir:WorkGroupType;
	       next:workpnt;
	   END;

    dirpnt = ^dirrec;                   {Entry for a directory}
    dirrec = RECORD
		 dirname:  asciz;        {directory name}
		 ugrps: udlpnt;  {User groups}
		 dgrps: udlpnt;  {Directory groups}
		 next: dirpnt;   {Next directory}
	     END;    {record}


    {"Not Used" in record below means not used in Groupe}
    CrDirBlk = RECORD		{Block for CRDIR/GTDIR}
	CDLen : HalfWords;	{Length of Argument Block and more flags}
	CDPsw : AscizPnt;	{New Password for Directory - not used}
	CDLIQ : integer;	{Logged in (Working) Quota - not used}
	CDPRV : integer;	{Privledges for directory - not used}
	CDMOD : integer;	{Directory Modifiers (Files only) - not used}
	CDLOQ : integer;	{Logged Out (Permanent) Quota - not used}
	CDNum : integer;	{forced number for new directory - not used}
	CDFPt : integer;	{Default file protection - not used}
	CDDPt : integer;	{Directory protection - not used}
	CDRet : Integer;	{Default generation retention count- not used}
	CDLLD : Integer;	{Last Login Date/Time - not used}
	CDUGp : UGrpAryPnt;	{pointer to user group block}
	CDDGp : UGrpAryPnt;	{pointer to directory group block}
	CDSDQ : Integer;	{Maximum subdirectories allowed - not used}
	CDCug : UGrpAryPnt;	{Subdirectory user groups allowed - not used}
	CDDAc : AscizPnt;	{Default account string - not used}
	CDDNE : Integer;	{default ONline exp. date/time}
	CDDFE : Integer;	{default OFFline exp. date/time}
      END; {CrDirBlk}
    CRDirBlkpnt = ^CRDirBlk;

    byteptr = integer;      {how do I make arbitrary byte pointers?}
    grplist = integer;      {would be very nice to understand groups}


    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}
    {Global variables                                                 }
    {                                                                 }
    {Constants that PASCAL won't let me declare, throwaway variables, }
    {execution parameters, stuff for PASCMD                           }
    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}

VAR
    initflag : Boolean;      {TRUE if INIT done}
    credesflag : boolean;   {TRUE if Change/Create/Destroy command done}
    ingrp:text;             {Group input file}
    outgrp:text;            {Group output file}
    f: text;                {temporary file}
    ignore: integer;        {for executing functions as procedures}
    nullasciz: asciz;       {for initializing strings}
    innullasciz:asciz;      {Forusrname}
    username: asciz;        {name of user running this program}
    dirbrkmask: brkmsk;     {for parsing directories as fields}
    usrbrkmask: brkmsk;     {for parsing usernames as fields}
    globUGP : UGrpAryPnt;   {Global temporary group arrays}
    GlobDGP : UGrpAryPnt;   {This crud from pascals not keeping variables
			     intact between procedure calls, including NEWed}
    iswheel: boolean;	    {True if user is a wheel}
    dirstr:   asciz;        {string of dir type for all directories}

    newmsgfile: asciz;      {file for confirmation msg for new dir}
    renmsgfile: asciz;      {file for confirmation msg for renewal}

    defaultgrp: asciz;      {"current" group number}
    newdefgrp: asciz;       {will become current after confirm}
    
    defaultkind: asciz;     {"current" group kind (dir/user)"}
    newdefkind: asciz;      {will become current after confirm}

    defaultdir: asciz;      {"current" directory name}
    newdefdir: asciz;      {will become current after confirm}

    allsubtable : table;    {Table in info ALL subcommands}
    cmdtable:   table;      {table of main commands}
    gentable: table;        {table of GENERATE subcommands}
    infotable: table;       {Table of info commands}
    namessubtable: table;   {Table of info NAMES subcommands}
    yesnotable: table;      {table for YES and NO}
    udgtable:   table;      {table for user / dir goups}
    nevtable:   table;      {table for dates (only one entry)}

    diruse : dirpnt;        {Lists of entries}
    dirend  : dirpnt;	    {Pointer to the end of DIRUSE}
    grpuse : grpnt;
    grpend : grpnt;	    {Pointer to the end of GRPUSE}
    workuse : workpnt;

    dirheap : dirpnt;       {Garbage heaps}
    grpheap : grpnt;
    udlheap : udlpnt;
    workheap : workpnt;
    nums : set of '0'..'9';
    ttyhld : flagset;


    {Procedures for manipulating CRDIR/GTDIR calls - quite complex}

PROCEDURE NumToString(GNum : integer;VAR OutS:asciz);
    {Converts GNum to a string, trailing spaces}

    VAR Hold: asciz;
	I,J,K : Integer;
	Num : integer;

    BEGIN
    Hold := NullAsciz;
    OutS := NullAsciz;
    I := AscizLen;
    Num := GNum;
    WHILE Num  <> 0 DO 
	BEGIN
	Hold[I] := Chr(Ord('0') + Num MOD 10);
	I := I - 1;
	Num := Num DIV 10;
	END;
    K := I + 1;
    J := 1;
    FOR I := K to AscizLen DO
	BEGIN
	OutS[J] := Hold[I];
	J := J + 1;
	END;
    END; {NumToString}


PROCEDURE Convtostring (num: integer; VAR str: asciz);
    {Convert a directory number to a ST:<USERNAME> string, IN NULL ENDING
    (Out) FORMAT!}

    VAR
	tmpstr:asciz;

    BEGIN   {convToString}
    str := innullasciz;                 {Set string to nulls}
    jsys (dirst,2; str, num);           {Dirst does Number => string}
    END;    {convToString}

PROCEDURE Convdevdirtostring (dev, num: integer; VAR str: asciz);
    {Convert a directory number to a ST:<USERNAME> string, IN NULL ENDING
    (Out) FORMAT!. Device number is merged in.}

    VAR
	tmpstr:asciz;

    BEGIN   {convDevDirToString}
    str := innullasciz;                 {Set string to nulls}
    jsys (dirst,2; str, dev:num);           {Dirst does Number => string}
    END;    {convDevDirToString}


PROCEDURE InitCRDIRBlk(Var CBP : CrDirBlkPnt);
    {This procedure will initialize a CrDirBlk}

    BEGIN
    WITH CBP^ DO		{Initialize the CrDirBlk}
	BEGIN
	CDLen[Left] := 0;		CDLen[Right] := CrDirBlkLen;
	CDPsw := nil;	CDLIQ := 0;	CDPRV := 0;	CDMOD := 0;
	CDLOQ := 0; 	CDNum := 0;	CDFPt := 0;	CDDPt := 0;
	CDRet := 0;	CDLLD := 0;     CDUGp := Nil;	CDDGp := Nil;
	CDSDQ := 0; 	CDCug := Nil;	CDDAc := Nil;	CDDNE := 0;
	CDDFE := 0;
      END; {With}
    END; {InitCRDIRBlk}


PROCEDURE InitUGrpAry(VAR UGAP : UgrpAryPnt);
    {Initialize a UserGroup List}

    VAR
	I : Integer;

    BEGIN
    UGAP^[1] := UgrpAryLen;		{First word is block length}
    FOR I := 2 TO UgrpAryLen DO		{rest are set to zero}
	UGAP^[I] := 0;
    END; {InitUGrpAry}    
    

PROCEDURE DoGTDIR(DirNum : Integer; VAR C : CrDirBlkPnt;
                  VAR UG, DG : UGrpAryPnt;Devnum : integer);
    {Does a GTDIR - returns all bits set in C^.CDLEN if the GTDIR fails.
     If Devnum = 0, device number is already in DirNum - else, we merge
     it in.}

    VAR
	RetVal : Integer;		{return +? on jsys}
    
    BEGIN
    C^.CDUGP := UG;
    C^.CDDGP := DG;
    IF (DevNum = 0) THEN
	Jsys(GTDIR,-2,RetVal;DirNum, C, 0)
    ELSE
	Jsys(GTDIR,-2,RetVal;Devnum:dirnum,C,0);
    IF (RetVal = 3) THEN		{RetVal = 3 means ERJMP}
	BEGIN
	C^.CDLen[Left] := 777777B;
	C^.CDlen[Right] := 777777B;
	END; {IF}
    END; {DoGTDir}
    	
PROCEDURE DoCRDIR(DirNum : Integer; VAR C : CrDirBlkPnt;
                  VAR UG, DG : UGrpAryPnt);
    {Does a GTDir - Then merges the arrays of groups indicated into the list
     given by the GTDIR. If Any of UG and DG's elements are NEGATIVE, then we
     remove the user from that specified directory/user group}

    CONST
	CRDirBits = 400140B;	{CD%UGP + CD%DGP - set groups up}
	CDNSQBit = 400000B;	{CD%NSQ for no update to superior quotas}
   
    VAR
	DirStr : Asciz;			{ASCII Directory string}
	RetVal : Integer;		{Return Skip val. for Jsys}



    PROCEDURE CleanUGary(VAR Old, Newer: UGrpAryPnt);
	{Add an occurrance of or remove the occurrance of entries in new
         to/from entries in old. If Number in New is positive, add it. If
	 it is negative, remove it. The code assumes that at least
         New[UGrpAryLen] = 0}

	VAR
	    I , J : Integer;		{Loopcounters}
	    Found : Boolean;		{Random boolean}
	    ABSGrp : Integer;		{Absolute of current group}
    
	BEGIN
	I := 1;
	WHILE (Newer^[I] <> 0) DO		{process each one}
	    BEGIN
	    Found := False;
	    J := 2;
	    ABSGrp := ABS(Newer^[I]);
	    WHILE (Not(Found) AND (Old^[J] <> 0)) DO
		BEGIN
		IF (Old^[J] = ABSGrp) THEN
		    Found := True
		ELSE
		    J := J + 1;
		END; {While}
	    IF (Newer^[I] > 0) THEN	{If to ADD to list...}
		BEGIN
		IF Not(Found) THEN	{add it... j is at new end of list}
		    Old^[J] := Newer^[I]
		END
	    ELSE			{If to REMOVE from list}
		BEGIN
		WHILE (Old^[J] <> 0) DO	{...move all up one}
		    BEGIN
		    Old^[J] := Old^[J+1];
		    J := J + 1;
		    END; {While}
		END; {Else}
	    I := I + 1;			{Check next entry}
	    END; {While}	    
	END; {CleanUGary}

    PROCEDURE CrDirErr;
	{Prints error message about CRDIR}

	VAR
	    MessHld : ErrMess;           {Holds error message}
	    SaveFile : Text; 		 {File to save self in}
	    SaveRetVal : Integer;	 {Return from SAVE Jsys}
	
	PROCEDURE WriteUGary(UP : UGrpAryPnt);
	    VAR J : Integer;
	    BEGIN
	    J := 1;
	    While (UP^[J] <> 0) DO
		BEGIN
		Write(tty,UP^[J]);
		J := J+1;
		END;
	    END; {WriteUGary}

	BEGIN {CrDirErr}
	Writeln(tty);
	Writeln(tty,'Unexpected CRDIR error:');
	Jsys (ErStr,4; MessHld, fhslf:-1, ErrMessLen);
	Jsys (ESout,3; MessHld);
	Writeln(tty);
	Writeln(tty,'Unable to do CRDIR Jsys for directory ',DirNum);
	Write(tty,'UG=');WriteUGary(UG);Writeln(tty);
	Write(tty,'CurUG=');WriteUGary(GlobUGP);Writeln(tty);
	Write(tty,'DG=');WriteUGary(DG);Writeln(tty);
	Write(tty,'CurDG=');WriteUGary(GlobDGP);Writeln(tty);
	Writeln(tty);
	Writeln(tty,'Please add or remove this group manually with the EXEC build command.');
	Writeln(tty,'In doing so, please try to see what is wrong. There is a good');
	Writeln(tty,'chance that the working quotas of the directory being modified');
	Writeln(tty,'(or of any of its superiors) are under the current disk useage,');
	Writeln(tty,'so you may want to check these out.');
	Writeln(tty);    
	Writeln(tty,'Groupe will now attempt to save itself in the SYSTEM: directory');
	Writeln(tty,'as GROUPE-CRASH.EXE - please send mail to Joel Bion telling him');
	Writeln(tty,'about this so he can look into it. Thank you.');
	Writeln(tty);
	Write(tty,'Saving self as SYSTEM:GROUPE-CRASH.EXE ');
        Jsys(GTJFN,2,SaveRetval;1:0,-1:'SYSTEM:GROUPE-CRASH.EXE';SaveFile);
			       {1:0 = GJ%SHT}
	Jsys(SAVE,-2,SaveRetVal;FhSlf:SaveFile,777760B:20B);
	IF (SaveRetVal = 3) THEN		{SAVE Error?}
	    BEGIN
	    writeln(tty,'...failed. Oh well. No file written.');
	    writeln(tty,'Please tell Joel about this too...');
	    END {If SAVE Error}
	ELSE
	    Writeln(tty,'[OK]');
	writeln(tty,'Groupe will try to continue. There may be more errors.');
	END; {CrDirErr}

    BEGIN {DoCrDir}
    InitUGrpAry(GlobUGP);		{Initialize user/dir group list}
    InitUGrpAry(GlobDGP);
    InitCRDirBlk(C);
    DoGTDir(DirNum, C, GlobUGP, GlobDGP,0);
    IF Not((C^.CDLen[Left] = 777777B) AND (C^.CDLen[Right] = 777777B)) THEN
	BEGIN
	CleanUGary(GlobUGP, UG);		{fix up user group list}
	CleanUGary(GlobDGP, DG);		{fix up directory group list}
	ConvToString(DirNum, DirStr);	{get the string for this dir. num.}
	C^.CDUGP := GlobUGP;		{reset blocks...}
	C^.CDDGP := GlobDGP;		{...pointers}
	GLobUGP^[1] := UGrpAryLen;	{reset lengths}
	GlobDGP^[1] := UGrpAryLen;	{reset lengths}
	C^.CDLen[Left] := CDNSQBit;
	C^.CDLen[Right] := CrDirBlkLen;
	Jsys(CRDIR,-2,RetVal;DirStr,CRDirBits:C, 0); {the CRDIR Jsys!}
	IF (RetVal = 3) THEN		{CRDIR Error?}
	    CrDirErr;	
	END; {If GTDIR okay}
    END; {DoCRDir}

    {This doesn't fit anywhere else}

PROCEDURE ContNo;
    {Halts, with no continue}
    
    BEGIN
    WHILE true DO
	BEGIN
        Jsys(Haltf,2);
        Writeln(tty,'Please resume Groupe at this point by using the START command.');
	END; {While}
    END; {ContNo}


PROCEDURE TimDay(VAR Time:timary);
    {Gets time of day in form "HH:MM:SS  "}

    CONST 
	OTNDaFlag = 0; {No Date - Time Only}
    VAR
	TimeWord: flagset;

    BEGIN
    TimeWord := [OTNDaFlag];
    Jsys (OdTim,2; Time, -1, TimeWord);
    END; {TimDay}


    {Miscellaneous string manipulation routines}

FUNCTION RightHalf(a : Flagset):Integer;
    {Gets us back the right half of A as the result}

    VAR
	I : Integer;
	Result : Integer;

    BEGIN
    Result := 0;
    FOR I := 18 to 35 DO
	BEGIN
	IF I in A THEN
	    Result := Result*2+1
	ELSE
	    Result := Result*2
	END; {For}
    RightHalf := Result;
    END; {RightHalf}


FUNCTION Width(a : integer):integer;
    {returns a printing width for A}

    BEGIN
    Width := Trunc(Log(a) + 1);             {Ie: Trunc(Log(100->999)) = 2}
    END; {Width}

FUNCTION Getsig(str:asciz):integer;
    {Gets the significant character length of STR. (Ie: trailing spaces
     and nulls dropped)}

    VAR
	signif : integer;           {Temp. result}
	good :boolean;              {True if non-space hit}

    BEGIN
    signif := ascizlen;
    good := false;
    WHILE ((NOT good) AND (signif > 0)) DO  {Ie: trailing spaces}
	BEGIN
	good := ((str[signif] <> ' ') AND (Str[signif] <> CHR(0)));
	IF NOT good THEN
	    signif := signif -1;
	END; {if}
    Getsig := signif;
    END; {GetSig}


PROCEDURE Writeasciz (VAR dest: FILE; str: asciz);
    {Write an asciz string to DEST, trailing spaces cut off}

    VAR
	index: integer;
	signif: integer;

    BEGIN
    signif := Getsig(str);
    FOR index := 1 TO signif DO         {Write the string}
	Write(dest,str[index]);
    END; {writeAsciz}




FUNCTION SEQ(a,b : asciz) : Boolean;
    {True if A = B}

    CONST LTFlag = 0;  {Sc%Lss}
	  SUBFlag = 1; {sc%Sub}
	  GTFlag = 2;  {sc%GTR}
	  
    VAR
	result : boolean;       {Temporary}
	resval : flagset;

    BEGIN
    a[ascizlen] := Chr(0); b[ascizlen] := Chr(0);
    jsys(STCMP,2;a,b;resval);
    SEQ :=Not ((LTFlag IN ResVal) or (SUBFlag in ResVal)
	       or (GTFlag in ResVal));
    END; {SEQ}



FUNCTION SGREQ(a, b : asciz):boolean;

    {Returns true if A >= B}
    {IGNORES CASE!}

    CONST LTFlag = 0;  {Sc%Lss}

    VAR
	result : boolean;       {Temporary}
	resval : flagset;

    BEGIN
    a[ascizlen] := Chr(0); b[ascizlen] := Chr(0);
    jsys(STCMP,2;a,b;resval);
    SGREQ := Not(LTFlag IN ResVal);
    END; {SGREQ}

FUNCTION SGT(a, b : asciz):boolean;

    {Returns true if A > B}
    {IGNORES CASE!}

    CONST GTFlag = 2;  {Sc%Gtr}

    VAR
	result : boolean;       {Temporary}
	resval : flagset;

    BEGIN
    a[ascizlen] := Chr(0); b[ascizlen] := Chr(0);
    jsys(STCMP,2;a,b;resval);
    SGT := (GTFlag IN ResVal);
    END; {SGT}

FUNCTION SLT(a, b : asciz):boolean;

    {Returns true if A < B}
    {IGNORES CASE!}

    CONST LTFlag = 0;  {Sc%Lss}

    VAR
	result : boolean;       {Temporary}
	resval : flagset;

    BEGIN
    a[ascizlen] := Chr(0); b[ascizlen] := Chr(0);
    jsys(STCMP,2;a,b;resval);
    SLT := (LTFlag IN ResVal);
    END; {SLT}



PROCEDURE Clearasciz (VAR str: asciz; point: integer);
    {Fills an ASCIZ string with spaces}

    BEGIN
    WHILE point <= ascizlen DO BEGIN            {Fill array}
	str[point] := ' ';
	point := point + 1;
	END;
    {while}
    END; {clearAsciz}






PROCEDURE Outtoin (src:asciz;VAR dest:asciz);
    {Converts trailing nulls to trailing spaces. NOTE THAT ALL OF
    Groupe'S DATA IS ASSUMED TO HAVE TRAILING SPACES. THIS IS FOR
    TALKING WITH SYSTEM ROUTINES.}

    VAR
	index:integer;          {LoopCounter}

    BEGIN
    dest := nullasciz;              {Set up clear}
    index := 1;
    WHILE (index < ascizlen) AND (src[index] <> Chr (0)) DO
	BEGIN
	dest[index] := src[index];  {While characters, copy}
	index := index + 1;
	END;
    {While}
    END; {OutToIn}



PROCEDURE Intoout (src:asciz;VAR dest:asciz);
    {Converts trailing spaces to trailing nulls}

    VAR
	index,
	sig:integer;                {# of sig. characters}

    BEGIN
    sig := Getsig(src);
    FOR index := 1 TO sig DO            {Copy string into word}
	dest[index] := src[index];
    FOR index := (sig+1) TO ascizlen DO     {Fill rest with nulls}
	dest[index] := Chr(0);
    END; {InToOut}






PROCEDURE PrintHeader;
    {Print the beginning of the program header onto the user's terminal}

    VAR
        thetime : timary;	    {time of day}
        thehour : integer;

    BEGIN
    TimDay(TheTime);
    TheHour := Ord(TheTime[2]) - Ord('0');
    If (TheTime[1] = '1') THEN
        TheHour := TheHour + 10;
    If (TheTime[1] = '2') THEN
        TheHour := TheHour + 20;
    Write(tty,'Good ');
    IF TheHour > 16 THEN
        Write(tty,'evening')
    ELSE IF TheHour > 11 THEN
	Write(tty,'afternoon')
    ELSE
	Write(tty,'morning');
    Write(tty,', and welcome to Groupe.');
    IF (thehour < 7) THEN
	writeln(tty,' (Up early, (or late) aren''t we?)')
    ELSE
	writeln(tty);
    writeln(tty,'Type HELP for help. Type ''?'' at any time if you are unsure of what to do.');
    END; {PrintHeader}



PROCEDURE Writefas (VAR f:FILE;str:asciz);
    {Write an entire asciz string to the file mentioned}

    VAR
	index : integer;

    BEGIN
    FOR index := 1 TO ascizlen DO   {Write the string}
	Write(f,str[index]);
    END; {WriteFas}



PROCEDURE Readfas (VAR f:FILE;VAR str:asciz);
    {Read an entire string from the file specified}

    VAR
	index : integer;
	tempch: char; {Ugh! Pas20 won't let us read into packed arrays...}

    BEGIN
    FOR index := 1 TO ascizlen DO   {Read the string}
	BEGIN
	Read(f,TempCh);
	Str[index] := TempCh;
	END; {For}
    END; {ReadFas}




{* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}
{Garbage collecting                                                 }
{* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}

{Only the first set of code is commented. The others follow}


{Garbage collecting for master GROUPS}

FUNCTION Grpempty(head : grpnt): boolean;
    {See if the list given is empty}

    BEGIN
    Grpempty := (head = NIL);
    END; {grpempty}


PROCEDURE Tossgrp(VAR thing : grpnt);
    {Throw away THING}

    BEGIN
    thing^.next := grpheap;     {Put thing on top of heap}
    grpheap := thing;
    END; {TossGrp}


PROCEDURE Newgrp(VAR thing : grpnt);
    {Get a record for THING. Also initializes the THING record}

    BEGIN
    IF Grpempty(grpheap) THEN           {If nothing on heap, make a new one}
	New(thing)
    ELSE
	BEGIN
	thing := grpheap;           {Else take off top of heap}
	grpheap := grpheap^.next;
	END;
    {else}
    thing^.num := -1;                   {Initialize the THING record}
    thing^.name := innullasciz;
    thing^.next := NIL;
    END; {NewGrp}

PROCEDURE Addgrp(VAR thing : grpnt);
    {Add THING to the universal group list. Put it in alphabetical order
    (ie: insertion sort)}

    VAR prev,                           {Where to add it after}
	cur : grpnt;                {Eventually, place after where it goes}
	hit : boolean;

    BEGIN
    IF Grpempty(grpuse) THEN            {If empty list, toss it on}
	BEGIN
	grpuse := thing;
	thing^.next := NIL;
        grpend := thing;
	END {If}
    ELSE                        {Not empty...}
	BEGIN
        IF (grpend^.num < thing^.num) THEN
	    BEGIN
	    grpend^.next := thing;
	    thing^.next := nil;
	    grpend := thing;
	    END {If}
	ELSE
	    BEGIN
	    IF (grpuse^.num > thing^.num) THEN  {Put it right in front?}
		BEGIN
		thing^.next := grpuse;  {yes!}
		grpuse := thing;
		END {if}
	    ELSE
		BEGIN
		prev := grpuse;         {Setup}
		cur := grpuse^.next;
		hit := false;
		WHILE (cur <> NIL) AND (NOT hit) DO     {Find where to put it}
		    BEGIN
		    IF cur^.num > thing^.num THEN
			hit := true             {Put it HERE!}
		    ELSE
			BEGIN
			prev := cur;    {Advance pointers}
			cur := cur^.next;
			END; {Else}
		    END; {While}
		thing^.next := cur;             {Insert THING}
		prev^.next := thing;
		END; {Else}
	    END; {Else}
       END; {Else}
     END; {AddGrp}



    {Code for the Directory list}

FUNCTION Dirempty(head : dirpnt): boolean;
    BEGIN {dirempty}
    Dirempty := (head = NIL);
    END;

PROCEDURE Tossdir(VAR thing : dirpnt);
    BEGIN
    thing^.next := dirheap;
    dirheap := thing;
    END;

PROCEDURE Newdir(VAR thing : dirpnt);
    BEGIN
    IF Dirempty(dirheap) THEN
	New(thing)
    ELSE
	BEGIN
	thing := dirheap;
	dirheap := dirheap^.next;
	END;
    thing^.dirname := innullasciz;
    thing^.ugrps := NIL;
    thing^.dgrps := NIL;
    thing^.next := NIL;
    END;

PROCEDURE Adddir(VAR thing : dirpnt);
    VAR prev,
	cur : dirpnt;
	hit : boolean;

    BEGIN
    IF Dirempty(diruse) THEN
	BEGIN
	diruse := thing;
	thing^.next := NIL;
	dirend := thing;
	END
    ELSE
	BEGIN
        IF SGREQ(thing^.dirname,dirend^.dirname) THEN
	     BEGIN
	     dirend^.next := thing;
	     thing^.next := nil;
	     dirend := thing;
	     END
	ELSE
	BEGIN
	IF SGREQ(diruse^.dirname,thing^.dirname) THEN
	    BEGIN
	    thing^.next := diruse;
	    diruse := thing;
	    END
	ELSE
	    BEGIN
	    hit := false;
	    prev := diruse;
	    cur := diruse^.next;
	    WHILE (cur <> NIL) AND (NOT hit) DO
		BEGIN
		IF SGREQ(cur^.dirname, thing^.dirname) THEN
		    hit := true
		ELSE
		    BEGIN
		    prev := cur;
		    cur := cur^.next;
		    END;
		END;
	    thing^.next := cur;
	    prev^.next := thing;
	    END;
	END;
      END;
    END;


    {Code for User/Dir groups in a directory}
 

FUNCTION Udlempty(head : udlpnt): boolean;
    BEGIN {udlempty}
    Udlempty := (head = NIL);
    END;

PROCEDURE Tossudl(VAR thing : udlpnt);
    BEGIN
    thing^.next := udlheap;
    udlheap := thing;
    END;

PROCEDURE Newudl(VAR thing : udlpnt);
    BEGIN
    IF Udlempty(udlheap) THEN
	New(thing)
    ELSE
	BEGIN
	thing := udlheap;
	udlheap := udlheap^.next;
	END;
    thing^.num := -1;
    thing^.next := NIL;
    END;


PROCEDURE Addudl(VAR dirtoadd:udlpnt; VAR thing : udlpnt);

    VAR prev,
	cur : udlpnt;
	hit : boolean;

    BEGIN
    IF Udlempty(dirtoadd) THEN
	BEGIN
	dirtoadd := thing;
	thing^.next := NIL;
	END
    ELSE
	BEGIN
	IF (dirtoadd^.num > thing^.num) THEN
	    BEGIN
	    thing^.next := dirtoadd;
	    dirtoadd := thing;
	    END
	ELSE
	    BEGIN
	    prev := dirtoadd;
	    cur := dirtoadd^.next;
	    hit := false;
	    WHILE (cur <> NIL) AND (NOT hit) DO
		BEGIN
		IF cur^.num > thing^.num THEN
		    hit := true
		ELSE
		    BEGIN
		    prev := cur;
		    cur := cur^.next;
		    END;
		END;
	    thing^.next := cur;
	    prev^.next := thing;
	    END;
	END;
    END;


    {Code for adding work - slightly different}


FUNCTION Workempty(head : workpnt): boolean;
    BEGIN {workempty}
    Workempty := (head = NIL);
    END;

PROCEDURE Tosswork(VAR thing : workpnt);

    BEGIN
    thing^.next := workheap;
    workheap := thing;
    END;


PROCEDURE Tossallwork(VAR Start :Workpnt);
    {Forget about all of our work}
VAR
    Tempp : WorkPnt;

    BEGIN
    WHILE Start <> NIL DO {Toss the links we created}
	BEGIN
        tempp := start^.next;
	TossWork(Start);
	Start := tempp;
	END; {While}
    END; {TossAllWork}


PROCEDURE Newwork(VAR thing : workpnt);

    BEGIN
    IF Workempty(workheap) THEN
	New(thing)
    ELSE
	BEGIN
	thing := workheap;
	workheap := workheap^.next;
	END;
    thing^.AddRem := WorkNone;
    thing^.Dirnum := 0;
    Thing^.UseOrDir := WorkUser;
    thing^.UGrp := 0;
    thing^.next := NIL;
    END;

PROCEDURE Addwork(direcnum, grp : Integer; grpusedir : WorkGroupType;
                  addrm: WorkType);
    {Add work to the END of the heap to avoid errors!  Slow, but won't be
    done that often.}

    VAR t:workpnt;
	prev,
	cur: workpnt;

    BEGIN
    NewWork(t);
    WITH t^ DO                  {Set up work record}
	BEGIN
	next := NIL;
	dirnum:= direcnum;  {Directory to change}
	addrem:= addrm;     {Add or remove group?}
	ugrp := grp;        {Group number}
	useordir:= grpusedir;  {User or directory group?}
	END; {With}
    IF WorkEmpty(WorkUse) THEN
	workuse := t
    ELSE
	BEGIN               {Add it to the end - code obvious}
	T^.Next := WorkUse;
	WorkUse := T;
	END; {Else}
    END; {AddWork}


    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}
    {Initialization of variables                                        }
    {                                                                   }
    {This procedure is run at start-up time and sets all global         }
    {variables that need initial values.  Many of these are changed     }
    {later and so can not be set in an INITPROCEDURE.                   }
    {                                                                   }
    {All keyword tables for the PASCMD package are initialized here.    }
    {The long tables each have subprocedures associated with them,      }
    {and the one-word ones are all initialized by another subprocedure. }
    {                                                                   }
    {If the user is not a WHEEL or OPERATOR, this routine will complain }
    {and stop the program.                                              }
    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}

    {Read in file. File has format:

    Group Num1
    Group name1
    ...
    -1
    UserName1
    UG1
    ...
    -1
    DG1
    ...
    -1                 }

PROCEDURE Readfile;

    CONST
	GJBits = 100001000000B;	 {GJ%OLD+GJ%SHT}

    VAR
	tnum : integer;
	td : dirpnt;
	tg : grpnt;
	tu : udlpnt;
	RetVal : Integer;
    
    BEGIN
    Jsys(GTJFN,2,Retval;GJBits,-1:'SYSTEM:GROUPE.BIN');
    IF (RetVal <> 2) THEN
	BEGIN
	IF Not(IsWheel) THEN {You lose...}
	    BEGIN
	    writeln(tty,'Cannot get access to the system group data file. Please report this');
	    writeln(tty,'to the computer facility.');
	    Contno;
	    END {IF}
        ELSE                 {Give WHL/OPR a chance to create one}
	    BEGIN
	    writeln(tty,'Cannot get access to the system group data file, SYSTEM:GROUPE.BIN.');
	    writeln(tty,'If this is your first run of Groupe, or if you have lost the data file,');
	    writeln(tty,'then please give the INITIALIZE command, follwed by the EXIT');
	    writeln(tty,'command. This sequence will create a system group data file');
	    writeln(tty,'for you that will be right up to date.');
	    Reset(Ingrp);
	    END; {Else is a WHEEL}
	END {If file not found}
    ELSE  {File was found}
	Reset(ingrp,'SYSTEM:GROUPE.BIN');
    IF NOT(Eof(ingrp)) THEN
	BEGIN
	REPEAT
	    Read(ingrp,tnum);
	    IF tnum <> -1 THEN
		BEGIN
		Newgrp(tg);
		WITH tg^ DO
		    BEGIN
		    num := tnum; Readfas(ingrp,name);
		    END;
		Addgrp(tg);
		END;
	UNTIL tnum = -1;
	WHILE NOT(Eof(ingrp)) DO
	    BEGIN
	    Newdir(td);
	    Readfas(ingrp,td^.dirname);
	    REPEAT
		Read(ingrp,tnum);
		IF tnum <> -1 THEN
		    BEGIN
		    Newudl(tu);
		    tu^.num := tnum;
		    Addudl(td^.ugrps,tu);
		    END;
	    UNTIL tnum = -1;
	    REPEAT
		Read(ingrp,tnum);
		IF tnum <> -1 THEN
		    BEGIN
		    Newudl(tu);
		    tu^.num := tnum;
		    Addudl(td^.dgrps,tu);
		    END;
	    UNTIL tnum = -1;
	    Adddir(td);
	    END; {While}
	END; {if}
    END; {ReadFile}



PROCEDURE Writefile(VAR outgrp : Text);
    {Write out the data file from the pointers. See READFILE for format}

VAR
    G : Grpnt;
    D : DirPnt;
    U : UdlPnt;

    BEGIN
    G := GrpUse;
    WHILE g <> NIL DO BEGIN
	Write(outgrp,g^.num);
	Writefas(outgrp,g^.name);
	g := g^.next;
	END;
    Write(outgrp,-1);
    D := DirUse;
    WHILE D <> NIL DO BEGIN
	WITH D^ DO BEGIN
	    Writefas(outgrp,dirname);
	    U := UGrps;
	    WHILE U <> NIL DO BEGIN
		Write(outgrp,U^.num);
		U := U^.next;
		END;
	    Write(outgrp,-1);
	    U := DGrps;
	    WHILE U <> NIL DO BEGIN
		Write(outgrp,U^.num);
		U := U^.next;
		END;
	    Write(outgrp,-1);
	    END; {with}
	D := D^.next;
	END; {While}
    rclose(outgrp);
    END; {WriteFile}



PROCEDURE Initialize;
    {do all one-time initialization}

    CONST
	abbreviation = 5b;      {CM%ABR!CM%INV}
	invisible = 1b; {CM%INV}

    TYPE
	char1 = PACKED ARRAY [1..1] OF char;    {for abbrevs}


    PROCEDURE Checkcapabilities;    {user must be a wheel or operator}

	CONST
	    wheelcaps = 18; {SC%WHL}
	    oprcaps = 19;   {SC%OPR}

	VAR
	    capsword: flagset;

	BEGIN   {checkCapabilities}
	jsys (rpcap; fhslf; ignore, capsword);
	iswheel:= ((wheelcaps IN capsword) OR (oprcaps IN capsword));
	jsys (epcap; fhslf, capsword, capsword);        {enable}
	END;    {checkCapabilities}


    PROCEDURE Tbaddchr (tab: table; data: integer; c: char; bits: integer);

	VAR
	    charstr: PACKED ARRAY [1..1] OF char;

	BEGIN   {tbAddChr}
	charstr[1] := c;
	tbadd (tab, data, charstr, bits);
	END;    {tbAddChr}


    PROCEDURE Buildallsub;
	{Build table for INFO ALL subcommands}

	BEGIN
	allsubtable := tbmak(allsubsize+1);
	tbadd (allsubtable, allbothkey, 'BOTH-DIRECTORIES-AND-GROUPS',0);
	tbadd (allsubtable, alldirectorykey, 'DIRECTORIES',0);
	tbadd (allsubtable, allgroupkey, 'GROUPS',0);
	END;


    PROCEDURE Buildgen;
	{Build table for GENERATE subcommands}

	BEGIN
	gentable := tbmak(gensize+1);
	tbadd (gentable, gencombine, 'COMBINED-DIRECTORY-AND-USER-GROUP',0);
	tbadd (gentable, gendg, 'DIRECTORY-GROUP',0);
	tbadd (gentable, genug, 'USER-GROUP',0);
	END;


    PROCEDURE Buildinfo;
	{Build table of INFO subcommands}

	BEGIN
	infotable := tbmak(infosize+1);
	tbadd (infotable, allkey, 'ALL',0);
	tbadd (infotable, changesKey, 'CHANGES',0);
	tbadd (infotable, directorykey, 'DIRECTORY',0);
	tbadd (infotable, groupkey, 'GROUP',0);
	tbadd (infotable, lowgroupkey, 'LOWEST-GROUP-NUMBER',0);
	tbadd (infotable, nameskey, 'NAMES-OF-GROUPS',0);
	END;

     PROCEDURE Buildnames;
	{Build table of info NAMES subcommands}

	BEGIN
        namessubtable := TBMak(NameSubSize+1);
	tbadd (namessubtable, alphasortkey, 'ALPHABETICALLY',0);
	tbadd (namessubtable, numbersortkey, 'NUMERICALLY',0);
	END;

    PROCEDURE Buildcmd;
	{build table of main commands}

	CONST
	    numextra = 3;   {number of synonyms and abbreviations}

	BEGIN   {buildCmd}
	cmdtable := tbmak (cmdsize+numextra+1);
        if iswheel then
	    tbadd (cmdtable, abortcmd, 'ABORT', 0);
        if iswheel then
	    tbadd (cmdtable, addcmd, 'ADD', 0);
        if iswheel then
	    tbadd (cmdtable, changecmd, 'CHANGE', 0);
        if iswheel then
	    tbadd (cmdtable, createcmd, 'CREATE', 0);
        if iswheel then
	    tbadd (cmdtable, destroycmd, 'DESTROY', 0);
	tbadd (cmdtable, exitcmd, 'EXIT', 0);
        if iswheel then
	    tbadd (cmdtable, forgetcmd, 'FORGET', 0);
	tbadd (cmdtable, genercmd, 'GENERATE', 0);
	tbadd (cmdtable, helpcmd, 'HELP', 0);
        if iswheel then
	    tbadd (cmdtable, initcmd, 'INITIALIZE', 0);
	tbadd (cmdtable, infocmd, 'INFORMATION', 0);
	tbAdd (CmdTable, listcmd, 'LIST',0);
	tbadd (cmdtable, exitcmd, 'QUIT', invisible);
        if iswheel then
	    tbadd (cmdtable, removecmd, 'REMOVE', 0);
	if iswheel then
	    tbadd (cmdtable, updatecmd, 'UPDATE', 0);
        if iswheel then
	    Tbaddchr (cmdtable, tbluk (cmdtable, 'ADD') + 1,
		  'A', abbreviation);
        if iswheel then
	    Tbaddchr (cmdtable, tbluk (cmdtable, 'CHANGE') + 1, 'C',
		  abbreviation);
	Tbaddchr (cmdtable, tbluk (cmdtable, 'INFORMATION') + 1, 'I',
		  abbreviation);
	END;    {buildCmd}


    PROCEDURE Buildudg;
	{Build table for dir/user group}

	BEGIN
	udgtable := tbmak (udgsize+1);
	tbadd (udgtable, dirgrp, 'DIRECTORY-GROUP',0);
	tbadd (udgtable, usergrp, 'USER-GROUP',0);
	END;    {buildUDG}


    PROCEDURE Buildyesno;
	{build table for YES and NO}

	BEGIN   {buildYesNo}
	yesnotable := tbmak (yesnosize+1);
	tbadd (yesnotable, yeskey, 'YES', 0);
	tbadd (yesnotable, nokey, 'NO', 0);
	END;    {buildYesNo}


    PROCEDURE Initvars;
	{initialize various global variables}

	VAR
	    index: integer;

	BEGIN   {initVars}
        initflag := false;
	credesflag := False;
	dirheap := NIL;         {Set up heaps}
	grpheap := NIL;
	udlheap := NIL;
	workheap := NIL;
	diruse := NIL;
	grpuse := NIL;
	workuse := NIL;
        dirend := NIL;
	grpend := NIL;
	New(GlobUGP);
	New(GlobDGP);
	InitUGrpAry(GlobDGP);
	InitUGrpAry(GlobUGP);
	brini (dirbrkmask, filb0, filb1, filb2, filb3);
	brini (usrbrkmask, usrb0, usrb1, usrb2, usrb3);
	END;    {initVars}


    PROCEDURE Initstrs;
	{initialize packed arrays of char}

	VAR
	    index:integer;

	BEGIN   {initStrs}
	Clearasciz (nullasciz, 1);
	FOR index:= 1 TO ascizlen DO
	    innullasciz[index] := Chr(0);
	newmsgfile := nullasciz;
	renmsgfile := nullasciz;
	defaultdir := innullasciz;
	NewDefDir := innullasciz;
	defaultgrp := innullasciz;
	NewDefGrp :=  innullasciz;
	defaultKind := innullasciz;
	NewDefKind :=  innullasciz;
	END;    {initStrs}


    PROCEDURE Getusername;
	{sets global variable UserName}

	VAR
	    usernumber: integer;
	    tmpusr: asciz;
	BEGIN   {getUserName}
	jsys (gjinf; ; usernumber);
	tmpusr := innullasciz;
	jsys (dirst,2; username, usernumber);
	END;    {getUserName}


    BEGIN   {initialize}
    Nums := ['0'..'9'];	    {Set up set of digits}
    Checkcapabilities;      {make sure we can use this program}
    Buildcmd;               {build table of main commands}
    Buildallsub;            {build table of INFO ALL subcommands}
    BuildNames;             {Build table of INFO NAMES subcommands}
    Buildinfo;              {build table of information subcommands}
    Buildgen;               {Build table for generate subcommands}
    Buildudg;               {build user/dir choice table}
    Buildyesno;             {build table for YES and NO}
    Initvars;               {set random small variables}
    Initstrs;               {set various arrays of char}
    Readfile;               {Read file into memory}
    Getusername;    {find out who the user is}
    {do any other necessary initialization}
    END;    {initialize}


    {Miscellaneous work routines}

PROCEDURE Unbracket (source: asciz; VAR dest: asciz);
    {Changes ST:<USERNAME> to USERNAME}

    VAR
	index: integer;
	i2: integer;

    BEGIN   {unBracket}
    dest := nullasciz;
    index := 1;
    i2 := 1;
    WHILE source[index] <> '<' DO
	index := index + 1;
    index := index + 1;
    WHILE source[index] <> '>' DO BEGIN
	dest[i2] := source[index];
	i2 := i2 + 1;
	index := index + 1;
	END; {while}
    END;    {unBracket}



FUNCTION Filesonly (direcname: asciz): boolean;
    {Return TRUE if directory is Files-only}

    CONST
	filonlyflag = 0;
    VAR
	tmpdir : asciz;
	flags : flagset;   {If flags is negative, bit 0 is set, hence
			   files only directory}
    BEGIN
    Intoout(direcname,tmpdir);
    jsys (rcdir, -1; 0, tmpdir; flags);
    Filesonly := (filonlyflag IN flags);
    END;



PROCEDURE GetDirNum (direcname: asciz; VAR DoesIt : Boolean;
		     VAR ResNum : Integer);
    {return TRUE in DoesIt if DirName refers to a real directory
                    ResNum is its number}

    CONST
	nomatchflag = 3;        {RC%NOM}
    VAR
	flags: flagset; {flags for RCDIR call}
	outform : asciz;
	Trash : integer;

    BEGIN   {GetDirNum}
    Intoout(direcname,outform);
    jsys (rcdir, -1; 0, outform; flags,Trash,ResNum);
    DoesIt := NOT(nomatchflag IN flags);
    END; {GetDirNum}



FUNCTION Sysdirnum(direc : asciz):integer;
    {Gets the system directory number of DIREC}

    VAR
	DirDoes : Boolean;
	DirNum : Integer;

    BEGIN
    GetDirNum(direc,DirDoes,DirNum);
    IF DirDoes THEN
	SysDirNum := DirNum
    ELSE
	SysDirNum := 0;
    END; {SysDirNum}


PROCEDURE Cleandir;
    {Cleans up the directory list by removing all directories with no
    groups associated with them}

    VAR
	prevtd,
	temptd,
	td:dirpnt;


    BEGIN
    td := diruse;
    prevtd := diruse;
    WHILE td <> NIL DO
	BEGIN
	IF ((td^.ugrps = NIL) AND (td^.dgrps = NIL)) THEN
	    BEGIN
	    IF td = diruse THEN
		BEGIN
		IF Diruse^.next = Nil THEN
		      Dirend := Nil;
		diruse := diruse^.next;
		Tossdir(td);
		td := diruse;
		prevtd := diruse;
		END
	    ELSE
		BEGIN
		prevtd^.next := td^.next;
		If td^.Next = nil THEN
		   dirend := prevtd;
		temptd := td^.next;
		Tossdir(td);
		td := temptd;
		END;
	    END
	ELSE
	    BEGIN
	    prevtd := td;
	    td := td^.next;
	    END;
	END; {while}
    END; {Cleandir}


PROCEDURE Readdir(VAR dirname: asciz;VAR dirnum:integer);
    {parse an existing directory name}

    VAR hld : integer;
	hlddir:asciz;

    BEGIN   {Readdir}
    IF defaultdir <> innullasciz THEN
	cmdef (defaultdir);
    hld := cmdir;
    Convtostring (hld, dirname);
    NewDefDir := dirname;
    dirnum := hld;
    END;    {Readdir}


PROCEDURE Confirm;
    {do cmCfm, cleaning up things like DefaultDir}

    BEGIN   {confirm}
    cmcfm;
    IF NewDefDir <> innullasciz THEN
	BEGIN
	defaultdir := NewDefDir;
	NewDefDir := innullasciz;
	defaultgrp := NewDefGrp;
	NewDefGrp := Innullasciz;
	DefaultKind := NewDefKind;
	NewDefKind := Innullasciz;
	END; {if}
    END;    {confirm}



FUNCTION Readyesno: boolean;
    {returns TRUE for a YES answer, FALSE for NO}

    BEGIN   {ReadYesNo}
    cmhlp ('YES or NO');
    Readyesno := (cmkey (yesnotable) = yeskey);
    Confirm;        {make user confirm with carriage return}
    END;    {ReadYesNo}



FUNCTION Readgrp: integer;
    {read a directory group, returned as an integer}

    VAR
	num: integer;

    BEGIN   {ReadGrp}
    IF defaultgrp <> innullasciz THEN
	cmdef (defaultgrp);
    cmhlp('Group number, an integer');
    num := cmnum;
    IF (num <= 0) THEN
	cmuerr ('Illegal group number');
    NumToString(Num , NewDefGrp);
    InToOut(NewDefGrp, NewDefGrp);
    Readgrp := num;
    END;    {ReadGrp}



FUNCTION ReadGroupKind : Integer;
    {Will return the keyword index for reading in USER-GROUP/DIRECTORY-GROUP}

    VAR
        Kind : Integer;				{Keyword table index}

    BEGIN
    IF (defaultKind <> innullasciz) THEN
	cmdef (defaultKind);
    Kind := cmkey(udgtable);
    IF Kind = DirGrp THEN
        NewDefKind := 'DIRECTORY-GROUP                                   '
    ELSE
	NewDefKind := 'USER-GROUP                                        ';
    InToOut(NewDefKind, NewDefKind);
    ReadGroupKind := Kind;
    END; {ReadGroupKind}

PROCEDURE Cutwork;
    {Will try to reduce the amount of work. The code is quite simple.
     Look at each item in the list, respectively. If in the list you
     find the converse of the item, delete both the item you were initially
     looking at and also that item. Then look at the next item in the list
     and do the same thing until you are at the end of the list (or it is
     empty)}

    {For ease of coding, two passes are made on the list. The first pass
     notes the duplicates, and sets the ADDREM field of duplicates to
     WORKNONE. The second pass goes through and deletes these entries}

    {One could argue that if the list were maintained and added to properly,
     this procedure would not be needed. Foo to you! 2 passes are better
     than N!}

VAR
    tw,
    tw2 : WorkPnt;
    hit : boolean;	    {True if a match found}

    BEGIN {Cutwork}

    {Pass one - Set redundancies to Addrem = WorkNone}
    tw := WorkUse;
    WHILE (TW <> Nil) DO
	BEGIN
	IF (TW^.AddRem <> WorkNone) THEN
	    BEGIN
	    tw2 := Tw^.Next;
	    hit := false;
	    WHILE (tw2 <> nil) AND (Not(Hit)) DO {Search for match}
		BEGIN
		IF  (TW^.Dirnum = TW2^.Dirnum) AND
		    (TW^.UGrp = TW2^.UGrp) AND
    		    (TW^.UseOrDir = TW2^.UseOrDir) THEN
    			BEGIN
			Hit := True;
			END; {If}
		IF Not(HIT) THEN
    		    BEGIN
		    TW2 := TW2^.Next;
		    END; {If}
		END; {While}
	    IF Hit THEN {Ah ha! A Match!}
	        BEGIN
		TW^.AddRem := WorkNone;
		TW2^.AddRem := WorkNone;
		END; {If a match}
	    END; {If TW^.Addrem <> WorkNone}
        TW := TW^.Next;
        END; {While TW <> NIL}

    {Pass 2 - Remove all entries of AddRem = WorkNone}
    TW := NIL;
    TW2 := WorkUse;
    WHILE (TW2 <> NIL) DO
	BEGIN
	IF (TW2^.AddRem = WorkNone) THEN     {remove it if no work}
	    BEGIN
	    IF TW = NIL	THEN		    {if so, we are at workuse}
		BEGIN
	    	WorkUse := WorkUse^.Next;
		END {If at WorkUse}
            ELSE
		BEGIN
	        TW^.Next := TW^.Next^.Next {if not, skip this entry}
		END; {If in middle}
	    END {If no work}
	ELSE {If work, move on}
	    TW := TW2;
	TW2 := TW2^.Next;
	END; {While Work in stack}
    END; {Cutwork}

PROCEDURE Dowork;
    {Takes the work list and asks the subroutine DCRDIR to perform the
    jsys}

    VAR
	tw: workpnt;
	TUGrp : Integer;
	CP : CrDirBlkPnt;
	UGAP,
	DGAP : UGrpAryPnt;

    BEGIN
    tw := workuse;
    New(CP);
    New(UGAP);
    New(DGAP);
    InitUgrpAry(DGAP);
    InitUgrpAry(UGAP);
    WHILE tw <> NIL DO
	BEGIN
	IF (Tw^.AddRem <> WorkNone) THEN
    	    BEGIN
	    WITH tw^ DO
		BEGIN	    
		UGAP^[1] := 0;
		DGAP^[1] := 0;
		TUGrp := ABS(Ugrp);
		IF (AddRem = WorkRem) THEN
		    TUGrp := -TUGrp;
		IF (UseOrDir = WorkUser) THEN
		    UGAP^[1] := TUGrp;
		IF (UseOrDir = WorkDir) THEN
		    DGAP^[1] := TUGrp;
		DoCrdir(dirnum, CP, UGAP, DGAP);
		END; {With}
	    END; {If}
	tw := tw^.next;
	END; {While}
    END; {DoWork}



FUNCTION Grpexists(gn : integer):boolean;
    {Returns true if group in data base}

    VAR
	g: grpnt;
	temp : boolean;
    BEGIN
    temp := false;
    g := grpuse;
    WHILE (g <> NIL) AND (NOT(temp)) DO BEGIN
	temp := (gn = g^.num);
	g := g^.next;
	END;
    Grpexists := temp;
    END;



PROCEDURE Findcredir(direc:asciz;VAR td:dirpnt);
    {Finds or creates an entry for The directory given}

    VAR temp:dirpnt;
	found:boolean;
    BEGIN
    found := false;
    temp:= diruse;
    WHILE ((NOT found) AND (temp <> NIL)) DO
	BEGIN
	IF temp^.dirname = direc THEN
	    found := true
	ELSE
	    temp := temp^.next;
	END;
    IF found THEN td := temp
    ELSE
	BEGIN
	Newdir(td);
	td^.dirname := direc;
	Adddir(td);
	END;
    END;



PROCEDURE Findgrpcre(VAR pnt:udlpnt;VAR tu:udlpnt);
    {Finds or creates a group for the group given}

    VAR
	temp :udlpnt;
	found :boolean;
    BEGIN
    found := false;
    temp := pnt;
    WHILE ((NOT found) AND (temp <> NIL)) DO
	BEGIN
	IF temp^.num = tu^.num THEN
	    found := true
	ELSE
	    BEGIN
	    temp := temp^.next;
	    END;
	END;
    IF NOT found THEN
	Addudl(pnt,tu);
    END;



FUNCTION Useringroup(tu: udlpnt;num:integer):boolean;
    {Returns TRUE if the list TU contains the group NUM}

    VAR temp : udlpnt;
	result : boolean;
    BEGIN
    result := false;
    temp := tu;
    WHILE (NOT(result) AND (temp <> NIL)) DO
	IF temp^.num = num THEN
	    result := true
	ELSE
	    temp := temp^.next;
    Useringroup := result;
    END;



PROCEDURE Adddugrp(direc:asciz;kind,grpnum:integer);
    {Adds a group to a directory. Direc=directory name; kind=1 for
    directory group, 2 for user group}

    VAR
	td:dirpnt;
	tu:udlpnt;
    BEGIN
    Newudl(tu);
    tu^.num := grpnum;
    Findcredir(direc,td);
    CASE kind OF
	dirgrp: BEGIN
	    IF Useringroup(td^.dgrps,grpnum) THEN
		cmuerr('User already in group')
	    ELSE
		Findgrpcre(td^.dgrps,tu);
	    END;
	usergrp:BEGIN
	    IF Useringroup(td^.ugrps,grpnum) THEN
		cmuerr('User already in group')
	    ELSE

		Findgrpcre(td^.ugrps,tu);
	    END;
    END;
    END;



PROCEDURE Finddir(top: dirpnt;dirname : asciz; VAR pnt : dirpnt);
    {Returns a pointer to the directory DIRNAME if it is in the
    list TOP. Returns NIL if it is not}

    VAR found : boolean; temp:dirpnt;

    BEGIN
    found := false;
    temp:= top;
    WHILE ((NOT found) AND (temp <> NIL)) DO
	BEGIN
	IF temp^.dirname = dirname THEN
	    found := true
	ELSE
	    temp := temp^.next;
	END;
    IF found THEN pnt := temp
    ELSE
	pnt := NIL;
    END;



PROCEDURE Findgroup (num : integer;VAR pnt : grpnt);
    {Returns a pointer to the directory NUM if it is in the universal
    group list. Returns nil if it is not}

    VAR hit : boolean;

    BEGIN
    pnt := grpuse;
    hit := false;
    WHILE ((pnt <> NIL) AND NOT(hit)) DO
	BEGIN
	IF pnt^.num = num THEN
	    hit := true
	ELSE
	    pnt := pnt^.next;
	END;
    IF NOT(hit) THEN
	pnt := NIL;
    END;



FUNCTION Removedgroupok(VAR tu:udlpnt;grp:integer):boolean;
    {Removes a group from a group list. Returns TRUE if it could do it}

    VAR old,
	cur:udlpnt;
	result : boolean;

    BEGIN
    old := tu;
    cur := tu;
    result := false;
    IF tu <> NIL THEN BEGIN
	IF tu^.num = grp THEN
	    BEGIN
	    tu := tu^.next;
	    result := true;
	    Tossudl(cur);
	    END
	ELSE
	    BEGIN
	    cur := tu^.next;
	    WHILE (NOT result) AND (cur <> NIL) DO
		IF cur^.num = grp THEN
		    BEGIN
		    old^.next := cur^.next;
		    result := true;
		    Tossudl(cur);
		    END
		ELSE
		    BEGIN
		    old := cur;
		    cur := cur^.next;
		    END;
	    END;
	END;
    Removedgroupok := result;
    END;


    PROCEDURE Printgroup(VAR f:text;tg: grpnt);

	VAR 
	    Topu: dirpnt;	    {Top of UGRoup list}
	    Topd: dirpnt;	    {Top of DGRoup list}
	    dp:dirpnt;
	    TempD : Dirpnt;
	    i : integer;
	    anyone : boolean;


	PROCEDURE AddEndDir(Var Top:Dirpnt; RecToAdd : DirPnt);
	    {Adds a copy of RecToAdd to the end of Top}

        VAR t,
    	    prev,
	    cur: dirpnt;

	    BEGIN
	    New(t);
	    WITH t^ DO                  {Set up work record}
		BEGIN
		next := NIL;
		Dirname := RecToAdd^.Dirname;
		END; {With}
	    IF Top = NIL THEN
		Top := t
	    ELSE
		BEGIN               {Add it to the end - code obvious}
		prev := Top;
		cur := Top^.next;
		WHILE cur <> NIL DO BEGIN
		    prev := cur;
		    cur := cur^.next;
		    END; {While}
		prev^.next := t;
		END; {else}
	    END; {AddEndDir}



	BEGIN
	Write(f,'Group #',tg^.num:6,'  Name:');
	Writeasciz(f,tg^.name);Writeln(f);
	anyone := false;
	TopU := Nil; TopD := Nil;
	dp := diruse;
	WHILE dp<> NIL DO
	    BEGIN
	    IF Useringroup(dp^.ugrps,tg^.num) THEN
		BEGIN
		AddEndDir(TopU,DP);
		AnyOne := True;
		END; {If}
	    Dp := Dp^.Next;
	    END;
	dp := diruse;
	WHILE dp<> NIL DO
	    BEGIN
	    IF Useringroup(dp^.dgrps,tg^.num) THEN
		BEGIN
		AddEndDir(TopD,DP);
		AnyOne := True;
		END; {If}
	    Dp := Dp^.Next;
	    END; {While}
	IF Not(anyone) THEN
	    Writeln(f,' [No directories in group]')
	ELSE
	    BEGIN
	    Writeln(f,' Directories in USER group:                 Directories in DIRECTORY group:');
	    IF DirEmpty(TopU) OR DirEmpty(TopD) THEN	{Special Kludges for empty}
		BEGIN				{-note that both are not empty}
		IF DirEmpty(TopD) THEN
		    BEGIN
		    Write(f,'  ');
		    For I := 1 to 42 DO 
			Write(F,TopU^.Dirname[i]);
		    Writeln(F,' [None]');
		    TempD := TopU^.Next;
		    TossDir(TopU);
		    TopU := TempD;
		    END {If}
		ELSE				{Empty TopU}
		    BEGIN
		    Write(f,'  [None]                                     ');
		    WriteAsciz(f,TopD^.Dirname);
		    Writeln(F);
		    TempD := TopD^.Next;
		    TossDir(TopD);
		    TopD := TempD;
		    END; {Else}
		END; {If}
	    WHILE (Not(DirEmpty(TopU)) or Not(DirEmpty(TopD))) DO
		BEGIN
		If DirEmpty(TopU) THEN
		    FOR I := 1 to 45 DO
		        Write(F,' ')
		ELSE
		    BEGIN
		    Write(F,'  ');
		    FOR I := 1 to 42 DO
			Write(F,TopU^.Dirname[i]);
		    TempD := TopU^.Next;
		    TossDir(TopU);
		    TopU := TempD;
		    Write(F,' ');
		    END; {Else}
		IF DirEmpty(TopD) THEN
		    Writeln(F)
		ELSE
		    BEGIN
		    WriteAsciz(F,TopD^.Dirname);
		    Writeln(F);
		    TempD := TopD^.Next;
		    TossDir(TopD);
		    TopD := TempD;
		    END; {Else}
		END; {While}
	    END; {Else}
	Writeln(F);
	END; {Procedure}


    PROCEDURE Printdirectory(VAR f:text; td: dirpnt);

	VAR tg: grpnt;
	    tu:udlpnt;
	    one: boolean;

	BEGIN
	Write(f,'Directory name: ');
	Writeasciz(f,td^.dirname);
	Writeln(f);
	one := false;
	tu:=td^.dgrps;
	WHILE tu <> NIL DO
	    BEGIN
	    IF NOT one THEN BEGIN
		Writeln(f,' DIRECTORY groups:'); one := true;END;
	    Write(f,'  ',tu^.num:6,' (');
	    Findgroup(tu^.num,tg);
	    IF tg = NIL THEN
		Write(f,'WARNING - THIS GROUP NOT IN DATABASE!')
	    ELSE
		Writeasciz(f,tg^.name);
	    Writeln(f,')');
	    tu := tu^.next;
	    END;
	one := false;
	tu:=td^.ugrps;
	WHILE tu <> NIL DO
	    BEGIN
	    IF NOT one THEN BEGIN
		Writeln(f,' USER groups:'); one := true;END;
	    Write(f,'  ',tu^.num:6,' (');
	    Findgroup(tu^.num,tg);
	    IF tg = NIL THEN
		Write(f,'WARNING - THIS GROUP NOT IN DATABASE!')
	    ELSE
		Writeasciz(f,tg^.name);
	    Writeln(f,')');
	    tu := tu^.next;
	    END;
	Writeln(f);
	END;


    PROCEDURE Printalldirectories(VAR f:text);
	VAR td : dirpnt;
	BEGIN
	IF diruse = NIL THEN
	    Writeln(tty,'[No directories in data base]')
	ELSE
	    BEGIN
	    Writeln(f);
	    td := diruse;
	    WHILE td <> NIL DO
		BEGIN
		Printdirectory(f,td);
		td := td^.next;
		END;
	    Writeln(f);
	    END;
	END;


    PROCEDURE Printallgroups(VAR f:text);
	VAR tg : grpnt;
	BEGIN
	IF grpuse = NIL THEN
	    Writeln(tty,'[No groups in data base]')
	ELSE
	    BEGIN
	    Writeln(f);
	    tg := grpuse;
	    WHILE tg <> NIL DO
		BEGIN
		Printgroup(f,tg);
		tg := tg^.next;
		END;
	    Writeln(f);
	    END;
	END;


    PROCEDURE SortGroupNames(VAR t: Grpnt);

	VAR
	    GP : Grpnt;
            Thing : Grpnt;
            prev,                           {Where to add it after}
	    cur : grpnt;                {Eventually, place after where it goes}
	    hit : boolean;

    BEGIN
    T := Nil;
    GP := Grpuse;
    While (GP <> Nil) DO
        BEGIN
        NewGrp(Thing);				
	Thing^.NUm := GP^.Num;
	Thing^.Name := GP^.Name;
        IF Grpempty(T) THEN            {If empty list, toss it on}
	    BEGIN
	    T := thing;
	    thing^.next := NIL;
	    END {If}
        ELSE                        {Not empty...}
	    BEGIN
	    IF SGREQ(t^.name,thing^.name) THEN  {Put it right in front?}
	        BEGIN
	        thing^.next := t;  {yes!}
	        t := thing;
	        END {if}
	    ELSE
	        BEGIN
	        prev := t;         {Setup}
	        cur := t^.next;
	        hit := false;
	        WHILE (cur <> NIL) AND (NOT hit) DO     {Find where to put it}
		    BEGIN
		    IF SGREQ(cur^.name,thing^.name) THEN
		        hit := true             {Put it HERE!}
		    ELSE
		        BEGIN
		        prev := cur;    {Advance pointers}
		        cur := cur^.next;
		        END; {Else}
		    END; {While}
	        thing^.next := cur;             {Insert THING}
	        prev^.next := thing;
	        END; {Else}
	    END; {Else}
       GP := GP^.Next; {Get the next thing}
       END; {While}
    END; {SortGroupNames}

    PROCEDURE Printgnames(VAR f: text; alpha : Boolean);

	VAR tg: grpnt;
	    gn: grpnt;
	BEGIN
	IF grpuse = NIL THEN
	    Writeln(tty,'[No groups in data base]')
	ELSE
	    BEGIN
	    IF Not(Alpha) THEN
		BEGIN
		tg := grpuse;
	        WHILE tg <> NIL DO
		    BEGIN
		    Write(f,'Group #',tg^.num:6,' ');
		    Writeasciz(f,tg^.name);
		    Writeln(f);
		    tg := tg^.next;
		    END; {While}
		END {IF}
	    ELSE
		BEGIN {Else Alphabetical order}
		Write(tty,'Sorting... hang on...');
		SortGroupNames(GN);
		writeln(tty,'done.');
		tg := gn;
	        WHILE tg <> NIL DO
		    BEGIN
		    Write(f,'Group #',tg^.num:6,' ');
		    Writeasciz(f,tg^.name);
		    Writeln(f);
		    tg := tg^.next;
		    END; {While}
	        WHILE GN <> NIL DO {Toss the links we created}
		    BEGIN
		    tg := gn^.next;
		    Tossgrp(gn);
		    gn := tg;
		    END;
		END; {Else alphabetical}
	    END; {If any groups}
	END; {Procedure}

    PROCEDURE PrintLowGroup(VAR F: Text);
	{Prints the number of the lowest unused group, ie: starts at one,
	 and works its way down the list until noticing an empty spot}

    VAR
	Count : Integer; { Counts to the lowest group}
	tg: grpnt;       { Pretty obvious, by now... points to current group}
	found : Boolean; { True if low group number found...}

	BEGIN
	Found := false;  { Low group not found }
	Count := 1;      { Lowest decent group number }
	IF grpuse = NIL THEN
	    BEGIN
	    Writeln(f,'No groups in data base - start off on the right foot');
	    Writeln(f,'with the number "1".');
	    END
	ELSE
	    BEGIN
	    tg := grpuse;
	    WHILE ((tg <> NIL) and (Not (Found))) DO
		BEGIN
		If (Count < tg^.num) THEN     {Yay! Low group found!}
		    Found := True
		ELSE
		    BEGIN			{Aww... try again}
		    Count := Count + 1;
		    TG := Tg^.Next;
		    END; {Else}
		END; {While}
	    Writeln(f,'The lowest unused Group number is ',Count:Width(Count));
	    END; {Else}
    END; {PrintLowGroup}



    PROCEDURE PrintChanges(VAR F: Text);
	{Print the changes, ie: work, to the file F}

    VAR
        WT : WorkPnt;

	PROCEDURE WriteWork(VAR F: Text; W:WorkPnt);
	    VAR
		DirStr : Asciz;
		tg : grpnt;

	    BEGIN
	    WITH W^ DO
		BEGIN
		IF (AddRem = WorkAdd) then Write(f,' Adding ')
				      else Write(f,' Removing ');
		ConvToString(Dirnum,DirStr);
		WriteAsciz(f,DirStr);
		IF (AddRem = WorkAdd) then Write(f,' to ')
				      else Write(f,' from ');
		If (UseOrDir = WorkUser) Then Write(f, 'user ')
				         else Write(f, 'directory ');
		Write(f,'group #', UGrp:Width(Ugrp),' (');
	        Findgroup(UGrp,tg);
		IF (TG <> NIL) THEN
		    Writeasciz(f,tg^.name)
		ELSE
		    write(f,'UNKNOWN NUMBER - HELP!');
    	        Writeln(f,').');
		END; {With}
           END; {WriteWork}
		
    BEGIN
    CutWork;
    IF (WorkUse = Nil) THEN
	Writeln(f,'[No net changes since last EXIT/UPDATE command]')
    ELSE
	BEGIN {Hmm... some changes made!}
	WT := WorkUse;
	Writeln(f);
	WHILE (WT <> Nil) DO
	    BEGIN
	    WriteWork(f,Wt);
	    WT := WT^.Next;
	    END; {While}
	Writeln(f);
	END; {Else}
    END; {PrintChanges}

    PROCEDURE PrintTimeStamp(VAR F: Text);
	{Print the date and time on the file F}

    VAR
	DateHld : DatAry;
	TimeHld : TimAry;

    BEGIN
    DateHld := Date;
    TimDay(TimeHld);
    Writeln(f,'Groupe listing for ',DateHld,' ',TimeHld);
    Writeln(f);
    END; {PrintTimeStamp}

PROCEDURE NameNewGroup(VAR G : Grpnt);
    {Name the group given us. from the number in G, name the group
	'Group G'}

    VAR
	I : Integer;
       TS : Asciz;

    BEGIN
    ClearAsciz(G^.Name,1);
    G^.Name[1] := 'G';
    G^.Name[2] := 'r';
    G^.Name[3] := 'o';
    G^.Name[4] := 'u';
    G^.Name[5] := 'p';
    G^.Name[6] := ' ';
    NumToString(G^.Num,TS);    
    I := 1;
    WHILE (Ts[I] <> ' ') DO	    {Put in X}
	BEGIN
	G^.Name[I+6] := Ts[I];
	I := I + 1;
	END; {While}
    END; {NameNewGroup}


    {Command Code}

    {Abort code}

PROCEDURE Helpabort;

    BEGIN {HelpAbort}
    writeln(tty);
    writeln(tty,'  Format: ABORT');
    writeln(tty);
    Writeln(tty,'    The ABORT command will leave Groupe, FORGETTING');
    Writeln(tty,'    about any changes made in the current session.');
    Writeln(tty,'    Use this command if you make a horrible mistake.');
    Writeln(tty,'    As you can lose a lot of work this way, you will');
    Writeln(tty,'    be asked if you really wish to use this command');
    Writeln(tty,'    before it is executed.');
    writeln(tty);
    END; {HelpAbort}


PROCEDURE Doabort;

    BEGIN {doAbbort}
    Cutwork;
    IF (iswheel) and (Not(Workempty(WorkUse)) or (Initflag) or CreDesFlag) THEN
        BEGIN
        cmnoi ('and IGNORE all changes');
	confirm;
	IF InitFlag THEN
	    BEGIN
	    writeln(tty,'WARNING: Aborting here will make Groupe forget about');
	    writeln(tty,'all the updates that were made with the INITIALIZE command!');
	    END; {If}
	cmini('Are you SURE you want to abort any changes you have made?');
	IF Readyesno THEN
	    BEGIN
	    writeln(tty,'Groupe aborted ... type CONTINUE to get back to where you were, with');
	    writeln(tty,'all your updates still intact.');
	    Jsys(Haltf,2);
	    END {If}
        ELSE
	    writeln(tty,'ABORTing aborted... (hee hee)');
        END {If}
    ELSE
	BEGIN
	cmnoi('from Groupe');
	confirm;
	writeln(tty,'Type CONTINUE to get back to Groupe.');
    	Jsys(Haltf,2);
	END;
    END; {doAbort}



    {Add Code}

PROCEDURE Helpadd;

    BEGIN {HelpAdd}
    writeln(tty);
    writeln(tty,'  Format: ADD (directory) ST:<NAME> (to) KIND (number) NUM');
    writeln(tty);
    Writeln(tty,'    The ADD command lets you add a directory to a user');
    Writeln(tty,'    or directory group. ST:<NAME> is the name of the');
    Writeln(tty,'    user or directory (don''t forget the angle brackets!)');
    writeln(tty,'    to add to the group. KIND is either USER-GROUP');
    writeln(tty,'    or DIRECTORY-GROUP, depending on whether you want that');
    writeln(tty,'    directory to access something or to be accessed.');
    writeln(tty,'    NUM is the number of the group you want to add');
    writeln(tty,'    the directory to.');
    writeln(tty);
    END; {HelpAdd}

PROCEDURE Doadd;
    VAR td : dirpnt;
	dirnum,
	kind : integer;
	grp:integer;
	indir,
	tmpdir : asciz;
	workgtype : WorkGroupType;

    BEGIN
    If not(iswheel) then
	writeln(tty,'WHEEL or OPERATOR capability required.')
    else begin
    cmnoi ('directory');
    Readdir(tmpdir,dirnum);
    Outtoin(tmpdir,indir);
    cmnoi ('TO');
    kind:= ReadGroupKind;
    cmnoi ('number');
    grp :=Readgrp;
    IF NOT(Grpexists(grp)) THEN
	cmuerr('Group does not exist');
    Confirm;
    Adddugrp(indir,kind,grp);
    IF (Kind = DirGrp) THEN WorkGType := WorkDir ELSE WorkGType := WorkUser;
    Addwork(dirnum,grp,workGtype,workadd);
    end;
    END;


    {Change Code}

PROCEDURE Helpchange;

    BEGIN {HelpChange}
    writeln(tty);
    writeln(tty,'  Format: CHANGE (the name of group number) NUM');
    writeln(tty);
    Writeln(tty,'    The CHANGE command lets you change the name of a group.');
    Writeln(tty,'    NUM is the number of the group to change. After being');
    writeln(tty,'    told the current name of the group, you will be asked');
    writeln(tty,'    for a new name on a separate line.');
    writeln(tty);
    END; {HelpChange}

PROCEDURE Dochange;

    VAR n : integer;
	tg : grpnt;
	tgt : grpnt;

    BEGIN
    Newgrp(tgt);
    ClearAsciz(tgt^.Name,1);
    if not(iswheel) then
        writeln(tty,'WHEEL or OPERATOR capability required.') else begin
    cmnoi ('the name of group number');
    n := Readgrp;
    IF NOT(Grpexists(n)) THEN
	cmuerr('Group does not exist - use the CREATE command to make a new one');
    Confirm;
    Findgroup(n,tg);
    IF tg = NIL THEN cmuerr('Group does not exist');
    tgt^.Num := N;
    CreDesFlag := True;
    NameNewGroup(Tgt);
    Write(tty,'Current name is:');
    Writeasciz(tty,tg^.name);Writeln(tty);
    writeln(tty,'Choose a new name for the group. Hit RETURN if you wish to');
    write(tty,'choose the default name "');
    writeasciz(tty,tgt^.Name);writeln(tty,'".');
    cmini ('New name of group: ');
    CMMult;
    Ignore := CMTxt(Tg^.Name);
    Confirm;
    CASE CMDO of 
	1: BEGIN
	    Clearasciz (tg^.name, CmInt + 1);
	    Confirm;
	    END;
	2: BEGIN TG^.Name := TGT^.Name; END;
	END;
    end;
    tossgrp(tgt);
    END;


    {Create Code}

PROCEDURE Helpcreate;

    BEGIN {HelpCreate}
    writeln(tty);
    writeln(tty,'  Format: CREATE (a NEW group numbered) GROUPNUMBER');
    writeln(tty);
    Writeln(tty,'    The CREATE command enters a new group into the database.');
    Writeln(tty,'    GROUPNUMBER is the number of the group to create.');
    Writeln(tty,'    You will be asked to give a name to the group on a separate.');
    writeln(tty,'    line.');
    writeln(tty);
    END; {HelpCreate}

PROCEDURE Docreate;
    VAR tg : grpnt;
	n : integer;

    BEGIN
    if not(iswheel) then
       writeln(tty,'WHEEL or OPERATOR capability required.') else begin
    cmnoi ('a NEW group numbered');
    n := Readgrp;
    IF Grpexists(n) THEN
	cmuerr('Group already exists - use the CHANGE command to change its name.');
    Confirm;
    Newgrp(tg);
    ClearAsciz(tg^.Name,1);
    tg^.num := n;
    NameNewGroup(tg);
    writeln(tty,'Now give a name to the new group.');
    writeln(tty,'If you wish, just hit RETURN to choose the');
    write(tty,'default new name: "');
    Writeasciz(tty,Tg^.name);
    writeln(tty,'".');
    cmini ('Name of group: ');
    CMMult;
    Ignore := CMTxt(Tg^.Name);
    Confirm;
    CASE CMDO of 
	1: BEGIN
	    Clearasciz (tg^.name, CmInt + 1);
	    Confirm;
	    END;
	2: BEGIN END;
	END;
    Addgrp(tg);
    credesflag := True;
    end;
    END;


    {Destroy Code}

PROCEDURE Helpdestroy;

    BEGIN {HelpDestroy}
    writeln(tty);
    writeln(tty,'  Format: DESTROY (group number) GROUPNUMBER');
    writeln(tty);
    Writeln(tty,'    The DESTROY commands deletes a group from the database');
    writeln(tty,'    GROUPNUMBER is, of course, the number of the group to destroy.');
    writeln(tty);
    Writeln(tty,'    Note that by doing this you are automatically removing EVERY');
    Writeln(tty,'    directory from that user/directory group. To remove ONE');
    Writeln(tty,'    directory from a group, use the DELETE command. As this');
    Writeln(tty,'    command is quite dangerous, Groupe will ask you if you');
    Writeln(tty,'    really want to use it before executing it.');
    writeln(tty);
    END; {HelpDestroy}


PROCEDURE Dodestroy;

    VAR
	hit : boolean;
	tgprev,
	tgtemp,
	tg: grpnt;
	td,
	tdtemp: dirpnt;
	grp: integer;
	dummy : boolean;
	dirnum : integer;
    BEGIN
    if not(iswheel) then writeln(tty,'WHEEL or OPERATOR capability required.')
    else begin
    cmnoi('group number');
    grp := Readgrp;
    IF NOT(Grpexists(grp)) THEN
	cmuerr('Group does not exist');
    Confirm;
    Writeln(tty, 'WARNING: Doing this command removes all people from this group.');
    cmini('Are you SURE you want to do this?');
    IF Readyesno THEN
	BEGIN
	IF grpuse^.num = grp THEN
	    BEGIN
	    tgtemp := grpuse;
	    IF Grpuse^.Next = nil THEN
	       Grpend := nil;
    	    grpuse := grpuse^.next;
	    Tossgrp(tgtemp);
	    END
	ELSE
	    BEGIN
	    tgprev := grpuse;
	    tgtemp := grpuse^.next;
	    hit := false;
	    WHILE (NOT hit) AND (tgtemp <> NIL) DO
		BEGIN
		IF tgtemp^.num = grp THEN
		    BEGIN
		    tgprev^.next := tgtemp^.next;
		    IF grpend = tgtemp THEN
		      grpend := tgprev;
		    Tossgrp(tgtemp);
		    hit := true
		    END
		ELSE
		    BEGIN
		    tgprev := tgtemp;
		    tgtemp := tgtemp^.next;
		    END;
		END;
	    END;
	td := diruse;
	WHILE td <> NIL DO
	    BEGIN
	    dirnum := 0;
	    dummy :=  Removedgroupok(td^.dgrps,grp);
	    IF Dummy THEN
		dirnum := Sysdirnum(td^.dirname);
	    IF dummy AND (dirnum <> 0) THEN
		Addwork(dirnum,grp,workdir,workrem);
	    dummy := Removedgroupok(td^.ugrps,grp);
	    IF Dummy and (dirnum = 0) THEN
		dirnum := Sysdirnum(td^.dirname);
	    IF dummy AND (dirnum <> 0) THEN
		Addwork(dirnum,grp,workuser,workrem);
	    td := td^.next;
	    END;
	Cleandir;
	END;
     credesflag := True;
     end;
    END;


    {Exit Code}

PROCEDURE Helpexit;

    BEGIN   {helpExit}
    writeln(tty);
    writeln(tty,'  Format: EXIT');
    writeln(tty);
    Writeln (tty,'    The EXIT command makes all the changes requested in the current');
    Writeln (tty,'    session with Groupe, writes out the data file, and returns');
    Writeln (tty,'    you to the EXEC.');
    writeln(tty);
    END;    {helpExit}

PROCEDURE Doexit;

    BEGIN   {doExit}
    Cutwork;
    IF (iswheel) and (Not(Workempty(WorkUse)) or InitFlag or CredesFlag) THEN
	BEGIN
    	cmnoi ('from Groupe and update database');
	confirm;
	IF InitFlag THEN
	    BEGIN
	    writeln(tty,'REMEMBER: Exiting now will make Groupe remember about the');
	    writeln(tty,'updates that were made with the INITIALIZE command (this is good).');
	    END; {If}
    	cmini('Are you sure you wish to EXIT?');
	IF Readyesno THEN
	    BEGIN
	    Dowork;
	    Write(tty,'Writing data file...');
	    close(ingrp);
	    Rewrite(outgrp,'SYSTEM:GROUPE.BIN');
	    Writefile(outgrp);
	    Writeln(tty,'done.');
	    TossAllWork(WorkUse);
	    INitFlag := False;
	    CreDesFlag := False;
	    Writeln(tty,'To continue Groupe, type CONTINUE.');
	    Jsys(HALTF,2);
    	    END; {If}
       END {If Changes}
    ELSE {No Changes}
	BEGIN
        cmnoi ('from Groupe');
        Confirm;
	If ((IsWheel) and WorkEmpty(WorkUse)) THEN
	    writeln(tty,'No net changes made since last EXIT/UPDATE command.');
        TossAllWork(WorkUse);
	Writeln(tty,'To continue Groupe, type CONTINUE.');
	Jsys(HALTF,2);
	END; {No Changes}
    END;    {doExit}

    {Forget code}

PROCEDURE HelpForget;

    BEGIN   {helpForget}
    writeln(tty);
    writeln(tty,'  Format: FORGET');
    writeln(tty);
    Writeln (tty,'    The FORGET command will cause Groupe to forget about all of the');
    Writeln (tty,'    changes you have made since you last gave the EXIT or UPDATE');
    Writeln (tty,'    commands. This command is used when you want to start things'); 
    Writeln (tty,'    up fresh.');
    Writeln (tty);
    Writeln (tty,'    Note: Groupe will not forget about any changes made BEFORE');
    Writeln (tty,'    and EXIT or UPDATE command because those changes were made');
    Writeln (tty,'    PERMANENT by either of those two commands.');
    writeln(tty);
    END;    {helpForget}

PROCEDURE DoForget;
    {FORGET will cause Groupe to forget about all of the changes you have
     requested. It is identical to ABORT, without the exiting of the program.}

    BEGIN
    Cutwork;
    IF (iswheel) and (Not(Workempty(WorkUse)) or (Initflag) or CreDesFlag) THEN
        BEGIN
        cmnoi ('ALL changes made since last EXIT or UPDATE command');
	confirm;
	IF InitFlag THEN
	    BEGIN
	    writeln(tty,'WARNING: Forgetting here will make Groupe forget about');
	    writeln(tty,'all the updates that were made with the INITIALIZE command!');
	    END; {If}
	cmini('Are you SURE you want Groupe to forget any changes you have made?');
	IF Readyesno THEN
	    BEGIN
	    TossAllWork(WorkUse);
	    INitFlag := False;
	    CreDesFlag := False;
	    writeln(tty,'Groupe has forgotten all of your changes.');
	    END {If}
        ELSE
	    writeln(tty,'FORGET command forgotten... (hee hee)');
        END {If}
    ELSE
	BEGIN {No changes made}
	writeln(tty,'There is nothing for Groupe to forget.'); 
	END;
    END; {DoForget}


    {Generate Code}

PROCEDURE Helpgener;
    BEGIN   {helpGener}
    writeln(tty);
    writeln(tty,'  Format: GENERATE (a mailing list of) TYPE (number) NUM (to file) FILE');
    writeln(tty);
    Writeln (tty,'    The GENERATE command makes a mailing list of the directories in');
    Writeln(tty,'     a specific user or directory group.');
    Writeln(tty);
    Writeln(tty,'     NUM is the number of a group,');
    Writeln(tty,'     FILE is a filename to put the maillist on,');
    Writeln(tty,'          (It deafults to GROUP-X.LIST, where X is the group number).');
    Writeln(tty,'     and TYPE is one of the following:');
    Writeln(tty,'          COMBINED-DIRECTORY-AND-USER-GROUP');
    Writeln(tty,'                (Mails to people both in that user');
    Writeln(tty,'                 AND directory group)');
    Writeln(tty,'          DIRECTORY-GROUP');
    Writeln(tty,'                (Mails to people in the directory group)');
    Writeln(tty,'          USER-GROUP');
    Writeln(tty,'                (Mails to peole in that user group)');
    Writeln(tty);
    Writeln(tty,'    To use such a list in MM, use the command:');
    Writeln(tty,'       SEND @FILENAME or use the EXEC Mail command:');
    Writeln(tty,'       MAIL @FILENAME');
    writeln(tty);
    END;    {helpGener}


PROCEDURE Dogener;

    VAR
	tg : grpnt;             {Points to group to print}
	gnum : integer; {number of group}
	gentype : integer;      {type of generate}
	onedone: boolean;       {whether a record was printed}
	namesin : dirpnt;       {top of names used list}
	td : dirpnt;    {Where we are checking}
	tempd : dirpnt; {temporary pointer}
	hold : asciz;   {Holds unbracketed usrname}
	filnam : asciz;	{Default file to output}

    PROCEDURE CreGenFilnam(Gnum : integer;VAR outfil:asciz);
	{Creates a default filename of form: GROUP-X.LIST
	 Note that result has trailing NULLS (external format)}

    VAR Nums: asciz;
	I : integer;

    BEGIN
    OutFil := InNullAsciz;
    NumToString(Gnum,NumS);
    OutFil[1] := 'G';    OutFil[2] := 'R';    OutFil[3] := 'O';
    OutFil[4] := 'U';    OutFil[5] := 'P';    OutFil[6] := '-';
    I := 1;
    WHILE (Nums[I] <> ' ') DO	    {Put in X}
	BEGIN
	OutFil[I+6] := Nums[I];
	I := I + 1;
	END;
    OutFil[I+6] := '.';    OutFil[I+7] := 'L';    OutFil[I+8] := 'I';
    OutFil[I+9] := 'S';    OutFil[I+10] := 'T';   OutFil[I+11] := '.';
    OutFil[I+12] := '0';
    END;

    FUNCTION Psstruc(direc : asciz):boolean;
	{Returns true if directory is a PS directory}

	BEGIN
	Psstruc := true;
	IF direc[1] <> 'P' THEN Psstruc := false;
	IF direc[2] <> 'S' THEN Psstruc := false;
	IF direc[3] <> ':' THEN Psstruc := false;
	END;

    PROCEDURE Genrecord;
	{argument to mapOverDatabase - generate a single record}
	{uses var WhatToDo from doGener to tell whether to print rec}
	BEGIN
	IF Psstruc(td^.dirname) THEN BEGIN
	    Unbracket(td^.dirname,hold);
	    Finddir(namesin,hold,tempd);
	    IF tempd = NIL THEN
		BEGIN
	        IF NOT(onedone) THEN
	    	    BEGIN
	    	    Write(f,'"People in ');
	    	    CASE gentype OF
		        gencombine: Write(f,'User and Directory Group ');
		        gendg : Write(f,'Directory Group ');
		        genug : Write(f,'User Group ');
	            END;
	            Write(f,gnum:Width(gnum),' (');
	            Writeasciz(f,tg^.name);
	            Writeln(f,')":-');
	            END {If}
	        ELSE
		    Writeln(f, ',-');
		Newdir(tempd);
		tempd^.next := namesin;
		tempd^.dirname := hold;
		namesin := tempd;
		Writeasciz(f,hold);
		onedone := true;
		END;
	    END;
	{If PSStruc}
	END;    {genRecord}

    BEGIN   {doGener}
    onedone := false;
    cmnoi ('mailing list of');
    gentype := cmkey(gentable);
    cmnoi ('number');
    gnum := Readgrp;
    IF NOT(Grpexists(gnum)) THEN
	cmuerr('Group does not exist');
    Findgroup(gnum,tg);
    cmnoi ('to file');
    cregenfilnam(gnum,filnam);
    CMHlp('Output file name for the mailing list, hit <ESC> to see the default');
    cmdef (filnam);
    cmofi (f);
    Confirm;
    onedone := false;
    Rewrite (f);
    td := diruse;
    namesin := NIL;
    WHILE td <> NIL DO
	BEGIN
	    CASE gentype OF
		gencombine : IF (Useringroup(td^.ugrps,gnum)OR
				 (Useringroup(td^.dgrps,gnum))) THEN
			 	IF NOT Filesonly(td^.dirname) THEN
				 Genrecord;
		gendg:  IF  (Useringroup(td^.dgrps,gnum)) THEN
				IF NOT Filesonly(td^.dirname) THEN
					 Genrecord;
		genug:  IF  (Useringroup(td^.ugrps,gnum)) THEN
				IF NOT Filesonly(td^.dirname) THEN
					 Genrecord;
		    END;
	td := td^.next;
	END;
    IF NOT onedone THEN
	Writeln (tty, '[No directories in that group -file is empty]')
    ELSE
	Writeln(f);
    rclose (f);
    td := namesin;
    WHILE td <> NIL DO
	BEGIN
	namesin := td;
	td := td^.next;
	Tossdir(namesin);
	END;
    END;    {doGener}


    {Information Code}

PROCEDURE Helpinfo;

    BEGIN {HelpInfo}
    writeln(tty);
    writeln(tty,'  Format: INFORMATION (about) INFOTYPE (to file) FILE');
    writeln(tty);
    Writeln(tty,'    The INFO command prints information from the database in');
    Writeln(tty,'    many ways. The format of the command depends on the option, so');
    Writeln(tty,'    each format will be given separately.');
    Writeln(tty);
    Writeln(tty,'    INFORMATION DIRECTORY ST:<USERNAME>');
    Writeln(tty,'      -prints the user/directory groups of the given directory');
    Writeln(tty,'    INFORMATION GROUP GROUPNUMBER');
    Writeln(tty,'      -prints all of the users in that user/directory group');
    Writeln(tty,'    INFORMATION ALL GROUPS');
    Writeln(tty,'      -prints all of the information, sorted by groups');
    Writeln(tty,'    INFORMATION ALL DIRECTORIES');
    Writeln(tty,'      -prints all of the information, sorted by directory name');
    writeln(tty);
    writeln(tty,'    INFORMATION CHANGES');
    writeln(tty,'      -prints a list of all changes made in the current session');
    writeln(tty,'       of Groupe. Most useful after the INITIALIZE command to');
    writeln(tty,'       see what changes were made to the user group structure');
    writeln(tty,'       outside of the use of this program.');
    writeln(tty);
    Writeln(tty,'    INFORMATION LOWEST-GROUP-NUMBER');
    Writeln(tty,'      -prints the number of the lowest unused group. That is,');
    Writeln(tty,'       it gives you the number you should use in creating your');
    Writeln(tty,'       next group. If your system gives some meaning to the numbers,');
    Writeln(tty,'       (for instance, reserving numbers 1000 through 1999 for a');
    Writeln(tty,'       certain special interest group), then this command will not');
    Writeln(tty,'       be very useful for you.');

    writeln(tty);
    Writeln(tty,'    INFORMATION NAMES-OF-GROUPS ALPHABETICALLY');
    writeln(tty,'    INFORMATION NAMES-OF-GROUPS NUMERICALLY');
    Writeln(tty,'      -prints only the names of the groups, one per line');
    writeln(tty,'       the second parameter specifies the type of sorting to use.');
    Writeln(tty);
    Writeln(tty,'    NOTE: ALL of the INFORMATION commands can be followed by a FILENAME');
    Writeln(tty,'    indicating where to send its output. For example:');
    Writeln(tty,'        INFORMATION ALL GROUPS GRP.LIST');
    Writeln(tty,'    will send the complete group list to the file GRP.LIST');
    writeln(tty,'    If no filename is specified, output will go to the terminal.');
    writeln(tty);
    END; {HelpInfo}


PROCEDURE Doinfo(PrintLPT:boolean);
    {Print information about the data-base. If PrintLPT is true, print on
     lineprinter and do not ask for a file name}

    VAR tempdir,indir:asciz;
	dirnum:integer;
	td:dirpnt;
	tg:grpnt;
	grp : integer;

	    PROCEDURE GetInfoFileandStamp;
		{Get the INFO/LIST filename and stamp it if need be}

		BEGIN
		If Not(PrintLpt) THEN BEGIN
    		    cmnoi('to file');
		    cmdef('TTY:');
		    cmhlp('Output file name for the information, default is to the terminal');
		    cmofi(f);
		    Confirm;
		    Rewrite(f);
		    END
		ELSE
		    BEGIN
		    Confirm;
		    ReWrite(f,'LPT:');
		    END;
		IF PrintLpt THEN
		    PrintTimeStamp(f);
		END; {GetInfoFileandStamp}

    BEGIN {Doinfo}
    If not(PrintLpt) then
	cmnoi('about')
    ELSE
	cmnoi('information (on the LINEPRINTER) about');
    CASE cmkey(infotable) OF
	allkey: CASE cmkey(allsubtable) OF
	    allbothkey : BEGIN
		GetinfoFileandStamp;
		Printalldirectories(f);
		Printallgroups(f);
		rclose(f);END;
	    alldirectorykey: BEGIN
		GetinfoFileandStamp;
		Printalldirectories(f);
		rclose(f);END;
	    allgroupkey:BEGIN 
		GetinfoFileandStamp;
		Printallgroups(f);rclose(f);END;
	    END;

	changeskey: BEGIN
		GetinfoFileandStamp;
		PrintChanges(f);
		rclose(f);
		END;
	directorykey: BEGIN
	    cmnoi('name');
	    Readdir(tempdir,dirnum);
	    Outtoin(tempdir,indir);
	    Finddir(diruse,indir,td);
	    IF td = NIL THEN
		cmuerr('Name not in database')
	    ELSE
		BEGIN
		GetinfoFileandStamp;
		Printdirectory(f,td);rclose(f);
		END;
	    END;
	groupkey:    BEGIN
	    cmnoi('number');
	    grp := Readgrp;
	    Findgroup(grp,tg);
	    IF tg = NIL THEN
		cmuerr('Group not found')
	    ELSE
		BEGIN
		GetinfoFileandStamp;
		Printgroup(tty,tg);rclose(f);END;
	    END;
	LowGroupKey: BEGIN
	    GetinfoFileandStamp;
	    PrintLowGroup(f);
	    RClose(f);
	    End; {LowGroupKey}
	NamesKey: BEGIN
	    cmnoi('and sort');
	    CMDef('NUMERICALLY');
	    CASE cmkey(namessubtable) OF
	    alphasortkey: BEGIN
		GetinfoFileandStamp;
		printgnames(f,true);
		rclose(f);END;
	    numbersortkey:BEGIN 
	    	GetinfoFileandStamp;
	    	Printgnames(f,false);
	    	rclose(f);
	    	END;
	    END; {Case}
	    END; {NamesKey}
    END; {Case}
    END; {DOINFO}


    {Initialize Code}

PROCEDURE Helpinit;

    BEGIN {HelpInit}
    writeln(tty);
    writeln(tty,'  Format: INITIALIZE CHANGES.TXT');
    writeln(tty,'       or INITIALIZE');
    writeln(tty);
    Writeln(tty,'    The INITIALIZE command resets the ENTIRE data base, by');
    Writeln(tty,'    doing an exhaustive search of the system''s directories');
    Writeln(tty,'    for evey directory and user group. You should only use this');
    Writeln(tty,'    command if you KNOW what you are doing. As it is SO dangerous,');
    Writeln(tty,'    Groupe will ask you if you really want to use it before');
    Writeln(tty,'    executing it.');
    writeln(tty);
    writeln(tty,'    CHANGES.TXT is the file to which changes from the old');
    writeln(tty,'    database to the new should be reported.');
    writeln(tty);
    writeln(tty,'    If you do not give a filename to the INITIALIZE command,');
    writeln(tty,'    no comparison file will be generated.');
    writeln(tty);
    END; {HelpInit}

PROCEDURE Doinit;


    CONST
	MaxDirNum = 12777B;		{Assumed top dir num for a disk}
	MaxDevs = 10;			{Max. number of devices}
	
    TYPE
	DevAry = ARRAY[1..MaxDevs] of Asciz;

    VAR
	ChangeFlag : boolean;           {True if they want changes reported}
	ChangeFile : Text;		{file in which to report changes}
	Devs : DevAry;			{Devices in system}
	CurDev : Integer;		{Current Device offset we are on}
	DevInt : Integer;		{Current internal Device number we are on}
	DevHld : Flagset;		{Kludge to get the halfword}
	CurDirNum : Integer;		{Current directory we are on}
	OldDirUse : DirPnt;		{Old list of Directories}
	DGP,
	UGP : UGrpAryPnt;		{Pointers to above}
	CP : CrDirBlkPnt;		{Pointer to above}
	i : integer;
	temp : integer;
	td : dirpnt;            {Pointers}
	tu : udlpnt;
	tg : grpnt;
	as1,
	as2,
	as3: asciz;                     {More temps}
	retval : Integer;		{return values for jsyi}
	writemess : Boolean;		{Write status messages?}


	PROCEDURE ReportChanges(VAR F: Text; ODiruse, NDiruse : DirPnt);
	    {Report the differences in the old and the new directories
	     pointers. The algorightm works, assuming the sorted list,
             as follows:
		Point both pointers to the top. Call one pointer A, the
		other pointer B. It doesn't matter which one is which.
		IF DirName(A) = DirName(B) go check-groups
		   checkgroups: If GroupNum(A) = GroupNum(B), advance both
		    pointers.
				If GN(A) < GN(B), print A until GN(A) > GN(B).
				Then do again until both lists nil.
		Now if DN(A) = DN(B), print DN(A) until DN(A) > DN(B), then do
		B. }

	VAR
	    OD,
	    ND,
	    TOD,
            TND : DirPnt;		{Temp pointers to old/new}
	    TNG, TOG : UdlPnt;		{pointers to groups}
	    DoOld : Boolean;		{Which list to do - If True, do old}
	    CurDir : Asciz;		{Current Directory name}

	    FUNCTION GrpGt(A, B : UdlpNt) : Boolean;
	    VAR T : Boolean; 
		    BEGIN T := False; IF ((A <> Nil) AND (B <> NIL)) THEN
		    IF A^.Num > B^.Num THEN T := True; GrpGt := T; 
		    IF ((A <> Nil) AND (B = Nil)) THEN GrpGt := True;
		    END;

	    FUNCTION GrpEQ(A, B : udlpNt) : Boolean;
	    VAR T : Boolean; 
		    BEGIN T := False; IF ((A <> Nil) AND (B <> NIL)) THEN
		    IF A^.Num = B^.Num THEN T := True; GrpEq := T;
		    END;

	    FUNCTION GrpLT(A, B : UdlpNt) : Boolean;
	    VAR T : Boolean; 
		    BEGIN T := False; IF ((A <> Nil) AND (B <> NIL)) THEN
		    IF A^.Num < B^.Num THEN T := True; GrpLt := T; 
		    IF ((A <> nil) and (B = Nil)) THEN GrpLt := True;
		    END;

	    PROCEDURE DoGroups(W : WorkGroupType);

		BEGIN
		IF DoOld THEN
		    BEGIN
		    WHILE GrpLT(TOG, TNG) DO
			BEGIN
			writeasciz(f,CurDir);
			write(f,' removed from ');
			IF W = WorkUser then write(f,'user') else
			    write(f,'directory');
			Writeln(f,' group #', TOG^.Num:Width(TOG^.Num),'.');
			TOG := TOG^.Next;
			END; {While TOG < TNG}
		    WHILE GrpEQ(TOG, TNG) DO
			BEGIN
			TOG := TOG^.Next; TNG := TNG^.Next;
			END;
		    END {If DoOld}
		ELSE
		    BEGIN
		    WHILE GrpLT(TNG, TOG) DO
			BEGIN
			writeasciz(f,CurDir);
			write(f,' added to ');
			IF W = WorkUser then write(f,'user') else
			    write(f,'directory');
			Writeln(f,' group #', TNG^.Num:Width(TNG^.Num),'.');
			TNG := TNG^.Next;
			END; {While TNG < TOG}
		    WHILE GrpEQ(TOG, TNG) DO
			BEGIN
			TOG := TOG^.Next; TNG := TNG^.Next;
			END;
		    END {If not DoOld}
		END; {Do Groups}

	    FUNCTION DirGT(A, B : DirPNt) : Boolean;
	    VAR T : Boolean; 
		    BEGIN T := False; IF ((A <> Nil) AND (B <> NIL)) THEN
		    IF SGT(A^.Dirname,B^.Dirname) THEN T := True; DirGt := T; 
		    IF ((A <> Nil) AND (B = Nil)) THEN DirGt := True;END;

	    FUNCTION DirLT(A, B : DirPNt) : Boolean;
	    VAR T : Boolean; 
		    BEGIN T := False; IF ((A <> Nil) AND (B <> NIL)) THEN
		    IF SLT(A^.Dirname,B^.Dirname) THEN T := True; DirLt := T; 
		    IF ((A <> Nil) AND (B = Nil)) THEN DirLt := True;END;

	    FUNCTION DirEQ(A, B : DirPnt) : Boolean;
	    VAR T : Boolean; 
		    BEGIN T := False; IF ((A <> Nil) AND (B <> NIL)) THEN
		    IF SEQ(A^.Dirname,B^.Dirname) THEN T := True; Direq := T;
		    END;

	BEGIN
	PrintTimeStamp(f);
	writeln(f);
	TND := NDirUse;
	TOD := ODirUse;
	WHILE ((TND <> Nil) OR (TOD <> Nil)) DO
	    BEGIN
	    DoOld := true;
	    While DirLT(TOD, TND) DO
		BEGIN TOG := TOD^.Dgrps; TNG := nil; CurDir := TOD^.DirName;
			    DoGroups(WorkDir);
		      TOG := TOD^.Ugrps; TNG := nil; DoGroups(WorkUser);
		      TOD := TOD^.Next; END;
	    While DirEQ(TOD, TND) DO
		BEGIN TOG := TOD^.Dgrps; TNG := TND^.DGrps; CurDir := TOD^.Dirname;
		    DoGroups(WorkDir);
		      TOG := TOD^.Ugrps; TNG := TND^.UGrps; DoGroups(WorkUser);
		      TOD := TOD^.Next; TND := TND^.Next; END;
	    DoOld := False;
	    While DirLT(TND, TOD) DO
		BEGIN TNG := TND^.Dgrps; TOG := nil; CurDir := TND^.DirName;
			DoGroups(WorkDir);
		      TNG := TND^.Ugrps; TOG := nil; DoGroups(WorkUser);
		      TND := TND^.Next; END;
	    While DirEQ(TOD, TND) DO
		BEGIN TOG := TOD^.Dgrps; TNG := TND^.DGrps; CurDir := TND^.Dirname; 
			DoGroups(WorkDir);
		      TOG := TOD^.Ugrps; TNG := TND^.UGrps; DoGroups(WorkUser);
		      TOD := TOD^.Next; TND := TND^.Next; END;
	    END; {BIG While}

	writeln(f);
	END; {ReportChanges}

	PROCEDURE InitDevs;
	    {Initialize device array}
	Var I : Integer;
	BEGIN
	FOR I := 1 to MaxDevs DO
	    Devs[I] := Innullasciz;
	{BEGINNING OF SYSTEM SPECIFIC CODE}
	  Devs[1] := 'PS:                                               ';
	  Devs[2] := 'MRC:                                              ';
	  For I := 1 to 2 DO
	    IntoOut(Devs[I],Devs[I]);
	{END OF SYSTEM SPECIFIC CODE}
	END; {InitDevs}


    BEGIN {DoInit}
    if not(iswheel) then writeln(tty,'WHEEL or OPERATOR capability required.')
    else begin
    cmnoi('the Groupe database');
    CMMult;
    CMDef('GROUPE.CHANGES.0');
    CMofi(ChangeFile);			{Alternative 1: Output filename}
    Confirm;				{Alternative 2: Carriage return}
    Case CMDo OF
	1: BEGIN
	   ChangeFlag := True;
	   Confirm;
	   END; {If changes desired}
	2: ChangeFlag := False;
	END; {Case}
    Writeln(tty,'WARNING: This command COMPLETELY changes the data base! The');
    Writeln(tty,'only thing left intact is the current list of possible user');
    Writeln(tty,'groups. Please note if you REALLY wish to use this command!');
    cmini('Do you wish to go through with the INITIALIZE command?');
    IF Readyesno THEN
	BEGIN
	cmini('Do you want to see the directory and group names as they are added?');
	IF ReadYesNo THEN
	    WriteMess := True
	ELSE
	    WriteMess := False;
	InitFlag := True;
	IF WriteMess THEN BEGIN
	  Writeln(tty,'This command takes a few minutes. The directories you see are');
	  Writeln(tty,'exactly those directories that have user/directory groups');
	  Writeln(tty,'associated with them.');
	  END
	ELSE
	  Writeln(tty,'This command takes a few minutes...');
	OldDiruse := Diruse;		{save old and reset new list}
	DirUse := Nil;
	New(DGP);			{get things we need...}
	New(UGP);
	New(CP);
	InitDevs;
	CurDev := 0;			{start with first device}
	CurDirNum := 0;		{...and max. dir number}
	InitUGrpAry(UGP);		{Initialize user/dir group list}
	InitUGrpAry(DGP);
	InitCrDirBlk(CP);
	{THE NUMBER 2 BELOW IS SYSTEM SPECIFIC, AND REFERS TO THE NUMBER
         OF DEVICES ON THE SYTEM}
	WHILE (CurDev < 2) DO
	    BEGIN
	    CurDev := CurDev + 1;
	    CurDirNum := MaxDirNum;
	    Jsys(STDev,-2,Retval;Devs[CurDev];Ignore,DevHld);
	    IF (Retval <> 2) THEN
	        CurDirNum := 0		{if no such device, don't check it}
	    ELSE
		BEGIN
		Devint := RightHalf(DevHld);
		Write(tty,'Working on device ');
		writeAsciz(tty,Devs[CurDev]); writeln(tty);
		END;
    	    WHILE (CurDirNum > 0) DO
		BEGIN
		DoGTDir(CurDirNum,CP,UGP,DGP,Devint);
		IF (UGP^[2] <> 0) OR (DGP^[2] <> 0) THEN
		    BEGIN
		    Convdevdirtostring (DevInt,CurDirNum, as1);
		    Outtoin(as1,as2);
		    IF WriteMess THEN BEGIN
				      Write(tty,'Working on:');
				      Writeasciz(tty,as2);
				      Writeln(tty);
				      END;
		    Newdir(td);
		    td^.dirname := as2;
		    Adddir(td);
		    I := 2;
		    WHILE (DGP^[I] <> 0) DO
			BEGIN
			IF NOT(Grpexists(Dgp^[i])) THEN
			    BEGIN
			    Newgrp(tg);
			    tg^.num := dgp^[i];
			    NameNewGroup (tg);
			    Addgrp(tg);
			    END; {If}
			IF WriteMess THEN
			  Writeln(tty,'Putting them in dir. group ',dgp^[i]);
			Adddugrp(as2,dirgrp,dgp^[i]);
			I := I + 1;
			END; {While}
		    I := 2;
		    WHILE (UGP^[I] <> 0) DO
			BEGIN
			IF NOT(Grpexists(ugp^[i])) THEN
			    BEGIN
			    Newgrp(tg);
			    tg^.num := ugp^[i];
			    NameNewGroup(tg);
			    Addgrp(tg);
		    	    END;
			IF WriteMess THEN
			  Writeln(tty,'Putting them in user group ',ugp^[i]);
			Adddugrp(as2,usergrp,ugp^[i]);
			I := I + 1;
			END; {While}
		    InitUGrpAry(UGP);	{Initialize user/dir group list}
		    InitUGrpAry(DGP);
		    InitCrDirBlk(CP);
		    END; {If User or Directory groups}
		CurDirNum := CurDirNum - 1;
		UGP^[1] := UGrpAryLen;
		DGP^[1] := UGrpAryLen;
		END; {While CurDirNum > 0}
    	    END; {WHILE CurDev < Number of Devs in system}
	Writeln(tty);
	Writeln(tty,'All done! You may wish to give the INFO ALL BOTH FILE.EXT');
	Writeln(tty,'command, and on the basis of the results, choose names for');
	Writeln(tty,'each of the user/directory groups that do not have names.');
	Writeln(tty);
    	Writeln(tty,'(P.S. Don''t forget to use the EXIT command to save all of');
	Writeln(tty,'these changes!)');

	IF ChangeFlag THEN
	    BEGIN
	    writeln(tty);
	    write(tty,'Now writing the differences file...');
	    ReWrite(changefile);
	    ReportChanges(changefile, OldDirUse, DirUse);
	    RClose(changefile);
	    writeln(tty,'done.');
	    END; {If we should do reports}
    	WHILE (OldDirUse <> NIL) DO		{Flush the old directory list}
	    BEGIN
	    WHILE OldDirUse^.dgrps <> NIL DO
		BEGIN
		tu := OldDirUse^.dgrps^.next;
		Tossudl(OldDirUse^.dgrps);
		OldDirUse^.dgrps := tu;
		END;
	    WHILE OldDirUse^.ugrps <> NIL DO
		BEGIN
		tu := OldDirUse^.ugrps^.next;
		Tossudl(OldDirUse^.ugrps);
		OldDirUse^.ugrps := tu;
		END;
	    td := OldDirUse^.next;
	    Tossdir(OldDirUse);
	    OldDirUse := td;
	    END; {WHile OldDirUse <> nil}

	END; {if yesno}
    end; {If WHEEL}
    END; {DoInit}


    {List code}

PROCEDURE HelpList;

    BEGIN 
    writeln(tty);
    writeln(tty,'  Format: LIST (information (on the LINEPRINTER) about) INFOTYPE');
    writeln(tty);
    writeln(tty,'     The LIST command is like the INFORMATION command, but');
    writeln(tty,'     it will list the information on the lineprinter. It');
    writeln(tty,'     has many subcommands; here are all of the possibilities');
    Writeln(tty);
    Writeln(tty,'    LIST GROUP GROUPNUMBER');
    Writeln(tty,'      -prints all of the users in that user/directory group');
    Writeln(tty,'    LIST DIRECTORY ST:<USERNAME>');
    Writeln(tty,'      -prints the user/directory groups of the given directory');
    Writeln(tty,'    LIST ALL GROUPS');
    Writeln(tty,'      -prints all of the information, sorted by groups');
    Writeln(tty,'    LIST ALL DIRECTORIES');
    Writeln(tty,'      -prints all of the information, sorted by directory name');
    writeln(tty);
    writeln(tty,'    LIST CHANGES');
    writeln(tty,'      -prints a list of all changes made in the current session');
    writeln(tty,'       of Groupe. Most useful after the INITIALIZE command to');
    writeln(tty,'       see what changes were made to the user group structure');
    writeln(tty,'       outside of the use of this program.');
    writeln(tty);
    Writeln(tty,'    LIST LOWEST-GROUP-NUMBER');
    Writeln(tty,'      -prints the number of the lowest unused group. That is,');
    Writeln(tty,'       it gives you the number you should use in creating your');
    Writeln(tty,'       next group. If your system gives some meaning to the numbers,');
    Writeln(tty,'       (for instance, reserving numbers 1000 through 1999 for a');
    Writeln(tty,'       certain special interest group), then this command will not');
    Writeln(tty,'       be very useful for you.');

    writeln(tty);
    Writeln(tty,'    LIST NAMES-OF-GROUPS ALPHABETICALLY');
    writeln(tty,'    LIST NAMES-OF-GROUPS NUMERICALLY');
    Writeln(tty,'      -prints only the names of the groups, one per line');
    writeln(tty,'       the second parameter specifies the type of sorting to use.');
    Writeln(tty);
    END;

    {Remove Code}

PROCEDURE Helpremove;

    BEGIN {helpremove}
    writeln(tty);
    Writeln(tty,'  Format: REMOVE (directory) ST:<USERNAME> (FROM) GROUPKIND (number) NUM');
    writeln(tty);
    Writeln(tty,'    The REMOVE command removes a user from a directory or');
    Writeln(tty,'    user group. ST:<USERNAME> is the directory name, GROUPKIND');
    writeln(tty,'    is either DIRECTORY-GROUP or USERGROUP, and NUM is the');
    writeln(tty,'    number of the group to remove.');
    writeln(tty);
    END; {HelpRemove}


PROCEDURE Doremove;

    VAR
	dirnum,
	grp,
	kind :integer;
	test :asciz;
	ok : boolean;
	indir,
	tmpdir:asciz;
	td : dirpnt;
	WorkGType : WorkGroupType;

    BEGIN
    if not(iswheel) then writeln(tty,'WHEEL or OPERATOR capability required.')
    else begin
    cmnoi('directory');
    Readdir(tmpdir,dirnum);
    Outtoin(tmpdir,indir);
    cmnoi('FROM');
    kind := ReadGroupKind;
    cmnoi('number');
    grp := Readgrp;
    IF NOT(Grpexists(grp)) THEN
	cmuerr('Group does not exist');
    Confirm;
    Findcredir(indir,td);
    CASE kind OF
	dirgrp: ok :=  Removedgroupok(td^.dgrps,grp);
	usergrp: ok := Removedgroupok(td^.ugrps,grp);
    END;
    IF NOT(ok) THEN
	cmuerr('User not in Group')
    ELSE
	BEGIN
	IF Kind = DirGrp then WorkGType := WorkDir ELSE WorkGType := WorkUser;
	Addwork(dirnum,grp,WorkGType,workrem);
	END; {Else}
    Cleandir;
    end;
    END;


    {UPDATE code}

    PROCEDURE HelpUpdate;

	BEGIN
	writeln(tty);
        writeln(tty,'  Format: UPDATE');
	writeln(tty);
	writeln(tty,'    The UPDATE command will let you write the Groupe data file, and');
	writeln(tty,'    will then make the changes you have requested. It will then');
	writeln(tty,'    return you to the Groupe> prompt. This command is just like');
	writeln(tty,'    the EXIT command, but Groupe will not quit after it has made');
	writeln(tty,'    the changes.');
	writeln(tty);
	END; {HelpUpdate}


PROCEDURE DoUpdate;
    {Perform the UPDATE command}

    BEGIN   {doUpdate}
    Cutwork;
    If (not(Iswheel)) THEN
        writeln(tty,'WHEEL or OPERATOR capability required.')
    ELSE BEGIN
    cmnoi ('the Groupe and system databases');
    confirm;
    IF (Not(Workempty(WorkUse)) or InitFlag or CreDesFlag) THEN
	BEGIN
    	cmini('Are you sure you wish to make the current changes permanent?');
	IF Readyesno THEN
	    BEGIN
	    Dowork;
	    Write(tty,'Writing data file...');
	    close(ingrp);
	    Rewrite(outgrp,'SYSTEM:GROUPE.BIN');
	    Writefile(outgrp);
	    Writeln(tty,'done.');
	    TossAllWork(WorkUse);
	    CreDesFlag := False;
	    InitFlag := False;
    	    END; {If}
       END {If Changes}
    ELSE {No Changes}
	BEGIN
	writeln(tty,'No net changes made since last EXIT/UPDATE command.');
        TossAllWork(WorkUse);
	END; {No Changes}
      END; {NeedsWheel}
    END;    {doUpdate}




    {Help Code}

    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}
    {The HELP command -- Describe Groupe or one of its commands.      }
    {                                                                 }
    {If describing a command FOO, does so by calling the procedure    }
    {helpFoo defined with each command.                               }
    {                                                                 }
    {This must be the last command before the main program so that all}
    {the procedures describing each command will be within its scope. }
    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}

PROCEDURE Helphelp;
    BEGIN   {helpHelp}
    writeln(tty);
    writeln(tty,'  Format: HELP');
    writeln(tty,'       or HELP COMMAND');
    writeln(tty);
    Writeln (tty, '    The HELP command can be used to describe this program.');
    Writeln (tty, '    If given no argument, it will give a general description');
    Writeln (tty, '    of the program.  If given a command name as an argument,');
    Writeln (tty, '    it will describe that individual command.');
    writeln(tty);
    END;    {helpHelp}

PROCEDURE Dohelp;


    PROCEDURE Helpovrview;
	BEGIN   {helpOvrview}
	writeln(tty);
	Writeln(tty,'    The Groupe program is a general user-directory group data base');
	Writeln(tty,'    facility. It allows you to create and destroy groups, as well as');
	Writeln(tty,'    add and delete individual directories from a group. It can also');
	Writeln(tty,'    print information about the groups and directories, and generate');
	Writeln(tty,'    system mailing lists for each of the groups. The best way to learn');
	Writeln(tty,'    about the program is through the HELP facility. Type HELP');
	Writeln(tty,'    to the Groupe> prompt to get general help about Groupe.');
	Writeln(tty,'    If you wish help about a specific command, then if you simply type');
	Writeln(tty,'    HELP COMMANDNAME, you will be given information on how to use that');
	Writeln(tty,'    command. Any comments, gripes, or suggestions should be sent to');
	Writeln(tty,'    Joel P. Bion, Graduate School of Business Computer facility,');
	writeln(tty,'    Stanford CA, 94305.');
	writeln(tty);
	writeln(tty,'    Groupe is Copyright (c) 1982, Stanford GSB Computer Facility.');
	writeln(tty,'    All rights reserved.');
	writeln(tty);
	END;    {helpOvrview}

    BEGIN   {doHelp}
    cmnoi ('on (type a command name or press "RETURN" for general help)');
    cmmult;
    Confirm;                        {option 1: confirm}
    ignore := cmkey (cmdtable);     {option 2: command name}
    CASE cmdo OF
	1: Helpovrview; {confirm}
	2: BEGIN        {CmKey}
	    Confirm;
	    CASE cmint OF
		abortcmd: Helpabort;
		addcmd: Helpadd;
		changecmd: Helpchange;
		createcmd: Helpcreate;
		destroycmd: Helpdestroy;
		exitcmd:   Helpexit;
	        forgetcmd: HelpForget;
		genercmd: Helpgener;
		helpcmd:   Helphelp;
		infocmd: Helpinfo;
		initcmd: Helpinit;
		listcmd: Helplist;
		removecmd: Helpremove;
		updatecmd: HelpUpdate;
	    END;    {case}
	    END;        {CmKey}
    END;    {case}
    END;    {doHelp}



    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}
    {Main program                                                     }
    {                                                                 }
    {Call the initialization procedure and then do the top-level loop.}
    {Parses commands and calls the procedures associated with each    }
    {command to do all the actual work.  For a command FOO, calls     }
    {procedure doFoo, which should take no arguments.                 }
    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}

BEGIN   {program}
Initialize;
PrintHeader;
WHILE true DO BEGIN
    cmini ('Groupe>');
    NewDefDir := innullasciz;        {clean up on reparse}
    NewDefKind := Innullasciz;
    NewDefGrp := Innullasciz;
    CASE cmkey (cmdtable) OF        {read a command}
        abortcmd: Doabort;
        addcmd: Doadd;
        changecmd: Dochange;
        createcmd: Docreate;
        destroycmd: Dodestroy;
        exitcmd:   Doexit;
        forgetcmd: DoForget;
        genercmd: Dogener;
        helpcmd:   Dohelp;
        infocmd: Doinfo(false);
        initcmd: Doinit;
        ListCmd: DoInfo(true);
        removecmd: Doremove;
        updatecmd: DoUpdate;
        END;    {case}
    END; {while}
END.    {program}
