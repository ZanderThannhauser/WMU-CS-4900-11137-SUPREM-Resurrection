#ifndef STRUCTS_H

#define STRUCTS_H

struct arg_data
{
    const char* str_path;
    const char* csv_path;

};

struct str_data
{
    unsigned int size;
    double* depth_from_surface;
    double* boron_concentration;
    double* phosphorus_concentration;
    double* net_doping_concentration;
};


#endif
