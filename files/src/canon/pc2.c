
/* updated for new imagen 2/2/83 by mef */

/*
 *                  pc - Packet Communication module
 *
 *						Chris Ryland
 *						August, 1981
 *
 *
 *  This module provides multi-channel reliable byte-stream
 * communications over potentially unreliable media to its users.
 *
 * at sri: notify and wait user when printer problems
 */

/* # define debug 1 */			/* Comment out for non-debug mode */

/* # define debugW	1	*/

					/* Comment out for "." echo on
						controlling terminal */
/* # define Echo */

/* Get our interface definitions */

# include "pc.h"
				/*SYNC*/
# include "hostms.h"

# include <stdio.h>
				/*SYNC*/

/* Parameters */
#define WINDOWFUDGE	128

/* Sizes */

# define PktOvhd	(1 + 2 + 4 + 1)	/* Encapsulation overhead */
# define DPktOvhd	(1 + 2 + 1)	/* Data packet overhead */
# define MaxPktSize	128		/* Maximum encapsulated length */
# define StsSize	(1 + 1 + 4 + 2 + 1) /* # of bytes in Status packet */
# define SeqSpace	128		/* seqno's in [0, SeqSpace) */
# define NullSeq	(-1)		/* the empty sequence number */
# define MaxUnackedPkts	(SeqSpace / 2 - 1) /* # unacked (outstanding) pkts */
# define InBufferSize	(MaxPktSize * 3) /* Our raw input buffer size */
# define OurWindow	0xFFFF		/* Essentially infinite window */

/* Sequence math */

# define SeqNext(n)	((n + 1) % SeqSpace)
# define SeqLT(a,b)	((((a) - (b)) & (SeqSpace>>1)) != 0)
# define SeqLE(a,b)	(SeqLT(a,b) || a == b)
# define SeqGE(a,b)	((((a) - (b)) & (SeqSpace>>1)) == 0)
# define SeqGT(a,b)	(SeqGE(a,b) && ((a) != (b)))

/* 16-bit left rotate for checksum computation */

# define rot16(v)	(((v<<1) | ((v>>15) & 1)) & 0177777)

/* Packet types and ancillary info */

# define DataPkt	'D'		/* Data packet: */
# define  ChanBase	'0'		/*  Channels are '0', '1', ... '9' */
# define  MkChanBase	'A'		/*  Ditto, with implicit Mark */
# define  MaxChans	10		/*  Max # of data channels */
# define StsPkt		'S'		/* Status packet: */
# define  InfoSts 	'i'		/*  info-only Status */
# define  RSVPsts	'r'		/*  please-reply Status */
# define  NAKsts	'n'		/*  Negative acknowledge */
# define SynchSts	's'		/*  synchronize */
# define ErrPkt		'E'		/* Protocol violation packet */

/* Quoting mechanism definitions */

# define QuoteBase	'A'		/* Where quoted char indices start */
# define QuoteLimit	'Z'		/*  and end */

/* Packet quoting mechanism */

int iqcmap[]= { QuotedChars };		/* Quoted index -> quoted char map */
char qcimap[256];			/* quoted char -> quoted index map */

/* Transmission variables */

char pktxq[SeqSpace*MaxPktSize];	/* re-xmission q */
int lpktxq[SeqSpace];			/* length of corr'ing pkt in pktxq */
# define Apktxq(pak) (&pktxq[MaxPktSize*pak])

/* Sequencing and flow control */
int synched;
int psLSent;				/* current seq #: last packet sent */
int psLRcv;				/* last-validly-rcvd seq # from frn */
int psLAcked;				/* largest seq # acked by frn host */
int psLNAKed;				/* sequence # in last NAK: if not equal
					   NullSeq, in rcv error-recovery */
int psReXmit;				/* != NullSeq, first pkt to rexmit */
int npssts;				/* # packets rcvd since we last ackd */
int npavail;				/* # packets available for xmission:
					   0 <= npavail <= SeqSpace/2 */
int owindow;				/* # bytes available at frn host */

						/*SYNC*/
/* System Status */
char otherStatus;			/* OTHER status */
char ourStatus;				/* OUR status */

# define 	RESCNT		40	/* Reset cycle count */
# define	STANULL		0
# define	STARESET	1
# define	STAPRINT	2
# define	STADONE		3
# define	STAFATAL	4
# define	STAATT		5

/* Reception variables */

char pktrcb[InBufferSize];		/* Input packet buffer */
char *pktrp;				/* pktrcb ref */
NOTHING PCrreset() { pktrp = pktrcb; }	/* Reset the reading process */


/* Connection-closing mechanism variables */

int ccyclesleft;			/* Closing cycles left */
int waitCnt;				/* Wait cycles count */	/*SYNC*/

