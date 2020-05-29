#ifndef STRUCTS_H

#define STRUCTS_H

struct arg_data
{
	const char* str_path;
	const char* csv_path;

};

struct str_row
{
    double depth_from_surface;
    double boron_concentration;
    double phosphorus_concentration;
    double net_doping_concentration;
};


struct str_data
{
	unsigned int size;
	struct str_row* rows;
};


#endif
