#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../src/error.h"
#include "../src/process_args.h"
#include "../src/structs.h"


void test(int argc_input, const char *argv_input[],
struct arg_data arg_data){

    struct arg_data actual_arg_data = {NULL, NULL};

    int actual_error = process_args(argc_input, argv_input, &actual_arg_data);

    assert(actual_error == 0);
    assert(actual_arg_data.str_path != NULL);
    assert(actual_arg_data.csv_path != NULL);
    assert(!strcmp(actual_arg_data.str_path, arg_data.str_path));
    assert(!strcmp(actual_arg_data.csv_path, arg_data.csv_path));
    
}

void test_error(int argc_input, const char *argv_input[], int expected_error){

    struct arg_data actual_arg_data = {NULL, NULL};

    int actual_error = process_args(argc_input, argv_input, &actual_arg_data);

    assert(actual_error == expected_error);

}

int main(){

    int error;
    
    test(3, (const char*[]){"foo", "bar", "baz"}, (struct arg_data){"bar", "baz"});


    test_error(4, (const char*[]){"foo", "bar", "baz", "qux"}, e_wrong_number_of_parameters);

    test_error(2, (const char*[]){"quux", "grault"}, e_wrong_number_of_parameters);

    return 0;

}