/* General runtime-tuneable parameters */

int pktsize= { 0 };			/* Can change with conditions */
int databytes;				/* Current max # of data bytes */

int cyclebase= { 0 };			/* Current # MS per cycle */
int cpsense;				/* Cycles per sense */
int cpclose;				/* Cycles to idle after closing */
int cpgiveup;				/* Cycles to wait before giving up */

extern BOOL Logfile;			/* Indicates a log is being kept */


/*
 * PCswitch - handle a command-line switch beginning with -p
 */

char dswitches[26];			/* Debugging switches (a-z) */

/* Use this macro to reference debug switches, and use lower-case */
# define dswitch(s) (dswitches[s - 'a'])

PCswitch(cp)
char *cp;
{
	char s;

	switch (*cp) {

	case 'c':
		cyclebase = atoi(++cp);
		break;

	case 'm':
		pktsize = atoi(++cp);
		break;

	default:	
		Logfile = TRUE;
		while (s = *cp++)
			if (s >= 'A' && s <= 'Z')
				dswitches[s - 'A'] = TRUE;
			else if (s >= 'a' && s <= 'z')
				dswitches[s - 'a'] = TRUE;
		break;
	}
  return;
}
/*
 * PCinit - initialize everything for the PC module.
 *
 * Before this is called, all switch setting has taken place.
 */

PCinit(master)
BOOL master;
{
	char *cp;
	int i;

	/* Clear qcimap */
	cp = qcimap;
	for (i = 0; i++ < sizeof qcimap; )
		*cp++ = 0;

# ifdef debugI
		printf("Quote count %d\n",(sizeof iqcmap)/4);
# endif
	/* Set up quoting maps */
	for (i = 0; i < (sizeof iqcmap)/4; ++i) {
		if (i >= (QuoteLimit - QuoteBase + 1))
			PCdie("init: too many quoted characters");
		qcimap[iqcmap[i]] = i + QuoteBase;
# ifdef debugI
		printf("Quoting %o as %o\n",iqcmap[i],qcimap[iqcmap[i]]);
# endif
	}

	/* Set up initial tunable sizes */
	pktsize = (pktsize > 0 ? pktsize : MaxPktSize);
	databytes = pktsize - PktOvhd - DPktOvhd;

	/* Set up basic timing, and adjust other dependents */
	cyclebase = (cyclebase > 0 ? cyclebase : NCycleBase);
	PCadjcycles();

	/* Sequence number initialization */
	psLSent = SeqSpace - 1;
	psLAcked = psLSent;
	psLNAKed = NullSeq;
	psLRcv = SeqSpace - 1;
	psReXmit = NullSeq;

	/* Sequence space avail for unacked packets */
	npavail = (dswitch('l')) ? 0xFFFF : MaxUnackedPkts;

	/* No packets sent since last status (ack) we sent */
	npssts = 0;

	/* No window into other side yet (will come with 1st Status pkt) */
	owindow = (dswitch('l')) ? 0xFFFF : 0;
	
	/* Set up for packet reading */
	PCrreset();

	/* If we're initiating the connection, send an RSVP to start things 
	otherStatus= STANULL;	* undefined status *
	ourStatus= STARESET;	* undefined status *
	if (master)
		PCssts(RSVPsts);
*/
	/* If we're initiating the connection, send an RSVP to start things */
	synched = FALSE ;
	while (! synched) {
	    while(PCget())
		PCread() ;
	    PCssts(SynchSts) ;
	    PHsleep(cyclebase) ;
	}

}

/*
 * PCadjcycles - set the cyclic parameters, based on the value of
 *		 cyclebase
 */

PCadjcycles()
{
	cpsense = 1000 / cyclebase * SPsense;	/* ms/sec * cycle/ms * sec */
	cpclose = 1000 / cyclebase * SPclose;
	cpgiveup = 1000 / cyclebase * SPgiveUp;
}

/*
 * PCdrive - drive the communications process
 *
 * Handle character quoting and basic packetizing for transmission
 * at this level.  Returns after EndMark seen from host module.
 */

/* Reset the writing process, with and without stream marks */

# define PCflushMark {PCwrite(qpkt, qpp - qpkt, currChan, TRUE); qpp = qpkt;}
# define PCflush {PCwrite(qpkt, qpp - qpkt, currChan, FALSE); qpp = qpkt;}


