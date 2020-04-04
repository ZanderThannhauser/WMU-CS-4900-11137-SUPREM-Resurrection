void tri7_nodal_stress(double xl[7][2], double ul[7][2], double epsn[7][3],
					   double sign[7][3], int (*coeff)(), int mat, double *xsj);
int  tri7_stiff(double stiff[14][14], double wrhs[7][2], double xl[7][2],
				double disp[7][2], int (*coeff)(), int mat);
