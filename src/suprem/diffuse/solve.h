int soldif(int init, char *label, void (*do_setup)(double *));
void steady_state(float temp, int nsol, int *sol, double **new,
				  double *newarea);

int soldif_tr(double del_t, float temp, double **old, double **new,
			  double **olda, double **newa, double *oldarea, double *newarea,
			  int init);

int soldif_bdf(double del_t, double old_t, float temp, double **new,
			   double **mid, double **old, double **newa, double **mida,
			   double **olda, double *newarea, double *midarea, double *oldarea,
			   int init);
