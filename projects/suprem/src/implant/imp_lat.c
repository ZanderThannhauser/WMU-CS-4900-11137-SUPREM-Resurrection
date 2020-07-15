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
/*   imp_lat.c                Version 5.1     */

#include <math.h>
#include <stdio.h>

#include "suprem/include/constant.h"
#include "suprem/include/global.h"
#include "suprem/include/implant.h"

/* 2020 includes:*/
#include "imp_lat.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	imp_latr( pos, left, right, data ) - This routine calculates 	*
 *  the lateral distribution function for lateral standard deviation 	*
 *  side and distance from the right and left sides of the slice.	*
 *									*
 *  Original:	MEL	1/87						*
 *									*
 ************************************************************************/
double imp_latr(double pos, struct imp_info *data)
{
	double val;

	val = exp(-(pos * pos) / (2.0 * data->lat[LATSIG] * data->lat[LATSIG]));
	val /= data->lat[LATAREA] * 1.0e-4;
	return (val);
}
