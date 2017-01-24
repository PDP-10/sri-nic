{$M-  This is a file of external procedures}
program user;

const
{jsys definitions}
  msend=510B;
  mrecv=511B;
  mutil=512B;
  erstr=11B;
{parameters}
  priou=101B;
  ipciw=1B;
  
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
    message=array[1:512]of integer;
    messpt=^message;
     
var packet:packetdescriptor;
    mess:^shortmessage;
    m:messpt;
    messpage:integer;  {page number of m^}
    ourpid,hispid:integer;
    ret:integer;
    userlen,i,j:integer;
    conv:packed record case Boolean of
	true:(word:integer);
	false:(dum:0..77777777B;error:0..77B)
	end;

procedure getpages(num:integer;var page:integer;var loc:messpt); extern;

procedure quit; extern;

procedure fatal;
{Print most recent error and quit}
  begin
  jsys(erstr,3;priou,400000B:-1,0);
  quit  
  end;

function user(name:alfa):messpt;
{Find the PID with name 'MAKEUSER'. Returns PID as as HISPID.  Fatal if fails}
	var requestpid:packetdescriptor;
  begin
{Allocate memory}
  new(mess);
  getpages(1,messpage,m);
  user := m;
{send to info asking for pid and name}
  packet.ipcfl := 010000000000B;  {Create pid, temp}
  packet.ipcfs := 0;  {sender not assigned yet}
  packet.ipcfr := 0;  {to info}
  packet.ipcfpl := 3;  {create message for info - length 3}
  packet.ipcfpt := mess;  {use short message}
  mess^.ipci0 := ipciw;  {get pid for name}
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
  conv.word := packet.ipcfl;  {check for error}
  if conv.error <> 0
    then begin
    if conv.error = 76B
      then writeln(tty,'? The server you have asked to talk to is not running')
      else writeln(tty,'? Unable to find the server you have asked to talk to - INFO error code ',conv.error:2:O);
    quit
    end;
  if ret <> 2
    then fatal;
  hispid := mess^.ipci1;  {This was what info said}
  end;  

procedure sendrequest;
{Receive request from user.}
  begin
  packet.ipcfpl := 512;
  packet.ipcfpn := messpage;
  packet.ipcfl := 200000B; {Page mode}
  packet.ipcfs := ourpid;
  packet.ipcfr := hispid;
  jsys(msend,2,ret;4,packet);  {send user request}
  if ret <> 2
    then fatal;
  end;  

procedure getreply;
{Send acknowledgement to user.}
  begin
  packet.ipcfl := 200000B; {Page mode}
  packet.ipcfs := hispid;
  packet.ipcfpl := 512;
  packet.ipcfpn := messpage;
  packet.ipcfr := ourpid;
  jsys(mrecv,2,ret;4,packet);
  if ret <> 2
    then fatal
  end.
