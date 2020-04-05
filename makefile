

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
#CFLAGS += -Wfatal-errors
CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-array-bounds
#CFLAGS += -O2
CFLAGS += -g

DFLAGS += -D DEBUGGING_2020

LDLIBS += -lm

.%-srclist.mk:
	find ./projects/$* -name '*.c' | sed 's/^/'$*'_src += /' > $@

include .checker-srclist.mk
include .keyread-srclist.mk
include .preprocessor-srclist.mk
include .scraper-srclist.mk
include .suprem-srclist.mk

.%-othervars.mk:
	echo > $@
	echo $*'_objs = $$('$*'_src:.c=.o)' >> $@
	echo $*'_dobjs = $$('$*'_src:.c=.d.o)' >> $@
	echo $*'_depends = $$('$*'_src:.c=.mk)' >> $@

include .checker-othervars.mk
include .keyread-othervars.mk
include .preprocessor-othervars.mk
include .scraper-othervars.mk
include .suprem-othervars.mk

#ARGS += ./projects/suprem/system-tests/exam1/boron.in
#ARGS += ./projects/suprem/system-tests/exam2/oed.in
#ARGS += ./projects/suprem/system-tests/exam3/oed.in
#ARGS += ./projects/suprem/system-tests/exam4/oed.in
#ARGS += ./projects/suprem/system-tests/exam5/sup2pis.in
ARGS += ./projects/suprem/system-tests/exam5/whole.in
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

.%-executable.mk:
	echo > $@
	echo 'bin/'$*': $$('$*'_objs)' >> $@
	echo 'bin/'$*'.d: $$('$*'_dobjs)' >> $@

include .checker-executable.mk
include .keyread-executable.mk
include .preprocessor-executable.mk
include .scraper-executable.mk
include .suprem-executable.mk

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

.%-systestlist.mk:
	find ./projects/$* -name '*.stdin' | sort | sed 's/^/'$*'_systests += /' > $@

include .checker-systestlist.mk
include .keyread-systestlist.mk
include .preprocessor-systestlist.mk
include .scraper-systestlist.mk
include .suprem-systestlist.mk

%.success: bin/checker %.flags %.stdin \
	%.stdout.correct %.stderr.correct %.exit-code.correct
	xargs -a $*.flags -d \\n ./bin/checker < $*.stdin \
		1> $*.stdout.actual \
		2> $*.stderr.actual; \
		echo $$? > $*.exit-code.actual
	cmp $*.stdout.actual $*.stdout.correct
	cmp $*.stderr.actual $*.stderr.correct
	cmp $*.exit-code.actual $*.exit-code.correct
	touch $@

projects/suprem/%.success: bin/suprem data/suprem.uk bin/checker \
	projects/suprem/%.stdout.actual \
	projects/suprem/%.stderr.actual \
	projects/suprem/%.exit-code.actual
	xargs -a projects/suprem/$*.flags -d \\n ./$< < projects/suprem/$*.stdin \
		1> projects/suprem/$*.stdout.actual \
		2> projects/suprem/$*.stderr.actual; \
		echo $$? > projects/suprem/$*.exit-code.actual
	./bin/checker \
		-i projects/suprem/$*.stdout.actual \
		-p projects/suprem/$*.stdout.pattern
	cmp projects/suprem/$*.stderr.actual projects/suprem/$*.stderr.correct
	cmp projects/suprem/$*.exit-code.actual projects/suprem/$*.exit-code.correct
	touch $@

.%-systest-othervars.mk:
	echo '' > $@
	echo $*'_systests_success = $$('$*'_systests:.stdin=.success)' >> $@
	echo 'systest_'$*': $$('$*'_systests_success)' >> $@

include .checker-systest-othervars.mk
include .keyread-systest-othervars.mk
include .preprocessor-systest-othervars.mk
include .scraper-systest-othervars.mk
include .suprem-systest-othervars.mk

systest: systest_checker
systest: systest_keyread
systest: systest_preprocessor
systest: systest_scraper
systest: systest_suprem

test: systest

format:
	find -name '*.c' -exec 'clang-format' '-i' '-verbose' '{}' \;
	find -name '*.h' -exec 'clang-format' '-i' '-verbose' '{}' \;

clean-successes:
	find -name '*.actual' -exec 'rm' '-v' '{}' \;
	find -name '*.success' -exec 'rm' '-v' '{}' \;
	
clean:
	find -name '*.o' -exec 'rm' '-v' '{}' \;
	find -name '*.mk' -exec 'rm' '-v' '{}' \;
	find -executable -a -type f -exec 'rm' '-v' '{}' \;

include $(checker_depends)
include $(keyread_depends)
include $(preprocessor_depends)
include $(scraper_depends)
include $(suprem_depends)















