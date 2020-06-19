# project-name

projects/project-name/srclist.mk:
	find ./projects/project-name -name '*.c' | sed 's/^/project-name_src += /' > $@

include projects/project-name/srclist.mk

project-name_objs = $(project-name_src:.c=.o)
project-name_dobjs = $(project-name_src:.c=.d.o)
project-name_depends = $(project-name_src:.c=.mk)

bin/project-name: $(project-name_objs)
bin/project-name.d: $(project-name_dobjs)

projects/project-name/systestlist.mk:
	find ./projects/project-name -path '*/stdin' | sort -V | sed 's/^/project-name_systests += /' > $@

include projects/project-name/systestlist.mk

projects/project-name/unittestlist.mk:
	find ./projects/project-name -path '*/stdin' | sort -V | sed 's/^/project-name_unittests += /' > $@

include projects/project-name/unittestlist.mk

project-name_systests_success = $(project-name_systests:/stdin=/success)
systest_project-name: $(project-name_systests_success)

project-name_unittests_success = $(project-name_unittests:/stdin=/success)
systest_project-name: $(project-name_unittests_success)

systest: systest_project-name

unittest: unittest_project-name


include $(project-name_depends)
