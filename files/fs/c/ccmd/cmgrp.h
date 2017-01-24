/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/


/*
 * symbols for username parser.
 */

#define GRP_EXACT 1
#define GRP_PARTIAL 2

struct grp {
  struct group *grp;
  int flags;
};