NOTHING PCdrive(master)
BOOL master;
{
	static char qpkt[MaxPktSize], *qpp; /* Output accumulator */
	static int currChan;
	char qci;
	int qpc, chan;
	BYTE b;

	/* Initialize the communications world */
	PCinit(master);

							/*SYNC*/
	/* Get OTHER into reset state *
	PCwait (4,RESCNT);
	if (otherStatus!=STARESET) {
		PHres();
		PCwait (4,RESCNT);
		if (otherStatus!=STARESET) PCdie("Other system dead");
	}
							*SYNC*/
	qpp = qpkt;
	currChan = NullChan;

	/* Loop until EndMark seen from local user herein */
	while (TRUE) {

		/* Get a byte to be sent to the foreign host */
		b = PHobyte(&chan);

		/* Switch streams if necessary */
		if (chan != NullChan && chan != currChan) {
			if (currChan != NullChan && qpp > qpkt)
				PCflush;
			currChan = chan;
		}

		/* Check for a special stream character */
		if (b >= SpecialBase) switch(b) {

			/* Wait for some data to show up */
		case DataWait:
			PCdwait();
			break;
			
			/* Handle a stream mark: mark, delay till drained */
		case StreamMark:
			PCflushMark;
/*			ourStatus= STADONE;		*SYNC*/
			PCdrain();
			break;

			/* If end-mark, be sure everyone is happy, and leave */
		case EndMark:
/*			ourStatus= STADONE;		*SYNC*/
			PCclosewait();
			return;

			/* Something bogus here, die */
		default:
			PCdie("Invalid stream byte seen");

		} else {
		/* Ordinary character */

			qpc = qpp - qpkt;
			b &= 0377;
			if ((qci = qcimap[b]) != 0) {
				/* Needs to be quoted, flush if necessary */
				if (qpc + 2 > databytes)
					PCflush;

				/* Drop in the quoted pair */
# ifdef debug
				if (qci < QuoteBase || qci > QuoteLimit) {
					PHnote("Bad quoted char %o\n", qci);
					qci = '?';
				}
# endif
				*qpp++ = QuoteChar;
				*qpp++ = qci;
			} else {
				/* No quoting; flush if necessary */
				if (qpc + 1 > databytes)
					PCflush;

				/* Drop in this one character */
				*qpp++ = b;
			}
		}
	}
}

/*
 * Check for any input from the foreign host and handle it.
 * Then, delay until the given predicate proves true.
 */

PCwait(waitcase, arg)
int waitcase, arg;
{
	int sense, giveup;
	int wcond;
	char rply;

	sense = cpsense;		/* Init count-downers */
	giveup = cpgiveup;

	/* Termination test (the predicate funcall) in middle of loop */
	for (waitCnt= 0;1 ;waitCnt++) {		/*SYNC*/

		/* While there are potential packets, handle */
		while (PCget())
			PCread();

		/* Do any retransmission, if needed */
		PCrexmit();

		/* If predicate now true, we're done */
/*		if ((*pred)(arg))
			break;
*/	
		switch (waitcase) {
			case 1:
				wcond= PCdrnwait(arg); break;
			case 2:
				wcond= PCwclose(arg); break;
			case 3:
				wcond= PCspcwait(arg); break;
/* FRYD SUSPECTS THAT WE NEE TO EDIT HERE  - this doesn't match apollo *
			case 4:
				wcond= PCreswait(arg); break;	*SYNC*/
		}
# ifdef debugW
	printf("Wait sense:%d cond:%d\n",sense,wcond);
# endif
		if (wcond) break;

		/* Wait a while */
		PHsleep(cyclebase);

		/* If time is up, send a prodding RSVP Status */
		if (--sense < 0) {
# ifdef debugW
	printf(" Sense ");
# endif
			PCssts(RSVPsts);
			sense = cpsense;
		}

		/* If nothing concrete for a long time, tell re death */
		/* at sri, notify user and let him fix it */
		if (--giveup < 0) {
			pridwn();
/*
			PHibyte(HostDead, ErrChan);
			PHibyte(StreamMark, ErrChan);  */
		}
	}
}

/*	Wait for RESET Status

						*SYNC*
PCreswait(cycount)
int cycount;
{
	return ((otherStatus==STARESET) || (waitCnt>=cycount));
}
						*SYNC*/

/*
 * Wait for some data to show up.
 * Doesn't actually wait for data, just allows it to get through,
 * by waiting for a packet to show up before returning.
 */

PCdwait()
{
	while (TRUE)
		if (PCget()) {
			do
				PCread();
			while (PCget());
			return;
		} else
			PHsleep(cyclebase);
}

/*
 * TRUE when output is drained
 */

BOOL PCdrnwait()
{
	return (SeqGE(psLAcked, psLSent));
}

/*
 * Wait for all data to be sent and acknowledged
 */

PCdrain()
{
/*	PCwait(PCdrnwait, 0);
*/
	PCwait(1,0);
}

