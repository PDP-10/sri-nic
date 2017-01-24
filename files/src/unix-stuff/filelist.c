/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/*
 * Given a vector of directories, build a table of all files in those
 * directories
 * If the path passed here is the same as the previous one (help, and the 
 * completion, uses previous listing for speed.
 *
 * All buffers are either malloc'ed or realloc'ed when needed.  If 
 * these fail, bad things can happen -- i'm not yet sure what to do in this 
 * case.
 *
 */

#if (BSD|SYSV) 
#define UNIX
#endif

#include <sys/types.h>			/* get data types used in dir.h */
#include <sys/dir.h>			/* directory structure */
#include <sys/stat.h>			/* for read/write date */
#include <ctype.h>
#include <errno.h>
#ifdef MSDOS
#include <dos.h>
#include <direct.h>
#endif
#include "filelist.h"			/* local symbols */
#include "ccmd.h"
#ifdef UNIX
#ifndef _CMUSR
#include <pwd.h>			/* for "~" completion */
#endif /*  _CMUSR */
#endif /*  UNIX */

#if SYSV

#include <sys/file.h>
#include <fcntl.h>
typedef struct {
    int size;
    struct direct *files;
    struct direct *current;
} DIR;

#endif /* SYSV */

/* make sure toupper does what we want */
#ifdef toupper
#undef toupper
#endif
#define toupper(c) (islower(c) ? (c)-'a'+'A' : (c))
#define BUFLEN 120
char *tilde_expand();

char *namebuf;				/* hold filenames here */
char *prevnamebuf;			/* previous name buffer */

dirfile *files;				/* list of files */
dirfile *prevfiles;			/* list from previous pass */

dirfile **fileindex;			/* vector of files for sorting */
dirfile **previndex;			/* index from previous pass */

int filecount;				/* number of files entries used */
int prevfilecount;			/* from previous pass */
int maxfiles;				/* number of file entries malloc'ed */

char *curfile;				/* current file in namelist */
int namelen;				/* used length of namebuf */
int maxnamelen;				/* malloc'ed length of namebuf */
char **prevpath=NULL;			/* previous search path  */
char **usepath=NULL;			/* expanded path to use */
int currentpath= 0;			/* current spot in usepath */
int usepathlen=0;
int redofiles = TRUE;
int *modtimes = NULL;

char *malloc(), *realloc(), *index();
void qsort();				/* quick sort routine */

int dirfilecmp();			/* predicate for qsort */


/*
 * cmfileini:
 * (re)initialize file vars.  Useful in making new file appear for
 * file parse.
 */
cmfileini()
{
    redofiles = TRUE;
}

/*
 * search_path:
 * takes a NULL terminated vector of directories.
 * returns a pointer to a listing of the files.
 * this listing is guaranteed to stay intact until the function is
 * called again.
 * fcount is set to the number of files returned
 */

