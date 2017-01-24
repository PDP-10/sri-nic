(* SX:<K.KRONJ>FFIND.PAS.16,  7-May-82 21:24:34, Edit by K.KRONJ *)
(*  try parse-only if normal file parse fails. *)
(*  add tty number and parse-only flag to jfn display. *)
(*  fix problem with 0 extensions in filename matching. *)
(* SX:<K.KRONJ>FFIND.PAS.1, 15-Apr-82 22:31:52, Edit by K.KRONJ *)
(*  conversion to PASCAL and cleanup of original FAIL program by KDO *)

program FFind;

    include
	'sys:jsys.pas',	    (* definitions for JSYS system calls *)
	'sys:pascmd.pas',   (* definitions for PASCMD command reader *)
	'sys:extern.pas';   (* defintions for some more P20 externs *)

    const
	FNLen = 150;	(* maximum chars in filename *)

	TestMonRd = 0;	(* .RDTST *)
	SymMonRd = 1;	(* .RDSYM *)
	JSBMonRd = 2;	(* .RDJSB *)
	PSBMonRd = 3;	(* .RDPSB *)
	ForkMonRd = 4;	(* .RDSTS *)

	JSVarSixBit  = 526366416200b;    (* SIXBIT/JSVAR/ *)
	MLJFNSixBit  = 555452465600b;    (* SIXBIT/MLJFN/ *)
	FilDDNSixBit = 465154444456b;	(* SIXBIT/FILDDN/ *)
	FilDNMSixBit = 465154445655b;	(* SIXBIT/FILDMN/ *)
	FilNENSixBit = 465154564556b;	(* SIXBIT/FILNEN/ *)
	FilVerSixBit = 465154664562b;	(* SIXBIT/FILVER/ *)
	FilDevSixBit = 465154444566b;	(* SIXBIT/FILDEV/ *)
	SysFkSixBit  = 637163465300b;	(* SIXBIT/SYSFK/ *)
	FilOFNSixBit = 465154574656b;	(* SIXBIT/FILOFN/ *)
	MaxJFNSixBit = 554170524656b;   (* SIXBIT/MAXJFN/ *)
	FilStsSixBit = 465154636463b;   (* SIXBIT/FILSTS/ *)

    type
	WordSet = packed set of 0..35;
	FName = packed array [1..FNLen] of char;

    var
	JSVarAddress: integer;	(* start of JSB *)
	JFNOffset: integer; (* number words per JFN *)
	Ignore: integer;    (* scratch var for jsys calls etc *)
	JobNumber: integer; (* job we're testing this cycle *)
	OldJob: integer;    (* job number last printed *)

	NullString: FName;  (* for clearing out strings *)
	Target: FName;	    (* target filename (perhaps wild) *)
	FoundOne: boolean;  (* whether any JFNs have matched yet *)

	NumJobs: integer;   (* maximum number of jobs on the system *)

    procedure stopForever;

	begin (* stopForever *)
	    while True do begin
		jsys (HaltF);
		writeLn (tty, '?Can''t continue');
	    end;    (* while *)
	end; (* stopForever *)

    procedure writeOctal (Num, NCols: integer);

	const
	    FillBit = 100000b;	(* left half quantity: NO%LFL *)

	begin (* writeOctal *)
	    jsys (NOut, 1; PRIOU, Num, (NCols + FillBit):8);
	end; (* writeOctal *)

    function LeftHalf (FullWord: integer): integer;

	var
	    Switcher: record
			  case boolean of
			      True: (int: integer);
			      False: (half: packed array [1..2] of 0..777777b);
		      end;  (* Switcher *)

	begin	(* LeftHalf *)
	    Switcher.int := Fullword;
	    LeftHalf := Switcher.half[1];
	end;	(* LeftHalf *)

    function RightHalf (FullWord: integer): integer;

	var
	    Switcher: record
			  case boolean of
			      True: (int: integer);
			      False: (half: packed array [1..2] of 0..777777b);
		      end;  (* Switcher *)

	begin	(* RightHalf *)
	    Switcher.int := Fullword;
	    RightHalf := Switcher.half[2];
	end;	(* RightHalf *)

    procedure makeSureMonRdWorks;

	const
	    TestShouldRet = 123456b;	(* .TSTNM *)

	var
	    TestRet: integer;

	begin (* makeSureMonRdWorks *)
	    jsys (MonRd; TestMonRd, 0; Ignore, TestRet);
	    if TestRet <> TestShouldRet then begin
		writeLn (tty, '?MONRD% jsys not implemented here');
		stopForever;
	    end; (* if *)
	end; (* makeSureMonRdWorks *)

    function GetWord (Address: integer): integer;

	var
	    TheWord: integer;

	begin	(* GetWord *)
	    jsys (MonRd; JSBMonRd, JSVarSixBit, Address - JSVarAddress,
		  JobNumber; Ignore, TheWord);
	    GetWord := TheWord;
	end;	(* GetWord *)

    function GetJFNInfo (JFN, SixBit: integer): integer;

	var
	    MonRdRet: integer;
	    TheWord: integer;

	begin (* GetJFNInfo *)
	    jsys (MonRd; JSBMonRd, SixBit, JFNOffset * RightHalf (JFN),
		  JobNumber; MonRdRet, TheWord);
	    if MonRdRet = 0 then
		GetJFNInfo := TheWord
	    else
		GetJFNInfo := 0;    (* bad call to MONRD, return safe value *)
	end; (* GetJFNInfo *)

    procedure initGlobals;

	const
	    JobRunTimeTable = 1;    (* .JOBRT *)

	var
	    Index: integer;

	begin	(* initGlobals *)
	    for Index := 1 to FNLen do
		NullString [Index] := Chr (0);
	    jsys (Getab, 1; -1:JobRunTimeTable; NumJobs);   (* get num jobs *)
	    NumJobs := -NumJobs;    (* but it is negative so negate it *)
	    jsys (MonRd; SymMonRd, JSVarSixBit; Ignore, JSVarAddress);
	    jsys (MonRd; SymMonRd, MlJFNSixBit; Ignore, JFNOffset);
	    FoundOne := False;	(* haven't found any matches yet *)
	    OldJob := -1;	(* no previous job for match *)
	end;	(* initGlobals *)

    procedure append (C: char; var Str: FName; var Index: integer);

	begin (* append *)
	    if Index > FNLen then begin
		writeLn (tty, 'String out of bounds: "', Str, '"');
		stopForever;
	    end;    (* if *)
	    Str [Index] := C;
	    Index := Index + 1;
	end; (* append *)

    procedure appendNumber (N: integer; var Str: FName; var Index: integer);

	begin (* appendNumber *)
	    if N = 777775b then
		append ('*', Str, Index)
	    else begin
		if N < 0 then begin
		    N := -N;
		    append ('-', Str, Index);
		end;	(* if *)
		if N > 9 then
		    appendNumber (N div 10, Str, Index);
		append (Chr (Ord ('0') + (N mod 10)), Str, Index);
	    end;    (* else *)
	end; (* appendNumber *)

    procedure getTargetFile;

	const
	    OldWild =   101100000000b;	(* deleted, wild *)
	    ParseOnly = 000140000000b;	(* same, but parse-only *)
	    StarBits = 000010000000b;	(* bits to see if generation is '*' *)
	    JFNSBits = 111100000001b;	(* bits for JFNS jsys *)

	var
	    Index: integer;
	    StarExt: boolean;	(* because FILVER gives me random number *)

	begin	(* getTargetFile *)
	    cmIniR ('File: ');	(* prompt *)
	    cmAuto (False);	(* turn off system error handling *)
	    gjGen (OldWild);	(* set up for reading a file *)
	    cmFil (Input);	(* read a file name *)
	    if CmErr then begin	(* was it parsed? *)
		cmAuto (True);	(* no, turn error handling back on *)
		gjGen (ParseOnly);
		cmFil (Input);	(* and try again, parse-only *)
	    end (* if *) else
		cmAuto (True);	(* else just turn error handling back on *)
	    cmCfm;		(* confirm with carriage return *)
	    jsys (JFNS; Target, CurJFN (Input), StarBits, 0);
	    StarExt := (Target [1] = '*');  (* check extension for "*" *)
	    Target := NullString;
	    jsys (JFNS; Target, CurJFN (Input), JFNSBits, 0);
	    Index := 1;
	    while Target [Index] <> Chr (0) do
		Index := Index + 1; (* search for null *)
	    if Index = 1 then  (* if no filename, seperate file from ext *)
		append ('.', Target, Index)
	    else if Target [Index - 1] = ':' then   (* same if device only *)
		append ('.', Target, Index);
	    append ('.', Target, Index);    (* append extension to filename *)
	    if StarExt then
		append ('*', Target, Index) (* either as a star *)
	    else begin			    (* or as a number *)
		jsys (GJInf; ; Ignore, Ignore, JobNumber);
		appendNumber (RightHalf (GetJFNInfo (CurJFN (Input),
						     FilVerSixBit)),
			      Target, Index);
	    end;    (* else *)
	    RCLose (Input);	(* release the JFN after getting string *)
	end;	(* getTargetFile *)

    function JobExists: boolean;
	(* make sure there is a job on the current job number *)

	const
	    GetRunTime = 1;  (* .JOBRT *)

	var
	    RunTime: integer;

	begin	(* JobExists *)
	    jsys (GETAB, 1; JobNumber:GetRunTime; RunTime);
	    JobExists := (RunTime >= 0);
	    (* according to the original code, this is the test *)
	    (* used in the monitor for job existance...seems strange to me *)
	end;	(* JobExists *)

    procedure findFile;

	const
	    FileOKBit = 10; (* GS%NAM ... what is this really for? *)

	var
	    MaxJFN: integer;
	    ThisJFN: integer;
	    FileStat: wordSet;
	    JobFork: integer;

	function FileMatches: boolean;

	    var
		FString: FName;
		FIndex: integer;
		MonWord: integer;

	    procedure getString (Addr: integer; Quoting: boolean);

		var
		    Word: record
			case boolean of
			    True:  (Int: integer);
			    False: (Str: packed array [1..5] of char);
		    end;    (* Word *)

		    Index: integer;
		    Done: boolean;

		function NeedsQuoting (C: char): boolean;

		    begin (* NeedsQuoting *)
			NeedsQuoting := not (((Ord (C) >= Ord ('A')) and
					      (Ord (C) <= Ord ('Z'))) or
					     (C in ['0'..'9', '-', '_', '$']));
		    end; (* NeedsQuoting *)

		begin (* getString *)
		    Done := False;
		    while not Done do begin
			Addr := Addr + 1;
			Index := 1;
			Word.Int := GetWord (Addr);
			while (Index <= 5) and (not Done) do begin
			    if Word.Str [Index] = Chr (0) then
				Done := True
			    else begin
				if Quoting then
				    if NeedsQuoting (Word.Str [Index]) then
					append (Chr (22), FString, FIndex);
				append (Word.Str [Index], FString, FIndex);
				Index := Index + 1;
			    end;    (* else *)
			end;	(* while *)
		    end;    (* while *)
		end; (* getString *)

	    begin (* FileMatches *)
		FIndex := 1;
		getString (LeftHalf (GetJFNInfo (ThisJFN, FilDDNSixBit)),
			   False);
		append (':', FString, FIndex);
		MonWord := LeftHalf (GetJFNInfo (ThisJFN, FilDNMSixBit));
		if MonWord > 0 then begin
		    append ('<', FString, FIndex);
		    getString (MonWord, False);
		    append ('>', FString, FIndex);
		end;	(* if *)
		MonWord := GetJFNInfo (ThisJFN, FilNENSixBit);
		if LeftHalf (MonWord) > 0 then
		    getString (LeftHalf (MonWord), True);
		append ('.', FString, FIndex);
		if RightHalf (MonWord) > 0 then
		    getString (RightHalf (MonWord), True);
		MonWord := GetJFNInfo (ThisJFN, FilVerSixBit);
		JobFork := LeftHalf (MonWord);
		append ('.', FString, FIndex);
		appendNumber (RightHalf (MonWord), FString, FIndex);
		append (Chr (0), FString, FIndex);
		jsys (Wild; 0, Target, FString; MonWord);
		FileMatches := (MonWord = 0);
	    end; (* FileMatches *)

	procedure printStats;
	    (* print info about the holder of the JFN *)

	    const
		ParseBit = 11;	(* if jfn is parse-only *)
		OpenBit = 0;	(* whether file is opened *)
		ReadBit = 1;	(* if open for reading *)
		WriteBit = 2;	(* if open for writing *)
		ExecuteBit = 3;	(* if open for executing *)
		RndAccBit = 4;	(* if random access allowed in writing *)
		EOFBit = 5;	(* if end-of-file reached *)
		ErrorBit = 6;	(* if error found *)
		RestBit = 17;	(* if restricted *)

		JobTerm = 1;	(* .JITNO *)

		BUName = '                             ';
		NLName =         'Not logged in        ';
		Header = 'Job Tty User                 JFN OFN Frk Sys  Flags';

	    var
		Terminal, UserNumber: integer;
		Index: integer;
		UserName: FName;
		SysFork: integer;

	    begin (* printStats *)
		if not FoundOne then begin
		    write (tty, 'Jobs accessing file ');
		    Index := 1;
		    while Target [Index] <> Chr (0) do begin
			write (tty, Target [Index]);
			Index := Index + 1;
		    end;    (* while *)
		    writeLn (tty, ' are:');
		    writeLn (tty);
		    writeLn (tty, Header);
		    FoundOne := True;
		end;	(* if *)
		if JobNumber = OldJob then
		    write (tty, BUName)
		else begin
		    write (tty, JobNumber:3, ' ');
		    jsys (GetJi, 1; JobNumber, -2:3, JobTerm;
			  Ignore, Ignore, Terminal, UserNumber);
		    if Terminal = -1 then
			write (tty, 'Det')
		    else
			writeOctal (Terminal, 3);
		    write (tty, ' ');
		    if UserNumber = 0 then
			write (tty, NLName)
		    else begin
			UserName := NullString;
			jsys (DirSt, 1; UserName, 500000b:UserNumber);
			for Index := 1 to 19 do
			    if UserName[Index] = Chr (0) then
				write (tty, ' ')
			    else
				write (tty, UserName[Index]);
			if UserName [20] = Chr (0) then
			    write (tty, '  ')
			else if UserName [21] = Chr (0) then
			    write (tty, UserName [20], ' ')
			else
			    write (tty, '..');
			OldJob := JobNumber;
		    end;    (* else *)
		end;	(* else *)
		writeOctal (ThisJFN, 3);
		write (tty, ' ');
		if (OpenBit in FileStat) and not (ParseBit in FileStat) then
		    writeOctal (LeftHalf (GetJFNInfo (ThisJFN,
						      FilOFNSixBit)), 3)
		else
		    write (tty, ' --');
		write (tty, ' ');
		writeOctal (JobFork, 3);
		jsys (MonRd; JSBMonRd, SysFkSixBit, JobFork, JobNumber;
		      Ignore, SysFork);
		write (tty, ' ');
		writeOctal (RightHalf (SysFork), 3);
		write (tty, ' ');
		if ParseBit in FileStat then
		    write (tty, ' Parse')
		else if not (OpenBit in FileStat) then
		    write (tty, ' NOpen');
		if ReadBit in FileStat then
		    write (tty, ' Rd');
		if WriteBit in FileStat then begin
		    if RndAccBit in FileStat then
			write (tty, ' Wr')
		    else
			write (tty, ' App');
		end;	(* if *)
		if ExecuteBit in FileStat then
		    write (tty, ' Xct');
		if RestBit in FileStat then
		    write (tty, ' Res');
		if EOFBit in FileStat then
		    write (tty, ' EOF');
		if ErrorBit in FileStat then
		    write (tty, ' Err');
		writeLn (tty);
	    end; (* printStats *)

	begin (* findFile *)
	    jsys (MONRD; JSBMonRd, MaxJfnSixBit, 0, JobNumber; Ignore, MaxJFN);
	    for ThisJFN := 1 to MaxJFN do begin
		jsys (MonRd; JSBMonRd, FilStsSixBit, JFNOffset*ThisJFN,
		      JobNumber; Ignore, FileStat);
		if FileOKBit in FileStat then
		    if FileMatches then
			printStats;
	    end;    (* for *)
	end; (* findFile *)

    procedure sayNoneFound;

	var
	    Index: integer;

	begin	(* sayNoneFound *)
	    write (tty, 'The file ');
	    Index := 1;
	    while Target [Index] <> Chr (0) do begin
		write (tty, Target [Index]);
		Index := Index + 1;
	    end;    (* while *)
	    writeLn (tty, ' is not being accessed by any job.');
	end;	(* sayNoneFound *)

    begin   (* FFind *)
	makeSureMonRdWorks;
	initGlobals;
	getTargetFile;
	for JobNumber := 0 to NumJobs - 1 do
	    if JobExists then
		findFile;
	if not FoundOne then
	    sayNoneFound;
    end.    (* FFind *)
