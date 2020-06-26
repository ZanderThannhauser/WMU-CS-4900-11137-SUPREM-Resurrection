# scraper

projects/scraper/srclist.mk:
	find ./projects/scraper/src -name '*.c' | sed 's/^/scraper_src += /' > $@

include projects/scraper/srclist.mk

scraper_objs = $(scraper_src:.c=.o)
scraper_dobjs = $(scraper_src:.c=.d.o)
scraper_depends = $(scraper_src:.c=.mk)

bin/scraper: $(scraper_objs)
bin/scraper.d: $(scraper_dobjs)

projects/scraper/systestlist.mk:
	find ./projects/scraper/system-tests -path '*/input' | \
		sort -V | sed 's/^/scraper_systests += /' > $@

include projects/scraper/systestlist.mk

projects/scraper/unittestlist.mk:
	find ./projects/scraper -path '*/input' | sort -V | sed 's/^/scraper_unittests += /' > $@

include projects/scraper/unittestlist.mk

scraper_systests_success = $(scraper_systests:/input=/success)
systest_scraper: $(scraper_systests_success)

projects/scraper/system-tests/%/success: bin/scraper \
	projects/scraper/system-tests/%/input \
	projects/scraper/system-tests/%/stdout.correct \
	projects/scraper/system-tests/%/stderr.correct \
	projects/scraper/system-tests/%/exit-code.correct \
	projects/scraper/system-tests/%/flags
	PROGRAM=`realpath bin/scraper`; \
	export SUP4KEYFILE=`realpath data/suprem.uk`; \
	export SUP4MODELRC=`realpath data/modelrc`; \
	export SUP4IMPDATA=`realpath data/sup4gs.imp`; \
		cd projects/scraper/system-tests/$*; \
		xargs -a ./flags -d \\n $$PROGRAM ./input \
			1> ./stdout.actual \
			2> ./stderr.actual; \
		echo $$? > ./exit-code.actual; \
		diff ./stdout.actual ./stdout.correct && \
		diff ./stderr.actual ./stderr.correct && \
		diff ./exit-code.actual ./exit-code.correct
	touch $@

#  diff ./str.actual ./str.correct && \

scraper_unittests_success = $(scraper_unittests:/stdin=/success)
unittest_scraper: $(scraper_unittests_success)

systest: systest_scraper

unittest: unittest_scraper


include $(scraper_depends)