dirfile **
search_path(path,fcount) char **path; int *fcount; {
  int pathlen;				/* length of the path */
  char **p;
  int i;
  register char **p1, **p2;		/* as last time */
  struct stat sbuf;
  int *m;
  p1 = path;
  p2 = prevpath;
  m = modtimes;

  for (;!redofiles;) {				/* check if it is same path */
    if (p1 == NULL || p2 == NULL) break; /* NULL list? */
    if ((*p1 != NULL) && (*p2 != NULL) && 
	(strcmp(*p1,*p2)))  break;	/* or different entry...rebuild list */
    stat(*p1,&sbuf);
    if ((*p1 != NULL) && (*p2 != NULL) && /* if a directory changed, rebuild */
	sbuf.st_mtime != *m) break;
    if (*p1 == NULL) {			/* end of list?  they are the same */
					/* same list as last time...so */
      namebuf = prevnamebuf;		/* make everything point to last set*/
      *fcount = filecount = prevfilecount;
      files = prevfiles;
      fileindex = previndex;
      return(previndex);		/* and return prev index */
    }
    p1++,p2++;				/* keep walking the list */
  }
  redofiles = FALSE;
					/* different path...free up old */
					/* buffers */

  if (prevnamebuf != NULL)		/* free old name buffer */
    free(prevnamebuf);
  if (prevfiles != NULL)		/* and old file list */
    free(prevfiles);
  if (previndex != NULL)		/* old index */
    free(previndex);
  if (prevpath != NULL) {		/* and old path */
    for (p = prevpath; *p != NULL; p++)	/* have to free each name in path */
      free(*p);
    free(prevpath);
  }
  if (modtimes != NULL)
      free(modtimes);
  filecount = prevfilecount = 0;	/* no previous files */
  fileindex = NULL;			/* or current files */
  files = NULL;
  namebuf = NULL;
  curfile = NULL;
  namelen = 0;
  maxnamelen = 0;
  maxfiles = 0;
					/* now copy the current path */
  pathlen = 0;
  for (p = path; *p != NULL; p++)	/* first find it's length */
    pathlen++;
  prevpath = (char **) malloc((pathlen+1)*sizeof(char *)); /* build vector */
  modtimes = (int *) malloc((pathlen)*sizeof(int));
  for (i = 0,p = path; *p != NULL; i++,p++) { /* then copy the names */
    prevpath[i] = malloc((1+strlen(*p)) * sizeof(char));
    strcpy(prevpath[i],*p);
    stat(*p,&sbuf);
    modtimes[i] = sbuf.st_mtime;
  }
  prevpath[pathlen] = NULL;		/* NULL termination */

  if (usepath != NULL) {		/* and expanded path */
    for (i = 0,p = usepath; i < usepathlen;p++,i++) {
      if (*p != NULL) free(*p);
    }
    free(usepath);
  }
  usepath = (char **) malloc((pathlen+1)*sizeof(char *)); /* build vector */
  usepathlen=pathlen;			/* and remember length */

  for (i = 0; i < usepathlen; i++) {
    usepath[i] = malloc((1+strlen(prevpath[i])) * sizeof(char));
    strcpy(usepath[i],prevpath[i]);
  }
  usepath[usepathlen] = NULL;
  for (i =0; i < usepathlen; i++) {	/* now build up list */
    currentpath = i;
    if (usepath[i] != NULL) search_dir(usepath[i]);
  }
					/* build up index into list */
  fileindex = (dirfile **) malloc(sizeof(dirfile *) * filecount);
  for (i = 0; i < filecount; i++) fileindex[i] = &files[i];
					/* and sort it */
  qsort(fileindex,filecount,sizeof(dirfile *), dirfilecmp);
 
  previndex = fileindex;			/* save these for next time */
  prevnamebuf = namebuf;
  prevfiles = files;
  *fcount = prevfilecount = filecount;
  return(previndex);
}

/*
 * search_dir:
 * builds up a file list for a single directory
 * if a directory is wild, expands it.
 * if directory is CWD, then includes ".", and ".."
 */

DIR *opendir();

