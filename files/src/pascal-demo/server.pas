{$M-  This is a file of external procedures}
program server;

const
{jsys definitions}
  msend=510B;
  mrecv=511B;
  mutil=512B;
  erstr=11B;
{parameters}
  priou=101B;
  ipcii=3B;

type packetdescriptor=packed record
	ipcfl:integer;
	ipcfs:integer;
	ipcfr:integer;
	case Boolean of
	  true: (ipcfpl:0..777777B;ipcfpn:0..777777B;
		 ipcfd:integer);
	  false: (ipcxx:0..777777B;ipcfpt:^shortmessage)
	end;
     shortmessage=record
	ipci0:integer;
	ipci1:integer;
	ipci2:alfa
	end;
     messpt=^message;
     message=array[1:512]of integer;  {dummy definition}	

var packet:packetdescriptor;
    mess:^shortmessage;
    m:messpt;
    messpage:integer;  {page number of m^}
    ourpid,hispid:integer;
    ret:integer;
    i:integer;
    conv:packed record case Boolean of
	true:(word:integer);
	false:(dum:0..77777777B;error:0..77B)
	end;
    xwd:packed record case Boolean of
	true:(word:integer);
	false:(LH:0..777777B;RH:0..777777B)
	end;
    bits:set of 0..35; {standard word to put bits from jsys in}

procedure quit; extern;

procedure getpages(num:integer;var pagenum:integer;var pagept:messpt);
   extern;

procedure fatal;
{Print most recent error and quit}
  begin
  jsys(erstr,3;priou,400000B:-1,0);
  quit  
  end;

function server(name:alfa):messpt;
{Get a PID with name 'MAKEUSER'.  Returns PID as as OURPID.  Fatal if fails}
	var requestpid:packetdescriptor;
  begin
{Initialize storage}
  new(mess);
  getpages(1,messpage,m);
  server := m;
{send to info asking for pid and name}
  packet.ipcfl := 010000000000B;  {Create pid, jobwide}
  packet.ipcfs := 0;  {sender not assigned yet}
  packet.ipcfr := 0;  {to info}
  packet.ipcfpl := 3;  {create message for info - length 3}
  packet.ipcfpt := mess;
  mess^.ipci0 := ipcii;  {assign name to pid}
  mess^.ipci1 := 0;  {no duplicate}
  mess^.ipci2 := name;  {name of pid}
  jsys(msend,2,ret;4,packet);  {Now ask for the pid}
  if ret <> 2
    then fatal;
  ourpid := packet.ipcfs;   {This is the pid we got}
{receive response}
  packet.ipcfl := 0;  {No special flags}
  packet.ipcfr := ourpid;
  jsys(mrecv,2,ret;4,packet);  {Now get response}
  if ret <> 2
    then fatal;
  conv.word := packet.ipcfl;  {check for error}
  if conv.error <> 0
    then begin
    writeln(tty,'INFO error code ',conv.error:2:O);
    quit
    end;
  end;  

procedure getrequest;
{Receive request from user.}
  begin
  packet.ipcfl := 200000B; {Page mode}
  packet.ipcfr := ourpid;  
  packet.ipcfpl := 512;
  packet.ipcfpn := messpage;
  jsys(mrecv,2,ret;5,packet);  {Get user request}
  if ret <> 2
    then fatal;
  hispid := packet.ipcfs;
  end;  

procedure sendreply;
{Send acknowledgement to user.}
  begin
  packet.ipcfl := 200000B; {page mode}
  packet.ipcfs := ourpid;
  packet.ipcfpl := 512;
  packet.ipcfpn := messpage;
  packet.ipcfr := hispid; {back to sender}
  jsys(msend,2,ret;4,packet);
  if ret <> 2
    then fatal
  end.
