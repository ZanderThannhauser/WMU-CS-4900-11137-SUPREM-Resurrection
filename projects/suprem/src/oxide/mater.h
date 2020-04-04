
#include "../misc/get.h"

void  c_mater(char *par, struct par_str *param);
void  SupToXtal(double *normal, double *vec3);
void  comp_mat(float temp);
int   ChosenMater(char *par, struct par_str *param, int other);
int   ChosenBC(char *par, struct par_str *param, int other);
float DoOriDep(double *dir, float valOri[3]);
