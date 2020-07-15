

default: bin/suprem

CC = gcc

CPPFLAGS = -I . -I projects

CPPFLAGS += -D DEVICE
CPPFLAGS += -D NO_F77

CPPFLAGS += -D _GNU_SOURCE
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

NDFLAGS += -O2

DFLAGS += -Wno-unused-but-set-variable
DFLAGS += -D DEBUGGING_2020

LDLIBS += -lm

#ARGS += ./projects/suprem/system-tests/durban1/stdin
#ARGS += ./projects/suprem/system-tests/exam1/stdin
#ARGS += ./projects/suprem/system-tests/exam2/stdin
#ARGS += ./projects/suprem/system-tests/exam3/stdin
#ARGS += ./projects/suprem/system-tests/exam4/stdin
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

bin:
	mkdir -p bin

data/suprem.uk: ./bin/keyread ./data/suprem.key
	./bin/keyread ./data/suprem.uk < ./data/suprem.key

projects/%/makefile: template.mk
	sed 's/projectname/$*/g' < $< > $@

include projects/keyread/makefile
include projects/preprocessor/makefile
include projects/scraper/makefile
include projects/suprem/makefile

%.mk: %.c
	$(CPP) -MM -MT $@ $(CPPFLAGS) -MF $@ $< || ($$EDITOR $< && false)

%.h %.c: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	mv y.tab.h $*.h

%.o: %.c %.mk
	$(CC) -c $(NDFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

%.d.o: %.c %.mk
	$(CC) -c $(DFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@ || ($$EDITOR $< && false)

.PHONY: test open-all-suprem format clean-successes clean
.PHONY: test-keyread test-preprocessor test-scraper test-suprem

test: systest
test: unittest

clean-successes:
	find -name '*.actual' -delete
	find -name 'success' -delete
	find -path '*/success' -delete
	
clean:
	rm -rf bin
	find -name '*.o' -print -delete
	find -path './*/*.mk' -print -delete
	find -executable -a -type f -print -delete













