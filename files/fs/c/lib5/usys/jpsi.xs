
<DOCUMENTATION.JSYS>ACCES.TXT.1

( 3.45) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>ADBRK.TXT.1

( 2.30) {software,interrupt,interrupt}
        1.  Set up  the  software  interrupt  system  for  interrupts  on

( 2.37) {interrupt}
        3.  When a channel 19 interrupt occurs, perform an RFSTS JSYS for

( 2.38) {interrupt}
            each  inferior  process.  The interrupted process that caused

( 3.26) {software,interrupt}
   a software interrupt on channel 19 (.ICIFT) if it has enabled for that

( 3.27) {interrupt}
   channel.  After processing the interrupt,  the  superior  process  can

( 4.43) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>AIC.TXT.1

( 2.8) {software,interrupt}
   Activates specific software interrupt  channels.   (Refer  to  Section

( 2.18) {software}
   The DIC monitor call can be  used  to  deactivate  specified  software

( 2.19) {interrupt}
   interrupt channels.

( 2.21) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>ATACH.TXT.1

( 2.15) {interrupt}
   ACCEPTS IN AC1:  B0(AT%CCJ) generate a CTRL/C interrupt to the  lowest

( 2.17) {interrupt}
                               CTRL/C interrupt if the job  is  currently

<DOCUMENTATION.JSYS>ATI.TXT.1

( 2.8) {software,interrupt}
   Assigns a terminal code to a software interrupt  channel.   (Refer  to

( 2.10) {interrupt}
   process' terminal interrupt mask.  (Refer to the STIW and RTIW monitor

( 2.24) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

( 2.30) {software,interrupt}
   ATIX1:    Invalid software interrupt channel number

<DOCUMENTATION.JSYS>BIN.TXT.1

( 2.24) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

<DOCUMENTATION.JSYS>BOOT.TXT.1

( 2.9) {software}
   and  dumping communications software.  The TOPS-20 system process that

( 6.5) {interrupt}
                          interrupts from the given DTE-20.

( 6.26) {interrupt}
                          ignores  interrupts  from  the  given DTE-20 or

( 14.31) {interrupt,software}
         23     .BTCHN    Set the  interrupt  channel  so  that  software

( 14.32) {interrupt}
                          interrupts will be generated when input data is

( 14.40) {software,interrupt}
                          1    .BTCOD    Software interrupt channel

( 15.14) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>BOUT.TXT.1

( 2.23) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

<DOCUMENTATION.JSYS>CFIBF.TXT.1

( 2.19) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>CFOBF.TXT.1

( 2.19) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>CHANM.TXT.1

( 1.70) {interrupt}
	Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>CHAPTER-0.TXT.1

( 2.20) {software}
   The software described in this document is furnished under  a  license

( 2.25) {software}
   reliability  of  its  software  on  equipment  that is not supplied by

( 4.11) {software,software}
           2.5     SOFTWARE DATA MODES  . . . . . . . . . . . . . .  2-63                   SOFTWARE DATA MODES

( 4.12) {software,interrupt,software,interrupt}
           2.6     SOFTWARE INTERRUPT SYSTEM  . . . . . . . . . . .  2-66                   SOFTWARE INTERRUPT SYSTEM

( 4.13) {software,interrupt,software,interrupt}
           2.6.1     Software Interrupt Channels                     Software Interrupt Channels

( 4.14) {software,interrupt,software,interrupt}
           2.6.2     Software Interrupt Priority Levels                     Software Interrupt Priority Levels

( 4.15) {software,interrupt,software,interrupt}
           2.6.3     Software Interrupt Tables                     Software Interrupt Tables

( 4.18) {interrupt,interrupt}
           2.6.6     Terminal Interrupts                     Terminal Interrupts

( 4.19) {interrupt,interrupt}
           2.6.7     Dismissing an Interrupt                     Dismissing an Interrupt

( 10.35) {software,interrupt}
|  and  tasks,  such  as  using the software interrupt system.  Chapter 3

( 11.48) {software}
|  Installation Guide            TOPS-20 Software Installation Guide                                 _______ ________ ____________ _____

<DOCUMENTATION.JSYS>CHAPTER-1.TXT.1

( 1.564) {software,interrupt}
|  monitor  generates  a  software  interrupt.   The  calling program can

( 1.565) {interrupt,software,interrupt}
|  process the interrupt by means of the software interrupt  system.   If

( 1.566) {interrupt}
|  the  program  is not prepared to process the interrupt, the process is

( 1.607) {software,interrupt}
|  an error without using the software interrupt system.  In fact, use of

( 1.608) {software,interrupt}
|  these symbols overrides the software interrupt system.

( 1.703) {interrupt}
   used  in  a  user  program, it causes an illegal instruction interrupt

<DOCUMENTATION.JSYS>CHAPTER-2.TXT.1

( 1.1179) {software}
   end-of-file  condition.   This  condition  may  generate  a   software

( 1.1180) {interrupt}
   interrupt (refer to Section 2.6) if the user has not included an ERJMP

( 1.1205) {software,interrupt}
   initiating  a software interrupt request (refer to Section 2.6) if the

( 1.1208) {interrupt,interrupt}
   interrupt,  the  interrupt  is  changed  into  a  process  terminating

( 5.4) {software}
                                           2  software reload

( 6.19) {interrupt}
                 11                number of terminal interrupts

( 11.14) {interrupt,software}
                           an  interrupt  on  software  channel .ICDAE.

( 11.21) {software}
     B14    MO%SER         Software   error.    (Would   generate    an

( 11.22) {interrupt}
                           interrupt on an assignable channel.)

( 11.25) {interrupt,software}
                           interrupt on software channel .ICDAE.)

( 12.40) {software}
     B14    MO%SER         Software error has occurred (would  generate

( 12.41) {interrupt}
                           interrupt on an assignable channel)

( 12.44) {interrupt}
                           interrupt on channel .ICDAE)

( 15.43) {interrupt,software}
                                   generates  an  interrupt  on  software

