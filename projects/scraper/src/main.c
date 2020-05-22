
#include <stdio.h>
#include <assert.h>

#include "structs.h"
#include "error.h"

#include "process_args.h"
#include "read_str.h"
#include "write_csv.h"
#include "free_str_data.h"

int main(int argc, char *argv[])
{
	int error = 0;
	struct arg_data arg_data;
	struct str_data str_data;
	
	
	
	error = process_args(argc, argv, &arg_data);

	if(!error) {
		error = read_str(arg_data.str_path, &str_data);

	}

	if(!error){
		error = write_csv(arg_data.csv_path, &str_data);
		free_str_data(&str_data);
	}

		switch (error)
		{
		case e_success:
			break;

		case e_wrong_parameters:
			break;
		case e_wrong_number_of_parameters:
			fprintf(stderr, "Wrong number of paramerters\n");
			break;
		case e_system_call_failed:
			perror("scraper");
			break;
		
		default:
			assert(0);
			break;
		}

	

	
	return 0;
}
