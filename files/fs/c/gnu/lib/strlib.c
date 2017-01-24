
/* #define DEBUG 1 */
/*LINTLIBRARY*/
/*
 *	GNULIB
 *
 *	STRLIB - miscellaneous string routines
 *
 *	On a given installation it often makes sense to
 *	hand code these in assembler, particularly if
 *	you can take advantage of block oriented machine
 *	instructions.
 *
 *	AUTHOR
 *		Barry Shein, Boston University (bzs%bostonu@csnet-relay)
 */
#define REGARG	register
#define REGISTER register
#ifndef NULL
#define NULL 0
#endif
/*
 *	strlen - return length of string
 */
strlen(s) REGARG char *s ;
{
	REGISTER i = 0 ;

	while(*s++) i++ ;
	return(i) ;
}
/*
 *	strcmp - compare strings, return difference
 */
strcmp(s,t) REGARG char *s, *t ;
{
	for(; *s == *t ; ++s,++t)
		if(*s == '\0') break ;
	return(*s - *t) ;
}
/*
 *	strncmp - compare first n chars up to null, return difference
 */
strncmp(s,t,n) REGARG char *s, *t ; REGARG int n ;
{
	if(n <= 0) return(0) ;
	while(*s == *t)
		if((*s != '\0') && --n)
		{
			s++ ;
			t++ ;
		}
		else break ;
 	return(*s - *t) ;
}
/*
 *	strcpy - copy a string
 */
char *
strcpy(s,t) REGARG char *s, *t ;
{
	char *r = s ;

	while(*s++ = *t++)
		;
	return(r) ;
}
/*
 *	strncpy - copy at most n chars
 */
char *
strncpy(s,t,n) REGARG char *s, *t ; REGARG int n ;
{
	char *r = s ;

	for(; n-- > 0 ; s++,t++)
		if((*s = *t) == '\0') break ;
	*s = '\0' ;
	return(r) ;
}
/*
 *	strcat - concatenate strings
 */
char *
strcat(s,t) REGARG char *s, *t ;
{
	strcpy(s + strlen(s),t) ;
	return(s) ;
}
/*
 *	strncat - concat at most n chars to string
 */
char *
strncat(s,t,n) REGARG char *s, *t ; REGARG int n ;
{
	strncpy(s + strlen(s),t,n) ;
	return(s) ;
}
/*
 *	index - return ptr to first postion of char in str (or NULL)
 */
char *
index(s,c) REGARG char *s, c ;
{
	for(; *s ; s++)
		if(*s == c) return(s) ;
	return(NULL) ;
}
/*
 *	rindex - return ptr to last postion of char in str (or NULL)
 */
char *
rindex(s,c) REGARG char *s, c ;
{
	REGISTER char *t = s ;

	s += strlen(s) ;
	while(s != t)
		if(*--s == c) return(s) ;
	return(NULL) ;
}
#ifdef DEBUG
#undef NULL
/*
 *	main I used to test routines
 */
#include <stdio.h>
main()
{
	char buf[50], *s, *t ;

	s = "Hello " ;
	t= "World" ;
	printf("s = \"%s\" t = \"%s\"\n",s,t) ;
	strcpy(buf,s) ;
	printf("strcpy(buf,s) => buf = %s\n",buf) ;
	printf("strcmp(buf,s) => %d\n",strcmp(buf,s)) ;
	printf("strncmp(buf,\"Hell\",4) => %d\n",strncmp(buf,"Hell",4)) ;
	printf("'%c' - '%c' => %d\n",'l','c','l'-'c') ;
	printf("strcmp(buf,\"Heck\") => %d\n",strcmp(buf,"Heck")) ;
	printf("strncmp(buf,\"Heck\",4) => %d\n",strncmp(buf,"Heck",4)) ;
	strcat(buf,t) ;
	printf("strcat(buf,t) => buf = %s\n",buf) ;
	printf("index(buf,'l') => %s\n",index(buf,'l')) ;
	printf("rindex(buf,'l') => %s\n",rindex(buf,'l')) ;
	printf("index(buf,'x') => %o (octal)\n",index(buf,'x')) ;
	printf("rindex(buf,'x') => %o (octal)\n",rindex(buf,'x')) ;
	printf("strlen(buf) => %d\n",strlen(buf)) ;
	printf("strncat(buf,s,4) => %s\n",strncat(buf,s,4)) ;
	printf("strncpy(buf,s,4) => %s\n",strncpy(buf,s,4)) ;
	printf("strncpy(buf,s,10) => %s\n",strncpy(buf,s,12)) ;
}
#endif
