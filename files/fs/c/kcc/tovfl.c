
main()
{
    char *cp = "1234567890123456789";
    unsigned uv = 0;

    for (; *cp; ++cp) {
	uv = uv*10 + (*cp - '0');
	printf("Added '%c': %#13o %u %d\n", *cp, uv, uv, uv);
    }
}
