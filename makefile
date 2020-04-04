

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
	find ./projects/keyread -name '*.c' | sed 's/^/KYRD_SRCS += /' > $@
	
preprocessor-srclist.mk:
	find ./projects/preprocessor -name '*.c' | sed 's/^/PPSR_SRCS += /' > $@
	
scraper-srclist.mk:
	find ./projects/scraper -name '*.c' | sed 's/^/SCPR_SRCS += /' > $@
	
suprem-srclist.mk:
	find ./projects/suprem -name '*.c' | sed 's/^/SPRM_SRCS += /' > $@

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


#ARGS += ./projects/suprem/system-tests/exam1/boron.in
#ARGS += ./projects/suprem/system-tests/exam2/oed.in
#ARGS += ./projects/suprem/system-tests/exam3/oed.in
ARGS += ./projects/suprem/system-tests/exam4/oed.in
#ARGS += ./projects/suprem/system-tests/exam5/sup2pis.in
#ARGS += ./projects/suprem/system-tests/exam5/whole.in
#ARGS += ./projects/suprem/system-tests/exam6/oxcalib.in
#ARGS += ./projects/suprem/system-tests/exam7/fullrox.in
#ARGS += ./projects/suprem/system-tests/exam8/nit-stress.in
#ARGS += ./projects/suprem/system-tests/exam9/sdep.in
#ARGS += ./projects/suprem/system-tests/exam10/example10.in
#ARGS += ./projects/suprem/system-tests/exam11/example11.in
#ARGS += ./projects/suprem/system-tests/exam12/example12.in
#ARGS += ./projects/suprem/system-tests/exam13/example13.in
#ARGS += ./projects/suprem/system-tests/exam14/example14.in
#ARGS += ./projects/suprem/system-tests/exam15/example15.in
#ARGS += ./projects/suprem/system-tests/exam16/example16.in
#ARGS += ./projects/suprem/system-tests/exam17/example17.in
#ARGS += ./projects/suprem/system-tests/gaas/example1.in
#ARGS += ./projects/suprem/system-tests/gaas/example2.in
#ARGS += ./projects/suprem/system-tests/gaas/example3.in
#ARGS += ./projects/suprem/system-tests/gaas/example4.in
#ARGS += ./projects/suprem/system-tests/gaas/example5.in
#ARGS += ./projects/suprem/system-tests/gaas/example6.in
#ARGS += ./projects/suprem/system-tests/gaas/example7.in
#ARGS += ./projects/suprem/system-tests/gaas/example8.in

run: bin/suprem data/suprem.uk
	./bin/suprem $(ARGS)

valrun: bin/suprem data/suprem.uk
	valgrind ./bin/suprem $(ARGS)

valrun-stop: bin/suprem data/suprem.uk
	valgrind --gen-suppressions=yes ./bin/suprem $(ARGS)

run.d: bin/suprem.d data/suprem.uk
	./bin/suprem.d $(ARGS)

valrun.d: bin/suprem.d data/suprem.uk
	valgrind ./bin/suprem.d $(ARGS)

valrun-stop.d: bin/suprem.d data/suprem.uk
	valgrind --gen-suppressions=yes ./bin/suprem.d $(ARGS)

bin/%:
	mkdir -p bin
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

bin/keyread: $(KYRD_OBJS) 
bin/keyread.d: $(KYRD_DOBJS) 
bin/preprocessor: $(PPSR_OBJS) 
bin/preprocessor.d: $(PPSR_DOBJS) 
bin/scraper: $(SCPR_OBJS) 
bin/scraper.d: $(SCPR_DOBJS) 
bin/suprem: $(SPRM_OBJS) 
bin/suprem.d: $(SPRM_DOBJS) 


data/suprem.uk: ./bin/keyread ./data/suprem.key
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
.PHONY: test-keyread test-preprocessor test-scraper test-suprem

keyread-systestlist.mk:
	find ./projects/keyread -name '*.in' \
		| sort | sed 's/^/KYRD_SYSTST += /' > $@
	
preprocessor-systestlist.mk:
	find ./projects/preprocessor -name '*.in' \
		| sort | sed 's/^/PPSR_SYSTST += /' > $@
	
scraper-systestlist.mk:
	find ./projects/scraper -name '*.in' \
		| sort | sed 's/^/SCPR_SYSTST += /' > $@
	
suprem-systestlist.mk:
	find ./projects/suprem -name '*.in' \
		| sort -V | sed 's/^/SPRM_SYSTST += /' > $@

include keyread-systestlist.mk
include preprocessor-systestlist.mk
include scraper-systestlist.mk
include suprem-systestlist.mk

KYRD_SYSTST_SUCCESSES = $(KYRD_SYSTST:.in=.success)
PPSR_SYSTST_SUCCESSES = $(PPSR_SYSTST:.in=.success)
SCPR_SYSTST_SUCCESSES = $(SCPR_SYSTST:.in=.success)
SPRM_SYSTST_SUCCESSES = $(SPRM_SYSTST:.in=.success)

$(KYRD_SYSTST_SUCCESSES): bin/keyread
$(PPSR_SYSTST_SUCCESSES): bin/preprocessor
$(SCPR_SYSTST_SUCCESSES): bin/scraper
$(SPRM_SYSTST_SUCCESSES): bin/suprem data/suprem.uk

%.success: %.in %.stdout.correct
	./bin/suprem $*.in < /dev/null > $*.stdout.actual
	cmp $*.stdout.actual $*.stdout.correct
	touch $@

#%.success: ./bin/suprem data/suprem.uk %.in %.stdout.correct %.str.correct
#	./bin/suprem $*.in < /dev/null > $*.stdout.actual
#	cmp $*.stdout.actual $*.stdout.correct
#	cmp $*.str.actual $*.str.correct
#	touch $@

systest-keyread: $(KYRD_SYSTST_SUCCESSES)
systest-preprocessor: $(PPSR_SYSTST_SUCCESSES)
systest-scraper: $(SCPR_SYSTST_SUCCESSES)
systest-suprem: $(SPRM_SYSTST_SUCCESSES)

systest: systest-keyread systest-preprocessor systest-scraper systest-suprem
#test: unit-test-keyread unit-test-preprocessor unit-test-scraper unit-test-suprem

test: systest

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



















