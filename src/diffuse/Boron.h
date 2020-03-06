void boron(char *par, struct par_str *param);
void Bdiff_coeff(float temp, double **new, double **equil, double *noni,
                 double *idf, double *vdf, double *iprt, double *vprt);
void Bboundary(struct bound_str *bval);
void Gaboundary(struct bound_str *bval);

void Gaactive(int simple, int nn, float temp, double **chem, double **act,
              double **equil, double *noni);

void Bactive(int simple, int nn, float temp, double **chem, double **act,
             double **equil, double *noni);
