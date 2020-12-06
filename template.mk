
bin/projects/projectname/srclist.mk:
	find ./projects/projectname/src -name '*.c' ! -path '*/#*' | sed 's/^/projectname_srcs += /' > $@

include bin/projects/projectname/srclist.mk

projectname_objs = $(patsubst %.c,bin/%.o,$(projectname_srcs))
projectname_dobjs = $(patsubst %.c,bin/%.d.o,$(projectname_srcs))
projectname_deps = $(patsubst %.c,bin/%.mk,$(projectname_srcs))

projectname_wobjs = $(patsubst %.c,bin/%.win.o,$(projectname_srcs))
projectname_wdobjs = $(patsubst %.c,bin/%.win.d.o,$(projectname_srcs))
projectname_wdeps = $(patsubst %.c,bin/%.win.mk,$(projectname_srcs))

include $(projectname_deps)

bin/projectname: $(projectname_objs) | bin
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LINUX_LDLIBS) -o $@

bin/projectname.d: $(projectname_dobjs) | bin
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LINUX_LDLIBS) -o $@

bin/projectname.exe: $(projectname_wobjs) | bin
	$(WIN_CC) $(LDFLAGS) $^ $(LOADLIBES) $(WINDOWS_LDLIBS) -o $@

bin/projectname.d.exe: $(projectname_wdobjs) | bin
	$(WIN_CC) $(LDFLAGS) $^ $(LOADLIBES) $(WINDOWS_LDLIBS) -o $@

#projects/projectname/system-tests/%/winsuccess: bin/projectname.exe data/suprem.uk \
#	projects/projectname/system-tests/%/input \
#	projects/projectname/system-tests/%/flags \
#	projects/projectname/system-tests/%/stdout.correct \
#	projects/projectname/system-tests/%/stderr.correct \
#	projects/projectname/system-tests/%/exit-code.correct
#	PROGRAM=`realpath bin/projectname.exe`; \
#	export SUP4KEYFILE=`realpath data/suprem.uk`; \
#	export SUP4MODELRC=`realpath data/modelrc`; \
#	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
#		cd projects/projectname/system-tests/$*; \
#		xargs -a ./flags -d \\n wine $$PROGRAM ./input \
#			1> ./stdout.win.actual \
#			2> ./stderr.win.actual; \
#		echo $$? > ./exit-code.win.actual; \
#		diff --strip-trailing-cr ./stdout.{win.actual,correct} && \
#		diff --strip-trailing-cr ./stderr.{win.actual,correct} && \
#		diff --strip-trailing-cr ./exit-code.{win.actual,correct}
#	touch $@

#systest_projectname: $(projectname_systests_success)
#winsystest_projectname: $(projectname_systests_winsuccess)

#systest: systest_projectname
#systest: winsystest_projectname

#projects/projectname/unittestlist.mk:
#	find ./projects/projectname/unit-tests -name '*.c' | sort -V | \
#		sed 's/^/projectname_unittests += /' > $@

#include projects/projectname/unittestlist.mk

#projectname_unittests_success = $(projectname_unittests:.c=.success)

#projectname_unittests_depends = $(projectname_unittests:.c=.mk)

#include $(projectname_unittests_depends)

#projects/projectname/unit-tests/%: \
#	projects/projectname/src/%.o \
#	projects/projectname/unit-tests/%.o
#	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

#projects/projectname/unit-tests/%.success: projects/projectname/unit-tests/%
#	valgrind --leak-check=full --error-exitcode=2 ./$<
#	touch $@

#unittest_projectname: $(projectname_unittests_success)

#unittest: unittest_projectname








