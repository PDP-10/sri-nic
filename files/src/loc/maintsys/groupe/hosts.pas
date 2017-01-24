const
  buckets = 127;

type

sourceplace=(patches,nic,hosts2);

word = packed array[1..25] of char;

adrlstpt = ^addresslist;

addresslist = record
	address: integer;
	hosts2: word;
	hosts2length: integer;
	nohosts2: Boolean;
	next:	^addresslist
	end;

namlstpt = ^namelist;

namelist = record
	name: word;
	namelength: integer;
	next: ^namelist
	end;

hstlstpt = ^hostlist;

hostlist = record
	hostname: word;
	hostnamelength: integer;
	source: sourceplace;  {where this data came from}
	nicknames: ^namelist;
	addresses: ^addresslist;
	machine: word; machinelength: integer;
		{Machine is basically for HOSTS2, so we convert to
		 its format for PDP10, PDP11, VAX, C/30, and C/70}
	opsys: word; opsyslength: integer;
		{This is also for HOSTS2, so we convert to TOPS-10,
		 TOPS-20, etc.}
	server: Boolean;
	next: ^hostlist
	end;

namehashlist = record
	hashname: word;
	object: ^hostlist;
	next: ^namehashlist
	end;

addresshashlist = record
	hashaddress: integer;
	object: ^hostlist;
	next: ^addresshashlist
	end;

var

temphost:hostlist;
currentsource:sourceplace;
str:text;
nichosts,newhost,oldhost,thishost: ^hostlist;
hosts: ^hostlist;
atom: word;  atomlength: integer;
convert: record case Boolean of
	true: (n: word);
	false: (w: packed array[1..5]of char)
	end;
lastaddress: ^addresslist;
thisname: ^namelist;
namehasharray: array [1..buckets] of ^namehashlist;
addresshasharray: array [1..buckets] of ^addresshashlist;
i: integer;

procedure quit; extern;

{*** Hash table routines.  We use the hash table to catch duplicate names ***}

function namehashlookup(var name:word):hstlstpt;
	var index:integer;
	hash: ^namehashlist;
  begin
  index := ord(name[1]) + ord(name[2])*2 + ord(name[7])*4 + ord (name[10]);
  index := (index mod buckets) + 1;
  hash := namehasharray[index];
  while hash <> nil do
    begin
    if hash^.hashname = name
      then begin
      namehashlookup := hash^.object; 
      goto 666
      end;
    hash := hash^.next
    end;
  namehashlookup := nil;
666:
  end;

procedure namehashinsert(var name:word;ob:hstlstpt);
	var index:integer;
	hash: ^namehashlist;
  begin
  index := ord(name[1]) + ord(name[2])*2 + ord(name[7])*4 + ord (name[10]);
  index := (index mod buckets) + 1;
  new(hash);
  with hash^ do
    begin
    hashname := name;
    object := ob;
    next := namehasharray[index];
    end;
  namehasharray[index] := hash;
  end;

function addresshashlookup(var address:integer):hstlstpt;
	var index:integer;
	hash: ^addresshashlist;
  begin
  if address <> 0 then
    begin
    index := (address mod buckets) + 1;
    hash := addresshasharray[index];
    while hash <> nil do
      begin
      if hash^.hashaddress = address
        then begin
        addresshashlookup := hash^.object; 
        goto 666
        end;
      hash := hash^.next
      end;
    end;
  addresshashlookup := nil;
666:
  end;

procedure addresshashinsert(address:integer;ob:hstlstpt);
	var index:integer;
	hash: ^addresshashlist;
  begin
  if address <> 0 then
    begin
    index := (address mod buckets) + 1;
    new(hash);
    with hash^ do
      begin
      hashaddress := address;
      object := ob;
      next := addresshasharray[index];
      end;
    addresshasharray[index] := hash;
    end
  end;

{*** Low level I/O routines  ***}

{GETATOM reads the next name.  It skips blanks before and after and stops
 at the next break char.  Because of skipping afterwards, the user
 can just look at INPUT^ afterwards to see what the next break char is}

