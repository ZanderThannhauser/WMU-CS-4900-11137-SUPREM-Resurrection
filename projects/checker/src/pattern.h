
#ifndef PATTERN_STRUCT_H
#define PATTERN_STRUCT_H

enum requirement_type
{
	rt_exact_string,
	rt_number_range,
	rt_whitespace,
};

struct requirement
{
	enum requirement_type type;
	union
	{
		/* for exact string: */
		struct
		{
			char* chars;
			unsigned int n_chars;
		};
		
		/* for number range: */
		struct
		{
			double min_val, max_val;
		};
		
		/* for whitespace */
		struct
		{
			/* no parameters needed! */
		};
	};
	
	struct requirement* next;
};

struct pattern
{
	struct requirement* head;
};

#endif
