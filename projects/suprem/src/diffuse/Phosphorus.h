void phosphorus(char *par, struct par_str *param);
void Pdiff_coeff(float temp, double **new, double **equil, double *noni,
				 double *idf, double *vdf, double *iprt, double *vprt);
void Pboundary(struct bound_str *bval);
void Pactive(int simple, int nn, float temp, double **conc, double **act,
			 double **equil, double *noni);
