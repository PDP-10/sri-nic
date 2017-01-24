program xpatch(them);
 {This program is supplied as a simple example of interrupt handling.
  Actually I think it may have a slight timing problem, but it will give
  the idea. It crosspatches your terminal to another one, or to a PTY:.
  I.e. anything you type it gets and visa versa.  E.g. if you dial out
  from your computer to another one, this could let you communicate with
  it.

  On Tops-10 this program would be symmetrical, driven by interrupts on
  the channels on which the two terminals are openned.  However here it is
  only possible to get an interrupt for the controlling terminal, so the
  other is done in a loop.

  To get out of this, you type ^A. }  

label 1;
const
  sibe=102B;
  priin=100B;
  ati=137B;
  rfmod=107B;
  sfmod=110B;
  stiw=174B;
  fhslf=400000B;
  epcap=151B;
type
  byte=0..377B;
var
  us,them:file of byte;
procedure fromus;
	 {FROMUS is activated whenever the input buffer from our terminal
	  becomes non-empty (after having been empty).  We must keep
	  reading characters until it becomes empty, or we may hang.}
    var ret,count:integer;
  begin
  loop
    jsys(sibe,2,ret;0:us;count,count);   {Any more char's waiting?}
   exit if (ret = 2) or (count = 0);     {No}
    for count := count downto 1 do	 {SIBE returns how many char's}
      begin
      get(us);				 {Copy each one}
      if (us^ = 1) or (us^ = 201B)
        then goto 1;
      them^ := us^;
      put(them)
      end
   end;
  end;

procedure psidefine(chan,level:integer;procedure p); extern;
procedure psienable(chan:integer); extern;

begin
update(us,'TTY:',true,0,100000000000B,1);  {open in 8-bit mode, BIN/BOUT}
update(them,'',true,0,100000000000B,1);
jsys(epcap;fhslf,0,400000B:0);		   {turn on ability to trap ^C}
jsys(stiw;0:-5,20B);   		{turn off all char interrupts except 31}
psidefine(35,1,fromus);		{call fromus on chan 35 int., level 1}
psienable(35);			{turn on those interrupts}
jsys(ati;31:35);		{assign int 31 (incoming char) to chan 35}
fromus;				{prevent possible race}
while true do			{now do them to us, us to them at int. level}
  begin
  get(them);
  us^ := them^;
  put(us)
  end;

1:				{here when type ^A}
end.
