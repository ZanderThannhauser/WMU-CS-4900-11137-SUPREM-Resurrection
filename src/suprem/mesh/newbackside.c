
/*	newbackside.c		Version 5.1		*/
/*	Last Modification:	7/3/91 08:32:22		*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./src/suprem/include/constant.h"
#include "./src/suprem/include/geom.h"
#include "./src/suprem/include/implant.h"
#include "./src/suprem/include/shell.h"

/* 2020 includes:*/
#include "./src/suprem/shell/do_action.h"
#include "./src/suprem/geom/limits.h"
#include "newbackside.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/*  newbackside( line ) -  etch substrate at y=line and place a new
 *	backside contact at the new position for Pisces.  To prevent
 *	damage to the database, the structure will be saved in a file
 *	called suprem4temp.str.
 *
 *	Goodwin Chin
 *	Stanford University
 *	November 14, 1989
 */

int newbackside(float line) {
	char instr[300];
	int ie;
	float min_x;
	float max_x;
	float min_y;
	float max_y;
	float offset = 1.0;

	strcpy(instr, "structure outf=suprem4temp.str");
	do_string(instr, NULL, 0);

	/*    hack up grid here */
	dev_lmts(&min_x, &max_x, &min_y, &max_y);
	min_x *= 1.0e4;
	max_x *= 1.0e4;
	min_y *= 1.0e4;
	max_y *= 1.0e4;
	if (line < max_y) {

		snprintf(instr, 300, "etch silicon start x=%g y=%g", min_x - offset,
				 line);
		do_string(instr, "/dev/null", 0);

		snprintf(instr, 300, "etch silicon continue x=%g y=%g", max_x + offset,
				 line);
		do_string(instr, "/dev/null", 0);

		snprintf(instr, 300, "etch silicon continue x=%g y=%g", max_x + offset,
				 max_y + offset);
		do_string(instr, "/dev/null", 0);

		snprintf(instr, 300, "etch silicon done x=%g y=%g", min_x - offset,
				 max_y + offset);
		do_string(instr, "/dev/null", 0);

		strcpy(instr, "structure outf=etch.str");
		do_string(instr, "/dev/null", 0);

		/* now that we have the final structure, recompute the backside
		 *  boundary condition
		 */

		/* for now lets just walk through all the triangle.  it's inefficient
		 *	but should work
		 */
		for (ie = 0; ie < ne; ie++) {
			if (WITHIN(pt[nd[tri[ie]->nd[0]]->pt]->cord[1], line * 1.0e-4)) {
				if (WITHIN(pt[nd[tri[ie]->nd[1]]->pt]->cord[1], line * 1.0e-4))
					tri[ie]->nb[2] = BC_OFFSET + 1;
				else if (WITHIN(pt[nd[tri[ie]->nd[2]]->pt]->cord[1],
								line * 1.0e-4))
					tri[ie]->nb[1] = BC_OFFSET + 1;
			} else {
				if (WITHIN(pt[nd[tri[ie]->nd[1]]->pt]->cord[1], line * 1.0e-4))
					if (WITHIN(pt[nd[tri[ie]->nd[2]]->pt]->cord[1],
							   line * 1.0e-4))
						tri[ie]->nb[0] = BC_OFFSET + 1;
			}
		}
	}
	return (0);
}
