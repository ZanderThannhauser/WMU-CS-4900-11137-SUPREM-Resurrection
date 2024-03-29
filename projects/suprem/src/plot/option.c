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
/*   option.c                Version 5.1     */

#include <math.h>
#include <stdio.h>

#include "suprem/include/global.h"

/* 2020 includes*/
#include "debug.h"
#include "../misc/get.h"
#include "option.h"
/* end of includes*/

/************************************************************************
 *									*
 *	This file implements the option card.  It currently only has	*
 *  one parameter, terminal.  It should in some future day ave more 	*
 *  configuration options present.					*
 *  									*
 *  Now it does - CSR Sep 86						*
 *									*
 ************************************************************************/

/************************************************************************
 *									*
 *	option( par, param ) - this card handles config	parameters for 	*
 *  the system.								*
 *									*
 *  Original:	MEL	10/84						*
 *									*
 ************************************************************************/
void option(char *par, struct par_str *param)
{
	ENTER;
	
#define CHOSEN(x) (is_specified(param, x) && get_bool(param, x))
	/*how much barfola the user wants*/
	if (CHOSEN("quiet"))
		verbose = V_QUIET;
	if (CHOSEN("normal"))
		verbose = V_NORMAL;
	if (CHOSEN("chat"))
		verbose = V_CHAT;
	if (CHOSEN("barf"))
		verbose = V_BARF;
	
	EXIT;
}















