
#include "./src/suprem/misc/get.h"

double Nmobil();

void Ncoupling(float temp, double *area, double **new, double **equil,
			   double **dequ, double **rhs);

void Nboundary(struct bound_str *bval);

void electron(char *par, struct par_str *param);

double Phi_n(double elec, double psi, double ec, int mat, double *dqdp,
			 double *dqde);
