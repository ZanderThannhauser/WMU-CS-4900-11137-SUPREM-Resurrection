void carbon(char *par, struct par_str *param);
void Cdiff_coeff(float temp, double **new, double **equil, double *noni,
				 double *idf, double *vdf, double *iprt, double *vprt);
void Cboundary(struct bound_str *bval);
void Cactive(int simple, int nn, float temp, double **conc, double **act,
			 double **equil, double *noni);
