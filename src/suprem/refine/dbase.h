int cr_sreg(int mat);
void ad_edge(int ir, int ie, struct LLedge *lep, int iscc, int pos);
void triple_fix(int p, int p2);
void trip_tri(int p, int *t1, int *m1, int *t2, int *m2);
int cr_tri(int rn, int ir);
void add_ang(struct sreg_str *r, struct LLedge *lep);
struct LLedge *eindex(int ir, int ie);
