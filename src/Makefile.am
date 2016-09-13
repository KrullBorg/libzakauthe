if HAVE_LIBZAKCONFI
	ZAKCONFI_GIR=ZakConfi-1.0
else
	ZAKCONFI_GIR=
endif

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

CLEANFILES =

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --warn-all
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = $(libzakauthe_la_SOURCES) $(libzakauthe_include_HEADERS)

ZakAuthe-1.0.gir: libzakauthe.la
ZakAuthe_1_0_gir_INCLUDES = GObject-2.0 Peas-1.0 Gtk-3.0 $(ZAKCONFI_GIR)
ZakAuthe_1_0_gir_CFLAGS = $(AM_CPPFLAGS)
ZakAuthe_1_0_gir_LIBS = libzakauthe.la
ZakAuthe_1_0_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += ZakAuthe-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
