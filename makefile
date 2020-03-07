

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
#CFLAGS += -O3 # Might have to comment this out

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

run: bin/suprem data/suprem.uk
	./bin/suprem

test-exam1: bin/suprem data/suprem.uk
	valgrind ./bin/suprem < ./examples/exam1/boron.in

test-exam2: bin/suprem data/suprem.uk
	valgrind ./bin/suprem < ./examples/exam2/oed.in

test-exam3: bin/suprem data/suprem.uk
	valgrind ./bin/suprem < ./examples/exam3/oed.in

test-exam4: bin/suprem data/suprem.uk
	valgrind ./bin/suprem < ./examples/exam4/oed.in

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

.PHONY: clean

clean:
	find -name '*.o' -exec 'rm' '-v' '{}' \;
	find -name '*.mk' -exec 'rm' '-v' '{}' \;
	find ! -name '*.sh' -a -executable -a -type f  -exec 'rm' '-v' '{}' \;

include $(SUPDEPENDS) $(KRDEPENDS)



















