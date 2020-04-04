
#include <stdio.h>

#include "flags.h"

#include "process_flags.h"
#include "read_json.h"
#include "preprocess.h"
#include "delete_json_value.h"

#ifdef DEBUGGING_2020
int debugging_depth;
#endif

int main(int n, const char** args)
{
	int error = 0;
	struct flags flags;
	struct json_value* value = NULL;
	
	if(!error)
		error = process_flags(&n, &args, &flags);
	
	if(!error)
		error = read_json(flags.input_filepath, &value);
	
	if(!error)
		error = preprocess(flags.output_filepath, value);
	
	delete_json_value(value);
	
	return error;
}
