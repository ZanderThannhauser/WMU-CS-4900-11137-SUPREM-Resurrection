void selenium(char *par, struct par_str *param);

void Sediff_coeff(float temp, double **new, double **equil, double *noni,
				  double *idf, double *vdf, double *iprt, double *vprt);

void Seboundary(struct bound_str *bval);

void Seactive(int simple, int nn, float temp, double **conc, double **act,
			  double **equil, double *noni);
