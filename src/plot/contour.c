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

#include "./src/include/constant.h"
#include "./src/include/geom.h"
#include "./src/include/global.h"
#include "./src/include/material.h"
#include "./src/include/plot.h"

// 2020 includes
#include "./src/dbase/dispose.h"
#include "./src/misc/get.h"
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
void contour(char *par, struct par_str *param) { ; }
