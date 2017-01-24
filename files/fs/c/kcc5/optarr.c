optarr(x,y)
{	static int array[4][7];
	return array[x][y];
}
#if 0
Produces:			Could be better:
foo:
	MOVE 5,-1(17)
	IMULI 5,7
	XMOVEI 4,array(5)	ADD 5,-2(17)
	ADD 4,-2(17)		--
	MOVE 1,0(4)		MOVE 1,array(5)
	POPJ 17,
#endif
