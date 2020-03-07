/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   species.c                Version 5.1     */
/*   Last Modification : 7/3/91  10:50:21 */

#include <stdio.h>

#include "./src/include/constant.h"
#include "./src/include/diffuse.h"
#include "./src/include/geom.h"
#include "./src/include/global.h"
#include "./src/include/impurity.h"
#include "./src/include/matrix.h"

// 2020 includes:
#include "species.h"
// end of includes

// 2020 forward declarations
// end of declarations

/************************************************************************
 *									*
 *	This file contains definitions of routines which are used in	*
 * the specie dependent routines.  They are mostly data base access 	*
 * routines written so that a user may modify the model routines, and 	*
 * not have to understand the structure of the data base.		*
 *									*
 ************************************************************************/

/************************************************************************
 *									*
 *	clear_row(n, imp) - This zeroes the row for the node n.  	*
 *  It is useful for fixed boundary conditions.				*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void clear_row(int n, int sol) {
    register int j, k;

    for (j = 0; j < n_imp; j++) {

        switch (blktype[sol][j]) {

        case B_NONE:
            break;

        case B_DIAG:
            a[sol][j][n] = 0.0;
            for (k = irow_d[n]; k < irow_d[n + 1]; k++)
                a[sol][j][jrow_d[k]] = 0.0;
            break;

        case B_TRI:
        case B_BLCK:
            a[sol][j][n] = 0.0;
            for (k = irow[n]; k < irow[n + 1]; k++)
                a[sol][j][jrow[k]] = 0.0;
            break;
        }
    }

    newb[sol][n] = 0.0;
}
