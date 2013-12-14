#
# Makefile for libfiredns (stolen from musl) (requires GNU make)
#
# Use config.mak to override any of the following variables.
# Do not make changes here.
#

prefix = /usr/local
exec_prefix = $(prefix)

bindir = $(exec_prefix)/bin
libdir = $(prefix)/lib
includedir = $(prefix)/include

SRCS := $(sort $(wildcard src/*.c))
OBJS := $(SRCS:.c=.o)
LOBJS := $(OBJS:.o=.lo)
EXSRCS := $(sort $(wildcard examples/*.c))
EXOBJS := $(EXSRCS:.c=.o)
EXBINS := $(EXSRCS:.c=.out)
TESTSRCS := $(sort $(wildcard tests/*.c))
TESTOBJS := $(TESTSRCS:.c=.o)
TESTBINS := $(TESTSRCS:.c=.out)

# if you want to compile without IPV6 support for smaller linkage, add
# CFLAGS += -UHAVE_IPV6
# to your config.mak
CFLAGS  = -Os -std=c99 -D_XOPEN_SOURCE=700 -pipe -DHAVE_IPV6
INC     = -I./include
AR      = $(CROSS_COMPILE)ar
RANLIB  = $(CROSS_COMPILE)ranlib
OBJCOPY = $(CROSS_COMPILE)objcopy

ALL_INCLUDES := $(sort $(wildcard include/*.h include/*/*.h))

MY_LIBS = lib/libfiredns.a
ALL_LIBS = $(MY_LIBS) 

-include config.mak

all: $(ALL_LIBS)

examples: $(EXBINS)

tests: $(TESTBINS)

install: $(ALL_LIBS:lib/%=$(DESTDIR)$(libdir)/%) $(ALL_INCLUDES:include/%=$(DESTDIR)$(includedir)/%) $(ALL_TOOLS:tools/%=$(DESTDIR)$(bindir)/%)

clean:
	rm -f $(OBJS)
	rm -f $(LOBJS)
	rm -f $(EXOBJS)
	rm -f $(EXBINS)
	rm -f $(TESTOBJS)
	rm -f $(TESTBINS)
	rm -f $(ALL_LIBS) lib/*.[ao] lib/*.so

%.out: %.o $(ALL_LIBS)
	$(CC) -o $@ $< -l:lib/libfiredns.a $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

lib/libfiredns.a: $(OBJS)
	rm -f $@
	$(AR) rc $@ $(OBJS)
	$(RANLIB) $@

$(DESTDIR)$(bindir)/%: tools/%
	install -D $< $@

$(DESTDIR)$(prefix)/%: %
	install -D -m 644 $< $@

.PHONY: all clean install examples tests
