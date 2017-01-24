/*
 * webster - look words up in the dictionary
 *
 * This program connects to the Webster server running on SRI-NIC to get
 * definitions of words.  Words may be given on the command line, or, if
 * no arguments are given, the program runs interactively.
 *
 * In either mode, a word may include the wildcard characters '%' and '*'.
 * The '%' character matches exactly one character, while the '*' matches
 * zero or more characters.  If wildcards are used, the program will
 * return either "No match" or a list of matching words.
 *
 * In interactive mode only, Tenex-style command completion may also be
 * used.  Typing a '?' following part of a word will cause the program
 * to print all words which begin with the partial word, or the program
 * will beep if nothing matches.  Typing an ESCape character causes the
 * program to attempt to complete the word.  If the word can be completed,
 * the new word is printed; otherwise, the program beeps.  Wildcards
 * may be used to specify the partial words.
 *
 * This program runs under 4.2BSD UNIX.
 *
 * David A. Curry, davy@purdue-ecn
 * Purdue Engineering Computer Network
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sgtty.h>
#include <netdb.h>
#include <ctype.h>
#include <stdio.h>

#define WEBSTERHOST	"sri-nic.arpa"	/* host where the server lives	*/
#define WEBSTERPORT	103		/* port where the server lives	*/
#define BACKSPACE	010		/* backspace character       ^H */
#define WORDERASE	027		/* word erase character      ^W */
#define LINEERASE	030		/* line kill character       ^X */
#define LINERTYPE	022		/* line reprint character    ^R */
#define COMPLETE	033		/* word completion character ^[ */
#define ENDINGS		'?'		/* print matches character      */
#define EOFCH		0200		/* EOF record from the server   */

struct sgttyb sgttyb;			/* tty modes when interactive	*/
struct sgttyb rsgttyb;			/* original tty modes		*/

FILE *WebsterSock;			/* for reading from the server	*/
int interactive = 0;			/* 1 when running interactive	*/

main(argc, argv)
int argc;
char **argv;
{
	/*
	 * Connect to the server.
	 */
	connectup();

	/*
	 * If we were given command line arguments, just
	 * try to define each word.
	 */
	if (argc > 1) {
		while (--argc) {
			/*
			 * Define the word.
			 */
			define(*++argv);
		}

		exit(0);
	}

	/*
	 * If no arguments were given, set up the
	 * terminal modes and run interactively.
	 */
	setup();
	interact();
}

/*
 * connectup - connects to the Webster server.
 */
connectup()
{
	register int s;
	struct sockaddr_in sin;
	register struct hostent *hp;
	struct hostent *gethostbyname();

	/*
	 * Look up the host in the host file.
	 */
	if ((hp = gethostbyname(WEBSTERHOST)) == NULL) {
		fprintf(stderr, "webster: %s: unknown host.\n", WEBSTERHOST);
		exit(1);
	}

	bzero(&sin, sizeof(struct sockaddr_in));

	/*
	 * Build the server's address.
	 */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(WEBSTERPORT);
	bcopy(hp->h_addr, &sin.sin_addr, hp->h_length);

	/*
	 * Get a TCP socket.
	 */
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("webster: socket");
		exit(1);
	}

	/*
	 * Try to connect.
	 */
	if (connect(s, &sin, sizeof(struct sockaddr_in)) < 0) {
		perror("webster: connect");
		exit(1);
	}

	/*
	 * Open the socket for stdio.
	 */
	WebsterSock = fdopen(s, "r");
}

/*
 * setup - turns on CBREAK, turns off ECHO.  Also trap signals.
 */
setup()
{
	extern int byebye();
	extern int suspend();

	interactive = 1;
	ioctl(0, TIOCGETP, &sgttyb);

	rsgttyb = sgttyb;

	signal(SIGINT, byebye);
	signal(SIGQUIT, byebye);
	signal(SIGTSTP, suspend);

	sgttyb.sg_flags |= CBREAK;
	sgttyb.sg_flags &= ~ECHO;
	ioctl(0, TIOCSETP, &sgttyb);
}

