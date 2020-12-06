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

#include <stdio.h>
#include <assert.h>

#include "suprem/include/constant.h"
#include "suprem/include/dbaccess.h"
#include "suprem/include/global.h"
#include "suprem/include/skel.h"
#include "suprem/include/plot.h"

/* 2020 includes:*/
#include <debug.h>
#include "../xsupr4/interface.h"
#include "plot.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

#define XC(A)	cordinate( A, 0 )
#define YC(A)	cordinate( A, 1 )

void node_pl(int i)
{
	#if 0
    char str[256];

    sprintf(str, "%d", i);
    xgLabel(str, XC(pt_nd(i)), YC(pt_nd(i)));
    if ( pl_debug ) {xgUpdate(FALSE); check_x();}
    #endif
    TODO;
}

void edge_pl(int i)
{
    xgNewGroup();
    xgPoint( XC( pt_edg(i,0) ), YC(pt_edg(i,0)) );
    xgPoint( XC( pt_edg(i,1) ), YC(pt_edg(i,1)) );
    if ( pl_debug ) {xgUpdate(FALSE); check_x();}
}

void reg_pl (int i)
{
	ENTER;
    struct LLedge *f, *b;
    int temp;

    temp = pl_debug;
    pl_debug = FALSE;
    WALK (sreg[i]->bnd, f, b) {
		edge_pl (f->edge);
		if (temp) {
			node_pl( nd_edg(f->edge,0) );
			node_pl( nd_edg(f->edge,1) );
		}
    }
    pl_debug = temp;
    if ( pl_debug ) {xgUpdate(FALSE); check_x();}
    EXIT;
}

void surf_pl(int color)
{
	#if 0
	int i;

	for (i = 0; i < ned; i++)
	{
		if (is_surf(pt_edg(i, 0)) && is_surf(pt_edg(i, 1)))
			edge_pl(i);
	}
	#endif
	TODO;
}

void xgdebug()
{
	#if 0
	pl_debug=1;
	xgNewSet();
	xgSetName("debug");
	#endif
	TODO;
}















