
/*stolen from /usr/include/sys/times.h on my Linux machine:*/

typedef long int __clock_t;

typedef __clock_t clock_t;

struct tms
{
	clock_t tms_utime;
	clock_t tms_stime;

	clock_t tms_cutime;
	clock_t tms_cstime;
};

