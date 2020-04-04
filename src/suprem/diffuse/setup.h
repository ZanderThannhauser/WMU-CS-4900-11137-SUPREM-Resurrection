void time_val(int sol, double *rhsnm, struct call_str *cs);
void bval_compute(float temp, double **chem, double del);
void zero_setup(struct call_str *cs);
void setup_2d(int sol, float temp, int nsol, int *solve, double *inter,
			  double *vacan, double **chem, double **act, double **equil,
			  double *newarea, struct bound_str *bval, int bcnum);
void do_setup(double *rhs2);
void init_rhs(float temp, double **old, double **olda, double *oarea);
