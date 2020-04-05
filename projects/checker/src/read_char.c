
#include <unistd.h>
#include <stdio.h>

#include "debug.h"

#include "error_codes.h"
#include "read_char.h"

int read_char(
	int fd,
	char* current_char)
{
	int error = 0, read_ret;
	ENTER;
	
	read_ret = read(fd, current_char, 1);
	if(read_ret > 0);
	else if(read_ret == 0)
		error = e_read_char_EOF;
	else if(read_ret < 0)
		perror("read"),
		error = e_systemcall_failed;
	
	EXIT;
	return error;
}
