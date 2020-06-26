# keyread

projects/keyread/srclist.mk:
	find ./projects/keyread/src -name '*.c' | sed 's/^/keyread_src += /' > $@

include projects/keyread/srclist.mk

keyread_objs = $(keyread_src:.c=.o)
keyread_dobjs = $(keyread_src:.c=.d.o)
keyread_depends = $(keyread_src:.c=.mk)

bin/keyread: $(keyread_objs)
bin/keyread.d: $(keyread_dobjs)

projects/keyread/systestlist.mk:
	find ./projects/keyread/system-tests -path '*/input' | \
		sort -V | sed 's/^/keyread_systests += /' > $@

include projects/keyread/systestlist.mk

projects/keyread/unittestlist.mk:
	find ./projects/keyread -path '*/input' | sort -V | sed 's/^/keyread_unittests += /' > $@

include projects/keyread/unittestlist.mk

keyread_systests_success = $(keyread_systests:/input=/success)
systest_keyread: $(keyread_systests_success)

projects/keyread/system-tests/%/success: bin/keyread \
	projects/keyread/system-tests/%/input \
	projects/keyread/system-tests/%/stdout.correct \
	projects/keyread/system-tests/%/stderr.correct \
	projects/keyread/system-tests/%/exit-code.correct \
	projects/keyread/system-tests/%/flags
	PROGRAM=`realpath bin/keyread`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/keyread/system-tests/$*; \
		xargs -a ./flags -d \\n $$PROGRAM ./input \
			1> ./stdout.actual \
			2> ./stderr.actual; \
		echo $$? > ./exit-code.actual; \
		diff ./stdout.actual ./stdout.correct && \
		diff ./stderr.actual ./stderr.correct && \
		diff ./exit-code.actual ./exit-code.correct
	touch $@

#  diff ./str.actual ./str.correct && \

keyread_unittests_success = $(keyread_unittests:/stdin=/success)
unittest_keyread: $(keyread_unittests_success)

systest: systest_keyread

unittest: unittest_keyread


include $(keyread_depends)