/*
 * TRUE when closing cycles run out
 */

BOOL PCwclose() { return (ccyclesleft-- < 0 /* || (otherStatus==STADONE)*/ ||
	   (otherStatus==STARESET) || (otherStatus==STAFATAL));	/*SYNC*/ }


/*
 * Delay for some time to make sure that both sides agree on things
 * being acknowledged fully, before closing up the connection.
 */

PCclosewait()
{
/*	PCwait(PCwclose, 0);
*/
	ccyclesleft = cpclose; 		/*SYNC*/
	PCwait(2,0);
}

/*
 * Send a Status packet of some flavor.
 * Sense packet format is "Stwwwwaae" where wwww is hex rep'n
 * of the receive window space left on our side, and aa is
 * next-expected packet sequence from the foreign host.
 * t is either i (info), r (RSVP), or n (NAK).  e is an error-
 * type byte, currently unused.
 */

PCssts(type)
char type;
{
	static char sts[StsSize];
	char *sp;
	int window;
# ifdef debugW
	printf("Send Status %c",type);
# endif

	sp = sts;

	/* Note that we've just sent an ack: no packets since last status */
	npssts = 0;

	/* Build Status packet of either flavor */
	*sp++ = StsPkt;
	*sp++ = type;

	/* Drop in our window */
	window = OurWindow;
	*sp++ = hex((window>>12) & 017);
	*sp++ = hex((window>>8) & 017);
	*sp++ = hex((window>>4) & 017);
	*sp++ = hex(window & 017);

	/* Ditto for last-received packet sequence # */
	*sp++ = hex((psLRcv>>4) & 017);
	*sp++ = hex(psLRcv & 017);

	/* Fill in (empty) error-type byte */
	*sp++ = ' ';

	/* If a NAK, remember in LNAK what we're acking (for error recovery) */
	if (type == NAKsts)
		psLNAKed = psLRcv;

	/* Otherwise, this an info ack (implying things are OK), reset LNAK */
	else if (type == InfoSts)
		psLNAKed = NullSeq;

	/* Send it off */
# ifdef debug
	if (dswitch('s'))
		PHnote("Sending %s Status: acking %d, window %d\n",
		       (type == RSVPsts ? "RSVP" :
			(type == InfoSts ? "info" :
			 (type == NAKsts ? "NAK" : "Synch"))),
		       psLRcv, window);
# endif
	PCship(sts, StsSize, type == RSVPsts);
}

/*
 * Return TRUE only when ok to send a packet of size size;
 * also waits for the # unacknowledged packets to get below
 * SeqSpace/2, the limit, and for the retransmission queue
 * to empty.
 */

BOOL PCspcwait(size)
int size;
{
	return (owindow >= size && npavail > 0 && psReXmit == NullSeq);
}

/*
 * Send off a data packet, queueing for re-transmission
 */

PCwrite(pktp, count, chan, mark)
char *pktp;
int count, chan;
BOOL mark;
{
	char *pp;

	/*
	 * Wait for room at the other end to open up; if there's
	 * no room now, poke the other side first to get room quickly.
	 */
	if (! PCspcwait(count)) {
# ifdef debugW
	printf(" PCwrite  ");
# endif
		PCssts(RSVPsts);
	}

/*	PCwait(PCspcwait, count);
*/
	PCwait(3,count);

	/* Get next sequence number */
	psLSent = SeqNext(psLSent);

	/* Steal as much of window as we need now */
	owindow -= count;
# ifdef debugW
	printf(" pcwrite win:%x ",owindow);
# endif
	--npavail;

	/* Now, build our re-transmittable data packet */
	pp = Apktxq(psLSent);
	lpktxq[psLSent] = count + DPktOvhd;
	*pp++ = DataPkt;
	*pp++ = hex((psLSent>>4) & 017);
	*pp++ = hex(psLSent & 017);
	*pp++ = chan + (mark ? MkChanBase : ChanBase);
	while (count-- > 0)
		*pp++ = *pktp++;

	/* Send this packet off */
# ifdef debug
	if (dswitch('s'))
		PHnote("Sending Data, seq %d, channel %d%s, length %d\n",
		       psLSent, chan, (mark ? " (stream mark)" : ""),
		       lpktxq[psLSent] - DPktOvhd);
# endif
	PCship(Apktxq(psLSent), lpktxq[psLSent], FALSE);
}

/*
 * Retransmit any packets needing it.
 */

