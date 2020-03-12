/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   limits.c                Version 5.1     */
/*   Last Modification : 7/3/91  08:34:04 */

#include <stdio.h>

#include "./src/suprem/include/constant.h"
#include "./src/suprem/include/geom.h"
#include "./src/suprem/include/global.h"

/* 2020 includes:*/
#include "limits.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	dev_lmts(dxmin, dxmax, dymin, dymax) - this routine calculates  *
 *  the limits of the device.						*
 *									*
 *    Original : CSR           Stanford University        Nov, 1983 	*
 *    Revision : MEL	       (C translation)		  Oct, 1984	*
 *									*
 ************************************************************************/
void dev_lmts(float *dxmin, float *dxmax, float *dymin, float *dymax) {
    register float *val;
    register struct pt_str **p;

    /*initialize max and mins to first values**/
    *dxmin = *dxmax = pt[0]->cord[0];
    *dymin = *dymax = pt[0]->cord[1];

    /*loop through all the points*/
    for (p = pt; *p != NULL; p++) {
        val = p[0]->cord;
        *dxmin = (*dxmin < *val) ? *dxmin : *val;
        *dxmax = (*dxmax > *val) ? *dxmax : *val;
        val++; /*advance to y coordinate*/
        *dymin = (*dymin < *val) ? *dymin : *val;
        *dymax = (*dymax > *val) ? *dymax : *val;
    }
}
