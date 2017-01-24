/* -*-C-*- strcm2.h */
/*-->strcm2*/
/**********************************************************************/
/******************************* strcm2 *******************************/
/**********************************************************************/


/***********************************************************************
 Compare strings (ignoring case), and return:
	s1>s2:	>0
	s1==s2:  0
	s1<s2:	<0
***********************************************************************/

/* toupper() is supposed to work for all letters, but PCC-20 does it
incorrectly if the argument is not already lowercase; this definition
fixes that. */

#define UC(c) (islower(c) ? toupper(c) : c)

int
strcm2(s1, s2)
register char *s1, *s2;

{
    while ((*s1) && (UC(*s1) == UC(*s2)))
    {
	s1++;
	s2++;
    }
    return((int)(UC(*s1) - UC(*s2)));
}
#undef UC