PCrexmit()
{
	int seq, size;

	/* See if any work to do */
	if (psReXmit == NullSeq)
		return;

	/* Run through and re-xmit as many as will "fit" */
	seq = psReXmit;

	for (; SeqLE(psReXmit, psLSent); psReXmit = SeqNext(psReXmit)) {
		size = lpktxq[psReXmit] - DPktOvhd;

		/* If no more room available, get out */
		if (owindow < size || npavail <= 0)
			break;

		/* adjust window for this and # available */
		owindow -= seq;
		--npavail;

		/* Ship this off */
		PCship(Apktxq(psReXmit), lpktxq[psReXmit], FALSE);
# ifdef debug
		if (dswitch('r') || dswitch('s'))
			PHnote("Re-xmitting Data, seq %d\n", psReXmit);
# endif
	}

	/* Now, clear psReXmit if we have sent everything pending */
	if (SeqGT(psReXmit, psLSent))
		psReXmit = NullSeq;

# ifdef debug
	if (dswitch('r') || dswitch('s'))
		PHnote("Window now %d, # pkts avail now %d\n",
		       owindow, npavail);
# endif
}

/*
 * Look for any packet input and piece it together with
 * any previous partial input.
 * Return TRUE only when a potential complete packet is in hand.
 * pktrcb and pktrp are updated to reflect any packet data input.
 *
 * Somewhat system-dependent in that it assumes the PHpeek
 * routine can return the number of input bytes available on a
 * communications line.  Actually, it only assumes that PHpeek
 * can return 1 or 0, depending on whether there is input available
 * or not.
 */

BOOL PCget()
{
	int new;
	BOOL result;
	char	*ptrHack;

# ifdef debugS
	printf(" PCget ");
# endif
	/* Keep looping while data present */
	while ((new = PHpeek()) > 0) {

		/* Check for buffer overflow, and reset if so */
		if (new + (pktrp - pktrcb) > InBufferSize) {
			PCrreset();
# ifdef debugG
	printf(" Buffer oflo\n");
# endif
			break;
		}

		/* Read what's there now, breaking on the EOP char */
		new = PHin(pktrp, new);


		/* Account for data read */
# ifdef debugS
	printf(" [#%d,\'%o]",new,*pktrp);
# endif
		ptrHack= pktrp;
		pktrp = pktrp + new;

		/* Return TRUE iff we saw an end-packet character */
		if (new > 0 && *ptrHack == EOPchar) {
# ifdef debugG
			printf(" pakt ");
# endif
			return (TRUE);
		}
	}

	/* Once here, we ran out of data w/o seeing the EOP char */
	return (FALSE);
}

/*
 * Piece together a candidate packet, and
 * dispatch to appropriate handler
 */

/* On failure, reset reading process, and tell foreign host of lossage */
/* # define prFail(a){printf("FAILED a \n");
	               PCrreset(); PCssts(NAKsts); return;} */

# define prFail {				\
	*pktrp = '\0' ;				\
	PHnote("Bad pkt: %s\n", pktrcb) ;	\
	PCrreset() ;				\
	PCssts(NAKsts) ;			\
	return ;				\
}

# define ick(b)	(cksum = ((b) & 0xff) + rot16(cksum))

PCread()
{
	char *pp, *ep, *app;
	int cksum, pcksum, len, plen;
	char d1, d2, d3, d4, ptype;
	int dd1, dd2, dd3, dd4;

# ifdef debugG
	printf("\n PCread RCVD:");
	for(pp= pktrcb; pp<pktrp; printf("%c",*pp++));
# endif
	/* Prepare to dissect pktrcb */
	pp = pktrcb;
# ifdef debugS
	printf("{%o,%o}",pp,pktrcb);
# endif
	cksum = 0;

	/* Look for start of packet */
	while (*pp++ != BOPchar)
		if (pp > pktrp) prFail;
	ick(BOPchar);
# ifdef debugS
	printf("{%o,%c}",pp,*pp);
# endif

	/* Check for minimum length */
 	len = pktrp - pp;
	if (++len < PktOvhd)
		prFail;

	/* Get and check purported length */
	ick(dd1 = *pp++);
	ick(dd2 = *pp++);
	plen = unhex('0', '0', dd1, dd2);
# ifdef debugS
	printf(" (d d %o %o lg %d, plg %d) ",dd1, dd2, len, plen);
# endif
	if (plen < 0 || plen != len)
		prFail;

	/* Check opcode (packet type) */
	app = pp;
	if ((ptype = *app++) != DataPkt &&
	               ptype != StsPkt &&
		       ptype != ErrPkt)
		prFail;
	ick(ptype);

	/* Point at the (purported) EOPchar */
	ep = pktrp - 1;

	/* Check (and sum) end of packet char */
	if (*ep-- != EOPchar)
		prFail;

	/* Now, get and check purported checksum */
	d4 = *ep--;  d3 = *ep--;  d2 = *ep--;  d1 = *ep--;
	pcksum = unhex(d1, d2, d3, d4);
	while (app <= ep)
		ick(*app++);
	ick(EOPchar);

	/* Unless no checksumming wanted, check it now */
	if (! dswitch('c'))
		if (pcksum < 0 || pcksum != cksum) {
# ifdef debug
		 PHnote("Pkt cksum digits '%c%c%c%c', value %x, computed %x\n",
			       d1, d2, d3, d4, pcksum, cksum);
# endif
			prFail;
		}

	/* Pr<epare for next read */
	PCrreset();

	/* Dispatch to appropriate packet-input handler */
	PCdispatch(pp, len - PktOvhd);
}

