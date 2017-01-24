/*
 * is - Imagen-10 spooler
 */

/* Get environment definitions */

# include <stdio.h>

/* Get packet communications module definitions */

# include "pc.h"

# define ATTENTION

		/* Remove comments if you wish Protocol to hold on
				on requests for attention */

/* # define Hold */	
# define Hold 

/* Useful definitions */

# define defaultLog	"isLog"
# define DataChannel	1		/* All data is on this channel */

/* For now, we talk to the I10 over the standard tty line */


/*
 # define I10fdi		stdin
 # define I10fdo		stdout
*/

/* # define Idebug */


/* Globals */
FILE *infd, *logfd;

BOOL debug, specialChars, vswitch, diagsw;
int pcount = 0;				/* Page count */
int ndiagbytes;				/* Number of bytes to send in test */
int testb = 0377;			/* For use in diagnostic mode */

/*
struct sgttyb savetty,setty;
*/
BOOL	inRawMode= FALSE;
BOOL	HasOpened= FALSE;		/* Suppresses closing of unopened files */
BOOL	Logfile  = FALSE;		/* Generate log file */

extern int bsize();

/* Enter here */

main(argc, argv)
int argc;
char *argv[];
{
	int bsz;
    	char *cp, *fs, *ls, ccc;

	ls = NIL;			/* No file specs yet */
	fs = NIL;
	diagsw = vswitch = specialChars = debug = FALSE;

	/* Scan args */
	while (--argc > 0) {
		cp = *++argv;		/* pick up next arg */

		/* Non-switch must be our filespec */
		if (*cp++ != '-')
			fs = *argv;

		/* Otherwise, a switch */
		else switch (ccc= *cp++) {

			/* Debugging */
		case 'd':
			debug = TRUE;
			break;

			/* a PC module switch; pass it through */
		case 'p':
			PCswitch(cp);
			break;

			/* Log file spec */
		case 'l':
			if (! --argc < 0)
			    /*	die("usage: is -l logfile input"); */
				ls = *++argv;
			Logfile = TRUE;
			break;

			/* print special chars (to find Quoting errors) */
		case 'q':
			specialChars = TRUE;
			break;

			/* Run "diagnostics" (test mode) */
		case 't':
			diagsw = TRUE;
			ndiagbytes = atoi(*++argv);
			break;

			/* view packets as they come out */
		case 'v':
			PCswitch("l");		/* Turn on packet-logging */
			vswitch = TRUE;
			break;
		default:
			printf("Watcha doing? %c %o\n",ccc,ccc);
		}
	}

	/* open the file (unless in test mode) */
	if (! diagsw) {
		if (fs == NIL)
				die("usage: is -l logfile input");
			if  ((bsz = bsize(fs)) == 8)
			infd = fopen(fs, "R") ; else
			infd = fopen(fs, "r") ;

			if (infd == NULL)
			die("%s: can't open for read", fs);
	}

	/* get our log file */
	if (Logfile) {
		if (ls == NIL)
			ls = defaultLog;
		if (*ls == '-')
			logfd = stdout;
		else if ((logfd = fopen(ls, "w")) == NULL)
			die("%s: can't open for write as log file", ls);
	}

	/* Set up the Imagen-10 communications line */
	i10init();

	/* do the actual file shipment (we're the master) */
	PCdrive(TRUE);

	/* shut down the I10 line */
	i10close();

	/* report the number of pages printed */
	Report(pcount);

	/* close our input file and log file */
	if (! diagsw) fclose(infd);
}

/*
 * Setup the I10 line
 */


i10init()
{
	if (vswitch) return;
/*
	system("mesg n");

	setbuf(stdout,NULL);

	gtty(fileno(stdin),&savetty);
	gtty(fileno(stdin),&setty);
	setty.sgflags|= RAW;
	stty(fileno(stdin),&setty);
	inRawMode= TRUE;
*/
	if (openremote() == NULL) {
		die("Can't open Canon line\n");
		}
	HasOpened= TRUE;
}


/*
 * shut down the I10 line
 */

i10close()
{
/*	if (inRawMode) {
		stty(fileno(stdin),&savetty);
		system("mesg y");
	}
*/
	if (HasOpened) closeremote();
	if (Logfile) fclose(logfd);
		return;
}

