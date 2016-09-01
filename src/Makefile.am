AM_CPPFLAGS = $(LIBZAKAUTHE_CFLAGS) \
              $(LIBZAKCONFI_CFLAGS) \
              -DLIB_ZAK_AUTHE_PLUGINS_DIR=\""$(libdir)/$(PACKAGE)/plugins"\"

LIBS = $(LIBZAKAUTHE_LIBS) \
       $(LIBZAKCONFI_LIBS)

lib_LTLIBRARIES = libzakauthe.la

libzakauthe_la_SOURCES = aute.c \
                         authepluggable.c

libzakauthe_la_LDFLAGS = -no-undefined

libzakauthe_includedir = $(includedir)/$(PACKAGE)

libzakauthe_include_HEADERS = libzakauthe.h \
                              authe.h \
                              authepluggable.h