/*
 * Dispatch on packet type
 *
 * pp refs start of abstract packet (packet type), len is
 * length of abstract packet (ie, total length minus
 * encapsulation)
 */

PCdispatch(pp, len)
char *pp;
int len;
{
	switch (*pp) {

	case DataPkt:
		PCdata(pp, len);
		break;

	case ErrPkt:
	case StsPkt:
		PCsts(pp, len);
		break;

	/* Undefined packet type: shouldn't be here */
	default:
		PCdie("dispatch: invalid packet type");
	}
}

/*
 * Handle Data packet
 * A data packet looks like "Dsscdddd..." where ss is the sequence
 * number of this data, and c is the channel designator.
 */

/* On failure, tell foreign host of lossage */
# define pdFail  {return (PCssts(NAKsts));}

/* Count but otherwise ignore an error */
# define pdIgnore  {return;}

PCdata(pp, len)
char *pp; int len;
{
	char c, d1, d2;
	int seq, chan;
	BOOL mark;

	/* Skip type byte and check lengths */
	++pp;
	if (len < DPktOvhd)
		pdFail;

	/* Get sequence number */
	d1 = *pp++;  d2 = *pp++;
	seq = unhex('0', '0', d1, d2);

	/* Get and check channel designator, noting stream mark */
	c = *pp++;
	if (c >= ChanBase && c < (ChanBase + MaxChans)) {
		chan = c - ChanBase;
		mark = FALSE;
	} else if (c >= MkChanBase && c < (MkChanBase + MaxChans)) {
		chan = c - MkChanBase;
		mark = TRUE;
	} else
		pdFail;

	/* Check sequence number */
	if (seq != SeqNext(psLRcv))

		/* If we are in error recovery, or see an old pkt, ignore */
		if (psLNAKed != NullSeq || SeqLE(seq, psLRcv)) {
			pdIgnore;
		} else {
		/* We have a strange sequence number, complain */
# ifdef debug
			if (dswitch('r'))
				PHnote("Rcvd seq #%d; expected #%d\n",
				       seq, SeqNext(psLRcv));
# endif
			pdFail;
		}

	/* Now, update the last-received sequence number */
	psLRcv = SeqNext(psLRcv);

	/* Pull out data bytes, unquoting as we do, and ship them out */
	len =- DPktOvhd;
# ifdef debug
	if (dswitch('r'))
		PHnote("Rcvd Data: seq %d, channel %d%s, length %d\n",
		       psLRcv, chan, (mark ? " (stream mark)" : ""), len);
# endif
	while (len-- > 0) {
		if ((c = *pp++) == QuoteChar) {
			if (len-- < 0) PCerr("Quotechar without quoted");
			c = *pp++;
			if (c < QuoteBase || c > QuoteLimit)
				PCerr("Quoted rep out of bounds");
			c = iqcmap[c - QuoteBase];
		}
		PHibyte(c, chan);
	}

	/*
	 * If time to ack (end-stream mark, # of unacked packets getting
	 * too high, we are seeing the first good packet since a NAK),
	 * tell foreign host about this successful reception
	 */
	if (mark || (++npssts > (MaxUnackedPkts / 2)) || psLNAKed != NullSeq)
# ifdef debugW
	printf(" PCdata ");
# endif
		PCssts(InfoSts);

	/* Propagate any stream mark on this channel */
	if (mark)
		PHibyte(StreamMark, chan);
}

/*
 * Handle Status packet
 *
 * The status packet looks like "Stwwwwaae", where t is the
 * type (informational only, RSVP sensing, or NAK), wwww is the
 * hex representation of the window at the other side,
 * aa is the last-validly received packet sequence number
 * at the other side, and e is an error-type byte code.
 *
 * Also handle Error packets, which look like Status packets
 * but which have an error message after them.
 */

# define stFail  {return;}

