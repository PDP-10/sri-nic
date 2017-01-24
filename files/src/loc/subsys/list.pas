(* [SRI-NIC]SRC:<LOC.SUBSYS>LIST.PAS.31, 16-Feb-87 23:30:29, Edit by VIVIAN *)
(* 	Make this do a new page on form feeds.  			*)
(* SRC:<LOC.SUBSYS>LIST.PAS.13 18-Dec-86 Vivian.  Nuke out unnecessary *)
(*	stuff - buffer, field.  Nuking out buffer makes it work for extra *)
(*	long lines.  Fix header to be bold.		  *)
(* SRC:<LOC.SUBSYS>LIST.PAS.7 1-Dec-86 Vivian.  Make list do laser *)
(*	printer too *)
(* [SRI-NIC]PS:<HSS.PROG>LIST.PAS.2, 11-Jan-85 12:08:02, Edit by HSS *)
(* Made list accept wild cards *)
(* A quick and dirty program *)
PROGRAM List;

INCLUDE 'pascal:pascmd.pas';		    (* COMND JSYS stuff *)

CONST
    esc=27;				    (* ASCII for ESC *)
    FF=12;				    (* ASCII for Form Feed *)
    CR=13;				    (* ASCII for carriage return *)
    odtim=220B;				    (* JSYS for current time *)
    jfns=30B;				    (* Translate JFN to string *)
    haltf=170B;				    (* Guess *)
    maxline=132;			    (* 132 columns/printer page *)
    swidth=80;				    (* Standard width *)
    slength=49;				    (* Standard length *)
TYPE	linetype=ARRAY[1..maxline] OF char;

