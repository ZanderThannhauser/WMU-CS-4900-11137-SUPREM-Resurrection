void silicon(char *par, struct par_str *param);

void Sidiff_coeff(float temp, double **new, double **equil, double *noni,
                  double *idf, double *vdf, double *iprt, double *vprt);

void Siboundary(struct bound_str *bval);

void Siactive(int simple, int nn, float temp, double **conc, double **act,
              double **equil, double *noni);