PCsts(pp, len)
char *pp;
int len;
{
	char stype, opc, etype;
	int ack, window, d1, d2, d3, d4;

	/* Get the opcode (ErrPkt or StsPkt) and check for error */
	opc = *pp++;

	/* Check this packet's length and ignore if ill-formed */
	if (opc == StsPkt && len != StsSize)
		stFail;

	/* Check the type */
	stype = *pp++;
	if (stype != InfoSts && stype != RSVPsts && stype != NAKsts &&
	stype != SynchSts)
		stFail;

	/* Dig out and check the foreign host's current receive window */
	d1 = *pp++;  d2 = *pp++;  d3 = *pp++;  d4 = *pp++;
	if ((window = unhex(d1, d2, d3, d4)) < 0)
		stFail;
	owindow = ((3 * window) / 4) - WINDOWFUDGE;
# ifdef debugW
	printf(" win:%x ",owindow);
# endif
	
	/* Dig out the last-validly-received packet info */
	d1 = *pp++;  d2 = *pp++;
	if ((ack = unhex('0', '0', d1, d2)) < 0)
		stFail;

	/* Get error-type byte */
	etype = *pp++;

	/* If error, tell user of fatal condition */
	if (opc == ErrPkt) {
		len =- StsSize;
		PHibyte(ErrReceived, ErrChan);
		while (len-- > 0)
			PHibyte(*pp++, ErrChan);
		PHibyte(StreamMark, ErrChan);
	}

						/*SYNC*/
	if (opc == StsPkt && stype == SynchSts)
	    synched = TRUE ;
#ifdef SYNC
	if (opc == StsPkt && stype == InfoSts) {
		switch (etype) {
			case MSGRESET:
				otherStatus= STARESET;
				break;
			case MSGDONE:
				otherStatus= STADONE;
				if (ourStatus!=STADONE && ourStatus!=STARESET) 
					PCdie("IMPRINT-10 done prematurely\n");
				break;
			case MSGFATAL:
				otherStatus= STAFATAL;
				if (ourStatus!=STARESET)
				 PCdie ("IMPRINT-10 ended with a fatal error\n");
				break;
			case MSGLBP(MSGLBPNR):
				PHAttR("Printing Engine not responding\n");
				otherStatus= STAATT;
				break;
			case MSGLBP(MSGLBPNR+1): case MSGLBP(MSGLBPNR+2):
			case MSGLBP(MSGLBPNR+3): case MSGLBP(MSGLBPNR+4):
			case MSGLBP(MSGLBPNR+5): case MSGLBP(MSGLBPNR+6):
			case MSGLBP(MSGLBPNR+7): case MSGLBP(MSGLBPNR+8):
			case MSGLBP(MSGLBPNR+9): case MSGLBP(MSGLBPNR+10):
			case MSGLBP(MSGLBPNR+11): case MSGLBP(MSGLBPNR+12):
			case MSGLBP(MSGLBPNR+13): case MSGLBP(MSGLBPNR+14):
			case MSGLBP(MSGLBPNR+15):
				PHAttR("IMPRINT-10 requires attention\n");
				otherStatus= STAATT;
				break;
			case MSGLBPOK:
				otherStatus= STAPRINT;
				break;
			default:
				otherStatus= STANULL;
				break;
		}
	}
						/*SYNC*/
#endif

# ifdef debug
	if (dswitch('r'))
		PHnote("Rcvd %s Status: acking %d, window %d, error %c\n",
		       (stype == RSVPsts ? "RSVP" :
			(stype == InfoSts ? "info" : 
			 (stype == NAKsts ? "NAK" : "Synch"))),
		       ack, window, etype);
# endif

	/*
	 * Now, if an RSVP Status, send our state; if in
	 * error-recovery state, send NAK to inform other side
	 */
	if (stype == RSVPsts) {
# ifdef debugW
	printf(" PCsts ");
# endif
		PCssts(psLNAKed == NullSeq ? InfoSts: RSVPsts);
	}

	/* Handle this ack: do pruning, rexmission, etc. */
	PChack(ack, stype == NAKsts);
}

/*
 * Handle an acknowledgement from the foreign host
 *
 * Note that receiving acknowledgement sequence number "ack"
 * means that (a) all packets sent up through ack can be
 * "pruned" from the re-transmission queue, and (b) all packets
 * with sequence greater than ack must be re-transmitted when
 * a NAK is received, so we mark them as needing re-xmission.
 */

