void Zndiff_coeff(float temp, double **new, double **equil, double *noni,
				  double *idf, double *vdf, double *iprt, double *vprt);
void Znboundary(struct bound_str *bval);
void Znactive(int simple, int nn, float temp, double **conc, double **act,
			  double **equil, double *noni);
void zinc(char *par, struct par_str *param);
