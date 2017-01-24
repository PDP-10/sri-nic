#define offsetof(s,m) \
	( (char *) (&((struct s *)0)->m) - (char *)0)
struct foo {
	short a,b,c;
};
int res;
main()
{
    res = offsetof(foo,c);
}