/*
 * interact - interact with the user.
 */
interact()
{
	int c;
	char buf[1024];
	register char *s, *t;

	/*
	 * Forever...
	 */
	for (;;) {
		/*
		 * Prompt for a word.
		 */
		s = buf;
		write(1, "Word: ", 6);

		/*
		 * Forever... read characters.  We
		 * break out of this from inside.
		 */
		for (;;) {
			if (read(0, &c, 1) <= 0)
				byebye();

			switch (c) {
			case BACKSPACE:
				/*
				 * If not at the beginning of a line,
				 * back up one character.
				 */
				if (s > buf) {
					write(1, "\b \b", 3);
					s--;
				}
				continue;
			case WORDERASE:
				/*
				 * Until we hit beginning of line
				 * or beginning of word, back up.
				 */
				while ((s > buf) && (*s != ' ')) {
					write(1, "\b \b", 3);
					s--;
				}
				continue;
			case LINEERASE:
				/*
				 * Until we hit beginning of line,
				 * back up.
				 */
				while (s > buf) {
					write(1, "\b \b", 3);
					s--;
				}
				continue;
			case LINERTYPE:
				/*
				 * Retype the line.
				 */
				write(1, "\r\nWord: ", 8);

				for (t=buf; t < s; t++)
					write(1, t, 1);
				continue;
			case COMPLETE:
				/*
				 * Try to complete what they typed
				 * so far.  Put the pointer at the
				 * end of the new word.
				 */
				*s = NULL;
				complete(buf);
				for (s=buf; *s; s++)
					;
				continue;
			case ENDINGS:
				/*
				 * If it's the first character,
				 * then print some help.  Otherwise,
				 * try to find endings for the word.
				 * endings() returns 1 if no endings
				 * were found, 0 if some were found.
				 * This tells us whether to reprint
				 * the current word or not.
				 */
				if (s == buf) {
					help();
				}
				else {
					*s = NULL;
					if (endings(buf) == 0) {
						write(1, "Word: ", 6);

						for (s=buf; *s; s++)
							write(1, s, 1);
					}

					continue;
				}
				break;
			case '\n':
				/*
				 * If at the start of a word,
				 * newline is exit.
				 */
				if (s == buf)
					byebye();

				/*
				 * Otherwise, try to define
				 * the word.
				 */
				*s = NULL;
				write(1, "\n", 1);
				define(buf);
				break;
			default:
				/*
				 * Echo the character and copy it.
				 */
				write(1, &c, 1);
				*s++ = c;
				continue;
			}

			break;
		}
	}
}

/*
 * define - try to define a word and print its definition.
 */
define(word)
char *word;
{
	int c, refs;
	char buf[1024];
	register char *s;

	/*
	 * Command is "DEFINE<space>word<nl>".
	 */
	sprintf(buf, "DEFINE %s\r\n", word);

	/*
	 * Send the command.
	 */
	if (send(fileno(WebsterSock), buf, strlen(buf), 0) < 0) {
		perror("webster: send");
		byebye();
	}

	/*
	 * Read the first line back from the server.  This
	 * line tells us what the result of our DEFINE
	 * request was.
	 */
	getline(buf);

	/*
	 * "WILD<space>0<nl>" means they used wild cards and no
	 * matches were found.
	 */
	if (!strncmp(buf, "WILD 0", 6)) {
		printf("No match.\n");
		return;
	}

	/*
	 * "WORD<nl>" means that the wildcard matched, so we
	 * print a list of possible matches.
	 */
	if (!strncmp(buf, "WILD", 4)) {
		printf("Possible matches are:\n");

		/*
		 * List lines.
		 */
		listlines(0, 1);
		putchar('\n');
		return;
	}

	/*
	 * "SPELLING<space>0<nl>" means the word is not defined,
	 * and there are no alternate spellings.
	 */
	if (!strncmp(buf, "SPELLING 0", 10)) {
		printf("No definition for '%s'.\n", word);
		return;
	}

	/*
	 * "SPELLING<nl>" means the word is not defined, but
	 * some alternate spellings were found.  Print
	 * them out.
	 */
	if (!strncmp(buf, "SPELLING", 8)) {
		printf("No definition for '%s'.  Maybe you mean:\n", word);

		/*
		 * List lines.
		 */
		listlines(0, 1);
		putchar('\n');
		return;
	}

	/*
	 * "DEFINITION<space>n<nl>" means the word is defined,
	 * and there are n cross-references.
	 */
	if (!strncmp(buf, "DEFINITION", 10)) {
		sscanf(buf+11, "%d", &refs);

		/*
		 * Print any cross references.
		 */
		if (refs > 0) {
			printf("Cross references:\n");

			/*
			 * List lines.
			 */
			listlines(refs, 1);
			putchar('\n');
		}

		/*
		 * Print the definition.
		 */
		while ((c = getc(WebsterSock)) != EOF) {
			if (c == EOFCH)
				break;

			c &= 0177;
			putchar(c);
		}

		putchar('\n');
		return;
	}

	/*
	 * Should never get here.
	 */
	while (((c = getc(WebsterSock)) != EOF) && (c != EOFCH))
		;
}

