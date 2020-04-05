/*************************************************************************
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
 *									 *
 *************************************************************************/
/*   select.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:39:07 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suprem/include/constant.h"
#include "suprem/include/dbaccess.h"
#include "suprem/include/expr.h"
#include "suprem/include/global.h"
#include "suprem/include/material.h"
#include "suprem/include/plot.h"
#include "suprem/include/sysdep.h"

/* 2020 includes:*/
#include "debug.h"
#include "../oxide/mater.h"
#include "../check/parse.h"
#include "../check/eval.h"
#include "select.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	sel_var( par, param ) - this routine sets up a solution vector 	*
 *  for the plot commands that need one.				*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void sel_var(char *par, struct par_str *param)
{
	char *			zstr;
	struct vec_str *zexp;
	char *			err;
	char *			tmp;
	ENTER;

	zstr = get_string(param, "z");
	znn = nn;
	z = salloc(float, nn);

	if (is_specified(param, "label"))
	{
		if (label != NULL)
			free(label);
		tmp = get_string(param, "label");
		label = (char *)malloc(strlen(tmp) + 1);
		strcpy(label, tmp);
	}
	else
	{
		if (label != NULL)
			free(label);
		label = (char *)malloc(strlen(zstr) + 1);
		strcpy(label, zstr);
	}

	if (is_specified(param, "title"))
	{
		if (title != NULL)
			free(title);
		tmp = get_string(param, "title");
		title =
			(char *)malloc(strlen(tmp) + strlen(VERSION) + strlen(" - ") + 1);
		strcpy(title, tmp);
		strcat(title, " - ");
		strcat(title, VERSION);
	}
	else
	{
		if (title != NULL)
			free(title);
		title = (char *)malloc(strlen(VERSION) + 1);
		strcpy(title, VERSION);
	}

	if (is_specified(param, "temp"))
	{
		last_temp = get_float(param, "temp") + 273.0;
		/*compute the sislicon material constants*/
		comp_mat(last_temp);
	}

	/*attempt to parse the string*/
	if ((err = (char *)parse_expr(zstr, &zexp)) != NULL)
	{
		fprintf(stderr, "%s\n", err);
		EXIT;
		return; /*(-1);*/
	}

	/*evaluate the vector expression*/
	if ((err = (char *)eval_vec(zexp, z)) != NULL)
	{
		fprintf(stderr, "%s\n", err);
		EXIT;
		return; /*(-1);*/
	}

	sel_log = islogexp(zexp);

	free_expr(zexp);
	
	EXIT;
	return; /*(0);*/
}