search_dir(dirname) char *dirname; {
  struct stat sbuf;
  DIR *dirp, *dotdirp;			/* directory pointers */
  register struct direct *d;		/* file entries in a dir */
  struct direct *dotd, *readdir();
  int i;
  char *dirr=dirname;			/* actual directory name to use */
  struct passwd *user;			/* passwd structure for "~" files */
  int ispwd;				/* flag if looking at connect dir */
  register dirfile *f;
  int dirstrlen;
#ifdef MSDOS
  static char pwdbuf[51];
  char *getcwd();
  int ustrcmp();
#endif
  if (iswild(dirname)) {
    expandwilddir(dirname);    
    return;
  }
#if (SYSV|BSD)
  if (dirname[0] == '~') {		/* "~user"? */ 
    dirr = tilde_expand(dirname);	/* expand it */
  }
#endif
  dirstrlen = strlen(dirr);
  ispwd = !strcmp(dirr,".");		/* if looking at "." then pwd */

					/* otherwise, we have to check */
  if (stat(dirr,&sbuf) != 0)
      return;
  if (!((sbuf.st_mode & S_IFMT) == S_IFDIR)) {
      return;
  }
  if (!ispwd) {
#ifdef MSDOS
    if (index(STRUCTTERM,dirr[strlen(dirr)-1])) { /* on msdos, */
      char buf[70];			/* check if the directory is */
      int unit;				/* the same as the connected */
      union REGS sregs, oregs;		/* by doing a DOS call to get the */
      struct SREGS segregs;		/* connected dir */
      int i;
      
      segread(&segregs);		/* set segment registers */
      unit = 0;				/* use default structure */
      if (isalpha(*dirr))
	unit = toupper(*dirr) - 'A' + 1; /* no use the specified struct */

      strcpy(buf,dirr);			/* copy the directory name */
      strcat(buf,"/");			/* add a "/" */
      sregs.h.ah = 0x47;		/* DOS int get cwd */
      sregs.x.si = (int) &buf[strlen(buf)];
      sregs.h.dl = unit;
      intdosx(&sregs, &oregs, & segregs); /* do the interrupt */
      for (i = 0; i < strlen(buf); i++)	/* and lowercase the string */
	if (isupper(buf[i])) buf[i] = tolower(buf[i]);
      dirp = opendir(buf);		/* open up directory */
    }
    else
      dirp = opendir(dirr);		/* open directory we are looking at */
#else
    dirp = opendir(dirr);		/* open directory we are looking at */
#endif
    if (dirp == NULL) return;		/* can't...just return */
    if (defstruct(dirr)) {		/* same structure? */
      dotdirp = opendir(".");		/* open up "." */
      if (dotdirp != NULL) {
	d = readdir(dirp);		/* read dir entries */
	dotd = readdir(dotdirp);
	if (d != NULL && dotd != NULL)
#if (BSD|SYSV)
	  ispwd = (d->d_ino == dotd->d_ino); /* are they the same? */
#endif /* (BSD|SYSV) */
#ifdef MSDOS
          ispwd = (!ustrcmp(d->d_name,getcwd(pwdbuf,50)));
#endif
	closedir(dotdirp);
        rewinddir(dirp);
      }
    }					/* if different struct, different */
  }
  else {
    dirp = opendir(dirr);		/* open up directory */
    if (dirp == NULL) return;		/* couldn't...go home */
  }
					/* directory is now open */
  if (!ispwd) {				/* if not CWD, skip ".", ".." */
    d= readdir(dirp);
    if (strncmp(d->d_name,".",14)) {
      rewinddir(dirp);
    }
    else readdir(dirp);
  }
					/* loop through directory */
  for (d = readdir(dirp); d != NULL; d = readdir(dirp)) {
					/* do we need more files? */

    if (filecount == maxfiles) {
      maxfiles += FILEINCR;
      files =(dirfile *)cmrealloc(files, sizeof(dirfile)*(maxfiles));
    }
					/* do we need more name space  */
#if BSD
    if (maxnamelen - (curfile - namebuf) < d->d_namlen+1) 
#endif /* BSD */
#if MSDOS
    if (maxnamelen - (curfile - namebuf) < strlen(d->d_name)+1) 
#endif /* MSDOS */
#if SYSV
#define max(a,b) ((a)>(b)?(a):(b))
    if (maxnamelen - (curfile - namebuf) < max(strlen(d->d_name),14)+1) 
#endif /* SYSV */
    {
      int offset = curfile - namebuf;
      maxnamelen += NAMEINCR;
      namebuf = cmrealloc(namebuf,sizeof(char)*maxnamelen);
      curfile = namebuf + offset;
    }
    f = &files[filecount];		/* set up pointer to current file */
    f->offset = curfile-namebuf;	/* it's offset into namebuf */
#if BSD
    strncpy(curfile,d->d_name,d->d_namlen); /* it's name */
    curfile[d->d_namlen] = '\0';
    curfile += d->d_namlen+1;		/* bump current pointer by namelen */
#endif
#if (MSDOS)
    strcpy(curfile,d->d_name);		/* it's name */
    curfile += strlen(d->d_name)+1;	/* bump current pointer by namelen */
#endif
#if (SYSV)
    strncpy(curfile,d->d_name,14);
    curfile[14] = 0;
    curfile += strlen(curfile) + 1;
#endif
    f->directory = dirname;		/* point to it's directory */
    f->flags = 0;			/* no flags */
    filecount++;
  }
  closedir(dirp);
}
  

/*
 * dirfilecmp:
 * predicate to compare two dirfile struct's for qsort
 */
dirfilecmp(a,b) register dirfile **a,**b; {
  return(strcmp(&namebuf[(*a)->offset],&namebuf[(*b)->offset]));
}


/*
 * ustrcmp:
 * case insensitive string comparison
 */

ustrcmp(s1,s2) char *s1, *s2; {
  register char c1, c2;
  for (;;s1++,s2++) {
    c1 = toupper(*s1);
    c2 = toupper(*s2);
    if (c1 < c2) return(-1);
    if (c1 > c2) return(1);
    if (c1 == '\0') return(0);
  }
}

