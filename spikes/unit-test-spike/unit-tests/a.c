
#include <stdio.h>
#include <assert.h>

#include "../src/b.h"
#include "../src/a.h"

static const struct
{
	int a_x;
		int b_x;
		int b_retval;
	int a_retval;
} test_cases[] = 
{
	{.a_x = 1, .b_x = 0, .b_retval = -2, .a_retval = -2},
	{.a_x = 2, .b_x = 1, .b_retval = -1, .a_retval = -1},
	{.a_x = 3, .b_x = 2, .b_retval =  0, .a_retval =  0},
};

#define N (sizeof(test_cases) / sizeof(test_cases[0]))

static int test_case_index;

int b(int x)
{
	assert(x == test_cases[test_case_index].b_x);
	return test_cases[test_case_index].b_retval;
}

int main()
{
	int actual_a_retval;
	
	for (test_case_index = 0; test_case_index < N; test_case_index++)
	{
		actual_a_retval = a(test_cases[test_case_index].a_x);
		assert(actual_a_retval == test_cases[test_case_index].a_retval);
	}
	
	return 0;
};



























