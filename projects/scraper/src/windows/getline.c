
#ifdef WINDOWS

#include "debug.h"

#include "getline.h"

/*https://stackoverflow.com/questions/735126/are-there-alternate-implementations-of-gnu-getline-interface/735472#735472*/

ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
	char *bufptr = NULL;
	char *p = bufptr;
	size_t size;
	int c;
	ENTER;
	
	if (lineptr == NULL) {
		EXIT;
		return -1;
	}
	if (stream == NULL) {
		EXIT;
		return -1;
	}
	if (n == NULL) {
		EXIT;
		return -1;
	}
	bufptr = *lineptr;
	size = *n;

	c = fgetc(stream);
	if (c == EOF) {
		HERE;
		EXIT;
		return -1;
	}
	if (bufptr == NULL) {
		bufptr = malloc(128);
		if (bufptr == NULL) {
			EXIT;
			return -1;
		}
		size = 128;
	}
	p = bufptr;
	while (c != EOF) {
		if ((p - bufptr) > (size - 1)) {
			size = size + 128;
			bufptr = realloc(bufptr, size);
			if (bufptr == NULL) {
				EXIT;
				return -1;
			}
		}
		*p++ = c;
		if (c == '\n') {
			break;
		}
		c = fgetc(stream);
	}
	
	*p++ = '\0';
	*lineptr = bufptr;
	*n = size;
	
	EXIT;
	return p - bufptr - 1;
}

#endif

