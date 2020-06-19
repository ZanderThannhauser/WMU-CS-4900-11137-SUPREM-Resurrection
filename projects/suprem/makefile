# suprem

projects/suprem/srclist.mk:
	find ./projects/suprem -name '*.c' | sed 's/^/suprem_src += /' > $@

include projects/suprem/srclist.mk

suprem_objs = $(suprem_src:.c=.o)
suprem_dobjs = $(suprem_src:.c=.d.o)
suprem_depends = $(suprem_src:.c=.mk)

bin/suprem: $(suprem_objs)
bin/suprem.d: $(suprem_dobjs)

projects/suprem/systestlist.mk:
	find ./projects/suprem -path '*/stdin' | sort -V | sed 's/^/suprem_systests += /' > $@

include projects/suprem/systestlist.mk

suprem_systests_success = $(suprem_systests:/stdin=/success)
systest_suprem: $(suprem_systests_success)

systest: systest_suprem


include $(suprem_depends)
