
#include <stdio.h>

#include "b.h"
#include "a.h"

int a(int x)
{
	printf("real: a\n");
	return b(x - 1);
}
