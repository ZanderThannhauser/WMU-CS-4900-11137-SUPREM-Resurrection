/*************************************************************************
 *									 *
 *   Original : MEL         Stanford University        Sept, 1984	 *
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   echo.c                Version 5.1     */
/*   Last Modification : 7/3/91  08:41:07 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./src/include/expr.h"
#include "./src/include/global.h"

// 2020 includes:
#include "./src/check/eval.h"
#include "./src/shell/do_action.h"
#include "./src/check/split.h"
#include "./src/check/parse.h"
#include "echo.h"
// end of includes

/************************************************************************
 *									*
 *	echo( par, param ) - this routine prints its parameter string.  *
 *									*
 ************************************************************************/
void echo(char *par, struct par_str *param) {
    char tmp[512];
    char *err;
    struct vec_str *out;
    float val;

    if (par == NULL) {
        fprintf(stdout, "\n");
        fflush(stdout);
        return;
    }

    /*strip the leading spaces*/
    while (isspace(*par))
        par++;

    /*check for a real number expression*/
    strcpy(tmp, par);

    if ((err = parse_expr(par, &out)) != NULL) {
        fprintf(stdout, "%s\n", tmp);
        fprintf(stdout, "%s\n", err);
    } else if ((err = eval_real(out, &val)) != NULL) {
        fprintf(stdout, "%s\n", tmp);
        fprintf(stdout, "%s\n", err);
    } else
        fprintf(stdout, "%g\n", val);

    fflush(stdout);
    if (out != NULL)
        free_expr(out);
}

/*-----------------uPause-----------------------------------------------
 * Wait before proceeding
 * Nice for reading someone else's input deck and checking it out on
 * the fly.
 *----------------------------------------------------------------------*/
void uPause(char *unused_0, struct par_str* unused_1) {
    char *CommandBuf;
    char *read_line();

    if (isatty(fileno(stdin))) {
        for (;;) {

            printf("Type <RETURN> to continue, or a command to be executed: ");
            fflush(stdout);

#ifdef HAVE_KSH
            s = read_line(NULL);
            CommandBuf = salloc(char, strlen(s) + 2);
            if (strlen(s) == 0)
                break;
            strcpy(CommandBuf, s);
            strcat(CommandBuf, "\n");
#else
            CommandBuf = salloc(char, 256);
            fgets(CommandBuf, 256, stdin);
            if (!CommandBuf[0])
                break;
#endif

            do_string(CommandBuf, NULL, 0);
            fflush(stdout);
            fflush(stderr);
        }
    }

    return; //(0);
}

/************************************************************************
 *									*
 *	user_print( par, param ) - this routine prints its parameter string.  *
 *									*
 ************************************************************************/
void user_print(char *par, struct par_str* param) {
    char *argv[50];
    char tmp[512];
    struct vec_str *out;
    float val;
    int i;

    if (par == NULL) {
        fprintf(stdout, "\n");
        fflush(stdout);
        return; // (0);
    }

    if (par != NULL)
        if (split(par, argv, FALSE) == -1)
            return; //(-1);

    for (i = 0; argv[i] != NULL; i++) {

        /*check for a real number expression*/
        strcpy(tmp, argv[i]);

        if ((parse_expr(tmp, &out)) != NULL)
            fprintf(stdout, "%s ", argv[i]);
        else if ((eval_real(out, &val)) != NULL)
            fprintf(stdout, "%s ", argv[i]);
        else
            fprintf(stdout, "%g ", val);

        if (out != NULL)
            free_expr(out);
        free(argv[i]);
    }
    fprintf(stdout, "\n");
    return; // (0);
}
