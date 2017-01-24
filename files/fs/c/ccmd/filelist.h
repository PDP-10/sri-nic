/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
					/* files to alloc at a time */
#define FILEINCR 100
					/* name space to alloc each time */
#define NAMEINCR 1000

#ifdef UNIX
#define DIRSEP "/"
#define STRUCTTERM ""
#endif /*  UNIX */
#ifdef MSDOS
#define DIRSEP "/\\:"
#define STRUCTTERM ":"
#endif /*  MSDOS */
#define WILDCHARS "*?[{"

typedef struct {
  char *directory;			/* directory file is located in */
  int offset;				/* offset into namebuffer */
  int flags;				/* files flags for this parse */
}  dirfile;

