
#include <stdio.h>

#include "debug.h"

#include "flags.h"
#include "usage.h"
#include "pattern.h"
#include "error_codes.h"
#include "check_match.h"
#include "process_args.h"
#include "read_pattern.h"
#include "delete_pattern.h"

#ifdef DEBUGGING_2020
int debugging_depth;
#endif

int main(int n, const char** args)
{
	int error = 0;
	const char* arg0 = *args;
	struct flags flags;
	struct pattern* pattern = NULL;
	ENTER;
	
	verpvs(arg0);
	
	if(!error)
		error = process_args(arg0, &n, &args, &flags);
	
	switch(error)
	{
		case e_flag_not_enough_information:
		{
			usage(arg0);
			break;
		}
		
		case e_help_flag:
		{
			usage(arg0);
			error = e_success;
			break;
		}
		
		case e_success:
		{
			if(!error)
				error = read_pattern(arg0, flags.pattern_filepath, &pattern);
			
			if(!error)
				error = check_match(arg0, flags.input_filepath,
					flags.verbose, pattern);
			
			break;
		}
	}
	
	delete_pattern(pattern);
	
	EXIT;
	return error;
}
























