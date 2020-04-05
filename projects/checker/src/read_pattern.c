
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"

#include "error_codes.h"
#include "pattern.h"
#include "read_char.h"
#include "delete_pattern.h"
#include "read_pattern.h"

static int skip_whitespace(
	int fd,
	char* current_char)
{
	int error = 0;
	ENTER;
	while(!error && index(" \t\n\r", *current_char))
	{
		error = read_char(fd, current_char);
	}
	EXIT;
	return error;
}

static int read_requirement(
	const char* arg0,
	int fd,
	char* current_char,
	struct requirement** requirement)
{
	char* chars;
	int error = 0, suberror, n_chars, cap_chars;
	
	struct requirement* r = malloc(sizeof(struct requirement));
	ENTER;
	
	r->next = NULL;
	
	switch(*current_char)
	{
		/* exact string: */
		case '"':
		{
			HERE;
			
			r->type = rt_exact_string;
			chars = NULL, n_chars = 0, cap_chars = 0;
			
			HERE;
			
			/* eat open quote: */
			if((suberror = read_char(fd, current_char)))
				switch(suberror)
				{
					case e_read_char_EOF:
						fprintf(stderr, "%s: unexpected EOF when reading "
							"exact string!\n", arg0),
						error = e_read_exact_string_unexpected_EOF;
						break;
					
					default:
						error = suberror;
						break;
				}
			
			/* until close quote: */
			while(!error && *current_char != '\"')
			{
				if(n_chars + 1 > cap_chars)
					chars = realloc(chars, cap_chars = cap_chars * 2 ?: 1);
				
				verpvc(*current_char);
				
				chars[n_chars++] = *current_char;
				
				HERE;
				
				if((suberror = read_char(fd, current_char)))
				{
					switch(suberror)
					{
						case e_read_char_EOF:
							fprintf(stderr, "%s: unexpected EOF when reading "
								"exact string!\n", arg0),
							error = e_read_exact_string_unexpected_EOF;
							break;
						
						default:
							error = suberror;
							break;
					}
				}
				
				verpvc(*current_char);
			}
			
			/* append null terminator: */
			if(!error)
			{
				if(n_chars + 1 > cap_chars)
					chars = realloc(chars, cap_chars = cap_chars * 2 ?: 1);
				
				chars[n_chars] = '\0';
			}
			
			/* eat close quote: */
			if(!error && (suberror = read_char(fd, current_char)))
				switch(suberror)
				{
					case e_read_char_EOF:
						fprintf(stderr, "%s: unexpected EOF when reading "
							"after exact string!\n", arg0),
						error = e_read_exact_string_unexpected_EOF;
						break;
					
					default:
						error = suberror;
						break;
				}
			
			if(error)
				free(chars),
				chars = NULL;
			else
				r->chars = chars,
				r->n_chars = n_chars;
			
			break;
		}
		
		/* number range: */
		case '(':
		{
			double min, max;
			
			r->type = rt_number_range;
			chars = malloc(1), n_chars = 0, cap_chars = 1;
			
			/* open '(': */
			chars[n_chars++] = *current_char;
			
			do
			{
				if(!error)
					switch((error = read_char(fd, current_char)))
					{
						case e_read_char_EOF:
							fprintf(stderr, "%s: unexpected EOF when reading "
								"number range!\n", arg0),
							error = e_read_number_range_unexpected_EOF;
							break;
						
						default:
							break;
					}
				
				if(n_chars + 1 > cap_chars)
					chars = realloc(chars, cap_chars = cap_chars * 2 ?: 1);
				
				verpvc(*current_char);
				
				chars[n_chars++] = *current_char;
			}
			while(!error && *current_char != ')');
		
			/* eat close ')': */
			if(!error)
				switch ((error = read_char(fd, current_char)))
				{
					case e_read_char_EOF:
						fprintf(stderr, "%s: unexpected EOF when reading "
							"after number range!\n", arg0),
						error = e_read_number_range_unexpected_EOF;
						break;
					
					default:
						break;
				}
			
			/* push null terminator: */
			if(!error)
			{
				if(n_chars + 1 > cap_chars)
					chars = realloc(chars, cap_chars = cap_chars * 2 ?: 1);
			
				chars[n_chars++] = '\0';
			}
			
			verpvs(chars);
			if(!error && sscanf(chars, "(%lf < X < %lf)", &min, &max) != 2)
				fprintf(stderr, "%s: invaild number range format!\n", arg0),
				error = e_read_number_range_invaild_format;
			
			free(chars), chars = NULL;
			
			if(!error)
				r->min_val = min,
				r->max_val = max;
			
			break;
		}
		
		/* whitespace (tabs and spaces ONLY) */
		case '_':
		{
			r->type = rt_whitespace;
			
			/* eat close '_': */
			if(!error)
				switch ((error = read_char(fd, current_char)))
				{
					case e_read_char_EOF:
						fprintf(stderr, "%s: unexpected EOF when reading "
							"after whitespace requirement ('_')!\n", arg0),
						error = e_read_number_range_unexpected_EOF;
						break;
					
					default:
						break;
				}
			
			break;
		}
		
		default:
		{
			fprintf(stderr, "%s: cannot parse pattern file, unexpected '%c'\n",
				arg0, *current_char);
			error = e_read_pattern_syntax_error;
			break;
		}
	}
	
	if(error)
		free(r);
	else
		*requirement = r;
	
	EXIT;
	return error;
}

int read_pattern(
	const char* arg0,
	const char* pattern_filepath,
	struct pattern** pattern)
{
	char current_char;
	bool keep_going = true;
	int error = 0, suberror, fd = 0;
	struct requirement *head = NULL, *here = NULL, *there = NULL;
	ENTER;
	
	if(!strcmp(pattern_filepath, "-"))
		fd = 0;
	else if((fd = open(pattern_filepath, O_RDONLY)) < 0)
		perror("open"),
		error = e_systemcall_failed;
	
	if(!error && (suberror = read_char(fd, &current_char)))
		switch(suberror)
		{
			case e_read_char_EOF:
				error = e_read_pattern_file_empty;
				break;
			
			default:
				error = suberror;
				break;
		}
	
	while(!error && keep_going)
	{
		if(!error && (suberror = skip_whitespace(fd, &current_char)))
			switch(suberror)
			{
				case e_read_char_EOF:
					error = e_success,
					keep_going = false;
					break;
				
				default:
					break;
			}
		
		if(!error && keep_going)
			error = read_requirement(arg0, fd, &current_char, &there);
		
		if(!error && keep_going)
		{
			if(here)
				here->next = there,
				here = there;
			else
				head = here = there;
		}
	}
	
	if(error)
		delete_requirement(head);
	else
		(*pattern) = malloc(sizeof(struct pattern)),
		(*pattern)->head = head;
	
	if(fd != 0 && close(fd) < 0)
		perror("close"),
		error = e_systemcall_failed;
	
	EXIT;
	return error;
}


























