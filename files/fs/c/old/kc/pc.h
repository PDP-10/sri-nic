/*
 * pc.h - Parameter file for packet-communications module
 *
 * Some of these parameters are system-dependent.
 * Others can be used to tune the communications process.
 * Most are merely the interface declarations.
 */

/* Syntax sugar */

# define BOOL		int
# define FALSE		0
# define TRUE		1
# define NIL		0
# define NOTHING	int

/* Parameters */

# define NCycleBase	1000		/* MS between data input checks */
# define SPsense	3		/* Secs between Sense proddings */
# define SPclose	10		/* Secs to idle after closing */
# define SPgiveUp	60000		/* Secs before giving up */

/* Special-character definitions */

# define BOPchar	0176
# define EOPchar 	0012		/* Don't change this blithely */
# define QuoteChar	0175
# define CtrlS		('S'-0100)	/* XON/XOFF need quoting for */
# define CtrlSs		('S'-0100+0200)	/*  most systems */
# define CtrlQ		('Q'-0100)	/* (as well as their shifted */
# define CtrlQs		('Q'-0100+0200)	/*  versions) */

# define QuotedChars BOPchar,EOPchar,QuoteChar,CtrlS,/* CtrlSs,*/ CtrlQ ,CtrlQ

/* Communications-stream logical character bytes (plus 8-bit bytes) */

# define BYTE		int		/* A comm stream byte */
# define SpecialBase	0400		/* Start of special stream chars */
# define StreamMark	0400		/* Stream boundary marker */
# define NullByte	0401		/* (for internal use only) */
# define HostDead	0402		/* Received when other side dies */
# define FatalError	0403		/* Disastrous internal error seen */
# define EndMark	0404		/* Wrap up packet communications */
# define ErrReceived	0405		/* Error packet received */
# define DataWait	0406		/* Wait for data */

/* Special channels */

# define ErrChan	(-1)		/* Error-report channel */
# define NullChan	(-2)		/* The non-existent channel */

/* Interface routines - supplied by PC module */

/*
 * PCdrive(master)
 *
 * Called by the PC module user to run the communications process;
 * returns after an EndMark is seen (and handled) by PCobyte (q.v.).
 * The communications line (as accessed by PHin, PHout, and PHpeek)
 * should be set up appropriately before calling PCdrive.
 * The boolean master indicates whether we're the initiator (user) of
 * the communications channel, or the slave (server).
 */


/*
 * PCswitch(cp)
 *
 * Called by the PC module user to pass a switch string to PC.
 * The current switches (don't depend on them) are:
 *
 *  c<d>	set the cycle base in MS to <d> (a decimal #)
 *  m<d>	set the maximum packet size (<=MaxPktSize) to <d>
 *  d		comment on driver actions when debugging (# def debug 1)
 *  s		comment on packet sending actions when debugging
 *  r		comment on packet receiving actions when debugging
 *  t		dump statistics when closing down
 *  l		essentially, produce a "listing" of packets: pretend
 *		there's always window to send things
 *  
 */


/* Interface routines - supplied by the host module */

/*
 * byte = PHobyte(&chan)
 *
 * Called by the PC module to extract the next byte and its channel
 * to ship to the foreign host.  Chan is passed by reference and set
 * by PHobyte.  If EndMark is returned by PHobyte, communications
 * ceases immediately (all channels should be quiesced via StreamMarks
 * for reliable communications before this), and PCdrive returns.
 * If DataWait is returned by PHobyte (and chan should be NullChan),
 * then the PC driver will delay until a packet is received (this
 * is a way of "idling" a transmit side to allow reception to occur.)

BYTE PHobyte();

 */

/*
 * PHibyte(byte, chan)
 *
 * Called by the PC module to deliver a byte on a channel to the
 * local host module.  byte can be one of the special stream bytes
 * such as StreamMark, HostDead, FatalError, ErrReceived.  The
 * latter three, indicating some form of catastrophic failure,
 * assume that the PHibyte will never return.  The latter two
 * continue sending bytes, up through a StreamMark, with the error
 * message, on the error channel (ErrChan).

NOTHING PHibyte();
  
 */

/*
 * PHout(string, length)
 *
 * Called by the PC module to write a string to the communications
 * line, of length.  The local user environment is responsible for
 * having the communications line set up before the call to PCdrive.
 * On most systems, this means doing special things do the line
 * (such as putting it in "binary mode", etc.)

NOTHING PHout();

 */

/*
 * bytesread = PHin(string, maxlength)
 *
 * Called by the PC module to read a string from the communications
 * line, of maximum length maxlength, and ONLY UP TO an EOPchar
 * (as defined above.)  It returns the number of bytes read.

int PHin();

 */

/*
 * bytesavail = PHpeek()
 *
 * Called by the PC module to return the number of bytes available
 * for input on the communications line.

int PHpeek();

 */

/*
 * PHsleep(ms)
 *
 * Called by the PC module to sleep (delay) for ms milliseconds.
 * On some systems, which can't deal with time units less than a
 * second, ms will always be 1000 or more, according to the NCycleBase
 * and associated settings.

NOTHING PHsleep();

 */

/*
 * PHint(num, l)
 *
 * Called by the PC statistics routine to write a decimal integer of
 * left-space-padded length l.  If l is 0, then it means no padding.

NOTHING PHint();
 
 */

/*
 * PHstring(string)
 *
 * Called by the PC statistics routine to write an asciz string.

NOTHING PHstring();

 */

/*
 * PHfloat(flt, l)
 *
 * Called by the PC statistics routine to write a float of fractional
 * lenth l (e.g., PHfloat(3.1415926, 3) => 3.141)

NOTHING PHfloat();

 */

/*
 * secs = PHstamp()
 *
 * Called by the PC statistics routines to get a real-time time-stamp
 * in seconds.

int PHstamp();

 */

/*
 * PHnote(a1, a2, ...)
 *
 * Called by the PC routines to do a cprint on the debugging output
 * channel, when debugging.

NOTHING PHnote();

 */
