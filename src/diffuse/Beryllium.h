void beryllium(char *par, struct par_str *param);
void Bediff_coeff(float temp, double **new, double **equil, double *noni,
                  double *idf, double *vdf, double *iprt, double *vprt);
void Beboundary(struct bound_str *bval);
void Beactive(int simple, int nn, float temp, double **conc, double **act,
              double **equil, double *noni);
