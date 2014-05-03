#DIET=`which diet`
CROSS=
CC?=gcc
LD?=gcc
AR?=ar
RM?=/bin/rm -f
RANLIB?=ranlib
INSTALL?=install
DESTDIR=
BINDIR?=/usr/bin
MANDIR?=/usr/share/man
STRIP=

OBJDIR=src
SRCDIR=src
PREFIX?=/usr
MAKE?=make

ALL=libibaard.a doc

#set up some flags for later use
Q?=@
ARFLAGS=cru
#-pipe crashes on IRIX
WARN=-W -Wundef -Wno-endif-labels -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wmissing-declarations -Wno-multichar
#-Wunreachable-code
#-Wextra will clash with gcc3
#-Wdeclaration-after-statement
CFLAGS?=-Wall -W $(WARN)
COMMON_CFLAGS=-std=c99 -D_XPG6 -D_SVID_SOURCE -D_XOPEN_SOURCE=500 
INCLUDES=-Isrc
LDFLAGS=-s
DEBUG_CFLAGS=-g -Wall -W -O0 -fprofile-arcs -ftest-coverage
DEBUG_LDFLAGS=-g -fprofile-arcs

CFLAGS+=$(COMMON_CFLAGS)
DEBUG_CFLAGS+=$(COMMON_CFLAGS)

SOLARIS_LIBS=-lresolv -lsocket
WIN32_LIBS=-lws2_32 -lwsock32 -lgdi32

MATRIX_LIBS=-lmatrixsslstatic -lpthread
MATRIX_CFLAGS=-DHAVE_MATRIXSSL

GNUTLS_LIBS=-lgnutls-openssl -lgnutls -ltasn1 -lgcrypt -lgpg-error -lz
GNUTLS_CFLAGS=-DHAVE_SSL

SSL_LIBS=-lssl -lcrypto
SSL_CFLAGS=-DHAVE_SSL

DEV_CFLAGS=-D_DEV
BROKEN_CFLAGS=-D_BROKEN_IO
