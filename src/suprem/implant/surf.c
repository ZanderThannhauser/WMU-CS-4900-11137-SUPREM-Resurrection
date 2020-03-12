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
/*   surf.c                Version 2.1     */
/*   Last Modification : 2/3/93 15:10:37 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "./src/suprem/include/constant.h"
#include "./src/suprem/include/geom.h"
#include "./src/suprem/include/global.h"
#include "./src/suprem/include/implant.h"
#include "./src/suprem/include/material.h"
#include "./src/suprem/include/refine.h"
#include "./src/suprem/include/sysdep.h"

/* 2020 includes:*/
#include "./src/debug.h"
#include "./src/suprem/refine/surface.h"
#include "./src/suprem/plot/matedge.h"
#include "./src/suprem/geom/misc.h"
#include "surf.h"
/* end of includes*/

/* 2020 forward declarations*/
void get_mat(struct surf_info *cur, int angle);
/* end of declarations*/

double tr[MAXDIM][MAXDIM];

#define XTRAN(x, y)                                                            \
    (mode == ONED) ? (x / tr[1][1]) : (x * tr[0][0] + y * tr[0][1])
#define YTRAN(x, y) (mode == ONED) ? (0.0) : (x * tr[1][0] + y * tr[1][1])

/************************************************************************
 *									*
 *	make_surf() - This routine finds and orders a surface list	*
 *  for use in implantation.						*
 *									*
 *  Original:	MEL	12/86						*
 *									*
 ************************************************************************/
void make_surf(struct surf_info **v, double ang, double lat) {
    register int i, j; /*every routine needs these for indexing*/
    struct line *s;    /*surface list*/
    int ns;            /*and number thereof */
    struct surf_info *cur, *prev, lft, rht;
    float x;
    float atop;
    int id;
    ENTER;

    /*build the translation matrix*/
    tr[0][0] = cos(ang);
    tr[0][1] = sin(ang);
    tr[0][2] = 0.0;
    tr[1][0] = -sin(ang);
    tr[1][1] = cos(ang);
    tr[1][2] = 0.0;
    tr[2][0] = 0.0;
    tr[2][1] = 0.0;
    tr[2][2] = 1.0;

    /* generate a surface description */
    ns = find_surf(&s);

    /*before angular rotation, get the materials at left and right*/
    lft.rht[0] = lft.lft[0] = pt[s[0].p]->cord[0] + ERR;

    get_mat(&lft, TRUE);

    HERE;
    verpv(lft.top[0]);
    HERE;

    rht.lft[0] = rht.rht[0] = pt[s[ns - 1].p]->cord[0] - ERR;
    get_mat(&rht, TRUE);

    HERE;

    /*translate the mesh onto the new angle coordinates*/
    for (i = 0; i < np; i++) {

        HERE;

        for (j = 0; j < mode; j++) {
            pt[i]->cordo[j] = pt[i]->cord[j];
        }
        pt[i]->cord[0] = XTRAN(pt[i]->cordo[0], pt[i]->cordo[1]);
        pt[i]->cord[1] = YTRAN(pt[i]->cordo[0], pt[i]->cordo[1]);
    }

    if (mode == ONED) {

        HERE;

        cur = scalloc(struct surf_info, 1);
        *v = cur;
        cur->next = NULL;
        get_mat(cur, FALSE);
        return;
    }

    if (mode != ONED) {

        HERE;

        /*copy the line into the surface info structure*/
        cur = scalloc(struct surf_info, 1);
        *v = cur;
        prev = cur;
        x = pdist(s[0].p, s[1].p);
        for (i = 0; i < mode; i++) {
            cur->rht[i] = pt[s[0].p]->cord[i];
            cur->lft[i] =
                lat * (pt[s[0].p]->cord[i] - pt[s[1].p]->cord[i]) / x +
                pt[s[0].p]->cord[i];
            HERE;
            verpv(cur->lft[i]);
        }
    }

    /*build the list*/
    for (i = 1; i < ns; i++) {

        HERE;

        cur = scalloc(struct surf_info, 1);

        for (j = 0; j < mode; j++) {
            cur->lft[j] = pt[s[i - 1].p]->cord[j];
            cur->rht[j] = pt[s[i].p]->cord[j];
            HERE;
            verpv(cur->lft[j]);
        }

        /*set up linked list*/
        prev->next = cur;
        prev = cur;
    }

    if (mode != ONED) {
        HERE;

        cur = scalloc(struct surf_info, 1);
        prev->next = cur;
        cur->next = NULL;
        x = pdist(s[ns - 2].p, s[ns - 1].p);
        for (i = 0; i < mode; i++) {
            cur->rht[i] =
                lat * (pt[s[ns - 1].p]->cord[i] - pt[s[ns - 2].p]->cord[i]) /
                    x +
                pt[s[ns - 1].p]->cord[i];
            cur->lft[i] = pt[s[ns - 1].p]->cord[i];
            HERE;
            verpv(cur->lft[i]);
            HERE;
        }
    }

    prev = NULL;
    if (mode == ONED)
        id = 0;
    else
        id = 1;

    HERE;
    verpv(lft.top[0]);
    HERE;

    cur = *v;
    while (cur != NULL) {
        HERE;
        verpv(cur);
        HERE;
        if (prev == NULL) {
            HERE;
            atop = 0.5 * (cur->lft[id] + cur->rht[id]);
            HERE;
            verpv(atop);
            HERE;
            cur->nmat = lft.nmat;
            for (j = 0; j < lft.nmat; j++) {
                verpv(j);
                cur->top[j] = lft.top[j] - lft.top[0] + atop;
                cur->bot[j] = lft.bot[j] - lft.top[0] + atop;
                cur->mat[j] = lft.mat[j];
            }
        } else if (cur->next == FALSE) {
            HERE;
            cur->nmat = rht.nmat;
            atop = 0.5 * (cur->lft[id] + cur->rht[id]);
            for (j = 0; j < rht.nmat; j++) {
                verpv(j);
                HERE;
                verpv(rht.top[j]);
                HERE;
                verpv(rht.top[0]);
                HERE;
                verpv(atop);
                HERE;
                cur->top[j] = rht.top[j] - rht.top[0] + atop;
                cur->bot[j] = rht.bot[j] - rht.top[0] + atop;
                cur->mat[j] = rht.mat[j];
            }
        } else if (cur->lft[0] >= cur->rht[0]) {
            HERE;
            cur->nmat = 0;
        } else {
            HERE;
            get_mat(cur, FALSE);
        }
        prev = cur;
        cur = cur->next;
    }

#ifdef DEBUGGING_2020

    cur = *v;
    verpv(cur->nmat);
    while (cur != NULL) {
        HERE;
        verpv(cur);
        HERE;
        verpv(cur->top[0]);
        cur = cur->next;
    }

#endif

    EXIT;
}

