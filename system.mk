ALL=libibaard.a doc

SOLARIS_LIBS=-lresolv -lsocket
WIN32_LIBS=-lws2_32 -lwsock32 -lgdi32

DEV_CFLAGS=-D_DEV
BROKEN_CFLAGS=-D_BROKEN_IO