/*
 * complete - try to complete the word.
 */
complete(word)
char *word;
{
	int c;
	char buf[1024];
	register char *s;

	/*
	 * Command is "COMPLETE<space>word<nl>".
	 */
	sprintf(buf, "COMPLETE %s\r\n", word);

	/*
	 * Send the command.
	 */
	if (send(fileno(WebsterSock), buf, strlen(buf), 0) < 0) {
		perror("webster: send");
		byebye();
	}

	/*
	 * Get the first line from the server, which tells
	 * us the reult of our request.
	 */
	getline(buf);

	/*
	 * "AMBIGUOUS<space>n<nl>" means the word is ambiguous,
	 * with n possible matches.  We ignore the n, and just
	 * beep.
	 */
	if (!strncmp(buf, "AMBIGUOUS", 9)) {
		write(1, "\007", 1);
		return;
	}

	/*
	 * "COMPLETION<space>full-word<nl>" means the
	 * word was completed.  Erase what they typed
	 * and print the new word over it.  This takes
	 * care of things if they used wildcards.
	 */
	if (!strncmp(buf, "COMPLETION", 10)) {
		for (s=word; *s; s++)
			write(1, "\b", 1);

		s = buf+11;
		while (((*s & 0177) != '\r') && ((*s & 0177) != '\n') &&
		       ((*s & 0177) != NULL)) {
			write(1, s, 1);
			s++;
		}

		/*
		 * Put the new word back into word.  This
		 * gets rid of the wildcards here.
		 */
		*s = NULL;
		strcpy(word, buf+11);

		return;
	}

	/*
	 * Should never get here.
	 */
	while (((c = getc(WebsterSock)) != EOF) && (c != EOFCH))
		;
}

/*
 * endings - find possible endings for a word.
 */
endings(word)
char *word;
{
	int c;
	char buf[1024];
	register char *s;

	/*
	 * Command is "ENDINGS<space>word<nl>".
	 */
	sprintf(buf, "ENDINGS %s\r\n", word);

	/*
	 * Send the command.
	 */
	if (send(fileno(WebsterSock), buf, strlen(buf), 0) < 0) {
		perror("webster: send");
		byebye();
	}

	/*
	 * Get the first line from the server, which tells
	 * us the result of the search.
	 */
	getline(buf);

	/*
	 * "MATCHS<space>0<nl>" means nothing matched,
	 * so we beep at them.
	 */
	if (!strncmp(buf, "MATCHS 0", 8)) {
		write(1, "\007", 1);
		return(1);
	}

	/*
	 * "MATCHS<nl>" means there were matches, so
	 * print them out.
	 */
	if (!strncmp(buf, "MATCHS", 6)) {
		printf("\nMaybe you mean:\n");

		/*
		 * List lines.
		 */
		listlines(0, 0);
		putchar('\n');
		return(0);
	}

	/*
	 * Should never get here.
	 */
	while (((c = getc(WebsterSock)) != EOF) && (c != EOFCH))
		;

	return(0);
}

