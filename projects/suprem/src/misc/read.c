/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *     Copyright c 1991 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of the University of 	 *
 *     Florida.								 *
 *									 *
 *									 *
 *************************************************************************/
/*   read.c                Version 5.1     */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suprem/include/check.h"
#include "suprem/include/global.h"
#include "suprem/include/shell.h"
#include "suprem/include/sysdep.h"

/* 2020 includes:*/
#include "debug.h"
#include "../misc/reader.h"
#include "../shell/proc.h"
#include "read.h"
/* end of includes*/

/************************************************************************
 *									*
 *	read_ukfile() - this routine opens up the file keyfile and 	*
 *  and calls the unformatted key file reader.  It will have to be more *
 *  intelligent than this about finding the unformatted key file.  It   *
 *  should probably be set up with a path global and have that added to *
 *  the start of the name.						*
 *									*
 *	Original	Mark E. Law		Oct, 1984		*
 *									*
 ************************************************************************/
int read_ukfile(char *name)
{
	ENTER;
	
	verpvs(name);
	
	FILE *fd;
	char *file;
	int   status;
	int   i, j;
	
	/*try several places to get the key file name*/
	if ((file = (char *)getenv("KEYFILE")) == NULL)
		/*use the system wide one*/
		file = name;
	
	verpvs(file);
	
	#ifdef LINUX
	fd = fopen(file, "r");
	#endif
	
	#ifdef WINDOWS
	fd = fopen(file, "rb");
	#endif
	
	verpv(fd);
	
	if (fd == NULL)
	{
		fprintf(stderr, "where is the unformatted key file???\n");
		return (-1);
	}
	
	status = read_list(cards - 1, fd);
	
	/*now match up the command names with the card names*/
	for (i = 0; cards[i] && strlen(cards[i]->name) != 0; i++)
	{
		j = get_proc(cards[i]->name);
		if (j != -1)
			command[j].param = i;
		else
			fprintf(stderr, "no command defined for name %s\n", cards[i]->name);
	}
	
	verpv(status);
	fclose(fd);
	EXIT;
	return (status);
}














