/*
 * Taken from net.sources.   Thanks to some unknown author somewhere
 * out there.
 */

/*
 *	dir.c - contains routines which behave (hopefully) like the
 *		similar versions on 4.2 BSD.
 *
 */
#include <errno.h>
#include <dir.h>

extern int errno;
extern char *calloc();

/*
 *	opendir - opens given directory and reads its contents into memory.
 *	Other functions use this data or the DIR structure to do their work.
 */
DIR *
opendir(dirname)
char *dirname;
{
	struct direct **namelist;
	DIR *dirptr;

	dirptr = (DIR *)calloc(1, sizeof(DIR));
	if(dirptr == (DIR *)0)
	{
		errno = ENOMEM;
		return((DIR *)0);
	}
	dirptr->d_magic = DMAGIC;
	dirptr->d_pos = 0;
	dirptr->d_length = scandir(dirname, &(dirptr->namelist),
		(int (*)())0, (int (*)())0); 
	if(dirptr->d_length < 0)
	{
		free((char *)dirptr);
		return((DIR *)0);
	}
	return(dirptr);
}

/*
 *	readdir - returns the next directory structure from the list and
 *	updates values in the DIR structure.
 */
struct direct *
readdir(dirptr)
DIR *dirptr;
{
	if(dirptr->d_magic != DMAGIC)
	{
		errno = ENOTDIR;
		return((struct direct *)0);
	}
	if(dirptr->d_pos >= dirptr->d_length)
	{
		errno = ENFILE;
		return((struct direct *)0);
	}
	return(dirptr->namelist[dirptr->d_pos++]);
}

/*
 *	telldir - return the current position of the directory.
 */
long
telldir(dirptr)
DIR *dirptr;
{
	if(dirptr->d_magic != DMAGIC)
	{
		errno = ENOTDIR;
		return(-1L);
	}
	return((long)dirptr->d_pos);
}

/*
 *	seekdir - position the given DIR stream to position given.
 */
seekdir(dirptr, loc)
DIR *dirptr;
long loc;
{
	if(dirptr->d_magic != DMAGIC)
	{
		errno = ENOTDIR;
		return(-1);
	}
	if(loc > (long)dirptr->d_length)
	{
		errno = EINVAL;
		return(-1);
	}
	dirptr->d_pos = (int)loc;
	return(0);
}

/*
 *	rewinddir - rewind given DIR to beginning
 */
rewinddir(dirptr)
DIR *dirptr;
{
	if(dirptr->d_magic != DMAGIC)
	{
		errno = ENOTDIR;
		return(-1);
	}
	dirptr->d_pos = 0;
	return(0);
}

/*
 *	closedir - close given directory. destroy given DIR struct so we
 *	know it is closed.
 */
closedir(dirptr)
DIR *dirptr;
{
	if(dirptr->d_magic != DMAGIC)
	{
		errno = ENOTDIR;
		return(-1);
	}
	dirptr->d_magic = ~DMAGIC;	/* mess it up a little */
	freedir(dirptr->namelist);
	free(dirptr);
	return(0);
}




/****************************/

#include <dos.h>

#define NULL (char *)0

static struct direct buffer;

extern char *malloc(), *realloc();

scandir(dirname, namelist, select, compar)
char *dirname;
struct direct *(*namelist[]);
int (*select)();
int (*compar)();
{
	register struct direct **names;
	register int dirno;
	union REGS sregs, oregs;
	struct SREGS segregs;
	char *ptr, *paths;

	names = (struct direct **)calloc(1, sizeof(struct direct *));
	paths = calloc(128, 1);
	if(names == (struct direct **)0 || paths == NULL)
	{
		errno = ENOMEM;
		return(-1);
	}
	strcpy(paths, dirname);
	ptr = &paths[strlen(paths) - 1];
	if(*ptr == '/' || *ptr == '\\')
		*ptr = '\0';
	strcat(paths, "/*.*");

	segread(&segregs);		/* set up segment registers */
	ptr = (char *)&buffer;
	sregs.h.ah = 0x1a;		/* set DTA to buffer */
	sregs.x.dx = FP_OFF(ptr);	/* offset */
#ifdef	M_I86LM
	segregs.ds = FP_SEG(ptr);	/* pointer in large model */
#endif /* M_I86LM */
	intdosx(&sregs, &oregs, &segregs);
	sregs.x.ax = 0x4e00;		/* search for first */
	sregs.x.cx = 0x1f;		/* include all attributes */
	sregs.x.dx = FP_OFF(paths);	/* offset to path */
#ifdef	M_I86LM
	segregs.ds = FP_SEG(paths);	/* segment for large model */
#endif /* M_I86LM */
	intdosx(&sregs, &oregs, &segregs);
	if(oregs.x.cflag)
	{
		errno = ENOTDIR;
		return(-1);
	}
	sregs.x.ax = 0x4f00;		/* search for next */
	for(dirno = 0; oregs.x.cflag == 0; intdosx(&sregs, &oregs, &segregs))
	{
	    if (!(buffer.d_attr & 0x08)) { /* skip Volume label */
		for(ptr = buffer.d_name; *ptr; ptr++)
			*ptr = tolower(*ptr);
		if(select == (int (*)())0 || (*select)(&buffer))
		{
			names = (struct direct **)realloc((char *)names,
				(dirno + 2)*sizeof(struct direct *));
			if(names == (struct direct **)0)
			{
				errno = ENOMEM;
				return(-1);
			}
			names[dirno] = (struct direct *)calloc(1,
				sizeof(struct direct));
			if(names[dirno] == (struct direct *)0)
			{
				errno = ENOMEM;
				return(-1);
			}
			*names[dirno] = buffer;
			names[++dirno] = (struct direct *)0;
		}
	    }
	}

	if(compar != (int (*)())0)
		qsort((char *)names, dirno, sizeof(char *), compar);

	*namelist = names;
	free(paths);			/* free temp space */
	return(dirno);
}

freedir(dirs)
register struct direct **dirs;
{
	register int ii;

	if(dirs == (struct direct **)0)
		return(-1);
	for(ii = 0; dirs[ii] != (struct direct *)0; ii++)
		free(dirs[ii]);
	free(dirs);
	return(0);
}
