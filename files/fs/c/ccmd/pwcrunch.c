/*
 * program to create a passwd.crunch file.  This is a passwd table which can
 * be read into memory very quickly.  Note this file is presorted by username.
 * it format is:
 * bytes 1-4: number passwd entries in the passwd file -- written as
 *		write(fd, &int, sizeof(int) -- that is, byteswapped on a vax.
 * n bytes:  passwd entries, with all string pointers as offsets into a single
 *		buffer.  This section will be of size:
 *			(n * sizeof struct passwd)
 * 		where n is the number of passwd entries read in above.
 * the rest of the file contains all of the strings from these passwd entries.
 * the address of the buffer this is read into should be added to all of 
 * the pointer fields in each passwd entry.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <stdio.h>
#include <pwd.h>

#define GROW 100
#define BUFGROW 1000

static struct passwd *ents;
static char *buf;
static int howmany = 0;
static int buflen = 0;
static int used = 0;
static max = 0;
char *malloc(), *realloc();

main(argc,argv) 
int argc;
char **argv;
{
    char buf[100];

    switch(argc) {
    case 1:
	crunchpw("/etc/passwd.crunch");
	break;
    case 2:
	crunchpw(argv[1]);
	break;
    default:
	fprintf(stderr,"usage: crunchpw [crunchfile]");
	break;
    }
}

crunchpw(tofile) {
    struct passwd *p;
    struct stat b1, b2;

    if (stat("/etc/passwd", &b1) != 0) {
	perror("/etc/passwd");
	exit(1);
    }
    if (stat(tofile, &b2) == 0 && b2.st_mtime > b1.st_mtime)
	exit(0);
    while(p = getpwent())		/* for all entries */
	crunchpwent(p);			/* compact it */
    write_crunchfile(tofile);		/* write out compacted version */
}

/*
 * compact a single passwd entry
 */
crunchpwent(p) 
struct passwd *p;
{
    struct passwd *p1;

    if (howmany == max)			/* make space if we need it */
	grow_entries();
    p1 = &ents[howmany];
    bcopy(p,p1,sizeof(struct passwd));	/* copy all the data */
    copystr(&p1->pw_name);		/* and then all of the strings */
    copystr(&p1->pw_passwd);		/* but fix the string ptrs as we go */
#ifdef SYSTEM_FIVE
    copystr(&p1->pw_age);
#endif
    copystr(&p1->pw_comment);
    copystr(&p1->pw_gecos);
    copystr(&p1->pw_dir);
    copystr(&p1->pw_shell);
    howmany++;
}

/*
 * make more space for entries.
 * we want a contiguous table.
 */
grow_entries() {
    max += GROW;
    if (ents == NULL) {
	ents = (struct passwd *) malloc((max)*sizeof(struct passwd));
    }
    else {
	ents = (struct passwd *) realloc(ents,(max)*sizeof(struct passwd));
    }
}

/*
 * copy the string into the buffer we are making, and set the string pointer
 * to be the index into that buffer.
 */
copystr(s)
char **s;
{
    if (used + strlen(*s) > buflen) {	/* expand buffer if necessary */
	buflen += BUFGROW;
	if (buf == NULL) buf = malloc(buflen);
	else buf = realloc(buf,buflen);
    }
    strcpy(&buf[used], *s);		/* copy the string */
    *s = (char *) used;			/* set ptr to be the index */
    used += strlen(&buf[used])+1;	/* count the space */
}

pwcmp(p1,p2)
register struct passwd *p1,*p2;
{
    register char *b1 = &buf[(int)p1->pw_name], *b2 = &buf[(int)p2->pw_name];
    return(strcmp(b1,b2));
}

/*
 * write out the compacted file
 */
write_crunchfile(tofile) {
    int fd;
    fd = open(tofile,O_WRONLY|O_CREAT,0444);
    if (fd == -1) {
	perror(tofile);
	exit(1);
    }
    err = xwrite(fd,&howmany,sizeof(howmany)); /* write how many entries */
    qsort(ents, howmany, sizeof(struct passwd), pwcmp);
    xwrite(fd,ents,sizeof(struct passwd)*howmany); /* write the entries */
    xwrite(fd,buf,used);		/* write the strings */
    close(fd);
}

xwrite(fd,buf,len)
int fd,len;
char *buf;
{
    int sofar = 0,x;

    while(sofar < len) {
	x = write(fd, buf+sofar, len - sofar);
	if (x == -1) {
	    perror("write");
	    return(-1);
	}
	sofar += x;
    }
    return(len);
}

