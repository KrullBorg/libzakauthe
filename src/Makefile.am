AM_CPPFLAGS = $(LIBAUTE_CFLAGS) \
              $(LIBCONFI_CFLAGS) \
              -DLIB_ZAK_AUTHE_PLUGINS_DIR=\""$(libdir)/libzakauthe/plugins"\"

LIBS = $(LIBAUTE_LIBS) \
       $(LIBCONFI_LIBS)

lib_LTLIBRARIES = libzakauthe.la

libzakauthe_la_SOURCES = aute.c

libzakauthe_la_LDFLAGS = -no-undefined

include_HEADERS = libzakauthe.h
