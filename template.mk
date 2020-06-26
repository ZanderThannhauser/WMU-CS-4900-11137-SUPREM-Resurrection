# project-name

projects/project-name/srclist.mk:
	find ./projects/project-name/src -name '*.c' | sed 's/^/project-name_src += /' > $@

include projects/project-name/srclist.mk

project-name_objs = $(project-name_src:.c=.o)
project-name_dobjs = $(project-name_src:.c=.d.o)
project-name_depends = $(project-name_src:.c=.mk)

bin/project-name: $(project-name_objs)
bin/project-name.d: $(project-name_dobjs)

projects/project-name/systestlist.mk:
	find ./projects/project-name/system-tests -path '*/input' | \
		sort -V | sed 's/^/project-name_systests += /' > $@

include projects/project-name/systestlist.mk

projects/project-name/unittestlist.mk:
	find ./projects/project-name -path '*/input' | sort -V | sed 's/^/project-name_unittests += /' > $@

include projects/project-name/unittestlist.mk

project-name_systests_success = $(project-name_systests:/input=/success)
systest_project-name: $(project-name_systests_success)

projects/project-name/system-tests/%/success: bin/project-name \
	projects/project-name/system-tests/%/input \
	projects/project-name/system-tests/%/stdout.correct \
	projects/project-name/system-tests/%/stderr.correct \
	projects/project-name/system-tests/%/exit-code.correct \
	projects/project-name/system-tests/%/flags
	PROGRAM=`realpath bin/project-name`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/project-name/system-tests/$*; \
		xargs -a ./flags -d \\n $$PROGRAM ./input \
			1> ./stdout.actual \
			2> ./stderr.actual; \
		echo $$? > ./exit-code.actual; \
		diff ./stdout.actual ./stdout.correct && \
		diff ./stderr.actual ./stderr.correct && \
		diff ./exit-code.actual ./exit-code.correct
	touch $@

project-name_unittests_success = $(project-name_unittests:/stdin=/success)
unittest_project-name: $(project-name_unittests_success)

systest: systest_project-name

unittest: unittest_project-name


include $(project-name_depends)
