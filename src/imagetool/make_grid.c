/*	make_grid.c 		Version 5.1	*/
/*	Last Modification:  7/3/91 15:46:18		*/

/*	make_grid.c		Version 1.2		*/
/*	Last Modification:	12/11/89 13:44:31		*/

#include <stdio.h>
#include <stdlib.h>

#include "./src/include/bound.h"

// 2020 includes:
#include "make_grid.h"
// end of includes

// 2020 forward declarations
float Fmax(float x, float y);
float Fmin(float x, float y);
void shiftwindow(float *array, int size, float min, float max, int min_index,
                 int max_index);
// end of declarations

int make_grid(int xsize, int ysize, float **xd, float **yd, float win_xmin,
              float win_xmax, float win_ymin, float win_ymax,
              int *win_xmin_index, int *win_xmax_index, int *win_ymin_index,
              int *win_ymax_index) {
    int index;
    int redox = 0;
    int redoy = 0;
    float *xdata;
    float *ydata;
    float loc;
    float spacing;
    float *ptr;

    if ((xdata = (float *)malloc((xsize + 3) * sizeof(float))) == NULL) {
        fprintf(stderr, "unable to allocate memory\n");
        exit(ERROR);
    }
    if ((ydata = (float *)malloc((ysize + 3) * sizeof(float))) == NULL) {
        fprintf(stderr, "unable to allocate memory\n");
        exit(ERROR);
    }

    /* for now use uniform spacing */

    /* for the case where the window coordinates are outside the
     *   current device dimensions, we need to redefine the spacing
     *   to be max( (max_x - min_x)/xsize ,
     *		(max(win_xmax, max_x) - min(win_xmin, min_x))/xsize )
     */
    spacing = Fmax((max_x - min_x) / xsize,
                   (Fmax(win_xmax, max_x) - Fmin(win_xmin, min_x)) / xsize);
    ptr = xdata;
    for (index = 0, loc = Fmin(win_xmin, min_x); index < xsize + 3; index++) {
        *ptr++ = loc;
        loc += spacing;
    }
    /* adjust last point to coincide with edge */
    xdata[xsize] = Fmax(max_x, win_xmax);

    spacing = Fmax((max_y - min_y) / ysize,
                   (Fmax(win_ymax, max_y) - Fmin(win_ymin, min_y)) / ysize);
    ptr = ydata;
    for (index = 0, loc = Fmin(win_ymin, min_y); index < ysize + 3; index++) {
        *ptr++ = loc;
        loc += spacing;
    }
    /* adjust last point to coincide with edge */
    ydata[ysize] = Fmax(max_y, win_ymax);

    if (win_xmin <= min_x) {
        *win_xmin_index = 0;
    } else {
        *win_xmin_index = 1;
        redox = 1;
    }

    if (win_xmax < max_x)
        redox = 1;
    if (*win_xmin_index != 0)
        *win_xmax_index = xsize + 1;
    else
        *win_xmax_index = xsize;

    if (win_ymin <= min_y) {
        *win_ymin_index = 0;
    } else {
        *win_ymin_index = 1;
        redoy = 1;
    }

    if (win_ymax < max_y)
        redoy = 1;
    if (*win_ymin_index != 0)
        *win_ymax_index = ysize + 1;
    else
        *win_ymax_index = ysize;

    if (redox)
        shiftwindow(xdata, xsize + 3, win_xmin, win_xmax, *win_xmin_index,
                    *win_xmax_index);
    if (redoy)
        shiftwindow(ydata, ysize + 3, win_ymin, win_ymax, *win_ymin_index,
                    *win_ymax_index);

    *xd = xdata;
    *yd = ydata;

    return (0);
}

void shiftwindow(float *array, int size, float min, float max, int min_index,
                 int max_index) {
    float loc;
    float *ptr;
    float spacing;
    int i;

    array[min_index] = min;
    array[max_index + 1] = array[size - 3];
    array[max_index] = max;
    spacing = (max - min) / ((float)max_index - (float)min_index);

    ptr = &(array[min_index + 1]);
    for (i = min_index + 1, loc = min + spacing; i < max_index; i++) {
        *ptr++ = loc;
        loc += spacing;
    }
}

float Fmax(float x, float y) { return (x > y) ? x : y; }

float Fmin(float x, float y) { return (x < y) ? x : y; }
