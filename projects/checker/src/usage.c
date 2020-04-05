
#include <assert.h>
#include <stdio.h>

#include "debug.h"

#include "usage.h"

void usage(const char* arg0)
{
	ENTER;
	
	printf(
		"usage: %s: [-i <input filepath>] [-p <pattern filepath>]\n"
		"Checks input file against pattern file, either can be '-', which tells\n"
		"the program to read from stdin\n", arg0);
	
	EXIT;
}
