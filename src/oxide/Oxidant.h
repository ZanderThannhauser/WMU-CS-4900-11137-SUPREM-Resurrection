
/*Macro up the argument list for repeated use*/
#define argh temp, new, equil, noni, idf, vdf, iprt, vprt
#define argl                                                                   \
    float temp, double **new, double **equil, double *noni, double *idf,       \
        double *vdf, double *iprt, double *vprt

#include "./include/impurity.h"

void FE_oxbulk(double *matco, int mat, double *eps, double *sig);
void ksubs(double *rval, double *sig, double *n, double *extra);
void ThermSig(float temp1, float temp2);
void AddIntSig();
void sup4_ecoeff(double *matco, int mat, double *eps, double *sig);
void O2diff_coeff(argl);
void O2boundary(struct bound_str *bval);
void H2Odiff_coeff(argl);
void H2Oboundary(struct bound_str *bval);
