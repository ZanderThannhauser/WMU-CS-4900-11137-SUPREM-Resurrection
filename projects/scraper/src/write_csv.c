
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "debug.h"

#include "structs.h"
#include "write_csv.h"
#include "error_codes.h"

int write_csv(const char* csv_path, const struct str_data *str_data)
{
	int error = 0;
	unsigned long i;

	FILE* csv_file = fopen(csv_path, "w");
	
	if (!csv_file)
		error = e_failed_to_open_csv_file;
	
	for (i = 0; !error && i < str_data->size; i++)
	{
		if (fprintf(csv_file, "%f, %f, %f, %f\n",
			str_data->rows[i].depth_from_surface,
			str_data->rows[i].boron_concentration, 
			str_data->rows[i].phosphorus_concentration,
			str_data->rows[i].net_doping_concentration) < 0)
		{
			error = e_fprintf_failed;
		}
	}
	
	if (csv_file)
		fclose(csv_file);
	
	return error;
}
