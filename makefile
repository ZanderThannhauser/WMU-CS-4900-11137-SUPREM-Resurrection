

default: bin/suprem

CC = gcc

CPPFLAGS = -I . -I projects

CPPFLAGS += -D DEVICE
CPPFLAGS += -D NO_F77

CPPFLAGS += -D _XOPEN_SOURCE=500 # this brings in the needed stdlib functions

# Define which platform we're using. I made up a new one!
CPPFLAGS += -D UBUNTU

CFLAGS += -std=c90

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wfatal-errors
CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-array-bounds
#CFLAGS += -O2
CFLAGS += -g

DFLAGS += -D DEBUGGING_2020

LDLIBS += -lm

keyread-srclist.mk:
	find ./projects/keyread -name '*.c' | sort | sed 's/^/KYRD_SRCS += /' > $@
	
preprocessor-srclist.mk:
	find ./projects/preprocessor -name '*.c' | sort | sed 's/^/PPSR_SRCS += /' > $@
	
scraper-srclist.mk:
	find ./projects/scraper -name '*.c' | sort | sed 's/^/SCPR_SRCS += /' > $@
	
suprem-srclist.mk:
	find ./projects/suprem -name '*.c' | sort | sed 's/^/SPRM_SRCS += /' > $@

include keyread-srclist.mk
include preprocessor-srclist.mk
include scraper-srclist.mk
include suprem-srclist.mk

KYRD_OBJS = $(KYRD_SRCS:.c=.o)
KYRD_DOBJS = $(KYRD_SRCS:.c=.d.o)
KYRD_DEPENDS = $(KYRD_SRCS:.c=.mk)

PPSR_OBJS = $(PPSR_SRCS:.c=.o)
PPSR_DOBJS = $(PPSR_SRCS:.c=.d.o)
PPSR_DEPENDS = $(PPSR_SRCS:.c=.mk)

SCPR_OBJS = $(SCPR_SRCS:.c=.o)
SCPR_DOBJS = $(SCPR_SRCS:.c=.d.o)
SCPR_DEPENDS = $(SCPR_SRCS:.c=.mk)

SPRM_OBJS = $(SPRM_SRCS:.c=.o)
SPRM_DOBJS = $(SPRM_SRCS:.c=.d.o)
SPRM_DEPENDS = $(SPRM_SRCS:.c=.mk)


ARGS = ./projects/suprem/examples/exam1/boron.in
#ARGS = ./examples/exam3/oed.in
#ARGS = ./examples/exam4/oed.in
#ARGS = ./examples/exam5/whole.in
#ARGS = ./examples/exam7/fullrox.in
#ARGS = ./examples/exam9/sdep.in


run: bin/suprem data/suprem.uk
	./bin/suprem $(ARGS)

run.d: bin/suprem.d data/suprem.uk
	./bin/suprem.d $(ARGS)

valrun: bin/suprem data/suprem.uk
	valgrind ./bin/suprem $(ARGS)

valrun-stop: bin/suprem data/suprem.uk
	valgrind --gen-suppressions=yes ./bin/suprem $(ARGS)

valrun.d: bin/suprem.d data/suprem.uk
	valgrind ./bin/suprem.d $(ARGS)

valrun-stop.d: bin/suprem.d data/suprem.uk
	valgrind --gen-suppressions=yes ./bin/suprem.d $(ARGS)


bin/%:
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

bin/keyread: $(KYRD_OBJS) 

bin/keyread.d: $(KYRD_DOBJS) 

bin/preprocessor: $(PPSR_OBJS) 

bin/preprocessor.d: $(PPSR_DOBJS) 

bin/scraper: $(SCPR_OBJS) 

bin/scraper.d: $(SCPR_DOBJS) 

bin/suprem: $(SPRM_OBJS) 

bin/suprem.d: $(SPRM_DOBJS) 

data/suprem.uk: ./bin/keyread
	./bin/keyread ./data/suprem.uk < ./data/suprem.key


%.mk: %.c
	$(CPP) -MM -MT $@ $(CPPFLAGS) -MF $@ $< || (gedit $< && false)

%.h %.c: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	mv y.tab.h $*.h

%.o: %.c %.mk
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $< && false)

%.d.o: %.c  %.mk
	$(CC) -c $(DFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $< && false)


.PHONY: test open-all-suprem format clean-successes clean


#TESTCASES += ./examples/exam1/boron
#TESTCASES += ./examples/exam2/oed
#TESTCASES += ./examples/exam3/oed
###TESTCASES += ./examples/exam4/oed
##TESTCASES += ./examples/exam5/whole
##TESTCASES += ./examples/exam6/oxcalib
###TESTCASES += ./examples/exam7/fullrox # unreliable
##TESTCASES += ./examples/exam8/nit-stress
###TESTCASES += ./examples/exam9/sdep # unreliable
##TESTCASES += ./examples/exam10/example10
##TESTCASES += ./examples/exam11/example11
##TESTCASES += ./examples/exam12/example12
##TESTCASES += ./examples/exam13/example13
##TESTCASES += ./examples/exam14/example14
##TESTCASES += ./examples/exam15/example15
##TESTCASES += ./examples/exam16/example16
##TESTCASES += ./examples/exam17/example17
##TESTCASES += ./examples/gaas/example1
##TESTCASES += ./examples/gaas/example2
##TESTCASES += ./examples/gaas/example3
##TESTCASES += ./examples/gaas/example4
##TESTCASES += ./examples/gaas/example5
##TESTCASES += ./examples/gaas/example6
##TESTCASES += ./examples/gaas/example7
##TESTCASES += ./examples/gaas/example8

#TESTCASE_SUCCESSES = $(addsuffix .success,$(TESTCASES))

#%.success: ./bin/suprem data/suprem.uk %.in %.stdout.correct
#	./bin/suprem $*.in < /dev/null > $*.stdout.actual
#	cmp $*.stdout.actual $*.stdout.correct
#	touch $@

#%.success: ./bin/suprem data/suprem.uk %.in %.stdout.correct %.str.correct
#	./bin/suprem $*.in < /dev/null > $*.stdout.actual
#	cmp $*.stdout.actual $*.stdout.correct
#	cmp $*.str.actual $*.str.correct
#	touch $@

#test: $(TESTCASE_SUCCESSES)

format:
	find -name '*.c' -exec 'clang-format' '-i' '-verbose' '{}' \;
	find -name '*.h' -exec 'clang-format' '-i' '-verbose' '{}' \;

clean-successes:
	find -name '*.success' -exec 'rm' '-v' '{}' \;
	
clean:
	find -name '*.o' -exec 'rm' '-v' '{}' \;
	find -name '*.mk' -exec 'rm' '-v' '{}' \;
	find -executable -a -type f -exec 'rm' '-v' '{}' \;

include $(KYRD_DEPENDS)
include $(PPSR_DEPENDS)
include $(SCPR_DEPENDS)
include $(SPRM_DEPENDS)



















