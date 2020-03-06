
void magnesium(char *par, struct par_str *param);

void Mgdiff_coeff(float temp, double **new, double **equil, double *noni,
                  double *idf, double *vdf, double *iprt, double *vprt);
void Mgboundary(struct bound_str *bval);
void Mgactive(int simple, int nn, float temp, double **conc, double **act,
              double **equil, double *noni);
