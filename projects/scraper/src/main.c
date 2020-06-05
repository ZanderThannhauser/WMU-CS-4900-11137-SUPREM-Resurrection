
#include <stdio.h>
#include <assert.h>

#include "debug.h"

#include "structs.h"
#include "error.h"

#include "process_args.h"
#include "read_str.h"
#include "write_csv.h"
#include "free_str_data.h"

#ifdef DEBUGGING_2020
int debugging_depth;
#endif

int main(int argc, char *argv[])
{
	int error = 0;
	struct arg_data arg_data;
	struct str_data str_data;
		
	error = process_args(argc, argv, &arg_data);

    verpv(error);

	if (!error)
		error = read_str(arg_data.str_path, &str_data);

    verpv(error);
	
	if (!error)
		error = write_csv(arg_data.csv_path, &str_data),
		free_str_data(&str_data);

	switch (error)
	{
		case e_success:
			break;

		case e_wrong_number_of_parameters:
			fprintf(stderr, "Wrong number of paramerters\n");
			break;
		
		case e_system_call_failed:
			perror("scraper");
			break;
		
		case e_failed_to_open_str_file:
			fprintf(stderr, "e_failed_to_open_str_file\n");
			break;
		
		case e_invalid_input_file:
			fprintf(stderr, "checker: Invaild .str file!\n");
			break;
		
		case e_no_data_in_str_file:
			fprintf(stderr, "Empty .str file or no datapoints to scrape from .str file!\n");
			break;
		
		default:
			assert(0);
			break;
	}

	return error;
}
