AM_CPPFLAGS = $(LIBZAKAUTHE_CFLAGS) \
              $(LIBZAKCONFI_CFLAGS) \
              -DLIB_ZAK_AUTHE_PLUGINS_DIR=\""$(libdir)/libzakauthe/plugins"\"

LIBS = $(LIBZAKAUTHE_LIBS) \
       $(LIBZAKCONFI_LIBS)

lib_LTLIBRARIES = libzakauthe.la

libzakauthe_la_SOURCES = aute.c

libzakauthe_la_LDFLAGS = -no-undefined

include_HEADERS = libzakauthe.h
