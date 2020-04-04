/*************************************************************************
 *									 *
 *   Original : MEL         Stanford University        Oct, 1984	 *
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   man.c                Version 5.1     */
/*   Last Modification : 7/3/91  08:41:09 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./src/suprem/include/sysdep.h"

/* 2020 includes:*/
#include "./src/debug.h"
#include "man.h"
/* end of includes*/

/* 2020 forward declarations*/
/* end of declarations*/

/************************************************************************
 *									*
 *	man( par, param ) - this routine reads the first thing 		*
 *  its input line and sees if it can find a helpfile by that name.  If *
 *  it does, it opens the file and pipes it to more.			*
 *									*
 *	Original	Mark E. Law		Oct. 16, 1984		*
 *									*
 *	Caution - This routine will have to be modified for non UNIX	*
 *  systems.								*
 *									*
 ************************************************************************/
void man(char *par, struct par_str *param) {
	FILE *help, *more;
	char *s;
	char filename[80];
	int c;
	char *dir;
	ENTER;

	/*figure out where to look for the manual directory*/
	if ((dir = (char *)getenv("MANDIR")) == NULL)
		dir = MANDIR;

	strcpy(filename, dir);
	strcat(filename, "/");

	if (par == NULL)
		strcat(filename, "suprem.h");
	else {
		/*skip over leading white space*/
		while (isspace(*par))
			par++;

		/*skip over non blanks*/
		for (s = par; (!isspace(*s) && (*s != '\0')); s++)
			;
		*s = '\0';

		strcat(filename, par);
		strcat(filename, ".h");
	}

	/*open a file to read from*/
	if ((help = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "can not find help for %s\n", par);
		EXIT;
		return;
	}

	/*open up the command more to write to*/
	more = (FILE *)popen(PAGER, "w");

	while ((c = getc(help)) != EOF)
		putc(c, more);

	fclose(help);
	pclose(more);
	EXIT;
	return;
}
