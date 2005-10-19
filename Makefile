.include "system.mk"

VERSIONNR!=head -1 CHANGES|sed 's/://'
VERSION=aardmail-$(VERSIONNR)
#CURNAME=$(notdir $(shell pwd))
OS!=uname

.ifdef DEBUG
CFLAGS=$(DEBUG_CFLAGS)
LDFLAGS=$(DEBUG_LDFLAGS)
.endif

.ifdef WIN32
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

.ifdef BROKEN
CFLAGS+=$(BROKEN_CFLAGS)
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
LIBS+=$(SSL_LIBS)
CFLAGS+=$(SSL_CFLAGS)
.endif

.ifdef DEV
CFLAGS+=$(DEV_CFLAGS)
.endif

.include "build.mk"

.c.o:
	$(Q)echo "CC $@"
	$(Q)$(DIET) $(CROSS)$(CC) $(CFLAGS) -c $< -o $@
.ifdef $(STRIP)
	$(Q)$(COMMENT) -$(CROSS)$(STRIP) $@
.endif

ibaard.a: $(OBJDIR)/cat.o $(OBJDIR)/cati.o $(OBJDIR)/aardlog.o $(OBJDIR)/strip.o \
	$(OBJDIR)/split.o \
	$(OBJDIR)/fs_td.o $(OBJDIR)/fs_tf.o $(OBJDIR)/fs_md.o $(OBJDIR)/fs_mf.o $(OBJDIR)/fs_filewrite.o \
	$(OBJDIR)/fs_openreadclose.o \
	$(OBJDIR)/netaddrinfo.o $(OBJDIR)/netconnect.o $(OBJDIR)/netlogportservice.o \
	$(OBJDIR)/netnameinfo.o $(OBJDIR)/netread.o $(OBJDIR)/netreadline.o $(OBJDIR)/netwriteline.o\
	$(OBJDIR)/netsocket.o $(OBJDIR)/netsslread.o $(OBJDIR)/netsslwrite.o $(OBJDIR)/netsslstart.o
	$(Q)echo "AR $@"
	$(Q)$(CROSS)$(AR) $(ARFLAGS) $@ $>

