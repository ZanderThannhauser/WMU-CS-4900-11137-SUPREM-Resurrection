
void chrg_block_set(int ss, int sol, double *df, double *dprt, double *val,
					double *lval, double *sqval, double *dval[MAXIMP],
					double *pot, double spot);
void BAsblock_set(int ss, float temp, int nsol, int *solve, double *cpl,
				  double **chem, double **sub, double *inter, double *vacan);
void neut_block_set(int sol, float temp, int nu1, int *nu2, double *area,
					double **chem, double **nu3, double *nu4, double *nu5);
void PSbblock_set(int ss, float temp, int nsol, int *solve, double *cpl,
				  double **chem, double **sub, double *inter, double *vacan);
