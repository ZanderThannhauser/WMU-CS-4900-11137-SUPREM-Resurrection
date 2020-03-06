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

#include "./include/constant.h"
#include "./include/dbaccess.h"
#include "./include/expr.h"
#include "./include/global.h"
#include "./include/material.h"
#include "./include/plot.h"
#include "./include/sysdep.h"

// 2020 includes:
#include "select.h"
// end of includes

// 2020 forward declarations
// end of declarations

/************************************************************************
 *									*
 *	sel_var( par, param ) - this routine sets up a solution vector 	*
 *  for the plot commands that need one.				*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void sel_var(char *par, struct par_str* param) {}
