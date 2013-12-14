#
# Makefile for libfiredns (stolen from musl) (requires GNU make)
#
# Use config.mak to override any of the following variables.
# Do not make changes here.
#

exec_prefix = /usr/local
bindir = $(exec_prefix)/bin

prefix = /usr/local/
includedir = $(prefix)/include
libdir = $(prefix)/lib

SRCS := $(sort $(wildcard src/*.c))
OBJS := $(SRCS:.c=.o)
LOBJS := $(OBJS:.o=.lo)
EXSRCS := $(sort $(wildcard examples/*.c))
EXOBJS := $(EXSRCS:.c=.o)
EXBINS := $(EXSRCS:.c=.out)
TESTSRCS := $(sort $(wildcard tests/*.c))
TESTOBJS := $(TESTSRCS:.c=.o)
TESTBINS := $(TESTSRCS:.c=.out)

CFLAGS  = -Os -std=c99 -D_XOPEN_SOURCE=700 -pipe
#LDFLAGS = -nostdlib -shared -fPIC -Wl,-e,_start -Wl,-Bsymbolic-functions
INC     = -I./include
#PIC     = -fPIC -O3
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

lib/%.o:
	cp $< $@

$(DESTDIR)$(bindir)/%: tools/%
	install -D $< $@

$(DESTDIR)$(prefix)/%: %
	install -D -m 644 $< $@

.PHONY: all clean install
