int symfac(int n, int *sol, int nsol, int *ia, int aoff, int **pil, int *loff);
void dzero(double *a, int n);
void blkfac(int nv, int nsol, int *sol, int *il, double *l, int tloff);

void app_inv(int nv, int nsol, int *sol, int *il, int loff, double *l,
			 double *x[], double *ax[]);

void dcopy(double *a, double *b, int n);

void numbac(int n, int il[], int off, double l[], double rhs[]);
void numfac(int n, int *sol, int nsol, int ia[], int aoff, double *a, int *il,
			int loff, double l[]);
