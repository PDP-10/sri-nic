/* Test "offsetof" macro */

#define offsetof(s,m) _KCC_offsetof(s,m)

struct foo { int a,b,c; char d, x; short e; char f, y, z, m; };

int offa = offsetof(struct foo,a);
int offb = offsetof(struct foo,b);
int offc = offsetof(struct foo,c);
int offd = offsetof(struct foo,d);
int offx = offsetof(struct foo,x);
int offe = offsetof(struct foo,e);
int offf = offsetof(struct foo,f);
int offy = offsetof(struct foo,y);
int offz = offsetof(struct foo,z);
int offm = offsetof(struct foo,m);

