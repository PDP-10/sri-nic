{$M-}  

{This is the source for the QUASAR interface.  See QUASAR.PAS for
 documentation}

program qsr;
const
  ntries=10;
{jsys definitions}
  msend=510B;
  mrecv=511B;
  mutil=512B;
  erstr=11B;
  disms=167B;
{parameters}
  priou=101B;
  ipciw=1B;
  muqry=20B;
  mursp=25B;
  spqsr=2;
  mucre=6;
  ipcf16=601035B;
  
type 
{packetdescriptor describes the IPCF packet descriptor, which is
what you give to MSEND, MRECV, etc.  See Monitor calls for details.
The case Boolean is because "page mode" IPCF needs a page number,
which we handle as an integer, but normal mode needs an address,
which we handle as a pointer.}
     packetdescriptor=packed record
	ipcfl:integer;
	ipcfs:integer;
	ipcfr:integer;
	case Boolean of
	  true: (ipcfpl:0..777777B;ipcfpn:0..777777B;
		 ipcfd:integer);
	  false: (ipcxx:0..777777B;ipcfpt:^sendmessage)
	end;
    messtypes=(type0);

{sendmessage is used messages we send to Quasar.  For simplicity, we
always send in page mode.  Here is an explanation of the general
structure of messages to QUASAR:
	length of whole message,,message type code
	flags - bit 0 is always a flag asking for an acknowlegement
		typically the acknowlegement is the text saying
		"5 jobs killed", that the EXEC prints with [] around it.
	acknowlegement code - we could supply a code so we can tell
		the acknowlegements apart from each other
	message-specific flags
	count of arguments.  The rest of the message consists of
		"arguments", each of which has the following form.  This
		is the number of them.
	length of argument including this word,,argument type code
	data for argument}
    sendmessage=record case messtypes of
	type0: (qsrmsg:array[0..511]of integer)
	end;
    smesspt = ^sendmessage;

{message is used for received messages. The fields described below are the
standard QUASAR message fields.  We typically ignore the header, which
contains a count and message type.  The next word contains some flags,
of which we are only interested in MORE.  This indicates when a message
is too long, and is continued into another one.  NUMARGS indicates the
number of fields in this message.  The rest of it then contains the
messages in the usual QUASAR form of COUNT,,MSGTYPE followed by data.
We let you access halfwords in order to read these headers, and characters
in order to get at the data, which is text for the messages we are
interested in.  Note that some messages we receive are in page mode and
some in normal mode.  However we always use the same area, so no one
needs to know about this except that routine that does the MRECV.}
    message=packed record
      header:array[0..2]of integer;
      dumflg:Boolean; more:Boolean;
      numargs:integer;
      case Boolean of
        true: (halfs:packed array[0..1013]of 0..777777B);
        false: (chars:packed array[0..2534]of char)
      end;
    messpt=^message;

{The following types are used to define parameters to various switches.
QUEUE is sort of interesting.  There are two different representations
used when talking to QUASAR to tell it what kind of object you are
talking about.  The most common one is an integer.  But in the LIST
command there is a word of bits, with one bit for each object type.
This lets you ask to see any combination of queues you want.  The
QUEUE type is defined so that QUEUESET gives you the right bits.  To
get the integer version for use in the other commands, you have to
translate using the table QUEUETRAN.  We prefer to always use the
same representation for queues.  We chose the one used for LIST because
the other one has gaps in the assignments, as you will see if you
look at QUEUETRAN.}
    queue=(batch,printer,cardpunch,papertape,plotter,dumqueue,mount,retrieve);
    queueset = set of queue;
    brevity=(notverbose,verbose,brief);
{Fstring is used to define arguments which are going to be passed as
STRING.  When the caller specifies STRING, the callee see two variables:
the data and a length code.  Normally the data is defined as a packed
array of char, but for this program we chose to define it as words.
Note that Pascal will not make sure they are ASCIZ.  Thus the routines
that take string arguments use the length code to figure out how many
words to copy, and then clear the rest of the last word.}
    fstring=array[0..100]of integer;
    ptpform=(tapedum,ptpascii,ptpimage,ptpimagebinary,ptpbinary);
    outputlog=(outlogdum,lognever,logalways,logerror);    
    notification=(notifdum,notifytty,notifymail,notifyjob);
    batchlog=(batlogdum,logappend,logsupercede,logspool);
    sstring=packed array[1..12]of char;
     
