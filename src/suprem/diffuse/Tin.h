void Sndiff_coeff(float temp, double **new, double **equil, double *noni,
				  double *idf, double *vdf, double *iprt, double *vprt);
void Snboundary(struct bound_str *bval);
void Snactive(int simple, int nn, float temp, double **conc, double **act,
			  double **equil, double *noni);
void tin(char *par, struct par_str *param);
