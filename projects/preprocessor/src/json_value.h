
#ifndef JSON_VALUE_STRUCT_H
#define JSON_VALUE_STRUCT_H

#include <stdbool.h>

struct json_value
{
	enum json_kind
	{
		jk_boolean,
		jk_integer,
		jk_double,
		jk_string,
		jk_list,
		jk_object,
	} kind;
	union
	{
		bool bval;
		int ival;
		double dval;
		char* sval;
		struct
		{
			struct json_value** values;
			unsigned int n;
		} lval;
		struct
		{
			struct { char* name; struct json_value* value; }* fields;
			unsigned int n;
		} oval;
	};
};

#endif
