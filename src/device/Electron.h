
double Nmobil();

void Ncoupling(float temp, double *area, double **new, double **equil,
               double **dequ, double **rhs);

void Nboundary(struct bound_str *bval);

void electron(char *par, struct par_str *param);
