
extern int debugging_depth;

#ifndef DEBUGGING_2020
#define DEBUGGING_2020 (0)
#endif

#if DEBUGGING_2020
	#include <assert.h>
	#include <stdio.h>
	#include <unistd.h>

	#define D_2020(...) __VA_ARGS__
	#define ND_2020(...)

	#define TODO \
	{\
		printf("%*sTODO: File: %s, Line: %i\n", debugging_depth, "", \
			__FILE__, __LINE__);\
		char buffer[100];\
		sprintf(buffer, "+%i", __LINE__);\
		execlp("gedit", "gedit", __FILE__, buffer, NULL);\
		assert(0);\
	}
	
	#define CHECK TODO
	#define NOPE CHECK
	#define HERE \
		printf("%*sHERE: File: %s, Line: %i\n", debugging_depth, "", \
			__FILE__, __LINE__);
	
#else
	#define D_2020(...)
	#define ND_2020(...) __VA_ARGS__
	
	#define TODO assert(!"TODO");
	#define CHECK assert(!"CHECK");
	#define NOPE assert(!"NOPE");
	#define HERE ;

#endif

#define ENTER                                                                  \
	D_2020(printf("%*s<%s>\n", debugging_depth, "", __PRETTY_FUNCTION__),      \
	  debugging_depth++);

#define EXIT                                                                   \
	D_2020(debugging_depth--,                                                  \
	  printf("%*s</%s>\n", debugging_depth, "", __PRETTY_FUNCTION__));

#define verpv(val) dprint(val)

#define dprint(val) D_2020(print(val))

#define verprintf(...)                                                         \
	D_2020(printf("%*s", debugging_depth, ""), printf(__VA_ARGS__))

#define verpvb(b)                                                              \
	D_2020(printf("%*s" #b " == %s\n", debugging_depth, "",                    \
			 (b) ? "true" : "false"))

#define verpvc(ch)                                                             \
	D_2020(printf("%*s" #ch " == '%c'\n", debugging_depth, "", ch))

#define verpvs(str)                                                            \
	D_2020(printf("%*s" #str " == \"%s\"\n", debugging_depth, "", str))

#define verpvsn(str, len)                                                      \
	D_2020(printf("%*s" #str " == \"%.*s\"\n", debugging_depth, "",            \
			 (int)(len), str))

#define print(val)                                                             \
	printf("%*s" #val " == ", debugging_depth, ""),                       \
		printf((_Generic(val, char                                             \
						 : "%%c: %i\n", signed char                            \
						 : "%%uc: %u\n", unsigned char                         \
						 : "%%uc: %u\n", signed short                          \
						 : "%%ss: %i\n", unsigned short                        \
						 : "%%us: %u\n", signed int                            \
						 : "%%si: %i\n", unsigned int                          \
						 : "%%ui: %u\n", signed long                           \
						 : "%%sl: %li\n", unsigned long                        \
						 : "%%ul: %lu\n", float                                \
						 : "%%f: %f\n", double                                 \
						 : "%%lf: %lf\n", default                              \
						 : "%%p: %p\n")), val)                                 \








