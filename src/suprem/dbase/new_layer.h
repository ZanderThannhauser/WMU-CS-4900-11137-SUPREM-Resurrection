void NativeOxide(double thick);
int dlocal_normal(int n, int matl, double dln[2]);

int new_layer(int Msil, int Mox, double oxide_grid, double dt);

int local_normal(int n, int matl, float ln[MAXDIM]);

float rough_perp(int p, int matl, float dir[MAXDIM]);
