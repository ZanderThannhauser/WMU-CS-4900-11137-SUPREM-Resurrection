/*	window.c		Version 5.1	*/

#include <math.h>
#include <stdio.h>

/* 2020 includes:*/
#include "window.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

#ifdef WDEBUG
main()
{
	float xarray[101];
	float yarray[101];
	float xkey;
	float ykey;
	int   xlo;
	int   ylo;
	int   xhi;
	int   yhi;
	int   xsize = 101;
	int   ysize = 101;
	int   i;

	for (i = 0; i < 101; i++)
	{
		xarray[i] = 0.01 * (float)i;
		yarray[i] = 0.01 * (float)i;
	}

	printf("enter values :  ");
	scanf("%f %f", &xkey, &ykey);
	fflush(stdout);
	window(xkey, ykey, xarray, yarray, xsize, ysize, &xlo, &xhi, &ylo, &yhi);

	printf("xlo=%d, xhi=%d, ylo=%d, yhi=%d\n", xlo, xhi, ylo, yhi);
}
#endif /*WDEBUG*/

void window(float x, float y, float *xarray, float *yarray, int xsize,
			int ysize, int *xlo, int *xhi, int *ylo, int *yhi)
{
	static int lo;
	static int hi;
	static int middle;
	static int done;

	/* check for exceeding boundary conditions */
	if (x * 1.0e4 > xarray[xsize - 1])
	{
		*xlo = xsize - 1;
		*xhi = xsize - 1;
	}
	else if (x * 1.0e4 < xarray[0])
	{
		*xlo = 0;
		*xhi = 0;
	}
	else
	{
		/* window in x direction */
		lo = 0;
		hi = xsize;
		done = 0;
		while (!done)
		{
			middle = (hi + lo) / 2;
			if ((xarray[middle] <= x * 1.0e4) && (middle > lo))
				lo = middle;
			if ((xarray[middle] >= x * 1.0e4) && (middle < hi))
				hi = middle;
			if ((hi + lo) / 2 == middle)
				done = 1;
		}
		if (x == xarray[lo])
			hi = lo;
		if (x == xarray[hi])
			lo = hi;
		*xlo = lo;
		*xhi = hi;
	}

	/* check for exceeding boundary conditions */
	if (y * 1.0e4 > yarray[ysize - 1])
	{
		*ylo = ysize - 1;
		*yhi = ysize - 1;
	}
	else if (y * 1.0e4 < yarray[0])
	{
		*ylo = 0;
		*yhi = 0;
	}
	else
	{
		/* window in y direction */
		lo = 0;
		hi = ysize;
		done = 0;
		while (!done)
		{
			middle = (hi + lo) / 2;
			if ((yarray[middle] <= y * 1.0e4) && (middle > lo))
				lo = middle;
			if ((yarray[middle] >= y * 1.0e4) && (middle < hi))
				hi = middle;
			if ((hi + lo) / 2 == middle)
				done = 1;
		}
		if (y == yarray[lo])
			hi = lo;
		if (y == yarray[hi])
			lo = hi;
		*ylo = lo;
		*yhi = hi;
	}
}
