(*$d-,c-,v:000001000005b*)
Program Relprt(input,output);

{  program to print relocatable files for debugging
					-- Greg Satz }
type
	string = packed array [1..10] of char;

var
	input: file of integer;

	i,count,tmp: integer;
	rad50_chars: packed array [1..40] of char;
	filename, device: alfa;
	word_count: integer;

	word: packed record {for word manipulation}
			  case integer of
			      1:(full_word: integer);
			      2:(second_half, first_half: 0..777777b);
			      3:(code: 0..17b;
				 rest: 0..37777777777b);
			      4:(cpu: 0..77b;
				 compiler: 0..7777b;
				 blank_common: 0..777777b);
			      5:(asciz: packed array [1..5] of 0..127;
				 bit35: 0..1)
			  end;

procedure from6(i: integer; var a: alfa); extern;
function ipower(a,b:integer):integer; extern;
procedure quit; extern;

procedure rad50(number: integer);

{prints the character value of a radix 50 number}

    const
	    len = 5; {number of characters in rad50 per word}

    var
	i:integer;
	j:integer;


    begin {rad50}
	for i:=len downto 0 do
	    begin
		j:=number div ipower(40,i);
		if j <> 0 then
		    write(output,rad50_chars[j+1]);
		number:=number-(j*ipower(40,i))
	    end
    end; {rad50}

procedure skip(count: integer);

{skips count number of input words}

var
    i: integer;

    begin {skip}
	for i:=1 to count do
	    begin
		get(input);
		word_count:=word_count+1
	    end
    end; {skip}