/* 
 * Report the number of page output
 */

Report(count)
int count;
{
	fprintf(stderr, "%5d",count);
	fprintf(stderr, " pages printed");
	if (count > 0) Repacc();
}

/* The following definitions are used by the page counter the parse the
	input stream */


# define ignore		1
# define smglyw		2
# define smglyh		3
# define biglwa		4
# define biglwb		5
# define biglha		6
# define biglhb		7
# define scan		8
# define movscan	9

# define Isgly		198
# define Ibgly		199
# define Ipage		213
# define Iepage		219

# define Imov		130
# define Ihpos		195
# define Ivpos		196
# define Istsp		210
# define Istbol		209
# define Istil		208
# define Idelg		201
# define Idelf		202
# define Istf		207
# define Isrl		192
# define Ibrl		193

/* varaibles used by the input stream parser of the page counter */

int state = scan;
int nstate = scan;
int icnt = 0;
int height = 0;
int width = 0;


/*
 * Give the next byte to the communications module
 */

BYTE PHobyte(chan)
int *chan;
{
	static BOOL done = {FALSE};
	int i;

	/* Assume no channel at this point */
	*chan = NullChan;

	/* If we've previously seen the EOF, say we're done now */
	if (done)
		return (EndMark);

	/* Otherwise, get a byte; from here on, use the data channel only */
	*chan = DataChannel;

	/* If in test mode, send next byte in line, unless done */
	if (diagsw) {
		if (ndiagbytes-- <= 0) {
			i = StreamMark;
			done = TRUE;
		} else {
			if (testb == 0377)
				testb = 0;
			else 	++testb;
			i = testb;
		}
	} else
		i = getc(infd);

	/* The following code parses the input stream to calculate 
		the number of pages printed */

		switch (state) {
		
		case ignore:
			--icnt;
			if (icnt == 0) 
				state = nstate;
			break;
		
		case movscan:
			if (i == Imov) state = scan;
			break;

		case smglyw:
			width = i;
			icnt = 1; state = ignore;
			nstate = smglyh;
			break;

		case smglyh:
			height = i;
			width = (width+7)/8;
			icnt = height*width+1;
			state = ignore; nstate = scan;
			break;

		case biglwa:
			width = i * 256;
			state = biglwb;
			break;

		case biglwb:
			width = i + width;
			icnt = 2; state = ignore;
			nstate = biglha;
			break;

		case biglha:
			height = i * 256;
			state = biglhb;
			break;

		case biglhb:
			height = i + height;
			width = (width+7)/8;
			icnt = height*width+2;
			state = ignore; nstate = scan;
			break;

		case scan:
			switch (i) {

			case Imov:
# ifdef Idebug
				fprintf(stderr," Imov ");
# endif
				state = movscan;
				break;

			case Ihpos:
# ifdef Idebug
				fprintf(stderr," Ihpos ");
# endif
				icnt = 2; state = ignore;
				break;

			case Ivpos:
# ifdef Idebug
				fprintf(stderr," Ivpos ");
# endif
				icnt = 2; state = ignore;
				break;

			case Istsp:
# ifdef Idebug
				fprintf(stderr," Istsp ");
# endif
				icnt = 2; state = ignore;
				break;

			case Istbol:
# ifdef Idebug
				fprintf(stderr," Istbol ");
# endif
				icnt = 2; state = ignore;
				break;

			case Istil:
# ifdef Idebug
				fprintf(stderr," Istil ");
# endif
				icnt = 2; state = ignore;
				break;
				
			case Idelg:
# ifdef Idebug
				fprintf(stderr," Idelg ");
# endif
				icnt = 2; state = ignore;
				break;
			
			case Idelf:
# ifdef Idebug
				fprintf(stderr," Idelf ");
# endif
				icnt = 1; state = ignore;
				break;

			case Istf:
# ifdef Idebug
				fprintf(stderr," Istf ");
# endif
				icnt = 1; state = ignore;
				break;

			case Isrl:
# ifdef Idebug
				fprintf(stderr," Isrl ");
# endif
				icnt = 3; state = ignore;
				break;

			case Ibrl:
# ifdef Idebug
				fprintf(stderr," Ibrl ");
# endif
				icnt = 6; state = ignore;
				break;

			case Isgly:
# ifdef Idebug
				fprintf(stderr," Isgly ");
# endif
				icnt = 3; state = ignore;
				nstate = smglyw;
				break;

			case Ibgly:
# ifdef Idebug
				fprintf(stderr," Ibgly ");
# endif
				icnt = 4; state = ignore;
				nstate = biglwa;
				break;

			case Iepage:
# ifdef Idebug
				fprintf(stderr," Iepage ");
# endif
				++pcount;
				break;
# ifdef Idebug
			case Ipage:
				fprintf(stderr," Ipage ");
				break;
# endif
			}
		}


	/* If this is end-of-file, send a stream mark and say we're done */
	if (i == EOF && ! diagsw) {
		done = TRUE;
		return (StreamMark);
	} else
		return (i);
}

