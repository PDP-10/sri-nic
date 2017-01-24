struct small {
	int stuff[3];	/* More than 2 wds long */
	int sone;
};

struct big {
	int one;
	int two;
	struct small three;
	int four;
} bigone = { 1, 2, {{111, 112, 113}, 12}, 4};

struct big bar();
struct small a;
int res1, res2;
main()
{
	a = bar().three;
	res1 = a.stuff[1];
}
test()
{
	return bar().three.sone;
}
struct big
bar()
{
	return bigone;
}