begin {main}
    rad50_chars:=' 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.$%';
    word_count:=0;
    reset(input);
    rewrite(output);

    while not eof(input) do
	begin
	    word.full_word:=input^;
	    write(output,'Block Type: (',word.first_half:6:o,') ');
	    case word.first_half of

		0b:
		    begin
		        writeln(output,' Zero word -- Ignored,,',
				 word.second_half:6:o);
			if word.second_half <> 0 then
			    skip(word.second_half+1)
		    end;

		1b:
		    begin
			writeln(output,' Code,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		2b:
		    begin
			writeln(output,' Symbols,,',word.second_half:6:o);
			count:=word.second_half div 2;
			get(input); {relocation word}
			word_count:=word_count+1;
			write(output,'Global symbols: ');
			for i:=1 to count do
			    begin
				get(input);
				word_count:=word_count+1;
				word.full_word:=input^;
				tmp:=input^;
				if word.code = 14b then {code=60b}
				    begin {global request}
					get(input);
					word_count:=word_count+1;
					word.full_word:=input^;
					if word.code = 00b then
					    begin {valid name}
						word.full_word:=tmp;
						rad50(word.rest);
						write(output,', ')
					    end
				    end
				else
				    begin
					get(input);
					word_count:=word_count+1
				    end
			    end;
			writeln(output)
		    end;

		3b:
		    begin
			writeln(output,' HISEG,,',word.second_half:6:o);
			count:=word.second_half;
			get(input); {relocation word}
			word_count:=word_count+1;
			get(input);
			word_count:=word_count+1;
			word.full_word:=input^;
			writeln(output,'HISEG break,,origin: ',
				   word.first_half:6:o,',,',
				   word.second_half:6:o);
			if count = 2 then
			    begin
				get(input);
				word_count:=word_count+1;
				word.full_word:=input^;
				writeln(output,'LOSEG break,,origin: ',
				   word.first_half:6:o,',,',
				   word.second_half:6:o)
			    end
		    end;

		4b:
		    begin
			writeln(output,'Entry,,',word.second_half:6:o);
			count:=word.second_half;
			get(input); {relocation word}
			word_count:=word_count+1;
			write(output,'Symbols: ');
			while count <> 0 do
			    begin
				get(input);
				if input^ <> 0 then
				    begin
					word_count:=word_count+1;
					rad50(input^);
					write(output,', ');
					count:=count-1
				    end
			    end;
			writeln(output)
		    end;

		5b:
		    begin
			writeln(output,'End,,',word.second_half:6:o);
			get(input); {relocation word}
			word_count:=word_count+1;
			get(input);
			word_count:=word_count+1;
			i:=input^;
			get(input);
			word_count:=word_count+1;
			writeln(output,'HISEG break: ',i:12:o,
			    	       ' LOSEG or ABS break: ',input^:12:o)
		    end;

		6b:
		    begin
			writeln(output,'Name,,',word.second_half:6:o);
			count:=word.second_half;
			get(input); {relocation word}
			word_count:=word_count+1;
			get(input);
			word_count:=word_count+1;
			write(output,'Name: ');
			rad50(input^);
			writeln(output);
			if count = 2 then
			    begin
				get(input);
				word_count:=word_count+1;
				word.full_word:=input^;
				write(output,'CPU: ');
				case word.cpu of

				    20b: write(output,' KS10,');
				    4b:	 write(output,' KL10,');
				    2b:	 write(output,' KI10,');
				    1b:	 write(output,' KA10,');
				    others: write(output,' Unknown,')
				end; {case}
				write(output,' Compiled by ');
				case word.compiler of

				    0b: writeln(output,'Unknown');
				    1b: writeln(output,'Not used');
				    2b:	writeln(output,'COBOL-68');
				    3b:	writeln(output,'ALGOL');
				    4B: writeln(output,'NELIAC');
				    5b:	writeln(output,'PL/I');
				    6b:	writeln(output,'BLISS');
				    7b:	writeln(output,'SAIL');
				    10b: writeln(output,'FORTRAN');
				    11b: writeln(output,'MACRO');
				    12b: writeln(output,'FAIL');
				    13b: writeln(output,'BCPL');
				    14b: writeln(output,'MIDAS');
				    15b: writeln(output,'SIMULA');
				    16b: writeln(output,'COBOL-74');
				    17b: writeln(output,'(Reserved)');
				    20b: writeln(output,'BLISS-36');
				    21b: writeln(output,'BASIC');
				    22b: writeln(output,'SITGO');
				    23b: writeln(output,'G-float FORTRAN');
				    24b: writeln(output,'PASCAL');
				    25b: writeln(output,'JOVIAL');
				    others: writeln(output,' Unknown1')
				end;
				writeln(output,'Length of blank common: ',
					   word.blank_common)
			    end
		    end;

		7b:
		    begin
			writeln(output,'Start,,',word.second_half:6:o);
			count:=word.second_half;
			get(input); {relocation word}
			word_count:=word_count+1;
			get(input);
			word_count:=word_count+1;
			writeln(output,'Start address: ',input^:12:o);
			if count = 2 then
			    begin
				get(input);
				word_count:=word_count+1;
				word.full_word:=input^;
				write(output,'Code: ',word.code:3:o,
				     'Symbol: ');
				rad50(word.rest);
				writeln(output)
			    end
		    end;

		10b:
		    begin
			writeln(output,'Internal request,,',
				   word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		11b:
		    begin
			writeln(output,'Polish,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		12b:
		    begin
			writeln(output,'Chain,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		14b:
		    begin
			writeln(output,'Index,,',word.second_half:6:o);
			skip(word.second_half)
		    end;

		15b:
		    begin
			writeln(output,'ALGOL,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		16b,17b:
		    begin
			if word.first_half = 16b then
			    write(output,'Request load')
			else
			    write(output,'Request library');	
			writeln(output,',,',word.second_half:6:o);
			count:=word.second_half;
			get(input); {relocation count}
			word_count:=word_count+1;
			while count <> 0 do
			    begin
				get(input);
				word_count:=word_count+1;
				from6(input^, filename);
				get(input);
				word_count:=word_count+1;
				word.full_word:=input^;
				get(input);
				word_count:=word_count+1;
				from6(input^, device);
				writeln(output, 'Device: ', device,
					   ' Filename: ',filename,' ',
					   word.first_half:6:o,',,',
					   word.second_half:6:o);
				count:=count-3
			    end
		    end;

		20b:
		    begin
			writeln(output,'Common,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		21b:
		    begin
			writeln(output,'Sparse data,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		22b:
		    begin
			writeln(output,'PSECT origin,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		23b:
		    begin
			writeln(output,'PSECT End block,,',
				   word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		24b:
		    begin
			writeln(output,'PSECT Header block,,',
				   word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		37b:
		    begin
			writeln(output,'COBOL Symbols,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		100b:
		    begin
			writeln(output,'.ASSIGN,,',word.second_half:6:o);
			skip(word.second_half+1)
		    end;

		776b:
		    begin
			writeln(output,'Symbol File,,',word.second_half:6:o);
			skip(word.second_half)
		    end;

		777b:
		    begin
			writeln(output,'Universal file,,',
				   word.second_half:6:o);
			skip(word.second_half)
		    end;

		1000b:
		    begin
			writeln(output,'1000 -- Ignored,,',
				   word.second_half:6:o);
			skip(word.second_half)
		    end;

		1001b:
		    begin
			writeln(output,'1001 Entry,,',word.second_half:6:o);
			skip(word.second_half)
		    end;

		1002b:
		    begin
			writeln(output,'1002 Long entry,,',
				   word.second_half:6:o);
			skip(word.second_half)
		    end;

		1003b:
		    begin
			writeln(output,'Long Title,,',word.second_half:6:o);
			skip(word.second_half)
		    end;

		1004b:
		    begin
			writeln(output,'Byte Init.,,',word.second_half:6:o);
			skip(word.second_half)
		    end;

		1120b,1121b,1122b,1123b,1124b,1125b,1126b,1127b:
		    begin
			writeln(tty,'Arg. Desc. Block,,',
				   word.second_half:6:o);
			skip(word.second_half)
		    end;

		1130b:
		    begin
			writeln(output,'Coercion Block,,',
				   word.second_half:6:o);
			skip(word.second_half)
		    end;

		others:
		    begin
			if word.first_half > 3777b then	{asciz}
			    begin
				write(output,'ASCIZ: ');
				i:=1;
				while word.asciz[i] <> 0 do
				    begin
					write(output,chr(word.asciz[i]));
					i:=i+1;
					if i > 5 then
					    begin
						get(input);
						word_count:=word_count+1;
						word.full_word:=input^;
						i:=1
					    end
				    end;
				writeln(output)
			    end
			else
			    begin
				writeln(output,'?Error -- unexpected word in ',
				    'relocatable code (',word_count:8:o,'): ',
				    word.first_half:6:o,',,',
				    word.second_half:6:o);
				quit
			    end
		    end
	    end; {case}
	    writeln(output);
	    get(input);
	    word_count:=word_count+1
	end; {while}
    writeln(output,'Done.')
end.
