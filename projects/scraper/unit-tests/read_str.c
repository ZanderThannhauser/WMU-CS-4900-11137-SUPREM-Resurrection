
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "../src/error_codes.h"
#include "../src/read_str.h"
#include "../src/structs.h"

void free_str_data(struct str_data *str_data)
{
	printf("str_data->rows == %p\n", str_data->rows);
	free(str_data->rows);
}

void test(const char* str_file_content, struct str_data str_data_input)
{
	size_t i;
	
	struct str_data actual_str_data;

	FILE* tmp_str_file = fopen("/tmp/str_file", "w");

	fwrite(str_file_content, strlen(str_file_content), 1, tmp_str_file);
	fwrite("\n", 1, 1, tmp_str_file);
	fclose(tmp_str_file);

	int actual_error = read_str("/tmp/str_file", &actual_str_data);

	assert(actual_error == 0);
	assert(actual_str_data.size == str_data_input.size);

	for (i = 0; i < str_data_input.size; i++)
	{
		assert(actual_str_data.rows[i].depth_from_surface ==
			str_data_input.rows[i].depth_from_surface);
		assert(actual_str_data.rows[i].boron_concentration ==
			str_data_input.rows[i].boron_concentration);
		assert(actual_str_data.rows[i].phosphorus_concentration ==
			str_data_input.rows[i].phosphorus_concentration);
		assert(fabs(actual_str_data.rows[i].net_doping_concentration -
			str_data_input.rows[i].net_doping_concentration) < .000001);
	}
	
	free_str_data(&actual_str_data);
}

void test_error(const char* str_file_content, int expected_error)
{
	struct str_data actual_str_data;

	FILE* tmp_str_file = fopen("/tmp/str_file", "w");

	fwrite(str_file_content, strlen(str_file_content), 1, tmp_str_file);
	fwrite("\n", 1, 1, tmp_str_file);
	fclose(tmp_str_file);

	int actual_error = read_str("/tmp/str_file", &actual_str_data);;

	assert(actual_error == expected_error);
}

int main()
{
	test(
		"c 1 0  0\n"
		"c 2 0.01  0\n"
		"c 3 0.02  0\n"
		"c 4 0.03  0\n"
		"c 5 0.04  0\n"
		"n 0   0   1.000000e+08 1.000000e+08 1.000000e+20 1.000000e+20 1.000000e+00 1.000000e+00 5.000000e-01 0.000000e+00\n"
		"n 0   3   1.633037e+15 1.633037e+15 8.386373e+19 8.386373e+19 8.798462e+16 3.083052e+13 2.408665e-01 2.521051e-01\n"
		"n 1   3   3.565455e+15 3.565455e+15 8.170067e+19 8.170067e+19 8.491928e+16 3.056667e+13 2.408665e-01 2.491949e-01\n"
		"n 2   3   5.264468e+15 5.264468e+15 7.948436e+19 7.948436e+19 8.183090e+16 3.029817e+13 2.408665e-01 2.461365e-01\n"
		"n 3   3   6.692956e+15 6.692956e+15 7.721410e+19 7.721410e+19 7.872240e+16 3.002522e+13 2.408665e-01 2.429190e-01\n"
		"n 4   3   7.841758e+15 7.841758e+15 7.488936e+19 7.488936e+19 7.559708e+16 2.974810e+13 2.408665e-01 2.395307e-01\n",
		(struct str_data) {5, (struct str_row[]) {
			{0.000000, 1633037000000000.000000, 83863729999999991808.000000, 83862096962999992320.000000},
			{0.010000, 3565455000000000.000000, 81700670000000008192.000000, 81697104545000013824.000000},
			{0.020000, 5264468000000000.000000, 79484360000000000000.000000, 79479095532000002048.000000},
			{0.030000, 6692956000000000.000000, 77214100000000000000.000000, 77207407044000006144.000000},
			{0.040000, 7841758000000000.000000, 74889360000000000000.000000, 74881518241999994880.000000}
		}});
	
	printf("line 80\n");
	
	test_error(
		"c 1 0  0\n"
		"c 2 0.01  0\n"
		"c 3 0.02  0\n"
		"c 4 0.03  0\n"
		"c 5 0.04  0\n"
		"n 0   0   1.000000e+08 1.000000e+08 1.000000e+20 1.000000e+20 1.000000e+00 1.000000e+00 5.000000e-01 0.000000e+00\n"
		"n 0   3   1.633037e+15 1.633037e+15 8.386373e+19 8.386373e+19 8.798462e+16 3.083052e+13 2.408665e-01 2.521051e-01\n"
		"n 1   3   3.565455e+15 3.565455e+15 8.170067e+19 8.170067e+19 8.491928e+16 3.056667e+13 2.408665e-01 2.491949e-01\n"
		"n 2   3   5.264468e+15 5.264468e+15 7.948436e+19 7.948436e+19 8.183090e+16 3.029817e+13 2.408665e-01 2.461365e-01\n"
		"n 3   3   6.692956e+15 6.692956e+15 7.721410e+19 7.721410e+19 7.872240e+16 3.002522e+13 2.408665e-01 2.429190e-01\n",
		e_invalid_input_file);

	return 0;
}








