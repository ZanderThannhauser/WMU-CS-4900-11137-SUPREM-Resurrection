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
/*   plot_1d.c                Version 5.1     */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "suprem/include/constant.h"
#include "suprem/include/geom.h"
#include "suprem/include/global.h"
#include "suprem/include/material.h"
#include "suprem/include/plot.h"
#include "suprem/include/plot.h"

/* 2020 includes:*/
#include "debug.h"
#include "../misc/get.h"
#include "../dbase/dispose.h"
#include "../oxide/mater.h"
#include "../plot/do_1d.h"
#include "../geom/limits.h"
#include "plot_1d.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of decalarations*/

/************************************************************************
 *									*
 *	plot_1d( par, param ) - this routine is the father of the 	*
 *  plot_1d software.  It calls appropriate routines to set up 		*
 *  and do plots.							*
 *									*
 *  Original:	MEL	1/85	(modeled on pisces2)			*
 *									*
 ************************************************************************/
void plot_1d(char *par, struct par_str *param)
{
	ENTER;
	/* 2020: took all this out*/
	EXIT;
}

/*----------------------------------------------------------------------
 * Fix equal or swapped bounds.
 *----------------------------------------------------------------------*/
void CheckBound(float *lo, float *hi)
{
	float swap;

	if (*lo == *hi)
	{
		if (*lo == 0)
		{
			*lo = -1.0;
			*hi = 1.0;
		}
		else
		{
			*lo *= 0.9;
			*hi *= 1.1;
		}
	}

	if (*lo > *hi)
	{
		swap = *lo;
		*lo = *hi;
		*hi = swap;
	}
}
