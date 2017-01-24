/* Test sizeof values.
	This test based on one from Pieter Bowman <Bowman@SCIENCE.UTAH.EDU>,
	which he said was very much like a program posted in INFO-C.
*/

#include <stdio.h>

struct {
    unsigned a : 4;
    unsigned b : 4;
    unsigned c : 6;
    int d;
    char e;
    char f[5];
    short g;
} foo = {1, 2, 3, 4, '5', "1234", 24};

void test(f, d, s, c, a)
    float f;
    double d;
    short s;
    char c;
    int a[10];
{
    auto float af;
    auto double ad;
    auto short as;
    auto char ac;
    auto int aa[10];

    fprintf(stdout,
        "sizeof args:   float=%d  double=%d  short=%d  char=%d  int[10]=%d\n",
        (int)sizeof(f), (int)sizeof(d), (int)sizeof(s),
        (int)sizeof(c), (int)sizeof(a));
    fprintf(stdout,
        "sizeof autos:  float=%d  double=%d  short=%d  char=%d  int[10]=%d\n",
        (int)sizeof(af), (int)sizeof(ad), (int)sizeof(as),
        (int)sizeof(ac), (int)sizeof(aa));
    fprintf(stdout,
        "sizeof struct:  foo=%d  a=%d  b=%d  c=%d  d=%d  e=%d  f=%d  g=%d\n",
        (int)sizeof(foo), (int)sizeof(foo.a), (int)sizeof(foo.b),
        (int)sizeof(foo.c), (int)sizeof(foo.d), (int)sizeof(foo.e),
	(int)sizeof(foo.f), (int)sizeof(foo.g));
}

int array[10];

void main()
{
    test(0., 0., 0, 0, array);
    fprintf(stdout,
        "struct foo:  a=%d  b=%d  c=%d  d=%d  e=%c  f=%s  g=%d\n",
        foo.a, foo.b, foo.c, foo.d, foo.e, foo.f, foo.g);
}

#if 0
------------------------------------------------------------
sizeof args:   float=8  double=8  short=4  char=4  int[10]=4
sizeof autos:  float=4  double=8  short=2  char=1  int[10]=40
sizeof struct:  foo=24  a=1  b=1  c=1  d=4  e=1  f=5  g=2
struct foo:  a=1  b=2  c=3  d=4  e=5  f=1234  g=24
------------------------------------------------------------
#endif
