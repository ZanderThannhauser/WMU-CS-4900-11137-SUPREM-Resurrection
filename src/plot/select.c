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
/*   select.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:39:07 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./src/include/constant.h"
#include "./src/include/dbaccess.h"
#include "./src/include/expr.h"
#include "./src/include/global.h"
#include "./src/include/material.h"
#include "./src/include/plot.h"
#include "./src/include/sysdep.h"

// 2020 includes:
#include "./src/debug.h"
#include "./src/oxide/mater.h"
#include "./src/check/parse.h"
#include "./src/check/eval.h"
#include "select.h"
// end of includes

// 2020 forward declarations
// end of declarations

/************************************************************************
 *									*
 *	sel_var( par, param ) - this routine sets up a solution vector 	*
 *  for the plot commands that need one.				*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void sel_var(char *par, struct par_str* param)
{
    char *zstr;
    struct vec_str *zexp;
    char *err;
    char *tmp;

    zstr = get_string( param, "z" );
    znn = nn;
    z = salloc( float, nn );

    if ( is_specified( param, "label" ) ) {
	if ( label != NULL ) free( label );
	tmp = get_string( param, "label" );
	label = (char *)malloc( strlen(tmp) + 1 );
	strcpy( label, tmp );
    }
    else {
	if ( label != NULL ) free( label );
	label = (char *)malloc( strlen( zstr ) + 1 );
	strcpy( label, zstr );
    }

    if ( is_specified( param, "title" ) ) {
	if ( title != NULL ) free( title );
	tmp = get_string( param, "title" );
	title = (char *)malloc(strlen(tmp) + strlen(VERSION) + strlen(" - ") + 1);
	strcpy(title, tmp );
	strcat(title, " - ");
	strcat(title, VERSION );
    }
    else {
	if ( title != NULL ) free( title );
	title = (char *)malloc(strlen(VERSION) + 1);
	strcpy(title, VERSION );
    }

    if ( is_specified( param, "temp" ) ) {
	last_temp = get_float( param, "temp" ) + 273.0;
	/*compute the sislicon material constants*/
	comp_mat( last_temp );
    }


    /*attempt to parse the string*/
    if ( (err = (char *)parse_expr(zstr, &zexp)) != NULL) {
	fprintf(stderr, "%s\n", err);
	return; //(-1);
    }

    /*evaluate the vector expression*/
    if ( (err = (char *)eval_vec( zexp, z )) != NULL) {
	fprintf(stderr, "%s\n", err);
	return; //(-1);
    }

    sel_log = islogexp( zexp );

    free_expr( zexp );
    return; //(0);
}
