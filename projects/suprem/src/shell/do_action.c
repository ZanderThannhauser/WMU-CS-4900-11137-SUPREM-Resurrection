/*************************************************************************
 *									 *
 *   Original : MEL         Stanford University        Sept, 1984	 *
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *     Copyright c 1991 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of the University of 	 *
 *     Florida.								 *
 *									 *
 *************************************************************************/
/*   do_action.c                Version 5.1     */

#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef LINUX
#include <sys/wait.h>
#endif

#include "suprem/include/sysdep.h"

#include "suprem/include/check.h"
#include "suprem/include/global.h"
#include "suprem/include/shell.h"

/* 2020 includes:*/
#include "debug.h"
#include "yyparse.h"
#include "proc.h"
#include "../check/verify.h"
#include "../xsupr4/interface.h"
#include "do_action.h"
/* end of includes*/

/* 2020 forward declarations*/
void do_exec(char *par, int intr, int index, int no_exec);
/* end of declarations*/

/************************************************************************
 *									*
 *	do_source(file, redir, back) - this routine handles the 	*
 *  sourcing of an input file.  The file name is given in file, the	*
 *  redirect file name is in redir, and the if the job should be in the *
 *  background is indicted by back					*
 *									*
 *	Original	Mark E. Law		Sept, 1984		*
 *									*
 ************************************************************************/
void do_source(char *file,  /*the file to be sourced*/
			   char *redir, /*the filename for redirection, if any*/
			   int   back,  /*flag for backgorund execution*/
			   int   reperr)  /*report errors in the file open?*/
{
	FILE *tinf, *tsrc;
	FILE  tout, *t1;
	char *redirect;
	char *oldbuf;
	ENTER;

	verpv(file);
	verpv(redir);
	verpv(back);
	verpv(reperr);

	redirect = NULL;

	/*if we have not been given a file, save a lot of hassle*/
	if (file == NULL)
	{
		fprintf(stderr, "must specify a file to source\n");
		EXIT;
		return;
	}

	/*save the old input file pointer, set up the new*/
	tinf = in_file;
	tsrc = file_parse(file, "r");

	/*if a legal file get specified*/
	if (tsrc != NULL)
	{
		/*save the old output file pointer, set up the new*/
		tout = *stdout;
		if (redir != NULL)
		{
			t1 = file_parse(redir, "a");
			redirect = redir;
			*stdout = *t1;
		}

		/*save the old buffer away*/
		oldbuf = salloc(char, strlen(buffer + bufptr) + 1);
		strcpy(oldbuf, buffer + bufptr);

		/*null the input buffer*/
		bufptr = 0;
		buffer[bufptr] = '\0';

		/*if background job, handle it*/
		in_file = tsrc;
		if (back)
		{
			#ifdef LINUX
			if (fork() == 0)
			{
				/*save this location*/
				while (yyparse() != -1)
					;
				exit(0);
			}
			#endif
			
			#ifdef WINDOWS
			TODO;
			#endif
		}
		else
		{
			HERE;
			while (yyparse() != -1)
				;
			HERE;
		}
		fclose(tsrc);
		in_file = tinf;

		/*restore the buffer*/
		strcpy(buffer, oldbuf);
		bufptr = 0;
		sfree(oldbuf);
	}
	else if (reperr)
		fprintf(stderr, "Could not find file %s to open\n", file);

	/*clean up the output buffer*/
	if (redirect != NULL)
	{
		fclose(stdout);
		*stdout = tout;
		sfree(redirect);
	}
	EXIT;
	return;
}

/************************************************************************
 *									*
 *	do_string(str, rdir, back) - this routine handles the execution *
 *  of a string of input.  It is called by loops and bracket sets.	*
 *									*
 *	Original	Mark E. Law		Oct, 1986		*
 *									*
 ************************************************************************/
void do_string(char *instr, char *redir, int back)
{

	FILE  tout, *t1;
	int   len;
	char *redirect;
	char *oldbuf;

	ENTER;

	redirect = NULL;

	/*save the old output file pointer, set up the new*/
	tout = *stdout;
	if (redir != NULL)
	{
		t1 = file_parse(redir, "a");
		redirect = redir;
		*stdout = *t1;
	}

	/*if background job, handle it*/
	if (back)
	{
		#ifdef LINUX
		if (fork() == 0)
		{
			
			/*make sure there is enough space*/
			if ((len = (strlen(instr) + 3)) > supbln)
			{
				supbln = len;
				supbuf = sralloc(char, len * sizeof(int), supbuf);
			}
			
			/*copy the string int line size pieces into the buffer*/
			strcpy(supbuf, instr);
			strcat(supbuf, "\n\001");
			supbpt = 0;
			
			/*parse it*/
			while (yyparse() != -1)
				;
		}
		#endif
		
		#if WINDOWS
		TODO;
		#endif
	}
	else
	{
		/*save the old buffer away*/
		oldbuf = salloc(char, strlen(supbuf + supbpt) + 1);
		strcpy(oldbuf, supbuf + supbpt);

		/*make sure there is enough space*/
		if ((len = (strlen(instr) + 3)) > supbln)
		{
			supbln = len;
			supbuf = sralloc(char, len * sizeof(int), supbuf);
		}

		/*copy the string int line size pieces into the buffer*/
		strcpy(supbuf, instr);
		strcat(supbuf, "\n\001");
		supbpt = 0;

		/*parse it*/
		while (yyparse() != -1)
			;

		/*restore the buffer*/
		strcpy(supbuf, oldbuf);
		supbpt = 0;
		sfree(oldbuf);
	}

	/*clean up the output buffer*/
	if (redirect != NULL)
	{
		fclose(stdout);
		*stdout = tout;
		sfree(redirect);
	}

	EXIT;
}

