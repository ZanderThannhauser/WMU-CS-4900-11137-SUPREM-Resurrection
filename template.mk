# projectname

projects/projectname/srclist.mk:
	find ./projects/projectname/src -name '*.c' | sed 's/^/projectname_src += /' > $@

include projects/projectname/srclist.mk

projectname_objs = $(projectname_src:.c=.o)
projectname_dobjs = $(projectname_src:.c=.d.o)
projectname_depends = $(projectname_src:.c=.mk)

# <Program Linking/Building>

bin/projectname: $(projectname_objs) | bin
bin/projectname.d: $(projectname_dobjs) | bin

#bin/%:
#	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

# </Program Linking/Building>

# <Testing>

# <System Testing>

projects/projectname/systestlist.mk:
	find ./projects/projectname/system-tests -path '*/input' | sort -V | \
		sed 's/^/projectname_systests += /' > $@

include projects/projectname/systestlist.mk

projectname_systests_success = $(projectname_systests:/input=/success)

projects/projectname/system-tests/%/success: bin/projectname \
	projects/projectname/system-tests/%/input \
	projects/projectname/system-tests/%/stdout.correct \
	projects/projectname/system-tests/%/stderr.correct \
	projects/projectname/system-tests/%/exit-code.correct \
	projects/projectname/system-tests/%/flags
	PROGRAM=`realpath bin/projectname`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/projectname/system-tests/$*; \
		xargs -a ./flags -d \\n $$PROGRAM ./input \
			1> ./stdout.actual \
			2> ./stderr.actual; \
		echo $$? > ./exit-code.actual; \
		diff ./stdout.actual ./stdout.correct && \
		diff ./stderr.actual ./stderr.correct && \
		diff ./exit-code.actual ./exit-code.correct
	touch $@

systest_projectname: $(projectname_systests_success)

systest: systest_projectname

# </System Testing>

# <Unit Testing>

projects/projectname/unittestlist.mk:
	find ./projects/projectname/unit-tests -name '*.c' | sort -V | \
		sed 's/^/projectname_unittests += /' > $@

include projects/projectname/unittestlist.mk

projectname_unittests_success = $(projectname_unittests:.c=.success)

projectname_depends += $(projectname_unittests:.c=.mk)

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

include $(projectname_depends)














