
#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "debug.h"

#include "structs.h"
#include "write_csv.h"
#include "error.h"

int write_csv(const char* csv_path, struct str_data *str_data)
{
	int error = 0;
	unsigned long i;

    HERE;

	FILE* csv_file = fopen(csv_path, "w");
	
	if (!csv_file)
		error = e_failed_to_open_csv_file;
	
	for (i = 0; i < str_data->size && !error; i++)
	{
		if(fprintf(csv_file, "%f, %f, %f, %f\n",
			str_data->rows[i].depth_from_surface,
			str_data->rows[i].boron_concentration, 
			str_data->rows[i].phosphorus_concentration,
			str_data->rows[i].net_doping_concentration) < 0)
		{
			error = e_fprintf_failed;
		}
	}

	fclose(csv_file);
	return error;
}