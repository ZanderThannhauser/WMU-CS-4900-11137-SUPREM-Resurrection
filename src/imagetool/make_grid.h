
float interpolate(float x, float y, int tri_num, float *val);
int make_grid(int xsize, int ysize, float **xd, float **yd, float win_xmin,
              float win_xmax, float win_ymin, float win_ymax,
              int *win_xmin_index, int *win_xmax_index, int *win_ymin_index,
              int *win_ymax_index);
