{									   }
{		@	 @     @    @@@@    @@@@@			   }
{		@	 @     @   @    @     @				   }
{		@	 @     @  @	      @				   }
{		@	 @     @  @	      @				   }
{		@	 @     @  @   @@@     @		A game.		   }
{		@	  @   @    @    @     @				   }
{		@@@@@@@    @@@      @@@@    @@@@@			   }
{									   }
{									   }
{This is an exploration game based loosely on the original Adventure, which}
{is based loosely on Dungeons and Dragons, which is based loosely on Sword }
{and Sorcery fiction, which is based loosely on various legends of the     }
{past, which are based loosely on actual history.  The player tries to     }
{explore an embassy by giving pseudo-English commands.  He has limited     }
{terminal time in which to leave the embassy.  The map of the inside of the}
{embassy is randomly generated at the beginning of the game.		   }
{									   }
{**** This version was written for the Hedrick Pascal-20 at Stanford LOTS, }
{a DECsystem-2060.  The character set is ASCII, of course.  It allows set  }
{sizes up to 0..71.  Any parts of this program that may need attention in  }
{transportation are marked with **** in a comment.			   }
{									   }
{**** To generate a new LUGI.EXE for public use on SU-LOTS:		   }
{     [0)  ensure that LUGI-INIT.TXT is in the same directory as this file]}
{	1)  CD ACCT:<GAMES>	!or whatever directory they're in!	   }
{	2)  PAS LUGI /NODEBUG /NOCHECK /NOLINK	!to compile it!		   }
{	3)  EXECUTE LUGI /NOSYMBOLS	!it will link and start, and    !  }
{					!spit out some messages, lastly !  }
{					!"Initialization has succeeded."!  }
{	4)  DELETE LUGI.REL,	!clean up.  it will pompt with @@!	   }
{	5)   EXPUNGE		!say this to the @@!			   }
{	6)			!enter an empty line to @@!		   }
{	7)  CD SX:<GAMES>						   }
{	8)  SAVE							   }
{	---------------------------------------------------------------    }
{     [Do the remaining steps if there is no LUGI-HIGH-SCORES.TXT here]    }
{	9)  CREATE LUGI-HIGH-SCORES.TXT;P776060				   }
{     [10) use editor to put twelve identical lines into the file. Each is:}
{		___0____________0_(nobody)				   }
{	    ....where _ represents a blank.]				   }

{**** Watch out for "Chr(7)", which we use to mean "bell", if your machine is non-ASCII}
{**** The procedure UpdateTopTenAtStanfordLOTS is unportable, but a similar}
{procedure may be useful at many other sites.  If no such is used, flush   }
{the file variable (hiscores).						   }


{someday: make acetone & fluid true containers;
	  reduce the excess (Take out xxx) for failing eat, open, etc.;
	  enable "take x x x x out" and "open|shut car|trunk|cabinet"}




PROGRAM Lugi;  {**** parameters are left out so that all files are unopened at start}




CONST
    copyright = ' (c) 1980, 1981 by Jay Wilson and Paul Kienitz ';  {will still exist in machine code}
    wizardline = 'Mail yer gripes and comments to HSS@SRI-NIC.';  {****}
    initfilespec = 'LUGI-INIT.TXT  !You have to connect! ';  {**** filespec for opening (init) for initialization}
    mestextsize = 30000;		{size of (messagetext)}
    weightlimit = 9;			{maximum weight player can carry}
    maxwordlen = 15;			{number of chars a dictionary word can have}
    dictsize = 199;			{size of (dictionary) hash table - 1}
    shortgametime = 540;		{how many seconds a short game can last}
    timeloss = 100;			{amount of time to lose with a mistake}
    gleeplimit = 15;			{how many gleeps fit in pockets}
    gleepclumping = 35;			{higher number causes greater tendency for new gleeps to appear with old ones}
