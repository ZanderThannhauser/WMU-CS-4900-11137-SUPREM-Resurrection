
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
/*   main.c                Version 4.1     */
/*   Last Modification : 8/18/89  16:35:41 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STATIC_ALLOCATION_TIME
#include "suprem/include/check.h"
#include "suprem/include/key.h"
#include "suprem/include/sysdep.h"

/* 2020 includes:*/
#include "./parser.h"
#include "./expand.h"
#include "./write.h"
#include "main.h"
/* end of includes*/

#ifdef DEBUGGING_2020
int debugging_depth;
#endif

/* 2020 forward declarations*/
/* end of declarations*/

struct par_str **current;
struct par_str **stack[10];
int				 depth;
int				 cardnum;
char			 depthstr[20];

/************************************************************************
 *									*
 *	push(par) - push par on the stack and return a pointer to pars  *
 * sub parameters.							*
 *									*
 *	Original	Mark E. Law		Oct, 1984		*
 *									*
 ************************************************************************/
struct par_str **push(struct par_str **par)
{
	int i;

	stack[depth++] = par;

	/*we really want to set this up at the previous command*/
	par--;
	par[0]->param =
		(struct par_str **)malloc((NUMPAR + 1) * sizeof(struct par_str *));

	/*malloc off the space for the sub params*/
	for (i = 0; i < NUMPAR; i++)
		par[0]->param[i] = (struct par_str *)calloc(1, sizeof(struct par_str));
	par[0]->param[NUMPAR] = NULL;

	return ((par[0]->param));
}

/************************************************************************
 *									*
 *	pop() - this routine pops off a stack level			*
 *									*
 *	Original	Mark E. Law		Oct, 1984		*
 *									*
 ************************************************************************/
struct par_str **pop()
{
	depth--;
	return (stack[depth]);
}

int	main(argc, argv) int argc;
char **argv;
{
	int i;
	int fd;

	/*malloc off the space for the sub params*/
	for (i = 0; i < NUMPAR; i++)
		cards[i] = (struct par_str *)calloc(1, sizeof(struct par_str));
	/* cards[NUMPAR] = NULL; // 2020: 'array subscript is above array bounds'*/
	current = cards;
	stack[depth++] = NULL;
	yyparse();

	/*make the parameter value of all params = the depth string used in bools*/
	depth = 0;
	depthstr[0] = '\001';
	depthstr[1] = '\000';
	make_depth(cards);

	/*now expand the parameter references in the boolean expressions*/
	depth = 0;
	cardnum = 0;
	expand(cards);

	/*open a file, test it, and then write the unformatted key file*/
	if (argc >= 2)
	{
		/*open the file and try to write it*/
		fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd == -1)
		{
			fprintf(stderr, "unable to open unformatted keyfile %s\n", argv[1]);
			exit(-1);
		}
		depth = 0;
		if (write_list(cards, fd) == -1)
			fprintf(stderr, "unsuccessful write of unformatted key file\n");
	}
	else
	{
		/*try to open a default*/
		fd = open("suprem.uk", O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd == -1)
		{
			fprintf(stderr, "unable to open unformatted keyfile %s\n", argv[1]);
			exit(-1);
		}
		depth = 0;
		if (write_list(cards, fd) == -1)
			fprintf(stderr, "unsuccessful write of unformatted key file\n");
	}

	return (0);
}
