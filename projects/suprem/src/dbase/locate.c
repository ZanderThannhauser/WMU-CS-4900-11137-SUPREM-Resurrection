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
/*   locate.c                Version 5.1     */

#include <stdio.h>

#include "debug.h"

#include "suprem/include/constant.h"
#include "suprem/include/geom.h"
#include "suprem/include/global.h"

/* 2020 includes:*/
#include "locate.h"
/* end of includes*/

/*-----------------TWHICH, PWHICH---------------------------------------
 * Find which neighbor/node/pt of a triangle is the given neighbor/node/pt.
 * Would be macros were C perfect. Returns 3 if anything went wrong.
 *----------------------------------------------------------------------*/
int twhich(int in, int wanted)
{
	int j;
	for (j = 0; j < nedg; j++)
		if (tri[in]->nb[j] == wanted)
			break;
	return (j);
}

int nwhich(int in, int wanted)
{
	int j;
	for (j = 0; j < nvrt; j++)
		if (tri[in]->nd[j] == wanted)
			break;
	return (j);
}

int ewhich(int it, int wanted)
{
	int j;
	for (j = 0; j < num_edge(it); j++)
		if (edg_ele(it, j) == wanted)
			break;
	return (j);
}

/*-----------------NODE_MAT---------------------------------------------
 * Find the node under a particular node which is the right material.
 * -1 for bad.
 *----------------------------------------------------------------------*/
int node_mat(int in, int wanted)
{
	int ip, nx;
	ENTER;

	ip = pt_nd(in);

	for (nx = 0; nx < num_nd(ip); nx++)
		if (mat_nd(nd_pt(ip, nx)) == wanted)
		{
			EXIT;
			return (nd_pt(ip, nx));
		}

	EXIT;
	return (-1);
}