var packet:packetdescriptor;
    mess:smesspt;  {the place we put messages we are sending}
    smesspage:integer;  {page number of mess^}
    memdone: Boolean;  {indicates we have done memory allocation}
    m:messpt;		{the place we receive messages}
    messpage:integer;  {page number of m^}

{nextindex is used by the command to create queue entries.  This
builds a message incrementally, and nextindex keeps track of the
place where the next option should go in the block we are building.
MYPID is the pid for this process, which is assigned once at the
beginning.  QSRPID is Quasar's pid.  We get it each time we want
to do a command, in case Quasar happens to have crashed and restarted.}
    nextindex,mypid,qsrpid:integer;
    ret:integer;
    userlen,i,j:integer;
    conv:packed record case Boolean of
	true:(word:integer);
	false:(queues:queueset)
	end;

{Queuetran is used to convert from our normal representation of
queue types to one often used by Quasar.  See the definition of
QUEUE above for details}
    queuetran:array[queue]of integer;

{TEST is probably not used at the moment.  It is useful for testing
when we need to build an FSTRING}
    test:record case Boolean of
	true:(words:fstring);
	false:(chars:packed array[1..300]of char)
	end;

{TESTS is also used for testing}
    tests:sstring;

	{Here is the conversion table from the bit numbers used in LIST to
	the integers used in the other request types.}

initprocedure;
  begin
  queuetran[batch] := 4b;
  queuetran[printer] := 3b;
  queuetran[cardpunch] := 5b;
  queuetran[papertape] := 6b;
  queuetran[plotter] := 7b;
  queuetran[mount] := 14b;
  queuetran[retrieve] := 17B;
  end;

procedure getpages(num:integer;var page:integer;var loc:messpt); extern;
procedure getpagex(num:integer;var page:integer;var loc:smesspt); extern;

procedure quit; extern;

function to6(x:alfa):integer; extern;

procedure fatal;
{Print most recent error and quit}
  begin
  jsys(erstr,3;priou,400000B:-1,0);
  quit  
  end;

