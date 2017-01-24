/* This code is optimized in a suboptimal way.
** It folds DPB/LDB into just DPB when it probably should not, which counts
** as a bug.
** It also leaves a stray MOVEI around which does nothing.
*/
test()
{	
	char c = 5;
	long l = 2;
	c += l;
}
