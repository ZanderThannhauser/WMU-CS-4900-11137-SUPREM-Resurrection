
/*stolen from /usr/include/sys/times.h on my Linux machine:*/

#include "clock_t.h"

struct tms
{
	clock_t tms_utime;
	clock_t tms_stime;

	clock_t tms_cutime;
	clock_t tms_cstime;
};

