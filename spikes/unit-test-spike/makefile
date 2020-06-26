
CC = gcc

CFLAGS += -Wall -Werror -g

default: bin/main

run: bin/main
	bin/main

bin:
	mkdir bin

srclist.mk:
	find src -name '*.c' | sed 's/^/SRCS += /' > $@

include srclist.mk

testlist.mk:
	find unit-tests -name '*.c' | sed 's/^/TESTS += /' > $@

include testlist.mk

OBJS      = $(SRCS:.c=.o)
DEPENDS   = $(SRCS:.c=.mk) $(TESTS:.c=.mk)
SUCCESSES = $(TESTS:.c=.success)

%.mk: %.c
	$(CPP) $(CPPFLAGS) -MM -MT $@ -MF $@ $<

include $(DEPENDS)

bin/%: unit-tests/%.o src/%.o | bin
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

unit-tests/%.success: bin/%
	valgrind --leak-check=full --error-exitcode=1 $<
	touch $@

test: $(SUCCESSES)

%.o: %.c %.mk
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

bin/main: $(OBJS) | bin
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	find -name '*.o' -delete
	rm bin -rf

clean-successes:
	find -name '*.success' -delete

