/*
 * Accept a byte from the communications process
 */

NOTHING PHibyte(byte, chan)
BYTE byte;
int chan;
{
	static BOOL errorMode = {FALSE};

	/* Check for bogus channel */
	if (chan == NullChan)
		if (Logfile) fprintf(logfd, "ibyte: byte seen on null channel\n");

	/* Handle any error conditions */
	if (chan == ErrChan) {
		if (errorMode) {
			if (byte == StreamMark)
				die("");
			else {
				if (Logfile) putc(byte, logfd);
				return;
			}
		}
		else switch (byte) {

		case HostDead:
			if (Logfile) fprintf(logfd, "Imagen-10 seems to be dead\n");
			break;

		case FatalError:
			if (Logfile) fprintf(logfd, "ibyte: fatal error from PC module: \n");
			break;

		case ErrReceived:
			if (Logfile) fprintf(logfd, "ibyte: error packet received from Imagen-10: \n");
			break;

		default:
			if (Logfile) 
				return (fprintf(logfd, "ibyte: invalid byte on error channel\n"));
			else return;
		}
		errorMode = TRUE;
		return;
	}

	/* Normal channel byte: log it */
	else putc(byte, logfd);
}




/*
 * Notify stderr that something is being sent
 */

NOTHING PHack()
{
	fprintf(stderr,".");   
}


/*
 * Write a string to the Imagen-10 communications line
 */

NOTHING PHout(sp, length)
char *sp;
int length;
{
	/* fprintf(stderr,"."); */
	for (; length-- > 0; ){
		putremote(*sp++);
	}
}

/*
 * Get some bytes from the Imagen-10 comm line, up to the EOP character
 */

int PHin(sp, maxlen)
char *sp;
int maxlen;
{
	char c;
	int l;

	for (l = 1; l <= maxlen; ++l) {
		c = getremote() & 0x7F;
		*sp++ = c;
		if (debug && c == '\\') {
			if (Logfile) fclose(logfd);
			die("Exit requested\n");
		}
		if (c == EOPchar)
			return(l);
	}
	return (maxlen);
}

/*
 * Return the number of bytes available on the I10 comm line

   ****** returns true if there is character *******



 */

int PHpeek()
{
	int av;

/*
	ioctl(fileno(I10fdi), FIONREAD, &av);
*/
	return (waitremote());
}

/*
 * Sleep for some number of MS (we can only approximate it in secs)
 */

NOTHING PHsleep(ms)
int ms;
{
	sleep (ms / 1000);
}


/*
 * Do debugging output (as in printf)
 */

NOTHING PHnote(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
{
	if (Logfile) fprintf(logfd, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/*
 * Die
 */

NOTHING die(a1, a2, a3, a4, a5, a6)
{
	fprintf(stderr, a1, a2, a3, a4, a5, a6);
	i10close();
	Report(pcount);
	exit(-1);
}


/*
	Send a message to user
*/

PHAttR(a1)
char *a1;
{
	char rply;

	rply = 0;
	PHnote(a1);
	fprintf (stderr, a1);
# ifdef Hold
	while (rply != '\n') {
		fprintf (stderr, "\n Please type <return> when ready ");
		rply = getc(stdin);
	}	
# endif
}

/*
	Reset the OTHER system by sending a stream of RESET characters
*/
# define	RESLG	254
# define RESCHAR	'@'

PHres()
{
	int length;

	PHnote(" Reset send\n");
	for (length=RESLG; length-- > 0; ) {
		putremote(RESCHAR);
		flurem();
	}
}
