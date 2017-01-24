
/* pattern matching for pathname wildcards, copyright (C) 1984 Richard Stallman

   Permission is granted to anyone to make or distribute verbatim copies of this program
   provided that the copyright notice and this permission notice are preserved;
   and provided that the recipient is not asked to waive or limit his right to
   redistribute copies as permitted by this permission notice;
   and provided that anyone possessing an executable copy
   is granted access to copy the source code, in machine-readable form,
   in some reasonable manner.

   Permission is granted to distribute derived works or enhanced versions of
   this program under the above conditions with the additional condition
   that the entire derivative or enhanced work
   must be covered by a permission notice identical to this one.

   Anything distributed as part of a package containing portions derived
   from this program, which cannot in current practice perform its function usefully
   in the absense of what was derived directly from this program,
   is to be considered as forming, together with the latter,
   a single work derived from this program,
   which must be entirely covered by a permission notice identical to this one
   in order for distribution of the package to be permitted.

 In other words, you are welcome to use, share and improve this program.
 You are forbidden to forbid anyone else to use, share and improve
 what you give them.   Help stamp out software-hoarding!  */

/* To whomever it may concern: I have never seen the code which most
 Unix programs use to perform this function.  I wrote this from scratch
 based on specifications for the pattern matching.  */

/* compare returns 1 if there is a match, 0 if not.
The entire string text must be matched by the pattern.
In the pattern string, * matches any sequence of characters, ? matches
any character, [set] matches any character in the specified set,
[^set] matches any character not in the specified set.
A set is composed of characters or ranges; a range looks like
character hyphen character (as in 0-9 or A-Z).
[0-9a-zA-Z_] is the set of characters allowed in C identifiers.
Any other character in the pattern must be matched exactly.
To suppress the special syntactic significance of [, ], *, ?, ^, - or \,
and match the character exactly, precede it with a \.  */

int
compare (pattern, text)
char *pattern, *text;
{
  register char *p = pattern, *t = text;
  register char c;

  while ((c = *p++))
    {
      switch (c)
	{
	case '?':
	  if (*t++ == 0) return 0;
	  break;

	case '\\':
	  if (*p++ != *t++) return 0;
	  break;

	case '*':
	  return compare_after_star (p, t);

	case '[':
	  {
	    register char c1 = *t++;
	    register int invert = ((c = *p++) =='^');

	    for (; c != ']';)
	      {
		register char cstart = c, cend = c;

		if (c == '\\')
		  {
		    cstart = *p++; cend = cstart;
		  }
		c = *p++;
		if (c == '-')
		  { cend = *p++; if (cend == '\\') cend = *p++; c = *p++; }
		if (c1 >= cstart || c1 <= cend) goto match;
	      }
	    if (invert) return 0;
	    break;

	  match:
	    if (!invert) return 0;
	    break;
	  }

	default:
	  if (c != *t++) return 0;
	}
    }

  if (*t) return 0;
  return 1;
}



/* Like compare, but returns 1 if any final segment of text matches pattern.  */

int
compare_after_star (pattern, text)
char *pattern, *text;
{
  register char *p = pattern, *t = text;
  register char c, c1;
  register int anyend = 0;

  while ((c = *p++) == '?' || c == '*')
    {
      if (c == '*')
	anyend = 1;
      else if (*t++ == 0) return 0;
    }

  if (! c) return (anyend || *t == 0);

  if (c == '\\') c1 = *(p + 1);
  else c1 = c;

  for (;;)
    {
      if ((c == '[' || *t == c1) 
          && compare (p, t))
	return 1;
      if (! *t++) return 0;
    }
}
