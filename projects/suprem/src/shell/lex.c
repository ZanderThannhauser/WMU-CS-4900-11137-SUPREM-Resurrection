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
/*   lex.c                Version 5.1     */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suprem/include/global.h"
#undef FOR

#include "suprem/include/constant.h"
#include "suprem/include/shell.h"
#include "suprem/include/sysdep.h"

/* 2020 includes:*/
#include "debug.h"
#include "parser.h"
#include "input.h"
#include "lex.h"
/* end of includes*/

/* 2020 forward declarations*/
int lex_command();
/* end of declarations*/

#define C_STATE 1
#define PAR_STATE 2
#define READ_STATE 3
#define FOR_STATE 4

/************************************************************************
 *									*
 *	This file contains the routines that handle lexical analysis.	*
 *  These routines rely heavily on previous state knowledge.  		*
 *  Fortunately, an end of line curse all ills.  The basic states are	*
 *  are the following:							*
 *									*
 *  C_STATE - This is the command state.  Everything is read until a	*
 *	command delimeter is detected.					*
 *  PAR_STATE - This is the state of parameter reading.  Everything is	*
 *	eaten until a shell special character or newline appears.	*
 *	(comment: process real number expressions here?)		*
 *  READ_STATE - Everything until the end of the line is consumed.	*
 *	Useful for the define statement which turns off all other 	*
 *	processing.							*
 *  FOR_STATE - The rest of the line is parsed into two pieces, the	*
 *	variable and the list of values.				*
 *									*
 *  A one anywhere in the character stream indicates end of file.	*
 *									*
 ************************************************************************/

static int   ylstate = C_STATE;
static char *par_sep = ";>&\n\001";
char *		 read_quote(char *str);
char *		 read_until(char *str);

/************************************************************************
 *									*
 *	fetchc(in) - this routine returns characters off the input 	*
 *  buffer.  If the input buffer is finally empty, the routine 		*
 *  get_input is called.  get_input handles macro processing.		*
 *									*
 ************************************************************************/
char fetchc(FILE *in, int per)
{
	char c;
	int  i;

	/*this routine is very simple, just return character in the buffer*/
	c = *(buffer + bufptr++);

	if (c == '\0')
	{
		/*get and process some more input*/
		get_input(in, per);
		c = *(buffer + bufptr++);
	}

	*(echo_buffer + echo_bufptr++) = c;
	if (echo_bufptr == echo_buflen)
	{
		echo_buflen = 2 * echo_buflen;
		echo_buffer = sralloc(char, echo_buflen, echo_buffer);
	}

	/*store away any data that may be neccessary for the commands*/
	for (i = 0; i <= depth; i++)
	{
		*(store[i] + (store_ptr[i])++) = c;
		if (store_ptr[i] == store_len[i])
		{
			store_len[i] = 2 * store_len[i];
			store[i] = sralloc(char, store_len[i], store[i]);
		}
	}

	/*nobody wants to see percent signs*/
	if ((c == '%') && per)
		c = ' ';

	return (c);
}

/*define a macro to push characters back on the input buffer*/
#define pushc(c)                                                               \
	{                                                                          \
		int i;                                                                 \
		echo_bufptr--;                                                         \
		*(buffer + --bufptr) = c;                                              \
		for (i = 0; i <= depth; i++)                                           \
			store_ptr[i]--;                                                    \
	}

/************************************************************************
 *									*
 *	yylex() - This routine handles in a simpler fashion the tasks	*
 *  normally coded in lex.  Hopefully, this is will be faster and 	*
 *  smaller than that generated by lex.  The state information was 	*
 *  described above.							*
 *									*
 *  Original:	MEL	2/85						*
 *									*
 ************************************************************************/
