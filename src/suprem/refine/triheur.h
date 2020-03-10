double sk_cross(struct sreg_str *r, struct LLedge *lp1, struct LLedge *lp2,
                int rect);
struct LLedge *quadsplit(struct sreg_str *rg);
struct LLedge *chop(struct sreg_str *r, int must);
int divide(struct sreg_str *r, struct LLedge **lep1, struct LLedge **lep2);
