/*
 * generate the cmkeyval.h file.
 *
 * figure out what is the largest native type on the system,
 * and typedef keyval to that type.
 */

typedef struct {
    int size;
    char *name;
} type;

typedef int (* cmprocptr)();
type typetab[] = {
    { sizeof(int), "int" },
    { sizeof(char *), "char *" },
    { sizeof(int *), "int *" },
    { sizeof(cmprocptr), "cmprocptr" },
    { sizeof(void *), "void *" },
};

int typetablen = sizeof(typetab)/sizeof(type);

main() {
    int i;
    int max = 0;
    for(i = 0; i < typetablen; i++)
	if (typetab[max].size < typetab[i].size)
	    max = i;
    printf("typedef int (* cmprocptr)();\n");
    printf("typedef %s keyval;\n",typetab[max].name);
    return(0);
}
