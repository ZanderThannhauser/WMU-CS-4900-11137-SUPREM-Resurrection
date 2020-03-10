

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

keyread.mk:
	find ./src/keyread -name '*.c' | sort | sed 's/^/KR_SRCS += /' > keyread.mk

scraper.mk:
	find ./src/scraper -name '*.c' | sort | sed 's/^/SCPR_SRCS += /' > scraper.mk

suprem.mk:
	find ./src/suprem -name '*.c' | sort | sed 's/^/SUP_SRCS += /' > suprem.mk

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

# --gen-suppressions=yes

ARGS = ./examples/exam1/boron.in # seems to work
#ARGS = ./examples/exam2/oed.in # seems to work
#ARGS = ./examples/exam3/oed.in # seems to work?
#ARGS = ./examples/exam4/oed.in # aborts
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

.PHONY: format clean

format:
	find -name '*.c' -exec 'clang-format' '-i' '-verbose' '{}' \;
	find -name '*.h' -exec 'clang-format' '-i' '-verbose' '{}' \;

clean:
	find -name '*.o' -exec 'rm' '-v' '{}' \;
	find -name '*.mk' -exec 'rm' '-v' '{}' \;
	find ! -name '*.sh' -a -executable -a -type f -exec 'rm' '-v' '{}' \;

include  $(KR_DEPENDS) $(SCPR_DEPENDS) $(SUP_DEPENDS)



















