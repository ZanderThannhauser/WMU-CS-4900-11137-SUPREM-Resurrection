void FEsolve(int verbose);
double FEupdnorm(double *delta);
double FErhsnorm(double *a, double *rhs);
int FEconnect(int me, short *taken, int *num);
