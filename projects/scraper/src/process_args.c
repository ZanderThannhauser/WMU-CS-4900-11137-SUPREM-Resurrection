#include <assert.h>
#include <string.h>

#include "error.h"

#include "structs.h"
#include "process_args.h"

int process_args(int argc, const char *argv[], struct arg_data *arg_data){

    int error = 0;

    if (argc != 3)
        error = e_wrong_number_of_parameters;

    if (!error)
        arg_data->str_path = argv[1],
        arg_data->csv_path = argv[2];

    return error;
}