/*
 * getline - read one line from the server and put it in s.
 */
getline(s)
register char *s;
{
	register int c;

	/*
	 * Read in chars.  If we hit EOFCH, return
	 * 0.
	 */
	while ((c = getc(WebsterSock)) != EOF) {
		if (c == EOFCH)
			return(0);

		c &= 0177;

		if (c == '\r')
			continue;

		if (c == '\n')
			break;

		*s++ = c;
	}

	*s = NULL;
	return(1);
}

/*
 * listlines - list WILD-style lines on the screen.
 */
listlines(n, num)
register int n;
int num;
{
	char buf[1024];
	register int col;

	printf(" ");

	/*
	 * If n is non-zero, we only want to list n lines.
	 * Otherwise, we go till we hit EOFCH.  Lines are
	 * printed in four columns.
	 */
	if (n) {
		col = 0;
		while (n-- > 0) {
			getline(buf);
			putline(buf, num);

			if (++col == 3) {
				printf("\n ");
				col = 0;
			}
		}
	}
	else {
		col = 0;
		while (getline(buf) > 0) {
			putline(buf, num);

			if (++col == 3) {
				printf("\n ");
				col = 0;
			}
		}
	}

	if (col)
		putchar('\n');
}

/*
 * putline - put out a line, if num is 0, skip the line number.
 */
putline(buf, num)
char *buf;
int num;
{
	int lnum;
	char line[1024];

	sscanf(buf, "%d %[^\n]", &lnum, line);

	if (num)
		printf("%2d. %-22s", lnum, line);
	else
		printf("%-26s", line);
}

/*
 * help - print a help message.
 */
help()
{
	printf("\n   Type in the word you want defined, or a blank line to exit. Additionally,\n");
	printf("Webster can match words using wildcards.  The character '%%' in a word means\n");
	printf("match exactly one character; while the character '*' means match zero or more\n");
	printf("characters.\n");
	printf("   Typing a partial word followed by '?' will print all the words in the\n");
	printf("dictionary which match your partial word. Typing a partial word followed by an\n");
	printf("ESCape character will try to complete the word for you.  If the partial word\n");
	printf("is ambiguous, Webster will beep at you.  Note that you can use the wildcards\n");
	printf("along with ESC and ?.  For example (the underlined parts are typed by the\n");
	printf("user, the rest by Webster),\n");
	printf("\n");
	printf("Word: balla?   Maybe you mean:\n");
	printf("      ------\n");
	printf("  1. ballad           2. ballade          3. baladry         4. ballast\n");
	printf("Word: pluria<ESC>xial\n");
	printf("      --------------\n");
	printf("Word: plu*x<ESC>\n");
	printf("      -------------\n");
	printf("Word: pluriaxial\n");
	printf("\n---- End of Help File ----\n\n");
}

/*
 * byebye - called on exit.
 */
byebye()
{
	/*
	 * If interactive, reset the tty modes.
	 */
	if (interactive)
		ioctl(0, TIOCSETP, &rsgttyb);

	/*
	 * Close the socket and exit.
	 */
	fclose(WebsterSock);
	write(1, "\n", 1);
	exit(0);
}

/*
 * suspend - reset tty modes and supend ourselves.
 */
suspend()
{
	long blocked;
	extern int suspend();

	/*
	 * Reset tty modes and suspend.
	 */
	ioctl(0, TIOCSETP, &rsgttyb);
	signal(SIGTSTP, SIG_DFL);
	blocked = sigsetmask(0);
	kill(0, SIGTSTP);

	/*
	 * We come here on SIGCONT.  Reset
	 * the signal mask and tty modes.
	 */
	sigsetmask(blocked);
	signal(SIGTSTP, suspend);
	ioctl(0, TIOCSETP, &sgttyb);	
}
