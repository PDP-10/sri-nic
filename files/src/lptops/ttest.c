#include <stdio.h>
char *name;

main()
{
   int k;

   name = NULL;

   for (k = 2; k >= 0; --k)
     {
       name = tmpnam(NULL);
       fprintf(stderr, "\n%s", name);
       fopen(name, "w");
       fclose(name);
     }
}