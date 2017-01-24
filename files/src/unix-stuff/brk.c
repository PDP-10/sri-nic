#include <stdio.h>
main(argc,argv) int argc; char **argv; {
  unsigned char x[16];
  int i;  
  char *bp;

  if (argc != 2)  {
    fprintf(stderr,"expecting 1 arg\n");
    exit(1);
  }
  for (i = 0; i < 16; i++) x[i] = 0xff;
  for (i = 1; i < 128; i++)
    if (bp = (char *)index(argv[1],i)) {
      printf("%c\n",*bp);
      x[i/8] &= ~(1 << 7-(i%8));
    }
  x[0] |= 0x80;				/* don't handle nulls */
  for (i = 0; i < 16; i++) {
    if (!(i % 8)) printf("\n\t");	/* split in half */
    printf("0x");
    if (x[i] < 0x10) printf("0");
    printf("%x, ",x[i]);
  }
  printf("\n");
}

