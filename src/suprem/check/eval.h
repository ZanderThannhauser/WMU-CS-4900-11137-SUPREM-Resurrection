
#include "./src/suprem/include/expr.h"

void free_expr(struct vec_str *t);
char *eval_real(struct vec_str *expr, float *val);
char *eval_vec(struct vec_str *expr, float *val);
int islogexp(struct vec_str *expr);
