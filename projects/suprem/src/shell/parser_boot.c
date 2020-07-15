/*************************************************************************
 *									 *
 *   Original : CSR         Stanford University        Jan, 1987 	 *
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   parser_boot.c                Version 5.1     */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suprem/include/check.h"
#include "suprem/include/constant.h"
#include "suprem/include/dbaccess.h"
#include "suprem/include/expr.h"
#include "suprem/include/global.h"
#include "suprem/include/key.h"
#include "suprem/include/shell.h"
#include "suprem/include/sysdep.h"

/* 2020 includes:*/
#include "suprem/src/misc/read.h"
#include "parser_boot.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	parser_boot - do all the bits and pieces needed to get the      *
 *			parser going					*
 *	Orignal: CSR Thu Jan 15 17:07:46 PST 1987			*
 ************************************************************************/
void parser_boot(char *Pkeyfile, char *Pprompt)
{
	int i;

	/*set up the input buffer*/
	buflen = 160;
	buffer = scalloc(char, buflen);
	bufptr = 0;
	echo_buflen = 160;
	echo_buffer = scalloc(char, echo_buflen);
	echo_bufptr = 0;
	supbln = 160;
	supbuf = scalloc(char, echo_buflen);
	supbpt = 0;

	/*initialize shell variables*/
	interactive = FALSE;
	noexecute = FALSE;
	depth = -1;

	/*read the unformatted key file*/
	if (read_ukfile(Pkeyfile) == -1)
	{
		fprintf(stderr, "unable to read or open the unformatted key file %s\n",
				Pkeyfile);
		exit(-1);
	}

	/*set up the prompt*/
	prompt = salloc(char, strlen(Pprompt) + 1);
	strcpy(prompt, Pprompt);

	in_file = stdin;
	for (i = 0; i < 10; i++)
		store[i] = NULL;
}
