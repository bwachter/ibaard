# Allow overwriting of some common configuration values
ifeq (config.mk,$(wildcard config.mk))
include config.mk
endif

include mk/config.mk

# set up some basic flags
NAME=ibaard
VERSIONNR=$(shell head -1 CHANGES|sed 's/://')
MAJOR=$(shell echo $(VERSIONNR)|sed 's/~.*//;s/\..*//')
MINOR=$(shell echo $(VERSIONNR)|sed 's/~.*//;s/[0-9]*\.//;s/\.[0-9]*//')
RELEASE=$(shell echo $(VERSIONNR)|sed 's/~.*//;s/[0-9]*\.[0-9]*\.*//;s/^$$/0/;')
VERSION=ibaard-$(shell head -1 CHANGES|sed 's/://')
CURNAME=$(notdir $(shell pwd))
MK_ALL=$$^

ifdef DEBUG
CFLAGS=$(DEBUG_CFLAGS)
LDFLAGS=$(DEBUG_LDFLAGS)
endif

ifdef WIN32
LIBS+=-lws2_32 -lwsock32 -lgdi32
EXE=.exe
else
ifeq ($(shell uname),SunOS)
LIBS+=-lresolv -lsocket
ifeq ($(DEBUG),)
STRIP=strip -x
endif
endif
ifeq ($(shell uname),IRIX64)
ifdef DEBUG
STRIP=
CFLAGS=-Wall -W -Os 
LDFLAGS=-g
else
STRIP=
CFLAGS=-g -Wall -W -Os
endif
endif
endif

ifdef BROKEN
CFLAGS+=-D_BROKEN_IO
endif

ifdef MATRIXSSL
LIBS+=$(MATRIX_LIBS)
CFLAGS+=$(MATRIX_CFLAGS)
endif

ifdef GNUTLS
LIBS+=$(GNUTLS_LIBS)
CFLAGS+=$(GNUTLS_CFLAGS)
endif

ifdef SSL
LIBS+=$(SSL_LIBS)
CFLAGS+=$(SSL_CFLAGS)
endif

ifdef DEV
CFLAGS+=-D_DEV
ALL+=
endif

ifeq (dyn-conf.mk,$(wildcard dyn-conf.mk))
include dyn-conf.mk
endif

ifneq (dyn-gmake.mk,$(wildcard dyn-gmake.mk))
ALL=dep
endif

include build.mk mk/common-targets.mk

ifeq (dyn-gmake.mk,$(wildcard dyn-gmake.mk))
include dyn-gmake.mk
endif

dep: dyn-gmake.mk dyn-conf.mk
	$(MAKE)