/************************************************************************
 *									*
 *	get_mat() - This routine finds the material indices for a given	*
 *  surface point.							*
 *									*
 *  Original:	MEL	12/86						*
 *									*
 ************************************************************************/
void get_mat(struct surf_info *cur, int angle) {
    int j, k;
    float mval[2 * MAXMAT], tf;
    int mat[2 * MAXMAT], ti;
    int mnum;
    float x[3];
    ENTER;

    get_edge(mval, mat, &mnum, Y_SLICE, 0.5 * (cur->lft[0] + cur->rht[0]));

    HERE;
    verpv(mval[0]);
    HERE;

#ifdef DEBUGGING_2020

    for (j = 0; j < mnum; j++) {
        verpv(j);
        HERE;
        verpv(mval[j]);
        HERE;
    }

#endif

    HERE;

    /*order from top to bottom each material*/
    for (j = 0; j < mnum; j += 2) {
        if (mval[j] > mval[j + 1]) {
            /*swap them*/
            ti = mat[j + 1];
            mat[j + 1] = mat[j];
            mat[j] = ti;
            tf = mval[j + 1];
            mval[j + 1] = mval[j];
            mval[j] = tf;
        }
    }

    HERE;

    /*order the pairs of materials*/
    for (j = 0; j < mnum - 2; j += 2) {
        for (k = j + 2; k < mnum; k += 2) {
            if (mval[k] < mval[j]) {
                /*swap them*/
                ti = mat[k];
                mat[k] = mat[j];
                mat[j] = ti;
                tf = mval[k];
                mval[k] = mval[j];
                mval[j] = tf;
                ti = mat[k + 1];
                mat[k + 1] = mat[j + 1];
                mat[j + 1] = ti;
                tf = mval[k + 1];
                mval[k + 1] = mval[j + 1];
                mval[j + 1] = tf;
            }
        }
    }

    HERE;

#ifdef DEBUGGING_2020
    for (j = 0; j < mnum; j++) {
        verpv(j);
        HERE;
        verpv(mval[j]);
        HERE;
    }
#endif

    if (angle) {
        HERE;
        switch (mode) {
        case ONED:
            HERE;
            x[0] = mval[0] / tr[1][1];
            break;
        default:
            HERE;
            verpv(cur->top[0]);
            HERE;
            x[0] = cur->top[0] * tr[1][0] + mval[0] * tr[1][1];
            break;
        }

        HERE;
        verpv(x[0]);
        HERE;

        for (j = 1; j < mnum; j++)
            mval[j] = (mval[j] - mval[0]) / tr[0][0] + x[0];
        mval[0] = x[0];
    }

    HERE;
#ifdef DEBUGGING_2020
    for (j = 0; j < mnum; j++) {
        verpv(j);
        HERE;
        verpv(mval[j]);
        HERE;
    }
#endif
    HERE;

    /*load the material offsets*/
    for (cur->nmat = j = 0; j < mnum; j += 2) {

        verpv(j);
        HERE;

        cur->top[cur->nmat] = mval[j];
        cur->bot[cur->nmat] = mval[j + 1];
        cur->mat[cur->nmat] = mat[j];

        HERE;
        verpv(cur->top[cur->nmat]);
        HERE;

        cur->nmat++;
    }

    EXIT;
}

/************************************************************************
 *									*
 *	free_surf() - This routine finds and orders a surface list	*
 *  for use in implantation.						*
 *									*
 *  Original:	MEL	12/86						*
 *									*
 ************************************************************************/
void free_surf(struct surf_info **v) {
    struct surf_info *cur, *next;

    cur = *v;

    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}
