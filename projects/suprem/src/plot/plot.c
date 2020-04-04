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
/*   plot.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:38:59 */

#include <stdio.h>

#include "suprem/include/constant.h"
#include "suprem/include/dbaccess.h"
#include "suprem/include/global.h"
#include "suprem/include/skel.h"
#include "suprem/include/plot.h"

/* 2020 includes:*/
#include "plot.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

#define XC(A) cordinate(A, 0)
#define YC(A) cordinate(A, 1)

void node_pl(int x, int y) {}

void edge_pl(int i) {}

void reg_pl(int i) {}

void surf_pl(int color)
{
	int i;

	for (i = 0; i < ned; i++)
	{
		if (is_surf(pt_edg(i, 0)) && is_surf(pt_edg(i, 1)))
			edge_pl(i);
	}
}

void xgdebug() {}
