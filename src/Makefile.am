AM_CPPFLAGS = $(LIBAUTE_CFLAGS) \
              $(LIBCONFI_CFLAGS) \
              -DLIBAUTE_PLUGINS_DIR=\""$(libdir)/libaute/plugins"\"

LIBS = $(LIBAUTE_LIBS) \
       $(LIBCONFI_LIBS)

lib_LTLIBRARIES = libaute.la

libaute_la_SOURCES = aute.c

libaute_la_LDFLAGS = -no-undefined

include_HEADERS = libaute.h