( 15.52) {software,interrupt}
    B14        MO%SER              Software   error   (e.g.,    interrupt

( 16.6) {interrupt,software}
                                   interrupt on software channel .ICDAE.

( 16.18) {interrupt}
    B34        MO%LCI              Character interrupt.   This  generates

( 16.19) {interrupt}
                                   an interrupt on channel .ICDAE.

( 33.28) {software,software}
   2.5  SOFTWARE DATA MODES        SOFTWARE DATA MODES

( 34.43) {software}
                             software.

( 36.13) {software}
                             software.

( 36.47) {software,interrupt,software,interrupt}
   2.6  SOFTWARE INTERRUPT SYSTEM        SOFTWARE INTERRUPT SYSTEM

( 36.49) {software}
   The monitor calls in this group are used for controlling the  software

( 36.50) {interrupt}
   interrupt  system.   Note  that  if  the program has an ERJMP or ERCAL

( 36.51) {interrupt}
   after a monitor call that normally causes an interrupt on failure, the

( 36.52) {interrupt}
   ERJMP  or ERCAL overrides the interrupt.  Refer to the TOPS-20 Monitor

( 36.53) {software}
   Calls User's Guide for an overview and  description  of  the  software

( 36.54) {interrupt}
   interrupt system.

( 37.4) {software,interrupt,software,interrupt}
   2.6.1  Software Interrupt Channels          Software Interrupt Channels

( 37.6) {interrupt,software,interrupt}
   Each interrupt  is  associated  with  one  of  36  software  interrupt

( 37.8) {interrupt,interrupt}
   various conditions, such  as  terminal  interrupts,  IPCF  interrupts,

( 37.9) {interrupt}
   ENQ/DEQ  interrupts,  PTY  conditions,  and  terminal buffers becoming

( 37.12) {interrupt}
   interrupts (IIC call).

( 37.16) {software,interrupt}
                        Software Interrupt Channels

( 38.13) {software,interrupt,software,interrupt}
   2.6.2  Software Interrupt Priority Levels          Software Interrupt Priority Levels

( 38.17) {interrupt}
   level 1 being the highest level interrupt.  Level 0  is  not  a  legal

( 38.18) {interrupt}
   priority level.  If an interrupt request occurs in a process where the

( 38.20) {interrupt}
   process  not  prepared  to  handle the interrupt.  The process is then

( 38.26) {software,interrupt,software,interrupt}
   2.6.3  Software Interrupt Tables          Software Interrupt Tables

( 38.28) {software,interrupt}
   Before using the software interrupt system, a process must set up  the

( 38.36) {interrupt}
        PC  at  the  time  of  the interrupt and flags for the associated

( 38.43) {interrupt}
        channel.  The right half contains the address  of  the  interrupt

( 38.44) {interrupt}
        routine that will handle interrupts on that channel.

( 38.50) {interrupt}
   If an interrupt is received on a channel that is  activated,  but  the

( 38.51) {interrupt}
   interrupt cannot be initiated because

( 39.4) {interrupt}
        1.  the interrupt system for the  process  is  not  enabled  (EIR

( 39.5) {interrupt}
            JSYS)  and  the  channel on which the interrupt occurred is a

( 39.16) {interrupt}
   then the interrupt is considered a process termination condition.   In

( 39.17) {interrupt}
   this  case  the  process  that  was  to have received the interrupt is

( 39.19) {interrupt}
   capabilities  word, and a process termination interrupt is sent to its

( 39.28) {interrupt}
   deactivated  by  disabling the channel or the entire interrupt system.

( 39.29) {software,interrupt}
   A software interrupt  received  on  a  panic  channel  that  has  been

( 39.36) {interrupt,interrupt}
   2.6.6  Terminal Interrupts          Terminal Interrupts

( 39.39) {interrupt}
   conditions on which interrupts can be initiated.  A process can assign

( 39.41) {interrupt}
   interrupt  channels  with  the  ATI call.  Once the particular code is

( 39.44) {interrupt}
   causes an interrupt to be generated.  The terminal codes,  along  with

( 39.49) {interrupt}
                          Terminal Interrupt Codes

( 40.40) {interrupt}
   The terminal code .TICRF (30) is used to generate  an  interrupt  when

( 40.42) {interrupt}
   process can enable for this interrupt, only the top-level  process  in

( 40.44) {interrupt}
   If other processes enable for the  interrupt,  they  can  receive  the

( 40.45) {interrupt}
   interrupt  either  when the carrier state changes to off or later when

( 40.51) {interrupt}
   interrupts   on  receipt  of  any  character  instead  of  a  specific

( 40.52) {interrupt}
   character.  The .TICTI code generates an interrupt when the terminal's

( 41.4) {interrupt}
   generates  an  interrupt  when  the  terminal's  output buffer becomes

( 41.5) {interrupt}
   nonempty.  Note that neither one of these codes generates an interrupt

( 41.10) {interrupt}
   determines  if  the  interrupt  is  initiated  immediately.   Terminal

( 41.11) {interrupt}
   interrupts are effectively deactivated when a process is frozen,  even

( 41.12) {interrupt,interrupt}
   though the interrupts are indicated in the process' terminal interrupt

( 41.14) {interrupt}
   terminal interrupts are automatically reactivated.

( 41.17) {interrupt}
   interrupt  word  for  the  job  (e.g.,  a  process  freeze or unfreeze

( 41.18) {interrupt}
   operation), the interrupt word for the job (and for the terminal  line

( 41.20) {interrupt}
   unfrozen processes  in  the  job.   When  an  interrupt  character  is

( 41.22) {interrupt}
   process to interrupt.

( 41.24) {interrupt}
   The user cannot directly access the actual  terminal  interrupt  word.

( 41.26) {interrupt}
   the RTIW or STIW JSYS's, he can read or change the terminal  interrupt

( 41.28) {interrupt}
   off interrupt codes activated by superior  processes.   Normally,  the

( 41.29) {interrupt}
   mask  is -1, thereby enabling all terminal interrupts to be activated.

( 41.31) {interrupt}
   interrupt  from  being active.  However, the fact that a code has been

( 41.35) {interrupt}
   Section 2.7.1) to activate the terminal code for CTRL/C interrupts.

( 41.38) {interrupt}
   interrupts  for  a  process.   Note  that the process must have SC%SCT

( 41.40) {interrupt}
   the source of terminal interrupts.

( 41.44) {interrupt,interrupt}
   2.6.6.1  Terminal Interrupt Modes - TOPS-20 handles the receipt  of  a            Terminal Interrupt Modes

( 41.45) {interrupt}
   terminal  interrupt  character  in  either  immediate mode or deferred

( 41.46) {interrupt}
   mode.  An interrupt character handled in  immediate  mode  causes  the

( 41.47) {software,interrupt}
   initiation of a software interrupt immediately upon its receipt by the

( 41.48) {interrupt}
   system (i.e., as soon as the user types it).  An  interrupt  character

( 41.50) {software,interrupt}
   software interrupt only when the program attempts to read it from  the

( 41.52) {interrupt}
   program.  If two occurrences of the same deferred interrupt  character

( 41.53) {interrupt}
   are  received  without any intervening character, the interrupt has an

( 42.5) {interrupt}
   full.   The  system  assumes  that  interrupts  are  to   be   handled

( 42.9) {interrupt}
   The purpose of deferred mode is to allow interrupt actions to occur in

( 42.11) {interrupt}
   multiple processes, the deferred interrupt occurs when any process  of

( 42.12) {interrupt}
   the  job  reads  the  interrupt character.  If this process is the one

( 42.13) {interrupt,interrupt}
   enabled for the interrupt, it will  be  interrupted  before  any  more

( 42.15) {interrupt,interrupt}
   interrupted is the top process, then the interrupt occurs before  more

( 42.20) {interrupt}
   before  the  interrupt can take effect.  This is unavoidable since the

( 42.21) {interrupt}
   process doing input and the process to be  interrupted  are  logically

( 42.26) {interrupt,interrupt}
   2.6.7  Dismissing an Interrupt          Dismissing an Interrupt

( 42.28) {interrupt,interrupt}
   Once the processing of an interrupt is complete, the user's  interrupt

( 42.29) {interrupt}
   routine  returns  control  to  the interrupted process by means of the

( 42.33) {interrupt}
   to  its state prior to the interrupt.  For example, if the process was

( 42.38) {interrupt}
   The  process  can  determine  if  an  interrupt  occurred  during  the

( 42.42) {interrupt}
   monitor code (i.e., a JSYS).  If the  interrupt  routine  changes  the

( 42.43) {interrupt}
   return  PC during the processing of an interrupt, the user-mode bit of

( 42.47) {interrupt}
   interrupt  the  execution of a monitor call), the process must turn on

