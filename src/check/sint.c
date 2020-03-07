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
/*   sint.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:12:33 */

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "./src/include/constant.h"
#include "./src/include/dbaccess.h"
#include "./src/include/expr.h"
#include "./src/include/global.h"
#include "./src/include/plot.h"

// 2020 includes:
#include "./src/debug.h"
#include "./src/plot/do_1d.h"
#include "sint.h"
// end of includes

// 2020 forward declarations
// end of declarations

/************************************************************************
 *									*
 *	sol_interp() - This routine evaluates two the two parameter fns	*
 *  that are used to interpolate solution variables.			*
 *									*
 *  Original:	MEL	8/85						*
 *									*
 ************************************************************************/
float sol_interp(int type, float val1, float val2)
{
    register int i;
    struct d_str *data;
    int count;
    float ret;
    ENTER;
    verpv(type);
    verpv(val1);
    verpv(val2);

    if (mode == ONED)
    {
    	HERE;
    	verpv(3 * ned);
        data = salloc(struct d_str, 3 * ned);
        }
    else
    {
        HERE;
        data = salloc(struct d_str, 2 * ne);
        }

   verpv(data);
   verpv(mode);
    /*get a one dimensional line profile*/
    switch (mode) {
    case TWOD:
        verpv(type);
        switch (type) {
        case X:
            HERE;
            count = do_1d(YSEC, val1 * 1.0e-4, data, -1, -1, 0);
            break;
        case Y:
            HERE;
            count = do_1d(XSEC, val1 * 1.0e-4, data, -1, -1, 0);
            break;
        case Z:
            HERE;
            count = do_1d(XSEC, val1 * 1.0e-4, data, -1, -1, 0);
            break;
        }
        break;
    case ONED:
        verpv(type);
        switch (type) {
        case X:
            HERE;
            count = do_1d(YSEC, 0.0, data, -1, -1, 0);
            break;
        case Y:
            HERE;
            count = do_1d(YSEC, 0.0, data, -1, -1, 0);
            break;
        case Z:
            HERE;
            count = do_1d(YSEC, 0.0, data, -1, -1, 0);
            break;
        }
    }

    verpv(count);
    /*x and y functions work similarly*/
    if ((type == X) || (type == Y)) {

        /*run up the list and find the intersection*/
        for (i = count - 1;
             ((data[i].y > val2) == (data[i - 1].y > val2)) && (i > 1); i--)
            ;

        /*interpolate to the answer*/
        if (data[i].y == data[i - 1].y)
            ret = data[i].x;
        else
            ret = (val2 - data[i - 1].y) * (data[i].x - data[i - 1].x) /
                      (data[i].y - data[i - 1].y) +
                  data[i - 1].x;

        ret = ret * 1.0e4;
    } else {
        val2 *= 1.0e-4;

        /*run up the list and find the intersection*/
        for (i = count - 1;
             ((data[i].x > val2) == (data[i - 1].x > val2)) && (i > 1); i--)
         {
         verpv(i);
         }

        /*interpolate to the answer*/
        if (data[i].x == data[i - 1].x)
            ret = data[i].x;
        else
            ret = (val2 - data[i - 1].x) * (data[i].y - data[i - 1].y) /
                      (data[i].x - data[i - 1].x) +
                  data[i - 1].y;
    }
    sfree(data);
    EXIT;
    return (ret);
}

/************************************************************************
 *									*
 *	sol_interp() - This routine evaluates two the two parameter fns	*
 *  that are used to interpolate solution variables.			*
 *									*
 *  Original:	MEL	8/85						*
 *									*
 ************************************************************************/
float interface(type, loc) int type;
float loc;
{
    register int i;
    struct d_str *data;
    int count, match;
    int mat1, mat2;
    float ret;

    if (mode == ONED)
        data = salloc(struct d_str, 3 * ned);
    else
        data = salloc(struct d_str, 2 * ne);

    /*get a one dimensional line profile*/
    if (mode == TWOD)
        count = do_1d(XSEC, loc * 1.0e-4, data, -1, -1, 0);
    else
        count = do_1d(YSEC, 0.0, data, -1, -1, 0);

    /*split out the material numbers*/
    type /= MAXMAT;

    mat2 = type % MAXMAT;
    mat1 = (type - mat2) / MAXMAT;

    /*run up the data and find the intersection*/
    for (match = i = 0; (i < count - 1) && (!match); i++) {
        if ((data[i].mat == mat1) && (data[i + 1].mat == mat2))
            match = i;
        if ((data[i].mat == mat2) && (data[i + 1].mat == mat1))
            match = i + 1;
    }

    if (match)
        ret = data[match].x * 1.0e4;
    else
        ret = -1.0;

    sfree(data);
    return (ret);
}
