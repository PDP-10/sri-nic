#if 0
18-Aug-87 19:52:24-PDT,3498;000000000000
Return-Path: <info-c-request@brl-smoke.arpa>
Received: from BRL-SMOKE.ARPA by SRI-NIC.ARPA with TCP; Tue 18 Aug 87 19:51:42-PDT
Received: from BRL-SMOKE.ARPA by SMOKE.brl.ARPA id az00186; 13 Aug 87 16:13 EDT
Received: from brl-adm.arpa by SMOKE.BRL.ARPA id aa11436; 12 Aug 87 0:01 EDT
Received: from USENET by ADM.BRL.ARPA id aa04268; 11 Aug 87 23:49 EDT
From: Fred Fish <fnf@mcdsun.uucp>
Newsgroups: comp.lang.c
Subject: possible structure initialization bug
Message-ID: <352@mcdsun.UUCP>
Date: 11 Aug 87 20:40:42 GMT
To:       info-c@brl-smoke.arpa

The following possible bug report was generated due in part to
failure to compile code from a compiler test suite.  I'd like
to solicit opinions on whether or not the code is valid pre-ANSI
C, valid ANSI C, or not valid C.  Note also that Harbison and Steele
go into a little more detail than the draft ANSI standard, and also
introduce the notion that the "shape" of the initializers should
match the "shape" of the object being initialized, which in this case
I would take to mean that a valid initializer would be of the form
"{{a,b},{c,d}}".

-Fred
#endif

/*
 *	Possible structure initialization bug (see comments below).
 *	Results from various compilers are:
 *
 *	GNU C compiler:		No complaint.
 *				bar.foo[0][0] = 1;
 *				bar.foo[0][1] = 2;
 *				bar.foo[1][0] = 3;
 *				bar.foo[1][1] = 4;
 *
 *	SUN 3.2 C compiler:	complains about '}' expected
 *	(pcc based)		bar.foo[0][0] = 1;
 *				bar.foo[0][1] = 2;
 *				bar.foo[1][0] = 0;
 *				bar.foo[1][1] = 0;
 *
 *	Motorola 68020 cc	complains about '}' expected
 *	(pcc based)		bar.foo[0][0] = 1;
 *				bar.foo[0][1] = 2;
 *				bar.foo[1][0] = 0;
 *				bar.foo[1][1] = 0;
 *
 *	Greenhills 68020 cc	complains about initializer too large
 *				no output file generated.
 *
 *	The draft ANSI C standard is quite specific about the
 *	behavior when there are too few initializers for an
 *	aggregate or subaggregate, but doesn't really address
 *	the reverse situation of too many initializers (that I could
 *	find anyway).  Section 3.5.6 has this to say:
 *
 *	"There shall be no more initializers in an initializer list than
 *	there are objects to be initialized."   Well, I see four objects
 *	to initialize and four initializers, so read on...
 *
 *	"... the initializer for an object that has aggregate type shall
 *	be a brace-enclosed list of initializers for the members of the
 *	aggregate, wirttin in increasing subscript or member order.  If the
 *	aggregate contains members that are aggregates, the rules apply
 *	recursively to the subaggregates.  If the initializer of a sub-
 *	aggregate begins with a left brace, the succeeding initializers
 *	initialize the members of the subaggregate.  Otherwise, only
 *	enough initializers from the list are taken to account for the
 *	members of the first subaggregate; any remaining initializers
 *	are left to initialize the next member of the subaggregate of
 *	which the current aggregate is a part."
 *
 */

struct {
    int foo[2][2];
} bar = {
    {1, 2, 3, 4}
};

main ()
{
    printf ("bar.foo[0][0] = %d\n", bar.foo[0][0]);
    printf ("bar.foo[0][1] = %d\n", bar.foo[0][1]);
    printf ("bar.foo[1][0] = %d\n", bar.foo[1][0]);
    printf ("bar.foo[1][1] = %d\n", bar.foo[1][1]);
}
#if 0
-- 
= Drug tests; just say *NO*!
= Fred Fish  Motorola Computer Division, 3013 S 52nd St, Tempe, Az 85282  USA
= seismo!noao!mcdsun!fnf    (602) 438-3614
#endif
