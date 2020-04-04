void germanium(char *par, struct par_str *param);
void Gediff_coeff(float temp, double **new, double **equil, double *noni,
				  double *idf, double *vdf, double *iprt, double *vprt);
void Geboundary(struct bound_str *bval);
void Geactive(int simple, int nn, float temp, double **conc, double **act,
			  double **equil, double *noni);
