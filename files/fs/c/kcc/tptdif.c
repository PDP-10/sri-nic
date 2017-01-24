/* Program to check out pointer difference algorithms */
unsigned char uca, ucb, *ucp;
unsigned _KCCtype_char6 uc6a, uc6b,	*uc6p;	/* KCC specific frobs */
unsigned _KCCtype_char7 uc7a, uc7b,	*uc7p;
unsigned _KCCtype_char8 uc8a, uc8b,	*uc8p;
unsigned _KCCtype_char9 uc9a, uc9b,	*uc9p;
unsigned _KCCtype_char18 uc18a, uc18b,	*uc18p;

main()
{
	tcase(&uca, 9);
	tcase(&uc6a, 6);
	tcase(&uc7a, 7);
	tcase(&uc8a, 8);
	tcase(&uc9a, 9);
	tcase(&uc18a, 18);
}

char *p1, *p2;
int diff;
int nerrs = 0;
tcase(ptr, size)
char *ptr;		/* Actually a ptr of size bits */
{
	int i, j;
	int saverrs = nerrs;

	for(i = 0; i < 20; i++) for(j=0; j < 20; j++)
	  {	p1 = ptr+i;
		p2 = ptr+j;
		diff = p1-p2;
		if (diff != i-j)
		  {	printf("BAD, %d-bit %d-%d is %d, should be %d\n",
				size, i, j, diff, i-j);
			++nerrs;
		  }
	  }
	if (saverrs == nerrs)
		printf("%d-bit BP subtraction OK.\n", size);
	else	printf("%d-bit BP subtraction failed, %d errors.\n",
			size, nerrs-saverrs);
}
