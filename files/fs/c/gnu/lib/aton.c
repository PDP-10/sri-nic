 7-May-84 09:28:27-EDT,4742;000000000001
Received: from MIT-MC by MIT-OZ via Chaosnet; 7 May 84 09:28-EDT
Received: From bostonu.csnet by csnet-relay;  7 May 84 9:12 EDT
Received: by csvaxa.ARPA (4.12/4.7)
	id AA00824; Sun, 6 May 84 19:37:00 edt
Date: Sun, 6 May 84 19:37:00 edt
From: God <root%bostonu.csnet@csnet-relay.arpa>
Message-Id: <8405062337.AA00824@csvaxa.ARPA>
To: rms@mit-mc.arpa
Subject: atonum.c (should probably be called atof.c)


#ifdef DEBUG
#include <stdio.h>
#else
#define NULL 0
#endif

#define REGARG	register
#define REGISTER register

#define MAXBASE	36
/*
 *	ASCII VERSION!! will not work directly w/ EBCDIC
 */
#define islower(c) (((c) >= 'a') && ((c) <= 'z'))
#define isupper(c) (((c) >= 'A') && ((c) <= 'Z'))
#define isdigit(c) (((c) >= '0') && ((c) <= '9'))
#define isalpha(c) (islower(c) || isupper(c))
#define toupper(c) (islower(c) ? (c) - ' ' : c)
/*
 *	aton -	general purpose conversion of a number from a string
 *		res:	ptr to result cell
 *		s:	string to extract num from
 *		b:	base, if zero then use C rules (0nnn,0xnnn)
 *		return:	NULL if err otherwise ptr to char that stopped
 *			the parse.
 *
 *	AUTHOR
 *		Barry Shein, Boston University
 */
char *
aton(res,s,b) long *res ; REGARG char *s ; int b ;
{
	REGISTER long i = 0 ;
	register int c ;
	int sign = 1 ;

	if((b < 0) || (b > MAXBASE)) return(NULL) ;
	/*
	 *	Skip white space
	 */
	while(*s <= ' ')
		if(*s++ == '\0') return(NULL) ;
	/*
	 *	Check for - or +
	 */
	if(*s == '-')
	{
		++s ;
		sign = -1 ;
	}
	else if(*s == '+') ++s ;
	/*
	 *	If base is zero determine base from string a la C
	 */
	if(b == 0)
	{
		if(*s == '0')
		{
			++s ;
			if(toupper(*s) == 'X')
			{
				b = 16 ;
				++s ;
			}
			else b = 8 ;
		}
		else b = 10 ;
	}
	for(; c = *s ; s++)
	{
		if(isdigit(c))
		{
			c -= '0' ;
			if(c >= b) break ;
		}
		else if(isalpha(c))
		{
			c = toupper(c) - 'A' + 10 ;
			if(c >= b) break ;
		}
		else break ;
		i = i * b + c ;
	}
	*res = i * sign ;
	return(s) ;
}
/*
 *	atoi - Standard Ascii TO Integer, base 10, returns int
 */
int
atoi(s) char *s ;
{
	long i = 0 ;

	aton(&i,s,10) ;
	return((int) i) ;
}
/*
 *	atol - standard Ascii TO Long, base 10, returns long
 */
long
atol(s) char *s ;
{
	long i = 0 ;
	aton(&i,s,10) ;
	return(i) ;
}
/*
 *	atof - accept a number of the form:
 *		(and ignores leading white space)
 *
 *	null	::=
 *	digit	::= 0|1|2|3|4|5|6|7|8|9
 *	digits	::= <digit>*
 *	DIGITS	::= <digit>+
 *	sign	::= <null> | + | -
 *	-------------------------------
 *		accepted:
 *	-------------------------------
 *	integer	::= <sign><DIGITS>
 *	real	::= <integer> . <digits> | <null> . <DIGITS>
 *	epart	::= e <integer> | E <integer>
 *	float	::= <integer> <epart> | <real> <epart>
 *
 *	Always returned as a double
 *
 *	There is no particular attempt to reduce mpys/divs
 *	those machines that are still out there (eg. PDP11/Small)
 *	that shun floating point arithmetic might rethink this routine.
 */
double
atof(s) REGARG char *s ;
{
	REGISTER char *cp ;
	long ipart, epart ;
	int neg = 0 ;
	double res, powr ;

	while(*s <= ' ') if(*s == '\0') return(0.0) ;
	/*
	 *	Need to handle sign here due to '-.3' or '-0.3'
	 *	which aton won't pick up ;
	 */
	if(*s == '-')
	{
		++neg ;
		++s ;
	}
	cp = s ;
	/*
	 *	get ipart handling '.n' case
	 */
	if((s = aton(&ipart,s,10)) == cp)
	{
		if(*s == '.') ipart = 0 ;
		else return(0.0) ;
	}
	res = (double) ipart ;
	/*
	 *	either we find a '.' or e|E or done
	 */
	if(*s == '.')
	{
		++s ;
		for(powr = 10.0 ; isdigit(*s) ; s++,powr *= 10.0)
			res += ((float) (*s - '0'))/powr ;
	}
	/*
	 *	In either case (.) handle E part
	 */
	if(toupper(*s) == 'E')
	{
		++s ;
		aton(&epart,s,10) ;
		if(epart > 0)
			while(epart--) res *= 10.0 ;
		else while(epart++) res /= 10.0 ;
	}
	/*
	 *	fix sign
	 */
	if(neg) res = -res ;
	return(res) ;
}
#ifdef DEBUG
/*
 *	Test driver
 */
main()
{
	char *s ;
	int i ;
	long res ;
	char buf[1024] ;
	double atof() ;

	s = "100" ;
	for(i=2 ; i <= MAXBASE ; i++)
	{
		aton(&res,s,i) ;
		printf("%s as base %d => %d\n",s,i,res) ;
	}
	s = "aBc" ;
	for(i=11 ; i <= MAXBASE ; i++)
	{
		aton(&res,s,i) ;
		printf("%s as base %d => %d\n",s,i,res) ;
	}
	s = "-15" ;
	aton(&res,s,10) ;
	printf("Should be %s: %d\n",s,res) ;
	s = "0x1a" ;
	aton(&res,s,0) ;
	printf("Should be %s: 0x%x\n",s,res) ;
	s = "0200" ;
	aton(&res,s,0) ;
	printf("Should be %s: 0%o\n",s,res) ;
	printf("Test atoi(\"  -12\") => %d\n",atoi("  -12")) ;
	for(;;)
	{
		printf("Number for atof: ") ;
		if(gets(buf) == NULL) break ;
		printf("(15.6f) %15.6f\n",atof(buf)) ;
	}
}
#endif DEBUG
