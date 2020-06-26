
#include <stdio.h>

#include "c.h"
#include "b.h"

int b(int x)
{
	printf("real: b\n");
	return c(x - 1);
}
