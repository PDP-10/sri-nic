/* Definitions for data structures callers pass to functions in the regex library */

#ifndef RE_NREGS
#define RE_NREGS 10
#endif

/* This data structure is used to represent a compiled pattern. */

struct re_pattern_buffer
  {
    char *buffer;	/* Space holding the compiled pattern commands. */
    int allocated;	/* Size of space that  buffer  points to */
    int used;		/* Length of portion of buffer actually occupied */
    char *fastmap;	/* Pointer to fastmap, if any, or zero if none. */
			/* re_search uses the fastmap, if there is one,
			   to skip quickly over totally implausible characters */
    char *translate;	/* Translate table to apply to all characters before comparing.
			   Or zero for no translation.
			   The translation is applied to a pattern when it is compiled
			   and to data when it is matched. */
    char fastmap_accurate;
			/* Set to zero when a new pattern is stored,
			   set to one when the fastmap is updated from it. */
  };

/* Structure to store "register" contents data in.

   Pass the address of such a structure as an argument to re_match, etc.,
   if you want this information back.

   start[i] and end[i] record the string matched by \( ... \) grouping i,
   for i from 1 to RE_NREGS.  start[0] and end[0] record the entire string matched. */

struct re_registers
  {
    int start[RE_NREGS];
    int end[RE_NREGS];
  };

char *re_compile_pattern ();
