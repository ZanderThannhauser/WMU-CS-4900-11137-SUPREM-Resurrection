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
/*   plot_2d.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:39:03 */

#include <math.h>
#include <stdio.h>

#include "suprem/include/constant.h"
#include "suprem/include/geom.h"
#include "suprem/include/global.h"
#include "suprem/include/impurity.h"
#include "suprem/include/material.h"
#include "suprem/include/plot.h"
#include "suprem/include/regrid.h"
#include "suprem/include/plot.h"

/* 2020 includes:*/
#include "debug.h"
#include "../misc/get.h"
#include "../dbase/dispose.h"
#include "../geom/limits.h"
#include "../plot/plot.h"
#include "../plot/material.h"
#include "plot_2d.h"
/* end of includes*/

#define XCORD(A) pt[A]->cord[0]
#define YCORD(A) pt[A]->cord[1]

void draw_vornoi();
void draw_points(float);
void draw_stress(float vleng, float smax, int ccol, int tcol);
void draw_flow(float vleng, float vmax, int col);

/************************************************************************
 *									*
 *	plot_2d(par, param ) - this routine is the father of the 	*
 *  plot_2d software.  It calls appropriate routines to set up 		*
 *  and do plots.							*
 *									*
 *  Original :	MEL, CSR, MRP	11, 1984 (modeled on pisces2)		*
 *									*
 ************************************************************************/
void plot_2d(char *par, struct par_str *param)
{
	int			 i;
	int			 boundary;
	int			 grid;
	int			 fill;
	static int   line_bound = 1, line_com = 3, line_ten = 4;
	int			 vornoi, diamonds;
	int			 stress;
	static float txmin, txmax, tymin, tymax;
	float		 vleng, vmax;
	int			 deb;
	ENTER;

	if (InvalidMeshCheck())
	{
		EXIT;
		return; /* (-1);*/
	}

	deb = pl_debug;
	pl_debug = FALSE;

	if (mode == ONED)
	{
		fprintf(stderr, "plot.2d is illegal in one dimension\n");
		EXIT;
		return; /* (-1);*/
	}

	/*get the card parameters*/
	boundary = get_bool(param, "boundary");
	grid = get_bool(param, "grid");
	vornoi = get_int(param, "vornoi");
	diamonds = get_bool(param, "diamonds");
	stress = get_bool(param, "stress");
	fill = get_bool(param, "fill");
	if (is_specified(param, "line.bound"))
		line_bound = get_int(param, "line.bound");
	if (is_specified(param, "line.com"))
		line_com = get_int(param, "line.com");
	if (is_specified(param, "line.ten"))
		line_ten = get_int(param, "line.ten");
	if (is_specified(param, "vmax"))
		vmax = get_float(param, "vmax");
	if (is_specified(param, "vleng"))
		vleng = 1e-4 * get_float(param, "vleng");
	else if (stress)
	{
		fprintf(stderr, "A vector length must be specified!\n");
		stress = 0;
	}

	dev_lmts(&txmin, &txmax, &tymin, &tymax);

	if (is_specified(param, "x.min"))
		txmin = get_float(param, "x.min") * 1e-4;
	if (is_specified(param, "x.max"))
		txmax = get_float(param, "x.max") * 1e-4;
	if (is_specified(param, "y.min"))
		tymin = get_float(param, "y.min") * 1e-4;
	if (is_specified(param, "y.max"))
		tymax = get_float(param, "y.max") * 1e-4;

	/* have a heart */
	if (txmin > txmax)
	{
		float swap = txmin;
		txmin = txmax;
		txmax = swap;
	}
	if (tymin > tymax)
	{
		float swap = tymin;
		tymin = tymax;
		tymax = swap;
	}

	if (!fill)
	{

		/* Center the requested area in the window */

		float dx = txmax - txmin, dy = tymax - tymin, cenwin;
		if (dx > dy)
		{
			cenwin = 0.5 * (tymin + tymax);
			tymax = cenwin + 0.5 * dx;
			tymin = cenwin - 0.5 * dx;
		}
		else
		{
			cenwin = 0.5 * (txmin + txmax);
			txmax = cenwin + 0.5 * dy;
			txmin = cenwin - 0.5 * dy;
		}
	}

	/* Check the results to avoid disaster */
	if (txmin >= txmax || tymin >= tymax)
	{
		fprintf(stderr, "bad bounds in plotting: x(%f:%f) y(%f:%f)\n", txmin,
				txmax, tymin, tymax);
		EXIT;
		return; /* (-1);*/
	}

	yflip = TRUE;

	if (grid)
	{
		for (i = 0; i < ned; i++)
			edge_pl(i);
	}

	if (vornoi)
	{
		draw_vornoi();
	}

	if (boundary)
	{
		material(line_bound);
	}

	if (diamonds)
	{
		draw_points(0.005 * (xmax - xmin + ymax - ymin));
	}

	if (stress)
		draw_stress(vleng, vmax, line_com, line_ten);

	if (get_bool(param, "flow"))
		draw_flow(vleng, vmax, line_com);

	/*clean up plotting and post it out*/
	pl_debug = deb;

	EXIT;
	return; /* (0);*/
}

void draw_vornoi() {}

void draw_points(float delt) {}

/*-----------------DRAW_STRESS------------------------------------------
 *----------------------------------------------------------------------*/
void draw_stress(float vleng, float smax, int ccol, int tcol) {}

/*-----------------DRAW_FLOW--------------------------------------------
 * Little arrows flowing in the stream...
 *----------------------------------------------------------------------*/
void draw_flow(float vleng, float vmax, int col) {}
