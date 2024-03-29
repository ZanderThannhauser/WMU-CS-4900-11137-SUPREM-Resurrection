
CC = gcc
WIN_CC = x86_64-w64-mingw32-gcc

SHELL=/bin/bash

CPPFLAGS = -I . -I projects

CPPFLAGS += -D DEVICE
CPPFLAGS += -D NO_F77

LINUX_CPPFLAGS = $(CPPFLAGS)
LINUX_CPPFLAGS += -D HAVE_ASINH
LINUX_CPPFLAGS += -D _GNU_SOURCE
LINUX_CPPFLAGS += -D _XOPEN_SOURCE=500
LINUX_CPPFLAGS += -D LINUX

WINDOWS_CPPFLAGS = $(CPPFLAGS)
WINDOWS_CPPFLAGS += -D WINDOWS

CFLAGS += -std=c99

CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Werror
#CFLAGS += -Wfatal-errors
CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-array-bounds
CFLAGS += -Wno-format-overflow

RFLAGS += -O2
RFLAGS += -flto

DFLAGS += -Wno-unused-but-set-variable
DFLAGS += -D DEBUGGING_2020=1

LDLIBS += -lm

default: bin/suprem

ARGS += ./projects/suprem/system-tests/durbin1/input
#ARGS += ./projects/suprem/system-tests/exam1/input
#ARGS += ./projects/suprem/system-tests/exam2/input
#ARGS += ./projects/suprem/system-tests/exam3/input
#ARGS += ./projects/suprem/system-tests/exam4/input
#ARGS += ./projects/suprem/system-tests/exam5/input
#ARGS += ./projects/suprem/system-tests/exam6/input
#ARGS += ./projects/suprem/system-tests/exam7/input
#ARGS += ./projects/suprem/system-tests/exam8/input
#ARGS += ./projects/suprem/system-tests/exam9/input
#ARGS += ./projects/suprem/system-tests/exam10/input
#ARGS += ./projects/suprem/system-tests/exam11/input
#ARGS += ./projects/suprem/system-tests/exam12/input
#ARGS += ./projects/suprem/system-tests/exam13/input
#ARGS += ./projects/suprem/system-tests/exam14/input
#ARGS += ./projects/suprem/system-tests/exam15/input
#ARGS += ./projects/suprem/system-tests/exam16/input
#ARGS += ./projects/suprem/system-tests/exam17/input
#ARGS += ./projects/suprem/system-tests/gaas/input
#ARGS += ./projects/suprem/system-tests/gaas/input
#ARGS += ./projects/suprem/system-tests/gaas/input
#ARGS += ./projects/suprem/system-tests/gaas/input
#ARGS += ./projects/suprem/system-tests/gaas/input
#ARGS += ./projects/suprem/system-tests/gaas/input
#ARGS += ./projects/suprem/system-tests/gaas/input
#ARGS += ./projects/suprem/system-tests/gaas/input

bin:
	rm -f ./bin
	ln -s `mktemp -d` bin
#	mkdir bin
	find -type d | sed 's ^ bin/ ' | xargs -d \\n mkdir -p

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

data/suprem.uk: bin/keyread data/suprem.key
	./bin/keyread ./data/suprem.uk < ./data/suprem.key

.PRECIOUS: bin/%.mk bin/%.win.mk

bin/%.mk: %.c
	$(CPP) -MM -MT $@ $(LINUX_CPPFLAGS) -MF $@ $< || ($$EDITOR $< && false)

bin/%.win.mk: %.c
	$(WIN_CC) -MM -MT $@ $(WINDOWS_CPPFLAGS) -MF $@ $< || ($$EDITOR $< && false)

%.h %.c: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	mv y.tab.h $*.h

bin/%.o: %.c bin/%.mk
	$(CC) -c $(RFLAGS) $(LINUX_CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

bin/%.d.o: %.c bin/%.mk
	$(CC) -c $(DFLAGS) $(LINUX_CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

bin/%.win.o: %.c bin/%.win.mk
	$(WIN_CC) -c $(RFLAGS) $(WINDOWS_CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

bin/%.d.win.o: %.c bin/%.win.mk
	$(WIN_CC) -c $(DFLAGS) $(WINDOWS_CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

#.PHONY: test open-all-suprem format clean-successes clean
#.PHONY: test-keyread test-preprocessor test-scraper test-suprem

bin/winpack.zip: bin/suprem.exe \
	data/modelrc \
	data/sup4gs.imp \
	data/suprem.uk \
	examples/durbin1/input \
	examples/durbin1/stdout \
	examples/exam4/input \
	examples/exam4/stdout
	zip $@ $^

test: systest

bin/projects/%/makefile: template.mk | bin
	sed 's/projectname/$*/g' < $< > $@

include bin/projects/keyread/makefile
include bin/projects/suprem/makefile

# rule to search and list all system tests for suprem:
bin/systestlist.mk: | bin
	find ./projects/suprem/system-tests -path '*/input' | sort -V | sed 's/^/systests += /' > $@

include bin/systestlist.mk

systests_success = $(patsubst %/input,bin/%/success,$(systests))

systest: $(systests_success)

# rule to copy whatever over for testing:
bin/projects/suprem/system-tests/%: projects/suprem/system-tests/%
	cp -v $< $@

# example 15 needs 'be1' to by copied along for testing:
bin/./projects/suprem/system-tests/exam15/success: bin/projects/suprem/system-tests/exam15/be1

# example 16 needs 'file1' to by copied along for testing:
bin/./projects/suprem/system-tests/exam16/success: bin/projects/suprem/system-tests/exam16/file1

# rule for all system tests:
bin/%/success: bin/suprem \
	data/suprem.uk data/modelrc data/sup4gs.imp \
	%/input %/stdout %/stderr %/exitcode
	SUPREM=`realpath bin/suprem`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
	export INPUT=`realpath $*/input`; \
		cd bin/$*; \
		$$SUPREM $$INPUT 0< /dev/null 1> ./stdout 2> ./stderr; \
		echo $$? > ./exitcode;
	diff ./bin/$*/stdout   ./$*/stdout
	diff ./bin/$*/stderr   ./$*/stderr
	diff ./bin/$*/exitcode ./$*/exitcode
	touch $@



