/* 
 * expand the current directory, and place expansion in 
 * usepath vector after current loc.
 * expand usepath as we go.
 */

expandwilddir(dirname) char *dirname; {
  DIR *dirp;
  struct direct *d;
  static char basename[BUFLEN],fname[BUFLEN],rest[BUFLEN];
  char c;
  static char temp[BUFLEN];
  
  int i,base;
  struct stat s;
  
  base = -1;				/* find the non wild base */
  for (i = 0; i < strlen(dirname); i++) {
    c=dirname[i];
    if (index(DIRSEP,c)) {
      base = i;
      continue;
    }
    if (index(WILDCHARS,c)) break;
  }
  if (base == -1) {			/* no base dir.   use dot */
    strcpy(basename,".");
  }
  else {				/* a base dir.  use it */
    strncpy(temp,dirname,base);
    temp[base] = '\0';
    strcpy(basename,tilde_expand(temp));
  }

  if (strlen(basename) == 0) strcpy(basename,"/");

  fname[0] = '\0';			/* the current wild part */
  for (i = base+1; i < strlen(dirname); i++) {
    if (index(DIRSEP,dirname[i])) break;
    fname[i-(base+1)] = dirname[i];
  }
  fname[i-(base+1)] = '\0';

  i++;					/* get past dirsep */
  if (i < strlen(dirname))
    strcpy(rest,&dirname[i]);		/* and the rest of the string */
  else rest[0] =  '\0';
  if (rest[strlen(rest)-1] == '/') rest[strlen(rest)-1] = '\0';

  if (strcmp(basename,"/") && strcmp(basename,".") && strcmp(basename,"..")) {
    					/* we know these are dirs */
    if (stat(basename,&s) == -1) {	/* and root breaks the msdos stat */
      fprintf(stderr,"stat(%s) failed\n",basename); /* function */
      return;
    }
    if (!((s.st_mode & S_IFMT) == S_IFDIR)) {
      fprintf(stderr,"%s not a dir\n",basename);
      return;
    }
  }
  dirp = opendir(basename);
  if (dirp == NULL) return;
					/* insert directories into path */
  d = readdir(dirp);			/* kill off . and .. */
  if (!strcmp(d->d_name,"."))
    readdir(dirp);
  else rewinddir(dirp);
  for(d = readdir(dirp); d!= NULL; d = readdir(dirp)) {
    if ((d->d_name[0] != '.') || (d->d_name[0] == '.' && fname[0] == '.')) {
      if (fmatch(d->d_name,fname)) {	/* if we match the wildcard */
	static char buf[BUFLEN];
	if (!strcmp(basename,".")) 
	  basename[0] = '\0';
	else {
	  strcpy(buf,basename);
	}
	if (strlen(buf) > 0)
	  if (strcmp(buf,"/")) strcat(buf,"/");
	strcat(buf,d->d_name);
	if (strcmp(buf,".") && strcmp(buf,"..")) {
	  if (stat(buf,&s) == -1) continue;
	  if (!((s.st_mode & S_IFMT) == S_IFDIR)) continue;
	}
	if (strlen(rest) > 0) {
	  strcat(buf,"/");
	  strcat(buf,rest);
	}
	addtopath(buf);			/* add to the path vector */
      }
    }
  }
  closedir(dirp);			/* all done */
}

/*
 * actually add directories to the path in use.
 */
addtopath(dirname) char *dirname; {
  int i;

					/* grow the path */
  usepath =(char**) cmrealloc(usepath,(usepathlen+2)*sizeof(char *));
					/* move everything else down one */
  for (i = usepathlen; i > currentpath; i--) {
    usepath[i+1] = usepath[i];
  }
  usepathlen++;				/* make space for this string */
  usepath[currentpath+1] = malloc(strlen(dirname)+1);
  strcpy(usepath[currentpath+1],dirname); /* and copy it in */
}

/*
 * index function
 * returns index of char c in string s
 */
char *
index(sp,c)				/* get index into a string */
register char *sp, c;
{
  for (; *sp && *sp != c; sp++);
  if (*sp == '\0') return(NULL);
  else return(sp);
}

/*
 * routine to check if a string is wild
 * match all chars in string against all wild chars
 */
 
