void arsenic(char *par, struct par_str *param);
void Asdiff_coeff(float temp, double **new, double **equil, double *noni,
				  double *idf, double *vdf, double *iprt, double *vprt);
void Asboundary(struct bound_str *bval);
void Asactive(int simple, int nn, float temp, double **conc, double **act,
			  double **equil, double *noni);
