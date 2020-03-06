
extern int depth;

#ifndef DEBUGGING_2020
	#define DEBUGGING_2020 (0)
#endif

#if DEBUGGING_2020

	#include <assert.h>
	#include <stdio.h>
	
	#define D(...) __VA_ARGS__
	#define ND(...)
#else
	#define D(...)
	#define ND(...) __VA_ARGS__
#endif

#define TODO assert(!"TODO");

#define CHECK assert(!"CHECK");

#define NOPE assert(!"NOPE");

#define HERE D(printf("%*sHERE: File: %s, Line: %i\n",\
	depth, "", __FILE__, __LINE__));

#define ENTER D(printf("%*s<%s>\n", depth, "", __PRETTY_FUNCTION__), depth++);

#define EXIT  D(depth--, printf("%*s</%s>\n", depth, "", __PRETTY_FUNCTION__));

#define verpv(val) dprint(val)

#define dprint(val) D(print(val))

#define verprintf(...) \
	D(printf("%*s", depth, ""), printf(__VA_ARGS__))

#define verpvb(b) \
	D(printf("%*s" #b " == %s\n", depth, "", (b) ? "true" : "false"))

#define verpvc(ch) \
	D(printf("%*s" #ch " == '%c'\n", depth, "", ch))

#define verpvs(str) \
	D(printf("%*s" #str " == \"%s\"\n", depth, "", str))

#define verpvsn(str, len) \
	D(printf("%*s" #str " == \"%.*s\"\n", depth, "", (int) (len), str))

#define print(val) \
	printf("%*s" #val " == ", depth, ""), printf((_Generic(val, \
		char: "%%c: %i\n", \
		signed char: "%%uc: %u\n", \
		unsigned char: "%%uc: %u\n", \
		signed short: "%%ss: %i\n", \
		unsigned short: "%%us: %u\n", \
		signed int: "%%si: %i\n", \
		unsigned int: "%%ui: %u\n", \
		signed long: "%%sl: %li\n", \
		unsigned long: "%%ul: %lu\n", \
		float: "%%f: %f\n", \
		double: "%%lf: %lf\n", \
		default: "%%p: %p\n")), val)

#define printbool(val)\
	printf(#val " == %s\n", (val) ? "true" : "false")

#define printchar(val)\
	printf(#val " == '%c'\n", val)

#define printstr(val)\
	printf(#val " == \"%s\"\n", val)





