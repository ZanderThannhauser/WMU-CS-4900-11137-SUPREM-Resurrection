int  expand_macro(char **expand_str, int *len, struct macro_table *macro);
void define_macro(char *str, struct macro_table **macro);
int  undef_macro(char *name, struct macro_table **macro);
void dump_macro(struct macro_table *macro);
