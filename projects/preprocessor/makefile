# preprocessor

projects/preprocessor/srclist.mk:
	find ./projects/preprocessor -name '*.c' | sed 's/^/preprocessor_src += /' > $@

include projects/preprocessor/srclist.mk

preprocessor_objs = $(preprocessor_src:.c=.o)
preprocessor_dobjs = $(preprocessor_src:.c=.d.o)
preprocessor_depends = $(preprocessor_src:.c=.mk)

bin/preprocessor: $(preprocessor_objs)
bin/preprocessor.d: $(preprocessor_dobjs)

projects/preprocessor/systestlist.mk:
	find ./projects/preprocessor -path '*/stdin' | sort -V | sed 's/^/preprocessor_systests += /' > $@

include projects/preprocessor/systestlist.mk

preprocessor_systests_success = $(preprocessor_systests:/stdin=/success)
systest_preprocessor: $(preprocessor_systests_success)

systest: systest_preprocessor


include $(preprocessor_depends)
