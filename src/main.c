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
/*   main.c                Version 2.1     */
/*   Last Modification : 2/3/93 14:02:27 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <setjmp.h>
#define STATIC_ALLOCATION_TIME
#include "global.h"
#include "check.h"
#include "constant.h"
#include "dbaccess.h"
#include "diffuse.h"
#include "impurity.h"
#include "defect.h"
#include "key.h"
#include "material.h"
#include "matrix.h"
#include "plot.h"
#include "regrid.h"
#include "shell.h"
#include "implant.h"
#include "sysdep.h"
#include "expr.h"
#include "refine.h"
#include "poly.h"
#ifdef DEVICE
#include "device.h"
#endif

// function headers for command subroutines:
#include "./misc/echo.h"
#include "./misc/cpu.h"
#include "./misc/man.h"
#include "./plot/plot_2d.h"
#include "./plot/option.h"
#include "./math/symb.h"
#include "./diffuse/diffuse.h"
#include "./implant/implant.h"
#include "./plot/select.h"
#include "./plot/contour.h"
#include "./plot/plot_1d.h"
#include "./plot/print_1d.h"
#include "./oxide/coeffox.h"
#include "./diffuse/Vacancy.h"
#include "./diffuse/Interst.h"
#include "./diffuse/Antimony.h"
#include "./diffuse/Arsenic.h"
#include "./diffuse/Phosphorus.h"
#include "./mesh/rect.h"
#include "./mesh/initialize.h"
#include "./mesh/structure.h"
#include "./refine/deposit.h"
#include "./refine/etch.h"
#include "./diffuse/Gold.h"
#include "./plot/label.h"
#include "./oxide/viscous.h"
#include "./diffuse/Cesium.h"
#include "./diffuse/Trap.h"
#include "./device/device.h"
#include "./diffuse/Boron.h"
#include "./diffuse/Beryllium.h"
#include "./diffuse/Magnesium.h"
#include "./diffuse/Selenium.h"
#include "./diffuse/Silicon.h"
#include "./diffuse/Tin.h"
#include "./diffuse/Germanium.h"
#include "./diffuse/Zinc.h"
#include "./diffuse/Carbon.h"
#include "./diffuse/Generic.h"

int mode_cmd(char *par, int param);

struct command_table command[NUMCMD] = {
    "echo", echo, 0,			"man", man, 0,
    "initialize", initialize, 0,	"cpulog", cpu, 0,
    "plot.2d", plot_2d, 0, 		"option", option, 0,
    "method", method, 0,		"diffuse", diffuse, 0,
    "implant", implant, 0, 		"select", sel_var, 0,
    "contour", contour, 0,		"plot.1d", plot_1d, 0,
    "print.1d", print_1d, 0,		"vacancy", vacancy, 0,
    "interstitial", interstitial, 0,    "antimony", antimony, 0,
    "arsenic", arsenic, 0, 		"boron", boron, 0,
    "oxide", coeffox, 0,
    "phosphorus", phosphorus, 0, 	"region", rregion, 0,
    "boundary", redge, 0,		"line", rline, 0,
    "structure", structure, 0,	        "deposit", user_deposit, 0,
    "etch", user_etch, 0,		"gold", gold, 0,
    "stress", stress_analysis, 0,	"material", c_mater, 0,
    "cesium", cesium, 0,		"pause", uPause, 0,
    "trap", trap, 0,			"printf", user_print, 0,
    "profile", profile, 0,		"mode", mode_cmd, 0,
    "device", device, 0,		"beryllium", beryllium, 0,
    "magnesium", magnesium, 0,	        "selenium", selenium, 0,
    "isilicon", silicon, 0,		"tin", tin, 0,
    "germanium", germanium, 0,	        "zinc", zinc, 0,
    "carbon", carbon, 0,		"generic", generic, 0,
    "", NULL, 45,			"", NULL, 46,
    "", NULL, 47,			"", NULL, 48,
    "", NULL, 49};

int main(int argc, char **argv)
{
    int i;
    char dot_name[80], *t;

    /* this is for debugging only */
    if (0) { double x; void pa(); pa(&x, 0, 0); }

    /*line at a time buffering*/
#ifdef BSD
    setlinebuf(stdout);
    setlinebuf(stderr);
#endif

    /*initialize the file pointers, which are defined on the compile line*/
    if (t = (char *)getenv("SUP4MANDIR"))
	MANDIR = t;
    else
	MANDIR = MANLOC;

    if (t = (char *)getenv("SUP4KEYFILE"))
	KEYFILE = t;
    else
	KEYFILE = KEYLOC;

    if (t = (char *)getenv("SUP4MODELRC"))
	MODELFILE = t;
    else
	MODELFILE = MODELLOC;

    if (t = (char *)getenv("SUP4IMPDATA"))
	IMP_FILE = t;
    else
	IMP_FILE = IMPLOC;

    /* Parser initialization (verbose is in global.h, not in the parser) */
    strcpy( VersionString, "SUPREM-IV.GS B.9305");
    fprintf(stdout, "%s\n", VersionString);
    fprintf(stdout, "\t(c) 1991-1993 Stanford University\n");
    fprintf(stdout, "\t(c) 1991 University of Florida\n");
    fprintf(stdout, "Reading Models...\n");
    parser_boot( KEYFILE, "SUPREM4 ");
    verbose = V_CHAT;

    /*initialize the diffusion co_efficients and routine pointers*/
    diffuse_init();
#ifdef DEVICE
    device_init();
#endif
    vxmin = vymin = 0.0;	vxmax = vymax = 1.0;

    /*plot initialization*/
    xgPlotInit();

    /*some grid initialization*/
    rect_boot();

    /*plot initialization*/
    title = salloc(char, strlen(VERSION)+1);
    strcpy(title, VERSION);

    /*ready to start reading input files!*/

    /*read the modelrc file*/
    do_source(MODELFILE, NULL, FALSE, /*report errors*/ TRUE);

    /*read the .supremrc file*/
    strcpy(dot_name, (char *)getenv("HOME")); strcat(dot_name, "/.supremrc");
    do_source(dot_name, NULL, FALSE, /*report errors*/ FALSE);
    do_source(".supremrc", NULL, FALSE, /*report errors*/ FALSE);

    /*do any command line files*/
    for(i = 1; i < argc; i++) {
	do_source(argv[i], NULL, FALSE, /*report errors*/ TRUE );
    }

    /*main command parsing loop*/
    if ( argc < 2 )
	while ( (i = yyparse()) != -1) check_x();

    /*if we are logging cpu stats, close the file*/
    if (cpufile != NULL)
	fclose(cpufile);

    xgUpdate(1);

}






/************************************************************************
 *									*
 *	mode( par, param ) - This card controls the computation mode.	*
 *  It gets its card so that it doesn't get used by mistake.		*
 *									*
 *  Original:	MEL	10/84						*
 *									*
 ************************************************************************/
int mode_cmd( char *par, int param )
{
#   define CHOSEN(x) (is_specified( param, x) && get_bool( param, x))

    /*how much barfola the user wants*/
    if( CHOSEN("one.dim")) {
	set_dim( 1 );
    }
    if( CHOSEN("two.dim")) {
       set_dim( 2 );
    }

    return(0);
}

