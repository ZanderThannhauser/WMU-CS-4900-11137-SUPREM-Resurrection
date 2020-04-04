/* date: 05 jul 85 (mje)
 *
 * "qeqv" : Find the equivalent dose through a given depth.
 *
 * calling sequence:
 *	double
 *	qeqv(xdep, dx, kp)
 *
 * where:
 *	<return>	- (double) Dose through 'xdep'.
 *	xdep		- (double) Depth through which to calculate dose.
 *	dx		- (double) Delta x to use in calculations.
 *	kp		- (double []) Pearson constants.
 *
 * written: Michael Eldredge (apr 85)
 *
 * imp_qeqv.c	5.1	7/3/91	12:08:40
 */

#include <math.h>

#define abs(V) ((V) < 0. ? -(V) : (V))

/* Offsets to the Pearson-IV constants array */
#include "./src/suprem/include/constant.h"
#include "./src/suprem/include/global.h"
#include "./src/suprem/include/implant.h"

/* 2020 includes:*/
#include "./src/debug.h"
#include "./src/suprem/implant/imp_vert.h"
#include "imp_qeqv.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

double qeqv(double xst, double xen, double qtot, double dx,
			struct imp_info *data) {
	double x, val, ov;
	double q = 0.0;

	/*integrate from x = 0 until we get the correct dose*/
	ov = imp_vert(xst, data);

	for (x = xst + dx; (x < xen - dx) && (x < data->maxz); x += dx) {
		val = imp_vert(x, data);
		q += 0.5 * (val + ov) * dx;
		ov = val;
	}

	/*add the excess for the last bit*/
	if (x > (xen - dx)) {
		val = imp_vert(xen, data);
		q += 0.5 * (val + ov) * (xen - x);
	}

	q = q * 1e-4;
	return (q * qtot / data->area);
}
