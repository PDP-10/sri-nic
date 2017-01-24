/* <NETDB.H> - definitions for BSD network library routines
**
**	(c) Copyright Ken Harrenstien 1989
**
**	The BSD network routines are only partially supported.
*/

#ifndef _NETDB_INCLUDED
#define _NETDB_INCLUDED

#define gethostent	gthent	/* linker disambiguation */
#define gethostbyname	gthnam
#define gethostbyaddr	gthadr

#define gethostid	gtlsid
#define gethostname	gtlsnm

struct hostent {
    char *h_name;		/* official name of host */
    char **h_aliases;		/* alias list */
    int h_addrtype;		/* address type */
    int h_length;		/* length of address */
    char *h_addr;		/* address */
};

#define AF_INET		2	/* internet address type */
#define INET_ADDR_SIZE	4	/* size (in chars) of one */

struct hostent *gethostent(), *gethostbyname(), *gethostbyaddr();
void sethostent(), endhostent();

int gethostid(), gethostname();

#endif /* ifndef _NETDB_INCLUDED */