PChack(ack, nak)
int ack;
BOOL nak;
{
	int sseq;

	/*
	 * Make sure ack falls within the bounds of last-acked
	 * and last-sent
	 */
 	if (SeqLT(ack, psLAcked) || SeqGT(ack, psLSent))
		return;

	psLAcked = ack;

	/*
	 * Mark packets needing re-transmission, if any.
	 * Update it even if not a NAK if there is an outstanding request.
	 */
	if (nak || psReXmit != NullSeq)
		psReXmit =
		  SeqLT(psLAcked, psLSent) ? SeqNext(psLAcked) : NullSeq;

	/* Now, the difference between the ack and last-sent is # avail */
	npavail = MaxUnackedPkts - SeqDiff(psLSent, ack);

# ifdef debug
	if (dswitch('r')) {
		if (psReXmit != NullSeq)
			PHnote("Pkts in [%d, %d] queued for rexmission\n",
			       psReXmit, psLSent);
		PHnote("Packets avail: %d, window: %d\n", npavail, owindow);
	}
# endif
}

/*
 * Ship off a packet, ref'ed by pp, of length len.
 * Encapsulate it for the journey.
 * clear is true when our caller wants to clear up any
 * hangage due to a dropped XON or spurious XOFF (eg,
 * when a sensing Status packet is sent.)
 */

# define oock(b)		cksum = ((b) & 0xff) + rot16(cksum)
/*
 # define ock(b) {caux= b; oldchk= cksum; oock(caux); dumpchk;}
 # define dumpchk printf("\n CHK %x %x  %x\n",oldchk,caux,cksum)
*/

# define ock(b) {caux= b; oock(caux);}

char caux;
int oldchk;

PCship(pp, len, clear)
char *pp;
int len;
BOOL clear;
{
	int cksum, plen, clen;
	static char header[1 + 2], trailer[4 + 1];
	char *hp, *tp, *dp, d1, d2, d3, d4;

	cksum = 0;

	/* Double-check lengths here */
	if ((plen = len + PktOvhd) > pktsize || len <= 0)
		PCdie("ship: length screwup");

	/* Set up header: BOP and length (cksumming all the while) */
	hp = header;
	*hp++ = BOPchar;
	ock(BOPchar);
	d1 = hex((plen>>4) & 017);
	*hp++ = d1;
	ock(d1);
	d2 = hex(plen & 017);
	*hp++ = d2;
	ock(d2);

	/* Checksum all the abstract data contents */
	dp = pp;
	clen = len;
	while (clen-- > 0) {
		ock(*dp++);
	}

	/* Set up trailer: checksum and EOPchar */
	tp = trailer;
	ock(EOPchar);			/* Here to get it into cksum */
	d1 = hex((cksum>>12) & 017);
	*tp++ = d1;
	d2 = hex((cksum>>8) & 017);
	*tp++ = d2;
	d3 = hex((cksum>>4) & 017);
	*tp++ = d3;
	d4 = hex(cksum & 017);
	*tp++ = d4;
	*tp++ = EOPchar;

# ifdef Echo
	PHack();	/* Echo a "." on the controlling terminal */
# endif
	/* Now, ship out the 3 pieces to the comm line */

	if (clear) PHout("\021", 1);
	PHout(header, 1 + 2);
	PHout(pp, len);
	PHout(trailer, 4 + 1);
		FluRem();
}

/* Small utilities */

/*
 * Return difference of a and b in sequence space
 */

SeqDiff(a, b)
{
	int diff, c;
	if (SeqLT(a, b)) {
		c = b;
		b = a;
		a = c;
	}

	if ((diff = a - b) < 0)
		return (diff + SeqSpace);
	else	return (diff);
}


/*
 * Turn the value v into a hex digit
 */

int hex(v)
{
	v = v & 017;
	return ((v < 10) ? (v + '0') :
		 (v - 10 + 'A'));
}


/*
 * Un-hexify, returning a negative number if not valid hex
 * (assumes a larger-than-16-bit word machine!)
 */

int unhex1(d)
{
	if (d >= '0' && d <= '9')
		return (d - '0');
	else if (d >= 'A' && d <= 'F')
		return (d - 'A' + 10);
	else	return (-1);
}

int unhex(d1, d2, d3, d4)
{
        d1 = unhex1(d1)<<12;
        d2 = unhex1(d2)<<8;
        d3 = unhex1(d3)<<4;
        d4 = unhex1(d4);
# ifdef debugS
	printf(" (UNHEX %x %x %x %x)",d1,d2,d3,d4);
# endif
	return (d1+d2+d3+d4);
}


/*
 * Die by shipping off an error message over the error
 * channel.
 */

PCdie(msg)
char *msg;
{
	PHibyte(FatalError, ErrChan);
	fprintf(stderr,msg);
	while (*msg)
		PHibyte(*msg++, ErrChan);
	PHibyte(StreamMark, ErrChan);
}


/*
 * Die by shipping off an Error packet to the other side and
 * then doing a normal PCdie.
 */

PCerr(msg)
char *msg;
{
	/*** Unfinished: ship off error packet ***/
	PCdie(msg);
}
