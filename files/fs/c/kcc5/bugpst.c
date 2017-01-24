/* This produces an internal error message because pcode finds that it
** is about to generate a positive stack reference.  However, this is actually
** the fault of the user, and the now[2] reference should cause a
** USER error message!
**
** This should be caught by a general array bounds check.
** Note that it should still be possible to generate &now[2] (but not
** &now[3]) as that is explicitly allowed by ANSI for pointer comparison.
*/
bugpst()
{
    int *ip, now[2];

    ip = 0;
    ip = &now[2];		/* This should work OK */
    printf("garsh");
    now[1] = now[2] = time();	/* This should complain */
    utime("test.txt", now);
}
