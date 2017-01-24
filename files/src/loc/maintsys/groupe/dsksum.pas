PROGRAM DskSum;

    include 'sys:pascmd.pas';
    {file of definitions for COMND jsys}
    {functions and procedures of the form CMxxx, TBxxx, or GJxxx}
    {and the TABLE type are defined in this file.}

    include 'sys:jsys.pas';
    {definitions for other jsyses}
    {this file contains constants for the JSYS procedure.}

    include 'sys:extern.pas';
    {definitions for some built-in pascal functions and}
    {procedures that aren't loaded by default}


CONST

    ctrla  = 01;    {control characters}
    ctrlb  = 02;    ctrlc  = 03;    ctrld  = 04;    ctrle  = 05;
    ctrlf  = 06;    ctrlg  = 07;    ctrlh  = 08;    ctrli  = 09;
    ctrlj  = 10;    ctrlk  = 11;    ctrll  = 12;    ctrlm  = 13;
    ctrln  = 14;    ctrlo  = 15;    ctrlp  = 16;    ctrlq  = 17;
    ctrlr  = 18;    ctrls  = 19;    ctrlt  = 20;    ctrlu  = 21;
    ctrlv  = 22;    ctrlw  = 23;    ctrlx  = 24;    ctrly  = 25;
    ctrlz  = 26;    deletekey = 177B;

    DIChan = 1;  {interrupt channel for screen refresh}
    DILevel = 1; {priority for interrupt}
    turnon = true;
    turnoff = false;

    infinity = 377777000000B;		{Infinite disk space on TOPS-20V5}

    indent = 3; {Indent this number of spaces for every level}

    ascizlen = 50;  {length of a short string (for filenames)}
			{for comparison, only the first (Ascizlen-1) chars}
			{are used; this is not a crock - this is efficiency}
    errmesslen = 100; {Size of error message buffer}

    {Subcommands for Yes-or-No questions}
    yeskey = 1;     {affirmative answer}
    nokey = 2;      {negative answer}
    yesnosize = 2;  {number of entries in Y-or-N table}


    {* * * * * * * * * * * * * * * * * * * * * * * * * * * *}
    {Global data types                                      }
    {                                                       }
    {strings, forms for system calls, records, action links }
    {* * * * * * * * * * * * * * * * * * * * * * * * * * * *}

TYPE

    asciz = PACKED ARRAY [1..ascizlen] OF char; {Standard string}
    ascizpnt = ^Asciz;

    errmess = PACKED ARRAY [1..errmesslen] OF char; {Error message buffer}


    HalfW = 0..777777B; {Simple enough concept - 18 bits out of 36}
    HalfNames = (Left, Right);		{Names of the two halfwords}
    HalfWords = PACKED ARRAY [HalfNames] of Halfw;

    datary = PACKED ARRAY [1..9] of char; {Date array}
    timary = PACKED ARRAY [1..10] of char; {Time of day array}
    flagset = PACKED SET OF 0..35;             {Boolean flags word}


    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}
    {Global variables                                                 }
    {                                                                 }
    {Constants that PASCAL won't let me declare, throwaway variables, }
    {execution parameters, stuff for PASCMD                           }
    {* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *}

VAR

{General variables, used throughout programs}

    f: text;                {temporary file}
    ignore: integer;        {for executing functions as procedures}
    ignoreb: Boolean;	    {Boolean version of above}
    nullasciz: asciz;       {for initializing strings}
    innullasciz:asciz;      {Forusrname}
    username: asciz;        {name of user running this program}
    dirbrkmask: brkmsk;     {for parsing directories as fields}
    usrbrkmask: brkmsk;     {for parsing usernames as fields}
    dirstr:   asciz;        {string of dir type for all directories}

    yesnotable: table;      {table for YES and NO}
    nums : set of '0'..'9';
    ttyhld : flagset;

{Program specific variables}
    dirname: asciz;		        {Current directory name - made
                                         global for interrupts}
    outfile: text;			{Output file for stuff}
    maxdepth : integer;			{maximum depth to go `down'}
    topdirname : asciz;			{The very tiptop directory name}
    topdirnum : integer;		{The very tiptop directory number}
    topdirresult : asciz;		{Top Result for user}
    pageThreshold : Integer;		{Lower limit threshold}



		       {Different versions of Write}

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


   {Interrupt handling}

    PROCEDURE PrintCurDir;
       {Called on ^A interrupt - prints current directory}

       BEGIN
       Write(tty,'Working on ');
       WriteAsciz(tty,DirName);
       Writeln(tty);
       END; {PrintCurDir - the interrupt routine}

    PROCEDURE SetUpInterrupts (PROCEDURE PrintCurDir);
       {Current directory info on ^A}

	CONST
	    CtrlA = 1000001B; (* .ticca,,DIChan *)

	BEGIN (* SetUpInterrupts *)
	    JSys (ATI; CtrlA);	 (* ATI% - Activate Terminal Interrupt *)
	    PSIDefine (DIChan, DILevel, PrintCurDir);
	    PSIEnable (DIChan);
	END; {SetUpInterrupts}



FUNCTION HasWildCards(Str : Asciz) : Boolean;
    {Return TRUE iff Str has a wildcard (* or %) in it}

    VAR
        Index : Integer;
        Temp : Boolean;

    BEGIN {HasWildCards}
    Temp := False;
    Index := 1;
    WHILE (Not(Temp) AND (Index <= AscizLen)) DO
	If ((Str[Index] <>  '*') AND (Str[Index] <> '%')) THEN
	    Index := Index + 1
	ELSE
	    Temp := True;
    HasWildCards := Temp;
    END; {HasWildCards}


PROCEDURE UPPERCASEIFY (INS: ASCIZ;VAR OUTS : ASCIZ);
   {WRITTEN IN UPPER CASE, THIS PROCEDURE CONVERTS A STRING TO UPPER CASE}

   VAR
       I : INTEGER;

   BEGIN
   FOR I := 1 TO ASCIZLEN DO
       BEGIN
       IF ((INS[I] >= 'a') AND (INS[I] <= 'z')) THEN
	   OUTS[I] := CHR((ORD(INS[I]) - ORD('a')) + ORD('A'))
       ELSE
	   OUTS[I] := INS[I];
       END; {FOR}
   END; {UPPERCASEIFY}


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


FUNCTION  NumDots(InSNum : Integer) : Integer;
   {Returns the number of "dots" in the Directory InsNum}

   VAR
       I, Total : Integer;
       Ins : Asciz;

   BEGIN
   ConvToString(InsNum,Ins);
   Total := 0;
   FOR I := 1 to AscizLen DO
       IF Ins[I] = '.' THEN
	   Total := Total + 1;
   NumDots := Total;
   END; {NumDots}

    	

    {This doesn't fit anywhere else}

PROCEDURE ContNo;
    {Halts, with no continue}
    
    BEGIN
    WHILE true DO
	BEGIN
        Jsys(Haltf,2);
        Writeln(tty,'Please resume DskSum at this point by using the START command.');
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

FUNCTION SubStr(A,B : Asciz) : Boolean;
    {True if A is substring or equal to B}

    CONST
	WLSTR = 0;	    {Compare two arbitrary strings}
	NOMFlag = 0; {WL%NOM}
	ABRFlag = 1; {WL%ABR}
	WILD = 565B;	    {The WILD Jsys}

    VAR
	Result : Integer;
	resval : flagset;

    BEGIN
    JSys(WILD,2;WLSTR,A,B;ResVal);
    Substr := Not((NOMFlag IN ResVal) or (ABRFlag in ResVal));
    END; {SubStr}


FUNCTION SEQ(a,b : asciz) : Boolean;
    {True if A = B}

    CONST LTFlag = 0;  {Sc%Lss}
	  SUBFlag = 1; {sc%Sub}
	  GTFlag = 2;  {sc%GTR}
	  
    VAR
	resval : flagset;

    BEGIN
    A[AscizLen] := Chr(0); B[AscizLen] := Chr(0);
    Jsys(STCMP,2;a,b;resval);
    SEQ :=Not ((LTFlag IN ResVal) or (SUBFlag in ResVal)
	       or (GTFlag in ResVal));
    END; {SEQ}



FUNCTION SGREQ(a, b : asciz):boolean;

    {Returns true if A >= B}
    {IGNORES CASE!}

    CONST LTFlag = 0;  {Sc%Lss}

    VAR
	resval : flagset;

    BEGIN
    A[AscizLen] := Chr(0); B[AscizLen] := Chr(0);
    Jsys(STCMP,2;A,B;ResVal);
    SGREQ := Not(LTFlag IN ResVal);
    END; {SGREQ}

FUNCTION SGT(a, b : asciz):boolean;

    {Returns true if A > B}
    {IGNORES CASE!}

    CONST GTFlag = 2;  {Sc%Gtr}

    VAR
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
    DskSum'S DATA IS ASSUMED TO HAVE TRAILING SPACES. THIS IS FOR
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




PROCEDURE Writefas (VAR f:FILE;str:asciz);
    {Write an entire asciz string to the file mentioned}

    VAR
	index : integer;

    BEGIN
    FOR index := 1 TO ascizlen DO   {Write the string}
	Write(f,str[index]);
    END; {WriteFas}


PROCEDURE Initialize;
    {do all one-time initialization}


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
        DirName := InNullAsciz;
	END;    {initStrs}


    BEGIN   {initialize}
    Nums := ['0'..'9'];	    {Set up set of digits}
    Buildyesno;             {build table for YES and NO}
    Initvars;               {set random small variables}
    Initstrs;               {set various arrays of char}
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


PROCEDURE Readdir(VAR dirname: asciz;VAR dirnum:integer);
    {parse an existing directory name}

    VAR hld : integer;
	hlddir:asciz;

    BEGIN   {Readdir}
    hld := cmdir;
    dirnum := hld;
    END;    {Readdir}


PROCEDURE Confirm;
    {do cmCfm, cleaning up things like DefaultDir}

    BEGIN   {confirm}
    cmhlp('Just hit RETURN to finish this input line');
    cmcfm;
    END;    {confirm}



FUNCTION Readyesno: boolean;
    {returns TRUE for a YES answer, FALSE for NO}

    BEGIN   {ReadYesNo}
    cmhlp ('YES or NO');
    Readyesno := (cmkey (yesnotable) = yeskey);
    Confirm;        {make user confirm with carriage return}
    END;    {ReadYesNo}

    PROCEDURE PrintTimeStamp(VAR F: Text);
	{Print the date and time on the file F}

    VAR
	DateHld : DatAry;
	TimeHld : TimAry;

    BEGIN
    DateHld := Date;
    TimDay(TimeHld);
    Writeln(f,'DskSum listing for ',DateHld,' ',TimeHld);
    Writeln(f);
    writeln(f,' Directory                                     Working Permanent      Used');
    writeln(f,' -------------------------------------------------------------------------');
    END; {PrintTimeStamp}


                                {PRINTHEADER}

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
    Write(tty,', and welcome to DskSum.');
    IF (thehour < 7) THEN
	writeln(tty,' (Up early, (or late) aren''t we?)')
    ELSE
	writeln(tty);
    writeln(tty,'Type ''?'' at any time if you are unsure of what to do.');
    END; {PrintHeader}


                                {PROGRAM HELP}

Procedure Printhelp;
    {Print Information on how to use this program}

       BEGIN
       writeln(tty);
       writeln(tty,'DskSum is a program that will summarize your use of disk space. From this,');
       writeln(tty,'you can see where your disk space is going. The program will');
       writeln(tty,'ask you four questions:');
       writeln(tty);
       writeln(tty,'  1: What directories you want to check.');
       writeln(tty,'      Here, you specify a directory of the form: PS:<A.JPBION*>,');
       writeln(tty,'      telling the program to check all of the PS:<A.JPBION> directories.');
       writeln(tty,'  2: What is the maximum depth to search in checking the directories.');
       writeln(tty,'      Here, you tell it how far to go `down'' in the search. For example,');
       writeln(tty,'      specifying <A.JPBION*> above with a depth of one will have DskSum');
       writeln(tty,'      check <A.JPBION.A>, but not <A.JPBION.A.B>');
       writeln(tty,'  3: Page number threshold.');
       writeln(tty,'      If this is non-zero, any directories using up less space than this');
       writeln(tty,'      will not be printed.');
       writeln(tty,'  4: The Output File');
       writeln(tty,'       This is simply the name of the file where you would like the output');
       writeln(tty,'       to go.');
       writeln(tty);
       writeln(tty,'NOTE: DSKSUM takes a bit of time to complete. Be prepared to wait a while');
       writeln(tty,'for it to finish. To see how much of the disk it has worked on, just type');
       writeln(tty,'CONTROL-A at any time, and the program will respond with the name of the');
       writeln(tty,'directory it is currently reading.');
       writeln(tty);
       END; {PrintHelp}

		       {PARSING ROUTINES}

PROCEDURE GetRoot;
   {This procedure will get the top level directory to start the search on}

   VAR
      dirName : Asciz;

   BEGIN
   writeln(tty);
   CmIni('Please give the directories you wish to summarize: ');
   CmHlp('Directories specifier, as in PS:<REG*>. Hitting <return> gives PS:<*>');
   CmDef('PS:<*>');
   TopDirNum := CmDirW;
   Ignore := CMAtom(TopDirName);
   Uppercaseify(TopDirname, DirName);
   InToOut(DirName, TopDirName);
   Confirm;
   END; {GetRoot}


PROCEDURE GetDepth;
   {This procedure will get the maximum depth to go down}

   BEGIN
   writeln(tty);
   Writeln(tty,'Now enter the depth. If you want infinite depth, just hit return.');
   CMini('Enter the maximum depth of the directories you want summarized: ');
   CMHlp('Depth. Hit <return> for infinite depth');
   CMDef('-1');
   MaxDepth := CMNum;
   Confirm;
   END; {GetDepth}
      
PROCEDURE GetFileName;

   BEGIN
   writeln(tty);
   Writeln(tty,'Please enter an output filename for the summary file. If you');
   writeln(tty,'just hit <return>, the summary will go to DSKSUM.OUT');
   CmIni('Enter a filename for the output: ');
   CmHlp('Filename for DSKSUM output, hit <return> for DSKSUM.OUT');
   CmDef('DSKSUM.OUT');
   CmOfi(outfile);   
   Confirm;
   rewrite(outfile);
   END; {GetFileName}

PROCEDURE GetPageThreshold;
   {Gets the page threshold}

   BEGIN
   writeln(tty);
   Writeln(tty,'Finally, please enter the page threshold. This number tells this program not');
   writeln(tty,'to print summaries of directories with used pages of less than this amount.');
   writeln(tty,'If you enter ZERO here, DSKUSE will set the treshold to zero, causing');
   writeln(tty,'ALL directories to be summarized.');
   CmIni('Page Threshold? ');
   CMHlp('Page Threshold, an integer');
   PageThreshold := CmNum;
   Confirm;
   END; {GetPageThreshold}

		       {Main subroutine - Recursive Procedure DoWork}

PROCEDURE DoworkandPrintIt(TDName : ASCIZ; TDNum, CurDepth : Integer;
			       VAR DiskUseTotal, LastDir : Integer;
			       VAR VeryLast : Boolean; AboveUsed : Integer);
   {Does all of the work. TDName contains WILDS, TDNum contains number of
    first directory in sequence. Credit must be given where credit is due.
    The REPEAT loop idea comes from Sandy.Pas, written by Kirk Lougheed}

   CONST
       IRCDirFlags = 2000000B;		{RC%AWL - allow wildcards}
       LRCDirFlags = 6000000B;		{above + RC%STP - step to next one}
       RCNMD = 5;			{No matching directory - from RCDIR}

   VAR
       SupLine : Boolean;               {TRUE if we just outputted a lower
                                         level  or line - makes output prettier}
       Used : Integer;                  {Number of used pages for current dir}
       DiskUse : Integer;		{Current Disk Use count}
       LowerUse : Integer;		{Temporary useage counter}
       DirNum,
       NewDirNum : Integer;		{Current Directory Number}
       TempString : Asciz;		{needed since RCDIR destroys things}
       RetVal : Integer;		{return +? on jsys}
       InFlags : Flagset;
       OutFlags : Flagset;
       Done : Boolean;			{True if scan is done}
       
   PROCEDURE WriteIndent(VAR F : Text);
       {Write nice looking indentation to the output file F}
       
       VAR I : Integer;

	   BEGIN
	   WRite(f,' ':Indent);
	   FOR I := 1 TO (Curdepth-1) DO
	       Write(F,'|':1,'  ':(Indent-1));
	   END; {WriteIndent}

   PROCEDURE PrintThisDir(NumDir : Integer; NameDir : Asciz; VAR Space : Integer);
       {Print Specs on this directory}

       VAR
	   Working,
	   Permanent,
	   Used : Integer;			{Various parameters from GTDAL}

	   PROCEDURE WriteSpace(VAR F: Text;Value : Integer);

		BEGIN
	        IF (Value = Infinity) THEN
		       Write(f,'  Infinite')
	        ELSE
		       Write(f,Value:10)
	        END; {WriteSize}

	   BEGIN
           Jsys(GTDAL,-1;numdir;Working,Used,Permanent);
	   Space := Space + Used;
	   If (((CurDepth <= MaxDepth) OR (MaxDepth < 0)) AND (Used >= PageThreshold)) THEN
	       BEGIN
	       Writeindent(outfile);
	       Writeasciz(outfile,NameDir);
	       Write(outfile,' ':(45-GetSig(NameDir)-Indent*(Curdepth)));
	       WriteSpace(OutFile,Permanent);
	       WriteSpace(OutFile,Working);
               WriteSpace(OutFile,Used);
	       Writeln(Outfile);
	       END;
	   END; {PrintThisDir}

       PROCEDURE PrintSum;
	   {Called at the end, this procedure prints a summary of disk use.}

	   BEGIN
	   IF ((CurDepth <= MaxDepth) OR (MaxDepth < 0)) THEN
	       BEGIN
{	       WriteIndent(Outfile);
	       Writeln(outfile,' ':(65-Indent*(CurDepth)),'----------'); }
	       WriteIndent(Outfile);
	       Write(outfile,'Total for this group:',' ':(65-21-Indent*(Curdepth)));
	       Writeln(outfile,DiskUse:10);
	       END;
	   END; {PrintSum}

   BEGIN
   SupLine := TRUE;
   VeryLast := False;
   DiskUse := AboveUsed;			{`Zero' current diskUse Sum}
   TempString := TDName;
   DirNum := TDNum;
   OutFlags := Outflags - [RCNMD];
   REPEAT
       LastDir := Dirnum;
       Done := RCNMD IN OutFlags;
       VeryLast := Done;
       IF Not(Done) THEN
	   BEGIN
           Convtostring(DirNum, DirName);
           TempString := TDName;
           Jsys(Rcdir,2,RetVal;LRCDirFlags,TempString,DirNum;OutFlags,Ignore,NewDirNum);
           VeryLast := RCNMD IN OutFlags;
           IF (NumDots(NewDirNum) > NumDots(DirNum)) THEN
	      BEGIN
	      IF Not(SupLine) THEN
	          BEGIN
    	          WriteIndent(Outfile);
	          Writeln(Outfile,'|');
		  END;
	      PrintThisDir(DirNum,DirName,DiskUse);
              Jsys(GTDAL,-1;DirNum;Ignore,Used,Ignore);
	      DoWorkAndPrintIt(TDName,NewDirNum,CurDepth+1,LowerUse,LastDir,VeryLast,Used);
	      DiskUse := DiskUse + LowerUse;
	      DirNum := LastDir;
	      SupLine := True;
	      END
	   ELSE
	      BEGIN
              PrintThisDir(DirNum,DirName,DiskUse);
	      SupLine := False;
	      IF (NumDots(NewDirNum) < NumDots(DirNum)) THEN
	         BEGIN
		 Done := True;
		 LastDir := NewDirNum;
		 END
	      ELSE
		 DirNum := NewDirNum;
             END;
       END;
   UNTIL (Done or VeryLast);
   PrintSum;
   DiskUseTotal := DiskUse - AboveUsed;  {Hack forced upon us to print totals}
				         {nicely}
   If (CurDepth > 1) THEN
	   BEGIN
	   WriteIndent(Outfile);
	   writeln(outfile);
	   END;
   END; {DoWorkAndPrintIt}











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
cmini('Do you need help in using this program? ');
IF Readyesno THEN
       PrintHelp;
GetRoot;
GetDepth;
GetFileName;
GetPageThreshold;
PrintTimeStamp(outfile);
SetUpInterrupts(PrintCurDir);
writeln(tty,'Remember, typing CONTROL-A at any time will give you the name of');
writeln(tty,'the directory DSKUSE is currently reading.');
DoworkandPrintIt(TopDirName,TopDirNum,1,ignore,ignore,ignoreb,0);
RClose(Outfile);
END.    {program}
