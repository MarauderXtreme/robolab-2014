#include <stdio.h>
#include <stdlib.h>

int bit_test()
{
	int i = 0x01;
	int c = 0;

	while(i)
	{
		i = i<<1;
		c += 1;
	}

	return c;
}

int main()
{
	printf("%d\n", bit_test());

	return 0;
}
