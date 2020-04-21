

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
CFLAGS += -g

#NDFLAGS += -O2

DFLAGS += -Wno-unused-but-set-variable
DFLAGS += -D DEBUGGING_2020

LDLIBS += -lm

.%-srclist.mk:
	find ./projects/$* -name '*.c' | sed 's/^/'$*'_src += /' > $@

include .keyread-srclist.mk
include .preprocessor-srclist.mk
include .scraper-srclist.mk
include .suprem-srclist.mk

.%-othervars.mk:
	echo > $@
	echo $*'_objs = $$('$*'_src:.c=.o)' >> $@
	echo $*'_dobjs = $$('$*'_src:.c=.d.o)' >> $@
	echo $*'_depends = $$('$*'_src:.c=.mk)' >> $@

include .keyread-othervars.mk
include .preprocessor-othervars.mk
include .scraper-othervars.mk
include .suprem-othervars.mk

#ARGS += ./projects/suprem/system-tests/durban1/stdin
#ARGS += ./projects/suprem/system-tests/exam1/stdin
#ARGS += ./projects/suprem/system-tests/exam2/stdin
#ARGS += ./projects/suprem/system-tests/exam3/stdin
ARGS += ./projects/suprem/system-tests/exam4/stdin
#ARGS += ./projects/suprem/system-tests/exam5/stdin
#ARGS += ./projects/suprem/system-tests/exam6/stdin
#ARGS += ./projects/suprem/system-tests/exam7/stdin
#ARGS += ./projects/suprem/system-tests/exam8/stdin
#ARGS += ./projects/suprem/system-tests/exam9/stdin
#ARGS += ./projects/suprem/system-tests/exam10/stdin
#ARGS += ./projects/suprem/system-tests/exam11/stdin
#ARGS += ./projects/suprem/system-tests/exam12/stdin
#ARGS += ./projects/suprem/system-tests/exam13/stdin
#ARGS += ./projects/suprem/system-tests/exam14/stdin
#ARGS += ./projects/suprem/system-tests/exam15/stdin
#ARGS += ./projects/suprem/system-tests/exam16/stdin
#ARGS += ./projects/suprem/system-tests/exam17/stdin
#ARGS += ./projects/suprem/system-tests/gaas/stdin
#ARGS += ./projects/suprem/system-tests/gaas/stdin
#ARGS += ./projects/suprem/system-tests/gaas/stdin
#ARGS += ./projects/suprem/system-tests/gaas/stdin
#ARGS += ./projects/suprem/system-tests/gaas/stdin
#ARGS += ./projects/suprem/system-tests/gaas/stdin
#ARGS += ./projects/suprem/system-tests/gaas/stdin
#ARGS += ./projects/suprem/system-tests/gaas/stdin

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
	$(CC) -c $(NDFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $< && false)

%.d.o: %.c  %.mk
	$(CC) -c $(DFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $< && false)


.PHONY: test open-all-suprem format clean-successes clean
.PHONY: test-keyread test-preprocessor test-scraper test-suprem

.%-systestlist.mk:
	find ./projects/$* -path '*/stdin' | sort -V | sed 's/^/'$*'_systests += /' > $@

include .keyread-systestlist.mk
include .preprocessor-systestlist.mk
include .scraper-systestlist.mk
include .suprem-systestlist.mk

#%/success: bin/checker %/flags %/stdin \
#	%/stdout.correct %/stderr.correct %/exit-code.correct
#	CHECKER=`realpath bin/checker`; \
#		cd $*; \
#		xargs -a ./flags -d \\n $$CHECKER \
#			0< ./stdin \
#			1> ./stdout.actual \
#			2> ./stderr.actual; \
#		echo $$? > ./exit-code.actual; \
#		cmp ./stdout.actual ./stdout.correct && \
#		cmp ./stderr.actual ./stderr.correct && \
#		cmp ./exit-code.actual ./exit-code.correct
#	touch $@

projects/suprem/%/success: bin/suprem data/suprem.uk data/modelrc ./data/sup4gs.imp \
	projects/suprem/%/stdin \
	projects/suprem/%/stdout.correct \
	projects/suprem/%/stderr.correct \
	projects/suprem/%/str.correct \
	projects/suprem/%/exit-code.correct
	SUPREM=`realpath bin/suprem`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/suprem/$*; \
		xargs -a ./flags -d \\n $$SUPREM \
			0< ./stdin \
			1> ./stdout.actual \
			2> ./stderr.actual; \
		echo $$? > ./exit-code.actual; \
		diff ./stdout.actual ./stdout.correct && \
		diff ./stderr.actual ./stderr.correct && \
		diff ./str.actual ./str.correct && \
		diff ./exit-code.actual ./exit-code.correct
	touch $@

.%-systest-othervars.mk:
	echo '' > $@
	echo $*'_systests_success = $$('$*'_systests:/stdin=/success)' >> $@
	echo 'systest_'$*': $$('$*'_systests_success)' >> $@

include .keyread-systest-othervars.mk
include .preprocessor-systest-othervars.mk
include .scraper-systest-othervars.mk
include .suprem-systest-othervars.mk

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
	find -path '*/success' -exec 'rm' '-v' '{}' \;
	
clean:
	find -name '*.o' -exec 'rm' '-v' '{}' \;
	find -name '*.mk' -exec 'rm' '-v' '{}' \;
	find -executable -a -type f -exec 'rm' '-v' '{}' \;

include $(keyread_depends)
include $(preprocessor_depends)
include $(scraper_depends)
include $(suprem_depends)