procedure getatom;
	var i:integer;
  begin
  while input^ in [' ','	'] do
    get(input);
  read(atom:atomlength:[' ','	',':',',',']','[']);
  while input^ in [' ','	'] do
    get(input);
  end;

{SYNC makes sure there is a : where there should be}

procedure sync;
  begin
  if input^ = ':'
    then get(input)
    else begin
    writeln(tty,'? Input out of sync at ',curpos(input));
    quit
    end
  end;

{COMMA makes sure there is a , where there should be}

procedure comma;
  begin
  if input^ = ','
    then get(input)
    else begin
    writeln(tty,'? Input out of sync at ',curpos(input));
    quit
    end;
  while input^ in [' ','	'] do
    get(input);
  end;

{INTERNETADDRESSES scans a list of internet addresses, separated by
 commas, building up an addresslist to represent them.  It starts with
 INPUT^ pointing at the beginning of the first octet, and ends pointing
 at a break char (skipping blank space)}

function internetaddresses:adrlstpt;
   var	thisaddress,lastaddress: ^addresslist;
	i,j,k:integer;
  begin
  lastaddress := nil;
  loop
    new(thisaddress);
    thisaddress^.next := lastaddress;
    read(i);
    for k := 2 to 4 do
      begin
      if input^ <> '.' then
        begin writeln(tty,'? Dot missing at ',curpos(input)); quit end;
      get(input);
      read(j);
      i := i*256 + j
      end;
    thisaddress^.address := i;
    lastaddress := thisaddress;
    while input^ in [' ','	'] do
      get(input);
  exit if input^ <> ',';
    get(input)
  end;
  internetaddresses := thisaddress
  end;

{GETOCTAL reads an octal number}

function getoctal:integer;
 	var i:integer;
  begin
  i := 0;
  while input^ in ['0'..'7'] do
    begin
    i := i * 8 + (ord(input^) - 60B);
    get(input);
    end;
  getoctal := i;
  end;

{HOSTS2ADDRESSES scans a list of addresses from HOSTS.SCORE, separated by
 commas, building up an addresslist to represent them.  It starts with
 INPUT^ pointing at the beginning of the first address, either an actual
 code or a bracket}

function hosts2addresses:adrlstpt;
   var	thisaddress,lastaddress: ^addresslist;
	char3: packed array[1..3]of char;
	oneonly: Boolean;
	i,j,k:integer;
  begin
  oneonly := input^ <> '[';
  if not oneonly
    then begin
    get(input);  {skip [}
    while input^ in [' ','	'] do
      get(input)
    end;
  lastaddress := nil;
  loop
    newz(thisaddress);
    thisaddress^.next := lastaddress;
    if input^ = 'N' then
      begin
      getatom;
      if atom <> 'NOHOSTS2                 '
	then begin writeln(tty,'? Unknown net at ',curpos(input)); quit end;
      thisaddress^.nohosts2 := true
      end;
    case input^ of
	'0','1','2','3','4','5','6','7','8','9':
	{Arpanet or Internet}
	  begin
	  read(i);
	  if input^ = '.' then {Internet}
	    begin
            for k := 2 to 4 do
	      begin
	      if input^ <> '.' then
	        begin writeln(tty,'? Dot missing at ',curpos(input)); quit end;
	      get(input);
	      read(j);
	      i := i*256 + j
	      end;
	    thisaddress^.address := i
	    end
	  else if input^ = '/' then {Arpanet}
	    begin
	    get(input);
	    read(j);
	    thisaddress^.address := 10*"1000000 + i*"10000 + j;
	    end
	  else
	    begin
	    writeln(tty,'? / missing in Arpanet address at ',curpos(input));
	    quit
	    end;
	  end;
	'L':
	{LCS}
	  begin
	  read(char3);  {make sure it is}
	  if char3 <> 'LCS' then
	    begin writeln(tty,'? Unknown net at', curpos(input)); quit end;
	  while input^ in [' ','	'] do
	    get(input);
	  i := getoctal;
	  if input^ <> '/' then
	    begin
	    writeln(tty,'? / missing in LCS address at ',curpos(input));
	    quit
	    end;
	  get(input);
	  j := getoctal;
	  thisaddress^.address := 18*"1000000 + i*"10000 + j;
	  end;
	'R':  {RU or RCC}
	  begin
	  get(input);
	  if input^ = 'C' then
	    begin
	    get(input);
	    if input^ <> 'C' then
	      begin writeln(tty,'? Unknown net at', curpos(input)); quit end;
	    get(input);
	    read(i);
	    if input^ <> '/' then
	      begin
	      writeln(tty,'? / missing in RCC address at ',curpos(input));
	      quit
	      end;
	    get(input);
	    read(j);
	    thisaddress^.address := 3*"1000000 + i*"10000 + j;
	    end
	   else begin {input^ is U}
	    read(atom:i:[',',']']); {end of field must be , or ]}
	    strwrite(str,thisaddress^.hosts2);
	    write(str,'R',atom:i);
	    thisaddress^.hosts2length := i+1;
	    end
	  end;
	others:
	  begin
	  read(thisaddress^.hosts2:i:[',',']']);
	  thisaddress^.hosts2length := i
	  end;
	end;
    lastaddress := thisaddress;
  exit if oneonly or (input^ <> ',');
    get(input);  {skip ,}
    while input^ in [' ','	'] do
      get(input)
  end;
  if not oneonly
    then get(input);	{skip ]}
  hosts2addresses := thisaddress
  end;