/* for the perennially broken dbx */
void do_str(char *instr) { do_string(instr, NULL, 0); }

/************************************************************************
 *									*
 *	do_command(name, param, intr, file, back) - this procedure 	*
 *  handles the parsing of the data presented.  Name is the name of the *
 *  command to be parsed and executed.  Param is the remainder of the   *
 *  line which describes the parameters for the command.  Intr is a 	*
 *  flag which specifies if the command is to be done in interactive	*
 *  mode.  File is a filename for any possible redirection of output.	*
 *  Back is a flag for backgrounding the command.			*
 *									*
 *	Original	Mark E. Law		Sept, 1984		*
 *									*
 ************************************************************************/
void do_command(char *name, char *param, int intr, char *file, int back)
{
	FILE tout, *t1;
	int  index;
	
	ENTER;
	/*get proc parses the string associated with command*/
	index = get_proc(name);

	/*if command is -1, an illegal input was typed*/
	if (index != -1)
	{
		HERE;
		
		tout = *stdout;
		if (file != NULL)
		{
			t1 = file_parse(file, "a");
			*stdout = *t1;
		}

		if (back)
		{
			HERE;
			#ifdef LINUX
			/*background the process*/
			if (fork() == 0)
			{
				/*in the child, do the command*/
				do_exec(param, FALSE, index, noexecute);
				exit(0);
			}
			#endif
			
			#if WINDOWS
			TODO;
			#endif
		}
		else
			do_exec(param, intr, index, noexecute);

		if (file != NULL)
		{
			fclose(stdout);
			*stdout = tout;
		}
	}
	else
	{
		char *sh = (char *)getenv("SHELL");
		char *str;
		
		#if LINUX
		int pid, w;
		int status;
		#endif
		
		if (file != NULL)
			if (param != NULL)
				str = malloc(strlen(file) + strlen(name) + strlen(param) + 20);
			else
				str = malloc(strlen(file) + strlen(name) + 20);
		else if (param != NULL)
			str = malloc(strlen(name) + strlen(param) + 20);
		else
			str = malloc(strlen(name) + 20);
		
		if (sh == NULL)
			sh = "/bin/sh";
		
		strcpy(str, name);
		if (param != NULL)
			strcat(str, param);
		
		/*if a redirect add that on*/
		if (file != NULL)
		{
			strcat(str, ">");
			strcat(str, file);
		}
		
		/*add background on*/
		if (back)
			strcat(str, "&");
		
		HERE;
		#ifdef LINUX
		/*exec a shell to handle the request*/
		if ((pid = vfork()) == 0)
		{
			if (execl(sh, sh, "-c", str, NULL) == -1)
				printf("error number %d\n", 127);
			_exit(127);
		}

		/*wait for the process to finish*/
		while ((w = wait(&status)) != pid && w != -1)
			;
		#endif
		
		#if WINDOWS
		TODO;
		#endif
	}
	
	EXIT;
}

/************************************************************************
 *									*
 *	do_exec(par, intr, index, noexec ) -  This routine does 	*
 *  parameter checks and looks at the noexec and intr flags to do any	*
 *  preprocessing that may be needed.   If all goes well,  it calls 	*
 *  the appropriate routine to do the action.  				*
 *									*
 *  Original:	MEL	3/85	(gearing up for Pisces2)		*
 *									*
 ************************************************************************/
void do_exec(char *par, int intr, int index, int no_exec)
{
	ENTER;
	
	check_x();
	
	if (check(par, cards[command[index].param]) == -1)
	{
		fprintf(stderr, "errors detected on command input\n");
		EXIT;
		return;
	}
	else if (no_exec)
		fprintf(stderr, "no error in %s command input\n", command[index].name);

	if (intr)
		printf("invoked interactively\n");

	/*if not in no exec mode, do the work*/
	if (!no_exec)
	{
		verpv(index);
		verpv(command[index].func);
		command[index].func(par, cards[command[index].param]);
	}
	EXIT;
	return;
}



