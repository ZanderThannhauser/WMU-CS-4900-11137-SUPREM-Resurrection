
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#include "pattern.h"
#include "delete_pattern.h"

void delete_requirement(struct requirement* requirement)
{
	ENTER;
	if(requirement)
	{
		switch(requirement->type)
		{
			case rt_exact_string:
				free(requirement->chars);
				break;
			
			case rt_number_range:
			case rt_whitespace:
				break;
			
			default:
				TODO;
				break;
		}
		
		delete_requirement(requirement->next);
		
		free(requirement);
	}
	EXIT;
}

void delete_pattern(struct pattern* pattern)
{
	ENTER;
	if(pattern)
	{
		delete_requirement(pattern->head);
		free(pattern);
	}
	EXIT;
}
