/*************************************************************************
 *									 *
 *     Copyright c 1989 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of the University of 	 *
 *     Florida.								 *
 *									 *
 *************************************************************************/
/*   Electron.c                Version 5.2     */

#include <math.h>
#include <stdio.h>

#include "suprem/include/constant.h"
#include "suprem/include/device.h"
#include "suprem/include/diffuse.h"
#include "suprem/include/geom.h"
#include "suprem/include/global.h"
#include "suprem/include/impurity.h"
#include "suprem/include/material.h"
#include "suprem/include/matrix.h"

/* 2020 includes:*/
#include "debug.h"
#include "../misc/get.h"
#include "Electron.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	Nmobil(temp, noni, inter, vacan) - This routine calculates	*
 *  the interstitial diffusivity given the temperature, n/ni, and	*
 *  local defect concentrations.					*
 *									*
 *  Original:	MEL	2/89						*
 *									*
 ************************************************************************/
double Nmobil() { return (0.0); }

/************************************************************************
 *									*
 *	Ncoupling( ) - This routine computes the recombination of 	*
 *  interstitials with the other species.				*
 *									*
 *  Original:	MEL	2/89						*
 *									*
 ************************************************************************/
void Ncoupling(float temp, double *area, double **new, double **equil,
			   double **dequ, double **rhs)
{
}

/************************************************************************
 *									*
 *	Nboundary - This routine handles the computation of electrons	*
 *  boundary conditions at node i and solutions j and k.  The time step	*
 *  is delta and the temperature is temp.  Vel is the velocity of the	*
 *  boundary.								*
 *									*
 *  Original:	MEL	2/89						*
 *									*
 ************************************************************************/
void Nboundary(struct bound_str *bval) {}

/************************************************************************
 *									*
 *	Phi_n( ) - This routine computes the quasi-fermi level and	*
 *  derivatives given the potential, material, and electron 		*
 *  concentration							*
 *									*
 *  Original:	MEL	2/89						*
 *									*
 ************************************************************************/
double Phi_n(double elec, double psi, double ec, int mat, double *dqdp,
			 double *dqde)
{
	double qfn;

	if (IS_SEMI(mat))
	{
		qfn = -log(elec / Ncon(mat)) * devVt + psi - ec;
		*dqdp = 1.0;
		*dqde = -devVt / elec;
	}
	else
	{
		qfn = 0.0;
		*dqdp = 0.0;
		*dqde = 0.0;
	}

	return (qfn);
}

/************************************************************************
 *									*
 *	Electron( par, param ) - This routine gets the Electron 	*
 *  parameters from the cards and sets them up. 			*
 *									*
 *  Original:	MEL	2/89						*
 *									*
 ************************************************************************/
void electron(char *par, struct par_str *param)
{
	int mat, mat2 = -1;
	ENTER;

#define fetch(V, S, F)                                                         \
	if (is_specified(param, S))                                                \
	V = F * get_float(param, S)
#define listed(S) (get_bool(param, S) && is_specified(param, S))

	/*get the material number specified*/
	if (get_bool(param, "silicon"))
		mat = Si;
	if (get_bool(param, "oxide"))
		mat = SiO2;
	if (get_bool(param, "oxynitride"))
		mat = OxNi;
	if (get_bool(param, "poly"))
		mat = Poly;
	if (get_bool(param, "nitride"))
		mat = SiNi;
	if (get_bool(param, "gaas"))
		mat = GaAs;
	if (get_bool(param, "gas"))
		mat = GAS;

	/*get the interface material if given*/
	/*
	if ( listed( "/silicon" ) )		mat2 = Si;
	if ( listed( "/oxide" ) )		mat2 = SiO2;
	if ( listed( "/oxynitride" ) )	mat2 = OxNi;
	if ( listed( "/nitride" ) )		mat2 = SiNi;
	if ( listed( "/poly" ) )		mat2 = Poly;
	if ( listed( "/gaas" ) )		mat2 = GaAs;
	if ( listed( "/gas" ) )		mat2 = GAS;
	*/

	fetch(Econ(mat), "Ec", 1.0);
	fetch(Ncon(mat), "Nc", 1.0);

	/*if no second material listed, no interfce parameters can be*/
	if (mat2 != -1)
	{
	}
	
	EXIT;
}














