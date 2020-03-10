
#include "./src/suprem/misc/get.h"

void comp_intparam(float temp);

void Icoupling(float temp, double *area, double **new, double **equil,
               double **dequ, double **rhs);

void Idiff_coeff(float temp, double **new, double **equil, double *noni,
                 double *idf, double *vdf, double *iprt, double *vprt);

void Iboundary(struct bound_str *bval);

void Itime_val(int is, double *rhsnm, struct call_str *cs);

void interstitial(char *par, struct par_str *param);

float form_eval(char *expr, float total, float cord[2]);
