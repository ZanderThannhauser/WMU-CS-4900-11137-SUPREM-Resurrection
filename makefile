

default: bin/suprem

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

CFLAGS += -std=c99 # We updated the source to use c99
#CFLAGS += -std=c90 # Original SUPREM was written in c90, I think.

CFLAGS += -Wall
CFLAGS += -Werror
#CFLAGS += -Wfatal-errors
CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-array-bounds
CFLAGS += -Wno-format-overflow
CFLAGS += -g

NDFLAGS += -O2

DFLAGS += -Wno-unused-but-set-variable
DFLAGS += -D DEBUGGING_2020=1

LDLIBS += -lm

ARGS += ./projects/suprem/system-tests/durban1/input
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

bin:
	mkdir -p bin

data/suprem.uk: bin/keyread data/suprem.key
	./bin/keyread ./data/suprem.uk < ./data/suprem.key

projects/%/makefile: template.mk
	sed 's/projectname/$*/g' < $< > $@

include projects/keyread/makefile
include projects/preprocessor/makefile
include projects/scraper/makefile
include projects/suprem/makefile

%.mk: %.c
	$(CPP) -MM -MT $@ $(LINUX_CPPFLAGS) -MF $@ $< || ($$EDITOR $< && false)

%.win.mk: %.c
	$(WIN_CC) -MM -MT $@ $(WINDOWS_CPPFLAGS) -MF $@ $< || ($$EDITOR $< && false)

%.h %.c: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	mv y.tab.h $*.h

%.o: %.c %.mk
	$(CC) -c $(NDFLAGS) $(LINUX_CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

%.d.o: %.c %.mk
	$(CC) -c $(DFLAGS) $(LINUX_CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

%.win.o: %.c %.win.mk
	$(WIN_CC) -c $(NDFLAGS) $(WINDOWS_CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

%.d.win.o: %.c %.win.mk
	$(WIN_CC) -c $(DFLAGS) $(WINDOWS_CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

.PHONY: test open-all-suprem format clean-successes clean
.PHONY: test-keyread test-preprocessor test-scraper test-suprem

winpack.zip: bin/suprem.exe \
	data/modelrc \
	data/sup4gs.imp \
	data/suprem.uk \
	examples/durban1/input \
	examples/durban1/stdout.correct \
	examples/durban1/str.correct \
	examples/exam4/input \
	examples/exam4/stdout.correct \
	examples/exam4/str.correct
	zip -r $@ $^

test: systest
test: unittest

clean-successes:
	find -name '*.actual' -delete
	find -name '*.success' -delete
	find -path '*/success' -delete
	
clean:
	rm -rf bin winpack.zip
	find -name '*.o' -print -delete
	find -path './*/*.mk' -print -delete
	find -executable -a -type f -print -delete













