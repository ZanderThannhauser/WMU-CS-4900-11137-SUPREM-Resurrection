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
/*   Last Modification : 7/3/91 08:39:01 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "./src/include/constant.h"
#include "./src/include/geom.h"
#include "./src/include/global.h"
#include "./src/include/material.h"
#include "./src/include/plot.h"

// 2020 includes:
#include "./src/debug.h"
#include "./src/misc/get.h"
#include "./src/dbase/dispose.h"
#include "./src/oxide/mater.h"
#include "./src/plot/do_1d.h"
#include "./src/geom/limits.h"
#include "./src/include/plot.h"
#include "plot_1d.h"
// end of includes

// 2020 forward declarations
// end of decalarations

/************************************************************************
 *									*
 *	plot_1d( par, param ) - this routine is the father of the 	*
 *  plot_1d software.  It calls appropriate routines to set up 		*
 *  and do plots.							*
 *									*
 *  Original:	MEL	1/85	(modeled on pisces2)			*
 *									*
 ************************************************************************/
void plot_1d(char *par, struct par_str *param) {
	// 2020: took all this out
}

/*----------------------------------------------------------------------
 * Fix equal or swapped bounds.
 *----------------------------------------------------------------------*/
void CheckBound(float *lo, float *hi) {
    float swap;

    if (*lo == *hi) {
        if (*lo == 0) {
            *lo = -1.0;
            *hi = 1.0;
        } else {
            *lo *= 0.9;
            *hi *= 1.1;
        }
    }

    if (*lo > *hi) {
        swap = *lo;
        *lo = *hi;
        *hi = swap;
    }
}
