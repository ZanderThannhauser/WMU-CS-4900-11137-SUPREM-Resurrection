void poisson_block(int sol, float temp, int nsol, int *solve, double *coeff,
                   double **chem, double **nu1, double *nu2, double *nu3);
void electron_block(int sol, float nu1, int nu2, int *nu3, double *coeff,
                    double **chem, double **nu4, double *nu5, double *nu6);
void hole_block(int sol, float nu1, int nu2, int *nu3, double *coeff,
                double **chem, double **nu4, double *nu5, double *nu6);
