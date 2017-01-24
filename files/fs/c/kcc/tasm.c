int foo, bar;
int *barf0();
test()
{
    register int *adr;

	adr = barf0();
	barf2(&(*adr));
	barf3(*adr);
	foo = 123;
	asm("\
	move 1,bar\n\
	movem 1,foo"
	);
	bar = foo;
}