int yylex()
{

	static int for_spot = FALSE;
	char	   s;
	int		   error = FALSE, len;

	/*a legal return will end this night mare*/
	while (TRUE)
	{
		switch (ylstate)
		{
			case C_STATE:
				return (lex_command());
				break;

			case PAR_STATE: /*read until a separator character*/
				yylval.sval = read_quote(par_sep);
				ylstate = C_STATE;
				if (strlen(yylval.sval) != 0)
					return (PARAMETER);
				break;

			case READ_STATE: /*read until the end of a line*/
				yylval.sval = read_until("\n\001");
				ylstate = C_STATE;
				if (strlen(yylval.sval) != 0)
					return (PARAMETER);
				break;

			case FOR_STATE: /*if for state is false, we just got here*/
				if (!for_spot)
				{
					/*skip leading white space*/
					while (isspace(s = fetchc(in_file, TRUE)))
						;
					pushc(s);

					/*read off the name first*/
					yylval.sval = (char *)read_until("\n\t (\001");
					for_spot = TRUE;
					if (strlen(yylval.sval) != 0)
						return (NAME);
					else
					{
						for_spot = FALSE;
						return (PARAMETER);
					}
				}
				else
				{
					/*skip leading white space*/
					while (isspace(s = fetchc(in_file, TRUE)))
						;

					/*this had best be a paren*/
					if (s == '(')
					{
						/*stick it back so it leads the string*/
						pushc(s);
					}
					else
						error = TRUE;

					yylval.sval = (char *)read_until("\n)\001");

					/*character at the end better be right paren*/
					if ((s = fetchc(in_file, TRUE)) != ')')
					{
						error = TRUE;
						pushc(s);
					}
					else
					{
						len = strlen(yylval.sval);
						*(yylval.sval + len) = s;
						*(yylval.sval + len + 1) = '\0';
					}

					for_spot = FALSE;
					ylstate = C_STATE;
					if (strlen(yylval.sval) != 0)
					{
						if (error)
							return (PARAMETER);
						else
							return (LIST);
					}
				}
				break;
		}
	}
}

/************************************************************************
 *									*
 *	lex_command() - This routine implements the zero state of the	*
 *  old lexical analyzer.  The routine is reasonably straightforward.	*
 *  Characters are read until possibilities are elimanated.  		*
 *									*
 *  Original: 	MEL	2/85						*
 *									*
 ************************************************************************/
int lex_command()
{
	ENTER;
	char  c;
	char *s;

	while (!isalnum(c = fetchc(in_file, TRUE)))
	{
		/*do single character tests*/
		switch (c)
		{
			case '#': /*return the comment*/
				ylstate = READ_STATE;
				EXIT;
				return (COMMENT);
				break;
			case '!': /*return the shell escape*/
				ylstate = READ_STATE;
				EXIT;
				return (BANG);
				break;
			case '\n': /*return the end of line*/
				EXIT;
				return (EOL);
				break;
			case '\01':		 /*return the end of file characted*/
				pushc('\n'); /*get yacc all resynced*/
				EXIT;
				return (ENDFILE);
				break;
			case '>': /*return redirection request*/
				ylstate = PAR_STATE;
				EXIT;
				return (REDIRECT);
				break;
			case '&': /*return background*/
				EXIT;
				return (BACK);
				break;
			case '{': /*this is the beginning of the grouping stuff*/
				EXIT;
				return (BG_GRP);
				break;
			case '}': /*this is the end of the grouping stuff*/
				EXIT;
				return (END_GRP);
				break;
			case ';': /*command delimiter*/
				EXIT;
				return (DELIMIT);
				break;
			case '?': /*help command*/
				ylstate = PAR_STATE;
				EXIT;
				return (HELP);
				break;
			default: /*ignore any other bizarre characters*/
				break;
		}
	}

	/*if we are here, we got a alphanumeric*/

	/*push the character back on the stack*/
	pushc(c);

	/*read until a commadn separator*/
	s = (char *)read_until(";&>{} \t\n");

	/*now parse this string with respect to all the builtins*/
	if (strcmp(s, "quit") == 0)
	{
		EXIT;
		return (QUIT);
	}
	else if (strcmp(s, "exit") == 0)
	{
		EXIT;
		return (QUIT);
	}
	else if (strcmp(s, "bye") == 0)
	{
		EXIT;
		return (QUIT);
	}
	else if (strcmp(s, "logout") == 0)
	{
		EXIT;
		return (QUIT);
	}
	else if (strcmp(s, "end") == 0)
	{
		EXIT;
		return (END);
	}
	else if (strcmp(s, "source") == 0)
	{
		ylstate = PAR_STATE;
		EXIT;
		return (SOURCE);
	}
	else if (strcmp(s, "for") == 0)
	{
		ylstate = FOR_STATE;
		EXIT;
		return (FOR);
	}
	else if (strcmp(s, "foreach") == 0)
	{
		ylstate = FOR_STATE;
		EXIT;
		return (FOR);
	}
	else if (strcmp(s, "end") == 0)
	{
		EXIT;
		return (END);
	}
	else if (strcmp(s, "undef") == 0)
	{
		ylstate = READ_STATE;
		EXIT;
		return (UNDEF);
	}
	else if (strcmp(s, "define") == 0)
	{
		ylstate = READ_STATE;
		EXIT;
		return (DEFINE);
	}
	else if (strcmp(s, "set") == 0)
	{
		ylstate = PAR_STATE;
		EXIT;
		return (SET);
	}
	else if (strcmp(s, "unset") == 0)
	{
		ylstate = PAR_STATE;
		EXIT;
		return (UNSET);
	}
	else if (strcmp(s, "cd") == 0)
	{
		ylstate = PAR_STATE;
		EXIT;
		return (CHDIR);
	}
	else if (strcmp(s, "help") == 0)
	{
		ylstate = PAR_STATE;
		EXIT;
		return (HELP);
	}
	else
	{ /*other non built in command*/
		ylstate = PAR_STATE;
		yylval.sval = s;
		EXIT;
		return (COMMAND);
	}
	EXIT;
	return (-1);
}

