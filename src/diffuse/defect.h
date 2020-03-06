void init_pseudo(double temp);
void get_defaults(int imp);
void get_Cstar(int imp, int nv, float temp, double *noni, double *equil,
               double *dequ);

void IVblock_set(int ss, float temp, int nsol, int *solve, double *area,
                 double **chem, double **sub, double *inter, double *vacan);
void defect_block(int sol, float temp, double **chem, double *inter,
                  double *vacan, double *area);
