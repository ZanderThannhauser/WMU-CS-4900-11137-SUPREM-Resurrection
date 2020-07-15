/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   cpu.c                Version 5.1     */

#include <math.h>
#include <stdio.h>
#include <time.h>

#include "suprem/include/global.h"
#include "suprem/include/shell.h"

/* 2020 includes:*/
#include "debug.h"
#include "cpu.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	cpu( par, param ) - this routine sets up a log file cpu 	*
 *  times to be stored.							*
 *									*
 *	Original :	Mark E. Law	Oct, 1984
 *									*
 ************************************************************************/
void cpu(char *par, struct par_str *param)
{
	char *f;
	int   on;
	ENTER;

	f = get_string(param, "cpufile");
	on = get_bool(param, "log");

	/*if we are turning logging on, set everything up*/
	if (on)
	{
		/*if no name given, usr stdout*/
		if (f == NULL)
			cpufile = stdout;
		else
		{
			if ((cpufile = fopen(f, "a")) == NULL)
			{
				fprintf(stderr, "error on open of cpu file %s\n", f);
				EXIT;
				return; /* (-1);*/
			}
#ifdef BSD
			setlinebuf(cpufile);
#endif
		}
		
		/* 2020: Lets not print numbers that will always be different into
		   2020: an output file that we need to be consistent! */
		#if !2020
		/*write a header into it*/
		fprintf(cpufile, "\n\nSUPREM IV cpu usage summary");

		i = time(NULL);
		f = (char *)ctime(&i);
		fprintf(cpufile, "\t%s\n", f);
		#endif
	}
	else
	{
		/*turn it all off*/
		if (cpufile != NULL)
			if (cpufile != stdout)
				fclose(cpufile);
		cpufile = NULL;
	}
	EXIT;
	return; /* (0);*/
}
