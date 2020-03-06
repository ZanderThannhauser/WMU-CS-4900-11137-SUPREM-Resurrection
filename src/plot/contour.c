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
/*   contour.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:38:46 */

#include <math.h>
#include <stdio.h>
#ifdef CONVEX
#include <strings.h>
#else
#include <string.h>
#endif

#include "./include/constant.h"
#include "./include/geom.h"
#include "./include/global.h"
#include "./include/material.h"
#include "./include/plot.h"

// 2020 includes
#include "./dbase/dispose.h"
#include "./misc/get.h"
#include "contour.h"
// end of includes

/************************************************************************
 *									*
 *	contour( par, param ) - This routine draws contour lines for 	*
 *  in the selected variable.						*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void contour(char *par, struct par_str* param) { ; }
