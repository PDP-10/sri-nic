/* TSYMS.C - Check out symbol handling
**	Compile with -d=sym -S to see if the resulting TSYMS.CYM is correct.
**		use -DBADSYN to check error detection of duplicate syms.
*/

/* Verify very long identifiers work both in PP and C */
#define longmacro0123456789012345678901	 1 /* 31 char name */
int a = longmacro0123456789012345678901;
int longident0123456789012345678901 = 2;
int *ip = &longident0123456789012345678901;

/* Check namespace separation of members */
enum etag { e1, e2, e3, memb };
struct stag { int a, b, memb; };
union utag { int a, b, memb; };
enum etag etag;
struct stag stag;
union utag utag;
#define utag() barfage
typedef struct stag_t {int a,b,memb;} stag_t;

stag_t *
routine(etag, stag, utag, a, b)	/* Parameter block */
{
	struct stag {int a,b;};	/* Struct within block */
    etag: {	int etag = a;
	struct
}

#ifdef BADSYN
struct stag {int a,b; };	/* Duplicate tag definition */
struct etag {int a,b; };	/* Another duplicate */
int inta, inta;
rtn()
{
	etag: printf("foo");
	etag: return 1;
}
#endif
