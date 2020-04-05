
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "debug.h"

#include "error_codes.h"
#include "flags.h"
#include "process_args.h"

int process_args(
	const char* arg0,
	int* n, const char*** args,
	struct flags* flags)
{
	int error = 0;
	const char* arg;
	ENTER;
	
	/* set defaults: */
	flags->verbose = false;
	flags->input_filepath = NULL;
	flags->pattern_filepath = NULL;
	
	while(!error && (arg = *++*args))
	{
		verpvs(arg);
		if(!strcmp(arg, "-i"))
		{
			if((arg = *++*args))
				flags->input_filepath = arg;
			else
				fprintf(stderr, "%s: '-i' flag requires a path\n", arg0),
				error = e_flag_requires_argument;
		}
		else if(!strcmp(arg, "-p"))
		{
			if((arg = *++*args))
				flags->pattern_filepath = arg;
			else
				fprintf(stderr, "%s: '-p' flag requires a path\n", arg0),
				error = e_flag_requires_argument;
		}
		else if(!strcmp(arg, "-v"))
		{
			flags->verbose = true;
		}
		else if(!strcmp(arg, "-h") || !strcmp(arg, "--help"))
		{
			error = e_help_flag;
		}
		else
			fprintf(stderr, "%s: Unknown flag '%s'\n", arg0, arg),
			error = e_unknown_flag;
	}
	
	if(!error && !(flags->input_filepath && flags->pattern_filepath))
	{
		fprintf(stderr, "%s: both the input and patten paths need to be "
			"set!\n", arg0),
		error = e_flag_not_enough_information;
	}
	
	if(true
		&& !error
		&& flags->input_filepath
		&& flags->pattern_filepath
		&& !strcmp(flags->input_filepath, flags->pattern_filepath))
	{
		fprintf(stderr, "%s: both the input and patten can't be read from "
			"the file!\n", arg0),
		error = e_input_and_pattern_cant_be_same_path;
	}
	
	
	EXIT;
	return error;
}
















