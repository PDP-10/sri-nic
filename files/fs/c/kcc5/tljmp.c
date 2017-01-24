#include <setjmp.h>
jmp_buf j;

foo (a, n)
{
    setjmp (j);
    if (n-- == 0) return;
    printf ("foo: %d\n", a);
    bar (a);
}

bar (b)
{
    printf ("bar: %d\n", ++b);
    longjmp (j);
}

main ()
{
    foo (1, 3);
}
#if 0
CUCS20!dsk:test.exe
foo: 1
bar: 2
foo: 2
bar: 3
foo: 3
bar: 4
CUCS20!journal close
#endif
