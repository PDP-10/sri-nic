double three = 3+(1.0/3.0);
double ten = 10.0;
double sum, temp;
float f3, f10, fsum;
int i;
main()
{
	sum = ten;
	for (;;)
	{	temp = sum * three;
		fsum = (float)sum * three;
		sum = temp;
		if (fsum != (float)sum) {
		    printf("Failed at %d!!!  fsum %.20g, sum %.20g, test %g\n",
				i, fsum, (float)sum, 20.0);
		    break;
		}
		++i;
	}
}
