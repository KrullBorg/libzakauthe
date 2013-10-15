#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([libaute], [0.1.1], [azagli@libero.it])
AC_CONFIG_SRCDIR([src/aute.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE(-Wall)

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_MAINTAINER_MODE

AC_CANONICAL_SYSTEM

AC_LIBTOOL_WIN32_DLL

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

GTK_DOC_CHECK

# Checks for libraries.
have_libconfi=no

PKG_CHECK_MODULES(LIBAUTE, [glib-2.0 >= 2.10.0
                            gobject-2.0 >= 2.10.0
                            gmodule-2.0 >= 2.10.0
                            gtk+-2.0 >= 2.0.0])

AC_SUBST(LIBAUTE_CFLAGS)
AC_SUBST(LIBAUTE_LIBS)

PKG_CHECK_MODULES(LIBCONFI, [libconfi >= 0.0.2], [AC_DEFINE(HAVE_LIBCONFI, [1], [libconfi is present]), have_libconfi=yes], have_libconfi=no)

AM_CONDITIONAL(HAVE_LIBCONFI, test x"$have_libconfi" = "xyes")
AC_SUBST(LIBCONFI_CFLAGS)
AC_SUBST(LIBCONFI_LIBS)

if test x"$have_libconfi" = "xyes"
then
	AC_SUBST(PKGCONFIG_DEPEND, [", libconfi"])
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([
  libaute.pc
  Makefile
  src/Makefile
  data/Makefile
  docs/Makefile
  docs/reference/Makefile
  docs/reference/version.xml
])

AC_OUTPUT
