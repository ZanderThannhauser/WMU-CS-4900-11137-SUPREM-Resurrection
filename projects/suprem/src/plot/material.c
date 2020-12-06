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
/*   material.c                Version 5.1     */

#include <stdio.h>

#include "suprem/include/global.h"
#include "suprem/include/constant.h"
#include "suprem/include/dbaccess.h"

/* 2020 includes:*/
#include <debug.h>
#include "../xsupr4/interface.h"
#include "../refine/skel.h"
#include "../plot/plot.h"
#include "../refine/triang.h"
#include "material.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/*define a macro to get the material code for the material*/
#define XCORD(A, B) (pt[nd[tri[A]->nd[B]]->pt]->cord[0])
#define YCORD(A, B) (pt[nd[tri[A]->nd[B]]->pt]->cord[1])

/************************************************************************
 *									*
 *	material() - This routine sorts through all the triangles and   *
 *  draws any sides that are material boundaries.			*
 *									*
 ************************************************************************/
void material(int line_bound)
{
	ENTER;
	#ifdef LINUX
	register int ir;	/*current triangle*/
	register int sr;

	xgNewSet();
	xgSetName("Materials");
	for (ir = 0; ir < nreg; ir++) {
		sr = skel_reg(ir);
		reg_pl(sr);
		free_skel(sr);
	}
	#endif
	EXIT;
}



















