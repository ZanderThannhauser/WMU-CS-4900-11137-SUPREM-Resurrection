
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int open_retval;
	int write_retval;
	
	open_retval = _open("abc", _O_CREAT | _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE);
	
	printf("open_retval == %i\n", open_retval);
	
	write_retval = write(open_retval, "hello!", 6);
	
	printf("write_retval == %i\n", write_retval);
	
	return 0;
}