( 42.50) {software,interrupt}
   The monitor calls for controlling the software interrupt system are:

( 42.52) {interrupt}
|       SIR       Sets the interrupt table addresses for a single-section

( 42.54) {interrupt}
|       XRIR%     Sets   the   interrupt   table    addresses    for    a

( 43.5) {interrupt}
|       RIR       Reads   the   interrupt   table   addresses    for    a

( 43.7) {interrupt}
|       XRIR%     Reads   the   interrupt   table   addresses    for    a

( 43.9) {interrupt}
        EIR       Enables the interrupt system

( 43.10) {interrupt}
        DIR       Disables the interrupt system

( 43.11) {interrupt}
        CIS       Clears the interrupt system

( 43.12) {interrupt}
        SKPIR     Skips if the interrupt system is enabled

( 43.13) {interrupt}
        AIC       Activates interrupt channels

( 43.14) {interrupt}
        IIC       Initiates interrupts on specific channels in a process

( 43.15) {interrupt}
        DIC       Deactivates interrupt channels

( 43.20) {interrupt}
        DEBRK     Dismisses current interrupt

( 43.23) {interrupt}
        STIW      Sets terminal interrupt word

( 43.24) {interrupt}
        RTIW      Reads terminal interrupt word

( 43.27) {interrupt}
        SCTTY     Changes source of terminal interrupts

( 43.34) {interrupt}
   the  monitor  and  to  enable  for  CTRL/C  interrupts, to be given to

( 44.17) {software,interrupt}
   0        SC%CTC     Process can enable for CTRL/C software interrupts.

( 44.40) {interrupt}
                       interrupts for other processes.

( 44.49) {software,interrupt}
   17       SC%FRZ     Unprocessed  software  interrupts  can  cause  the

( 46.52) {software}
            GETOK%  JSYS's  in  appropriate  places  in other software to

( 48.46) {interrupt}
   WAIT           Dismisses process until interrupt occurs

( 49.46) {interrupt}
                space, PC, AC's, interrupt system, or traps, and  the

( 66.21) {software}
        software

<DOCUMENTATION.JSYS>CHFDB.TXT.1

( 2.50) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>CIS.TXT.1

( 2.8) {software,interrupt}
   Clears the software interrupt system for the current process.   Clears

( 2.9) {interrupt,interrupt}
   all interrupts in progress and all waiting interrupts.

<DOCUMENTATION.JSYS>CLZFF.TXT.1

( 2.53) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>COMND.TXT.1

( 21.53) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>CRJOB.TXT.2

( 5.54) {interrupt}
                                                 interrupt  to be handled

