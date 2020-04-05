
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>

#include "debug.h"

#include "error_codes.h"
#include "pattern.h"
#include "read_char.h"
#include "check_match.h"

int check_match(
	const char* arg0,
	const char* input_filepath,
	bool verbose,
	struct pattern* pattern)
{
	struct requirement* head;
	double number;
	char* buffer, *moving, current_char, *leftover;
	int error = 0, fd = 0, n_chars, cap_chars, countdown;
	ENTER;
	
	verpv(pattern);
	verpv(pattern->head);
	
	if (!strcmp(input_filepath, "-"))
		fd = 0;
	else if ((fd = open(input_filepath, O_RDONLY)) < 0)
		perror("open"),
		error = e_systemcall_failed;
	
	if(!error && pattern->head)
	{
		switch((error = read_char(fd, &current_char)))
		{
			case e_read_char_EOF:
				fprintf(stderr, "%s: input file is empty!\n", arg0),
				error = e_check_input_doesnt_match_pattern;
				break;
			
			default:
				break;
		}
		
		for(head = pattern->head;!error && head;head = head->next)
		{
			switch(head->type)
			{
				case rt_exact_string:
				{
					moving = head->chars;
					countdown = head->n_chars;
					if(verbose)
					{
						printf("matching against exact string: \"%s\"\n", moving);
					}
					
					HERE;
					
					while(!error && countdown--)
					{
						verpvc(current_char);
						HERE;
						
						if(*moving++ != current_char)
							fprintf(stderr, "%s: input and pattern differ in "
								"content!\n", arg0),
							error = e_check_input_doesnt_match_pattern;
						
						if(!error && countdown)
							switch(error = read_char(fd, &current_char))
							{
								case e_read_char_EOF:
									fprintf(stderr, "%s: input reached EOF "
										"while matching exact string in "
										"pattern!\n", arg0),
									error = e_check_input_doesnt_match_pattern;
									break;
							}
					}
					
					/* possibly prepare for next pattern: */
					if(!error && head->next)
						switch(error = read_char(fd, &current_char))
						{
							case e_read_char_EOF:
								fprintf(stderr, "%s: input reached EOF "
									"after exact string in "
									"pattern!\n", arg0),
								error = e_check_input_doesnt_match_pattern;
								break;
						}
					
					break;
				}
				
				case rt_number_range:
				{
					
					buffer = NULL, n_chars = 0, cap_chars = 0;
					
					HERE;
					if(verbose)
					{
						printf("matching number bewtten %f and %f\n",
							head->min_val, head->max_val);
					}
					
					verpv(n_chars);
					verpv(cap_chars);
					
					verpvc(current_char);
					
					/* read in numberic characters: */
					while(!error && index("1234567890.e", current_char))
					{
						HERE;
						if(n_chars + 1 > cap_chars)
							buffer = realloc(buffer,
								cap_chars = cap_chars * 2 ?: 1);
						
						buffer[n_chars++] = current_char;
						
						if(!error)
							switch(error = read_char(fd, &current_char))
							{
								case e_read_char_EOF:
									fprintf(stderr, "%s: input reached EOF "
										"while parsing number!\n", arg0),
									error = e_check_input_doesnt_match_pattern;
									break;
							}
					}
					
					/* append null character: */
					if(!error)
					{
						if(n_chars + 1 > cap_chars)
							buffer = realloc(buffer,
								cap_chars = cap_chars * 2 ?: 1);
						
						buffer[n_chars++] = '\0';
						
						verpvs(buffer);
					}
					
					/* try to parse number: */
					if(!error)
					{
						errno = 0, number = strtod(buffer, &leftover);
						if(errno)
							fprintf(stderr, "%s: input contained number too %s for "
								"a double to contain!\n", arg0,
								number > 0 ? "high" : "low"),
							error = e_check_input_doesnt_match_pattern;
						else if(*leftover)
							fprintf(stderr, "%s: input contained invaild or "
								"malformed number!\n", arg0),
							error = e_check_input_doesnt_match_pattern;
						else
						{
							if(verbose)
								printf("parsed number is %f\n", number);
							
							if(head->min_val > number || number > head->max_val)
								fprintf(stderr, "%s: input's parsed number (%f) is "
									"not in range (%f < X < %f)!\n", arg0, number,
									head->min_val, head->max_val),
								error = e_check_input_doesnt_match_pattern;
						}
					}
					
					free(buffer);
					break;
				}
				
				/* only spaces or tabs: */
				case rt_whitespace:
				{
					verpvc(current_char);
					
					while(!error && index(" \t", current_char))
					{
						if(!error)
							switch(error = read_char(fd, &current_char))
							{
								case e_read_char_EOF:
									fprintf(stderr, "%s: input reached EOF "
										"while skipping over whitespace!\n", arg0),
									error = e_check_input_doesnt_match_pattern;
									break;
							}
					}
					
					break;
				}
				
				default:
					TODO;
					break;
			}
		}
	}
	
	if(!error)
		switch((error = read_char(fd, &current_char)))
		{
			case e_success:
				fprintf(stderr, "%s: input file is longer than matched "
					"pattern!\n", arg0),
				error = e_check_input_doesnt_match_pattern;
				break;
			
			case e_read_char_EOF:
				/* we SHOULD get EOF this time: */
				error = 0;
				break;
		}
	
	
	if (fd != 0 && close(fd) < 0)
		perror("close"),
		error = e_systemcall_failed;
	
	EXIT;
	return error;
}




















