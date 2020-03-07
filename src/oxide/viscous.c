/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   viscous.c                Version 5.1     */
/*   Last Modification : 7/3/91 10:52:44 */

#include <assert.h>
#include <stdio.h>

#include "./src/include/constant.h"
#include "./src/include/geom.h"
#include "./src/include/global.h"
#undef EXTERN
#define EXTERN /* FEgeom isn't included in main.c */
#include "./src/include/FEgeom.h"
#undef EXTERN
#define EXTERN extern
#include "./src/include/FEmath.h"
#include "./src/include/material.h"

// 2020 includes:
#include "./src/oxide/FEconvert.h"
#include "./src/oxide/FEbc.h"
#include "./src/finel/FEsolve.h"
#include "./src/dbase/dispose.h"
#include "./src/misc/get.h"
#include "./src/oxide/Oxidant.h"
#include "./src/oxide/triox.h"
#include "./src/finel/tri6.h"
#include "./src/oxide/oxide_vel.h"
#include "./src/finel/tri7.h"
#include "viscous.h"
// end of includes

// 2020 forward declarations
void FEmathset(int flow);
// end of declarations

/* By arrangement with FEconvert.c */
#define TRIOX 0
#define ELAST6 1
#define ELAST7 2
#define DUMMY 3

/* another hack by Zak */
#define NEL 7

/************************************************************************
 *									*
 *	visco_growth()	- Controller for the viscous flow model		*
 *									*
 ************************************************************************/
float proc_temp;

void visco_growth(float temp, double dt) {
    proc_temp = temp;

    /*Generate appropriate data structures for the FE method*/
    if (FEconvert(TRIOX, 3) < 0)
        return;

    /* Boundary conditions */
    FEoxbc();

    /* Call the FE solver on the oxide problem */
    FEmathset(1);
    FEsolve(verbose >= V_CHAT);

    /* Store the displacements and stresses in S4's data structure */
    FE2s4ox();

    if (SilStress) {
        /* Postprocessing for fun and profit: do silicon stress calc */
        /* New BC's */
        FEdf = 2;
        FEsilbc();

        /* New math */
        FEmathset(0);

        /* and away we go. */
        FEsolve(verbose >= V_CHAT);

        /* Store silicon stresses in S4 data structure. */
        FE2s4sil();
    }

    /* Later */
    FEfree();
    return;
}

/*-----------------Stress_Analysis--------------------------------------
 * Thermal elastic stresses.
 *----------------------------------------------------------------------*/
void stress_analysis(char *par, struct par_str *param)
{
    float temp1 = 1000, temp2 = 1000;
    int element;

    if (InvalidMeshCheck())
        return; // -1;

    if (is_specified(param, "temp1"))
        temp1 = 273 + get_float(param, "temp1");
    if (is_specified(param, "temp2"))
        temp2 = 273 + get_float(param, "temp2");

    ThermSig(temp1, temp2);
    AddIntSig();

    if (!is_specified(param, "nel"))
        element = ELAST6;
    else if (get_int(param, "nel") == 6)
        element = ELAST6;
    else if (get_int(param, "nel") == 7)
        element = ELAST7;
    else {
        fprintf(stderr, "Only know 6 & 7 noded elements\n");
        return; // (-1);
    }

    /* fixed by Zak to ensure that data structures are initialized
     *  for the case of Stress calculations without any previous
     *  stress-dependent oxidation.
     */
    FEdesc[0].nel = NEL;
    FEdesc[0].gaussp = 3;
    FEdesc[0].stiff = triox_stiff;
    FEdesc[0].coeff = FE_oxbulk;
    FEdesc[0].bound = triox_bc;
    FEdesc[0].nodal_stress = triox_nodal_stress;

    FEdesc[1].nel = 6;
    FEdesc[1].gaussp = 3;
    FEdesc[1].stiff = tri6_stiff;
    FEdesc[1].coeff = sup4_ecoeff;
    FEdesc[1].bound = noop;
    FEdesc[1].nodal_stress = tri6_Snodal_stress;

    FEdesc[2].nel = 7;
    FEdesc[2].gaussp = 3;
    FEdesc[2].stiff = tri7_stiff;
    FEdesc[2].coeff = sup4_ecoeff;
    FEdesc[2].bound = noop;
    FEdesc[2].nodal_stress = tri7_nodal_stress;

    FEdesc[3].nel = NEL;
    FEdesc[3].gaussp = 0;
    FEdesc[3].stiff = NULL;
    FEdesc[3].coeff = NULL;
    FEdesc[3].bound = NULL;
    FEdesc[3].nodal_stress = NULL;

    FEdesc[4].nel = 7;
    FEdesc[4].gaussp = 3;
    FEdesc[4].stiff = tri7_stiff;
    FEdesc[4].coeff = sup4_ecoeff;
    FEdesc[4].bound = oxload;
    FEdesc[4].nodal_stress = tri7_nodal_stress;

    /* Generate appropriate data structures for the FE method */
    if (FEconvert(element, 2) < 0)
        return; // (-1);

    /*
     * Set up boundary conditions.
     */
    FEbc(temp1, par, param);

    /* Call the FE solver */
    FEmathset(0);
    FEsolve(verbose >= V_CHAT);

    /*store the displacements and stresses somewhere permanent*/
    FE2s4all();

    FEfree();
    return; // (0);
}

void FEmathset(int flow) /* Doing flow or stress calc? */
{
    float Oss(), Ovel();
    static double dummy[] = {0.0, 1.0};

    /* Use B/A to scale velocities */
    if (flow)
        FEabe[0] = FEabe[1] =
            Ovel(proc_temp, gas_type, Oss(gas_type, SiO2), dummy, 1.0);
    else
        FEabe[0] = FEabe[1] = 1.0;

    /* Use gas boundary condition to scale concentrations */
    if (flow)
        FEabe[2] = Oss(gas_type, SiO2);

    /* Boundary rows make flow problem asymmetric. */
    if (!flow)
        FEsymm = 1;
    else
        FEsymm = 0;

    /* Force 1 loop solution for linear problems */
    if (stress_dep)
        FEnonloop = 250;
    else
        FEnonloop = 1;

    /* Direct solution, no iterative nonsense. */
    FElinmeth = F_FULL;

    /* Do some sort of minimum degree order and reset things when done */
    FEorder = O_MF | O_RESETORDER;

    /* No accuracy checks on linear solution */
    FEcheck = 0;
}
