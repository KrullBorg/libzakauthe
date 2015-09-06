AM_CPPFLAGS = $(LIBAUTE_CFLAGS) \
              $(LIBCONFI_CFLAGS) \
              -DLIB_ZAK_AUTH_PLUGINS_DIR=\""$(libdir)/libzakauth/plugins"\"

LIBS = $(LIBAUTE_LIBS) \
       $(LIBCONFI_LIBS)

lib_LTLIBRARIES = libzakauth.la

libzakauth_la_SOURCES = aute.c

libzakauth_la_LDFLAGS = -no-undefined

include_HEADERS = libzakauth.h
