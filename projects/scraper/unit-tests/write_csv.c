
#include <assert.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <valgrind/memcheck.h>

#include "../src/structs.h"
#include "../src/error_codes.h"
#include "../src/write_csv.h"

static const struct
{
	const char* csv_path;
	struct str_data str_data;
	int retval;
	const char* outputfile_content;
} test_cases[] = 
{
	{
		.csv_path = "/tmp/abc",
		.str_data = {
			.size = 5,
			.rows = (struct str_row[]) {
				{0.1, 0.2, 0.3, 0.4},
				{0.1, 0.2, 0.3, 0.4},
				{0.1, 0.2, 0.3, 0.4},
				{0.1, 0.2, 0.3, 0.4},
				{0.1, 0.2, 0.3, 0.4},
			}
		},
		.retval = e_success,
		.outputfile_content = 
			"0.100000, 0.200000, 0.300000, 0.400000\n"
			"0.100000, 0.200000, 0.300000, 0.400000\n"
			"0.100000, 0.200000, 0.300000, 0.400000\n"
			"0.100000, 0.200000, 0.300000, 0.400000\n"
			"0.100000, 0.200000, 0.300000, 0.400000\n"
	},
	{
		.csv_path = "/tmp/abc/def",
		.str_data = {
			.size = 5,
			.rows = (struct str_row[]) {
				{0.1, 0.2, 0.3, 0.4},
				{0.1, 0.2, 0.3, 0.4},
				{0.1, 0.2, 0.3, 0.4},
				{0.1, 0.2, 0.3, 0.4},
				{0.1, 0.2, 0.3, 0.4},
			}
		},
		.retval = e_failed_to_open_csv_file,
	},
};

#define N (sizeof(test_cases) / sizeof(test_cases[0]))

int main()
{
	int error = 0;
	int test_case_index;
	int retval, fd, read_retval;
	uint8_t* file_content = NULL, chunk[256];
	size_t file_content_memlen, file_content_cap = 0;
	
	for (test_case_index = 0; !error && test_case_index < N; test_case_index++)
	{
		#define T (test_cases[test_case_index])
		
		retval = write_csv(T.csv_path, &T.str_data);
		
		if (retval != T.retval)
			fprintf(stderr, "test case does not match expected return value!\n"),
			error = 1;
		
		if (!error && !retval)
		{
			VALGRIND_MAKE_MEM_UNDEFINED(file_content, file_content_cap);
			
			fd = open(T.csv_path, O_RDONLY);
			
			if (fd < 0)
				perror("open"),
				error = 1;
			
			if (!error)
				for (file_content_memlen = 0;
					(read_retval = read(fd, chunk, sizeof(chunk))) > 0;)
				{
					while (file_content_memlen + read_retval > file_content_cap)
						file_content = realloc(file_content,
							file_content_cap = file_content_cap * 2 ?: 1);
					memcpy(file_content + file_content_memlen, chunk, read_retval);
					file_content_memlen += read_retval;
				}
			
			if (!error && read_retval < 0)
				perror("read"),
				error = 1;
			
			if (fd > 0 && close(fd) < 0)
				perror("close"),
				error = 1;
			
			if (!error && (false
				|| file_content_memlen != strlen(T.outputfile_content)
				|| memcmp(file_content, T.outputfile_content, file_content_memlen)))
				fprintf(stderr, "test case's output file does not match expected!\n"),
				fprintf(stderr, "expected output: \"%s\"!\n", T.outputfile_content),
				fprintf(stderr, "test case's output: \"%s\"!\n", file_content),
				error = 1;
		}
	}
	
	free(file_content);
	
	return error;
};


















