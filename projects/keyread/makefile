# keyread

projects/keyread/srclist.mk:
	find ./projects/keyread -name '*.c' | sed 's/^/keyread_src += /' > $@

include projects/keyread/srclist.mk

keyread_objs = $(keyread_src:.c=.o)
keyread_dobjs = $(keyread_src:.c=.d.o)
keyread_depends = $(keyread_src:.c=.mk)

bin/keyread: $(keyread_objs)
bin/keyread.d: $(keyread_dobjs)

projects/keyread/systestlist.mk:
	find ./projects/keyread -path '*/stdin' | sort -V | sed 's/^/keyread_systests += /' > $@

include projects/keyread/systestlist.mk

keyread_systests_success = $(keyread_systests:/stdin=/success)
systest_keyread: $(keyread_systests_success)

systest: systest_keyread


include $(keyread_depends)
