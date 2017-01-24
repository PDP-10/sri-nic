/*
 *	abort();	Abort process as illegally as possible
 */
void
abort(){
    for(;;)
	asm("\t0\n");	/* 0 is a good illegal instruction */
	/* We loop just in case int handler checks the address for presence
	** of an ERJMP instruction. */
}
