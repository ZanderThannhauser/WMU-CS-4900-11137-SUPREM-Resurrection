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
/*   Last Modification : 7/3/91 08:38:56 */

#include <stdio.h>

#include "./src/suprem/include/global.h"
#include "./src/suprem/include/constant.h"
#include "./src/suprem/include/dbaccess.h"

// 2020 includes:
#include "material.h"
// end of includes

// 2020 forward declarations
// end of declarations

/*define a macro to get the material code for the material*/
#define XCORD(A, B) (pt[nd[tri[A]->nd[B]]->pt]->cord[0])
#define YCORD(A, B) (pt[nd[tri[A]->nd[B]]->pt]->cord[1])

/************************************************************************
 *									*
 *	material() - This routine sorts through all the triangles and   *
 *  draws any sides that are material boundaries.			*
 *									*
 ************************************************************************/
void material(int line_bound) {}
