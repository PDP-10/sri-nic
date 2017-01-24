/* This code tests unsigned integer division by generating and testing
** all of the possible optimizations and pathways
** for the code sequences that CCOUT's simuidiv() routine generates.
*/

main() {tudiv();}
tudiv()
{
	static unsigned pos = 01000000, neg = -2, pvar = 0123, pv1 = 1;

	printf("UIDIV results:\n\
pos is %o, neg is %o, pvar is %o.\n\
setz	pos/0 = %o ? %o\n\
setz	pos/1 = %o ? %o\n\
lsh	pos/01000 = %o ? %o\n\
divi	pos/01001 = %o ? %o\n\
div []	pos/1,,1 = %o ? %o\n\
case1	pos/pvar = %o ? %o\n\
case2	neg/pvar = %o ? %o\n\
case2.1	neg/[1]  = %o ? %o\n\
case3	pos/[-1] = %o ? %o\n\
case4.1 neg/[-1] = %o ? %o\n\
case4.2	neg/neg  = %o ? %o\n",
		pos, neg, pvar,
		pos/0, pos%0,
		pos/1, pos%1,
		pos/01000, pos%01000,
		pos/01001, pos%01001,
		pos/01000001, pos%01000001,
		pos/pvar, pos%pvar,	/* case 1 */
		neg/pvar, neg%pvar,	/* case 2 */
		neg/pv1, neg%pv1,	/* case 2.1 */
		pos/(-1), pos%(-1),	/* case 3 */
		neg/(-1), neg%(-1),	/* case 4.1 */
		neg/neg, neg%neg	/* case 4.2 */
	);
}
