
TARGET = suprem

CC = gcc

CPPFLAGS = -I . 

# I don't understand the need for these, I just know the original makefile had
# them:
CPPFLAGS += -D DEVICE
CPPFLAGS += -D NO_F77

# Define which platform we're using. I made up a new one!
CPPFLAGS += -D UBUNTU

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wfatal-errors
#CFLAGS += -O3

DFLAGS += -g
DFLAGS += -D DEBUGGING_2020

LDLIBS += -lm

default: $(TARGET)

srclist.mk:
	find -name '*.c' ! -path './keyread/*' | sort | \
		sed 's/^/SRCS += /' > srclist.mk

include srclist.mk

OBJS = $(SRCS:.c=.o)
DOBJS = $(SRCS:.c=.d.o)
DEPENDS = $(SRCS:.c=.mk)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LOADLIBES) $(LDLIBS) -o $@

$(TARGET).d: $(DOBJS)
	$(CC) $(LDFLAGS) $(DOBJS) $(LOADLIBES) $(LDLIBS) -o $@

%.mk: %.c
	$(CPP) -MM -MT $@ $(CPPFLAGS) -MF $@ $< || (gedit $< && false)

%.h %.c: %.y
	$(YACC) $(YFLAGS) -d $< || (gedit $< && false)
	mv y.tab.c $*.c
	mv y.tab.h $*.h

%.o: %.c %.mk
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $< && false)

%.d.o: %.c  %.mk
	$(CC) -c $(DFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $< && false)

.PHONY: clean

clean:
	rm -f $(OBJS) $(DOBJS) $(DEPENDS)
	rm -f srclist.mk
	rm -f $(TARGET)

include $(DEPENDS)






















