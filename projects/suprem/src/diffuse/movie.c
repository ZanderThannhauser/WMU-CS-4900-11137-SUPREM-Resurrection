/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   movie.c                Version 5.1     */

#include <math.h>
#include <stdio.h>

#include "suprem/include/global.h"
#include "suprem/include/shell.h"

/* 2020 includes:*/
#include "../shell/macro.h"
#include "../shell/do_action.h"
#include "movie.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	make_movie( string, time ) - This routine executes the commands *
 *  in the string passed.  The time elapses is in time.			*
 *									*
 *  Original:	MEL	11/86						*
 *									*
 ************************************************************************/
void make_movie(char *commands, float time)
{
	char buffer[512];

	if (commands == NULL)
		return;

	/*define a macro string for the current time*/
	if (time < 1.0)
	{
		sprintf(buffer, "tlab %7.3fms", time * 1000.0);
		define_macro(buffer, &macro);
	}
	else if (time < 60.0)
	{
		sprintf(buffer, "tlab %7.3fs", time);
		define_macro(buffer, &macro);
	}
	else if (time < 3600.0)
	{
		sprintf(buffer, "tlab %7.3fm", time / 60.0);
		define_macro(buffer, &macro);
	}
	else
	{
		sprintf(buffer, "tlab %7.3fh", time / 3600.0);
		define_macro(buffer, &macro);
	}

	sprintf(buffer, "time %g", time);
	define_macro(buffer, &macro);

	/*make it fancy later with the time, but right now just execute*/
	do_string(commands, NULL, 0);

	/*undo the macro*/
	(void)undef_macro("time", &macro);
	(void)undef_macro("tlab", &macro);
}
