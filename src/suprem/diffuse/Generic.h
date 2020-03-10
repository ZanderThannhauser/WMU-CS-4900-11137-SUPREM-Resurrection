void generic(char *par, struct par_str *param);
void Gdiff_coeff(float temp, double **new, double **equil, double *noni,
                 double *idf, double *vdf, double *iprt, double *vprt);
void Gboundary(struct bound_str *bva);
void Gactive(int simple, int nn, float temp, double **conc, double **act,
             double **equil, double *noni);
