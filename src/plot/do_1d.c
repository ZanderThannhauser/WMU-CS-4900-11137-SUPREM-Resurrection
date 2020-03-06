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
/*   do_1d.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:38:48 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/constant.h"
#include "./include/geom.h"
#include "./include/global.h"
#include "./include/material.h"
#include "./include/plot.h"

// 2020 includes:
#include "./plot/intersect.h"
#include "./dbase/kill_node.h"
#include "do_1d.h"
// end of 2020 includes

// 2020 forward decalarations
int d_compar(struct d_str *f1, struct d_str *f2);
// end of decalarations

#define ABS(x) (((x) > 0.0) ? (x) : (-(x)))

/* A doubly linked list to represent the boundary */
typedef struct b_str {
    struct b_str *left, *right;
    int ie, j;
} b_typ;

/************************************************************************
 *									*
 *	do_1d( ptype, val, bound, out ) - This routine plots the one 	*
 * dimensional data on the already initialized screen.  It plots either	*
 * in ptype at value val.  bound is a flag for drawing material bounds	*
 * on the screen.  If out is non-NULL, then the data is printed instead	*
 *									*
 * Original:	MEL	1/85						*
 *									*
 ************************************************************************/
int do_1d(int ptype, float val, struct d_str data[], int mat1, int mat2,
          int byarc) {
    return -1; // 2020: Took out all this code
}

int d_compar(struct d_str *f1, struct d_str *f2) {
    if (f1->x > f2->x)
        return (1);
    else if (f1->x < f2->x)
        return (-1);
    else if (f1->mat > f2->mat)
        return (1);
    else if (f1->mat < f2->mat)
        return (-1);
    else
        return (0);
}

/*-----------------FindItf---------------------------------------------
 * Return an array of linked lists representing the interfaces between
 * two materials.
 * Orientation is such that leftmost node is first for mat1=Si, mat2=Ox.
 *----------------------------------------------------------------------*/
b_typ **FindItf(mat1, mat2) int mat1, mat2;
{
    int i, ns = 0, ie, j, Ms = 10;
    b_typ *ttt, **seeds, *AddItfEdge(), **touched;

    seeds = salloc(b_typ *, Ms);
    touched = salloc(b_typ *, 3 * ne);

    for (i = 0; i < 3 * ne; i++)
        touched[i] = 0;

    /* Find seed edges and form doubly linked lists*/
    for (ie = 0; ie < ne; ie++) {
        for (j = 0; j < 3; j++) {
            if (touched[3 * ie + j])
                continue;
            if ((ttt = AddItfEdge(ie, j, mat1, mat2, touched))) {
                for (seeds[ns] = ttt; seeds[ns]->left;
                     seeds[ns] = seeds[ns]->left)
                    ;
                if (ns++ >= Ms - 1) {
                    Ms *= 2;
                    seeds = sralloc(b_typ *, Ms, seeds);
                }
            }
        }
    }
    seeds[ns] = 0;
    free(touched);
    return (seeds);
}

/*-----------------AddItfEdge-----------------------------------------
 * Recursive routine to add one edge and its neighbors and
 * their neighbors and ...
 *----------------------------------------------------------------------*/
b_typ *AddItfEdge(ie, j, mat1, mat2, touched) int ie, j, mat1, mat2;
b_typ **touched;
{
    b_typ *new;
    int je, oje, k, kk, ib;

    /* Is this edge desired? */
    if (ie < 0 || mat_reg(reg_tri(ie)) != mat1)
        return (0);
    ib = tri[ie]->nb[j];
    if (((ib < 0) && (ib == mat2)) ||
        ((ib >= 0) && (mat_reg(reg_tri(ib)) == mat2))) {
        /* Yes, is it already there? */
        if (touched[3 * ie + j])
            return (touched[3 * ie + j]);

        /* Otherwise create it */
        new = salloc(b_typ, 1);
        new->ie = ie;
        new->j = j;
        touched[3 * ie + j] = new;

        /* Look clockwise, then anticlockwise */
        trotate(ie, (j + 2) % 3, 1, &je, &oje, &k, &kk);
        new->left = AddItfEdge(oje, 3 - k - kk, mat1, mat2, touched);
        trotate(ie, (j + 1) % 3, 0, &je, &oje, &k, &kk);
        new->right = AddItfEdge(oje, 3 - k - kk, mat1, mat2, touched);

        return (new);
    } else
        return (0);
}