iswild(str) char *str; {
  int hit[128],i;
  int len1, len2;
  for (i = 0; i < 128; i++) hit[i] = 0;	/* no hits yet */
  len1 = strlen(str);
  for(i = 0; i < len1; i++)		/* mark hits */
    hit[str[i]] = 1;
  len2 = strlen(WILDCHARS);
  for (i = 0; i < len2; i++)		/* and check for them */
    if (hit[WILDCHARS[i]]) return(TRUE);
  return(FALSE);
}

/*
 * check if directory is on connected structure
 */
defstruct(dirname) char *dirname; {
#if (BSD|SYSV)
  return(TRUE);				/* no structures in unix */
#endif
#ifdef MSDOS
  int i;
  char buf[100];
  
  getcwd(buf,100);			/* get connected dir */
  for(i = 0; i < strlen(dirname); i++)
    if (index(STRUCTTERM,dirname[i]))	/* a structure delimiter? */
      return(toupper(dirname[i-1]) == toupper(buf[0]));
  return(TRUE);
#endif
}


/*
 * expand ~user to the user's home directory.
 */
char *
tilde_expand(dirname) char *dirname; {
#ifdef MSDOS
  return(dirname);			/* no users in msdos */
#endif /*  MSDOS */
#ifdef UNIX
  struct passwd *user, *getpwuid(), *getpwnam();
  static char olddir[BUFLEN];
  static char oldrealdir[BUFLEN];
  static char temp[BUFLEN];
  int i;

  if (dirname[0] != '~') return(dirname); /* not a tilde...return param */
  if (!strcmp(olddir,dirname)) return(oldrealdir); /* same as last time. */
					/* so return old answer */
  else {
    for (i = 0; i < strlen(dirname); i++) /* find username part of string */
      if (!index(DIRSEP,dirname[i]))
	temp[i] = dirname[i];
      else break;
    temp[i] = '\0';			/* tie off with a NULL */
    if (strlen(temp) == 1) {		/* if just a "~" */
      user = getpwuid(getuid());	/*  get info on current user */
    }
    else {
      user = getpwnam(&temp[1]);	/* otherwise on the specified user */
    }
  }
  if (user != NULL) {			/* valid user? */
    strcpy(olddir, dirname);		/* remember the directory */
    strcpy(oldrealdir,user->pw_dir);	/* and their home directory */
    strcat(oldrealdir,&dirname[i]);
    return(oldrealdir);
  }
  else {				/* invalid? */
    strcpy(olddir, dirname);		/* remember for next time */
    strcpy(oldrealdir, dirname);
    return(oldrealdir);
  }
#endif
}

#if SYSV

bcopy(src,dest,len) char *src,*dest; int len;
{
    for(; len > 0; len--)
	*dest++ = *src++;
}

DIR *
opendir(name) {
    DIR *dir;
    int fd,open();
    struct stat sbuf;
    int x;

    if (stat(name,&sbuf) != 0) return(NULL);
    if (!(sbuf.st_mode & S_IFDIR)) {
	errno = ENOTDIR;
	return(NULL);
    }
    fd = open(name,O_RDONLY,0);
    if (fd == -1)
	perror("open");
    if (fd == -1) return(NULL);
    if ((dir = (DIR *)malloc(sizeof(DIR))) == NULL) {
	close(fd);
	return(NULL);
    }
    dir->size = sbuf.st_size;
    if ((dir->files = (struct direct *) malloc(dir->size)) == 0) {
	close(fd);
	free(dir);
	return(NULL);
    }
    x = read(fd,dir->files,dir->size);
    if (x == -1) perror("read");
    dir->current = dir->files;
    close(fd);
    return(dir);
}

struct direct *
readdir(dirp) DIR *dirp; {
    int s = dirp->size / sizeof(struct direct);
    struct direct *temp;
    if (dirp->current >= dirp->files + s)
	return(NULL);
    while (dirp->current->d_ino == 0)
	if (dirp->current >= dirp->files + s)
	    return(NULL);
	else dirp->current++;
	    if (dirp->current >= dirp->files + s)
	return(NULL);
    temp = dirp->current++;
    return(temp);
}
    
rewinddir(dirp) DIR *dirp; {
    dirp->current = dirp->files;
}

closedir(dirp) DIR *dirp;
{
    free(dirp->files);
    free(dirp);
}
#endif /* SYSV */
