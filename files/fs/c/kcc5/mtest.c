/*
 * Interactive Mail Access Protocol 2 (IMAP2) test program
 *
 * Mark Crispin, SUMEX Computer Project, 8 July 1988
 *
 * Copyright (c) 1988 Stanford University
 *
 */

#include <stdio.h>
#include <ctype.h>
#include "imap2.h"
#include "smtp.h"
#include "misc.h"
#include "tcpsio.h"


    char selected[MAXMESSAGES];	/* T if message selected */
    char *curhst = NIL;		/* currently connected host */
    char *curusr = NIL;		/* current login user */
    char personalname[256];	/* user's personal name */


/* Main program - initialization */

main ()
{
  IMAPSTREAM *stream;
  char tmp[256];
  int debug;
				/* user wants protocol telemetry? */
  printf ("Debug protocol (y/n)?");
  gets (tmp);
  ucase (tmp);
  if (tmp[0] == 'Y') debug = T;
  printf ("Personal name: ");	/* get user's name */
  gets (personalname);
  printf ("Mailbox: ");		/* get mailbox to open */
  gets (tmp);
  				/* open mailbox, no recycle stream */
  stream = map_open (tmp,NIL,debug);
  if (stream) mm (stream,debug);/* run user interface if opened */
}


/* MM command loop
 * Accepts: IMAP2 stream
 */

mm (stream,debug)
    IMAPSTREAM *stream;
    int debug;
{
  char cmd[256];
  char *arg;
  int i;
  int last;
  last = 0;
  mm_status (stream);		/* first report message status */
  while (T) {
    printf ("MTest>");		/* prompt user */
 				/* get command */
    if (!gets (cmd)) cmd[0] = 'Q';
    if (islower (cmd[0])) cmd[0] = toupper (cmd[0]);
    switch (cmd[0]) {
      case 'C':			/* Check command */
	map_checkmailbox (stream);
	mm_status (stream);
	break;
      case 'D':			/* Delete command */
	arg = (char *) strchr (cmd,' ');
	if (arg) {
	  ++arg;
	  last = atoi (arg);
	}
	else {
	  if (last == 0 ) {
	    printf ("?Missing message number\n");
	    break;
	  }
	  arg = cmd;
	  sprintf (arg,"%d",last);
	}
	if (last > 0 && last <= stream->nmsgs)
	  map_setflag (stream,arg,"\\DELETED");
	else printf ("?Bad message number\n");
	break;
      case 'E':			/* Expunge command */
	map_expungemailbox (stream);
	last = 0;
	break;
      case 'H':			/* Headers command */
	arg = (char *) strchr (cmd,' ');
	if (arg) {
	  ++arg;
	  if (!(last = atoi (arg))) {
	    memset (selected,NIL,stream->nmsgs);
	    map_search (stream,arg);
	    for (i = 0; i < stream->nmsgs; ++i)
	      if (selected[i]) mm_header (stream,i+1);
	    break;
	  }
	}
	else if (last == 0) {
	  printf ("?Missing message number\n");
	  break;
	}
	if (last > 0 && last <= stream->nmsgs) mm_header (stream,last);
	else printf ("?Bad message number\n");
	break;
      case 'N':			/* New mailbox command */
	arg = (char *) strchr (cmd,' ');
	if (!arg) {
	  printf ("?Missing mailbox\n");
	  break;
	}
	++arg;
				/* get the new mailbox */
	stream = map_open (arg,stream,debug);
	if (!stream) return;	/* lost */
	last = 0;
	mm_status (stream);
	break;
      case 'Q':			/* Quit command */
	map_close (stream);
	return;
      case 'S':			/* Send command */
	smtptest (debug);
	break;
      case 'T':			/* Type command */
	arg = (char *) strchr (cmd,' ');
	if (arg) {
	  ++arg;
	  last = atoi (arg);
	}
	else if (last == 0 ) {
	  printf ("?Missing message number\n");
	  break;
	}
	if (last > 0 && last <= stream->nmsgs)
	  printf ("%s\n",map_fetchmessage (stream,last));
	else printf ("?Bad message number\n");
	break;
      case 'U':			/* Undelete command */
	arg = (char *) strchr (cmd,' ');
	if (arg) {
	  ++arg;
	  last = atoi (arg);
	}
	else {
	  if (last == 0 ) {
	    printf ("?Missing message number\n");
	    break;
	  }
	  arg = cmd;
	  sprintf (arg,"%d",last);
	}
	if (last > 0 && last <= stream->nmsgs)
	  map_clearflag (stream,arg,"\\DELETED");
	else printf ("?Bad message number\n");
	break;
      case 'X':			/* Xit command */
	map_expungemailbox (stream);
	map_close (stream);
	return;
      case '+':
	map_debug (stream); debug = T;
	break;
      case '-':
	map_nodebug (stream); debug = NIL;
	break;
      case '?':			/* ? command */
	printf ("%s %s\n%s\n",
		"Check, Delete, Expunge, Headers, New Mailbox,",
		"Quit, Send, Type, Undelete, Xit",
		" or <RETURN> for next message");
	break;
      case '\0':		/* null command (type next message) */
	if (last > 0 && last < stream->nmsgs)
	  printf ("%s\n",map_fetchmessage (stream,++last));
	else printf ("%%No next message\n");
	break;
      default:			/* bogus command */
	printf ("?Unrecognized command: %s\n",cmd);
	break;
    }
  }
}


/* MM status report
 * Accepts: IMAP2 stream
 */

