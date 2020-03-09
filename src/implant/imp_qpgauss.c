/* date: 5 jan 87 (mel)
 *
 * "qpgauss" :	return area under a gaussian curve
 *
 * written: Mark E. Law (jan 87)
 *
 * imp_qpgauss.c	5.1	7/3/91	12:08:41
 */

#include <math.h>

#define abs(V) ((V) < 0. ? -(V) : (V))

#include "./src/include/constant.h"
#include "./src/include/global.h"
#include "./src/include/implant.h"

// 2020 includes:
#include "./src/implant/imp_vert.h"
#include "imp_qpgauss.h"
// end of includes

// 2020 forward declarations
// end of declarations

void qpgauss(double dx, struct imp_info *cvals) {

    double x;
    double val = 1, v0;
    double qp = 0.0;

    /* --- start ---- */

    /*integrate from zero down to the bottom of the profile*/
    v0 = imp_vert(0.0, cvals);
    for (x = dx; (val > 1e-36) || (x < cvals->vert[GUS_RP]); x += dx) {
        val = imp_vert(x, cvals);
        qp += val + v0;
        v0 = val;
    }

    /* The area under the Pearson. Also convert units since conc is
     *	atoms/cm^2  and dx is in microns.
     */
    cvals->area = 0.5 * qp * dx * 1e-4; /* The total area */
    cvals->maxz = x;
}