( 7.50) {interrupt}
                          reached, an interrupt is generated (via a TIMER

<DOCUMENTATION.JSYS>DEBRK.TXT.1

( 2.8) {software,interrupt}
   Dismisses the current  software  interrupt  routine  in  progress  and

( 2.12) {software,interrupt}
   RETURNS     +1:  if no software interrupt is currently in progress and

( 2.15) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

( 2.19) {interrupt}
   DBRKX1:   no interrupts in progress

<DOCUMENTATION.JSYS>DELDF.TXT.1

( 2.47) {interrupt}
   was expunged.  Generates an illegal  instruction  interrupt  on  error

<DOCUMENTATION.JSYS>DEQ.TXT.1

( 3.32) {software,interrupt}
   ENQX11:   invalid software interrupt channel number

<DOCUMENTATION.JSYS>DIBE.TXT.1

( 2.20) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>DIC.TXT.1

( 2.8) {software,interrupt}
   Deactivates the specified  software  interrupt  channels.   (Refer  to

( 2.18) {software,interrupt}
   Software interrupt requests to deactivated channels are ignored except

( 2.19) {interrupt,interrupt}
   for  interrupts generated on panic channels.  Panic channel interrupts

( 2.23) {software,interrupt}
   The AIC monitor call is used to activate specified software  interrupt

( 2.26) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>DIR.TXT.1

( 2.8) {software,interrupt}
   Disables the software interrupt system for a process.

( 2.14) {software,interrupt,interrupt}
   If software interrupt  requests  are  generated  while  the  interrupt

( 2.16) {interrupt}
   the interrupt system is reenabled unless an intervening  CIS  call  is

( 2.17) {interrupt}
   executed.   However,  interrupts  on  panic  channels  will  still  be

( 2.20) {interrupt}
   an interrupt  that  cannot  be  disabled  with  a  DIR  call.   CTRL/C

( 2.21) {interrupt}
   interrupts  can  be  disabled by deactivating the channel to which the

( 2.24) {software,interrupt}
   The EIR monitor call can be used  to  enable  the  software  interrupt

( 2.27) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>DOBE.TXT.1

( 2.20) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>DSKAS.TXT.1

( 2.34) {software}
                               specified address is a software address.

( 2.53) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>DSKOP.TXT.1

( 4.5) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>DTI.TXT.1

( 2.8) {interrupt}
   Deassigns a terminal interrupt code.

( 2.19) {interrupt}
   Generates an illegal instuction interrupt on error conditions below.

<DOCUMENTATION.JSYS>DVCHR.TXT.1

( 2.54) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>ENQ.TXT.1

( 2.46) {software}
                            error  return  (ENQX6:) is taken.  A software

( 2.47) {interrupt}
                            interrupt  will  occur  when  all   requested

( 3.36) {software,interrupt}
        1    .ENQID         software interrupt channel number in the left

( 4.36) {software,interrupt}
   The software interrupt channel specifies the number of the channel  on

( 4.37) {interrupt}
   which  to generate an interrupt with the .ENQSI function.  The request

( 8.7) {software,interrupt}
   ENQX11:   invalid software interrupt channel number

<DOCUMENTATION.JSYS>ENQC.TXT.1

( 5.11) {software}
      .ENQDF  !    flags    !software chan! job # creator queue entry !

( 5.52) {software,interrupt}
   ENQX11:   invalid software interrupt channel number

<DOCUMENTATION.JSYS>EPCAP.TXT.1

( 2.31) {interrupt}
   Generates an illegal instruction  interrupt  on  the  following  error

<DOCUMENTATION.JSYS>ERSTR.TXT.1

( 2.37) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>ESOUT.TXT.1

( 2.26) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

<DOCUMENTATION.JSYS>FFFFP.TXT.1

( 2.19) {interrupt}
   Generates an illegal instruction  interrupt  on  the  following  error

<DOCUMENTATION.JSYS>FFORK.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction  interrupt  on  the  following  error

<DOCUMENTATION.JSYS>GACCT.TXT.1

( 2.27) {interrupt}
   Generates an illegal instruction  interrupt  on  the  following  error

<DOCUMENTATION.JSYS>GDSTS.TXT.1

( 2.42) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GDVEC.TXT.1

( 2.10) {software}
   RESTRICTIONS:    Requires RMS software (currently available only  with

( 2.20) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GET.TXT.1

( 4.37) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

( 4.45) {interrupt}
   Generates an illegal instruction  interrupt  on  the  following  error

<DOCUMENTATION.JSYS>GETOK.TXT.1

( 6.50) {interrupt}
   Generates an illegal instruction  interrupt  on  the  following  error

<DOCUMENTATION.JSYS>GEVEC.TXT.1

( 2.21) {interrupt}
   Generates an illegal instruction  interrupt  on  the  following  error

<DOCUMENTATION.JSYS>GFRKH.TXT.1

( 2.28) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GFRKS.TXT.1

( 3.33) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GFUST.TXT.1

( 2.30) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GIVOK.TXT.1

( 2.25) {interrupt}
   Returns an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GPJFN.TXT.1

( 2.18) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GTDAL.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GTDIR.TXT.1

( 3.18) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GTFDB.TXT.1

( 2.34) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GTJFN-LONG.TXT.1

( 2.32) {software,interrupt}
   All I/O errors can occur.  These errors cause software  interrupts  or

<DOCUMENTATION.JSYS>GTJFN-SHORT.TXT.1

( 2.26) {software,interrupt}
   All I/O errors can occur.  These errors cause software  interrupts  or

<DOCUMENTATION.JSYS>GTRPI.TXT.1

( 2.28) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GTRPW.TXT.1

( 2.33) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>GTTYP.TXT.1

( 2.21) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>HFORK.TXT.1

( 2.19) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>HPTIM.TXT.1

( 2.9) {interrupt}
   Although  the main time base from interrupts generated by the internal

<DOCUMENTATION.JSYS>IDTIM.TXT.1

( 4.13) {software}
   All I/O  errors  are  also  possible.   These  errors  cause  software

( 4.14) {interrupt}
   interrupts or process terminations as described under the BIN call.

<DOCUMENTATION.JSYS>IDTNC.TXT.1

( 3.18) {software}
   All I/O  errors  are  also  possible.   These  errors  cause  software

( 3.19) {interrupt}
   interrupts  or  process  terminations  as described under the BIN call

<DOCUMENTATION.JSYS>IIC.TXT.1

( 2.8) {software,interrupt}
   Initiates software interrupts on the specified channels in a  process.

( 2.14) {software,interrupt}
                    Bit n on  means  initiate  a  software  interrupt  on

( 2.19) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>JFNS.TXT.1

( 4.13) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>KFORK.TXT.1

( 2.11) {interrupt}
   any  terminal  interrupt  assignments  that were acquired from another

( 2.27) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>METER.TXT.1

( 2.40) {interrupt}
   Interrupts caused by IO,  paging,  swapping,  and  so  on,  can  cause

( 2.42) {interrupt}
   in the count.  Because these interrupts depend on a variety of  system

<DOCUMENTATION.JSYS>MONRD.TXT.1

( 1.134) {interrupt}
Generates illegal instruction interrupt if not implemented.

<DOCUMENTATION.JSYS>MSFRK.TXT.1

( 2.29) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>MSTR.TXT.1

( 2.33) {interrupt}
   Generates an illegal instruction interrupt on all error conditions.

( 3.26) {interrupt}
        15        .MSOFL       Yes         Receive  interrupt  when  disk

( 3.42) {software}
   and unit numbers are updated, and the software information  about  the

( 4.9) {software}
     3       .MSRST         Returned  software  status  of   unit.    The

( 5.51) {software}
   channel,  controller, and unit numbers are unchanged, and the software

( 21.5) {interrupt,interrupt}
   Receiving Interrupt when Disk Comes On-line - .MSOFL   Receiving Interrupt when Disk Comes On-line - .MSOFL

( 21.7) {interrupt}
   This function specifies who is to receive an  interrupt  when  a  disk

( 21.10) {interrupt}
   status.   Only  one process on the system will receive the interrupts.

( 21.16) {software,interrupt}
        0    .MSCHN         Place this process on  a  software  interrupt

( 21.17) {interrupt}
                            channel.  An interrupt is then generated when

( 21.20) {interrupt}
                            interrupt channel will be deassigned.

<DOCUMENTATION.JSYS>MTALN.TXT.1

( 2.36) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>MTOPR.TXT.1

( 2.18) {software}
                    software.

( 2.50) {interrupt}
    24       .MOAIN    Assigns  interrupt  channels  through  which   the

( 2.51) {interrupt}
                       program is interrupted on either a change of state

( 2.53) {psi}
                       INR message.  The INS/INR PSI channel is stored in

( 3.5) {psi}
                       PSI  channel is stored in field MO%FSM (B12-17) of

( 3.7) {psi}
                       fields prevents assignment of a PSI channel.

( 3.16) {software,interrupt}
    24       .MOACN    Allow a network task to enable software  interrupt

( 3.21) {interrupt}
                            o  interrupt message available

( 3.26) {interrupt}
                       interrupt assignments for this link.  These fields

( 3.32) {interrupt}
                       B9-B17    MO%INA    Interrupt message available

( 3.41) {interrupt}
                       .MOCIA    Clear the interrupt

( 4.12) {interrupt}
                       MO%INT    B7        Link has an interrupt  message

( 7.5) {interrupt}
    35       .MORIM    Read interrupt message.   This  function  requires

( 7.16) {interrupt}
    36       .MOSIM    Send an interrupt message.  This function requires

( 7.19) {interrupt}
                       of  the  bytes  in  the interrupt message (sixteen

( 8.46) {software,interrupt}
                            software interrupt).

( 18.20) {interrupt}
    40       .MOOFL    Enable interrupts for  online/offline  transition.

( 18.21) {interrupt}
                       Allows  a  process to be interrupted if a magnetic

( 18.25) {interrupt}
                       for  each  drive  for  which  interrupts are to be

( 18.27) {interrupt}
                       interrupts,  then  a  .MOSTA  function  should  be

( 18.28) {interrupt}
                       performed (for each drive) before  interrupts  for

( 18.29) {interrupt}
                       the  drives  are enabled.  Then, when an interrupt

( 18.33) {interrupt}
                       determined which drive (or drives) interrupted.

( 18.39) {software,interrupt}
    42       .MOPST    Declares the software interrupt channel to be used

( 18.48) {psi}
                       AC3 contains the PSI channel number to  set.   The

( 20.49) {psi}
                       switch  has occurred and the volume switch PSI has

( 21.23) {psi}
                       performed)  or when a PSI is generated, indicating

( 21.36) {psi}
                       volume  switch  sequence.  Once the PSI indicating

( 21.39) {interrupt}
                       operation must be performed at interrupt level.

( 21.51) {psi}
                       EOV PSI from  within  the  code  writing  the  UTL

( 21.53) {psi}
                       receive an  EOV  PSI  while  writing  the  trailer

( 23.24) {software}
                       meant to be read with DEC-supplied software.  This

( 24.9) {psi,software,interrupt}
    27       .MOPSI    Enable for a software interrupt on nonfatal device

( 24.19) {software}
                       Other device errors or software conditions are not

( 24.21) {software,interrupt}
                       software interrupt on channel 11 (.ICDAE).

( 24.26) {interrupt}
                       E+1:  interrupt channel number

( 25.40) {software}
                       E+1:  software status  word,  with  the  following

( 25.53) {software}
                             B14(MO%SER)  Clear   the   software    error

( 26.6) {interrupt}
                                          on a character interrupt.

( 26.19) {interrupt,psi}
                             generates   an   interrupt   if  the  .MOPSI

( 26.24) {interrupt}
                             counter and will not generate  an  interrupt

( 26.54) {software,interrupt}
                                          generates a software  interrupt

( 27.13) {software}
                             B14(MO%SER)  A   software    error    (e.g.,

( 27.14) {interrupt}
                                          interrupt    character,    page

( 27.18) {software}
                                          This error generates a software

( 27.19) {interrupt}
                                          interrupt   on    channel    11

( 27.39) {interrupt}
                             B34(MO%LCI)  A character interrupt occurred.

( 27.40) {software}
                                          This   generates   a   software

( 27.41) {interrupt}
                                          interrupt   on    channel    11

( 27.47) {software}
                             Bits 2-17 contain the software  status  word

( 28.17) {psi,software,interrupt}
    27       .MOPSI    Enable for a software interrupt on nonfatal device

( 28.24) {software}
                       Other device errors or software conditions are not

( 28.26) {software,interrupt}
                       software interrupt on channel 11 (.ICDAE).

( 28.31) {interrupt}
                       E+1:  interrupt channel number

( 28.44) {software}
                       E+1:  status word.  Bits 2-17 contain the software

( 28.52) {software}
                             B14(MO%SER)  Software  error  has   occurred

( 28.53) {interrupt}
                                          (would generate an interrupt on

( 29.6) {interrupt}
                                          (would  generate  interrupt  on

( 29.29) {psi,software,interrupt}
    27       .MOPSI    Enable for a software interrupt on nonfatal device

( 29.36) {software}
                       Other device errors or software conditions are not

( 29.38) {software,interrupt}
                       software interrupt on channel 11 (.ICDAE).

( 29.43) {interrupt}
                       E+1:  interrupt channel number

( 30.6) {software}
                       E+1:  status word.  Bits 2-17 contain the software

( 30.21) {software,interrupt}
                                          generates a software  interrupt

( 30.29) {software}
                             B14(MO%SER)  A   software    error    (e.g.,

( 30.30) {interrupt}
                                          interrupt character) occurred.

( 30.33) {software}
                                          This error generates a software

( 30.34) {interrupt}
                                          interrupt   on    channel    11

( 31.16) {interrupt}
    24       .MOAPI    Assign  PTY  interrupt  channels.   This  function

( 31.18) {interrupt}
                       B0(MO%WFI)      enable waiting-for-input interrupt

( 31.19) {interrupt}
                       B1(MO%OIR)      enable output-is-ready interrupt

( 31.20) {software,interrupt}
                       B12-B17(MO%SIC) software interrupt channel  number

( 34.25) {interrupt,interrupt}
|   51       .MOTPS    Assign terminal interrupt channels.  An  interrupt

( 34.30) {psi,psi}
|                      output  PSI channel,,input PSI channel.  All input

( 34.31) {psi}
|                      or  output  PSI  channels  for  the  terminal  are

( 34.47) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

( 35.47) {software,interrupt}
   MTOX14:   invalid software interrupt channel number

( 36.5) {software,interrupt}
   MTOX18:   invalid software interrupt channel number

<DOCUMENTATION.JSYS>MUTIL.TXT.1

( 5.52) {software,interrupt}
     22      .MUPIC    Place the specified PID on  a  software  interrupt

( 5.53) {interrupt}
                       channel.  An interrupt is then generated when:

( 6.11) {interrupt}
                           cause an interrupt.

( 8.23) {software,interrupt}
   IPCF22:   invalid software interrupt channel number

<DOCUMENTATION.JSYS>NODE.TXT.1

( 8.7) {interrupt}
|    11      .NDSIC    Set topology interrupt channel

( 8.18) {interrupt}
|                       0    .NDCHN    Channel number on which interrupts

( 8.21) {interrupt}
|    12      .NDCIC    Clear topology interrupt channel

( 8.24) {interrupt}
|                      interrupt  on  topology  change  (set  by function

<DOCUMENTATION.JSYS>ODCNV.TXT.1

( 3.7) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>ODTIM.TXT.1

( 3.53) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

( 4.11) {software}
   All I/O  errors  are  also  possible.   These  errors  cause  software

( 4.12) {interrupt}
   interrupts  or  process  terminations  as  described for the BOUT call

<DOCUMENTATION.JSYS>ODTNC.TXT.1

( 2.41) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

( 3.7) {software,interrupt}
   All I/O errors can occur.  These errors cause software  interrupts  or

<DOCUMENTATION.JSYS>OPENF.TXT.1

( 2.30) {software}
                                  on software data modes.)

( 2.48) {interrupt}
                                  interrupt,  the process will instead be

( 2.50) {interrupt}
                                  interrupt  will  be generated.  If this

( 2.52) {interrupt}
                                  the   interrupt  is  generated  on  its

<DOCUMENTATION.JSYS>PBIN.TXT.1

( 2.14) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

<DOCUMENTATION.JSYS>PBOUT.TXT.1

( 2.16) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

<DOCUMENTATION.JSYS>PDVOP.TXT.1

( 4.34) {interrupt}
|  This call generates an illegal  instruction  interrupt  on  the  error

<DOCUMENTATION.JSYS>PLOCK.TXT.1

( 2.48) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>PMAP.TXT.1

( 6.33) {software,interrupt}
   Can cause several software interrupts on certain file conditions.

( 6.35) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>PMCTL.TXT.1

( 4.27) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>PPNST.TXT.1

( 2.29) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>PRARG.TXT.1

( 3.17) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>PSOUT.TXT.1

( 2.15) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

<DOCUMENTATION.JSYS>RCDIR.TXT.1

( 5.29) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RCM.TXT.1

( 2.8) {interrupt}
   Returns the word mask of the  activated  interrupt  channels  for  the

( 2.10) {software,interrupt}
   for information on  activating  and  deactivating  software  interrupt

( 2.18) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RCUSR.TXT.1

( 3.30) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RCVOK.TXT.1

( 3.20) {interrupt}
   Returns an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RESET.TXT.1

( 2.22) {software,interrupt}
        3.  Clears the current process' software interrupt  system.   The

<DOCUMENTATION.JSYS>RFACS.TXT.1

( 2.21) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RFCOC.TXT.1

( 2.24) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RFORK.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RFPOS.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RFSTS.TXT.1

( 3.22) {software,interrupt}
       B18-B35(RF%SIC)      The number of the software interrupt  channel

( 3.27) {interrupt}
   generates  an  illegal  instruction  interrupt  if   the   handle   is

( 4.27) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RIN.TXT.1

( 2.26) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

<DOCUMENTATION.JSYS>RIR.TXT.1

( 2.24) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RIRCM.TXT.1

( 2.8) {software,interrupt}
   Returns the mask for reserved  software  interrupt  channels  for  the

( 2.18) {software,interrupt}
   software interrupt channels.

( 2.20) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RLJFN.TXT.1

( 2.12) {interrupt}
   not at interrupt level.  The GS%ASG bit returned from a GTSTS call for

<DOCUMENTATION.JSYS>RMAP.TXT.1

( 2.31) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>ROUT.TXT.1

( 2.27) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

<DOCUMENTATION.JSYS>RPACS.TXT.1

( 2.41) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RPCAP.TXT.1

( 2.19) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RSMAP.TXT.1

( 2.40) {interrupt}
|  Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RTFRK.TXT.1

( 2.11) {interrupt}
   can  receive only one interrupt at a time.  Thus, the superior process

( 2.12) {interrupt}
   should execute the RTFRK call after receiving an interrupt to identify

( 2.13) {interrupt}
   the process that caused the interrupt.

( 2.16) {interrupt}
   and  that are waiting to interrupt the superior process monitoring the

( 2.18) {interrupt}
   if the call is not executed, subsequent interrupts are not generated.

( 2.24) {interrupt}
                    that  generated the interrupt, and AC2 containing the

( 2.36) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RTIW.TXT.1

( 2.8) {interrupt}
   Reads the terminal interrupt word (refer to  Section  2.6.6)  for  the

( 2.9) {interrupt}
   specified process or the entire job and returns the terminal interrupt

( 2.13) {interrupt}
                               interrupts

( 2.18) {interrupt}
   RETURNS     +1:  always, with the terminal interrupt mask in AC2,  and

( 2.19) {interrupt}
                    the  deferred  terminal  interrupt  mask in AC3.  The

( 2.20) {interrupt}
                    deferred interrupt mask  is  returned  only  if  both

( 2.24) {interrupt}
   The STIW monitor call can be used to set the terminal  interrupt  word

( 2.27) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RUNTM.TXT.1

( 2.18) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>RWM.TXT.1

( 2.8) {interrupt,software}
   Returns the word mask for the interrupts waiting on software  channels

( 2.16) {interrupt}
                        interrupt on channel n is waiting.

( 2.18) {interrupt}
                    AC2 containing  the  status  of  the  interrupts   in

( 2.20) {interrupt}
                        interrupt of priority level  n  occurring  during

( 2.22) {interrupt}
                        on in  the  right  half  means  an  interrupt  of

( 2.26) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SAVE.TXT.1

( 2.37) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

( 2.40) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SCTTY.TXT.1

( 2.37) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

( 2.40) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

( 4.24) {interrupt}
   The effect of terminal interrupts on a  process  is  dictated  by  the

( 4.26) {interrupt}
   have enabled specific terminal characters will  receive  an  interrupt

( 4.32) {interrupt}
   source of all terminal  interrupts,  the  job's  controlling  terminal

( 4.38) {interrupt}
   controlling terminal and the source of terminal interrupts are changed

( 4.40) {interrupt}
   will  receive interrupts only from the new controlling terminal and no

( 4.42) {interrupt}
   receive  or  change  terminal  interrupts  from  any other controlling

( 5.32) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SDSTS.TXT.1

( 2.24) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SDVEC.TXT.1

( 2.10) {software}
   RESTRICTIONS:    requires RMS software (currently available only  with

( 2.32) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SETER.TXT.1

( 2.20) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SETJB.TXT.1

( 4.31) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SEVEC.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SFACS.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SFORK.TXT.1

( 2.33) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SFRKV.TXT.1

( 2.36) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SFTAD.TXT.1

( 3.38) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SFUST.TXT.1

( 2.37) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SIN.TXT.1

( 2.50) {interrupt}
   terminated  by  an  end-of-file interrupt, AC1 through AC3 are updated

( 3.19) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

( 3.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SINR.TXT.1

( 3.9) {software,interrupt}
   smaller than the actual record.  Can cause several software interrupts

( 3.13) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SIR.TXT.1

( 2.23) {interrupt}
   call, the new contents will be used on the next interrupt.

( 2.28) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SIRCM.TXT.1

( 2.8) {software,interrupt}
   Sets the  mask  for  reserved  software  interrupt  channels  for  the

( 2.9) {software}
   specified inferior process.  Conditions occurring on software channels

( 2.10) {interrupt}
   that have the corresponding mask bit set do not generate an  interrupt

( 2.18) {interrupt}
              AC3:  deferred terminal interrupt word

( 2.23) {software,interrupt}
   software  interrupt  channels.   Although  a  process can read its own

( 2.28) {interrupt}
   symbol after instructions that generate an interrupt on  failure,  the

( 2.29) {interrupt}
   ERJMP  or  ERCAL  will prevent the generation of the interrupt.  Thus,

( 2.33) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SJPRI.TXT.1

( 3.5) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SKPIR.TXT.1

( 2.8) {software,interrupt}
   Tests to see if the software  interrupt  system  is  enabled  for  the

( 2.13) {software,interrupt}
   RETURNS     +1:  failure, software interrupt system is off

( 2.15) {software,interrupt}
               +2:  success, software interrupt system is on

( 2.17) {software,interrupt}
   The EIR monitor call is used to enable the software interrupt  system,

( 2.20) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SMAP.TXT.1

( 5.39) {interrupt}
|  Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SMON.TXT.1

( 4.15) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SNOOP.TXT.1

( 2.41) {interrupt}
   interrupt level or if a patch has been made in the page fault  handler

( 2.46) {interrupt}
   interrupt  levels, the program must specify a reentrant instruction to

( 3.33) {interrupt}
                            interrupt  level.   On  return,  the   entire

<DOCUMENTATION.JSYS>SOUT.TXT.1

( 3.9) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

( 3.11) {interrupt}
   description.) Generates an  illegal  instruction  interrupt  on  error

<DOCUMENTATION.JSYS>SOUTR.TXT.1

( 2.45) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

( 2.49) {interrupt}
   Generates an illegal instruction interrupt on error conditions  below.

<DOCUMENTATION.JSYS>SPACS.TXT.1

( 2.37) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SPJFN.TXT.1

( 2.24) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SPRIW.TXT.1

( 2.21) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SSAVE.TXT.1

( 3.30) {software,interrupt}
   Can cause several  software  interrupts  or  process  terminations  on

( 3.33) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>STI.TXT.1

( 2.21) {interrupt}
   The DIBE call can be used to prevent sending  an  interrupt  character

( 2.35) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>STIW.TXT.1

( 2.8) {interrupt}
   Sets the terminal interrupt word (refer  to  Section  2.6.6)  for  the

( 2.10) {interrupt}
   characters that usually cause an interrupt are instead to be passed to

( 2.11) {interrupt}
   the  program as input.  In actuality, the STIW call sets the interrupt

( 2.13) {interrupt}
   job  or  process  should  receive  an interrupt.  The call's effect is

( 2.18) {interrupt}
   to the bits on in the mask will cause an interrupt if a process in the

( 2.19) {interrupt}
   job has enabled for an interrupt on that code.  If multiple  processes

( 2.21) {interrupt}
   interrupt.  (If several  processes  at  the  same  lowest  level  have

( 2.22) {interrupt}
   enabled   the  code,  the  process  that  receives  the  interrupt  is

( 2.27) {interrupt}
   all codes are declared to cause an interrupt (i.e., all  bits  in  the

( 2.30) {interrupt}
   interrupt character as input, it executes the STIW call for the entire

( 2.35) {interrupt}
   enabled by the specific process and cause an interrupt if in fact they

( 2.51) {interrupt}
   to  disable  the  code  for  CTRL/C  interrupts  or  to  give -5 as an

( 2.54) {interrupt}
   ACCEPTS IN AC1:  B0(ST%DIM) set the deferred terminal  interrupt  mask

( 3.10) {interrupt}
              AC2:  terminal interrupt word mask.

( 3.13) {interrupt}
              AC3:  deferred terminal interrupt word mask.

( 3.19) {interrupt}
   interrupt word mask, if B0(ST%DIM) is not set or if the process handle

( 3.22) {interrupt}
   If multiple processes enable the same interrupt character and any  one

( 3.26) {interrupt}
   The RTIW call can be used to obtain the terminal interrupt word masks.

<DOCUMENTATION.JSYS>STO.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>STPPN.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>STTYP.TXT.1

( 2.25) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SWJFN.TXT.1

( 2.17) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>SWTRP.TXT.1

( 2.11) {software,interrupt}
   the software interrupt system.

<DOCUMENTATION.JSYS>SYERR.TXT.1

( 2.24) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>TBADD.TXT.1

( 2.19) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>TBDEL.TXT.1

( 2.19) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>TBLUK.TXT.1

( 4.35) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>TFORK.TXT.1

( 2.17) {interrupt,interrupt}
|  process caused the interrupt, and how to handle the interrupt.  It can

( 2.24) {interrupt}
|  interrupt,  and  control is passed to that superior.  If each superior

( 2.29) {interrupt}
|  Note that an RTFRK should be performed when an interrupt is  received,

( 2.38) {software,interrupt}
              AC2:  software interrupt channel number in the  left  half,

( 3.16) {software}
         3        .TFSPS    Set the given software channel as the channel

( 3.17) {interrupt}
                            on which to generate the interrupt.

( 3.19) {software}
         4        .TFRPS    Return in the left half of AC2  the  software

( 3.20) {interrupt}
                            channel   on  which  the  interrupt  will  be

( 3.30) {software}
                            clear  the  software  channel assigned to the

( 3.31) {interrupt}
                            interrupt for monitor call intercepts.

( 4.10) {software}
   If the software channel is given as  77,  any  intercepts  bypass  the

( 4.11) {interrupt}
   given process without causing either an interrupt to its superior or a

( 4.35) {interrupt}
        3.  All interrupts for the monitored process are queued  and  are

( 5.15) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>TIMER.TXT.1

( 2.9) {interrupt}
   entire  job  can  run.   An  interrupt  is generated when the time has

( 2.21) {interrupt}
              AC2:  time at which to generate an interrupt.  Refer to the

( 2.25) {software}
              AC3:  number of the software channel on which  to  generate

( 2.26) {interrupt}
                    an interrupt when the time has expired.

( 2.40) {interrupt}
                            can  accumulate  before   an   interrupt   is

( 2.47) {interrupt}
                            interrupt is generated for the given process.

( 2.49) {interrupt}
                            can   now  elapse  before  the  interrupt  is

( 2.52) {interrupt}
         2        .TIMDT    Specify an exact time at which  an  interrupt

( 3.6) {interrupt}
                            interrupt is to be generated.

( 3.8) {interrupt}
         3        .TIMDD    Remove any pending  interrupt  requests  that

( 3.12) {interrupt}
                            of the interrupt request to be removed.   AC3

( 3.15) {interrupt}
         4        .TIMBF    Remove any pending  interrupt  requests  that

( 3.40) {software,interrupt}
   TIMX5:    invalid software interrupt channel number

<DOCUMENTATION.JSYS>TMON.TXT.1

( 3.42) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>TTMSG.TXT.1

( 2.24) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>USAGE.TXT.1

( 4.51) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>UTEST.TXT.1

( 3.14) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>UTFRK.TXT.1

( 2.17) {interrupt}
   will receive the interrupt.

( 2.33) {interrupt}
                          generation of an illegal instruction  interrupt

( 2.47) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>VACCT.TXT.1

( 2.18) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>WAIT.TXT.1

( 2.9) {software,interrupt}
   the software interrupt system is enabled for this process, the process

( 2.10) {interrupt}
   can be interrupted out of the wait state.  Upon execution of  a  DEBRK

( 2.11) {interrupt}
   call,  the  process  continues to wait until the next interrupt unless

( 2.12) {interrupt}
   the interrupt routine changes the PC word.  In this case, the  process

( 2.13) {interrupt}
   resumes  execution  at  the new PC location.  If the interrupt routine

<DOCUMENTATION.JSYS>WFORK.TXT.1

( 2.23) {interrupt}
   Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>XGTPW.TXT.1

( 2.42) {interrupt}
|  Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>XGVEC.TXT.1

( 2.20) {interrupt}
|  Generates an illegal instruction  interrupt  on  the  following  error

<DOCUMENTATION.JSYS>XRIR.TXT.1

( 2.25) {interrupt}
|         !          Address of the interrupt level table         !

( 2.30) {interrupt}
|  To see the format of the channel and interrupt level tables, refer  to

<DOCUMENTATION.JSYS>XRMAP.TXT.1

( 3.13) {interrupt}
|  Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>XSFRK.TXT.1

( 2.32) {interrupt}
|  Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>XSIR.TXT.1

( 2.25) {interrupt}
|         !          Address of the interrupt level table         !

( 2.30) {interrupt}
|  To see the format of the channel and interrupt level tables, refer  to

( 2.34) {interrupt}
|  call, the new contents will be used on the next interrupt.

( 2.39) {interrupt}
|  Generates an illegal instruction interrupt on error conditions below.

<DOCUMENTATION.JSYS>XSVEC.TXT.1

( 2.25) {interrupt}
|  Generates an illegal instruction interrupt on error conditions below.


	Lines recognized = 562
   String    Matches  Unrecognized Matches
1) "psi"	16	0
2) "software"	165	0
3) "interrupt"	528	0
Letter case ignored ("Ab" = "aB").

Files with no matches: 	<DOCUMENTATION.JSYS>ALLOC.TXT.1	<DOCUMENTATION.JSYS>ARCF.TXT.1
<DOCUMENTATION.JSYS>ASND.TXT.1	<DOCUMENTATION.JSYS>ASNSQ.TXT.1	<DOCUMENTATION.JSYS>ATNVT.TXT.1
<DOCUMENTATION.JSYS>BKJFN.TXT.1	<DOCUMENTATION.JSYS>CACCT.TXT.1	<DOCUMENTATION.JSYS>CFORK.TXT.1
<DOCUMENTATION.JSYS>CHKAC.TXT.1	<DOCUMENTATION.JSYS>CLOSF.TXT.1	<DOCUMENTATION.JSYS>CRLNM.TXT.1
<DOCUMENTATION.JSYS>CVHST.TXT.1	<DOCUMENTATION.JSYS>CVSKT.TXT.1	<DOCUMENTATION.JSYS>DELF.TXT.1
<DOCUMENTATION.JSYS>DELNF.TXT.1	<DOCUMENTATION.JSYS>DEVST.TXT.1	<DOCUMENTATION.JSYS>DFIN.TXT.1
<DOCUMENTATION.JSYS>DFOUT.TXT.1	<DOCUMENTATION.JSYS>DIAG.TXT.1	<DOCUMENTATION.JSYS>DIRST.TXT.1
<DOCUMENTATION.JSYS>DISMS.TXT.1	<DOCUMENTATION.JSYS>DTACH.TXT.1	<DOCUMENTATION.JSYS>DUMPI.TXT.1
<DOCUMENTATION.JSYS>DUMPO.TXT.1	<DOCUMENTATION.JSYS>FFUFP.TXT.1	<DOCUMENTATION.JSYS>FLHST.TXT.1
<DOCUMENTATION.JSYS>FLIN.TXT.1	<DOCUMENTATION.JSYS>FLOUT.TXT.1	<DOCUMENTATION.JSYS>GACTF.TXT.1
<DOCUMENTATION.JSYS>GCVEC.TXT.1	<DOCUMENTATION.JSYS>GDSKC.TXT.1	<DOCUMENTATION.JSYS>GETAB.TXT.1
<DOCUMENTATION.JSYS>GETER.TXT.1	<DOCUMENTATION.JSYS>GETJI.TXT.1	<DOCUMENTATION.JSYS>GETNM.TXT.1
<DOCUMENTATION.JSYS>GJINF.TXT.1	<DOCUMENTATION.JSYS>GNJFN.TXT.1	<DOCUMENTATION.JSYS>GTAD.TXT.1
<DOCUMENTATION.JSYS>GTDOM.TXT.3	<DOCUMENTATION.JSYS>GTHST.TXT.1	<DOCUMENTATION.JSYS>GTNCP.TXT.1
<DOCUMENTATION.JSYS>GTSTS.TXT.1	<DOCUMENTATION.JSYS>GTWAA.TXT.1	<DOCUMENTATION.JSYS>HALTF.TXT.1
<DOCUMENTATION.JSYS>HANDS.TXT.1	<DOCUMENTATION.JSYS>HSYS.TXT.1	<DOCUMENTATION.JSYS>IDCNV.TXT.1
<DOCUMENTATION.JSYS>IDLE.TXT.1	<DOCUMENTATION.JSYS>INLNM.TXT.1	<DOCUMENTATION.JSYS>LGOUT.TXT.1
<DOCUMENTATION.JSYS>LNMST.TXT.1	<DOCUMENTATION.JSYS>LOGIN.TXT.1	<DOCUMENTATION.JSYS>MDDT.TXT.2
<DOCUMENTATION.JSYS>MRECV.TXT.1	<DOCUMENTATION.JSYS>MSEND.TXT.1	<DOCUMENTATION.JSYS>MTU.TXT.1
<DOCUMENTATION.JSYS>NIN.TXT.1	<DOCUMENTATION.JSYS>NOUT.TXT.1	<DOCUMENTATION.JSYS>PEEK.TXT.1
<DOCUMENTATION.JSYS>RCVIM.TXT.1	<DOCUMENTATION.JSYS>RDTTY.TXT.1	<DOCUMENTATION.JSYS>RELD.TXT.1
<DOCUMENTATION.JSYS>RELSQ.TXT.1	<DOCUMENTATION.JSYS>RFBSZ.TXT.1	<DOCUMENTATION.JSYS>RFMOD.TXT.1
<DOCUMENTATION.JSYS>RFPTR.TXT.1	<DOCUMENTATION.JSYS>RFRKH.TXT.1	<DOCUMENTATION.JSYS>RFTAD.TXT.1
<DOCUMENTATION.JSYS>RNAMF.TXT.1	<DOCUMENTATION.JSYS>RSCAN.TXT.1	<DOCUMENTATION.JSYS>RWSET.TXT.1
<DOCUMENTATION.JSYS>SACTF.TXT.1	<DOCUMENTATION.JSYS>SCVEC.TXT.1	<DOCUMENTATION.JSYS>SETSN.TXT.1
<DOCUMENTATION.JSYS>SFBSZ.TXT.1	<DOCUMENTATION.JSYS>SFCOC.TXT.1	<DOCUMENTATION.JSYS>SFMOD.TXT.1
<DOCUMENTATION.JSYS>SFPOS.TXT.1	<DOCUMENTATION.JSYS>SFPTR.TXT.1	<DOCUMENTATION.JSYS>SIBE.TXT.1
<DOCUMENTATION.JSYS>SIZEF.TXT.1	<DOCUMENTATION.JSYS>SKED.TXT.1	<DOCUMENTATION.JSYS>SNDIM.TXT.1
<DOCUMENTATION.JSYS>SOBE.TXT.1	<DOCUMENTATION.JSYS>SOBF.TXT.1	<DOCUMENTATION.JSYS>SPLFK.TXT.1
<DOCUMENTATION.JSYS>SPOOL.TXT.1	<DOCUMENTATION.JSYS>STAD.TXT.1	<DOCUMENTATION.JSYS>STCMP.TXT.1
<DOCUMENTATION.JSYS>STDEV.TXT.1	<DOCUMENTATION.JSYS>STPAR.TXT.1	<DOCUMENTATION.JSYS>STSTS.TXT.1
<DOCUMENTATION.JSYS>SYSGT.TXT.1	<DOCUMENTATION.JSYS>TEXTI.TXT.1	<DOCUMENTATION.JSYS>THIBR.TXT.1
<DOCUMENTATION.JSYS>TIME.TXT.1	<DOCUMENTATION.JSYS>TLINK.TXT.1	<DOCUMENTATION.JSYS>TWAKE.TXT.1
<DOCUMENTATION.JSYS>UFPGS.TXT.1	<DOCUMENTATION.JSYS>USRIO.TXT.1	<DOCUMENTATION.JSYS>WILD.TXT.1
263 files searched, 101 without matches.
