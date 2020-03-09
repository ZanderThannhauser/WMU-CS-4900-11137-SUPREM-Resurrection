

default: bin/suprem

CC = gcc

CPPFLAGS = -I .

# I don't understand the need for these, I just know the original makefile had
# them:
CPPFLAGS += -D DEVICE
CPPFLAGS += -D NO_F77

# Define which platform we're using. I made up a new one!
CPPFLAGS += -D UBUNTU

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wfatal-errors
CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-array-bounds
CFLAGS += -O3

DFLAGS += -g
DFLAGS += -D DEBUGGING_2020

LDLIBS += -lm

srclist-suprem.mk:
	find ./src -name '*.c' ! -path './src/keyread/*' \
		| sort | sed 's/^/SUPSRCS += /' > srclist-suprem.mk

srclist-keyread.mk:
	find ./src/keyread -name '*.c' \
		| sort | sed 's/^/KRSRCS += /' > srclist-keyread.mk

include srclist-suprem.mk srclist-keyread.mk

SUPOBJS = $(SUPSRCS:.c=.o)
SUPDOBJS = $(SUPSRCS:.c=.d.o)
SUPDEPENDS = $(SUPSRCS:.c=.mk)

KROBJS = $(KRSRCS:.c=.o)
KRDOBJS = $(KRSRCS:.c=.d.o)
KRDEPENDS = $(KRSRCS:.c=.mk)

# --gen-suppressions=yes

#ARGS = ./examples/exam1/boron.in # seems to work
#ARGS = ./examples/exam2/oed.in # seems to work
#ARGS = ./examples/exam3/oed.in # seems to work?
ARGS = ./examples/exam4/oed.in # aborts
#ARGS = ./examples/exam5/whole.s4 # aborts
#ARGS = ./examples/exam6/oxcalib.s4 # aborts
#ARGS = ./examples/exam7/fullrox.s4 # aborts
#ARGS = ./examples/exam8/nit-stress.s4 # aborts
#ARGS = ./examples/exam9/sdep.s4 # aborts
#ARGS = ./examples/exam10/example10.in # seems to work
#ARGS = ./examples/exam11/example11.in # aborts
#ARGS = ./examples/exam12/example12.in # seems to work
#ARGS = ./examples/exam13/example13.in # seems to work
#ARGS = ./examples/exam14/example14.in # seems to work
#ARGS = ./examples/exam15/example15.in # seems to work
#ARGS = ./examples/exam16/example16.in # seems to work
#ARGS = ./examples/exam17/example17.in # seems to work

#ARGS = ./examples/gaas/example1 # seems to work
#ARGS = ./examples/gaas/example2 # aborts
#ARGS = ./examples/gaas/example3 # seems to work
#ARGS = ./examples/gaas/example4 # seems to work
#ARGS = ./examples/gaas/example5 # seems to work
#ARGS = ./examples/gaas/example6 # seems to work
#ARGS = ./examples/gaas/example7 # seems to work
#ARGS = ./examples/gaas/example8 # seems to work

run: bin/suprem data/suprem.uk
	./bin/suprem $(ARGS)

run.d: bin/suprem.d data/suprem.uk
	./bin/suprem.d $(ARGS)

valrun: bin/suprem.d data/suprem.uk
	valgrind --gen-suppressions=yes ./bin/suprem.d $(ARGS)

bin/suprem: $(SUPOBJS)
	$(CC) $(LDFLAGS) $(SUPOBJS) $(LOADLIBES) $(LDLIBS) -o $@

bin/suprem.d: $(SUPDOBJS)
	$(CC) $(LDFLAGS) $(SUPDOBJS) $(LOADLIBES) $(LDLIBS) -o $@

bin/keyread: $(KROBJS) 
	$(CC) $(LDFLAGS) $(KROBJS) $(LOADLIBES) $(LDLIBS) -o $@

bin/keyread.d: $(KRDOBJS) 
	$(CC) $(LDFLAGS) $(KRDOBJS) $(LOADLIBES) $(LDLIBS) -o $@

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

.PHONY: format clean

format:
	find -name '*.c' -exec 'clang-format' '-i' '-verbose' '{}' \;
	find -name '*.h' -exec 'clang-format' '-i' '-verbose' '{}' \;

clean:
	find -name '*.o' -exec 'rm' '-v' '{}' \;
	find -name '*.mk' -exec 'rm' '-v' '{}' \;
	find ! -name '*.sh' -a -executable -a -type f -exec 'rm' '-v' '{}' \;

include $(SUPDEPENDS) $(KRDEPENDS)



















