
.PHONY: clean doc install tar rename upload deb maintainer-deb distclean

all: $(ALL)

$(SRCDIR)/version.h: CHANGES
	$(Q)echo "-> $@"
	$(Q)printf "#ifndef IBAARD_VERSION_H\n#define IBAARD_VERSION_H\n#define IBAARD_VERSION " > $@
	$(Q)printf '"$(VERSION)"\n' >> $@
	$(Q)printf '#define IBAARD_URL "http://bwachter.lart.info/projects/ibaard/"\n#endif\n' >> $@

clean:
	$(Q)echo "cleaning up"
	$(Q)$(RM) *.a *.exe *.lib $(OBJDIR)/*.{o,obj,lib} crammd5/*.{o,obj,lib} $(OBJDIR)/*.o dyn-*.mk
	$(Q)$(RM) $(OBJDIR)/*.gc* test tests/*.{o,obj,lib} tests/*.gc* *.gcov
	$(Q)$(RM) -R test-run coverage

distclean: clean
	$(Q)$(RM) Makefile.borland

doc:
	$(Q)echo "DOC"
	$(Q)doxygen doc/Doxyfile > /dev/null

debug: clean
	$(MAKE) DEBUG=1

coverage: debug
	$(MAKE) test DEBUG=1 
	$(Q)for i in src/*.gcda; do \
	  SOURCE=`echo $$i|sed 's/\.gcda/.c/'` ;\
	  gcov -o src $$SOURCE ;\
	done
	$(Q)mkdir coverage && mv *.gcov coverage/
	$(Q)echo -n "Files not covered at all: "
	$(Q)for i in src/*.c; do \
	  GCDA=`echo $$i|sed 's/\.c/.gcda/'` ;\
	  if [ ! -f $$GCDA ]; then echo -n " $$i"; fi ;\
	done
	$(Q)echo

dyn-tests.mk: build.mk system.mk
	$(Q)printf "test: " > $@
	$(Q)for i in `ls tests/*.c`; do \
	DEP=`echo $$i | sed "s/\.c/\.o/g"` ;\
	printf " $$DEP"; done >> $@
	$(Q)for i in 1; do  \
	printf '\n\t$$(Q)echo "LD $$@"\n' ;\
	printf '\t$$(Q)$$(DIET) $$(CROSS)$$(CC) $$(LDFLAGS) $$(INCLUDES) -o $$@ $(MK_ALL) $$(LIBS) -lcheck -L. -libaard\n' ;\
	printf '\t$$(Q)rm -Rf test-run && mkdir -p test-run && ./$$@\n\n' ;\
	done >> $@

dyn-library-targets.mk: $(SRCDIR)/version.h build.mk system.mk
	$(Q)echo > $@
	$(Q)printf "libibaard.a: " >> $@
	$(Q)for i in `ls src/*.c`; do \
	DEP=`echo $$i | sed "s/\.c/\.o/g" | sed 's,src/,\$$(OBJDIR)/,g'`;\
	printf " $$DEP"; done >> $@
	$(Q)for i in 1; do \
	printf '\n\t$$(Q)echo "AR $$@"\n';\
	printf '\t$$(Q)$$(CROSS)$$(AR) $$(ARFLAGS) $$@ $$^\n';\
	printf '\t$$(Q)$$(CROSS)$$(RANLIB) $$@\n';\
	printf '\n';\
	done >> $@

dyn-gmake.mk: dyn-library-targets.mk dyn-tests.mk
	$(Q)for i in 1; do \
	printf '%%.o: %%.c\n';\
	printf '\t$$(Q)echo "CC $$@"\n';\
	printf '\t$$(Q)$$(DIET) $$(CROSS)$$(CC) $$(CFLAGS) $(INCLUDES) -c $$< -o $$@\n';\
	printf 'ifdef $$(STRIP)\n';\
	printf '\t$$(Q)$$(COMMENT) -$$(CROSS)$$(STRIP) $$@\n';\
	printf 'endif\n\n';\
	done > $@
	echo 'include dyn-tests.mk dyn-library-targets.mk' >> $@

dyn-bsdmake.mk: dyn-library-targets.mk dyn-tests.mk
	$(Q)for i in 1; do \
	printf '.c.o:\n';\
	printf '\t$$(Q)echo "CC $$@"\n';\
	printf '\t$$(Q)$$(DIET) $$(CROSS)$$(CC) $$(CFLAGS) -c $$< -o $$@\n';\
	printf '.ifdef $$(STRIP)\n';\
	printf '\t$$(Q)$$(COMMENT) -$$(CROSS)$$(STRIP) $$@\n';\
	printf '.endif\n\n';\
	done > $@
	echo 'include dyn-tests.mk dyn-library-targets.mk' >> $@

mswin32.mak:
	$(Q)for i in 1; do \
	printf "!include <ntwin32.mak>\n\n" ;\
	printf "OBJDIR=src\\ \n" ;\
	printf "SRCDIR=src\\ \n" ;\
	printf ".PHONY: clean\n\n" ;\
	done > $@
	$(Q)printf "ibaard.lib: " >> $@
	$(Q)for i in `ls src/*.c`; do \
	  DEP=`echo $$i | sed "s/\.c/\.obj/g" | sed 's,src/,\$$(OBJDIR),g'`;\
	  printf " $$DEP";\
        done >> $@
	$(Q)for i in 1; do \
	  printf '\n\t$$(Q)echo "TLIB $$@"\n';\
	  printf '\t$$(Q)tlib $$(@F) /a $$**\n';\
	  printf '\n';\
	done >> $@
	$(Q)for i in 1; do \
	  printf '.c.obj:\n';\
	  printf '\t$$(Q)echo "CC $$@"\n';\
	  printf '\t$$(Q)$$(CC) $$(CFLAGS) $$(SSLCFLAGS) -o$$@ -c $$<\n';\
	  printf '\n';\
	done >> $@
	printf 'clean:\n\t$$(RM) *.exe *.lib *.tds src\*.obj\n\n' >> $@

Makefile.borland:
	$(Q)for i in 1; do \
	printf "CC=bcc32\n";\
	printf "LD=bcc32\nRM=del /F\n";\
	printf "LDFLAGS=-tWC -w- -k- -q -O2 -lq -lc -lx -lGpd -lGn -lGl -lw- -v\n";\
	printf "CFLAGS=-w- -O2 -q\n";\
	printf "LIBS=ws2_32.lib\n";\
	printf "!ifdef SSL\n";\
	printf "SSLLIBS=ssleay32.lib libeay32.lib\n";\
	printf "SSLCFLAGS=-DHAVE_SSL\n";\
	printf "!endif\n";\
	printf "Q=@\n";\
	printf 'OBJDIR=src\\\\\n';\
	printf 'SRCDIR=src\\\\\n';\
	printf ".PHONY: clean\n";\
	done > $@
	$(Q)printf "ibaard.lib: " >> $@
	$(Q)for i in `ls src/*.c`; do \
	DEP=`echo $$i | sed "s/\.c/\.obj/g" | sed 's,src/,\$$(OBJDIR),g'`;\
	printf " $$DEP"; done >> $@
	$(Q)for i in 1; do \
	printf '\n\t$$(Q)echo "TLIB $$@"\n';\
	printf '\t$$(Q)tlib $$(@F) /a $$**\n';\
	printf '\n';\
	done >> $@
	$(Q)for i in 1; do \
	printf '.c.obj:\n';\
	printf '\t$$(Q)echo "CC $$@"\n';\
	printf '\t$$(Q)$$(CC) $$(CFLAGS) $$(SSLCFLAGS) -o$$@ -c $$<\n';\
	printf '\n';\
	done >> $@
	$(Q)for i in 1; do \
	printf 'clean:\n';\
	printf '\t$$(RM) *.exe *.lib *.tds src\*.obj\n';\
	done >> $@


install: all
	# FIXME, install library and headers
	install -d $(DESTDIR)$(BINDIR)
	install -d $(DESTDIR)$(MANDIR)/man3
	install -m 644 doc/man/man3/*.3 $(DESTDIR)$(MANDIR)/man3

# legacy target, obsoleted by 'dist'
tar: distclean Makefile.borland rename
	$(Q)echo "building archive ($(VERSION).tar.bz2)"
	$(Q)cd .. && tar cvvf $(VERSION).tar.bz2 $(VERSION) --use=bzip2 --exclude CVS
	$(Q)cd .. && rm -Rf $(VERSION)

dist: Makefile.borland $(SRCDIR)/version.h
	$(Q)echo "building archive ($(VERSION).tar.bz2)"
	git archive --prefix=$(VERSION)/ HEAD | gzip > $(VERSION).tar.gz
	rm -f $(VERSION).zip
	tar2zip $(VERSION).tar.gz

rename:
	$(Q)if test $(CURNAME) != $(VERSION); then cd .. && cp -a $(CURNAME) $(VERSION); fi

upload: tar
	scp ../$(VERSION).tar.bz2 bwachter@lart.info:/home/bwachter/public_html/projects/download/snapshots

maintainer-deb: rename
	$(Q)cd ../$(VERSION) && ./debchangelog && dpkg-buildpackage -rfakeroot
	$(Q)cd .. && rm -Rf $(VERSION)

deb: rename
	$(Q)cd ../$(VERSION) && ./debchangelog && dpkg-buildpackage -rfakeroot -us -uc
	$(Q)cd .. && rm -Rf $(VERSION)

help:
	$(Q)echo "Variables for building:"
	$(Q)echo -e "SSL=0|1\t\tenable/disable SSL support and link against OpenSSL or any"
	$(Q)echo -e "\t\tcompatible library. Default is 0."
	$(Q)echo -e "DEBUG=0|1\tenable/disable debug build/stripping binaries. Default is 0."
	$(Q)echo -e "WIN32=0|1\tenable/disable build for Windows. Adds .exe to the binaries"
	$(Q)echo -e "\t\tand links against libws2_32, libwsock32 and libgdi32.  Default is 0."
	$(Q)echo -e "CROSS=\t\tset a cross compiler prefix, for example i386-pc-mingw32-."
	$(Q)echo -e "\t\tDefault is unset."
	$(Q)echo -e "DIET=\t\tset the diet-wrapper if you want to link against dietlibc."
	$(Q)echo -e "\t\tDefault is unset."
	$(Q)echo
	$(Q)echo "Available targets:"
	$(Q)echo -e "dep:\t\tCalculate dependencies. Should be done automatically"
	$(Q)echo -e "clean:\t\tRemove all files generated during build"
	$(Q)echo -e "doc:\t\tGenerate doxygen documentation"
	$(Q)echo -e "debug:\t\tDo a debug build (like make DEBUG=1)"
	$(Q)echo -e "coverage:\tDo a debug build, execute tests, and calculate test coverage information"
	$(Q)echo -e "install:\tInstall library and header files. Use DESTDIR for packaging purposes"
	$(Q)echo -e "tar:\t\tBuild a bzipped source tarball"
	$(Q)echo -e "help:\t\tDisplay this help text"
