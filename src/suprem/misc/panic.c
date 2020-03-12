
/*----------------------------------------------------------------------
 *
 * panic - remedial action attempted
 *
 * Copyright c 1985 The board of trustees of the Leland Stanford
 *                  Junior University. All rights reserved.
 * This subroutine may not be used outside of the SUPREM4 computer
 * program without the prior written consent of Stanford University.
 *
 * Original: CSR
 *---------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./src/suprem/include/constant.h"
#include "./src/suprem/include/diffuse.h"
#include "./src/suprem/include/geom.h"
#include "./src/suprem/include/global.h"
#include "./src/suprem/include/impurity.h"
#include "./src/suprem/include/material.h"
#include "./src/suprem/include/shell.h"
#include "./src/suprem/include/sysdep.h"

/* 2020 includes:*/
#include "./src/suprem/mesh/ig2_meshio.h"
#include "panic.h"
/* end of includes*/

void panic(char *s) {
    fprintf(stderr, "suprem4 panic: %s\n", s);
    if ((!strcmp(s, "nocore")) || (!strcmp(s, "Out of memory")))
        goto outtahere;

    fprintf(stderr, "Writing structure to /tmp/panic.str...");
    fflush(stderr);
    if ((ig2_write("/tmp/panic.str", 0, 1.0)) < 0)
        fprintf(stderr, "Failed\n");
    else
        fprintf(stderr, "Succeeded\n");

#ifdef SUPPORT
    fprintf(stderr, "Submitting bug report...");
    fflush(stderr);

    /* Let's try not to confuse the shell too badly. */
    /* People who run csh here lose big because it balks at quoted newlines */
    for (t = s; *t; t++)
        if (*t == '\'' || *t == '\\')
            *t = '#';
    sprintf(
        cmdbuf,
        "mail -s 'suprem4 panic: %s\nExecuting command: %s\nTotal time: %e' "
        "%s %s&",
        s, buffer, total, SUPPORT, (ier < 0) ? "</dev/null" : "<panic.str");

    if (system(cmdbuf))
        fprintf(stderr, "Failed\n");
    else
        fprintf(stderr, "Succeeded\n");
#endif
outtahere:

    fprintf(stderr, "Better luck next time.\n");
    fflush(stdout);
    fflush(stderr);
    abort();
}

/*-----------------GridSave---------------------------------------------
 * Store a valid grid before anything bad happens.
 *----------------------------------------------------------------------*/
void GridSave() {}
