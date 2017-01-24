/* TCOND.C - Test conditional code generation
**	Just compile with -S
*/

int a;
tcond()
{
	if ((char)a) return 0;
	if ((void *)0) return 0;
	return 1;
}
tcondn()
{
	if ((void *)0) return 0;
	return 1;
}
