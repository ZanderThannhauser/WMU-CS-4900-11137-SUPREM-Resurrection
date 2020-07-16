
#ifdef WINDOWS

#include <assert.h>
#include <time.h>

#include "debug.h"

#include "struct-tms.h"
#include "times.h"

// 2020: printing times has been commented out, so this implementation doesn't
// 2020: really matter that much.

clock_t times (struct tms *__buffer)
{
	ENTER;
	__buffer->tms_utime = clock();
	__buffer->tms_stime = clock();
	__buffer->tms_cutime = 0;
	__buffer->tms_cstime = 0;
	EXIT;
	// 2020: times()  returns  the  number of clock ticks that have elapsed
	// 2020: since an arbitrary point in the past.
	return 3;
}

#endif
