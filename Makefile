# set up some basic programs
#DIET=`which diet`
CROSS=
CC=gcc
LD=gcc
AR=ar
RM=/bin/rm -f
INSTALL=install
DESTDIR=
BINDIR=/usr/bin
MANDIR=/usr/share/man
STRIP=

# set up some basic flags
VERSIONNR=$(shell head -1 CHANGES|sed 's/://')
VERSION=aardmail-$(shell head -1 CHANGES|sed 's/://')
CURNAME=$(notdir $(shell pwd))

LIBS=-L. -laardmail

ifdef DEBUG
CFLAGS=-g -Wall -W -pipe -Os
LDFLAGS=-g
else
CFLAGS=-Wall -W -pipe  -Os
LDFLAGS=-s
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

ARFLAGS=cru
Q=@

ifdef DEV
CFLAGS+=-D_DEV
ALL+=
endif

# you should not need to touch anything below this line

OBJDIR=obj
SRCDIR=src
PREFIX?=/usr
.PHONY: clean install tar rename upload deb maintainer-deb

ibaard.a: $(OBJDIR)/cat.o $(OBJDIR)/fs.o $(OBJDIR)/aardlog.o $(OBJDIR)/strip.o
	$(Q)echo "AR $@"
	$(Q)$(CROSS)$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(Q)echo "CC $@"
	$(Q)$(DIET) $(CROSS)$(CC) $(CFLAGS) -c $< -o $@
ifdef STRIP
	$(Q)$(COMMENT) -$(CROSS)$(STRIP) $@
endif
%.o: %.c
	$(Q)echo "CC $@"
	$(Q)$(DIET) $(CROSS)$(CC) $(CFLAGS) -c $< -o $@
ifdef STRIP
	$(Q)$(COMMENT) -$(CROSS)$(STRIP) $@
endif

clean:
	$(Q)echo "cleaning up"
	$(Q)$(RM) $(ALL) *.exe $(OBJDIR)/*.o *.a

install: all
	install -d $(DESTDIR)$(BINDIR)
	install -d $(DESTDIR)$(MANDIR)/man1
	install -m 755 $(ALL) $(DESTDIR)$(BINDIR)
	install -m 644 doc/man/*.1 $(DESTDIR)$(MANDIR)/man1

tar: clean rename
	$(Q)echo "building archive ($(VERSION).tar.bz2)"
	$(Q)cd .. && tar cvvf $(VERSION).tar.bz2 $(VERSION) --use=bzip2 --exclude CVS
	$(Q)cd .. && rm -Rf $(VERSION)

rename:
	$(Q)if test $(CURNAME) != $(VERSION); then cd .. && cp -a $(CURNAME) $(VERSION); fi

upload: tar
	scp ../$(VERSION).tar.bz2 bwachter@lart.info:/home/bwachter/public_html/projects/download/snapshots

