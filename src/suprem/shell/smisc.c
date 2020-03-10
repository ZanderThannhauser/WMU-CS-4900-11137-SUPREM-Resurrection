
/*----------------------------------------------------------------------
 *
 * smisc.c - easier interface to expression parser
 *
 * Copyright c 1985 The board of trustees of the Leland Stanford
 *                  Junior University. All rights reserved.
 * This subroutine may not be used outside of the SUPREM4 computer
 * program without the prior written consent of Stanford University.
 *
 * Original: CSR Thu Jul 16 16:47:28 1987
 *---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./src/suprem/include/expr.h"
#include "./src/suprem/include/global.h"
#include "./src/suprem/include/shell.h"

// 2020 includes:
#include "./src/suprem/shell/macro.h"
#include "./src/suprem/check/eval.h"
#include "./src/suprem/check/parse.h"
#include "smisc.h"
// end of includes

// 2020 forward declarations
// end of declarations

/*-----------------STRING_TO_REAL---------------------------------------
 * Hide everything from the innocent users
 *----------------------------------------------------------------------*/
float string_to_real(char *expr, float defval) {
    char *err, *dexpr;
    struct vec_str *rexp;
    int len;
    float val;

    /* First make a copy that we can write all over */
    len = 1 + strlen(expr);
    dexpr = salloc(char, len);
    strcpy(dexpr, expr);

    /* Expand macros */
    (void)expand_macro(&dexpr, &len, macro);

    /* Try to parse */
    if ((err = parse_expr(dexpr, &rexp)) || (err = eval_real(rexp, &val))) {
        fprintf(stderr, "bad expression: %s\n", err);
        val = defval;
    }
    free_expr(rexp);
    sfree(dexpr);
    return (val);
}

/*-----------------SMACRO, FMACRO, UMACRO-------------------------------
 * Define and undefine macros without including the whole expression parser.
 *----------------------------------------------------------------------*/

void smacro(char *name, char *def) {
    char buffer[512];
    sprintf(buffer, "%s %s", name, def);
    define_macro(buffer, &macro);
}

void fmacro(char *name, float val, char *format) {
    char buffer[512], formbuf[512];
    strcpy(formbuf, "%s ");
    strcat(formbuf, format);
    sprintf(buffer, formbuf, name, val);
    define_macro(buffer, &macro);
}

void umacro(char *name) { (void)undef_macro(name, &macro); }
