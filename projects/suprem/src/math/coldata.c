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
/*   coldata.c                Version 5.1     */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#ifdef ALLIANT
#include <cncall.h>
#endif

#include "suprem/include/constant.h"
#include "suprem/include/global.h"
#include "suprem/include/matrix.h"

/* 2020 includes:*/
#include "../misc/panic.h"
#include "coldata.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	get_coldata() - This routine sets up the a matrix entries.	*
 *									*
 *  Original:	MEL	6/88						*
 *									*
 ************************************************************************/
void get_coldata(int vi, int bi, int nsol, int *sol, int *ia, int aoff,
				 double *col, double *row)
{
	register int  si, bj, sj;
	register int  j;
	register int  tloc;
	register int *tia, tao;

	si = sol[bi];
	for (bj = 0; bj < nsol; bj++)
	{
		sj = sol[bj];
		if (!facblk[si][sj])
			continue;

		if (blktype[si][sj] != B_NONE)
		{
			tia = bia[si][sj];
			tao = baoff[si][sj];

			tloc = vi * nsol + bj;
			row[tloc] = a[si][sj][vi];

			/*$dir no_recurrence*/
			for (j = tia[vi]; j < tia[vi + 1]; j++)
				row[tia[j] * nsol + bj] = a[si][sj][j + tao];
		}

		if (blktype[sj][si] != B_NONE)
		{
			tia = bia[sj][si];

			tloc = vi * nsol + bj;
			col[tloc] = a[sj][si][vi];

			/*$dir no_recurrence*/
			for (j = tia[vi]; j < tia[vi + 1]; j++)
				col[tia[j] * nsol + bj] = a[sj][si][j];
		}
	}
}

/************************************************************************
 *									*
 *	get_locs() - This routine sets the matrix locations.		*
 *									*
 *  Original:	MEL	6/88						*
 *									*
 ************************************************************************/
int get_locs(int vi, int bi, int nsol, int *sol, int *ia, int aoff, int *icol)
{
	register int  si, bj, sj;
	register int  j;
	register int  tloc;
	register int  last_nbr = 0;
	register int *tia;

	si = sol[bi];
	/*figure the positions in the matrix*/
	for (bj = 0; bj < nsol; bj++)
	{
		sj = sol[bj];
		if (!facblk[si][sj])
			continue;
		tia = bia[sj][si];
		if (blktype[sj][si] != blktype[si][sj])
			panic("must be symmetry");
		if (tia != NULL)
		{
			tloc = vi * nsol + bj;
			icol[tloc] = 1;
			if (tloc > last_nbr)
				last_nbr = tloc;
			/*$dir no_recurrence*/
			for (j = tia[vi]; j < tia[vi + 1]; j++)
				icol[tia[j] * nsol + bj] = 1;
			tloc = tia[j - 1] * nsol + bj;
			if ((tloc > last_nbr) && (tia[vi] != tia[vi + 1]))
				last_nbr = tloc;
		}
	}
	return (last_nbr);
}