(*  enforcefreq = 6;			{**** EnforceGame... is called with every (enforcefreq)'th Situation call} *)




TYPE
    letter = 'a'..'z';
    word = RECORD
	       w: PACKED ARRAY[1..maxwordlen] OF letter;
	       l: 0..maxwordlen
	       END;
    place = (nowhere,r1,r2,r3,pcabinet,r5,r6,r7,pniche,r9,r10,r11,  {player can only be in r1..pbalcony;}
	     pbunker,r13,r14,r15,pbuzzcr,r17,r18,r19,pgarden,r21,   {.... r<n> places have random appearences}
	     r22,r23,pcar,r25,r26,r27,ppillpile,r29,r30,r31,pcistern,
	     r33,r34,pbalcony,thronecage,throneshelf,incabinet,
	     intrunk,inbag,inflask,pockets,imaginaryplace);
    meaning = (nomeaning,
	       orope,ohammer,onitro,okeys,oflask,owater,ochemicals,ourine,
	       omatches,ofluid,ogum,oacetone,oskull,osandwich,obag,olysol,
	       oflysol,oraid,ostatuette,opills,
	       fstove,fnewspapers,fbooks,ftortstuff,fbadge,fpaintings,
	       fscalything,fgorilla,fdevice,fvermin,fsign,flizards,
	       fcircuitry,fguard,frunt,fcar,fplants,fghost,fmachine,fdirt,
	       fbones,flaundry,frats,fgags,fbirds,ftar,fagents,fwumpus,
	       fbagels,fboxes,fgnomes,fbattleship,fflies,fbeast,
	       ffungus,fcrystal,fcentrifuge,
	       vnorth,vsouth,veast,vwest,vjump,vclimb,vforward,vbackward,
	       vtake,vput,vdrop,vthrow,vinventory,vignite,vkill,vopen,
	       veat,venter,vout,vhelp,vread,vshut,vspit,vrun,vpour,
	       vbutton,vspray,vshit,vpiss,vfart,vfuck,vflush,vpeek,vplug,
	       vunplug,vstart,vfind,vlook,vbust,vquit,vdetonate,
	       mr,min,mit,mall,mgleeps,mnot);
    appearence = (arcturus,fogarc,fogged,burntplants,ghostbunker,scaredghost,
		  cleanbathroom,nasawreck,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,
		  balcony,seep,green,sooty,bathroom,library,plainwhite,bunker,
		  cabinet,bstudy,alcove,buzzcr,niche,rat,laundry,n356,office,
		  caf,birdcage,shark,operate,soggybox,car,cistern,sea,joke,
		  ghost,bubble,pillpile,cube,carvedfloor,picturetube,circuits,
		  airlock,lizardhole,nasa,device,tank,torture,kerosene,vent,
		  centralcr,machine,garden,drydock,freebirds,tar,archive,
		  peekhole,mensroom,stuffed,lobby,meadow,wumpus,rotate,
		  brightpoint,indesc,rec,largex,largey,waxarmy,meat,fisk,disk,
		  pancreas,columns,mirrorcube,arf,bagels,conwaylife,bester,
		  mtking,brick,streetlight,glowtube,pipe);  {**** lobby and fisk are meaningless outside SU-LOTS}
    roomarray = ARRAY[r1..pbalcony] OF RECORD
					   features: appearence;
					   paths: ARRAY[vnorth..vclimb] OF nowhere..pbalcony;
					   gleepses: integer;
					   explored: boolean
					   END;
    dictentry = RECORD
		    nextde: ^dictentry;
		    dword: word;
		    dmeaning: meaning
		    END;
    indexptr = RECORD  {pointer to a chunk of text in (messagetext)}
		   startpos,endpos: 1..mestextsize
		   END;
    converter = RECORD  {used to convert integers into places, meanings, and appearences}
		    CASE integer OF
			0: (cvi: integer);
			1: (cvp: place);
			2: (cvm: meaning);
			3: (cva: appearence)
			END;




VAR
    guardalive,goralive,		{guard is alive / gorilla is alive}
	attached,fungus,		{scaly thing is on leg / fungus is on arm}
	glued,strong,			{keys are glued down / player has super strength}
	burnt,clean,			{plants are burnt / bathroom is clean}
	chemgiven,overdose,		{the niche has given chemicals once / player has OD'ed on pills}
	peeked,tripped,			{player has just peeked at mating Lugimen / player is in (kerosene) with an object}
	shelffull,plugged,		{throne room shelf has something on it / centrifuge was just plugged in last turn}
	guardsees,plantslunged,		{the guard is about to shoot the player / man-eating plants are about to eat player}
	cabinetopen,trunkopen,		{cabinet is open / car trunk is open}
	acetoneopen,bystanders,		{acetone can is open / innocent bystanders have been killed}
	beastfollowing,badsandwich,	{large ugly beast is following player around / sandwich is infected}
	wipeout,tellsitu,		{player has wiped out city with plague / this command will take a turn}
	runtkilled,ready: boolean;	{allergic Lugiman has been killed / * game is initialized.  (See comment in Main.)}
    duration: (short,medium,long);	{time allowed for game}
    lastwarning: (nosight,ontrail,
		  detected,cries);	{how close pursuers have caught up}
    sickness: (none,mild,
	       severe,critical);	{how sick the player is from the Lugonian Plague}
(*  wayended: (notdone,pty,dialin,	{**** pty, dialin, queue, load, and daytime should be flushed outside SU-LOTS}
	       queue,load,daytime,
	       escaped,eaten,jumped,
	       killed,chicken,crash);	{how the game ended ( = notdone if not over yet)} *)
    wayended: (notdone,escaped,eaten,
	       jumped,killed,chicken,
	       crash);
    lastmove: vnorth..vclimb;		{direction of most recent move}
    verb,it,m: meaning;			{current verb / what "it" means / index}
    env,lastenv: appearence;		{player's surroundings / what (env) was at end of last turn}
    yourroom,dumproom,			{room player is in / room acetone was just dumped in}
	thingroom,gorroom,		{room scaly thing is in / room gorilla is in}
	guardroom,runtroom:		{room guard is in / room allergic Lugiman is in}
	nowhere..pbalcony;
    gleeps,starttime,			{number of gleeps in pockets / time embassy was entered}
	venttime,dumptime,		{time vent was entered / time acetone was dumped}
	sandtime,pilltime,		{time sandwich taken (or plague worsened) / time pills swallowed}
	fogtime,thrfogtime,		{time embassy fogged / time throne room fogged}
	baggleeps,flies,		{number of gleeps in bag / number of flies around player}
	invdepth,beenin,		{depth of Inventory nesting / number of rooms explored}
	heapbound: integer;		{* **** bound of space New'd in initialization}
    messagetext: PACKED ARRAY[1..mestextsize] OF char;  {* assorted text from init file}
    map: roomarray;					{array of room descriptions:
							   (.features) tells what it looks like
							   (.paths) tells connections to other rooms
							   (.explored) tells whether entered before}
    lastword: word;					{last word parsed from player's command}
    location: ARRAY[orope..opills] OF place;		{positions of objects orope..opills}
    dictionary: ARRAY[0..dictsize] OF ^dictentry;	{* relates written words to their meanings}
    bestword: ARRAY[orope..fcentrifuge] OF word;	{* best word to name each object}
    had: ARRAY[orope..opills] OF boolean;		{pseudo-set of those objects once posessed}
    dupcatch: ARRAY[meaning] OF boolean;		{pseudo-set of meanings found in present command so far}
    intro: indexptr;					{* pointer to introduction}
    obindex: ARRAY[orope..opills] OF indexptr;		{* index for incomplete object messages like "There's a coil of rope"}
    furnindex: ARRAY[fstove..fcentrifuge] OF indexptr;	{* index for furniture excuses}
    roomindex: ARRAY[appearence] OF indexptr;		{* index for room descriptions}
    roomits: ARRAY[appearence] OF meaning;		{* default value of (it) for each room variety}
    init,hiscores: text;				{initialization data / top ten scores}
(*  enforcecount,jobnumber: integer;			{**** used by EnforceGame... --flush outside of SU-LOTS} *)
    {(* in comment marks variable treated as constant after initialization)}







{PROCEDURE Rclose(VAR f: <any file>);
      predefined-- closes file (f).}


{FUNCTION Random(dummy: real): real;
      predefined-- returns a flatly distributed random
      real number >= 0.0 and < 1.0  --(dummy) is ignored.}




PROCEDURE SetRan(seed: integer);  {**** predefined at SU-LOTS}
{This procedure gives an initial seed to the Random function.}
    EXTERN FORTRAN;




PROCEDURE LoadMessages;
{This procedure is never called in a normal run; only in the very first     }
{one. (See comment at top of Main.)  It reads in various messages: the room }
{descriptions, the introduction, the object partial-descriptions, and the   }
{furniture excuses.  They are loaded into (messagetext) and referenced later}
{with various indexes.							    }
    VAR
	nextempty,i,j: integer;  {these are all indexes}
	ob: meaning;
	ap: appearence;
    BEGIN  {LoadMessages}
    Writeln;
    ready := true;
    nextempty := 0;
    FOR ap := arcturus TO pipe DO BEGIN  {read room descriptions}
	roomindex[ap].startpos := nextempty + 1;
	WHILE NOT (init^ = '\') AND ready DO BEGIN
	    nextempty := nextempty + 1;
	    messagetext[nextempty] := init^;
	    Get(init);
	    ready := nextempty < mestextsize;
	    IF NOT ready THEN
		Writeln('## messagetext bound exceeded in room descriptions.')
	    END;
	roomindex[ap].endpos := nextempty;
	Get(init)
	END;
    IF ready THEN
	Writeln('(Room descriptions read.)');
    intro.startpos := nextempty + 1;  {read 'detailed briefing'}
    WHILE (init^ <> '\') AND ready DO BEGIN
	nextempty := nextempty + 1;
	messagetext[nextempty] := init^;
	Get(init);
	ready := nextempty < mestextsize;
	IF NOT ready THEN
	    Writeln('## messagetext bound exceeded in briefing.')
	END;
    intro.endpos := nextempty;
    Get(init);
    IF ready THEN
	IF intro.endpos - intro.startpos < 1000 THEN BEGIN
	    Writeln('## Wrong number of room descriptions?  Briefing too short.');
	    ready := false
	    END
	ELSE
	    Writeln('(Briefing read.)');
    Readln(init);
    FOR ob := orope TO opills DO BEGIN  {read object descriptions}
	obindex[ob].startpos := nextempty + 1;
	WHILE NOT Eoln(init) AND ready DO BEGIN
	    nextempty := nextempty + 1;
	    messagetext[nextempty] := init^;
	    Get(init);
	    ready := nextempty < mestextsize;
	    IF NOT ready THEN
		Writeln('## messagetext bound exceeded in object descriptions.')
	    END;
	obindex[ob].endpos := nextempty;
	Readln(init)
	END;
    IF ready THEN
	IF Eoln(init) THEN
	    Writeln('(Object descriptions read.)')
	ELSE BEGIN
	    Writeln('## Wrong number of object descriptions.');
	    ready := false
	    END;
    Readln(init);
    FOR ob := fstove TO fcentrifuge DO BEGIN  {read furniture excuses}
	furnindex[ob].startpos := nextempty + 1;
	WHILE NOT Eoln(init) AND ready DO BEGIN
	    nextempty := nextempty + 1;
	    messagetext[nextempty] := init^;
	    Get(init);
	    ready := nextempty < mestextsize;
	    IF NOT ready THEN
		writeln('## messagetext bound exceeded in furniture excuses.')
	    END;
	furnindex[ob].endpos := nextempty;
	Readln(init)
	END;
    IF ready THEN
	IF Eoln(init) THEN BEGIN
	    Writeln('(Furniture excuses read.)');
	    Writeln('  >>> ',nextempty: 1,' chars used of possible ',mestextsize: 1,' in messagetext.')
	    END
	ELSE BEGIN
	    Writeln('## Wrong number of excuses.');
	    ready := false
	    END;
    Readln(init)
    END;  {LoadMessages}




PROCEDURE MakeDictionary;
{This reads the dictionary words from (init) and assigns default values}
{for (it) in each room.						       }
    VAR
	longestword,empties,longestlist,words,h,i,j: integer;  {four statistics / indexes}
	ap: appearence;
	m: meaning;
	newword: word;
	best,rightspot: boolean;
	p,q: ^dictentry;
    BEGIN  {MakeDictionary}
    FOR i := 0 TO dictsize DO
	dictionary[i] := NIL;
    longestword := 0;
    empties := dictsize + 1;
    longestlist := 0;
    words := 0;
    FOR m := orope TO mnot DO
	IF ready AND NOT Eof(init) THEN BEGIN
	    REPEAT
		best := init^ = '*';
		IF best THEN
		    Get(init);
		newword.l := 0;
		WHILE (newword.l < maxwordlen) AND (init^ > ' ') DO BEGIN
		    newword.l := newword.l + 1;
		    newword.w[newword.l] := init^;
		    Get(init)
		    END;
		WHILE ((init^ < 'a') OR (init^ > 'z')) AND (init^ <> '*') AND NOT Eoln(init) DO
		    Get(init);
		words := words + 1;
		IF newword.l = 1 THEN
		    h := Ord(newword.w[1]) MOD (dictsize + 1)
		ELSE
		    h := ((Ord(newword.w[1]) - Ord('a')) * 26 + Ord(newword.w[2])) MOD (dictsize + 1);
		IF newword.l > longestword THEN
		    longestword := newword.l;
		j := 1;
		p := dictionary[h];
		q := NIL;
		REPEAT
		    IF p <> NIL THEN BEGIN
			i := 1;
			WHILE (p^.dword.w[i] = newword.w[i]) AND (i < newword.l) AND (i < p^.dword.l) DO
			    i := i + 1;
			rightspot := p^.dword.w[i] > newword.w[i]
			END
		    ELSE
			rightspot := true;
		    IF NOT rightspot THEN BEGIN
			j := j + 1;
			q := p;
			p := p^.nextde
			END
		UNTIL rightspot;
		IF j > longestlist THEN
		    longestlist := j;
		IF q = NIL THEN BEGIN
		    IF p = NIL THEN
			empties := empties - 1;
		    New(dictionary[h]);
		    dictionary[h]^.dword := newword;
		    dictionary[h]^.dmeaning := m;
		    dictionary[h]^.nextde := p
		    END
		ELSE BEGIN
		    New(q^.nextde);
		    q^.nextde^.dword := newword;
		    q^.nextde^.dmeaning := m;
		    q^.nextde^.nextde := p
		    END;
		IF best THEN
		    bestword[m] := newword;
	    UNTIL Eoln(init);
	    Readln(init)
	    END
	ELSE
	    IF ready THEN BEGIN
		Writeln('## Not enough words.');
		ready := false
		END;
    WHILE NOT Eof(init) AND (init^ <= ' ') DO
	Get(init);
    IF NOT Eof(init) THEN BEGIN
	Writeln('## Too many words.');
	ready := false
	END;
    IF ready THEN BEGIN
	Writeln;
	Writeln('(Dictionary created.)');
	FOR ap := arcturus TO pipe DO
	    roomits[ap] := nomeaning;
	roomits[ghostbunker] := oskull;
	roomits[library] := fbooks;
	roomits[bunker] := oskull;
	roomits[laundry] := flaundry;
	roomits[office] := fnewspapers;
	roomits[caf] := fbones;
	roomits[cistern] := owater;
	roomits[joke] := fgags;
	roomits[pillpile] := opills;
	roomits[device] := fdevice;
	roomits[tank] := owater;
	roomits[torture] := fbadge;
	roomits[garden] := fplants;
	roomits[tar] := ftar;
	roomits[lobby] := fboxes;
	Writeln('(Default it-values assigned.)'); Writeln;
	Writeln('  >>> The longest word had ',longestword: 1,' characters.  The hash table');
	Writeln('has ',empties: 1,' of ',dictsize + 1: 1,' addresses unused, and the longest list');
	Writeln('has ',longestlist: 1,' words.  There are a total of ',words: 1,' words.'); Writeln;
	Writeln('Initialization has succeeded.')
	END
    ELSE
	Writeln('##### Initialization has failed. #####')
    END;  {MakeDictionary}




FUNCTION Timer: integer;
{Returns a number of seconds since some random time in the past.}
{In other words, increases by one each second.			}
    BEGIN
    Timer := Time DIV 1000  {**** Time returns the number of milliseconds since the last system start}
    END;  {Timer}




FUNCTION RandInt(range: integer): integer;
{This function returns a random integer in 1..(range).}
    BEGIN
    RandInt := Trunc(Random(0.0) * range) + 1
    END;  {RandInt}




FUNCTION RandDirection(drange: meaning): meaning;
{Returns a random meaning in vnorth..(drange).}
    VAR
	x: converter;
    BEGIN
    x.cvi := Trunc(Random(0.0) * (1 + Ord(drange) - Ord(vnorth))) + Ord(vnorth);
    RandDirection := x.cvm
    END;  {RandDirection}




FUNCTION Distant(avoid: place): place;
{This function returns a random room (place in r1..pbalcony) which is not}
{adjacent to (avoid).  Use Distant(nowhere) to get any random room.	 }
    VAR
	x: converter;
    BEGIN
    REPEAT
	x.cvi := Trunc(Random(0.0) * Ord(pbalcony)) + 1
    UNTIL (Abs(x.cvi - Ord(avoid)) >= 2) AND (Abs(x.cvi - Ord(avoid)) <= Ord(pbalcony) - 1) OR (avoid = nowhere);
    Distant := x.cvp
    END;  {Distant}




FUNCTION Opposite(d: meaning): meaning;
{This function returns the directional opposite of (d), where (d)}
{is in vnorth..vclimb.  i.e. Opposite(veast) = vwest.		 }
    BEGIN
    IF Odd(Ord(d) - Ord(vnorth)) THEN
	Opposite := Pred(d)
    ELSE
	Opposite := Succ(d)
    END;  {Opposite}




PROCEDURE SetUp;
{This procedure sets up the beginning of the game.  It initializes a bunch}
{of variables, and makes the map of the embassy.			  }
    VAR
	ob: meaning;


    PROCEDURE MakeMap;  {inside SetUp}
    {This procedure initializes the variables (map), (location),     }
    {(yourroom), (guardroom), (thingroom), (runtroom), and (gorroom).}
    {In other words, it gives each room (in r1..pbalcony) a randomly }
    {selected but unique appearence,  connects these rooms to each   }
    {other, places the objects, the player, the guard, the scaly     }
    {thing, the allergic Lugiman, and the gorilla in random places,  }
    {meanwhile preventing various bad combinations and making sure   }
    {that certain rooms and objects are always in certain places.    }
	CONST
	    gleepdensity = 4;  {controls the likelyhood of a room getting a gleep}
	    roomdensity = 25;  {controls the probable number of exits from each room}
	VAR
	    otherend,roomct,nextroom: place;  {where a path goes / index / room after (roomct)}
	    m: meaning;
	    i: integer;
	    ap: appearence;
	    x: converter;
	    rooms: ARRAY[appearence] OF boolean;  {**** pseudo-set of rooms used so far}
	BEGIN
	FOR ap := arcturus TO pipe DO  {**** this would be faster with a set, if it were possible}
	    rooms[ap] := false;
	rooms[bunker] := true;  {these several rooms will be added later}
	rooms[cabinet] := true;
	rooms[buzzcr] := true;
	rooms[niche] := true;
	rooms[car] := true;
	rooms[cistern] := true;
	rooms[pillpile] := true;
	rooms[balcony] := true;
	rooms[garden] := true;
	rooms[arcturus] := true;  {these don't exist at first}
	rooms[fogarc] := true;
	rooms[fogged] := true;
	rooms[burntplants] := true;
	rooms[ghostbunker] := true;
	rooms[scaredghost] := true;
	rooms[cleanbathroom] := true;
	rooms[nasawreck] := true;
	FOR roomct := r1 TO pbalcony DO
	    FOR m := vnorth TO vclimb DO
		map[roomct].paths[m] := nowhere;
	FOR roomct := r1 TO pbalcony DO
	    WITH map[roomct] DO BEGIN
		IF roomct = pbalcony THEN
		    nextroom := r1
		ELSE
		    nextroom := Succ(roomct);
		REPEAT
		    x.cvi := RandInt(Ord(pipe))
		UNTIL NOT rooms[x.cva];
		rooms[x.cva] := true;
		features := x.cva;
		gleepses := Ord((RandInt(gleepdensity) = 1) AND (features <> bunker) AND (features <> ghost));
		REPEAT  {connect this room both ways to the next}
		    m := RandDirection(vwest)
		UNTIL (paths[m] = nowhere) AND (map[nextroom].paths[Opposite(m)] = nowhere);
		paths[m] := nextroom;
		map[nextroom].paths[Opposite(m)] := roomct;
		explored := false
		END;
	FOR roomct := r1 TO pbalcony DO
	    WITH map[roomct] DO BEGIN
		FOR i := 1 TO roomdensity DO  {put in some other two-way connections at random}
		    IF Randint(10) = 1 THEN BEGIN  {effectively divides roomdensity by 10}
			m := RandDirection(vclimb);
			otherend := Distant(roomct);
			IF (paths[m] = nowhere) AND (map[otherend].paths[Opposite(m)] = nowhere) THEN BEGIN
			    paths[m] := otherend;
			    map[otherend].paths[Opposite(m)] := roomct
			    END
			END;
		paths[RandDirection(vclimb)] := Distant(roomct)  {put in a one-way connection}
		END;
	map[pcabinet].features := cabinet;
	map[pcar].features := car;
	map[pniche].features := niche;
	map[pbunker].features := bunker;
	map[pcistern].features := cistern;
	map[pbuzzcr].features := buzzcr;
	map[pgarden].features := garden;
	map[ppillpile].features := pillpile;
	map[pbalcony].features := balcony;
	FOR roomct := r1 TO pbalcony DO BEGIN  {prevent embarassing holes in floor, etc.}
	    IF map[roomct].features IN [cistern,balcony,tank,meadow] THEN
		map[roomct].paths[vjump] := nowhere;
	    IF map[roomct].features = meadow THEN
		map[roomct].paths[vclimb] := nowhere;
	    IF (map[roomct].features = lobby) OR (map[roomct].features = drydock) THEN
		map[roomct].paths[vclimb] := Distant(roomct);
	    IF (map[roomct].features = ghost) THEN
		FOR m := vnorth TO vclimb DO
		    map[roomct].paths[m] := nowhere
	    END;
	REPEAT
	    yourroom := Distant(nowhere)
	UNTIL (map[yourroom].features <> ghost) AND (yourroom <> pbalcony);
	env := map[yourroom].features;
	REPEAT
	    guardroom := Distant(nowhere)
	UNTIL NOT (map[guardroom].features IN [ghost,balcony,meadow]);
	REPEAT
	    runtroom := Distant(nowhere)
	UNTIL NOT (map[runtroom].features IN [ghost,balcony,meadow]);
	IF RandInt(2) = 1 THEN
	    REPEAT
		gorroom := Distant(nowhere)
	    UNTIL NOT (map[gorroom].features IN [cistern,ghost,balcony,tank])
	ELSE
	    gorroom := nowhere;
	IF RandInt(2) = 1 THEN
	    thingroom := Distant(nowhere)
	ELSE
	    thingroom := nowhere;
	IF gorroom > nowhere THEN  {make hole in floor of gorilla room}
	    map[gorroom].paths[vjump] := Distant(gorroom);
	baggleeps := RandInt(3) + 1;
	FOR m := orope TO opills DO  {place objects:}
	    CASE m OF
		orope,ohammer,okeys,omatches,ogum,oacetone,osandwich,obag: BEGIN  {place most of them randomly}
		    REPEAT
			roomct := Distant(nowhere)
		    UNTIL NOT (map[roomct].features IN [cistern,ghost,tank]) 
					AND ((m <> osandwich) OR (roomct <> gorroom)) AND ((m <> ogum) OR (roomct <> thingroom));
		    location[m] := roomct
		    END;
		onitro: location[m] := intrunk;
		oflask: location[m] := pniche;
		owater,ochemicals,ourine,ofluid,oskull,opills: location[m] := nowhere;
		olysol,oflysol,oraid: location[m] := incabinet;
		ostatuette: location[m] := throneshelf
		END
	END;  {MakeMap}


    BEGIN  {SetUp}
    starttime := 0;  {necessary for Timer to work}
    burnt := false;
    fungus := false;
    clean := false;
    attached := false;
    cabinetopen := false;
    peeked := false;
    strong := false;
    overdose := false;
    trunkopen := false;
    acetoneopen := false;
    bystanders := false;
    wipeout := false;
    beastfollowing := false;
    plantslunged := false;
    guardsees := false;
    plugged := false;
    chemgiven := false;
    runtkilled := false;
    shelffull := true;
    goralive := true;
    guardalive := true;
    badsandwich := true;
    glued := true;
    tellsitu := true;
    sandtime := maxint;
    dumptime := maxint;
    venttime := maxint;
    fogtime := maxint;
    pilltime := maxint;
    dumproom := nowhere;
    lastenv := fogged;
    lastmove := vnorth;
    lastwarning := nosight;
    sickness := none;
    wayended := notdone;
    flies := 0;
    gleeps := 0;
    beenin := 0;
    invdepth := 0;
(*  enforcecount := 0;  {**** flush outside SU-LOTS} *)
(*  jobnumber := 0;  {**** likewise} *)
    FOR ob := orope TO opills DO
	had[ob] := false;
    SetRan(Timer);
    MakeMap
    END;  {SetUp}




PROCEDURE ReadLetter(VAR c: letter);
{This procedure reads one letter from the player's terminal,	 }
{ignoring other characters.  It gives a prompt of three spaces   }
{whenever Eoln is true.  It changes the letter read to lowercase,}
{clears the buffer, and returns it as (c).			 }
    BEGIN
    WHILE (input^ < 'A') OR ((input^ > 'Z') AND (input^ < 'a')) OR (input^ > 'z') DO
	IF Eoln(input) THEN BEGIN
	    Write('   ');
	    Readln
	    END
	ELSE
	    Get(input);
    IF input^ < '_' THEN
	c := Chr(Ord(input^) + 32)
    ELSE
	c := input^;
    Writeln;
    WHILE NOT Eoln(input) DO
	Get(input)
    END;  {ReadLetter}




PROCEDURE Inform;
{This procedure introduces the player to the game, and asks him how much}
{time he wants.  It sets the variables (duration) and (wayended).	}
    VAR
	i: integer;
	c: letter;  {player's response to questions}
    BEGIN
    Writeln; Writeln; Writeln; Writeln;
    Writeln('This here is the game of LUGI, by Jay Wilson and Paul Kienitz.');
    Writeln(wizardline);
    REPEAT
	Writeln; Writeln;
	Writeln('You wanna detailed briefing, Charlie?');
	ReadLetter(c)
    UNTIL (c = 'y') OR (c = 'n');
    IF c = 'y' THEN
	FOR i := intro.startpos TO intro.endpos DO  {give briefing}
	    IF messagetext[i] = '#' THEN
		Write(Ord(pbalcony): 1)
	    ELSE IF messagetext[i] = '_' THEN
		Writeln
	    ELSE
		Write(messagetext[i]);
    REPEAT
	Writeln;
	Writeln('Will you accept the mission, and enter the embassy?');
	ReadLetter(c)
    UNTIL (c = 'y') OR (c = 'n');
    IF c = 'y' THEN BEGIN
	REPEAT
	    Writeln; Writeln;
	    Writeln('Short, medium, or long time period?');
	    ReadLetter(c)
	UNTIL (c = 'l') OR (c = 'm') OR (c = 's');
	IF c = 's' THEN
	    duration := short
	ELSE
	    IF c = 'm' THEN
		duration := medium
	    ELSE
		duration := long
	END
    ELSE
	wayended := chicken;
    IF wayended <> chicken THEN BEGIN
	Writeln('You are concealed in a load of steak and electronic parts and left in an');
	Writeln('abandoned building.  After a while, there are noises, and then all is quiet.');
	Writeln('You creep out of the crate, and through a small opening into another room...')
	END
    END;  {Inform}




FUNCTION Have(object: meaning; inside: boolean): boolean;
{This function returns true if the player has the object.  If (inside),}
{it returns true if (object) is inside another object the player has.  }
{Otherwise, the player must be holding it directly.		       }
    BEGIN
    IF (object >= orope) AND (object <= opills) THEN
	IF location[object] = pockets THEN
	    Have := true
	ELSE
	    IF inside AND (location[object] = inbag) THEN
		Have := Have(obag,true)
	    ELSE
		IF inside AND (location[object] = inflask) THEN
		    Have := Have(oflask,true)
		ELSE
		    Have := false
    ELSE
	Have := ((gleeps > 0) OR ((baggleeps > 0) AND Have(obag,true) AND inside)) AND (object = mgleeps)
    END;  {Have}




FUNCTION Here(object: meaning): boolean;
{Returns true if the object is inside the room.}
    VAR
	x: boolean;
    BEGIN
    x := Have(object,true);
    IF env <> fogged THEN
	IF (object >= orope) AND (object <= opills) THEN BEGIN
	    x := x OR (location[object] = yourroom) AND (env > fogarc);
	    x := x OR (object = oskull) AND ((env = ghostbunker) OR (env = bunker));
	    x := x OR (object = opills) AND (env = pillpile);
	    x := x OR (location[object] = incabinet) AND (env = cabinet) AND cabinetopen;
	    x := x OR (location[object] = intrunk) AND (env = car) AND trunkopen;
	    x := x OR (location[object] = thronecage) AND (env = arcturus);
	    x := x OR (object = owater) AND ((env = cistern) OR (env = tank) OR (env = seep));
	    x := x OR (location[object] = throneshelf) AND (env <= fogarc)
	    END
	ELSE
	    IF object = mgleeps THEN
		x := x OR (env > fogarc) AND (map[yourroom].gleepses > 0)
	    ELSE
		CASE object OF
		    fstove: x := env = meat;
		    fnewspapers: x := env = office;
		    fbooks: x := env = library;
		    ftortstuff,fbadge: x := env = torture;
		    fpaintings: x := env = carvedfloor;
		    fgorilla: x := yourroom = gorroom;
		    fscalything: x := attached;
		    fdevice: x := env = device;
		    fvermin,fdirt: x := env = pillpile;
		    fsign: x := env IN [sea,bubble,airlock,machine];
		    flizards: x := env = lizardhole;
		    fcircuitry: x := env = circuits;
		    fguard: x := yourroom = guardroom;
		    frunt: x := yourroom = runtroom;
		    fcar: x := env = car;
		    fplants: x := env = garden;
		    fghost: x := env = ghost;
		    fmachine: x := env = machine;
		    fbones: x := env = caf;
		    flaundry: x := env = laundry;
		    frats: x := env = rat;
		    fgags: x := env = joke;
		    fbirds: x := env = freebirds;
		    ftar: x := env = tar;
		    fagents: x := env = stuffed;
		    fwumpus,fbagels: x := false;
		    fboxes,fgnomes: x := env = lobby;
		    fbattleship: x := env = drydock;
		    fflies: x := flies > 0;
		    fbeast: x := (env = arcturus) OR beastfollowing;
		    ffungus: x := fungus;
		    fcrystal: x := env = archive;
		    fcentrifuge: x := env = nasa
		    END;
    Here := x
    END;  {Here}



(*
PROCEDURE EnforceGamePlayingRulesAtStanfordLOTS;
{**** This procedure uses some very un-standard stuff to enforce the}
{numerous rules restricting the playing of games at SU-LOTS.  Throw }
{it out without hesitation at all other sites.			    }
    CONST
	get_job_information = 11;
	get_acj_permission = 380;
	games_check_code = 131073;
    TYPE
	flagnames = (b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,
		     b14,b15,b16,b17,notokay,b19,b20,b21,b22,b23,
		     ptykick,ptywarn,generalkick,generalwarn,
		     daytimekick,daytimewarn,dialinkick,dialinwarn,
		     queuekick,queuewarn,loadkick,loadwarn);
	blk = RECORD
		  blklength: integer;  {size of this block in words (=2)}
		  CASE boolean OF
		      false: (flags: PACKED ARRAY[flagnames] OF boolean);
		      true: (flagsum: integer)
		      END;
	p_blk = ^blk;
    VAR
	p_p_blk: ^p_blk;
	garbage: integer;
    BEGIN
    IF jobnumber = 0 THEN
	Jsys(get_job_information;;garbage,garbage,jobnumber);  {gets job number}
    New(p_p_blk);
    New(p_p_blk^);
    p_p_blk^^.blklength := 2;
    p_p_blk^^.flagsum := 0;
    Jsys(get_acj_permission,-1;games_check_code,p_p_blk,1,jobnumber);  {asks Access Control Job for permission to play}
    WITH p_p_blk^^ DO
	IF flags[notokay] THEN
	    IF flags[generalkick] THEN
		IF flags[ptykick] THEN
		    wayended := pty
		ELSE
		    IF flags[dialinkick] THEN
			wayended := dialin
		    ELSE
			IF flags[queuekick] THEN
			    wayended := queue
			ELSE
			    IF flags[loadkick] THEN
				wayended := load
			    ELSE
				IF flags[daytimekick] THEN
				    wayended := daytime
				ELSE
				    wayended := crash
	    ELSE
		IF starttime = 0 THEN
		    Writeln('=== Warning: it''s not a good time to play games.')
		ELSE BEGIN
		    Writeln;
		    IF flags[ptywarn] THEN
			Writeln('Watch it, it''s not a good idea to explore with a PSEUDO-TERMINAL...');
		    IF flags[dialinwarn] THEN
			Writeln('Watch it, it''s not a good idea to explore with a DIAL-IN...');
		    IF flags[queuewarn] THEN BEGIN
			Writeln('There''s a nasty gnome in the room.  He mutters something about');
			Writeln('"the long CUE", but you see no pool table or related stuff...');
			Writeln('      (People are waiting for your terminal.)')
			END;
		    IF flags[loadwarn] THEN BEGIN
			Writeln('A tremendous LOAD suddenly weighs on your shoulders...  you may be forced');
			Writeln('to quit exploring if it gets worse...       ( try hitting Control-T )')
			END;
		    IF flags[daytimewarn] THEN BEGIN
			Writeln('For a moment, you have a vision of Father TIME, holding his scythe and');
			Writeln('intoning, "It is not a good time to explore the embassy!"')
			END;
		    IF flags[generalwarn] AND NOT (flags[ptywarn] OR flags[dialinwarn]
					OR flags[queuewarn] OR flags[loadwarn] OR flags[daytimewarn]) THEN
			Writeln(Chr(7),'Hey, they''ve changed the games-playing [warning] rules!  Lugi needs fixing!')
		    END
	ELSE
	    IF flagsum <> 0 THEN
		wayended := crash;
    IF wayended <> notdone THEN
	IF starttime = 0 THEN BEGIN
	    wayended := crash;
	    Writeln('You can''t play games now.')
	    END
	ELSE
	    CASE wayended OF
		pty: BEGIN
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('Sooo, you want to wxplore the embassy with a PSEUDO-TERMINAL, huh?');
		    Writeln('Well, TOUGH!')
		    END;
		dialin: BEGIN
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('Sorree, Charlie....  You can''t play Lugi on a DIAL-IN.')
		    END;
		queue: BEGIN
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('There''s a nasty little gnome in the room with you!  It yells, "Dammit, I''m');
		    Writeln('sick of waiting in the QUEUE!"  He knocks you flat and calls the Lugimen.')
		    END;
		load: BEGIN
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('A tremendous LOAD seems to weigh on your shoulders.  Everything slows down...');
		    Writeln('As a little gnome chuckles about the "load average", you collapse in defeat.')
		    END;
		daytime: BEGIN
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('Father TIME appears before you!  He intones, "It is not correct to explore');
		    Writeln('the embassy at this time!"  He swings his scythe, and cuts off your head.')
		    END;
		crash: Writeln(Chr(7),'OH NO!  They''ve changed the games-playing rules!  Lugi needs fixing!')
		END
    END;  {EnforceGamePlayingRulesAtStanfordLOTS}
*)



PROCEDURE Situation;
{This procedure describes the player's situation, and updates a few things.}
    VAR
	i: integer;


    PROCEDURE ShowRoom;  {inside Situation}
    {This procedure describes the room the player is in.}
	VAR
	    i: integer;

	PROCEDURE ShowWaysOut;  {inside ShowRoom, inside Situation}
	{This writes a line telling which of the six directions (n, s, e,}
	{w, j, c) will lead out of the room the player is in.		 }
	    VAR
		d: meaning;
	    BEGIN
	    Write('Open exits: ');
	    FOR d := vnorth TO vclimb DO
		IF map[yourroom].paths[d] > nowhere THEN
		    CASE d OF
			vnorth: Write(' N');
			vsouth: Write(' S');
			veast:  Write(' E');
			vwest:  Write(' W');
			vjump:  Write(' J');
			vclimb: Write(' C')
			END;
	    Writeln
	    END;  {ShowWaysOut}

	BEGIN  {ShowRoom}
	IF (env = balcony) AND (fogtime <= Timer) THEN BEGIN
	    Writeln('You are on a balcony over a busy street.  Thick brown fog is pouring out');
	    Writeln('of the room behind you and dissipating into the air.')
	    END
	ELSE
	    IF messagetext[roomindex[env].startpos] = '#' THEN
		Writeln('You are in a small, bare Lugonian cubicle, numbered ',Ord(yourroom) * 2 + 9: 2,'.')
	    ELSE
		IF (env = machine) AND (lastenv = machine) AND (verb <> vlook) THEN BEGIN
		    Writeln('You are in the bowels of a huge machine, etc.  The Lugiman is still pouring');
		    Writeln('enormous amounts of oil into the machine.')
		    END
		ELSE
		    FOR i := roomindex[env].startpos TO roomindex[env].endpos DO
			IF messagetext[i] = '_' THEN
			    Writeln
			ELSE
			    Write(messagetext[i]);
	IF env = nasa THEN
	    IF plugged THEN
		Writeln('The centrifuge is running wildly, about to tear itself apart!')
	    ELSE
		Writeln('It isn''t plugged in.  There are about a hundred gleeps inside the cage.');
	IF (roomits[env] > nomeaning) AND (lastenv <> env) THEN
	    it := roomits[env];
	IF NOT (env IN [arcturus..fogged,ghostbunker,balcony,ghost,bubble,kerosene,meadow]) THEN
	    ShowWaysOut
	END;  {ShowRoom}


    PROCEDURE ShowObjects;  {inside Situation}
    {Describes the objects in the player's room.}
	VAR
	    j: integer;
	    ob: meaning;
	    something,linefed: boolean;  {at least one object is in cabinet / initial linefeed has been written}
	BEGIN
	linefed := false;
	tripped := false;
	FOR ob := orope TO opills DO  {objects on the floor}
	    IF ((location[ob] = yourroom) AND (env > fogged)) OR ((location[ob] = thronecage) AND (env = arcturus)) THEN BEGIN
		IF NOT linefed AND NOT tripped THEN BEGIN
		    linefed := true;
		    Writeln
		    END;
		IF lastenv <> env THEN
		    it := ob;
		IF env = kerosene THEN BEGIN
		    IF NOT tripped THEN
			Writeln('Your foot hits against some object you can''t see.');
		    tripped := true
		    END
			ELSE BEGIN
			    FOR j := obindex[ob].startpos TO obindex[ob].endpos DO
				IF messagetext[j] = '_' THEN
				    Writeln
			        ELSE
				    Write(messagetext[j]);
			    IF (ob = osandwich) AND NOT badsandwich THEN
				Write(' (disinfected)');
			    IF (ob = okeys) AND glued THEN
				Writeln(' here, held in place by powerful glue.')
			    ELSE
				IF ((ob = omatches) OR (ob = osandwich)) AND ((env = cistern) OR (env = tank)) THEN
				    Writeln(' floating around here.')
				ELSE
				    Writeln(' here.')
			    END
		END;
	IF (env = cabinet) AND cabinetopen THEN BEGIN
	    something := false;
	    FOR ob := orope TO opills DO
		IF location[ob] = incabinet THEN BEGIN
		    IF lastenv <> env THEN
			it := ob;
		    IF NOT linefed THEN BEGIN
			linefed := true;
			Writeln
			END;
		    FOR j := obindex[ob].startpos TO obindex[ob].endpos DO
			IF messagetext[j] = '_' THEN
			    Writeln
		        ELSE
			    Write(messagetext[j]);
		    IF (ob = osandwich) AND NOT badsandwich THEN
			Write(' (disinfected)');
		    Writeln(' in the cabinet.');
		    something := true
		    END;
	    IF NOT something THEN
		Writeln('The cabinet is empty.')
	    END;
	IF (env = car) AND trunkopen THEN BEGIN
	    something := false;
	    FOR ob := orope TO opills DO
		IF location[ob] = intrunk THEN BEGIN
		    IF lastenv <> env THEN
			it := ob;
		    IF NOT linefed THEN BEGIN
			linefed := true;
			Writeln
			END;
		    FOR j := obindex[ob].startpos TO obindex[ob].endpos DO
			IF messagetext[j] = '_' THEN
			    Writeln
		        ELSE
			    Write(messagetext[j]);
		    Writeln(' in the car''s trunk.');
		    something := true
		    END;
	    IF NOT something THEN
		Writeln('The trunk is empty.')
	    END;
	IF (env <= fogarc) AND shelffull THEN BEGIN
	    IF NOT linefed THEN BEGIN
		linefed := true;
		Writeln
		END;
	    ob := okeys;
	    WHILE location[ob] <> throneshelf DO  {only one object fits here}
		ob := Succ(ob);
	    CASE ob OF
		okeys: Writeln('Some keys are on the shelf.');
		oflask: Writeln('There''s a small flask on the shelf.');
		omatches: Writeln('There''s a book of matches on the little shelf.');
		ofluid: Writeln('A bottle of sweet-smelling fluid is on the small shelf.');
		ogum: Writeln('The little shelf is holding a stick of gum.');
		ostatuette: Writeln('There is a small statuette on the shelf.  Perhaps it is religious in nature.');
		opills: Writeln('There''s a small handful of pills and capsules on the shelf.')
		END;
	    IF lastenv <> env THEN
		it := ob
	    END;
	IF (env > fogged) AND (map[yourroom].gleepses > 0) THEN BEGIN
	    IF lastenv <> env THEN
		it := mgleeps;
	    IF NOT linefed THEN
		Writeln;
	    IF env <> kerosene THEN
		IF Have(oskull,false) THEN BEGIN
		    Writeln('POP!  There were some gleeps here when you came in, but they vanished.');
		    map[yourroom].gleepses := 0
		    END
		ELSE
		    IF map[yourroom].gleepses = 1 THEN
			Writeln('There is a gleep in this place.')
		    ELSE
			Writeln('There are ',map[yourroom].gleepses: 1,' gleeps in this place.')
	    ELSE
		IF NOT tripped THEN BEGIN
		    Writeln('Your foot hits against some object you can''t see.');
		    tripped := true
		    END
	    END
	END;  {ShowObjects}


    BEGIN  {Situation}
    Writeln; Writeln; Writeln;
    ShowRoom;
    IF (env = arcturus) AND NOT beastfollowing THEN
	Writeln('There''s a huge doglike ugly alien beast lying at the Emperor''s feet.');
    IF Here(fguard) THEN BEGIN
	Writeln;
	IF guardalive THEN
	    IF guardsees THEN
		Writeln('There''s a Lugonian guard here, with his weapon drawn!')
	    ELSE
		Writeln('There''s a lone Lugiman on guard here, with his back turned.')
	ELSE
	    Writeln('There''s a dead Lugonian guard here.')
	END;
    IF Here(frunt) THEN BEGIN
	Writeln;
	Writeln('There''s a small, unhealthy-looking Lugiman here, a real runt, holding a');
	Writeln('cudgel!  He screams at you in Lugonian: something about "filthy Earth vermin"');
	Writeln('and "stinking Earth germs ...us allergic people can''t take it any more!"  He');
	Writeln('raises his cudgel, holding his nose, and prepares to bash your skull in...')
	END;
    ShowObjects;
    IF Here(fgorilla) THEN BEGIN
	Writeln;
	IF goralive THEN
	    Writeln('There''s a huge hungry-looking gorilla here!  The only way around is to jump.')
	ELSE
	    Writeln('There''s a large gorilla here.  It looks dead.')
	END;
    IF (flies > 0) AND (env > fogarc) THEN BEGIN
	flies := flies + flies DIV 3 + RandInt(flies DIV 3 + 1);
	Writeln;
	Writeln('There are ',flies: 1,' flies buzzing annoyingly around your head.')
	END;
    IF flies >= 10000 THEN BEGIN
	IF env = balcony THEN BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('Ten thousand muscular Lugonian flies are quite capable of lifting your entire');
	    Writeln('weight off the ground.  Exerting their utmost combined strength, the flies');
	    Writeln('around you lift you up, bring you near the balcony''s edge so you can see');
	    Writeln('the sidewalk beneath, and toss you over.')
	    END
	ELSE BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('Ten thousand muscular Lugonian flies are quite capable of lifting your');
	    Writeln('body off of the ground.  Exerting their utmost combined strength, the');
	    Writeln('flies around you carry you through rooms and corridors...  and fling');
	    Writeln('you off the balcony, five floors above the street.')
	    END;
	wayended := jumped
	END;
(*  enforcecount := enforcecount + 1;  {**** SU-LOTS only} *)
(*  IF (enforcecount  >= enforcefreq) AND (wayended = notdone) THEN BEGIN  {**** ditto}
	enforcecount := 0;
	EnforceGamePlayingRulesAtStanfordLOTS
	END; *)
    IF wayended = notdone THEN BEGIN
	Writeln;
	Writeln('What you wanna do, Lou?')
	END;
    IF (env = vent) AND (venttime > Timer) THEN
	venttime := Timer;
    IF (dumproom = yourroom) AND (dumptime > Timer) THEN
	dumptime := Timer
    END;  {Situation}




PROCEDURE ReadWord(VAR answer: meaning);
{This reads one word from the (input) buffer, stores it in (lastword), and}
{uses (dictionary) to translate it into a meaning, returned as (answer).  }
    VAR
	i: integer;
	p: ^dictentry;
	result: boolean;
    BEGIN
    REPEAT
	REPEAT  {input (lastword)}
	    i := 1;
	    WHILE (i <= maxwordlen) AND (((input^ >= 'A') AND (input^ <= 'Z'))
				OR ((input^ >= 'a') AND (input^ <= 'z'))) AND NOT Eoln(input) DO BEGIN
		IF input^ < '_' THEN
		    lastword.w[i] := Chr(Ord(input^) + 32)
		ELSE
		    lastword.w[i] := input^;
		Get(input);
		i := i + 1
		END;
	    lastword.l := i - 1;
	    IF (lastword.l = 0) AND NOT Eoln(input) THEN
		Get(input)
	UNTIL (lastword.l > 0) OR Eoln(input);
	IF lastword.l = 0 THEN  {translate}
	    answer := nomeaning
	ELSE BEGIN
	    IF lastword.l = 1 THEN
		p := dictionary[Ord(lastword.w[1]) MOD (dictsize + 1)]
	    ELSE
		p := dictionary[((Ord(lastword.w[1]) - Ord('a')) * 26 + Ord(lastword.w[2])) MOD (dictsize + 1)];
	    answer := nomeaning;
	    result := false;
	    REPEAT
		IF p <> NIL THEN BEGIN
		    i := 1;
		    WHILE (i < lastword.l) AND (i < p^.dword.l) AND (lastword.w[i] = p^.dword.w[i]) DO
			i := i + 1;
		    IF p^.dword.w[i] > lastword.w[i] THEN
			result := true
		    ELSE
			IF (p^.dword.w[i] = lastword.w[i]) AND ((lastword.l = p^.dword.l)
					OR ((lastword.l >= 5) AND (p^.dword.l >= lastword.l))) THEN BEGIN
			    result := true;
			    answer := p^.dmeaning
			    END
			ELSE
			    p := p^.nextde
		    END
		ELSE
		    result :=  true
	    UNTIL result;
	    IF dupcatch[answer] THEN
		answer := nomeaning
	    ELSE
		dupcatch[answer] := true;
	    END;
	IF answer = mr THEN
	    WHILE NOT Eoln(input) DO
		Get(input)
    UNTIL (answer > nomeaning) OR Eoln(input);
    IF ((answer >= orope) AND (answer <= fcentrifuge)) OR (answer = mgleeps) THEN
	it := answer
    END;  {ReadWord}




PROCEDURE SayName(object: meaning; fancy: boolean);
{This procedure writes the name of (object).  If (fancy), it writes }
{(when object = orope) like this: 'Rope:  '  -- If (fancy) is false,}
{it just writes 'rope', using (lastword) instead of looking the word}
{up, unless (lastword) = 'it'.					    }
    VAR
	i: integer;
    BEGIN
    IF object = mgleeps THEN
	IF fancy THEN
	    Write('Gleeps:  ')
	ELSE
	    Write('gleeps')
    ELSE BEGIN 
	IF fancy OR ((lastword.w[1] = 'i') AND (lastword.w[2] = 't') AND (lastword.l = 2)) THEN
	    lastword := bestword[object];
	IF fancy THEN
	    Write(Chr(Ord(lastword.w[1]) - 32))
	ELSE
	    Write(lastword.w[1]);
	FOR i := 2 TO lastword.l DO
	    Write(lastword.w[i]);
	IF fancy THEN
	    Write(':  ')
	END
    END;  {SayName}




PROCEDURE Inventory(where: place);
{This writes a list of all the objects in (where).  It indents the list to}
{reflect the depth of recursive Inventory calls.			  }
    VAR
	ob: meaning;
	nothing: boolean;  {player has no objects}
    BEGIN
    invdepth := invdepth + 1;
    IF invdepth > 20 THEN BEGIN
	Writeln('ARRRGH!  Inventory is into too-deep recursion!  Something contains itself??');
	wayended := crash
	END;
    IF wayended = notdone THEN BEGIN
	IF where = pockets THEN
	    nothing := gleeps = 0
	ELSE
	    IF where = inbag THEN
		nothing := baggleeps = 0
	    ELSE
		nothing := true;
	FOR ob := orope TO opills DO
	    IF location[ob] = where THEN BEGIN
		Write(' ': invdepth * 4);
		nothing := false;
		CASE ob OF
		    orope: Writeln(     'coil of rope');
		    ohammer: Writeln(   'large sledgehammer');
		    onitro: Writeln(    'can of nitroglycerine');
		    okeys: Writeln(     'oddly shimmering keys');
		    oflask: BEGIN
			Writeln(        'flask, which contains:');
			Inventory(inflask)
			END;
		    owater: Writeln(    'water');
		    ochemicals: Writeln('mysterious chemicals');
		    ourine: Writeln(    'warm urine');
		    omatches: Writeln(  'book of matches (one left)');
		    ofluid: Writeln(    'bottle of fluid');
		    ogum: Writeln(      'stick of gum');
		    oacetone:
			IF acetoneopen THEN
			    Writeln(    'open can of acetone')
			ELSE
			    Writeln(    'can of acetone');
		    oskull: Writeln(    'bloodstained skull');
		    osandwich:
			IF badsandwich THEN
			    Writeln(    'half-eaten sandwich')
			ELSE
			    Writeln(    'disinfected half-eaten sandwich');
		    obag: BEGIN
			  Writeln(      'small canvas bag, which contains:');
			  Inventory(inbag)
			  END;
		    olysol: Writeln(    'Lysol disinfectant');
		    oflysol: Writeln(   'Flysol infectant');
		    oraid: Writeln(     'Raid insecticide');
		    ostatuette: Writeln('statuette');
		    opills: Writeln(    'pills and capsules')
		    END
		END;
	IF where = pockets THEN
	    IF gleeps > 0 THEN
		IF gleeps = 1 THEN
		    Writeln(' ': invdepth * 4,'a gleep')
		ELSE
		    Writeln(' ': invdepth * 4,gleeps: 1,' gleeps')
	    ELSE
	ELSE
	    IF where = inbag THEN
		IF baggleeps > 0 THEN
		    IF baggleeps = 1 THEN
			Writeln(' ': invdepth * 4,'a gleep')
		    ELSE
			Writeln(' ': invdepth * 4,baggleeps: 1,' gleeps');
	IF nothing THEN
	    Writeln(' ': invdepth * 4,'nothing.')
	END;
    invdepth := invdepth - 1
    END;  {Inventory}




PROCEDURE NasaDeath;
{This kills the player when he messes with the centrifuge.}
    BEGIN
    Writeln('You reach upward and clamber half way into the centrifuge cage...');
    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
    Writeln('A small, giggling Lugiman, perhaps a child, runs into the room and plugs in');
    Writeln('the machine!  It starts to spin;  in moments you are pinned in place,');
    Writeln('unable to move from the doorway as the awful force of the machine crushes');
    Writeln('your torso against the hard edge of the frame...  With your last moments of');
    Writeln('conciousness, you seem to see a small, deformed Lugiman enter, and the child');
    Writeln('run toward it, looking like a guilty human child...  Then, as bones break and');
    Writeln('tissues tear, all senses fade.....');
    wayended := killed
    END;




FUNCTION Weight: integer;
{This function returns the total weight of all the objects the player is carrying.}
    VAR
	j: integer;
    BEGIN
    j := 0;
    IF Have(orope,true) THEN
	j := j + 2;
    IF Have(ohammer,true) THEN
	j := j + 5;
    IF Have(onitro,true) THEN
	j := j + 1;
    IF Have(owater,true) THEN
	j := j + 1;
    IF Have(ochemicals,true) THEN
	j := j + 1;
    IF Have(ourine,true) THEN
	j := j + 1;
    IF Have(oacetone,true) THEN
	j := j + 3;
    IF Have(oskull,true) THEN
	j := j + 1;
    IF Have(osandwich,true) THEN
	j := j + 1;
    IF Have(olysol,true) THEN
	j := j + 1;
    IF Have(oflysol,true) THEN
	j := j + 1;
    IF Have(oraid,true) THEN
	j := j + 1;
    IF Have(ostatuette,true) THEN
	j := j + 1;
    Weight := j	  {maximum possible = 18, because water, urine, and chemicals are mutually exclusive.}
    END;  {Weight}




PROCEDURE ImplicitTake(object: meaning; talk,lookin: boolean; VAR success: boolean);
{This procedure tries to take one object or furniture or bunch of gleeps.}
{It calls fancy SayName, but does not comfirm a successful take except by}
{setting (success) true.  This allows other procedures to try to take an }
{object they need, and use the SayName for themselves if the take doesn't}
{fail.  (If it does, a message will be output.)  DO NOT call this with   }
{object = mit or object = mall.  (talk) is true when this was called     }
{because of the command "take all", and is significant in Furniture.	 }
    VAR
	whereitwas: place;  {place object is being taken from}


    PROCEDURE Furniture(object: meaning);  {inside ImplicitTake}
    {Many of the room descriptions tell about objects in the room that  }
    {are not actual loose objects, but just decorations for the room.   }
    {When the player tries to take any of these, this procedure gives a }
    {plausible (most of the time) reason why he cannot.  The excuses are}
    {in (messagetext), pointed to by (furnindex).  (talk) means say "I  }
    {don't see it" if it's not there, else be silent.  It's false if    }
    {Furniture was invoked by 'take all'.				}
	VAR
	    i: integer;
	BEGIN
	IF (object = fbeast) AND (env = pillpile) AND talk AND NOT beastfollowing THEN
	    object := fvermin;
	IF (talk OR (object IN [fnewspapers..fbadge,fdirt..flaundry,fgags,ftar,fboxes]))
			    AND (object >= fstove) AND (object <= fcentrifuge) AND Here(object) THEN BEGIN
	    SayName(object,true);
	    IF object = fbadge THEN BEGIN
		Writeln('You pull at it, and discover that a thin thread is attached to it...');
		Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		Writeln('It''s a booby trap for sentimental CIA agents!  Ports spring open in the');
		Writeln('walls, deadly muzzles protrude, and Lugonian heat rays burn you to a crisp.');
		wayended := killed
		END
	    ELSE BEGIN
		FOR i := furnindex[object].startpos TO furnindex[object].endpos DO
		    IF messagetext[i] = '_' THEN
			Writeln
		    ELSE
			Write(messagetext[i]);
		Writeln;
		tellsitu := true
		END
	    END
	ELSE
	    IF talk THEN
		IF (env = columns) AND (object = ofluid) THEN
		    Writeln('Liquid:  You can''t get it out of the columns.')
		ELSE BEGIN
		    Write('I don''t see any ');
		    SayName(object,false);
		    IF lookin THEN
			Write(' inside anything');
		    Writeln(' here...')
		    END
	END;  {Furniture}


    BEGIN  {ImplicitTake}
    success := true;
    IF (Have(object,false) OR ((verb = vtake) AND Have(object,true) AND NOT lookin)) AND (object <> mgleeps) THEN
	IF verb = vtake THEN BEGIN
	    success := false;
	    IF talk THEN BEGIN
		SayName(object,true);
		IF (object = oskull) AND (env = bunker) OR (env = ghostbunker) THEN
		    Writeln('You already have one, and that''s plenty.')
		ELSE
		    IF (object = opills) AND (env = pillpile) THEN
			Writeln('You already have plenty.')
		    ELSE
			Writeln('You already have it.')
		END
	    END
	ELSE
	    IF talk THEN
		SayName(object,true)
	    ELSE
    ELSE BEGIN
	IF (verb <> vtake) AND talk THEN BEGIN
	    Write('(Take ');
	    IF (object >= orope) AND (object <= opills) AND Here(object) THEN
		IF (location[object] >= incabinet) AND (location[object] <= inflask) THEN BEGIN
		    lookin := true;
		    Write('out ')
		    END;
	    SayName(object,false);
	    Writeln(')')
	    END;
	success := false;
	IF (object >= orope) AND (object <= opills) THEN
	    IF Here(object) THEN BEGIN
		SayName(object,true);
		tellsitu := true;
		IF (object = omatches) AND ((env = cistern) OR (env = tank)) AND (location[omatches] <= pbalcony) THEN
		    Writeln('They''re soaking wet.  It would be pointless to take them.')
		ELSE
		    IF (object = osandwich) AND ((env = cistern) OR (env = tank)) THEN BEGIN
			Writeln('It''s so soggy that it disintegrates when you try to pick it up.');
			location[osandwich] := nowhere
			END
		    ELSE
			IF (object = okeys) AND glued AND NOT strong THEN
			    Writeln('Forget ''em--  the glue is way too strong.')
			ELSE
			    IF (object = owater) AND (env = seep) THEN
				Writeln('There''s not enough here to collect together.')
			    ELSE
				IF (object = owater) AND (NOT Have(oflask,true) OR
						(location[ochemicals] = inflask) OR (location[ourine] = inflask)) THEN
				    Writeln('You have nothing to put it in.')
				ELSE
				    IF ((object = owater) AND lookin) OR (object = ochemicals) OR (object = ourine) THEN
					Writeln('You can''t take it out because you wouldn''t be able to hold it.')
				    ELSE BEGIN
					whereitwas := location[object];
					location[object] := pockets;
					IF (object = okeys) AND glued THEN BEGIN
					    Writeln('You break the glue with your incredible strength...');
					    Write('       ');
					    glued := false
					    END;
					IF object = owater THEN
					    location[object] := inflask;
					IF Weight > (weightlimit - 2 * Ord(sickness)) * (1 + 2 * Ord(strong)) THEN BEGIN
					    Writeln('You can''t carry all that junk.  Drop something heavy first.');
					    location[object] := whereitwas
					    END
					ELSE BEGIN
					    success := true;
					    IF (object = osandwich) AND badsandwich THEN
						sandtime := Timer;
					    IF whereitwas = throneshelf THEN
						shelffull := false;
					    had[object] := true;
					    tellsitu := true
					    END
					END
		END
	    ELSE
		Furniture(object)  {merely says 'I don't see any <object>...'}
	ELSE
	    IF object = mgleeps THEN
		IF (NOT lookin AND (map[yourroom].gleepses > 0) AND (env > fogged))
					OR (lookin AND Have(obag,true) AND (baggleeps > 0)) THEN BEGIN
		    IF lookin THEN BEGIN
			gleeps := gleeps + baggleeps;
			baggleeps := 0
			END
		    ELSE BEGIN
			gleeps := gleeps + map[yourroom].gleepses;
			map[yourroom].gleepses := 0
			END;
		    IF gleeps > gleeplimit THEN BEGIN
			Writeln('(Not all the gleeps fit in your pocket.)');
			IF lookin THEN
			    baggleeps := gleeps - gleeplimit
			ELSE
			    map[yourroom].gleepses := gleeps - gleeplimit;
			gleeps := gleeplimit
			END;
		    SayName(mgleeps,true);
		    success := true;
		    Writeln('You now have ',gleeps: 1,' in your pockets.');
		    tellsitu := true
		    END
		ELSE
		    IF talk THEN
			IF env = nasa THEN
			    NasaDeath
			ELSE
			    IF lookin THEN
				Writeln('I don''t see any gleeps inside anything here...')
			    ELSE
				Writeln('I don''t see any gleeps here...')
		    ELSE
	    ELSE
		Furniture(object)
	END
    END;  {ImplicitTake}




PROCEDURE TakeThem;
{Responds to "take <object> <object> ...", taking what it can.}
    VAR
	object,ob: meaning;  {object being taken / index}
	success,onetried,  {individual object was really taken / at least one object has been attempted}
	    lookin,already: boolean;  {command was "take out" rather than "take" / player has object already}


    PROCEDURE Confirm(object: meaning);
    {Confirms that an object was taken.}
	BEGIN
	Writeln('OK');
	IF object = obag THEN BEGIN
	    Writeln('It contains:');
	    Inventory(inbag)
	    END;
	IF object = oflask THEN BEGIN
	    Writeln('It contains:');
	    Inventory(inflask);
	    it := oflask
	    END
	END;  {Confirm}


    BEGIN  {TakeThem}
    onetried := false;
    tellsitu := false;
    lookin := false;
    REPEAT
	ReadWord(object);
	IF object = mit THEN
	    object := it;
	IF object = mr THEN
	    object := nomeaning;
	IF object = vout THEN
	    IF onetried THEN
		Writeln('Sorry, you have to say "take out <objects>", not "take <objects> out".')
	    ELSE
		lookin := true
	ELSE
	    IF ((object >= orope) AND (object <= fcentrifuge)) OR (object = mgleeps) THEN BEGIN
		ImplicitTake(object,true,lookin,success);
		IF success AND (object <> mgleeps) THEN
		    Confirm(object)
		END
	    ELSE
		IF object = mall THEN BEGIN
		    FOR ob := orope TO fcentrifuge DO
			IF (wayended = notdone) AND ((ob <= opills) OR (ob = ftar) OR (ob = fcrystal) OR NOT lookin) THEN BEGIN
			    already := Have(ob,false);
			    ImplicitTake(ob,false,lookin,success);
			    IF success AND NOT already THEN
				Confirm(ob)
			    END;
		    ImplicitTake(mgleeps,false,lookin,success);
		    IF NOT tellsitu THEN
			Writeln('There is nothing here you can take.')
		    END
		ELSE
		    IF NOT onetried THEN
			Writeln('I don''t understand what you want to take.');
	    IF object <> vout THEN
		onetried := true
	UNTIL (((object < orope) OR (object > fcentrifuge)) AND (object <> mgleeps)) AND 
((object <> vout) OR onetried)
    END;  {TakeThem}




PROCEDURE PutThisThere(object: meaning; putgoal: place);
{This puts object (object) in place (putgoal), if possible.}


    PROCEDURE PutGleepsThere(putgoal: place);  {inside PutThisThere}
    {Puts gleeps into putgoal.}
	BEGIN
	IF (putgoal = inbag) AND (gleeps = 0) AND ((map[yourroom].gleepses = 0) OR (env <= fogged)) THEN
	    Writeln('They''re all in the bag already.')
	ELSE
	    IF (gleeps = 0) AND ((baggleeps = 0) OR NOT Have(obag,true)) AND (putgoal = yourroom) THEN
		Writeln('You have none to put down.')
	    ELSE
		IF putgoal = inflask THEN
		    Writeln('They don''t fit.')
		ELSE
		    IF (putgoal = incabinet) AND NOT cabinetopen OR (putgoal = intrunk) AND NOT trunkopen THEN
			Writeln('It isn''t open.')
		    ELSE BEGIN
			tellsitu := true;
			IF (putgoal >= thronecage) AND (putgoal <= intrunk) THEN BEGIN
			    Writeln('POP!  They disappear the moment they contact the surface.');
			    baggleeps := 0;
			    gleeps := 0;
			    IF env > fogarc THEN
				map[yourroom].gleepses := 0
			    END
			ELSE
			    IF putgoal = inbag THEN BEGIN
				baggleeps := baggleeps + gleeps;
				gleeps := 0;
				IF env > fogarc THEN BEGIN
				    baggleeps := baggleeps + map[yourroom].gleepses;
				    map[yourroom].gleepses := 0
				    END;
				Writeln('The bag now holds ',baggleeps: 1,'.')
				END
			    ELSE BEGIN
				map[yourroom].gleepses := map[yourroom].gleepses + gleeps;
				gleeps := 0;
				IF Have(obag,true) THEN BEGIN
				    map[yourroom].gleepses := map[yourroom].gleepses + baggleeps;
				    baggleeps := 0
				    END;
				IF (env = ghost) OR (env = ghostbunker) OR (env = bunker) THEN BEGIN
				    Writeln('POP!  They disappear in a puff of smoke.');
				    map[yourroom].gleepses := 0
				    END
				ELSE
				    Writeln('OK')
				END
			 END
	END;  {PutGleepsThere}


    PROCEDURE PutThisInImaginaryPlace(object: meaning);	  {inside PutThisThere}
    {Does the fancy footwork needed when an object is to be put into}
    {a place that doesn't actually exist.			    }
	BEGIN
	tellsitu := true;
	IF env = nasa THEN
	    NasaDeath
	ELSE
	    IF object = oflask THEN  {env must be niche}
		IF chemgiven THEN BEGIN
		    Writeln('As you let go of it, a glass door closes over the front of the niche.');
		    Writeln('A mechanical arm comes out of an opening in the back of the niche, picks');
		    Writeln('up the flask, and draws it back into the opening.  A few seconds later,');
		    Writeln('it emerges, with the flask now uncorked and full of urine.  It sets it down');
		    Writeln('and withdraws, and the glass door opens.');
		    location[ochemicals] := nowhere;
		    location[owater] := nowhere;
		    location[ourine] := inflask;
		    location[oflask] := yourroom
		    END
		ELSE BEGIN
		    Writeln('As you let go of it, a glass door closes over the front of the niche.');
		    Writeln('A mechanical arm comes out of an opening in the back of the niche, picks');
		    Writeln('up the flask, and draws it back into the opening.  A few seconds later,');
		    Writeln('it emerges, with the flask now corked and full of mysterious chemicals.  It');
		    Writeln('sets it down and withdraws, and the glass door opens.'); Writeln;
		    chemgiven := true;
		    location[ochemicals] := inflask;
		    location[owater] := nowhere;
		    location[ourine] := nowhere;
		    location[oflask] := yourroom
		    END
	    ELSE
		IF object = mgleeps THEN
		    PutGleepsThere(thronecage)  {POP! all vanish...}
		ELSE BEGIN
		    Writeln('As you let go of it, a glass door closes over the front of the');
		    Writeln('niche.  A mechanical arm comes out of an opening in the back of the niche,');
		    Writeln('picks it up, and turns it around a couple of times as if examining it.  The');
		    Writeln('door opens, you hear a sound like "pfui!", and the arm puts it down and');
		    Writeln('withdraws.  The door opens.'); Writeln;
		    location[object] := yourroom
		    END
	END;  {PutThisInImaginaryPlace}


    PROCEDURE PutSomethingElse(object: meaning; putgoal: place);  {inside PutThisThere}
    {Puts an ordinary object into a place successfully.}
	BEGIN
	tellsitu := true;
	location[object] := putgoal;
	IF (putgoal = yourroom) AND ((env = ghost) OR (env = ghostbunker)) THEN BEGIN
	    Writeln('It disappears in a puff of smoke.');
	    location[object] := nowhere
	    END
	ELSE
	    IF (putgoal = yourroom) AND (object = ogum) AND attached THEN BEGIN
		attached := false;
		location[ogum] := nowhere;
		Writeln('The scaly thing on your leg leaps for the gum, begins chewing it');
		Writeln('frantically, and, in a state of ecstacy, passes out on the floor.')
		END
	    ELSE
		IF goralive AND (putgoal = gorroom) AND (object = osandwich) AND (env <> fogged) THEN BEGIN
		    IF badsandwich THEN BEGIN
			Writeln('The gorilla grabs it and wolfs it down.  Soon, the mighty');
			Writeln('beast begins to look sick.  After about thirty seconds, it collapses.');
			goralive := false
			END
		    ELSE
			Writeln('The gorilla grabs it and wolfs it down.');
		    location[osandwich] := nowhere
		    END
		ELSE BEGIN
		    IF (putgoal = yourroom) AND ((env = bunker)
					OR (env = ghostbunker)) AND (object = oskull) THEN
			location[oskull] := nowhere;
		    IF (putgoal = yourroom) AND (env = pillpile) AND (object = opills) THEN
			location[opills] := nowhere;
		    Writeln('OK')
		    END
	END;  {PutSomethingElse}


    BEGIN  {PutThisThere}
    IF putgoal = imaginaryplace THEN
	PutThisInImaginaryPlace(object)
    ELSE
	IF object = mgleeps THEN
	    PutGleepsThere(putgoal)
	ELSE
	    IF location[object] = putgoal THEN
		Writeln('It''s there already.')
	    ELSE
		IF (putgoal = inbag) AND (object IN [orope,ohammer,oacetone,obag]) THEN
		    Writeln('It doesn''t fit in the bag.')
		ELSE
		    IF (putgoal = throneshelf) AND ((object IN [orope..onitro,oacetone..oraid]) OR shelffull) THEN
			Writeln('It doesn''t fit on the teeny shelf.')
		    ELSE
			IF (putgoal = inflask) AND (object <> owater) THEN
			    Writeln('It doesn''t fit in the flask.')
			ELSE
			    IF (env = seep) AND (object = owater) THEN
				Writeln('There''s not enough water here to collect together.')
			    ELSE
				IF (object = owater) AND (env <> cistern) AND (env <> tank) THEN
				    Writeln('I don''t see any water here...')
				ELSE
				    IF (putgoal = incabinet) AND NOT cabinetopen THEN
					Writeln('You have to open the cabinet first.')
				    ELSE
					IF (putgoal = intrunk) AND NOT trunkopen THEN
					    Writeln('You have to get the trunk open first.')
					ELSE
					    PutSomethingElse(object,putgoal)
    END;  {PutThisThere}




PROCEDURE PutThem;
{This procedure handles commands in the form "put <object> <object>...}
{[in [bag | flask]]".  It will take objects if necessary.	      }
    TYPE
	oblistelement = RECORD
			    next: ^oblistelement;
			    o: meaning
			    END;
    VAR
	obp,obhead: ^oblistelement;  {pointers for a list of objects appearing before "in"}
	object: meaning;
	success: boolean;
	putgoal: place;	  {where objects will be put}
    BEGIN
    tellsitu := false;
    obhead := NIL;
    REPEAT  {save list of objects for after the "in [bag | flask]" is maybe read}
	ReadWord(object);
	IF object = mit THEN
	    object := it;
	success := ((object >= orope) AND (object <= fcentrifuge)) OR (object = mit) OR (object = mall) OR (object = mgleeps);
	IF success THEN BEGIN
	    IF obhead = NIL THEN BEGIN
		New(obhead);
		obp := obhead
		END
	    ELSE BEGIN
		New(obp^.next);
		obp := obp^.next
		END;
	    obp^.o := object
	    END
    UNTIL NOT success;
    obp^.next := NIL;
    IF obhead = NIL THEN
	Writeln('I don''t understand what you want to put there.')
    ELSE BEGIN
	IF env <= fogarc THEN
	    putgoal := thronecage
	ELSE
	    putgoal := yourroom;
	IF object = min THEN BEGIN
	    ReadWord(object);
	    putgoal := nowhere;
	    IF (object = obag) AND NOT Here(obag) THEN
		Writeln('I don''t see any bag here...')
	    ELSE
		IF (object = oflask) AND NOT Here(oflask) THEN
		    Writeln('I don''t see any flask here...')
		ELSE
		    IF object = obag THEN
			putgoal := inbag
		    ELSE
			IF object = oflask THEN
			    putgoal := inflask
			ELSE
			    IF env IN [arcturus,fogarc,cabinet,niche,car,nasa] THEN
				CASE env OF
				    arcturus,fogarc: putgoal := thronecage;
				    cabinet: putgoal := incabinet;
				    niche,nasa: putgoal := imaginaryplace;
				    car: putgoal := intrunk
				    END
			    ELSE
				IF Here(obag) THEN
				    putgoal := inbag
				ELSE
				    IF Here(oflask) THEN
					putgoal := inflask
	    END;
	lastword.w[1] := 'i';
	lastword.w[2] := 't';
	lastword.l := 2;
	IF putgoal > nowhere THEN
	    REPEAT
		IF obhead^.o = mgleeps THEN
		    IF Here(mgleeps) THEN BEGIN
			SayName(mgleeps,true);
			PutThisThere(mgleeps,putgoal)
			END
		    ELSE
			Writeln('I don''t see any gleeps here...')
		ELSE
		    IF obhead^.o = mall THEN BEGIN
			FOR object := orope TO fcentrifuge DO
			    IF ((object <> obag) OR (putgoal <> inbag)) AND (object <> owater) 
						AND (object <> ourine) AND (object <> ochemicals) THEN BEGIN
				success := false;
				IF Have(object,false) THEN
				    SayName(object,true);
				IF (object >= orope) AND (object <= opills) THEN
				    IF location[object] <> putgoal THEN
					ImplicitTake(object,false,false,success);
				IF success THEN
				    PutThisThere(object,putgoal)
				END;
			SayName(mgleeps,true);
			PutThisThere(mgleeps,putgoal)
			END
		    ELSE
			IF (obhead^.o = owater) AND (putgoal = inflask) THEN BEGIN
			    SayName(owater,true);
			    PutThisThere(owater,inflask)
			    END
			ELSE BEGIN
			    IF ((obhead^.o = owater) OR (obhead^.o = ourine) OR (obhead^.o = ochemicals))
							  AND Have(obhead^.o,true) AND (putgoal <> inflask) THEN
				obhead^.o := oflask;
			    ImplicitTake(obhead^.o,true,false,success);
			    IF Have(obhead^.o,false) THEN
				PutThisThere(obhead^.o,putgoal)
			    END;
		obhead := obhead^.next
	    UNTIL (obhead = NIL) OR (wayended <> notdone)
	ELSE
	    IF (obhead^.o <> obag) AND (obhead^.o <> oflask) THEN
		Writeln('There''s nothing here to put it inside.')
	END
    END;  {PutThem}




PROCEDURE Blast(thrown: boolean);
{This procedure detonates the nitro.  (thrown) is true if the nitro}
{was thrown by the player, not dropped.			   	   }
    VAR
	m: meaning;
	x: converter;
    BEGIN
    location[onitro] := nowhere;
    IF thrown AND (env <> kerosene) THEN
	IF env <= fogarc THEN BEGIN
	    Writeln('      BOOOOOOMM!!'); Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('With a frightening roar, the nitro detonates, causing this end of the ceiling');
	    Writeln('to collapse slowly.  All the Lugimen run safely back, but you, trapped in the');
	    Writeln('cage, can''t emulate their escape, and are squashed like a bug.');
	    wayended := killed
	    END
	ELSE
	    IF env = balcony THEN BEGIN
		Writeln('It strikes a pedestrian below.  BLAAMMM!!   You''ve killed about');
		Writeln('three people, and you''re in big trouble.');
		bystanders := true
		END
	    ELSE
		IF env = meadow THEN BEGIN
		    Writeln('     BLLAAAMMM!!  The nitro detonates, destroying a bush and');
		    Writeln('several square feet of grass.')
		    END
		ELSE
		    IF (map[yourroom].features = cistern) OR (map[yourroom].features = tank) THEN BEGIN
			Writeln('Splash!  It lands in the water you are standing in without detonating.');
			location[onitro] := yourroom  {un-detonate}
			END
		    ELSE BEGIN
			Writeln('      BLAAMMMMMMM!!');
			Writeln('With a frightening roar, the nitroglycerine detonates, blasting holes in all');
			Writeln('four walls, and the ceiling besides!  You are miraculously unscathed.');
			FOR m := vnorth TO vclimb DO
			    map[yourroom].paths[m] := Distant(yourroom);
			map[yourroom].paths[RandDirection(vwest)] := Succ(yourroom);
			IF Here(fguard) AND guardalive THEN BEGIN
			    starttime := starttime - timeloss;
			    guardalive := false;
			    location[ofluid] := yourroom;
			    IF env <> fogged THEN BEGIN
				Writeln(Chr(7),'The stunned guard staggers to his feet, hits the alarm, and falls dead.');
				Writeln('A small, delicate glass bottle rolls out of his pocket.')
				END
			    END;
			IF Here(frunt) THEN BEGIN
			    Writeln('The unhealthy-looking Lugiman is blown to bits!');
			    runtroom := nowhere
			    END;
			IF (env = garden) AND NOT burnt THEN BEGIN
			    burnt := true;
			    env := burntplants;
			    map[yourroom].features := burntplants;
			    Writeln;
			    Writeln('The horrible plants are burnt to ashes instantly!')
			    END
			END
    ELSE BEGIN
	IF verb = vdrop THEN
	    Writeln('OK...');
	Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	Writeln('KER-BLAAMMMMMO!!!  The nitro detonates as it hits and blows you to bits!');
	wayended := killed
	END;
    IF (wayended = notdone) AND (location[onitro] = inbag) THEN BEGIN
	Writeln;
	Writeln('Since the nitro was in the bag when it detonated, said bag and all it''s');
	Writeln('contents have been utterly destroyed.');
	FOR m := orope TO opills DO
	    IF (location[m] = inbag) OR (m = obag) THEN
		location[m] := nowhere
	END
    END;  {Blast}




PROCEDURE Fumes;
{This procedure releases fumes when the player opens the flask,}
{and it contains the chemicals.				       }
    BEGIN
    Writeln('A cloud of horribly foul smelling fumes rise from the open flask.');
    location[ochemicals] := nowhere;
    IF env <= fogarc THEN BEGIN
	Writeln('It drives the Lugimen into utter madness!');
	Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	Writeln('Unable to escape the utterly intolerable (to them) odor, they run about');
	Writeln('shrieking, and many begin firing their weapons at random.  There are very few');
	Writeln('survivors;  the Emperor is one of them, but you''re not.');
	wayended := killed
	END
    ELSE BEGIN
	IF (yourroom = guardroom) AND (env > fogarc) AND guardalive THEN BEGIN
	    IF env > fogged THEN BEGIN
		Writeln;
		Writeln('The guard shrieks in dismay and runs out of the room!')
		END;
	    guardroom := Distant(yourroom)
	    END;
	IF (yourroom = runtroom) AND (env > fogarc) THEN BEGIN
	    IF env > fogged THEN BEGIN
		Writeln;
		Writeln('The unhealthy Lugiman shrieks and runs away holding his nose!');
		runtroom := Distant(yourroom)
		END;
	    runtroom := Distant(yourroom)
	    END;
	IF (lastwarning > nosight) AND (env <> meadow) THEN BEGIN
	    starttime :=  starttime + timeloss;
	    Writeln('The odor drives back the pursuing Lugimen, gaining you valuable time!')
	    END;
	IF env = machine THEN BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('The Lugiman that was putting the oil in the machine runs away!  The machine');
	    Writeln('starts to run hot...  hotter...  It shrieks and shudders...'); Writeln;
	    Writeln('With a terrible roar that must be audible for miles, the machine tears itself');
	    Writeln('asunder.  The huge mass of metal writhes and groans, collapsing down on you.');
	    Writeln('Your body is utterly crushed, and scorched, and beaten into a formless paste.');
	    wayended := killed
	    END;
	IF env = vent THEN BEGIN
	    Writeln;
	    Writeln('The Lugimen eating lunch shriek and scramble for the exits!  One brave');
	    Writeln('young Lugiman leaps for the cook''s Flysol spray and, straining to withstand');
	    Writeln('the intolerable smell, sprays the stuff into the air. The smell is quickly');
	    Writeln('eradicated.  Most of the other Lugimen, who are badly shaken, call him a hero');
	    Writeln('and resume eating, but some leave to search for the perpetrator.  You have');
	    Writeln('probably lost time, all things considered.');
	    starttime := starttime - timeloss
	    END;
	IF peeked THEN BEGIN
	    Writeln;
	    Writeln('The charging Lugiman is so enraged that he ignores the smell, normally');
	    Writeln('intolerable to Lugimen!')
	    END;
	IF attached AND (wayended = notdone) THEN BEGIN
	    Writeln;
	    Writeln('The scaly thing on your leg starts purring.')
	    END;
	IF (verb <> vdrop) AND (wayended = notdone) THEN BEGIN
	    Writeln;
	    Writeln('Hmm, you''ve lost the cork.  You must have dropped it because of');
	    Writeln('the horrible smell.')
	    END
	END
    END;  {Fumes}




PROCEDURE DropThisHere(object: meaning; dosayname: boolean);
{This procedure drops an object.  It calls fancy SayName if (dosayname).}
    VAR
	goal: place;  {where object ends up}
	thrown: boolean;  {object was thrown}


    PROCEDURE DropBag;  {inside DropThisHere}
    {This procedure drops (or throws) the bag, breaking things inside.}
	BEGIN
	location[obag] := goal;
	Write('These objects inside the bag also get ');
	IF thrown THEN
	    Writeln('thrown:')
	ELSE
	    Writeln('dropped:');
	Inventory(inbag);
	Writeln;
	IF location[onitro] = inbag THEN
	    Blast(thrown);
	IF thrown THEN
	    verb := vthrow;
	IF (location[oflask] = inbag) AND (wayended = notdone) THEN
	    DropThisHere(oflask,true);
	IF (location[ofluid] = inbag) AND (wayended = notdone) THEN
	    DropThisHere(ofluid,true);
	IF (location[oacetone] = inbag) AND (wayended = notdone) THEN
	    DropThisHere(oacetone,true);
	IF (location[osandwich] = inbag) AND (wayended = notdone) AND thrown THEN
	    DropThisHere(osandwich,true)
	END;  {DropBag}


    BEGIN  {DropThisHere}
    tellsitu := true;
    thrown := verb = vthrow;
    IF dosayname THEN
	SayName(object,true);
    IF thrown THEN
	verb := vdrop;
    IF thrown AND (env = balcony) THEN BEGIN
	Writeln('It sails over the balcony...');
	Write('       ');
	goal := nowhere
	END
    ELSE
	IF env <= fogarc THEN
	    IF thrown THEN BEGIN
		Writeln('It sails across the throne room...');
		Write('       ');
		goal := nowhere
		END
	    ELSE
		goal := thronecage
	ELSE
	    goal := yourroom;
    IF ((env = ghost) OR (env = ghostbunker)) AND (object <> ofluid) THEN BEGIN
	location[object] := nowhere;
	IF (object = okeys) OR (object = opills) THEN
	    Writeln('They disappear in a puff of smoke.')
	ELSE
	    Writeln('It disappears in a puff of smoke.')
	END
    ELSE BEGIN
	IF NOT (object IN [ohammer,onitro,oflask..ochemicals,ofluid,oacetone,osandwich]) THEN
	    PutThisThere(object,goal);
	CASE object OF
	    ohammer: BEGIN
		Writeln('CLUNK!');
		location[ohammer] := goal;
		IF (location[onitro] = goal) AND (goal > nowhere) THEN BEGIN
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('Ker-BLAAMMMM!    I''m afraid the shock of the heavy hammer falling has');
		    Writeln('detonated the nitroglycerine.  Bits of your flesh mar the walls.');
		    wayended := killed
		    END
		END;
	    onitro: Blast(thrown);
	    okeys:
		IF env <= fogarc THEN
		    Writeln('OK')
		ELSE
		    IF env = bubble THEN BEGIN
			Writeln('There is a dazzling flash of light, and a sudden wrenching');
			Writeln('discontinuity as the floor seems to heave madly.  You recover in moments...');
			env := arcturus
			END
		    ELSE BEGIN
			Writeln('The moment it hits:  Zzzzzzzzzzzzzzzzzzzziiiiiipp  ---    Pooooff!!');
			Writeln('You are engulfed in thick gray smoke.');
			yourroom := Distant(yourroom);
			env := map[yourroom].features;
			peeked := false;
			IF NOT (env IN [balcony,kerosene,meadow]) AND (fogtime <= Timer) THEN
			    env := fogged
			END;
	    oflask: BEGIN
		Writeln('It breaks open on the ground...');
		location[oflask] := nowhere;
		IF location[ochemicals] = inflask THEN
		    IF thrown AND (env = balcony) THEN
			Writeln('Foul smelling fumes rise up from the sidewalk.')
		    ELSE
			Fumes
		END;
	    ofluid:
		IF thrown AND (env = balcony) THEN BEGIN
		    Writeln('...and breaks into a million pieces on the sidewalk below.');
		    Writeln('A powerful smell drifts up, a smell you recognize...'); Writeln;
		    Writeln('It''s Lugi pheromone!  You hear yells and fast-running booted feet, and then');
		    Writeln('hundreds of lust-crazed Lugimen dash out onto the street!  You are sickened');
		    Writeln('by what follows...   When it''s over, and the satiated Lugimen finally');
		    Writeln('withdraw, several people have been killed.  You''re in big trouble.');
		    location[ofluid] := nowhere;
		    bystanders := true
		    END
		ELSE BEGIN
		    IF verb = vdrop THEN BEGIN
			Writeln('It breaks, releasing a powerful odor that instantly fills you with dread.');
			Writeln('Suddenly, you recall it''s nature...')
			END;
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('It is Lugi pheromone!  From near and far, hundreds of lust-crazed');
		    Writeln('Lugimen dash to the spot, guided by the overpowering, irresistable smell.');
		    Writeln('Nothing stands in their way...      The resulting experience is a fate');
		    Writeln('worse than death, so you don''t really mind when they eat you for dinner.');
		    location[ofluid] := nowhere;
		    wayended := eaten
		    END;
	    oacetone:
		IF acetoneopen THEN BEGIN
		    IF env <= fogarc THEN BEGIN
			Writeln('It spills all over the floor.  The big animal that was sitting by the');
			Writeln('Emperor''s feet leaps eagerly forward and laps up every trace of the liquid.')
			END
		    ELSE
			IF (env <> balcony) OR NOT thrown THEN BEGIN
			    IF glued AND (location[okeys] = yourroom) THEN BEGIN
				glued := false;
				Writeln('It spills out and dissolves the glue!');
				Writeln('The powerful fumes make your eyes sting.')
				END
			    ELSE BEGIN
				Writeln('It spills all over the floor.');
				Writeln('The powerful fumes make your eyes sting.')
				END;
			    IF beastfollowing THEN BEGIN
				Writeln('The big animal that has been following you around lunges forward and eagerly');
				Writeln('laps up every trace of the liquid.  After finishing, it returns to your side.')
				END
			    ELSE
				dumproom := yourroom
			    END
			ELSE
			    Writeln('It splashes all over the sidewalk, releasing powerful fumes.');
		    location[oacetone] := nowhere
		    END
		ELSE
		    PutThisThere(oacetone,goal);
	    osandwich:
		IF thrown THEN BEGIN
		    Writeln('It flies into pieces on impact.  No more sandwich.');
		    location[osandwich] := nowhere;
		    wipeout := true
		    END
		ELSE
		    PutThisThere(osandwich,goal);
	    obag: DropBag
	    END
	END
    END;  {DropThisHere}




PROCEDURE DropIt;
{This procedure attempts to drop or throw some object.}
    VAR
	object: meaning;
	success: boolean;  {not actually used for anything}
    BEGIN
    tellsitu := false;
    ReadWord(object);
    IF object = mit THEN
	object := it;
    IF (object = owater) OR (object = ochemicals) OR (object = ourine) THEN
	object := oflask;
    IF object = mall THEN
	Writeln('Don''t try to drop or throw more than one thing at once... It scares me.')
    ELSE
	IF (object >= orope) AND (object <= fcentrifuge) THEN BEGIN
	    ImplicitTake(object,true,false,success);
	    IF Have(object,false) THEN
		DropThisHere(object,false)
	    END
	ELSE
	    IF object = mgleeps THEN
		IF Here(mgleeps) THEN BEGIN
		    SayName(mgleeps,true);
		    IF ((env <= fogarc) OR (env = balcony)) AND (verb = vthrow) THEN BEGIN
			Writeln('They fly off into the distance...');
			tellsitu := true;
			gleeps := 0;
			IF env = balcony THEN
			    map[yourroom].gleepses := 0;
			IF Have(obag,true) THEN
			    baggleeps := 0
			END;
		    IF env <= fogarc THEN
			PutThisThere(mgleeps,thronecage)
		    ELSE
			PutThisThere(mgleeps,yourroom)
		    END
		ELSE
		    Writeln('I don''t see any gleeps here...')
	    ELSE BEGIN
		Write('I don''t understand what you wanna ');
		IF verb = vthrow THEN
		    Writeln('throw.')
		ELSE
		    Writeln('drop.')
		END
    END;  {DropIt}
    



PROCEDURE EatIt;
{This procedure attempts to eat or drink an object.}
    VAR
	object: meaning;
	c: letter;  {player's response to a question}
	success: boolean;  {we've got the object}
    BEGIN
    ReadWord(object);
    IF object = mit THEN
	object := it;
    IF (object >= orope) AND (object <= fcentrifuge) THEN BEGIN
	IF (object >= owater) AND (object <= ourine) THEN
	    success := Here(object)  {a rare priviledge: you get to drink it without picking it up}
	ELSE
	    ImplicitTake(object,true,false,success);
	IF success THEN
	    IF object IN [owater..ourine,ofluid..oacetone,osandwich,opills] THEN
		CASE object OF
		    owater: BEGIN
			SayName(owater,true);
			IF (env = cistern) OR (env = tank) THEN
			    Writeln('It is refreshing, despite the slight rotted taste.')
			ELSE
			    IF env = seep THEN
				Writeln('You can''t collect enough to drink.')
			    ELSE BEGIN
				Writeln('The small amount from the flask is refreshing, despite a funny taste.');
				location[owater] := nowhere
				END
			END;
		    ochemicals: BEGIN
			SayName(ochemicals,true);
			Writeln('To drink them, you pull out the stopper, and...');
			Fumes
			END;
		    ourine:  BEGIN
			SayName(ourine,true);
			Writeln('Steeling your nerves, you drink it down.  Five seconds later, you start');
			Writeln('to vomit repeatedly and violently.');
			Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
			Writeln('The Lugimen find you curled up on the floor dry-heaving, and eat you.');
			wayended := eaten
			END;
		    ofluid: BEGIN
			Writeln('As the seal cracks open, a powerful smell assaults your');
			Writeln('nostrils, which instantly fills you with unexplainable dread.  Suddenly, you');
			Writeln('recall its nature...'); Writeln;
			DropThisHere(ofluid,false)
			END;
		    ogum: BEGIN
			Writeln('Yeeeccccccchh!!  It''s flavored with creosote, the black tarry stuff you');
			Writeln('find on telephone poles.  You spit it out fast, and now it''s lost.');
			location[ogum] := nowhere
			END;
		    oacetone:
			IF acetoneopen THEN BEGIN
			    Writeln('(glug-glug-glug-glug-glug-glug.....)      (burp!)');
			    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
			    Writeln('Acetone happens to be toxic.');
			    wayended := killed
			    END
			ELSE
			    Writeln('You have to open the can first.');
		    osandwich:
			IF badsandwich THEN BEGIN
			    wayended := killed;
			    Writeln('(munch, munch, munch...)');
			    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
			    Writeln('It''s infected; you get sick and die.')
			    END
			ELSE BEGIN
			    Writeln('OK...  (munch munch)  --- It''s breast of toad and Velveeta, cold.');
			    location[osandwich] := nowhere
			    END;
		    opills:
			IF (env = cistern) OR (env = tank) OR Have(owater,true) THEN BEGIN
			    Writeln('You swallow some of your pills, washing them down with water.');
			    Writeln('Nothing happens just yet...');
			    IF (env <> cistern) AND (env <> tank) THEN
				location[owater] := nowhere;
			    IF pilltime > Timer THEN
				pilltime := Timer
			    ELSE BEGIN
				overdose := true;
				Writeln;
				Writeln('(You know, that was probably an overdose...)')
				END
			    END
			ELSE
			    Writeln('You try to swallow the pills, but you need water to wash them down with.')
		    END
	    ELSE
		Writeln('It can''t be swallowed.')
	ELSE
	    IF object = ochemicals THEN
		Writeln('I don''t see any chemicals here...')
	    ELSE
		IF object = owater THEN
		    Writeln('I don''t see any water here...')
		ELSE
		    IF object = ourine THEN
			Writeln('I don''t see any urine here...')
	END
    ELSE
	IF object = mall THEN
	    Writeln('You can only swallow one thing at a time.')
	ELSE
	    IF object = mgleeps THEN
		Writeln('Gleeps aren''t edible.')
	    ELSE BEGIN
		tellsitu := false;
		Writeln('I don''t understand what you want to swallow.')
		END
    END;  {EatIt}




PROCEDURE SprayIt;
{This procedure attempts to spray an object.  It succeeds if the object is}
{the Lysol, the Flysol, or the Raid.					  }
    VAR
	object: meaning;  {what to spray}
	didsomething,success: boolean;  {the spray has accomplished something / implicit take succeeded}
    BEGIN
    tellsitu := false;
    ReadWord(object);
    IF object = mit THEN
	object := it;
    IF ((object >= orope) AND (object <= fcentrifuge)) OR (object = mgleeps) THEN BEGIN
	ImplicitTake(object,true,false,success);
	IF success THEN
	    IF (object >= olysol) AND (object <= oraid) THEN
		CASE object OF
		    olysol: BEGIN
			tellsitu := true;
			didsomething := false;
			IF env = bathroom THEN BEGIN
			    Writeln('Almost instantly, the scummy bathroom fixtures around you');
			    Writeln('begin to sparkle!  The stink becomes a clean fresh smell.');
			    didsomething := true;
			    map[yourroom].features := cleanbathroom;
			    env := cleanbathroom;
			    clean := true
			    END;
			IF (env > fogged) AND (env <> balcony) AND (env <> meadow) THEN BEGIN
			    Writeln('Within seconds, it reacts with the Embassy''s semi-alien air, forming');
			    Writeln('a dense brown fog through which you can''t see two inches.');
			    didsomething := true;
			    fogtime := Timer;
			    IF env <> kerosene THEN
				env := fogged
			    END;
			IF env = arcturus THEN BEGIN
			    Writeln('Within seconds, it reacts with the alien air of Arcturus IV, forming');
			    Writeln('a dense brown fog that neither you nor the guards can see two inches through.');
			    didsomething := true;
			    thrfogtime := Timer;
			    env := fogarc
			    END;
			IF fungus THEN BEGIN
			    Writeln('You feel the fungus on your arm dry up and blow away!');
			    didsomething := true;
			    fungus := false
			    END;
			IF Here(osandwich) THEN BEGIN
			    badsandwich := false;
			    didsomething := true;
			    Writeln('You have just disinfected the sandwich.')
			    END;
			IF NOT didsomething THEN
			    Writeln('You can''t see any effects of the spray.')
			END;
		    oflysol: BEGIN
			tellsitu := true;
			didsomething := (env = fogarc) OR (flies > 0) OR (env = fogged);
			IF env = fogged THEN BEGIN
			    fogtime := maxint;
			    env := map[yourroom].features;
			    Writeln('Like magic, the brown fog dissipates, and you can see!')
			    END;
			IF env = fogarc THEN BEGIN
			    env := arcturus;
			    Writeln('Like magic, the brown fog dissipates, and the guards can see you!')
			    END;
			IF (env > fogarc) AND (flies = 0) THEN BEGIN
			    flies := 1 + RandInt(1);
			    Writeln('One or two flies buzz into the room.')
			    END
			ELSE
			    IF NOT didsomething THEN
				Writeln('You can''t see any effects of the spray.')
			END;
		    oraid: BEGIN
			tellsitu := true;
			didsomething := false;
			IF NOT fungus THEN BEGIN
			    fungus := true;
			    didsomething := true;
			    Writeln('Something starts to itch on your right arm.')
			    END;
			IF (flies > 0) AND (env > fogarc) THEN BEGIN
			    Writeln('Most of the flies die and fall on the floor.');
			    didsomething := true;
			    flies := flies DIV 4
			    END;
			IF NOT didsomething THEN
			    Writeln('You can''t see any effects of the spray.')
			END
		    END
	  ELSE
	    Writeln('It doesn''t spray.')
	END
    ELSE
	IF object = mall THEN
	    Writeln('You can only spray one thing at a time.')
	ELSE
	    Writeln('I don''t understand what you want to spray.')
    END;  {SprayIt}




PROCEDURE Defecate;
{This attempts to defecate.  Also takes care of urinating into a toilet.}
    BEGIN
    IF env = bathroom THEN BEGIN
	Writeln('Your waste matter fizzes as it hits the water.  The smell is horrible,');
	Writeln('and a scummy layer of eecccch settles on all the fixtures.');
	map[yourroom].features := bathroom;
	env := bathroom;
	clean := false
	END
    ELSE
	IF env = mensroom THEN BEGIN
	    tellsitu := false;
	    Writeln('I just told you that the toilets and urinals are all clogged with dust.')
	    END
	ELSE BEGIN
	    Writeln('Since no toilet is available, you lower your pants and defecate onto');
	    Writeln('the floor.  Within seconds, strange growths appear on the surface of your');
	    Writeln('waste matter.  With fantastic speed, the excrement becomes a mimiature jungle');
	    Writeln('of fantastic organisms, and soon it shrinks away.  Then the growths whither,');
	    Writeln('leaving behind only a bit of dust.')
	    END
    END;  {Defecate}




PROCEDURE Urinate;
{This attempts to urinate.}
    BEGIN
    IF Here(frunt) THEN
	Writeln('You open your pants, getting ready to urinate...')
    ELSE
	IF (env = bathroom) OR (env = mensroom) THEN
	    Defecate
	ELSE
	    IF Have(oflask,true) AND (location[ochemicals] <> inflask) AND
			     (location[ourine] <> inflask) AND (location[owater] <> inflask) THEN BEGIN
		Writeln('You urinate into the flask, since nothing else is available.');
		location[ourine] := inflask
		END
	    ELSE
		IF (env = cistern) OR (env = tank) THEN BEGIN
		    Writeln('Since absolutely nothing is available to urinate into, you piss into');
		    Writeln('the water, causing a slight change of it''s overall color.')
		    END
		ELSE BEGIN
		    Writeln('Since absolutely nothing is available to urinate into, you piss on the');
		    Writeln('ground.  The puddle begins to dry.')
		    END
    END;  {Urinate}




PROCEDURE Peeek(peekroom: place; VAR peeked: boolean);
{Peeks.}
    BEGIN
    IF env = peekhole THEN
	IF peekroom > nowhere THEN
	    Writeln('The small Lugonian is alone in the room, staring blankly at the wall.')
	ELSE BEGIN
	    peeked := true;
	    Writeln('You look through the hole, and see two Lugimen in a small room, apparently');
	    Writeln('engaging in noisy reproductive activity.  Something looks wrong, until you ');
	    Writeln('notice that the more passive of the two is only three feet tall and has a');
	    Writeln('very small skull, apparently a degenerate variety of Lugiman.  The larger');
	    Writeln('one is holding an old, heavily decorated projectile weapon.  Suddenly, he');
	    Writeln('feeezes, and looks right at your peephole!  He gives a scream of rage and');
	    Writeln('charges out the door!  You hear him bellowing and blundering around, nearer');
	    Writeln('and nearer!  If I were you, I would run!')
	    END
    ELSE BEGIN
	tellsitu := false;
	Writeln('I don''t see what there is to peek at, or into, or through.')
	END
    END;  {Peeek}




PROCEDURE PlugInCentrifuge;
{Plugs the centrifuge in.}
    VAR
	i,j: integer;
	r: place;
    BEGIN
    IF plugged THEN
	Writeln('It''s already plugged in.')
    ELSE BEGIN
	tellsitu := true;
	Writeln('With some difficulty, you force the plug into the Imperial Standard socket.');
	Writeln('The machine starts to spin rapidly.  You hear strange sounds from the cage...'); Writeln;
	Writeln('POP!  The gleeps vanish from the cage, reappearing everywhere at random!');
	map[yourroom].gleepses := map[yourroom].gleepses + 1;
	FOR i := 1 TO 80 + RandInt(40) DO BEGIN
	    j := 0;
	    REPEAT
		j := j + 1;
		r := Distant(nowhere)
	    UNTIL (j >= gleepclumping) OR (map[r].gleepses > 0);
	    IF (env <> ghost) AND (env <> ghostbunker) AND (env <> bunker) THEN
		map[r].gleepses := map[r].gleepses + 1
	    END;
	plugged := true
	END
    END;  {PlugInCentrifuge}




PROCEDURE PlugItIn;
{Plugs something in, if possible.}
    BEGIN
    tellsitu := false;
    REPEAT
	ReadWord(verb)
    UNTIL (verb = nomeaning) OR (verb = min);
    IF verb = nomeaning THEN
	Writeln('I don''t understand what "plug" could mean here.')
    ELSE
	IF env = nasa THEN
	    PlugInCentrifuge
	ELSE
	    Writeln('I don''t see anything you can plug in.')
    END;  {PlugItIn}




PROCEDURE Ignite;
{Lights the match}
    BEGIN
    IF Have(omatches,true) THEN BEGIN
	location[omatches] := nowhere;
	IF (env = kerosene) OR (dumproom = yourroom) THEN BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('KA-BLAAAAAAAMMM!   The highly flammable vapor explodes and kills you.');
	    wayended := killed
	    END
	ELSE
	    IF env = garden THEN BEGIN
		Writeln('The dry plants catch fire, burning to ashes in a moment!');
		burnt := true;
		map[yourroom].features := burntplants;
		env := burntplants;
		IF location[onitro] = yourroom THEN
		    Blast(true);
		IF Have(onitro,true) THEN BEGIN
		    wayended := killed;
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
			Writeln('Unfortunately...  BLAMM!!  Your nitro detonates, and blows you to bits!')
			END
		    END
		ELSE
		    Writeln('Your single match burns brightly for a moment, and then goes out.')
	    END
	ELSE BEGIN
	    tellsitu := false;
	    Writeln('You have nothing that you can light.')
	    END
    END;  {Ignite}




PROCEDURE Attack;
{Attacks a being or whatever is alive in this room}
    VAR
	victim: meaning;
    BEGIN
    ReadWord(victim);
    IF NOT (victim IN [fscalything,fgorilla,fvermin,flizards,fguard,frunt,
		       fplants,fghost,frats,fbirds,fwumpus,fgnomes,fflies,fbeast]) THEN
	IF Here(fguard) AND guardalive THEN
	    victim := fguard
	ELSE
	    IF Here(frunt) THEN
		victim := frunt
	    ELSE
		IF Here(fgorilla) AND goralive THEN
		    victim := fgorilla
		ELSE
		    IF attached THEN
			victim := fscalything
		    ELSE
			IF beastfollowing THEN
			    victim := fbeast
			ELSE
			    IF flies > 0 THEN
				victim := fflies
			    ELSE
				IF env IN [rat,ghost,pillpile,lizardhole,garden,freebirds,lobby,wumpus] THEN
				    CASE env OF
					rat: victim := frats;
					ghost: victim := fghost;
					pillpile: victim := fvermin;
					lizardhole: victim := flizards;
					garden: victim := fplants;
					freebirds: victim := fbirds;
					lobby: victim := fgnomes;
					wumpus: victim := fwumpus
					END
				ELSE
				    victim := nomeaning;
    IF victim = nomeaning THEN
	IF env IN [arcturus,fogarc,vent,machine,peekhole,picturetube] THEN BEGIN
	    tellsitu := false;
	    CASE env OF
		arcturus,fogarc,vent: Writeln('I''ll be glad to, if you''ll tell me HOW.');
		machine: Writeln('The Lugiman in coveralls is too far away.');
		peekhole:
		    IF peeked THEN BEGIN
			Writeln('You start to attack, but you''re not fast enough...');
			tellsitu := true
			END
		    ELSE
			Writeln('There''s nothing here you can kill.');
		picturetube: Writeln('You can''t get through the glass.')
		END
	    END
	ELSE BEGIN
	    tellsitu := false;
	    Writeln('There''s nothing here you can kill.')
	    END
    ELSE
	IF Here(victim) THEN
	    CASE victim OF
		fscalything,fplants: BEGIN
		    Writeln('I''ll be glad to, if you''ll tell me HOW.');
		    tellsitu := false
		    END;
		fgorilla:
		    IF goralive THEN
			IF strong THEN BEGIN
			    goralive := false;
			    Writeln('You advance to the gorilla, and throw a karate punch at it''s face.  Your');
			    Writeln('tremendous strength smashes the gorilla''s skull.  It falls dead.')
			    END
			ELSE
			    Writeln('You attack the gorilla fiercely, and it effortlessly brushes you aside.')
		    ELSE BEGIN
			Writeln('The gorilla is already dead.');
			tellsitu := false
			END;
		fvermin: Writeln('You stomp on a digging critter and kill it.  Another one drags the body away.');
		flizards: Writeln('The lizards easily evade you.');
		fguard:
		    IF guardalive THEN
			IF Have(ohammer,true) OR strong THEN BEGIN
			    IF strong THEN BEGIN
				Writeln('Confident in your newfound strength, you attach the guard fiercely.  Before');
				Writeln('you know it, he''s dead on the floor!  A small, delicate glass bottle rolls');
				Writeln('out of his pocket.')
				END
			    ELSE BEGIN
				Writeln('You attack insanely with your sledgehammer, and suddenly the Lugiman is');
				Writeln('dead on the floor!  A small, delicate glass bottle rolls out of his pocket.')
				END;
			    guardalive := false;
			    location[ofluid] := yourroom
			    END
			ELSE BEGIN
			    guardsees := true;
			    Writeln('Your attack is futile; you don''t have a decent weapon!')
			    END
		    ELSE BEGIN
			tellsitu := false;
			Writeln('The guard is already dead.')
			END;
		frunt: BEGIN
		    Writeln('You move in to attack.  The unhealthy Lugonian runt shrieks something');
		    Writeln('about Earth germs and runs away in cowardice.');
		    runtroom := Distant(yourroom)
		    END;
		fghost: BEGIN
		    Writeln('You can''t get a grip on the ghost.  Besides, it''s already dead.');
		    tellsitu := false
		    END;
		frats: Writeln('Some of the rats are now dead.  Congratulations.');
		fbirds: BEGIN
		    Writeln('The birds never come down where you can reach.');
		    tellsitu := false
		    END;
		fgnomes: BEGIN
		    Writeln('You stride toward the nearest gnome and kill it with one punch.  Another');
		    Writeln('gnome you hadn''t noticed gives a joyful shriek and dashes towards you.  It');
		    Writeln('sits down in front of the vacated glowing box, grunting with satisfaction, and');
		    Writeln('begins fiddling with it mysteriously.')
		    END;
		fflies: Writeln('These Lugonian flies dodge much better than Earth flies.');
		fbeast: BEGIN
		    Writeln('You wrap your fingers around the huge doglike beast''s throat and try');
		    Writeln('to strangle it.'); Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('It fights back desperately, and tears your guts out.');
		    wayended := killed
		    END
		END
	ELSE BEGIN
	    Write('I don''t see any ');
	    SayName(victim,false);
	    Writeln('...');
	    tellsitu := false
	    END
    END;  {Attack}




PROCEDURE OpenIt;
{This procedure carries out the "open" command.  If an object name follows }
{it, it will try to open that object.  Otherwise, it tries to open whatever}
{permanent feature of the room is openable.				   }
    VAR
	success: boolean;
	object: meaning;
    BEGIN
    tellsitu := false;
    ReadWord(object);
    IF object = mit THEN
	object := it;
    IF object = mall THEN
	Writeln('I can only open one thing at a time.')
    ELSE
	IF ((object >= orope) AND (object <= fcentrifuge)) OR (object = mgleeps) THEN BEGIN
	    ImplicitTake(object,true,false,success);
	    IF success THEN BEGIN
		tellsitu := true;
		IF (object = oflask) AND (location[ochemicals] = inflask) THEN
		    Fumes
		ELSE
		    IF (object = oflask) OR (object = obag) THEN
			Writeln('It''s already open.')
		    ELSE
			IF object = ofluid THEN BEGIN
			    Writeln('As the seal cracks open, a powerful smell fills the room.  Suddenly,');
			    Writeln('you recall it''s nature...');
			    DropThisHere(ofluid,false)
			    END
			ELSE
			    IF object = oacetone THEN
				IF acetoneopen THEN
				    Writeln('It''s already open.')
				ELSE BEGIN
				    acetoneopen := true;
				    Writeln('You unscrew the metal cap, which is attached with a piece of plastic.');
				    Writeln('Your eyes seem to sting (acetone does that), but just for a moment.')
				    END
			    ELSE
				Writeln('It can''t be opened.')
		END
	    END
	ELSE BEGIN
	    tellsitu := (env = ghost) OR (env = ghostbunker) OR (env = car);
	    IF tellsitu OR (env IN [cabinet,tar,archive,lobby]) THEN
		CASE env OF
		    cabinet:
			IF cabinetopen THEN
			    Writeln('The cabinet''s already open.')
			ELSE BEGIN
			    tellsitu := true;
			    cabinetopen := true;
			    Writeln('   OK')
			    END;
		    car:
			IF trunkopen THEN
			    Writeln('Door: OK;   Trunk: already open.')
			ELSE
			    IF Have(okeys,true) THEN BEGIN
				trunkopen := true;
				Writeln('Door: OK;   Trunk: OK')
				END
			    ELSE BEGIN
				Writeln('Door: OK;   Trunk: You don''t have the keys.');
				tellsitu := false
				END;
		    ghost: Writeln('   OK');
		    tar: Writeln('The barrels are already open.');
		    archive: Writeln('You can''t get the vacuum chamber open.');
		    lobby: Writeln('The "boxes" are funny machines with glowing fronts, not things with lids.')
		    END
	    ELSE
		Writeln('There''s nothing in the room to open.')
	    END
    END;  {Openit}




PROCEDURE Moov(direct: meaning);
{This moves the player into an adjoining room, in one of the directions }
{vnorth, vsouth, veast, vwest, vjump, or vclimb.  If (direct) = vforward}
{or vbackward, it refers to (lastdir).  If successful, (yourroom) :=    }
{(map[yourroom].paths[direct]).						}
    CONST
	meadowgrab = 2;
    BEGIN
    tellsitu := false;
    IF goralive AND Here(fgorilla) AND (direct <> vjump) AND NOT strong THEN
	Writeln('The gorilla blocks your way!')
    ELSE
	IF (env = balcony) AND (direct = vjump) THEN
	    wayended := jumped
	ELSE
	    IF env <= fogarc THEN
		IF strong THEN BEGIN
		    Writeln('You rend through the metal bars with irresistable strength.  The tearing');
		    Writeln('metal emits piercing shrieks...');
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('...and the guards fire their weapons, reducing your flesh to a grease spot.');
		    wayended := killed
		    END
		ELSE
		    Writeln('...No way through!')
	    ELSE
		IF (map[yourroom].paths[direct] = nowhere) AND ((env <> meadow) OR (direct > vwest)) THEN
		    Writeln('...No way through!')
		ELSE
		    IF NOT Have(orope,true) AND (direct = vclimb) AND NOT ((env = lobby) OR (env = drydock)) AND NOT strong THEN
			Writeln('You can''t climb without a rope.')
		    ELSE BEGIN
			tellsitu := true;
			lastmove := direct;
			IF (direct <= vwest) AND (env = meadow) AND ((map[yourroom].paths[direct] = nowhere) OR (RandInt(meadowgrab) > 1)) THEN
			    Writeln('   OK')
			ELSE BEGIN
			    IF (env = lobby) AND (verb = vclimb) THEN
				Writeln('You walk up the stairs...')
			    ELSE
				IF (env = drydock) AND (direct = vclimb) THEN
				    Writeln('You climb the structure and enter the ship.  It''s clearly no ordinary ship...')
				ELSE
				    IF goralive AND Here(fgorilla) AND (direct <> vjump) THEN
					Writeln('The gorilla blocks your way, and you calmly push it aside.')
				    ELSE
					IF verb <= vbackward THEN
					    Writeln('   OK');
			    IF (direct = vclimb) AND Have(ohammer,true) AND (env <> lobby) AND NOT strong THEN BEGIN
				Writeln;
				Writeln('As you climb, the heavy hammer slips from your grasp!');
				verb := vdrop;
				DropThisHere(ohammer,true)
				END;
			    yourroom := map[yourroom].paths[direct];
			    env := map[yourroom].features;
			    peeked := false;
			    IF NOT (env IN [balcony,kerosene,meadow]) AND (fogtime <= Timer) THEN
				env := fogged
			    END
			END
    END;  {Moov}




PROCEDURE Enter;
{This procedure attempts to enter whatever is enterable.}
    BEGIN
    tellsitu := env IN [ghost,ghostbunker,car,nasa,drydock,lobby];
    IF (env = cabinet) OR (env = archive) OR tellsitu THEN
	CASE env OF
	    ghost: env := ghostbunker;
	    ghostbunker: env := ghost;
	    cabinet: Writeln('You don''t fit in the cabinet.');
	    car: Writeln('You find that the seats are comfy.');
	    nasa: NasaDeath;
	    drydock: Moov(vclimb);
	    archive: Writeln('It''s not smart to enter a vacuum chamber.');
	    lobby: BEGIN
		Moov(vclimb);
		Writeln('You enter an elevator, and it whisks you up THROUGH the roof...')
		END
	    END
    ELSE
	Writeln('There is nothing here you can enter.')
    END;  {Enter}




PROCEDURE Peruse;
{This procedure reads what is readable.}
    BEGIN
    IF env = library THEN BEGIN
	Writeln;
	Writeln('The writing consists of small circular arcs scattered over a fine rectangular');
	Writeln('grid.  There''s a fragment of a note stuck in one book:');
	Writeln('        "...could learn to harness their properties, these keys');
	Writeln('        could revolutionize every..."')
	END
    ELSE
	IF env = office THEN BEGIN
	    Writeln;
	    Writeln('MAN SETS FOOT ON MOON'); Writeln;
	    Writeln('USS MAINE SUNK BY TORPEDO'); Writeln;
	    Writeln('BROOKLYN DODGERS PENNANT WINNERS'); Writeln;
	    Writeln('SOVIETS EXPLODE ATOMIC BOMB')
	    END
	ELSE
	    IF env = mensroom THEN BEGIN
		Writeln;
		Writeln('You attempt to translate from the Lugonian:');
		Writeln('       There was once an explorer [lit. conqueror] named [ "PF''isQ" ]');
		Writeln('       Whose [shoulders? hips? knees?] were exceedingly brisk.');
		Writeln('       So swift was his action');
		Writeln('       That [? "Ra!oyguo", prob. scientific term] contraction');
		Writeln('       Diminished his [? Ancient High Tongue: cattleprod] to a disk.')
		END
	    ELSE
		IF env = lobby THEN
		    Writeln('The glowing words on the boxes say "[DECsystem-20 continued]".')
		ELSE BEGIN
		    IF env IN [sea,bubble,airlock,machine] THEN
			Writeln('There is nothing here to read except the sign.')
		    ELSE
			Writeln('There is nothing here to read.');
		    tellsitu := false
		    END
    END;  {Peruse}




PROCEDURE ShutIt;
{This shuts what is open here.}
    VAR
	object: meaning;  {what, if anything, to shut}
    BEGIN
    ReadWord(object);
    IF object = mit THEN
	object := it;
    IF Here(object) THEN
	IF object = oacetone THEN
	    IF acetoneopen THEN BEGIN
		Writeln('   OK');
		acetoneopen := false
		END
	    ELSE
		Writeln('It''s already capped.')
	ELSE
	    IF object = oflask THEN
		IF location[ochemicals] = inflask THEN
		    Writeln('It''s already corked.')
    		ELSE
		    Writeln('You don''t have the cork.')
	    ELSE
		Writeln('That can''t be shut.')
    ELSE BEGIN
	tellsitu := (env = ghost) OR (env = ghostbunker) OR (env = car);
	IF (env = cabinet) OR (env = tar) OR tellsitu THEN
	    CASE env OF
		cabinet:
		    IF cabinetopen THEN BEGIN
			tellsitu := true;
			cabinetopen := false;
			Writeln('   OK')
			END
		    ELSE
			Writeln('The cabinet is already closed.');
		car:
		    IF NOT trunkopen THEN
			Writeln('Door: OK;   Trunk: already shut')
		    ELSE BEGIN
			Writeln('Door: OK;   Trunk: OK');
			trunkopen := false
			END;
		ghost: Writeln('   OK');
		tar: Writeln('There are no lids for the barrels.')
		END
	    ELSE
		Writeln('There''s nothing here you can shut.')
	END
    END;  {ShutIt}




PROCEDURE Expectorate;
{Spits on the floor.}
    BEGIN
    IF Here(frunt) THEN BEGIN
	Writeln('You spit at the crazed Lugonian runt with the cudgel.  It hits him in the');
	Writeln('face.  He stops dead and emits a harsh, tearing scream!  He runs out of the');
	Writeln('room, shrieking in mortal agony.  The echoing cries are soon intermingled');
	Writeln('with harsh, racking coughs.  He''ll be no more trouble.');
	runtroom := nowhere;
	runtkilled := true
	END
    ELSE
	IF env = bubble THEN
	    Writeln('The spit runs down the side of the bubble into your shoe.')
	ELSE
	    IF (env = cistern) OR (env = tank) THEN
		Writeln('There is now a bit of foam on the water.')
	    ELSE
		IF env = balcony THEN
		    Writeln('The spit flies over the balcony and lands on a pedestrian''s shoulder.')
		ELSE
		    IF (env = ghost) OR (env = ghostbunker) THEN
			Writeln('The spit disappears in a puff of smoke.')
		    ELSE
			Writeln('There is now some spit on the floor.')
    END;  {Expectorate}




PROCEDURE PourIt;
{Takes care of "pour <object>".}
    VAR
	object: meaning;
	success: boolean;
    BEGIN
    ReadWord(object);
    tellsitu := false;
    IF object = mit THEN
	object := it;
    IF ((object >= orope) AND (object <= fcentrifuge)) OR (object = mgleeps) THEN BEGIN
	IF ((object = owater) OR (object = ochemicals) OR (object = ourine)) AND Have(object,true) THEN
	    object := oflask;
	ImplicitTake(object,true,false,success);
	IF success THEN
	    IF object IN [oflask,ofluid,oacetone] THEN
		CASE object OF
		    oflask:
			IF (location[owater] <> inflask) AND (location[ochemicals] <> inflask) 
					AND (location[ourine] <> inflask) THEN
			    Writeln('There is nothing in it to pour out.')
			ELSE BEGIN
			    tellsitu := true;
			    IF location[owater] = inflask THEN BEGIN
				IF (env = cistern) OR (env = tank) OR (env = seep) THEN
				    Writeln('You dump the water out.')
				ELSE
				    Writeln('You dump the water out into a puddle, which starts to dry.');
				location[owater] := nowhere
				END
			    ELSE
				IF location[ourine] = inflask THEN BEGIN
				    IF (env = seep) OR (env = cistern) OR (env = tank) THEN
					Writeln('You dump the urine out.')
				    ELSE
					Writeln('You dump the urine out into a puddle, which starts to dry.');
				    location[ourine] := nowhere
				    END
				ELSE
				    Fumes
			    END;
		    ofluid: BEGIN
			Writeln('As the seal cracks open, a powerful smell fills the room.  Suddenly,');
			Writeln('you recall it''s nature...');
			DropThisHere(ofluid,false)
			END;
		    oacetone:
			IF acetoneopen THEN
			    DropThisHere(oacetone,false)
			ELSE
			    Writeln('You have to open it first.')
		    END
	    ELSE
		Writeln('That object can''t be poured.')
	END
    ELSE
	IF object = mall THEN
	    Writeln('You can only pour one thing at a time.')
	ELSE
	    Writeln('I don''t understand what you want to pour.')
    END;  {PourIt}




PROCEDURE PushButton;
{This pushes a button or flips a switch, if any are here.}
    BEGIN
    IF (env = buzzcr) OR (env <= fogarc) THEN BEGIN
	 IF env <= fogarc THEN
	     env := map[yourroom].features
	 ELSE
	     env := arcturus;
	 Writeln('There is a dazzling flash of light, and a sudden wrenching discontinuity as');
	 Writeln('the floor seems to heave madly.  You recover in moments...');
	 END
     ELSE
	 IF env = centralcr THEN BEGIN
	     Writeln('The great viewscreen glows to life...  It shows a monstrous, bloated Lugonian');
	     Writeln('face--  which you recognize as the Grugza Emperor Ra-Lugi himself, absolute');
	     Writeln('ruler of the Lugimen!  He sees you!  He screams in rage and brings up a huge');
	     Writeln('weapon!  He fires at you...'); Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	     Writeln('...and a searing beam lashes out of the weapon''s image and burns you');
	     Writeln('to a crisp where you stand.');
	     wayended := killed
	     END
	 ELSE BEGIN
	     Writeln('I don''t see any buttons or switches here...');
	     tellsitu := false
	     END
    END;  {PushButton}




PROCEDURE Leave;
{This procedure is called when the player gives the command 'leave'.  He }
{succeeds in getting out of the building if: he is on the balcony, he has}
{the rope, and the scaly thing is not on his leg.  It calls Enter when   }
{trapped in the ghost room or ghost bunker.				 }
    BEGIN
    tellsitu := false;
    IF env <> balcony THEN
	IF (env = ghost) OR (env = ghostbunker) THEN
	    Enter
	ELSE
	    Writeln('How?')
    ELSE
	IF Have(orope,true) THEN
	    IF attached THEN BEGIN
		Writeln;
		Writeln('The people see the horrid thing on your leg, and throw you back!')
		END
	    ELSE BEGIN
		wayended := escaped;
		Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		Writeln('You climb down the rope to freedom!');
		IF beastfollowing THEN
		    Writeln('The Emperor''s ugly animal, with surprising nimbleness, climbs down after you.')
		END
	ELSE
	    IF strong THEN
		wayended := jumped
	    ELSE BEGIN
		Writeln;
		Writeln('It''s five floors down, and you don''t have a rope!'); Writeln;
		Writeln('What you wanna do, Lou?')
		END
    END;  {Leave}




PROCEDURE StartIt;
{Starts any machinery lying around.}
    BEGIN
    IF env = car THEN
	IF Have(okeys,true) THEN
	    IF location[onitro] = intrunk THEN BEGIN
		Writeln('GrrrRRRrrrRRRrrr--VARROOM!   The car lurches sharply forward...');
		Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		Writeln('KA-BLLAAAMMMMM!!!  Something in the trunk explodes like a bomb, killing you.');
		wayended := killed
		END
	    ELSE BEGIN
		Writeln('GrrrRRRrrrRRRrrr--VARROOM!  The car lurches forward...');
		Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		Writeln('You crash through the window and onto the street!  You''ve escaped!');
		wayended := escaped
		END
	ELSE
	    Writeln('You don''t have the keys.')
    ELSE BEGIN
	tellsitu := false;
	IF env = nasa THEN
	    PlugInCentrifuge
	ELSE
	    Writeln('I don''t see anything startable here.')
	END
    END;  {StartIt}




PROCEDURE Update(peekroom,plugroom: place);
{Brings the player's situation up to date, in all kindsa ways.}
{(peekroom) is the place where the player peeked at the mating}
{Lugimen.  (plugroom) is the place where the player plugged in}
{the centrifuge.					      }
    CONST
	gleepfreq = 2;  {1 / probability of gleeps appearing somewhere}
	robfreq = 40;  {1 / probability of gleeps vanishing from pocket}
	snatchfreq = 40;  {1 / probability of skull being grabbed}
	odfreq = 4;  {1 / probability of single pill dosage being an overdose}
	safetime = 5;  {time allowed in QUICK rooms}
	fogfadetime = 50;  {time it takes for fog to dissipate}
	sandperiod = 50;  {time it takes for plague to get worse}
	strongtime = 360;  {time super strength lasts}
	pillwait = 25;  {time it takes for pills to take effect}
    VAR
	currentweightlimit: integer;
	m: meaning;


    PROCEDURE UpdateGleeps;  {inside Update}
    {Does that updating which is concerned with gleeps.}
	VAR
	    i: integer;
	    p: place;
	BEGIN
	IF RandInt(gleepfreq) = 1 THEN BEGIN
	    i := 0;
	    REPEAT
		i := i + 1;
		p := Distant(nowhere)
	    UNTIL (i > gleepclumping) OR (map[p].gleepses > 0);
	    IF ((RandInt(4 * gleepfreq) = 1) OR (map[p].gleepses > 0)) AND
					(map[p].features <> ghost) AND (map[p].features <> ghostbunker) THEN BEGIN
		map[p].gleepses := map[p].gleepses + 1;
		IF (p = yourroom) AND (env > fogged) AND (env <> kerosene) THEN BEGIN
		    Writeln;
		    Writeln('POP!  A gleep just appeared here.')
		    END
		END
	    END;
	IF (RandInt(robfreq) = 1) AND (gleeps > 5) THEN BEGIN
	    i := gleeps DIV 8 + 2;
	    Writeln;
	    Writeln('POP!  ',i: 1,' gleeps just vanished from your pocket.');
	    gleeps := gleeps - i
	    END
	END;  {UpdateGleeps}


    PROCEDURE UpdateSafeStuff;
    {Updates stuff that can't kill you.}
	BEGIN
	IF Have(oskull,false) AND (env = ghost) THEN BEGIN
	    map[yourroom].features := scaredghost;
	    env := scaredghost;
	    Writeln;
	    Writeln('There is a deathly cold shriek: "EEEEEEEEEEEEEEEEEEEKKK!!  A skull!!!!"');
	    FOR m := vnorth TO vclimb DO
		map[yourroom].paths[m] := Distant(yourroom);
	    map[yourroom].paths[RandDirection(vwest)] := Succ(yourroom)
	    END;
	IF (venttime <= Timer) AND (env <> vent) THEN BEGIN
	    IF Timer - venttime >= safetime THEN BEGIN
		starttime := starttime - timeloss;
		Writeln(Chr(7),'Too late, clod!  One of them spotted you, and a troop of guards quickly');
		Writeln('got up and headed for the exit!  You''ve probably lost valuable time!');
		END
	    ELSE
		Writeln('They didn''t see you.');
	    venttime := maxint
	    END;
	IF yourroom = thingroom THEN BEGIN
	    attached := true;
	    thingroom := nowhere;
	    Writeln;
	    Writeln('A small scaly thing lunges out and attaches itself to your leg!')
	    END;
	IF (env = fogged) AND (Timer - fogtime >= fogfadetime) THEN BEGIN
	    Writeln;
	    Writeln('The fog has gradually been dissipating, and now you can see through it.');
	    fogtime := maxint;
	    env := map[yourroom].features
	    END;
	IF (env = fogarc) AND (Timer - thrfogtime >= fogfadetime) THEN BEGIN
	    Writeln;
	    Writeln('The fog has gradually been dissipating, and now the guards can see you!');
	    env := arcturus
	    END;
	IF Have(oskull,false) AND (RandInt(snatchfreq) = 1) THEN BEGIN
	    Writeln;
	    Writeln('A small furry creature like a monkey just charged through the room,');
	    Writeln('snatched your skull, and ran away, making a noise like a hyena.');
	    location[oskull] := nowhere
	    END
	END;  {UpdateSafeStuff}


    PROCEDURE UpdatePills;  {inside Update}
    {Updates things related to the pills.}
	BEGIN
	IF (Timer - pilltime > strongtime) AND strong THEN BEGIN
	    strong := false;
	    Writeln;
	    Writeln(Chr(7),'Oh no, your super strength is wearing off!');
	    verb := vdrop;
	    pilltime := maxint
	    END;
	IF (wayended = notdone) AND (Timer - pilltime > pillwait) AND (overdose OR NOT strong) THEN
	    IF (RandInt(odfreq) > 1) AND NOT overdose THEN BEGIN
		strong := true;
		Writeln;
		Writeln('The pills are taking effect...  A sudden power courses through your body!');
		Writeln('You have gained three times your normal strength!')
		END
	    ELSE BEGIN
		Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		Writeln('BANG!  Your skull just exploded (I think).  Those pills must have taken');
		Writeln('effect.  The Lugimen chuck your poisoned body into the incinerator.');
		wayended := killed
		END
	END;  {UpdatePills}


    PROCEDURE UpdateSickness;  {inside Update}
    {Does that updating concerned with the Lugonian plague.}
	BEGIN
	IF (sandtime < Timer) AND NOT (Have(osandwich,true) AND badsandwich) THEN BEGIN
	    sandtime := maxint;
	    sickness := none
	    END;
	IF (Timer - sandtime > sandperiod) AND (sickness < critical) THEN BEGIN
	    sickness := Succ(sickness);
	    sandtime := Timer;
	    Writeln;
	    CASE sickness OF
		mild: Writeln('You are feeling a little queasy and weak, as if you had the flu.');
		severe: Writeln('You feel sick and dizzy, and it is becoming hard to go on.');
		critical: BEGIN
		    Writeln('You are reeling and staggering, your head aches and feels like it''s floating,');
		    Writeln('and you are starting to hallucinate.  Your training tells you that you clearly');
		    Writeln('have the Lugonian plague, and you have very little time left.')
		    END
		END;
	    Writeln
	    END;
	IF (sickness = critical) AND (Timer - sandtime > sandperiod DIV 2) AND (wayended = notdone) THEN BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('You have died of the Lugonian plague.');
	    wayended := killed
	    END
	END;  {UpdateSickness}


    PROCEDURE UpdateGarden;  {inside Update}
    {Does that updating related to the garden of man-eating plants.}
	BEGIN
	IF (env = garden) AND (lastenv = garden) THEN
	    IF plantslunged THEN BEGIN
		Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		Writeln('Gosh, the plants ate you.');
		wayended := eaten
		END
	    ELSE BEGIN
		Writeln;
		Writeln('The plants lunge toward you!  Move fast, Joe, or they''ll get you!');
		plantslunged := true
		END
	ELSE
	    plantslunged := false;
	IF (env = fogged) AND NOT burnt AND (map[yourroom].features = garden) AND (wayended = notdone) THEN BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('You have stumbled into a garden of dry, brown, hungry man-eating');
	    Writeln('plants, which immediately eat you.');
	    wayended := eaten
	    END
	END;  {UpdateGarden}


    PROCEDURE Hurry;  {inside Update}
    {This procedure gives messages to the player hinting at what portion of}
    {his time has elapsed.  If enough has elapsed, it kills him.	   }
	VAR
	    onepercent,timnow: integer;  {in seconds: one percent of the total time allowed / time since game started}
	BEGIN
	timnow := (Timer - starttime);
	onepercent := (shortgametime + Ord(duration) * (shortgametime DIV 2)) DIV 100;
	IF timnow > 95 * onepercent THEN BEGIN
	    Writeln;
	    Writeln('Tromp...Tromp...Tromp...')
	    END
	ELSE
	    IF (timnow > 85 * onepercent) AND (lastwarning < cries) THEN BEGIN
		lastwarning := cries;
		Writeln;
		Writeln(Chr(7),'I hear the sounds of distant cries and booted feet!')
		END
	    ELSE
		IF (timnow > 67 * onepercent) AND (lastwarning < detected) THEN BEGIN
		    lastwarning := detected;
		    Writeln;
		    Writeln(Chr(7),'The Lugonian detectors have picked you up!  Time is growing short!')
		    END
		ELSE
		    IF (timnow > 33 * onepercent) AND (lastwarning < ontrail) THEN BEGIN
			lastwarning := ontrail;
			Writeln;
			Writeln('I think the Lugimen are on your trail now, Jack...')
			END;
	IF timnow > 100 * onepercent THEN BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('"Aaaaaaaaahhhhh, we found us some dinner!"    (chomp, slobber, crunch...)');
	    wayended := eaten
	    END
	END;  {Hurry}


    BEGIN  {Update}
    UpdateGleeps;
    UpdateSafeStuff;
    UpdatePills;
    currentweightlimit := (weightlimit - 2 * Ord(sickness)) * (1 + 2 * Ord(strong));
    IF Weight > currentweightlimit THEN BEGIN
	Writeln;
	Writeln('You aren''t strong enough to carry all this stuff anymore-- you drop stuff...')
	END;
    IF (Weight > currentweightlimit) AND Have(ohammer,true) THEN
	DropThisHere(ohammer,true);
    IF (Weight > currentweightlimit) AND Have(orope,true) THEN
	DropThisHere(orope,true);
    IF (Weight > currentweightlimit) AND Have(oskull,true) THEN
	DropThisHere(oskull,true);
    IF (Weight > currentweightlimit) AND Have(olysol,true) THEN
	DropThisHere(olysol,true);
    IF (Weight > currentweightlimit) AND Have(oflysol,true) THEN
	DropThisHere(oflysol,true);
    IF (Weight > currentweightlimit) AND Have(oraid,true) THEN
	DropThisHere(oraid,true);
    IF (Weight > currentweightlimit) AND Have(ostatuette,true) THEN
	DropThisHere(ostatuette,true);
    IF (Weight > currentweightlimit) AND Have(osandwich,true) THEN
	DropThisHere(osandwich,true);
    IF (Weight > currentweightlimit) AND Have(obag,true) THEN
	DropThisHere(obag,true);
    IF (yourroom = peekroom) AND (wayended = notdone) THEN BEGIN
	wayended := killed;
	Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	Writeln('The maddened Lugiman suddenly barges around the corner and meets you');
	Writeln('face to face!  Screaming in rage at your violation of his privacy, he');
	Writeln('raises the ancient ceremonial weapon, decorated with carvings and forgotten');
	Writeln('writings, and blows your brains out.');
	END;
    IF (lastenv = arcturus) AND (env = arcturus) AND (wayended = notdone) THEN BEGIN
        wayended := killed;
	Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	Writeln('The Emperor''s guards fire their weapons, converting your');
	Writeln('body into a grease spot.')
	END;
    IF (env = fogarc) AND (lastenv <= fogarc) AND (wayended = notdone)
			AND Have(oacetone,true) AND acetoneopen AND NOT beastfollowing THEN BEGIN
	Writeln;
	Writeln('The big ugly beast that was sitting by the Emperor''s feet squeezes itself');
	Writeln('amazingly into the cage with you.  It must have bones of rubber.  It sniffs');
	Writeln('your acetone can eagerly.');
	beastfollowing := true
	END;
    IF (wayended = notdone) AND (plugroom > nowhere) THEN
	IF yourroom = plugroom THEN BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('With a tremendous crash, the centrifuge flies into fragments.  A piece');
	    Writeln('hits you in the temple and kills you on the spot.');
	    wayended := killed
	    END
	ELSE BEGIN
	    IF env > fogarc THEN BEGIN
		Writeln;
		Writeln('A tremendous crash reverberates through the building...')
		END;
	    map[plugroom].features := nasawreck;
	    plugged := false
	    END;
    IF wayended = notdone THEN
	IF (dumproom > nowhere) AND ((env <= fogarc) OR (yourroom <> dumproom)) AND (Timer - dumptime < safetime) THEN BEGIN
	    dumproom := nowhere;
	    IF env > fogarc THEN BEGIN
		Writeln;
		Writeln('Behind you are the sounds of a large animal slurping up liquid.')
		END
	    END
	ELSE
	    IF (dumproom > nowhere) AND (dumptime <= Timer) THEN BEGIN
		wayended := killed;
		Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		Writeln('You were too late!     *  *  *  *      Your legs weaken...');
		Writeln('         Conciousness fades, and you hear no more.')
		END
	    ELSE
		IF (dumproom = yourroom) THEN BEGIN
		    Writeln;
		    Writeln('You''d better get out QUICK, or the acetone fumes will knock you out!')
		    END;
    IF wayended = notdone THEN
	UpdateSickness;
    IF guardalive AND Here(fguard) AND (lastenv = env) AND (wayended = notdone) THEN
	IF guardsees THEN BEGIN
	    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	    Writeln('The guard sets his heat gun on medium rare and cooks you for dinner.');
	    wayended := eaten
	    END
	ELSE BEGIN
	    Writeln;
	    Writeln('The guard sees you!  He hits the and draws his weapon!',Chr(7));
	    Writeln('You''ve lost valuable time!');
	    guardsees := true;
	    starttime := starttime - timeloss
	    END
    ELSE
	guardsees := false;
    IF Here(frunt) AND (lastenv = env) AND (wayended = notdone) THEN BEGIN
	Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
	Writeln('The unhealthy Lugiman raises his cudgel and bashes your brains out, while');
	Writeln('standing as far back as he can manage and still yelling about germs.');
	wayended := killed
	END;
    IF wayended = notdone THEN
	UpdateGarden;
    IF (wayended = notdone) AND (env > fogged) THEN
	Hurry
    END;  {Update}




PROCEDURE HearAndObey;
{Carries out the player's command, and updates the situation.}
    VAR
	c: char;
	peekroom,plugroom: place;  {where player peeked / where player plugged centrifuge in}
    {(actually, everything from ReadWord down belongs right here, but who cares...)}
    BEGIN
    IF peeked THEN
	peekroom := yourroom
    ELSE
	peekroom := nowhere;
    IF plugged THEN
	plugroom := yourroom
    ELSE
	plugroom := nowhere;
    ReadWord(verb);
    IF verb = min THEN
	verb := venter;
    IF verb = ourine THEN
	verb := vpiss;
    Writeln;
    tellsitu := (verb >= vnorth) AND (verb <= mr);
    IF NOT tellsitu THEN
	IF verb > nomeaning THEN
	    IF ((verb >= orope) AND (verb <= fcentrifuge)) OR (verb = mgleeps) THEN
		Writeln('I don''t understand what you want to do with it.')
	    ELSE
		Writeln('I don''t understand.')
	ELSE
	    Writeln('Whaat??')
    ELSE
	CASE verb OF
	    vnorth,vsouth,veast,vwest,vjump,vclimb: Moov(verb);
	    vforward: Moov(lastmove);
	    vbackward: Moov(Opposite(lastmove));
	    vtake: TakeThem;
	    vput: PutThem;
	    vdrop,vthrow: DropIt;
	    vinventory: BEGIN
		Writeln('You are carrying:');
		Inventory(pockets);
		IF attached THEN BEGIN
		    Writeln;
		    Writeln('There''s a scaly thing on your leg.')
		    END;
		IF beastfollowing THEN BEGIN
		    Writeln;
		    Writeln('A huge ugly beast is following you around.')
		    END;
		IF fungus THEN BEGIN
		    Writeln;
		    Writeln('There is a patch of irritating fungus on your arm.')
		    END;
		IF beenin > 1 THEN BEGIN
		    Writeln;
		    Writeln('(You have explored ',beenin: 1,' rooms.)')
		    END
		END;
	    vignite: Ignite;
	    vkill: Attack;
	    vopen: OpenIt;
	    veat: EatIt;
	    venter: Enter;
	    vout: Leave;
	    vhelp: BEGIN
		Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		Writeln('The Lugimen help themselves to those who need help!');
		wayended := eaten
		END;
	    vread: Peruse;
	    vshut: ShutIt;
	    vspit: Expectorate;
	    vrun: Writeln('Which direction?');
	    vpour: PourIt;
	    vbutton: PushButton;
	    vspray: SprayIt;
	    vshit: Defecate;
	    vpiss: Urinate;
	    vfart:
		IF env <= fogged THEN BEGIN
		    tellsitu := false;
		    Writeln('You can''t summon up a fart right now.')
		    END
		ELSE
		    IF (env = balcony) OR (env = meadow) THEN
			Writeln('Pffffffffpppppttt!!  You fart with conviction.  There is now a bad smell.')
		    ELSE BEGIN
			Writeln('Pfffffffffpppppppttt!!  You fart with ghusto.  The fart gases react with');
			Writeln('the semi-alien air, forming a brown fog you can''t see two inches through.');
			IF NOT (env IN [balcony,kerosene,meadow]) THEN
			    env := fogged;
			fogtime := Timer
			END;
	    vfuck: Writeln('No one around here has the correct organs and/or orifices.');
	    vflush:
		IF env = bathroom THEN
		    Writeln('Flub, gisssshhhhh--aa ga ga gurgurGABLOOOPP! hhhhhsssssssss....')
		ELSE BEGIN
		    tellsitu := false;
		    IF env = mensroom THEN
			Writeln('You push the rusty levers, and nothing happens.')
		    ELSE
			Writeln('I don''t see any toilet here...')
		    END;
	    vpeek: Peeek(peekroom,peeked);
	    vplug: PlugItIn;
	    vunplug:
		IF plugroom = yourroom THEN
		    Writeln('Ooooff, it''s stuck!  Oh Christ....')
		ELSE BEGIN
		    tellsitu := false;
		    Writeln('I don''t see anything plugged in here...')
		    END;
	    vstart: StartIt;
	    vfind: Writeln('You''ll have to do that yourself.');
	    vlook:
		IF env = peekhole THEN
		   Peeek(peekroom,peeked)
		ELSE
		    Writeln('Sorry, I ain''t gonna say more than this:');
	    vbust: Writeln('Your concience stops you.');
	    vquit: BEGIN
		REPEAT
		    Writeln('Do you really want to stop the game?');
		    ReadLetter(c)
		UNTIL (c = 'y') OR (c = 'n');
		IF c = 'y' THEN BEGIN
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('You sit on the floor until the Lugimen come.');
		    wayended := eaten
		    END
		END;
	    vdetonate:
		IF Have(onitro,true) THEN BEGIN
		    Writeln('You give the nitro a good shaking, and...');
		    Writeln; Writeln; Writeln; Writeln; Writeln; Writeln; Writeln;
		    Writeln('BLAAAMMMM!!!  --blow yourself to bits.');
		    wayended := killed
		    END
		ELSE BEGIN
		    tellsitu := false;
		    Writeln('You don''t have anything that will detonate.')
		    END;
	    mr: tellsitu := false  {makes remainder of command a comment}
	    END;
    IF (verb = vrun) OR (verb = vfuck) OR (verb = vfind) OR (verb = vbust) OR (verb = vquit) THEN  {**** set is better}
	tellsitu := false;
    IF plugroom > nowhere THEN
	plugged := false;
    IF tellsitu THEN
	Update(peekroom,plugroom);
    IF NOT tellsitu AND (wayended = notdone) AND (verb <> nomeaning) AND (verb <> vout)
			AND (verb <> vrun) AND (verb <> mr) AND (verb <> mnot) THEN BEGIN  {**** set is better}
	Writeln;
	Writeln('What you wanna do, Lou?')
	END
    END;  {HearAndObey}




PROCEDURE Points;
{This procedure tells the player his score when the game is over.  It also }
{compares that score with the all-time top ten scores, and adds it in if   }
{it's high enough and the player doesn't have a higher score there already.}
    VAR
	score,obct,i: integer;  {player's total score / indexes}
	ob: meaning;


    PROCEDURE UpdateTopTen(score: integer);  {inside Points}
    {**** Checks whether the player's score belongs in the top ten.  If so,}
    {puts it in and shows them.  Otherwise, offers to show them.           }
	CONST
	    hisource = 'SRC:<GAMES>LUGI-HIGH-SCORES.TXT';  {filespec for opening (hiscores) at SRI}
	    year = 95682560;  {how much two funny-format date-and-time integers differ by when they're a year apart}
	TYPE
	    str42 = PACKED ARRAY[1..42] OF char;
	    ascore = RECORD
			 username: str42;
			 thescore,thedate: integer
			 END;
	VAR
	    yourusername: str42;  {we allow 41 chars for username (it actually could be longer!)}
	    highs: ARRAY[1..10] OF ascore;  {highest ten scores of the past year}
	    highest,lowest: ascore;  {highest and lowest scores of all time}
	    notbest,greatest: boolean;  {this is not the player's best score / this is history's greatest score}
	    now,i,j,k: integer;  {current date and time / anything / anything / anything}
	    c: char;  {answer to question, or throw-away character}


	FUNCTION SameName(VAR a,b: str42): boolean;  {inside UpdateTopTen, inside Points}
	{Returns true if (a) and (b) hold the same text up to the first char}
	{<= space.  This Pascal doesn't implement equality tests between    }
	{arrays.  It is assumed that there is at least one space present.   }
	    VAR
		i: integer;
	    BEGIN
	    i := 1;
	    WHILE (a[i] > ' ') AND (a[i] = b[i]) DO
		i := i + 1;
	    SameName := (a[i] <= ' ') AND (b[i] <= ' ')
	    END;  {SameName}


    PROCEDURE WriteScores(a: ascore);
    {Writes a line showing the information in (a).}
	VAR
	    i: integer;
	BEGIN
	Write('    ',a.thescore: 4,'     ');
	IF a.thedate = 0 THEN
	    Write(' (never) ')
	ELSE
	    Jsys(220B;0: output,a.thedate,[9]);  {ODTIM monitor call-- writes out date, given integer}
	Write('     ');
	i := 1;
	WHILE a.username[i] > ' ' DO BEGIN
	    Write(a.username[i]);
	    i := i + 1
	    END;
	Writeln
	END;  {WriteScoreas}


	BEGIN  {UpdateTopTen}
	Jsys(13B;;i);  {GJINF monitor call--  get user number}
	yourusername := '                                          ';
	Jsys(41B,1;yourusername,i);  {DIRST monitor call--  translate number into username}
	FOR i := 1 TO 42 DO
	    IF yourusername[i] < ' ' THEN
		yourusername[i] := ' ';
	Jsys(227B;;now);  {GTAD monitor call-- returns the date and time in funny-format}
	REPEAT
	    Reset(hiscores,hisource);  {open the file, set Eof(hiscores) if unable}
	    IF Eof(hiscores) THEN
		Jsys(167B;500)  {wait half a second and try again}
	UNTIL NOT Eof(hiscores);
	FOR i := 1 TO 10 DO
	    Readln(hiscores,highs[i].thescore,c,highs[i].thedate,c,highs[i].username);
	Readln(hiscores,highest.thescore,c,highest.thedate,c,highest.username);
	Readln(hiscores,lowest.thescore,c,lowest.thedate,c,lowest.username);
	FOR i := 1 TO 10 DO  {reject old scores}
	    WHILE (now - year >= highs[i].thedate) AND (highs[i].thedate <> 0) DO BEGIN
		FOR j := i TO 9 DO
		    highs[j] := highs[j + 1];
		highs[10].username := '(nobody)                                  ';
		highs[10].thescore := 0;
		highs[10].thedate := 0
		END;
	notbest := false;
	i := 1;
	WHILE (highs[i - Ord(i = 11)].thescore > score) AND (i <= 10) DO BEGIN
	    IF SameName(highs[i].username,yourusername) THEN
		notbest := true;
	    i := i + 1
	    END;
	c := 'n';
	IF i <= 10 THEN
	    IF notbest THEN BEGIN
		Rclose(hiscores);
		Writeln; Writeln; Writeln;
		Writeln('--That would have made it into the top ten, but it''s not your best score.');
		Writeln('Do you wanna see the top ten?');
		ReadLetter(c)
		END
	    ELSE BEGIN
		k := i;
		WHILE NOT SameName(highs[k - Ord(k = 11)].username,yourusername) AND (k <= 10) DO
		    k := k + 1;
		IF k = 11 THEN
		    k := 10;
		FOR j := k - 1 DOWNTO i DO
		    highs[j + 1] := highs[j];
		highs[i].thescore := score;
		highs[i].username := yourusername;
		highs[i].thedate := now;
		greatest := score > highest.thescore;
		IF greatest THEN
		    highest := highs[1];
		Rewrite(hiscores);
		FOR i := 1 TO 10 DO
		    Writeln(hiscores,highs[i].thescore: 4,' ',highs[i].thedate,' ',highs[i].username);
		Writeln(hiscores,highest.thescore: 4,' ',highest.thedate,' ',highest.username);
		Writeln(hiscores,lowest.thescore: 4,' ',lowest.thedate,' ',lowest.username);
		Rclose(hiscores);
		Writeln; Writeln; Writeln;
		IF greatest THEN BEGIN
		    Writeln;
		    Writeln(Chr(7),'W*O*W!!!!    YOUR SCORE IS THE',Chr(7)); Writeln;
		    Writeln('   @      @         @                   @@@@@@@    @@@@@    @     @   @@@@@@@');
		    Writeln('  @ @     @         @                      @         @      @@   @@   @');
		    Writeln(' @   @    @         @                      @         @      @ @ @ @   @');
		    Writeln('@@@@@@@   @         @         @@@@@@@      @         @      @  @  @   @@@@');
		    Writeln('@     @   @         @                      @         @      @     @   @');
		    Writeln('@     @   @         @                      @         @      @     @   @');
		    Writeln('@     @   @@@@@@@   @@@@@@@                @       @@@@@    @     @   @@@@@@@');
		    Writeln; Writeln;
		    Writeln('@     @    @@@@@      @@@@@    @     @   @@@@@@@    @@@@@    @@@@@@@    ##');
		    Writeln('@     @      @       @     @   @     @   @         @     @      @       ##');
		    Writeln('@     @      @      @          @     @   @         @            @       ##');
		    Writeln('@@@@@@@      @      @          @@@@@@@   @@@@       @@@@@       @       ##');
		    Writeln('@     @      @      @    @@@   @     @   @               @      @       ##');
		    Writeln('@     @      @       @     @   @     @   @         @     @      @');
		    Writeln('@     @    @@@@@      @@@@@    @     @   @@@@@@@    @@@@@       @       ##'); Writeln; Writeln;
		    END
		ELSE
		    Writeln('Hey, you''ve made it onto the list of the top ten high scores!');
		c := 'y'
		END
	ELSE
	    IF score <= lowest.thescore THEN BEGIN
		lowest.thescore := score;
		lowest.username := yourusername;
		lowest.thedate := now;
		Rewrite(hiscores);
		FOR i := 1 TO 10 DO
		    Writeln(hiscores,highs[i].thescore: 4,' ',highs[i].thedate: 5,' ',highs[i].username);
		Writeln(hiscores,highest.thescore: 4,' ',highest.thedate: 5,' ',highest.username);
		Writeln(hiscores,lowest.thescore: 4,' ',lowest.thedate: 5,' ',lowest.username);
		Rclose(hiscores);
		Writeln;
		Writeln('CONGRATULATIONS!!  You have achieved the lowest score of all time!!!');
		c := 'y'
		END
	    ELSE BEGIN
		Rclose(hiscores);
		Writeln;
		Writeln('Ya wanna see the ten highest scores?');
		ReadLetter(c)
		END;
	IF c = 'y' THEN BEGIN
	    Writeln('Highest scores of the last year:'); Writeln;
	    Writeln('        Score      Date        Username');
	    Writeln('        -----      ----        --------');
	    FOR i := 1 TO 10 DO BEGIN
		IF i < 10 THEN
		    Write(' ');
		Write('#',i: 1,':');
		WriteScores(highs[i]);
		END;
	    Writeln;
	    Writeln('All-time highest and lowest scores:'); Writeln;
	    Write('    ');
	    WriteScores(highest);
	    Write('    ');
	    WriteScores(lowest)
	    END
	END;  {UpdateTopTen}


  BEGIN  {Points}
    IF wayended = jumped THEN BEGIN
	Writeln;
	Writeln('H    A');
	Writeln('H___]   A');
	Writeln('H         A');
	Writeln('H          A');
	Writeln('H           A');
	Writeln('H');
	Writeln('H            A');
	Writeln('H');
	Writeln('H             H');
	Writeln('H');
	Writeln('H             !');
	IF Have(onitro,true) THEN BEGIN
	    Writeln('H          \ `   %');
	    Writeln('H        * !BLAMM!! =   (your nitro detonated.)');
	    Writeln('==============================')
	    END
	ELSE BEGIN
	    Writeln('H');
	    Writeln('H          >splat!<');
	    Writeln('==============================')
	    END;
	IF Have(osandwich,true) THEN
	    wipeout := true;
	IF strong AND NOT Have(onitro,true) THEN BEGIN
	    Writeln;
	    Writeln(' !!! My God, you survived the fall!');
	    beastfollowing := false;
	    wayended := escaped
	    END
	ELSE
	    wayended := killed
	END;
    IF wayended = escaped THEN
	IF Have(osandwich,true) THEN
	    wipeout := true;
    IF wipeout THEN BEGIN
	Writeln;
	Writeln('Unfortunately, you carried the Lugonian Plague out to the city and wiped');
	Writeln('out eighty percent of the population!')
	END;
    IF (wayended = escaped) AND fungus AND NOT wipeout THEN BEGIN
	Writeln;
	Writeln('The people of the city are annoyed at you for spreading the irritating fungus.')
	END;
    IF wayended <> chicken THEN BEGIN
	Writeln;
	Write('(Hit return for your score:) ');
	Readln
	END;
    Writeln; Writeln; Writeln; Writeln;
    Writeln('******************  YOUR SCORE  ******************');
    score := -200;  {**** most common, but only at SU-LOTS}
    CASE wayended OF
(*	pty: Writeln(    'Playing on a PTY:      -200 points');	 {**** SU-LOTS only-- from EnforceGame...}
	dialin:	Writeln( 'Playing on a dialin:   -200 points');	 {**** ditto}
	queue: BEGIN
	    Writeln(     'Playing while the');
	    Writeln(     '       queue is long:  -200 points')  {**** ditto}
	    END;
	load: BEGIN
	    Writeln(     'Playing with the');
	    Writeln(     '       load too high:  -200 points')  {**** ditto}
	    END;
	daytime: BEGIN
	    Writeln(     'Playing at the');
	    Writeln(     '        wrong time:    -200 points')  {**** ditto}
	    END; *)
	escaped: BEGIN
	    Writeln(     'For escaping:           100 points');
	    score := 100
	    END;
	eaten: BEGIN
	    Writeln(     'Being eaten:            -50 points');
	    score := -50
	    END;
	killed: BEGIN
	    Writeln(     'Getting killed:         -50 points');
	    score := -50
	    END;
	chicken: BEGIN
	    Writeln(     'Chickening out:        -100 points');
	    score := -100
	    END
	END;
    IF NOT guardalive THEN BEGIN
	Writeln(         'Killing the guard:       40 points');
	score := score + 40
	END;
    IF runtkilled THEN BEGIN
	Writeln(         'Dealing with');
	Writeln(         '   unhealthy Lugiman:    40 points');
	score := score + 40
	END;
    IF burnt THEN BEGIN
	Writeln(         'Dealing with plants:     40 points');
	score := score + 40
	END;
    IF clean THEN BEGIN
	Writeln(         'Cleaning the bathroom:   10 points');
	score := score + 10
	END;
    IF fungus AND NOT Have(osandwich,true) AND (wayended = escaped) THEN BEGIN
	Writeln(         'Spreading the fungus:   -25 points');
	score := score - 25
	END;
    IF bystanders AND NOT Have(osandwich,true) THEN BEGIN
	Writeln(         'Killing pedestrians:   -300 points');
	score := score - 300
	END;
    IF wipeout THEN BEGIN
	Writeln(         'Wiping out the city:   -700 points');
	score := score - 700
	END;
    i := 40 * (2 - Ord(duration));
(*  IF wayended IN [queue,load,daytime,escaped,eaten,killed] THEN BEGIN	  {****} *)
    IF wayended IN [escaped,eaten,killed] THEN BEGIN
	Writeln(         'Difficulty of game:      ',i: 2,' points');
	score := score + i
	END;
    obct :=  0;
    FOR ob := orope TO opills DO
	IF (ob <> ourine) AND had[ob] THEN
	    obct := obct + 1;
    Writeln(             'Objects picked up:      ',5 * obct: 3,' points');
    score := score + 5 * obct;
    IF wayended = escaped THEN BEGIN
	obct := 0;
	FOR ob := orope TO opills DO
	    IF (ob IN [okeys,ochemicals,ofluid,ogum,oflysol,ostatuette,opills]) AND Have(ob,true) THEN
		obct := obct + 1;
	i := obct * 10 + gleeps + Ord(Have(obag,true)) * baggleeps;
	score := score + i;
	Writeln(         'Bringing out gleeps');
	Writeln(         '    and alien objects: ',i: 4,' points')
	END;
    IF (wayended = escaped) AND beastfollowing THEN BEGIN
	Writeln(         'Bringing out animal:     80 points');
	score := score + 80
	END;
    Writeln(             'Rooms explored:         ',beenin * 3: 3,' points');
    score := score + beenin * 3;
    IF beenin = Ord(pbalcony) THEN BEGIN
	Writeln(         'Finding every room:      80 points');
	score := score + 80
	END;
    Writeln(             '                       ___________');
    Writeln(             'TOTAL SCORE:           ',score: 4,' POINTS.');
    Writeln(             '**************************************************');
    IF score < 0 THEN BEGIN
	Writeln;
	Writeln('That is a miserable score, strictly desk jockey class.')
	END
    ELSE
	IF score >= 700 THEN BEGIN
	    Writeln(Chr(7)); Writeln; Writeln;
	    Writeln('!!!!!!!!!!!!!!!!!!!!!!!!!');
	    Writeln('INCREDIBLE!!!  Throgg the Omnicient, Highest God of the Galaxy, appears to you');
	    Writeln('in a vision.  He crowns you the Secret Master of Earth, and gives you many great',Chr(7));
	    Writeln('powers, on the condition that you have no pride or other unspiritual emotions');
	    Writeln('for having achieved this score.   (Woops...)')
	    END
	ELSE BEGIN
	    Writeln;
	    CASE score DIV 100 OF
		0: Writeln('The CIA director, in a press conference, calls your work "really sloppy."');
		1: Writeln('Your work is unofficially evaluated as "well, not fantastic..."');
		2: Writeln('Your superior tells you that you did a half-way decent job.');
		3: Writeln('Your comrades slap you on the back and say "good going!"');
		4: Writeln('The CIA director praises you for an "excellent job.  Really good!"');
		5,6: BEGIN
		    Writeln('Your accomplishments are called "astounding!"  The president himself');
		    Writeln('decorates you.  You have a great future, just beginning, in the CIA.')
		    END
		END
	    END;
    UpdateTopTen(score);  {**** a different version may perhaps be retained at other sites}
    END;  {Points}








BEGIN  {Main}
Reset(input,'TTY:',true);  {**** opens the user's terminal for interactive input (default at SU-LOTS is a disk file)}
Rewrite(output,'TTY:');	  {**** ditto for output}
{The variable (ready) will be false immediately after compilation at SU-LOTS.
    Thus, the initialization will take place on the first run.  If successful,
    it will set (ready) true, and if the program is then SAVEd, it will be
    true whenever the saved program is run.  Thus, the initialization is
    never done in the "production" Lugi; the initialized values are preserved
    in the saved file.}
IF NOT ready THEN BEGIN
    Reset(init,initfilespec);  {**** open the file}
    LoadMessages;
    IF ready THEN
	MakeDictionary
    ELSE
	Writeln('##### Initialization has failed. #####');
    Mark(heapbound)  {****}
    END
ELSE BEGIN
    Release(heapbound);	  {**** tells New what part of the heap is already in use (it forgets when game is restarted)}
    SetUp;
(*  EnforceGamePlayingRulesAtStanfordLOTS;  {**** SU-LOTS only} *)
    IF wayended = notdone THEN
	Inform;
    starttime := Timer;
    WHILE wayended = notdone DO BEGIN
	IF tellsitu THEN
	    Situation;
	lastenv := env;
	IF wayended = notdone THEN BEGIN
	    REPEAT  {input the player's command; he types a line, it goes into a buffer that Get accesses}
		Write('   ');
		Readln;
		Write(Chr(13))  {**** helps sometimes}
	    UNTIL NOT Eoln(input);
	    FOR m := nomeaning TO mnot DO
		dupcatch[m] := false;
	    HearAndObey;
	    IF NOT map[yourroom].explored THEN
		beenin := beenin + 1;
	    map[yourroom].explored := true
	    END
	END;
    IF wayended <> crash THEN BEGIN
	Points;
	Writeln;
	Writeln('Bye...   Wasn''t that FUN?')
	END
    END
END.