function getqsr:integer;
{Returns Quasar's PID}
	var argblk:array[0..2]of integer;
  begin
  argblk[0] := mursp;  {get a cataloged PID}
  argblk[1] := spqsr;  {Quasar}
  jsys(mutil,2,ret;3,argblk);  {Now ask for the pid}
  if ret <> 2
    then fatal;
  getqsr := argblk[2]
  end;

function getme:integer;
{Gets and returns a PID for me}
	var argblk:array[0..2]of integer;
  begin
  argblk[0] := mucre;  {create a pid}
  argblk[1] := 400000B; {for me}
  jsys(mutil,2,ret;3,argblk);  {Now ask for the pid}
  if ret <> 2
    then fatal;
  getme := argblk[2]
  end;  

procedure getmem;
{Allocate memory}
  begin
  getpagex(1,smesspage,mess);
  getpages(1,messpage,m);  {a page for page mode, since it must be
			    on a page boundary}
  end;

procedure reqlist(howmuch:brevity;which:queueset;user:integer);
{Request a listing of queues}
  begin
  with mess^ do
{create a quasar message asking for a listing of the queues}
    begin
    qsrmsg[0] := 7000010B;	{length,,message type code}
    qsrmsg[1] := 400000000000B;  {ask for acknowlege message}
    qsrmsg[2] := 0;		{no ack code}
    case howmuch of		{flags - we use only the verbosity flag}
      notverbose: qsrmsg[3] := 0;
      verbose: qsrmsg[3] := 100000000000B;
      brief: qsrmsg[3] := 200000000000B
      end;
    qsrmsg[4] := 1;		{number of args}
    qsrmsg[5] := 2000000B;	{2 words,,arg type=tell us about these queues}
    conv.queues := which;
    qsrmsg[6] := conv.word;	{bits, one per queue}
    if user <> 0		{if he specifies a user name}
      then begin
      qsrmsg[0] := 11000010B;	{the block is longer}
      qsrmsg[4] := 2;		{it has 2 args}
      qsrmsg[7] := 2000001B;	{here is the extra arg: 2 words,,user spec}
      qsrmsg[10B] := user	{and the data: user number}
      end
    end;
  with packet do
    begin
    ipcfl := 0;
    ipcfs := mypid;
    ipcfr := qsrpid;
    ipcfpl := 9;
    ipcfpt := mess;
    end;
  jsys(msend,2,ret;4,packet);  {Now send the message}
  if ret <> 2
    then fatal;
  end;

procedure getack(dobrackets:Boolean);
{GETACK is a general routine used to wait for Quasar to send a response.
For this program, we will always get back a response that has some text
in it that should be printed on the terminal.  We don't bother to
analyze the messages.  If they have anything in them at all, the last
argument will be text.  So we just get the last argument.  Note that
sometimes the text will be too long for one message, in which case the
MORE bit will be set.  There is a slight complexity in that we don't
know in advance whether the messge will be send in page mode or normal
mode.  Quasar looks at the length, and uses page mode for big ones and
normal for small ones.  We first try page mode, and if that doesn't
work, try normal mode.  DOBRACKETS indicates we want to print [] around
it.  We do that in this routine instead of outside for reasons of
timing.  If you tried WRITE('['); GETACK; WRITE(']'), the result would
be [<long pause><message>].  The long pause is unesthetic, so we delay
doing the [ until we have the response to print.  That requires it to
be done in this routine.}

	var i,j,pos:integer;
	    pagemode:Boolean;
  begin
  repeat  {keep going as long as last message has MORE set, as it
	   indicates continuations}
    repeat  {keep going until we get something from QUASAR, in case someone
	     else sends us junk mail.  Also throw away trivial messages
	     from QUASAR.  Not sure why they happen}
      with packet do  {first try page mode}
        begin
        ipcfl := 200000B;  {Page mode}
        ipcfs := 0;
        ipcfr := mypid;
        ipcfpl := 512;
        ipcfpn := messpage
        end;
      jsys(mrecv,2,ret;4,packet;i);  {wait until there is something}
      if ret <> 2	    {Failed}
        then if i = ipcf16  {We get this if they sent normal mode}
               then begin   {since we tried to read it in page mode}
	       with packet do  {Try again in normal mode}
	         begin
	         ipcfl := 0; {normal mode}
	         ipcfs := 0;
	         ipcfr := mypid;
	         ipcfpl := 512;
	         ipcfpn := messpage * 1000B
	         end;
	       jsys(mrecv,2,ret;4,packet);  {get ack}
	       if ret <> 2		{If this fails, too, real error}
		 then fatal
	       end
	     else fatal;  {some other failure cause - real error}
     until (packet.ipcfs = qsrpid) and (m^.numargs > 0);
		{keep going until we get something from QUASAR and it is
		 non-trivial}

{The following delightful code finds the last "argument" in the message.
 POS is the position of the header of the argument we are currently looking
 at.  The only way to find the Nth argument is to skip the previous N-1}

    pos := 0;  {POS is kept in half-words}
    for i := 2 to m^.numargs do  {skip arguments til the last}
      pos := pos + 2*m^.halfs[pos];
    j := ((pos div 2) + 1) * 5;  {now convert offset to this arg to char's}
    if dobrackets then write(tty,'[');
    for i := j to j + 5*m^.halfs[pos] - 1 do  {write out the chars.
					       5*m^.half[pos] is the number of
					       chars.  m^.half[pos] is simply
					       the left half of the header
					       for the last arg}
      if m^.chars[i] = chr(0)  {it is ASCIZ, so stop on nul}
        then goto 1
        else write(tty,m^.chars[i]);
1:  if dobrackets then writeln(tty,']');
   until not m^.more
  end;

procedure dolist(howmuch:brevity;which:queueset;user:integer);
{INFO OUTPUT - main routine}
  begin
  qsrpid := getqsr;
  reqlist(howmuch,which,user);
  getack(false)
  end;

procedure startqueue(what:queue;jobname:alfa;var fname:fstring;flen:integer);
{PRINT, SUBMIT, etc.  These have to be done in three pieces because of
the presence of multiple switches.  It is not practical to have that many
arguments to the procedure.  So we have this procedure, which sets up
the part of the message containing required arguments.  Then the user
calls a routine for each switch he wants.  Each of these just appends an
argument specifying the data needed for that switch.  Finally, DOQUEUE
puts in a final count and sends off the message, waiting for ack if the
user wants to.}

	var conv:record case Boolean of
		true:(word:integer);
		false:(chars:packed array[0..4]of char)
		end;
	    i:integer;
  begin
  qsrpid := getqsr;
  with mess^ do
    begin
    qsrmsg[0] := 37B;	{message type - count will come at the end}
    qsrmsg[1] := 0;	{no switches}
    qsrmsg[2] := 0;	{no ack code}
    qsrmsg[3] := 0;	{no message-specific switches}
    qsrmsg[4] := 3;	{3 required arguments}
    qsrmsg[5] := 2000024B;	{count,,type of queue}
    qsrmsg[6] := queuetran[what];  {here is the queue type}
    qsrmsg[7] := 2000032B;	{count,,job name}
    qsrmsg[8] := to6(jobname);	   {here is the job name (sixbit)}
    qsrmsg[9] := (flen div 5 + 2) * 1000000B + 10B;  {count,,file name}
    for i := 0 to flen div 5 do	   {copies the file name}
      qsrmsg[10+i] := fname[i];
    conv.word := fname[flen div 5];  {make asciz by setting unused char's}
    for i := flen mod 5 to 4 do	     {in last word to 0}
      conv.chars[i] := chr(0);
    qsrmsg[10 + flen div 5] := conv.word;
    nextindex := 11 + flen div 5;    {set up NEXTINDEX so switch routines
				      know where to put their data}
    end;
  end;

procedure queueswitch(index:integer;value:integer);
{general routine to add a switch.  INDEX is the argument type code for
the particular switch.  VALUE is the value of the switch}

  begin
  mess^.qsrmsg[4] := mess^.qsrmsg[4] + 1;   {say we have one more arg}
  mess^.qsrmsg[nextindex] := index+ 2000000B;  {count,,switch type code}
  mess^.qsrmsg[nextindex+1] := value;	{value}
  nextindex := nextindex+2;	{advance so next switch goes after this}
  end;

procedure qtextswitch(index:integer;var name:fstring;len:integer);
{This is for switches whose value is text.  If you understand the last
two routines, you should understand this.}

	var conv:record case Boolean of
		true:(word:integer);
		false:(chars:packed array[0..4]of char)
		end;
  begin
  with mess^ do
    begin
    qsrmsg[4] := qsrmsg[4] + 1;
    qsrmsg[nextindex] := (len div 5 + 2) * 1000000B + index;
    qsrmsg[nextindex + 1 + len div 5] := 0;
    for i := 0 to len div 5 do
      qsrmsg[nextindex + 1 + i] := name[i];
    conv.word := name[len div 5];	{make asciz}
    for i := len mod 5 to 4 do
     conv.chars[i] := chr(0);
    qsrmsg[nextindex + 1 + len div 5] := conv.word;
    nextindex := nextindex + 2 + len div 5;
    end;
  end;

{Now here are the switches, in numerical order}

procedure qcopies(copies:integer);
  begin
  queueswitch(11B,copies);
  end;

procedure qform(name:alfa);
  begin
  queueswitch(12B,to6(name));
  end;

procedure qtapeform(form:ptpform);
  begin
  queueswitch(13B,ord(form))
  end;

procedure qdisposedelete;
  begin
  queueswitch(14B,1);
  end;

procedure qunit(unit:integer);
  begin
  queueswitch(15B,unit+3000000B)
  end;

procedure qlower;
  begin
  queueswitch(15B,1000000B)
  end;

procedure qupper;
  begin
  queueswitch(15B,2000000B)
  end;

procedure qafter(idtime:integer);
  begin
  queueswitch(16B,idtime)
  end;

procedure qlimit(limit:integer);
  begin
  queueswitch(17B,limit)
  end;

procedure qunique(unique:Boolean);
  begin
  queueswitch(20B,1+ord(unique))
  end;

procedure qrestart(restart:Boolean);
  begin
  queueswitch(21B,1+ord(restart))
  end;

procedure qoutput(log:outputlog);
  begin
  queueswitch(22B,ord(log))
  end;

procedure qaccount(var acctname:fstring;acctlen:integer);
  begin
  qtextswitch(23B,acctname,acctlen)
  end;

procedure qnode(node:alfa);
  begin
  queueswitch(25B,to6(node))
  end;  

procedure qusername(var username:fstring;userlen:integer);
  begin
  qtextswitch(26B,username,userlen)
  end;

procedure quser(user:integer);
  begin
  queueswitch(27B,user)
  end;

procedure qnotify(how:notification);
  begin
  queueswitch(30B,ord(how))
  end;

procedure qbatchlog(how:batchlog);
  begin
  queueswitch(31B,ord(how))
  end;

procedure qconnected(dir:integer);
  begin
  queueswitch(33B,dir)
  end;

procedure qnote(var notestr:sstring;notelen:integer);
{What fun!  This switch takes 1 - 12 char's, and converts them to sixbit}
	var i:integer;a:alfa;
  begin
  a := '          ';
  with mess^ do
    begin
    qsrmsg[4] := qsrmsg[4] + 1;
    if notelen <= 6   { <= 6 char's, will take one word for value}
      then begin
      qsrmsg[nextindex] := 2000034B;  {length,, arg type}
      for i := 1 to notelen do
	a[i] := notestr[i];
      qsrmsg[nextindex+1] := to6(a);
      nextindex := nextindex+2
      end
     else begin	{two words for value (3 total)}
      qsrmsg[nextindex] := 3000034B;  {length,, arg type}
      for i := 1 to 6 do
        a[i] := notestr[i];
      qsrmsg[nextindex+1] := to6(a);
      a := '          ';
      for i := 7 to notelen do
        a[i-6] := notestr[i];
      qsrmsg[nextindex+2] := to6(a);
      nextindex := nextindex + 3
      end;
    end;
  end;

procedure qbegin(block:integer);
  begin
  queueswitch(35B,block)
  end;

procedure qpriority(priority:integer);
  begin
  queueswitch(36B,priority)
  end;

procedure doqueue(doack:Boolean);
{Now we send the request}

  begin
  if doack   {If he wants an ack, set that flag}
    then mess^.qsrmsg[1] := 400000000000B;  
  mess^.qsrmsg[0] := nextindex * 1000000B + 37B;  {supply the total length}
  with packet do
    begin
    ipcfl := 200000B;  {Page mode}
    ipcfs := mypid;
    ipcfr := qsrpid;
    ipcfpl := 512;
    ipcfpn := smesspage;
    end;
  jsys(msend,2,ret;4,packet);  {Now send request}
  if ret <> 2
    then fatal;
  if doack		{and wait for ack if he wants it}
    then getack(true);
  end;

procedure dokill(what:queue;jobname:alfa;mask:integer;seqnum,reqnum:integer;
	         var owner:fstring;ownerlen:integer;doack:Boolean);
{Kill a request.  This is straightforward.  See the documentation for what
the args mean.  We just pass them to Quasar.}

	var conv:record case Boolean of
		true:(word:integer);
		false:(chars:packed array[0..4]of char)
		end;
  begin
  qsrpid := getqsr;
  with mess^ do
    begin
    qsrmsg[0] := 20000012B;  {length,,this is a kill}
    if doack   {if he wants ack, ask for it}
      then qsrmsg[1] := 400000000000B;  
    qsrmsg[2] := 0;   {no ack code}
    qsrmsg[3] := queuetran[what];   {queue type}
    qsrmsg[4] := to6(jobname);      {job name}
    qsrmsg[5] := mask;		    {job name mask}
    qsrmsg[6] := seqnum;
    qsrmsg[7] := reqnum;
    for i := 0 to ownerlen div 5 do   {owner - make it ASCIZ}
      qsrmsg[8+i] := owner[i];
    conv.word := owner[ownerlen div 5];
    for i := ownerlen mod 5 to 4 do
      conv.chars[i] := chr(0);
    qsrmsg[8+ownerlen div 5] := conv.word;
    end;
  with packet do
    begin
    ipcfl := 0;
    ipcfs := mypid;
    ipcfr := qsrpid;
    ipcfpl := 20B;
    ipcfpt := mess;
    end;
  jsys(msend,2,ret;4,packet);  {Now ask for kill}
  if ret <> 2
    then fatal;
  if doack			{and wait for ack if he wants it}
    then getack(true);
  end;

procedure qsrinit;
  begin
  mypid := getme;
  getmem
  end
.
