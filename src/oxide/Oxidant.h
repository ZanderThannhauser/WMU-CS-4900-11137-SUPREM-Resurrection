

// 2020: needed to bring this here to get the declarations to work:

#define argh temp, new, equil, noni, idf, vdf, iprt, vprt
#define argl                                                                   \
    float temp, double **new, double **equil, double *noni, double *idf,       \
        double *vdf, double *iprt, double *vprt

#include "./src/include/impurity.h"

void FE_oxbulk(double *matco, int mat, double *eps, double *sig);
void ksubs(double *rval, double *sig, double *n, double *extra);
void ThermSig(float temp1, float temp2);
void AddIntSig();
void sup4_ecoeff(double *matco, int mat, double *eps, double *sig);
void O2diff_coeff(argl);
void O2boundary(struct bound_str *bval);
void H2Odiff_coeff(argl);
void H2Oboundary(struct bound_str *bval);
float Ovel(float temp, int s, float conc, double *n, double noni);
float Oss(int s, int mat);
float Orr(float temp, int s, double *normal, double noni);