mm_status (stream)
    IMAPSTREAM *stream;
{
  int i;
  printf ("Mailbox: {%s}%s, %d messages, %d recent\n",
	  tcp_host (stream->tcpstream),stream->mailbox,
	  stream->nmsgs,stream->recent);
  if (stream->userFlags[0]) {
    printf ("Keywords:");
    for (i = 0; i <= NUSERFLAGS; ++i)
      if (stream->userFlags[i]) printf (" %s",stream->userFlags[i]);
    printf ("\n");
  }
}


/* MM display header
 * Accepts: IMAP2 stream
 *	    message number
 */

mm_header (stream,msgno)
    IMAPSTREAM *stream;
    int msgno;
{
  int i;
  char tmp[256];
  char tmpx[20];
  map_fetchenvelope (stream,msgno);
  sprintf (tmp,"%4d) ",stream->messagearray[msgno-1]->messageNumber);
  i = stream->messagearray[msgno-1]->systemFlags;
  if (i&fRECENT) {
    if (i&fSEEN) tmp[6] = 'R'; else tmp[6] = 'N';
    tmp[7] = ' ';
  }
  else {
    tmp[6] = ' ';
    if (i&fSEEN) tmp[7] = ' '; else tmp[7] = 'U';
  }
  if (i&fFLAGGED) tmp[8] = 'F'; else tmp[8] = ' ';
  if (i&fANSWERED) tmp[9] = 'A'; else tmp[9] = ' ';
  if (i&fDELETED) tmp[10] = 'D'; else tmp[10] = ' ';
  strncpy (tmp+11,stream->messagearray[msgno-1]->internalDate,6);
  tmp[17] = ' ';
  tmp[18] = '\0';
  strcat (tmp,map_fetchfromstring (stream,msgno,20));
  strcat (tmp," ");
  if (i = stream->messagearray[msgno-1]->userFlags) {
    strcat (tmp,"{");
    while (i) {
      strcat (tmp,stream->userFlags[find_rightmost_bit (&i)]);
      if (i) strcat (tmp," ");
    }
    strcat (tmp,"} ");
  }
  strcat (tmp,map_fetchsubjectstring (stream,msgno,35));
  sprintf (tmpx," (%d chars)",stream->messagearray[msgno-1]->rfc822_size);
  strcat (tmp,tmpx);
  printf ("%s\n",tmp);
}

mm_searched (stream,number)
    IMAPSTREAM *stream;
    int number;
{
  selected[number-1] = T;
}

mm_exists (stream,number)
    IMAPSTREAM *stream;
    int number;
{
}

mm_expunged (stream,number)
    IMAPSTREAM *stream;
    int number;
{
}

usrlog (string)
    char *string;
{
  printf ("%s\n",string);
}

dbglog (string)
    char *string;
{
  fprintf (stderr,"%s\n",string);
}


/* Get user name and password for this host
 * Accepts: host name
 *	    where to return user name
 *	    where to return password
 */

getpassword (host,username,password)
    char *host;
    char *username;
    char *password;
{
  if (curhst) free (curhst);
  curhst = malloc (1+strlen (host));
  strcpy (curhst,host);
  printf ("{%s} username: ",host);
  gets (username);
  if (curusr) free (curusr);
  curusr = malloc (1+sizeof (username));
  strcpy (curusr,username);
  printf ("password: ");
  gets (password);
}

/* test SMTP */

    char *hostlist[] = {
	"SUMEX-AIM.Stanford.EDU",
	"ARDVAX.Stanford.EDU",
	"KSL.Stanford.EDU",
	NIL
    };
	

smtptest (debug)
    int debug;
{
  SMTPSTREAM *stream;
  char date[256];
  char subject[256];
  char line[256];
  char text[8196];
  MESSAGE *msg = (MESSAGE *) malloc (sizeof (MESSAGE));
  MTPADR *from = (MTPADR *) malloc (sizeof (MTPADR));
  from->personalName = personalname;
  from->routeList = NIL;
  from->mailBox = curusr;
  from->host = curhst;
  from->error = NIL;
  from->next = NIL;
  msg->return_path = from;
  msg->from = from;
  msg->sender = NIL;
  msg->reply_to = NIL;
  msg->to = NIL;
  while (!msg->to) {
    printf ("To: ");
    if (!gets (line)) break;
    msg->to = mtp_parse_address (line,curhst);
  }
  printf ("cc: ");
  msg->cc = mtp_parse_address (gets (line),curhst);
  msg->bcc = NIL;
  msg->in_reply_to = NIL;
  msg->message_id = NIL;
  printf ("Subject: ");
  gets (subject);
  msg->subject = subject;
  printf (" Msg (end with a line with only a '.'):\n");
  text[0] = '\0';
  while (gets (line)) {
    if (line[0] == '.') {
      if (line[1] == '\0') break;
      else strcat (text,".");
    }
    strcat (text,line);
    strcat (text,"\015\012");
  }
  clearerr (stdin);
  printf ("Sending...\n");
  msg->text = text;
  rfc822_date (date);
  msg->date = date;
  stream = mtp_open (hostlist,NIL,debug);
  if (stream) {
    if (mtp_mail (stream,"MAIL",msg)) printf ("[Ok]\n");
    else printf ("[Failed]\n");
    mtp_close (stream);
  }
  else printf ("[Can't open connection to any server]\n");
  if (msg->to) mtp_free_address (msg->to);
  if (msg->cc) mtp_free_address (msg->cc);
  free ((char *) from);
  free ((char *) msg);
}
