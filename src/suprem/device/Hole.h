void Hboundary(struct bound_str *bval);
void Hcoupling(float temp, double *area, double **new, double **equil,
               double **dequ, double **rhs);
double Hmobil();
double Phi_p(double hole, double psi, double ev, int mat, double *dqdp,
             double *dqdh);
