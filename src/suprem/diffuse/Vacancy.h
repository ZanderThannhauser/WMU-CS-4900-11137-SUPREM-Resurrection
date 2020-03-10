void comp_vacparam(float temp);
void Vcoupling(float temp, double *area, double **new, double **equil,
               double **dequ, double **rhs);
void Vdiff_coeff(float temp, double **new, double **equil, double *noni,
                 double *idf, double *vdf, double *iprt, double *vprt);
void Vboundary(struct bound_str *bval);
void Vtime_val(int vs, double *rhsnm, struct call_str *cs);
void vacancy(char *par, struct par_str *param);
