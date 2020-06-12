
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../../../debug.h"

#include "structs.h"
#include "free_str_data.h"

void free_str_data(struct str_data *str_data)
{
    HERE;

    free(str_data->rows);
	
}