/************************************************************************
 *									*
 *	read_until( str ) - This routine reads characters until a match	*
 *  is found with those in str.  It pushes this last character back on 	*
 *  the stream and returns a pointer to all those read so far.		*
 *  									*
 *  Original:	MEL	2/85						*
 *									*
 ************************************************************************/
char *read_until(str) char *str;
{
	register char *s;
	char *		   s0;
	int			   cnt;
	int			   len;
	char *		   list;

	len = 80;
	s0 = salloc(char, len);
	list = str;

	/*read into the string until we see a terminator*/
	for (s = s0; (*s = fetchc(in_file, TRUE)); s++)
	{
		if (index(list, *s) != 0)
			break;

		/*check length*/
		if (s - s0 == (len - 1))
		{
			len *= 2;
			cnt = s - s0;
			s0 = sralloc(char, len, s0);
			s = s0 + cnt;
		}
	}
	pushc(*s);
	*s = '\0';

	return (s0);
}

/************************************************************************
 *									*
 *	read_quote( str ) - This routine reads characters until a match	*
 *  is found with those in str.  It pushes this last character back on 	*
 *  the stream and returns a pointer to all those read so far.  It 	*
 *  handles quotes so that a quoted string gets treated in it's 	*
 *  complete form.							*
 *  									*
 *  Original:	MEL	11/86						*
 *									*
 ************************************************************************/
char *read_quote(str) char *str;
{
	register char *s;
	char *		   s0;
	int			   cnt;
	int			   len;
	char *		   list;
	char *		   tp;
	char		   quote;

	len = 80;
	s0 = salloc(char, len);
	list = str;

	/*read into the string until we see a terminator*/
	for (s = s0; (*s = fetchc(in_file, TRUE)); s++)
	{
		if (index(list, *s) != 0)
			break;

		/*if we find a quote, read until matching quote*/
		if ((*s == '"') || (*s == '\''))
		{
			tp = prompt;
			prompt = "> ";
			quote = *s;
			for (s++; (*s = fetchc(in_file, FALSE)); s++)
			{
				if ((*s == quote) || (*s == '\001'))
					break;
				/*check length*/
				if (s - s0 == (len - 1))
				{
					len *= 2;
					cnt = s - s0;
					s0 = sralloc(char, len, s0);
					s = s0 + cnt;
				}
			}
			if (*s != quote)
			{
				pushc(*s);
				s--;
			}
			prompt = tp;
		}

		/*check length*/
		if (s - s0 == (len - 1))
		{
			len *= 2;
			cnt = s - s0;
			s0 = sralloc(char, len, s0);
			s = s0 + cnt;
		}
	}
	pushc(*s);
	*s = '\0';
	return (s0);
}

/************************************************************************
 *									*
 *	do_echo() - This routine echos the command buffer if needed.	*
 *									*
 *  Original:	MEL	11/86						*
 *									*
 ************************************************************************/
void do_echo()
{

	if (*(echo_buffer + echo_bufptr - 1) != '\n')
		*(echo_buffer + echo_bufptr++) = '\n';

	if (echo_bufptr == echo_buflen)
	{
		echo_buflen = 2 * echo_buflen;
		echo_buffer = sralloc(char, echo_buflen, echo_buffer);
	}

	*(echo_buffer + echo_bufptr) = '\0';
	echo_bufptr = 0;

	if (echoall)
		fprintf(stdout, "%s", echo_buffer);
}
