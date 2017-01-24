.for.rel:
	compile $<

.rel.exe:
	load $<
	save

all:	test.exe


test.exe:	test.rel
