# scraper

projects/scraper/srclist.mk:
	find ./projects/scraper -name '*.c' | sed 's/^/scraper_src += /' > $@

include projects/scraper/srclist.mk

scraper_objs = $(scraper_src:.c=.o)
scraper_dobjs = $(scraper_src:.c=.d.o)
scraper_depends = $(scraper_src:.c=.mk)

bin/scraper: $(scraper_objs)
bin/scraper.d: $(scraper_dobjs)

projects/scraper/systestlist.mk:
	find ./projects/scraper -path '*/stdin' | sort -V | sed 's/^/scraper_systests += /' > $@

include projects/scraper/systestlist.mk

scraper_systests_success = $(scraper_systests:/stdin=/success)
systest_scraper: $(scraper_systests_success)

systest: systest_scraper


include $(scraper_depends)
