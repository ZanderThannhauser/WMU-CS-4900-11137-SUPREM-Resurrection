void antimony(char *par, struct par_str *param);
void Sbdiff_coeff(float temp, double **new, double **equil, double *noni,
				  double *idf, double *vdf, double *iprt, double *vprt);
void Sbboundary(struct bound_str *bval);
void Sbactive(int simple, int nn, float temp, double **chem, double **act,
			  double **equil, double *noni);
