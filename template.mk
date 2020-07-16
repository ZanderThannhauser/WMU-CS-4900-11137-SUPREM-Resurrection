# projectname

projects/projectname/srclist.mk:
	find ./projects/projectname/src -name '*.c' | sed 's/^/projectname_src += /' > $@

include projects/projectname/srclist.mk

projectname_objs = $(projectname_src:.c=.o)
projectname_dobjs = $(projectname_src:.c=.d.o)
projectname_depends = $(projectname_src:.c=.mk)
projectname_windows_objs = $(projectname_src:.c=.win.o)
projectname_windows_dobjs = $(projectname_src:.c=.d.win.o)
projectname_windows_depends = $(projectname_src:.c=.win.mk)

include $(projectname_depends)
include $(projectname_winobjs_depends)

# <Program Linking/Building>

bin/projectname: $(projectname_objs) | bin
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

bin/projectname.d: $(projectname_dobjs) | bin
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

bin/projectname.exe: $(projectname_windows_objs) | bin
	$(WIN_CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

bin/projectname.d.exe: $(projectname_windows_dobjs) | bin
	$(WIN_CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

# </Program Linking/Building>

# <Testing>

# <System Testing>

projects/projectname/systestlist.mk:
	find ./projects/projectname/system-tests -path '*/input' | sort -V | \
		sed 's/^/projectname_systests += /' > $@

include projects/projectname/systestlist.mk

projectname_systests_success = $(projectname_systests:/input=/success)
projectname_systests_winsuccess = $(projectname_systests:/input=/winsuccess)

projects/projectname/system-tests/%/success: bin/projectname data/suprem.uk \
	projects/projectname/system-tests/%/input \
	projects/projectname/system-tests/%/flags \
	projects/projectname/system-tests/%/stdout.correct \
	projects/projectname/system-tests/%/stderr.correct \
	projects/projectname/system-tests/%/exit-code.correct
	PROGRAM=`realpath bin/projectname`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/projectname/system-tests/$*; \
		xargs -a ./flags -d \\n $$PROGRAM ./input \
			1> ./stdout.actual \
			2> ./stderr.actual; \
		echo $$? > ./exit-code.actual; \
		diff ./stdout.{actual,correct} && \
		diff ./stderr.{actual,correct} && \
		diff ./exit-code.{actual,correct}
	touch $@

projects/projectname/system-tests/%/winsuccess: bin/projectname.exe data/suprem.uk \
	projects/projectname/system-tests/%/input \
	projects/projectname/system-tests/%/flags \
	projects/projectname/system-tests/%/stdout.correct \
	projects/projectname/system-tests/%/stderr.correct \
	projects/projectname/system-tests/%/exit-code.correct
	PROGRAM=`realpath bin/projectname.exe`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/projectname/system-tests/$*; \
		xargs -a ./flags -d \\n wine $$PROGRAM ./input \
			1> ./stdout.win.actual \
			2> ./stderr.win.actual; \
		echo $$? > ./exit-code.win.actual; \
		diff --strip-trailing-cr ./stdout.{win.actual,correct} && \
		diff --strip-trailing-cr ./stderr.{win.actual,correct} && \
		diff --strip-trailing-cr ./exit-code.{win.actual,correct}
	touch $@

systest_projectname: $(projectname_systests_success)
winsystest_projectname: $(projectname_systests_winsuccess)

systest: systest_projectname
systest: winsystest_projectname

# </System Testing>

# <Unit Testing>

projects/projectname/unittestlist.mk:
	find ./projects/projectname/unit-tests -name '*.c' | sort -V | \
		sed 's/^/projectname_unittests += /' > $@

include projects/projectname/unittestlist.mk

projectname_unittests_success = $(projectname_unittests:.c=.success)

projectname_unittests_depends = $(projectname_unittests:.c=.mk)

include $(projectname_unittests_depends)

projects/projectname/unit-tests/%: \
	projects/projectname/src/%.o \
	projects/projectname/unit-tests/%.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

projects/projectname/unit-tests/%.success: projects/projectname/unit-tests/%
	valgrind --leak-check=full --error-exitcode=2 ./$<
	touch $@

unittest_projectname: $(projectname_unittests_success)

unittest: unittest_projectname

# </Unit Testing>

# </Testing>







