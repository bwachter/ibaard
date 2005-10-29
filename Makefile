.include "system.mk"

VERSIONNR!=head -1 CHANGES|sed 's/://'
VERSION=aardmail-$(VERSIONNR)
#CURNAME=$(notdir $(shell pwd))
OS!=uname

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

.include "build.mk"

.c.o:
	$(Q)echo "CC $@"
	$(Q)$(DIET) $(CROSS)$(CC) $(CFLAGS) -c $< -o $@
.ifdef $(STRIP)
	$(Q)$(COMMENT) -$(CROSS)$(STRIP) $@
.endif

libibaard.a: $(OBJDIR)/cat.o $(OBJDIR)/cati.o $(OBJDIR)/aardlog.o $(OBJDIR)/strip.o \
	$(OBJDIR)/split.o $(OBJDIR)/kirahvi.o $(OBJDIR)/authinfo.o \
	$(OBJDIR)/fs_td.o $(OBJDIR)/fs_tf.o $(OBJDIR)/fs_md.o $(OBJDIR)/fs_mf.o $(OBJDIR)/fs_filewrite.o \
	$(OBJDIR)/fs_openreadclose.o $(OBJDIR)/fs_xgetcwd.o \
	$(OBJDIR)/netaddrinfo.o $(OBJDIR)/netconnect.o $(OBJDIR)/netlogportservice.o \
	$(OBJDIR)/netnameinfo.o $(OBJDIR)/netread.o $(OBJDIR)/netreadline.o $(OBJDIR)/netwriteline.o\
	$(OBJDIR)/netsocket.o $(OBJDIR)/netsslread.o $(OBJDIR)/netsslwrite.o $(OBJDIR)/netsslstart.o \
	$(OBJDIR)/netsslcacert.o $(OBJDIR)/msg.o $(OBJDIR)/emsg.o $(OBJDIR)/dmsg.o $(OBJDIR)/shell.o
	$(Q)echo "AR $@"
	$(Q)$(CROSS)$(AR) $(ARFLAGS) $@ $>

