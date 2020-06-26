# suprem

projects/suprem/srclist.mk:
	find ./projects/suprem/src -name '*.c' | sed 's/^/suprem_src += /' > $@

include projects/suprem/srclist.mk

suprem_objs = $(suprem_src:.c=.o)
suprem_dobjs = $(suprem_src:.c=.d.o)
suprem_depends = $(suprem_src:.c=.mk)

bin/suprem: $(suprem_objs)
bin/suprem.d: $(suprem_dobjs)

projects/suprem/systestlist.mk:
	find ./projects/suprem/system-tests -path '*/input' | \
		sort -V | sed 's/^/suprem_systests += /' > $@

include projects/suprem/systestlist.mk

projects/suprem/unittestlist.mk:
	find ./projects/suprem -path '*/input' | sort -V | sed 's/^/suprem_unittests += /' > $@

include projects/suprem/unittestlist.mk

suprem_systests_success = $(suprem_systests:/input=/success)
systest_suprem: $(suprem_systests_success)

projects/suprem/system-tests/%/success: bin/suprem \
	projects/suprem/system-tests/%/input \
	projects/suprem/system-tests/%/stdout.correct \
	projects/suprem/system-tests/%/stderr.correct \
	projects/suprem/system-tests/%/exit-code.correct \
	projects/suprem/system-tests/%/flags
	PROGRAM=`realpath bin/suprem`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/suprem/system-tests/$*; \
		xargs -a ./flags -d \\n $$PROGRAM ./input \
			1> ./stdout.actual \
			2> ./stderr.actual; \
		echo $$? > ./exit-code.actual; \
		diff ./stdout.actual ./stdout.correct && \
		diff ./stderr.actual ./stderr.correct && \
		diff ./exit-code.actual ./exit-code.correct
	touch $@

#  diff ./str.actual ./str.correct && \

suprem_unittests_success = $(suprem_unittests:/stdin=/success)
unittest_suprem: $(suprem_unittests_success)

systest: systest_suprem

unittest: unittest_suprem


include $(suprem_depends)
