
int iarr[5];
const ica = 1;
volatile iva = 2;
int (*vec)(int *);

main(ip)
int *ip;
{
    iva = iva + foo(iva) + ica;
    ip = (int *)&iarr;
    ip[2] = 1;
    vec = main;
    ip[3] = 3;
    vec = &main;
}
