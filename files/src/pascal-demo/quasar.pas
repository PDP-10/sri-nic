{this file is designed to be INCLUDE'd by another program.  It should
 be loaded with QSR.REL.  There are three main functions supported:
	make a queue entry, STARTQUEUE and DOQUEUE
        kill a queue entry, DOKILL
	list queue entries, DOLIST
 In addition, there are routines for specifying switches for making
 queue entries.

 Before calling any of these routines, do QSRINIT

 Here is a demonstration program:

	program qsrtst;
	
	include 'quasar.pas';

	begin
	qsrinit;

	    (* PRINT PS:<HEDRICK>SYS.CTL/JOBNAME:SYS/USER:WATROUS/UNIT:5
	       /NOTIFY *)

	startqueue(printer,'SYS       ','ps:<hedrick>sys.ctl');
	qnotify(notifytty);
	qusername('WATROUS');
	qunit(5);
	doqueue(false);

	   (* INFO OUTPUT  - more or less, actually this specifies
	      only the printer, which you can't do in the EXEC *)

	dolist(notverbose,[printer],0);

	   (* CANCEL PRINT SYS/USER:WATROUS *)

	dokill(printer,'SYS       ',-1,0,0,'WATROUS',true);

	   (* INFO OUTPUT/FAST *)

	dolist(brief,[printer],0)
	end.


}

type
    queue=(batch,printer,cardpunch,papertape,plotter,dumqueue,mount,retrieve);
    queueset = set of queue;
    brevity=(notverbose,verbose,brief);
    ptpform=(tapedum,ptpascii,ptpimage,ptpimagebinary,ptpbinary);
    outputlog=(outlogdum,lognever,logalways,logerror);    
    notification=(notifdum,notifytty,notifymail,notifyjob);
    batchlog=(batlogdum,logappend,logsupercede,logspool);
    sstring=packed array[1..12]of char;

	{Do QSRINT before any of the other routines.  It gets a PID for the
	 fork and does some memory allocation}
procedure qsrinit;
  extern;
     

	{DOLIST - get a listing of one or more queues:
		HOWMUCH - /LONG /SHORT.  noverbose corresponds to the default.
		WHICH - which queues to list.  This is a set, so you can
			specify any combination you want.
		USER - if you want to see entries only for one user, put his
			user name here.  Use 0 to see them all}
procedure dolist(howmuch:brevity;which:queueset;user:integer);
  extern;

	{STARTQUEUE - make a request.  A request comes in three pieces:
		STARTQUEUE - sets up required arguments
		Qxxxx - set up switches
		DOQUEUE - actually do it
	 This design was used because there are so many switches that
	 it would be impossible to specify them by arguments to a single
	 function.
	     WHAT - which queue, e.g. BATCH, PRINTER
	     JOBNAME - an ALFA, of which the first 6 char's are used.
		Becomes the name of the request
	     FNAME - the file involved}
procedure startqueue(what:queue;jobname:alfa;fname:string);
  extern;

	{Here are the switch routines.  They correspond closely to
	 the switches in the EXEC.  I hope detailed documentation is
	 not needed}
procedure qcopies(copies:integer); extern;
procedure qform(name:alfa); extern;
procedure qtapeform(form:ptpform); extern;
procedure qdisposedelete; extern;
procedure qunit(unit:integer); extern;
procedure qlower; extern;
procedure qupper; extern;
procedure qafter(idtime:integer); extern;
procedure qlimit(limit:integer); extern;
procedure qunique(unique:Boolean); extern;
procedure qrestart(restart:Boolean); extern;
procedure qoutput(log:outputlog); extern;
procedure qaccount(acctname:string); extern;
procedure qnode(node:alfa); extern;
procedure qusername(username:string); extern;
procedure quser(user:integer); extern;
procedure qnotify(how:notification); extern;
procedure qbatchlog(how:batchlog); extern;
procedure qconnected(dir:integer); extern;
procedure qnote(var notestr:sstring;notelen:integer); extern;
procedure qbegin(block:integer); extern;
procedure qpriority(priority:integer); extern;

	{DOQUEUE - send off the request built up by STARTQUEUE and switches.
	 If DOACK, wait for QUASAR to respond with [job number...], and print
	 this on the terminal}
procedure doqueue(doack:Boolean); 
  extern;

	{DOKILL - kill a request.  You need not specify all of these
	 arguments.
		WHAT - a queue name, e.g. PRINTER.  Required.
		JOBNAME - a ALFA, of which the first 6 char's are used.
		    Only the bits for which corresponding bits are on in
		    MASK are used.  So to not specify this, use 0 for MASK.
		    (You would still have to specify a 10-char string though)
		MASK - specify which part of JOBNAME to use, allows things
		    like FOO*.  This would be done by 'FOO       ', with
		    MASK set to 777777000000B.
		SEQNUM, REQNUM - sequence number and external request numbers.
		    Leave 0 to ignore.
		OWNER - to ignore, use a 0-length field
		DOACK - if TRUE, waits for QUASAR to reply and types the
		    result, something like  [1 job killed]}
procedure dokill(what:queue;jobname:alfa;mask:integer;seqnum,reqnum:integer;
	         owner:string;doack:Boolean);
  extern
.