VAR	today:PACKED ARRAY[1..21] OF char;	(* Current date *)
    filename:PACKED ARRAY[1..50] OF char;   (* Input file *)
    width,				    (* Page width *)
    length,				    (* Page length *)
    spacing,				    (* What kind of spacing? *)
    printtype,				    (* Laser or lineprinter? *)
    line,				    (* Current line # on page *)
    cursor,
    i,
    c,					    (* Command *)
    page:integer;			    (* Current page # *)
    longline,buffer:linetype;		    (* One line of input *)
    command,spacetable,printtab:table;
    error:boolean;

    (* Low level stuff *)


FUNCTION Tail(l:linetype):integer;
    (* Find the last non-space character in a line *)
    VAR     i:integer;
    BEGIN   (* Tail *)
    i:=width;
    WHILE l[i]=' ' DO
	i:=i-1;
    Tail:=i;
    END;    (* Tail *)

PROCEDURE Space;
    (* Write the appropriate number of blank (spacing) lines *)
    VAR     i:integer;
    BEGIN
    line:=line+spacing;
    FOR i:=2 TO spacing DO
	Writeln(output);
    END;

PROCEDURE Clearline(VAR l:linetype);
    VAR     i:integer;
    BEGIN
    FOR i:=1 TO maxline DO
	l[i]:=' ';
    END;

FUNCTION Blank(l:linetype):boolean;
    (* Returns true if the line is blank *)
    VAR     flag:boolean;
    BEGIN
    flag:=true;
    FOR i:=1 TO maxline DO
	IF l[i]<>' ' THEN flag:=false;
    Blank:=flag;
    END;

PROCEDURE Cmdinit;
    (* Build the table of switches *)
    BEGIN
    command:=tbmak(4);
    tbadd(command,4,'UNIT: ',0);
    tbadd(command,3,'WIDTH: ',0);
    tbadd(command,2,'SPACING: ',0);
    tbadd(command,1,'LENGTH: ',0);
    spacetable:=tbmak(3);
    tbadd(spacetable,3,'TRIPLE',0);
    tbadd(spacetable,2,'DOUBLE',0);
    tbadd(spacetable,1,'SINGLE',0);
    printtab:=tbmak(2);
    tbadd(printtab,2,'LASER',0);
    tbadd(printtab,1,'LINEPRINTER',0);
    END;

    (* High stuff *)

PROCEDURE Parsecmd;
    (* Parse the command line and initialize things *)
    VAR     blank:integer;
	done:boolean;
    BEGIN   (* Init *)
    width:=swidth;			    (* Default page width *)
    length:=slength;			    (* Default page length *)
    spacing:=1;				    (* Default spacing *)
    printtype:=1;			    (* Default print to lpt *)
    Cmdinit;				    (* Set up command switches *)
    cminir('File to be listed: ');	    (* Set up the prompt *)
    gjgen(120100000000b);
    cmfil(input);
    done:=false;
    WHILE NOT done DO BEGIN
	cmauto(false);
	c:=cmswi(command);
	IF c<>0 THEN BEGIN
	    cmauto(true);
	    CASE c OF
		1, -1: REPEAT
		    length:=cmnum;
		    IF length<1 THEN
			Writeln(tty,'?Length of a page must be positive.');
		UNTIL length >= 1;
		2,-2: BEGIN
		    spacing:=cmkey(spacetable);
		    END;
		3,-3: REPEAT
		    width:=cmnum;
		    IF (width>maxline) OR (width<20) THEN BEGIN
			Write(tty,'?Width must be between 20');
			Write(tty,'and 132 inclusive');
			END;
		UNTIL (width>maxline) AND (width<20);
		4,-4: BEGIN
		    printtype:=cmkey(printtab);
		    END;
	    END;
	    END
	ELSE
	    IF cmerr THEN BEGIN
		done:=true;
		cmauto(true);
		cmcfm;
		END;
	END;
    Jsys(odtim;-1:today,-1,200300B:0);	    (* Get the current date *)
    END;

PROCEDURE Getline;
    VAR     i:integer;
    BEGIN
    Clearline(longline);
    i:=0;
    IF NOT Eoln(input) THEN
	REPEAT
	    i:=i+1;
	    Read(input,longline[i]);
	UNTIL Eoln(input) OR (i=width);
    (* Check for FF *)
    IF (Input^ = chr(FF)) AND (i<>width) THEN
	READ(input,longline[i+1]);
    END;

(*)
 *  Procedure to write the header on each page.  Increments the
 *  page count, and sets the line counter to zero.  Gross.  I know
 *  this uses globals.  It's just a hack...
(*)
PROCEDURE WriteHeader;
    BEGIN
    page:=page+1;
    line:=0;
    IF printtype = 2 then (* set diablo mode to bold for laser *)
 	WRITE(output,chr(esc),'O');
    WRITE(output,filename,'  ',today,'		   page ',page:0);
    IF printtype = 2 then  (* set diablo mode back to normal for laser *)
        WRITE(output,chr(esc),'X')
    ELSE
	BEGIN (* Over strike on the LPT *)
	WRITE(output,chr(cr));
	WRITE(output,filename,'  ',today,'		   page ',page:0);
	END;
    WRITELN(output);
    WRITELN(output);
    WRITELN(output);
    END;
    
BEGIN	(* main *)
Parsecmd;				(* Parse the command line *)
IF printtype = 1 THEN
    Rewrite(output,'LPT:')		(*  LPT *)
ELSE IF printtype = 2 THEN
    Rewrite(output,'CDP:')
ELSE
    BEGIN
    Writeln(tty,'Goof getting output type');
    Jsys(haltf);
    END;

(* Let 'er rip *)
REPEAT
    FOR c:=1 TO 30 DO
	filename[c]:=' ';
    Jsys(jfns;-1:filename,0:input,211110000001B;c);
    Reset(input,filename,'/E');
    Clearline(longline);
    Clearline(buffer);
    page:=0;
    REPEAT
	WriteHeader;
	REPEAT
	    Getline;
	    FOR i:=1 TO width DO
		IF (longline[i] = chr(FF)) THEN
		    BEGIN	(* Special processing for form feed *)
		    Write(output,Chr(FF));
		    WriteHeader;
		    END
		ELSE
		    Write(output,longline[i]);
	    Writeln(output);
	    Space;
	    IF Eoln(input) THEN
		Readln(input);
	UNTIL (line>length) OR Eof(input);
	IF NOT Eof(input) THEN Write(output,Chr(FF));
    UNTIL Eof(input);
    i:=Nextfile(input);
    IF i<>0 THEN
	Writeln(output,Chr(FF));
UNTIL i=0;
Writeln(tty,'Your listing is now in the printer queue. ');
END.
