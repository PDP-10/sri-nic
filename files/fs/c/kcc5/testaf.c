
char foo[12][14][16] = { 0123};
char *p1, *p2, *p3;

int i;
main()
{
	printf("foo     = %o\n", foo);
	printf("*foo    = %o\n", *foo);
	printf("**foo   = %o\n", **foo);
	printf("***foo  = %o\n", ***foo);
/*12*/	printf("&foo    = %o\n", &foo);
/*13*/	printf("&*foo   = %o\n", &*foo);
/*14*/	printf("&**foo  = %o\n", &**foo);
	printf("&***foo = %o\n", &***foo);
/*16*/	printf("&*foo   = %o\n", &*foo);
/*17*/	printf("*&*foo  = %o\n", *&*foo);

/*19*/	p1 = foo;
/*20*/	p2 = *foo;
	p3 = **foo;
/*	i = ***foo;	*/
}
