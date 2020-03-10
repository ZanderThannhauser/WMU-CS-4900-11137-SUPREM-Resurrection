
#define NEL 7

int triox_stiff(double stiff[3 * NEL][3 * NEL], double wrhs[NEL][3],
                double xl[NEL][2], double disp[NEL][3], int (*coeff)(),
                int mat);

int triox_bc(double stiff[3 * NEL][3 * NEL], double wrhs[NEL][3],
             double xl[NEL][2], double disp[NEL][3], int (*coeff)(), int mat,
             int fixity[NEL][3], double bc[NEL][3], int ie, int fixme);

void triox_nodal_stress(double xl[NEL][2], double ul[NEL][3],
                        double epsn[NEL][3], double sign[NEL][3],
                        int (*coeff)(), int mat, double *xsj);

int oxload(double stiff[2 * 7][2 * 7], double wrhs[7][2], double xl[7][2],
           double disp[7][2], int (*coeff)(), int mat, int fixity[7][2],
           double bc[7][2], int ie, int fixme);
