!include <ntwin32.mak>

Q=@

!if [windeps.bat /owndeps] == 0
!include "dyn-own_deps.inc"
!else
die:
	$(Q)echo "Unable to set up own source files"
	$(Q)exit 1
!endif

RM=del
OBJDIR=src\\
SRCDIR=src\\
CFLAGS=/Isrc

EXAMPLES=ash.exe

all: ibaard.lib $(EXAMPLES)

.PHONY: clean

ibaard.lib: $(IBAARD_DEPS)
        $(Q)echo "LIB $@"
        $(Q)lib /nologo /out:$(@F) $**

ash.exe: ibaard.lib examples/ash.obj
	$(Q)echo "EXE $@"
	$(Q)link /nologo /out:$(@F) $**

.c.obj:
        $(Q)echo "CC $@"
        $(Q)$(CC) /nologo $(CFLAGS) $(SSLCFLAGS) -Fo$@ -c $<

clean:
        $(Q)$(RM) *.exe *.lib *.inc *.tds src\*.obj
