/*************************************************************************
 *									 *
 *   Original : MEL         Stanford University        Sept, 1984	 *
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   input.c                Version 5.1     */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "suprem/include/shell.h"

/* 2020 includes:*/
#include "macro.h"
#include "../xsupr4/interface.h"
#include "input.h"
/* end of includes*/

/* 2020 forward declarations*/
#ifndef HAVE_KSH
char *read_line(char *prompt);
#endif
/* end of declarations*/

/************************************************************************
 *									*
 *	get_input(in) - this reads a line of input from in and loads    *
 *  the input buffer with it.						*
 *									*
 *	Original	Mark E. Law		Sept, 1984		*
 *									*
 ************************************************************************/
void get_input(FILE *in, int per)
{
	int   i;
	char *s;

	/*if nothing left in the super buffer*/
	if (!*(supbuf + supbpt))
	{

		/*clear the current pointers*/
		supbpt = 0;

		/*read input until and end of file or end of line*/
		if (isatty(fileno(in)))
		{
			s = read_line(prompt);
			strcpy(supbuf, s);
			strcat(supbuf, "\n");
			i = strlen(supbuf);
			free(s);
		}
		else
		{
			if (fgets(supbuf, supbln, in) == NULL)
				i = 0;
			else
				i = 1;
		}
		
		/*fix up the values*/
		if (i == 0)
		{
			/*add on the newline or eof*/
			supbuf[0] = '\001';
			supbuf[1] = '\0';
		}
	}

	check_x();
	
	/*read an input line into the lex buffer and macro expand*/
	for (bufptr = 0;
		 supbuf[supbpt] && (buffer[bufptr] = supbuf[supbpt]) != '\n';
		 bufptr++, supbpt++)
		;
	if (supbuf[supbpt] == '\n')
	{
		buffer[bufptr++] = '\n';
		supbpt++;
	}
	buffer[bufptr++] = '\0';
	bufptr = 0;

	/*only macro expand a new line if we are reading and executing*/
	if ((depth == -1) && per)
	{
		/*do the macro expansions*/
		if (expand_macro(&buffer, &buflen, macro) == -1)
		{
			strcpy(buffer, "\n");
		}
	}
}

#ifndef HAVE_KSH
char *read_line(char *prompt)
{
	char *s = malloc(128);

	fputs(prompt, stderr);
	assert(fgets(s, 128, stdin));
	return (s);
}
#endif