{WRITEADDRESS writes out an address in Internet format.  The oddball
 types are stored as text, so this is the only actual formatting we
 ever have to do.}

procedure writeaddress(var f:text;address:integer);
  begin
  write(f,'[',address div "1000000:0,
	  '.',(address div "10000) mod "100:0,
	  '.',(address div "100) mod "100:0,
	  '.',address mod "100:0,
	  ']')
  end;


{CHOMPADDRESSES validates a list of addresses.  It removes addresses
 that we have already and sorts them in order of desirability.  At
 the moment, just Arpanet first.}

function chompaddresses(addresses:adrlstpt;ob:hstlstpt):adrlstpt;
	var preferred: adrlstpt;  {our preferred address if any}
	    thisaddr, newaddr, oldaddr: adrlstpt; {used for copying}
  begin

  preferred := nil;
  thisaddr := addresses;
  newaddr := nil;

  while thisaddr <> nil do
    begin
    oldaddr := thisaddr^.next; 
    thisaddr^.next := newaddr;
    if addresshashlookup(thisaddr^.address) <> nil
      then begin
      write(tty,'% Address ');
      writeaddress(ttyoutput,thisaddr^.address);
      writeln(tty,' is already in table for another site - ignored')
      end
    else if (thisaddr^.address div "1000000) = 10  {Arpanet}
      then begin
      addresshashinsert(thisaddr^.address,ob);
      if preferred <> nil
	then begin
	preferred^.next := newaddr;
	newaddr := preferred
	end;	
      preferred := thisaddr;
      end
    else if ((thisaddr^.address div "1000000) <> 0) {other direct}
	     and (preferred = nil)
      then begin
      addresshashinsert(thisaddr^.address,ob);
      preferred := thisaddr
      end
    else begin
      addresshashinsert(thisaddr^.address,ob);
      newaddr := thisaddr
      end;
    thisaddr := oldaddr;
  end;

  if preferred <> nil
    then begin
    preferred^.next := newaddr;
    newaddr := preferred
    end;

  chompaddresses := newaddr;
  end;  

{GETNAMES scans a list of names separated by commas.  Because it uses
 GETATOM, it leaves INPUT^ pointing at a break character.  It returns
 a namelist}

function getnames(ob:hstlstpt): namlstpt;
    var	thisname,lastname: ^namelist;
  begin
  lastname := nil;
  while input^ = ',' do
    begin
    get(input);
    getatom;
    new(thisname);
    thisname^.name := atom;
    thisname^.namelength := atomlength;
    thisname^.next := lastname;
    if namehashlookup(atom) <> nil
      then writeln(tty,'% Nickname ',atom:atomlength,
		   ' is already in table for another site - ignored')
      else begin
      namehashinsert(atom,ob);
      lastname := thisname
      end
    end;
  getnames := lastname;
  end;

{WRITEADDRESSES is for HOSTS2 only}

procedure writeaddresses(addresses:adrlstpt);
	var onlyone: Boolean;
	    wrotesomething: Boolean;
	    nextaddress: adrlstpt;
	    addresscount: integer;
  begin
  addresscount := 0;
{See if more than one address}
  nextaddress := addresses;
  while (nextaddress <> nil) and (addresscount < 2) do
    begin
    if not nextaddress^.nohosts2
      then addresscount := addresscount + 1;
    nextaddress := nextaddress^.next
    end;
  onlyone := addresscount = 1;
  if not onlyone
    then write('[');
  wrotesomething := false;  {nothing written yet}
  repeat
    with addresses^ do
      begin
      if nohosts2 
	then wrotesomething := false  {not if suppressed}
      else if address = 0   {funny HOSTS2 address}
	then
	  begin
	    if wrotesomething then
	      write(',');
	    wrotesomething := true;
	    write(hosts2:hosts2length)
 	  end
      else if (address div "1000000) = 18  {LCS}
	then
	  begin
	    if wrotesomething then
	      write(',');
	    wrotesomething := true;
	    write('LCS ',(address div "10000) mod "100:3:o,
		   '/',address mod "100:3:o)
	  end
      else
        begin
	  if wrotesomething then
	    write(',');
	  wrotesomething := true;
	  write(address div "1000000:0,'.',
		 (address div "10000) mod "100:0,'.',
		 (address div "100) mod "100:0,'.',
		 address mod "100:0);
	end;
      addresses := next
      end;
    until addresses = nil;
  if not onlyone
    then write(']');
  end;      

procedure writenames(names:namlstpt);
  begin
  write('[');
  loop
    with names^ do
      begin
      write(name:namelength);
      names := next
      end;
  exit if names = nil;
    write(',')
  end;
  write(']')
  end;

{DUPLICATE name is called when we are trying to add a new host
 whose name is HOSTNAME, and we find that name is already in
 use.  The caller aborts the host in any case.  This routine
 has to figure out whether to warn the user.  This can be OK.
 The same host may well appear in all 3 files, and we do not
 want an error in that case.  Where we want the error is if
 the same name is being used for different hosts, so that this
 will cause us to miss a host.}

procedure duplicatename(hostname:word;hostnamelength:integer;
			oldhost,newhost:hstlstpt);
	var addlist:^addresslist;
  begin
  if currentsource = oldhost^.source
    then writeln(tty,'% Host name ',hostname:hostnamelength,
		' appears twice in the same file - second ignored')
    else begin
    addlist := newhost^.addresses;
    while addlist <> nil do
      if addresshashlookup(addlist^.address) <> nil
        then goto 666
        else addlist := addlist^.next;
    writeln(tty,'% Host name ',hostname:hostnamelength,
 	      ' is already in use for a different address - ignored');
666:
    end
  end;

procedure readnic;
begin
while not eof do
  begin
  getatom;
  if atom = 'HOST                     '
    then begin
    sync;
    new(newhost);
    with newhost^ do
      begin
      source := currentsource;
      next := hosts;
      addresses := internetaddresses;
      sync;
      getatom;
      hostname := atom;
      hostnamelength := atomlength;

{Check for duplicates.  At the moment we print a warning and ignore them.
 Various people get upset by duplicates}

      oldhost := namehashlookup(hostname);
      if oldhost <> nil
	then begin
	duplicatename(hostname,hostnamelength,oldhost,newhost); 
				 {message, if any}
	goto 666
	end;
      addresses := chompaddresses(addresses,newhost);
      if addresses = nil
	then goto 666;
      namehashinsert(hostname,newhost);
      nicknames := getnames(newhost);
      sync;
      getatom;
      machine := atom;
      machinelength := atomlength;
      convert.n := atom;
      if (convert.w = 'DEC-2') or (convert.w = 'DEC-1')
	then begin
        machine := 'PDP10                    ';
	machinelength := 5
	end
      else if convert.w = 'PDP-1'
	then begin
	machine := 'PDP11                    ';
	machinelength := 5
	end
      else if convert.w = 'VAX-1'
	then begin
	machine := 'VAX                      ';
	machinelength := 3
	end;
      sync;
      getatom;
      opsys := atom;
      opsyslength := atomlength;
      if atom = 'TOPS10                   '
	then begin
	opsys := 'TOPS-10                  ';
	opsyslength := 7
        end
      else if atom = 'TOPS20                   '
        then begin
	opsys := 'TOPS-20                  ';
	opsyslength := 7
	end;
      while input^ = ',' do	{handle TAC,NCP}
	begin
	get(input);
	getatom;
	end;
      sync;

 {For HSTNAM.MULTINET we are supposed to say whether the thing is a
  server.  We really don't know, so assume anything not a TIP or TAC is}

      server := not ((opsys = 'TAC                      ') or
		    (opsys = 'TIP                      '));
      hosts := newhost;
666:  {here if for some reason we decide to ignore this entry}
      end
    end;
  readln
  end;
end;

procedure readhosts2;
begin
while not eof do
  begin
  getatom;
  if atom = 'HOST                     '
    then begin
    new(newhost);
    with newhost^ do
      begin
      source := currentsource;
      next := hosts;
{host name}
      getatom;
      hostname := atom;
      hostnamelength := atomlength;
      comma;  {better be one}
      addresses := hosts2addresses;
{Check for duplicates.  At the moment we print a warning and ignore them.
 Various people get upset by duplicates}
      oldhost := namehashlookup(hostname);
      if oldhost <> nil
	then begin
	duplicatename(hostname,hostnamelength,oldhost,newhost); 
				 {message, if any}
	goto 616
	end;
{addresses}
      comma;
      addresses := chompaddresses(addresses,newhost);
      if addresses = nil
	then goto 616;
      namehashinsert(hostname,newhost);
{other junk}
      getatom;
      server := atom = 'SERVER                   ';
      if input^ = ',' 
	then begin
	get(input);  {gobble comma}
        getatom;
        opsys := atom;
        opsyslength := atomlength;
	if input^ = ','
	  then begin
	  get(input); {gobble comma}
          getatom;
          machine := atom;
          machinelength := atomlength;
          if input^ = ','	{nicknames coming}
	    then begin
	    get(input);  {skip comma}
	    input^ := ','; {getnames wants a comma here}
            nicknames := getnames(newhost);
            end
	  end
	end;
      hosts := newhost;
616:  {here if for some reason we decide to ignore this entry}
      end
    end;
  readln
  end;
end;


begin {MAIN}
hosts := nil;
for i := 1 to buckets do
  begin
  namehasharray[i] := nil;
  addresshasharray[i] := nil
  end;


{**** Read our adjustments file first}

currentsource := patches;
reset(input,'system:hosts.patches','/e');
writeln(tty,'[Reading local patch file]');
readhosts2;

{**** Now read HOSTS.SCORE, the HOSTS2 data}

currentsource := hosts2;
reset(input,'ps:<netinfo>hosts.score','/e');
writeln(tty,'[Reading SCORE host table]');
readhosts2;

{**** Now the official NIC file}

currentsource := nic;
reset(input,'ps:<netinfo>hosts.txt','/e');
writeln(tty,'[Reading NIC host table]');
readnic;

/*

nichosts := hosts;   {save for later sorting}
hosts := nil;

{Sort them, so we see them in the canonical order}

thishost := hosts;
newhost := nil;

while thishost <> nil do
  begin
  oldhost := thishost^.next; 
  thishost^.next := newhost;
  newhost := thishost;
  thishost := oldhost;
  end;

thishost := nichosts;

while thishost <> nil do
  begin
  oldhost := thishost^.next; 
  thishost^.next := newhost;
  newhost := thishost;
  thishost := oldhost;
  end;

hosts := newhost;

*/

{****Let's see what we have.  First we write out HSTNAM.MULTINET.  For this
	file we only care about things we can directly connect to, and we
	can only give one address per host, so we give preference to the
	Arpanet address, if any.  At the moment we assume this is the
	first one.  Quick inspection does suggest that the first entry
	is typically the best}

rewrite(output,'hstnam.multinet');

{first the non-nicknames}

newhost := hosts;
while newhost <> nil do
  with newhost^ do
  begin if addresses^.address <> 0
    then begin

{the addresses are sorted best first, so just use the first}

    write(addresses^.address:12:o,
	  ',',
	  hostname:hostnamelength);

{HSTNAM does not use the same set of legal host names, so we have to
 translate them}

    if opsys = 'TOPS-10                  '
      then write(',DEC10')
    else if opsys = 'TAC                      '
      then write(',TIP')
    else if (opsys = 'TENEX                    ') or
	    (opsys = 'TOPS-20                  ') or
	    (opsys = 'UNIX                     ') or
	    (opsys = 'ITS                      ') or
	    (opsys = 'TIP                      ') or
	    (opsys = 'MOS                      ') or
	    (opsys = 'ELF                      ') or
	    (opsys = 'MULTICS                  ')
     then write(',',opsys:opsyslength)
    else write(',');
    if server then
      write(',SERVER');

{everybody is a user, or at least we can't tell who isn't, and 
 everybody had better be using new telnet now}

    writeln(',USER,NEW');
    end;
  newhost := next
  end;

{now the nicknames}

newhost := hosts;
while newhost <> nil do
  with newhost^ do
  begin
  if addresses^.address <> 0
    then begin

{Go down list of add nicknames for this}

    thisname := nicknames;



    if server and ((addresses^.address div "1000000) = 10) then
    while thisname <> nil do
      with thisname^ do
        begin

{the addresses are sorted best first, so just use the first}

        write(addresses^.address:12:o,
	      ',',
	      name:namelength);
        if opsys = 'TOPS-10                  '
          then write(',DEC10')
        else if opsys = 'TAC                      '
          then write(',TIP')
        else if (opsys = 'TENEX                    ') or
	    (opsys = 'TOPS-20                  ') or
	    (opsys = 'UNIX                     ') or
	    (opsys = 'ITS                      ') or
	    (opsys = 'TIP                      ') or
	    (opsys = 'MOS                      ') or
	    (opsys = 'ELF                      ') or
	    (opsys = 'MULTICS                  ')
          then write(',',opsys:opsyslength)
	else write(',');
	write(',NICKNAME');
        if server then
          write(',SERVER');
        writeln(',USER,NEW');
	thisname := next
	end;
    end;
  newhost := next
  end;

{Now write out HOSTS.HOSTS2 for HOSTS2}

rewrite(output,'hosts.txt');

reset(input,'system:hosts.prolog','/e');
while not eof do
  begin
  output^ := input^;
  put(output);
  get(input)
  end;

newhost := hosts;
while newhost <> nil do
  begin
  oldhost := newhost;
  while oldhost <> nil do
    begin
    if oldhost^.hostname < newhost^.hostname then
      begin
      temphost := oldhost^;
      oldhost^ := newhost^;
      newhost^ := temphost;
      newhost^.next := oldhost^.next;
      oldhost^.next := temphost.next;
      end;
    oldhost := oldhost^.next;
    end;
  newhost := newhost^.next
  end;

newhost := hosts;

while newhost <> nil do
  with newhost^ do
    begin
    write('HOST ',hostname:hostnamelength,', ');
    writeaddresses(addresses);
    if server
      then write(',SERVER,')
      else write(',USER,');
    write(opsys:opsyslength,',',machine:machinelength);
    if nicknames <> nil
      then begin
      write(',');
      writenames(nicknames);
      end;
    writeln;    
    newhost := next
    end;

end.
