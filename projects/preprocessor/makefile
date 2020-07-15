# preprocessor

projects/preprocessor/srclist.mk:
	find ./projects/preprocessor/src -name '*.c' | sed 's/^/preprocessor_src += /' > $@

include projects/preprocessor/srclist.mk

preprocessor_objs = $(preprocessor_src:.c=.o)
preprocessor_dobjs = $(preprocessor_src:.c=.d.o)
preprocessor_depends = $(preprocessor_src:.c=.mk)

bin/preprocessor: $(preprocessor_objs)
bin/preprocessor.d: $(preprocessor_dobjs)

projects/preprocessor/systestlist.mk:
	find ./projects/preprocessor/system-tests -path '*/input' | \
		sort -V | sed 's/^/preprocessor_systests += /' > $@

include projects/preprocessor/systestlist.mk

#projects/preprocessor/unittestlist.mk:
#	find ./projects/preprocessor -path '*/input' | sort -V | sed 's/^/preprocessor_unittests += /' > $@

#include projects/preprocessor/unittestlist.mk

preprocessor_systests_success = $(preprocessor_systests:/input=/success)
systest_preprocessor: $(preprocessor_systests_success)

projects/preprocessor/system-tests/%/success: bin/preprocessor \
	projects/preprocessor/system-tests/%/input \
	projects/preprocessor/system-tests/%/stdout.correct \
	projects/preprocessor/system-tests/%/stderr.correct \
	projects/preprocessor/system-tests/%/exit-code.correct \
	projects/preprocessor/system-tests/%/flags
	PROGRAM=`realpath bin/preprocessor`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/preprocessor/system-tests/$*; \
		xargs -a ./flags -d \\n $$PROGRAM ./input \
			1> ./stdout.actual \
			2> ./stderr.actual; \
		echo $$? > ./exit-code.actual; \
		diff ./stdout.actual ./stdout.correct && \
		diff ./stderr.actual ./stderr.correct && \
		diff ./exit-code.actual ./exit-code.correct
	touch $@

preprocessor_unittests_success = $(preprocessor_unittests:/stdin=/success)
#unittest_preprocessor: $(preprocessor_unittests_success)

systest: systest_preprocessor

#unittest: unittest_preprocessor


include $(preprocessor_depends)
