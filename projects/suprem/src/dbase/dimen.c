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
/*   dimen.c                Version 5.1     */

#include "suprem/include/constant.h"
#include "suprem/include/geom.h"
#include "suprem/include/global.h"
#include "suprem/include/impurity.h"

/* 2020 includes:*/
#include "../misc/panic.h"
#include "dimen.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	set_dim( dim ) - This routine initiailizes the dimensionality	*
 *									*
 *  Original:	MEL	10/84						*
 *									*
 ************************************************************************/
void set_dim(int dim)
{
	switch (dim)
	{
		case 1:
			mode = ONED;
			nvrt = 2;
			nedg = 2;
			oxide_model = O_VERTICAL;
			break;
		case 2:
			mode = TWOD;
			nvrt = 3;
			nedg = 3;
			break;
		default:
			panic("three dimensions are net yet supported");
	}
}
