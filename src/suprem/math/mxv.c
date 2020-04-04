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
/*   mxv.c                Version 5.1     */
/*   Last Modification : 7/3/91 10:44:17 */

#include <math.h>
#include <stdio.h>

#include "./src/suprem/include/constant.h"
#include "./src/suprem/include/global.h"
#include "./src/suprem/include/impurity.h"
#include "./src/suprem/include/matrix.h"
#include "./src/suprem/include/sysdep.h"

/* 2020 includes:*/
#include "mxv.h"
/* end of includes*/

/* 2020 forward declarations:*/
void blkmxv(int b1, int b2, int bnot, int nv, int sum, double *vec,
			double *avec);
void vinit(double a[], int nv);
/* end of 2020 declarations*/

/*************************************************************************
 *									*
 *	mxv(nv, ia, aoff, a, v, av) - This routine multiplies a * v and	*
 *  stores it in av.							*
 *									*
 *  Original:	MEL	4/85						*
 *									*
 ************************************************************************/
void mxv(int nv, int *ia, int aoff, double sign, double *a, double *v,
		 double *av) {
	register int i, j;
	register int endloop;
	double mval;

	if (sign == 0.0) {
/*$dir no_recurrence*/
#pragma GCC ivdep
		for (i = 0; i < nv; i++)
			av[i] = a[i] * v[i];
	} else if (sign < 0.0) {
/*$dir no_recurrence*/
#pragma GCC ivdep
		for (i = 0; i < nv; i++)
			av[i] -= a[i] * v[i];
	} else {
/*$dir no_recurrence*/
#pragma GCC ivdep
		for (i = 0; i < nv; i++)
			av[i] += a[i] * v[i];
	}

	if (sign >= 0.0) {
		for (i = 0; i < nv; i++) {
			endloop = ia[i + 1];
			mval = av[i];
			/*$dir no_recurrence*/
			for (j = ia[i]; j < endloop; j++) {
				mval += a[j + aoff] * v[ia[j]];
				av[ia[j]] += a[j] * v[i];
			}
			av[i] = mval;
		}
	} else {
		for (i = 0; i < nv; i++) {
			endloop = ia[i + 1];
			mval = av[i];
			/*$dir no_recurrence*/
			for (j = ia[i]; j < endloop; j++) {
				mval -= a[j + aoff] * v[ia[j]];
				av[ia[j]] -= a[j] * v[i];
			}
			av[i] = mval;
		}
	}
}

/*************************************************************************
 *									*
 *  mxv_tri(nv, ia, aoff, a, v, av) - This routine multiplies a * v and	*
 *  stores it in av.  ( for 1-D tridiagonal block )                     *
 *									*
 *  Original:	LCC	6/90						*
 *	     (adapted from MXV)
 **
 ************************************************************************/
void mxv_tri(nv, ia, aoff, sign, a, v, av) int nv;
int *ia;
int aoff;
double sign;
double *a;
double *v;
double *av;
{
	register int i, j;

	if (sign == 0.0) {
		for (i = 0; i < nv; i++)
			av[i] = a[i] * v[i];
		for (i = 0, j = ia[0]; i < (nv - 1); i++, j++) {
			av[i] += a[j + aoff] * v[i + 1];
			av[i + 1] += a[j] * v[i];
		}
	} else if (sign > 0.0) {
		for (i = 0, j = ia[0]; i < (nv - 1); i++, j++) {
			av[i] += a[i] * v[i];

			av[i] += a[j + aoff] * v[i + 1];
			av[i + 1] += a[j] * v[i];
		}
		av[nv - 1] += a[nv - 1] * v[nv - 1];
	} else {
		for (i = 0, j = ia[0]; i < (nv - 1); i++, j++) {
			av[i] -= a[i] * v[i];

			av[i] -= a[j + aoff] * v[i + 1];
			av[i + 1] -= a[j] * v[i];
		}
		av[nv - 1] -= a[nv - 1] * v[nv - 1];
	}
}

/*************************************************************************
 *									*
 *  mxv_diag(nv, ia, aoff, a, v, av) -This routine multiplies a * v and	*
 *  stores it in av.	( for diagonal block )				*
 *									*
 *  Original:	LCC	6/90						*
 *	     (adapted from MXV)						*
 ************************************************************************/
void mxv_diag(int nv, double sign, double *a, double *v, double *av) {
	register int i;

	if (sign == 0.0) {
		for (i = 0; i < nv; i++)
			av[i] = a[i] * v[i];
	} else if (sign > 0.0) {
		for (i = 0; i < nv; i++)
			av[i] += a[i] * v[i];
	} else {
		for (i = 0; i < nv; i++)
			av[i] -= a[i] * v[i];
	}
}

/************************************************************************
 *									*
 *	bigmxv( vec, avec ) - This routine multiplies the big matrix	*
 *  times a vector of answers.						*
 *									*
 *  Original:	MEL	4/85						*
 *									*
 ************************************************************************/
void bigmxv(int nsol, int *sol, int nv, double **vec, double **avec) {
	register int i, j, si, sj;

	/*initialize the avec vector by doing the diagonal multiplies*/
	for (i = 0; i < nsol; i++) {
		si = sol[i];
		blkmxv(si, si, -1, nv, /*no sum*/ FALSE, vec[si], avec[si]);
	}

	/*multiply the block times the vector*/
	for (i = 0; i < nsol; i++) {
		si = sol[i];
		for (j = 0; j < nsol; j++) {
			sj = sol[j];
			blkmxv(si, sj, si, nv, TRUE, vec[sj], avec[si]);
		}
	}
}

/************************************************************************
 *									*
 *	blkmxv( ) - This routine checks the block type and multiplies	*
 *  the block times the vector.						*
 *							                *
 *  Original:	MEL	4/85						*
 *		LCC     6/90						*
 *			( 1-D use mxv_tri )				*
 *									*
 ************************************************************************/
void blkmxv(int b1, int b2, int bnot, int nv, int sum, double *vec,
			double *avec) {
	register int i;
	double sign = (sum ? 1.0 : 0.0);
	double *av = avec;
	double *am = a[b1][b2];

	if (b2 == bnot)
		return;

	if (blktype[b1][b2] == B_NONE) {
		if (!sum)
			vinit(av, nv);
	} else if (blktype[b1][b2] == B_TRI) {
		mxv(nv, bia[b1][b2], baoff[b1][b2], sign, a[b1][b2], vec, avec);
	} else if (blktype[b1][b2] == B_DIAG) {
		if (sign == 0.0) {
			for (i = 0; i < nv; i++)
				avec[i] = am[i] * vec[i];
		} else if (sign < 0.0) {
			for (i = 0; i < nv; i++)
				avec[i] -= am[i] * vec[i];
		} else {
			for (i = 0; i < nv; i++)
				avec[i] += am[i] * vec[i];
		}
	} else if (blktype[b1][b2] == B_BLCK) {
		mxv(nv, bia[b1][b2], baoff[b1][b2], sign, a[b1][b2], vec, avec);
	}
}

void vinit(double a[], int nv) {
	register int k;

	/*$dir no_recurrence*/
	for (k = 0; k < nv; k++)
		a[k] = 0.0;
}
