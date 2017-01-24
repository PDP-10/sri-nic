/*
 *	GETHOSTENT - gethostent, gethostbyaddr, gethostbyname,
 *		     sethostent, endhostent
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include <c-env.h>
#include "stdio.h"
#include "netdb.h"
#include "jsys.h"

#define MAX_HOSTNAME	80		/* good enough for a hostname? */

char gh_hostname[MAX_HOSTNAME];
char gh_address[INET_ADDR_SIZE];

struct hostent gh_hentry = {
    gh_hostname,			/* h_name = official hostname */
    NULL,				/* h_aliases */
    AF_INET,				/* h_addrtype = only type defined */
    INET_ADDR_SIZE,			/* h_length = length of address */
    gh_address				/* h_address = ptr to address */
};

struct hostent *
gethostbyname(name)		/* Actually _gthnam() */
char *name;
{
#if SYS_T20+SYS_10X
    int ablock[5];
    unsigned host;
    char *cp;

    ablock[1] = _GTHSN;
    ablock[2] = (int) (name - 1);
    if (!jsys(GTHST, ablock)) return NULL;
    cp = gh_address;				/* where to store the addr */
    host = ablock[3];				/* right-justified host# */
    *cp = (host >> 3*8) & 0377;
    *++cp = (host >> 2*8) & 0377;		/* store the four octets */
    *++cp = (host >> 8) & 0377;			/* as sequential chars */
    *++cp = host & 0377;
    ablock[1] = _GTHNS;				/* want official name now */
    ablock[2] = (int) (gh_hostname - 1);	/* where to put it */
    ablock[3] = host;				/* host# */
    if (!jsys(GTHST, ablock))			/* if can't get the official */
	strcpy(gh_hostname, name);		/* one then use theirs... */
    return &gh_hentry;				/* what they gave us */
#else
    return NULL;
#endif
}

struct hostent *
gethostbyaddr(addr, len, type)		/* actually _gthadr() */
char *addr;
int len, type;
{
#if SYS_T20+SYS_10X
    int ablock[5];
    unsigned host;
    char *cp;

    if (type != AF_INET || len != INET_ADDR_SIZE) return NULL;
    cp = addr;
    host = (*cp << 3*8) | (*++cp << 2*8) | (*++cp << 8) | *++cp;
    ablock[1] = _GTHNS;
    ablock[2] = (int) (gh_hostname - 1);
    ablock[3] = host;
    if (!jsys(GTHST, ablock)) return NULL;
    bcopy(addr, gh_address, len);		/* copy the address */
    return &gh_hentry;
#else
    return NULL;
#endif
}

struct hostent *
gethostent()			/* actually _gthent() */
{
    return NULL;
}

void
sethostent(stayopen)
int stayopen;
{
}

void
endhostent()
{
}
