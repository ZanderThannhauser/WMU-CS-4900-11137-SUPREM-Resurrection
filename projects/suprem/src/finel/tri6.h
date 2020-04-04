
int tri6_stiff(double stiff[12][12], double wrhs[6][2], double xl[6][2],
			   double disp[6][2], int (*coeff)(), int mat);

void tri6_Snodal_stress(double xl[6][2], double ul[6][2], double epsn[6][3],
						double sign[6][3], int (*coeff)(), int mat,
						double *xsj);
