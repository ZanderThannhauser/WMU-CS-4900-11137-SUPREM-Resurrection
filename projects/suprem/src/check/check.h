
#include "suprem/include/check.h"

val_str bool_check(struct bool_exp *bexp, int *typ);
void	assign_deflt(struct par_str **par);
void	deflt_check(struct par_str **par);
int		error_check(struct par_str **par, int bool);
