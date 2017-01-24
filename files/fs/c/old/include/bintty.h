/*	bintty.h	18-nov-85	*/

extern int ttyopen();	/* open binary-path to TTY:, 0 success, -1 on error */
extern ttycls();	/* close the channel */
extern tyo();		/* output the single byte arg to channel */
extern char tyi();	/* get a single byte from the channel */
