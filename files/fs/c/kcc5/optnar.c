/* The optimizer here could realize that ANDI is unnecessary, likewise
** the LDB of something we just stored.
*/
    char s[10];
    int x;
optnar()
{
    x = s[0] = 1000000;
	foo();
    s[0] = 1000000;
    x = s[0];
}
#if 0
Produces:				Could be better:
	MOVEI 3,100
	DPB 3,[$$BP90+$$SECT,,s]
	ANDI 3,777			--
	MOVEM 3,x
	PUSHJ 17,foo
	MOVEI 3,100
	DPB 3,[$$BP90+$$SECT,,s]
	LDB 5,[$$BP90+$$SECT,,s]	--
	MOVEM 5,x			MOVEM 3,x
	POPJ 17,
#endif
