#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "error.h"
#include "structs.h"
#include "read_str.h"


int read_str(const char* str_path, struct str_data *str_data)
{
	char* line = NULL;
    size_t len = 0;
	int i, j, counter = 0;
	double f, f2, f3;
    int error = 0;
	struct str_data sd = {0, NULL};
	unsigned long capacity = 0;
    FILE* str_file = fopen(str_path, "r");
    if (!str_file)
    {
        error = e_failed_to_open_str_file;
    }
   
    while (!error && getline(&line, &len, str_file) > 0)
    {
        line[strlen(line)-1] = '\0';

        switch (line[0])
        {
            case 'c':        
				if (sscanf(line, "c %i %lf %i", &i, &f, &j) != 3)
					error = e_invalid_input_file;
				if(!error)
				{
					if(sd.size + 1 >= capacity)
						sd.rows = realloc(sd.rows, sizeof(struct str_row) * (capacity = capacity * 2 ?:1));
					sd.rows[sd.size++].depth_from_surface = f;

				}
				break;
            case 'n':
				if (sscanf(line, "n %i %i %lf %lf %lf", &i, &j, &f, &f2, &f3) != 5)
					error = e_invalid_input_file;
				if(!error && j)
				{
					sd.rows[counter].boron_concentration = f;
					sd.rows[counter].phosphorus_concentration = f3;
					sd.rows[counter++].net_doping_concentration = (f3 - f);
				}
                break;
            default:
                break;
        }
								
    }
	if (!error)
		*str_data = sd;


/*	for (i = 0; i < sd.size; i++)
	{
		printf("line = %i\n", i);
		printf("Depth from Surface = %f\n", sd.rows[i].depth_from_surface);
		printf("Boron Concentration = %f\n", sd.rows[i].boron_concentration);
		printf("Phosphorus Concentration = %f\n", sd.rows[i].phosphorus_concentration);
		printf("Net Doping Concentration = %f\n\n", sd.rows[i].net_doping_concentration);
	}
*/	
	
    fclose(str_file);
    free(line);

    return error;
}
