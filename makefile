

default: bin/suprem

CC = gcc

CPPFLAGS = -I .

CPPFLAGS += -D DEVICE
CPPFLAGS += -D NO_F77

CPPFLAGS += -D _XOPEN_SOURCE=500 # this brings in the needed stdlib functions

# Define which platform we're using. I made up a new one!
CPPFLAGS += -D UBUNTU

CFLAGS += -std=c90
#CFLAGS += -std=c99

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wfatal-errors
CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-array-bounds
#CFLAGS += -O2

DFLAGS += -g
DFLAGS += -D DEBUGGING_2020

LDLIBS += -lm

keyread.mk:
	find ./src/keyread -name '*.c' | sort | sed 's/^/KR_SRCS += /' > $@

scraper.mk:
	find ./src/scraper -name '*.c' | sort | sed 's/^/SCPR_SRCS += /' > $@

suprem.mk:
	find ./src/suprem -name '*.c' | sort | sed 's/^/SUP_SRCS += /' > $@

include keyread.mk scraper.mk suprem.mk

KR_OBJS = $(KR_SRCS:.c=.o)
KR_DOBJS = $(KR_SRCS:.c=.d.o)
KR_DEPENDS = $(KR_SRCS:.c=.mk)

SCPR_OBJS = $(SCPR_SRCS:.c=.o)
SCPR_DOBJS = $(SCPR_SRCS:.c=.d.o)
SCPR_DEPENDS = $(SCPR_SRCS:.c=.mk)

SUP_OBJS = $(SUP_SRCS:.c=.o)
SUP_DOBJS = $(SUP_SRCS:.c=.d.o)
SUP_DEPENDS = $(SUP_SRCS:.c=.mk)

#ARGS = ./examples/exam1/boron.in
ARGS = ./examples/exam3/oed.in
#ARGS = ./examples/exam4/oed.in

run: bin/suprem data/suprem.uk
	./bin/suprem $(ARGS)

run.d: bin/suprem.d data/suprem.uk
	./bin/suprem.d $(ARGS)

valrun: bin/suprem.d data/suprem.uk
	valgrind ./bin/suprem.d $(ARGS)

valrun-stop: bin/suprem.d data/suprem.uk
	valgrind --gen-suppressions=yes ./bin/suprem.d $(ARGS)

TESTCASES += ./examples/exam1/boron
TESTCASES += ./examples/exam2/oed
TESTCASES += ./examples/exam3/oed
#TESTCASES += ./examples/exam4/oed # unreliable
#TESTCASES += ./examples/exam5/whole # unreliable
TESTCASES += ./examples/exam6/oxcalib
#TESTCASES += ./examples/exam7/fullrox # unreliable
TESTCASES += ./examples/exam8/nit-stress
#TESTCASES += ./examples/exam9/sdep # unreliable
TESTCASES += ./examples/exam10/example10
TESTCASES += ./examples/exam11/example11
TESTCASES += ./examples/exam12/example12
TESTCASES += ./examples/exam13/example13
TESTCASES += ./examples/exam14/example14
TESTCASES += ./examples/exam15/example15
TESTCASES += ./examples/exam16/example16
TESTCASES += ./examples/exam17/example17
TESTCASES += ./examples/gaas/example1
TESTCASES += ./examples/gaas/example2
TESTCASES += ./examples/gaas/example3
TESTCASES += ./examples/gaas/example4
TESTCASES += ./examples/gaas/example5
TESTCASES += ./examples/gaas/example6
TESTCASES += ./examples/gaas/example7
TESTCASES += ./examples/gaas/example8

TESTCASE_SUCCESSES = $(addsuffix .success,$(TESTCASES))

%.success: ./bin/suprem data/suprem.uk %.in %.stdout.correct
	./bin/suprem $*.in < /dev/null > $*.stdout.actual
	cmp $*.stdout.actual $*.stdout.correct
	touch $@

%.success: ./bin/suprem data/suprem.uk %.in %.stdout.correct %.str.correct
	./bin/suprem $*.in < /dev/null > $*.stdout.actual
	cmp $*.stdout.actual $*.stdout.correct
	cmp $*.str.actual $*.str.correct
	touch $@

test: $(TESTCASE_SUCCESSES)

#all: bin/keyread bin/keyread.d bin/scraper bin/scraper.d bin/suprem bin/suprem.d

bin/keyread: $(KR_OBJS) 
	$(CC) $(LDFLAGS) $(KR_OBJS) $(LOADLIBES) $(LDLIBS) -o $@

bin/keyread.d: $(KR_DOBJS) 
	$(CC) $(LDFLAGS) $(KR_DOBJS) $(LOADLIBES) $(LDLIBS) -o $@

bin/scraper: $(SCPR_OBJS) 
	$(CC) $(LDFLAGS) $(SCPR_OBJS) $(LOADLIBES) $(LDLIBS) -o $@

bin/scraper.d: $(SCPR_DOBJS) 
	$(CC) $(LDFLAGS) $(SCPR_DOBJS) $(LOADLIBES) $(LDLIBS) -o $@

bin/suprem: $(SUP_OBJS)
	$(CC) $(LDFLAGS) $(SUP_OBJS) $(LOADLIBES) $(LDLIBS) -o $@

bin/suprem.d: $(SUP_DOBJS)
	$(CC) $(LDFLAGS) $(SUP_DOBJS) $(LOADLIBES) $(LDLIBS) -o $@

data/suprem.uk: bin/keyread
	./bin/keyread data/suprem.uk < data/suprem.key

%.mk: %.c
	$(CPP) -MM -MT $@ $(CPPFLAGS) -MF $@ $< || (gedit $< && false)

%.h %.c: %.y
	$(YACC) $(YFLAGS) -d $< || (gedit $< && false)
	mv y.tab.c $*.c
	mv y.tab.h $*.h

%.o: %.c %.mk
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $< && false)

%.d.o: %.c  %.mk
	$(CC) -c $(DFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $< && false)

.PHONY: open-all format clean-successes clean

open-all:
	find -name '*.c' -exec 'gedit' '{}' \;
	find -name '*.h' -exec 'gedit' '{}' \;

format:
	find -name '*.c' -exec 'clang-format' '-i' '-verbose' '{}' \;
	find -name '*.h' -exec 'clang-format' '-i' '-verbose' '{}' \;

clean-successes:
	find -name '*.success' -exec 'rm' '-v' '{}' \;
	
clean:
	find -name '*.o' -exec 'rm' '-v' '{}' \;
	find -name '*.mk' -exec 'rm' '-v' '{}' \;
	find ! -name '*.sh' -a -executable -a -type f -exec 'rm' '-v' '{}' \;

include  $(KR_DEPENDS) $(SCPR_DEPENDS) $(SUP_DEPENDS)



















