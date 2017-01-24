/*
 *	GETHOSTENT.H
 */

#define gethostent	gthent	/* linker disambiguation */
#define gethostbyname	gthnam
#define gethostbyaddr	gthadr

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
