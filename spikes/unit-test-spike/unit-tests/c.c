
#include <stdio.h>
#include <assert.h>

#include "../src/c.h"

static const struct
{
	int c_x;
	int c_retval;
} test_cases[] = 
{
	{.c_x = 1, .c_retval = +0},
	{.c_x = 2, .c_retval = +1},
	{.c_x = 3, .c_retval = +2},
};

#define N (sizeof(test_cases) / sizeof(test_cases[0]))

static int test_case_index;

int main()
{
	int actual_c_retval;
	
	for (test_case_index = 0; test_case_index < N; test_case_index++)
	{
		actual_c_retval = c(test_cases[test_case_index].c_x);
		assert(actual_c_retval == test_cases[test_case_index].c_retval);
	}
	
	return 0;
};



























