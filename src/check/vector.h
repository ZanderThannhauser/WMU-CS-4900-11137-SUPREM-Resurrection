int vec_func(char *s, struct tok_str *tok);
int constants(char *str, struct tok_str *tok);
int sol_values(char *s, struct tok_str *tok);
char *get_solval(float *val, int type);
char *vfunc(float *val, int type, struct vec_str *expr);
