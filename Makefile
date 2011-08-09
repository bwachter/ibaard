.include "system.mk"

VERSIONNR!=head -1 CHANGES|sed 's/://'
VERSION=ibaard-$(VERSIONNR)
#CURNAME=$(notdir $(shell pwd))
OS!=uname
MK_ALL=$$>

.ifdef DEBUG
.if $(DEBUG) == 1
CFLAGS=$(DEBUG_CFLAGS)
LDFLAGS=$(DEBUG_LDFLAGS)
.endif
.endif

.ifdef WIN32
.if $(WIN32) == 1
LIBS+=$(WIN32_LIBS)
EXE=.exe
.else
.if $(OS) == SunOS
LIBS+=$(SOLARIS_LIBS)
.ifeq ($(DEBUG),)
STRIP=strip -x
.endif
.endif
.if $(OS) == IRIX64
STRIP=test
.ifdef DEBUG
CFLAGS=-Wall -W -Os 
LDFLAGS=-g
.else
CFLAGS=-g -Wall -W -Os
.endif
.endif
.if $(OS) == FreeBSD
.endif
.endif
.endif

.ifdef BROKEN
.if $(BROKEN) == 1
CFLAGS+=$(BROKEN_CFLAGS)
.endif
.endif

.ifdef MATRIXSSL
LIBS+=$(MATRIX_LIBS)
CFLAGS+=$(MATRIX_CFLAGS)
.endif

.ifdef GNUTLS
LIBS+=$(GNUTLS_LIBS)
CFLAGS+=$(GNUTLS_CFLAGS)
.endif

.ifdef SSL
.if $(SSL) == 1
LIBS+=$(SSL_LIBS)
CFLAGS+=$(SSL_CFLAGS)
.endif
.endif

.ifdef DEV
.if $(DEV) == 1
CFLAGS+=$(DEV_CFLAGS)
.endif
.endif

.if exists(dyn-bsdmake.mk)
.else
ALL=dep
.endif

.include "build.mk"

.if exists(dyn-bsdmake.mk)
.include "dyn-bsdmake.mk"  
.endif

dep: dyn-bsdmake.mk
	$(MAKE)
