
#include <stdio.h>
#include <assert.h>

#include "../src/c.h"
#include "../src/b.h"

static const struct
{
	int b_x;
		int c_x;
		int c_retval;
	int b_retval;
} test_cases[] = 
{
	{.b_x = 1, .c_x = 0, .c_retval = -1, .b_retval = -1},
	{.b_x = 2, .c_x = 1, .c_retval =  0, .b_retval =  0},
	{.b_x = 3, .c_x = 2, .c_retval =  1, .b_retval =  1},
};

#define N (sizeof(test_cases) / sizeof(test_cases[0]))

static int test_case_index;

int c(int x)
{
	assert(x == test_cases[test_case_index].c_x);
	return test_cases[test_case_index].c_retval;
}

int main()
{
	int actual_b_retval;
	
	for (test_case_index = 0; test_case_index < N; test_case_index++)
	{
		actual_b_retval = b(test_cases[test_case_index].b_x);
		assert(actual_b_retval == test_cases[test_case_index].b_retval);
	}
	
	return 0;
};



























