#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([libzakauthe], [0.5.99], [azagli@libero.it])
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

PKG_CHECK_MODULES(LIBZAKAUTHE, [gmodule-2.0 >= 2.10.0
							   gtk+-3.0 >= 3.0.0])

AC_SUBST(LIBZAKAUTHE_CFLAGS)
AC_SUBST(LIBZAKAUTHE_LIBS)

PKG_CHECK_MODULES(LIBZAKCONFI, [libzakconfi >= 0.0.2], [AC_DEFINE(HAVE_LIBZAKCONFI, [1], [libzakconfi is present]), have_libzakconfi=yes], have_libzakconfi=no)

AM_CONDITIONAL(HAVE_LIBZAKCONFI, test x"$have_libzakconfi" = "xyes")
AC_SUBST(LIBZAKCONFI_CFLAGS)
AC_SUBST(LIBZAKCONFI_LIBS)

if test x"$have_libzakconfi" = "xyes"
then
	AC_SUBST(PKGCONFIG_DEPEND, ["libzakconfi"])
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([
  libzakauthe.pc
  Makefile
  src/Makefile
  data/Makefile
  docs/Makefile
  docs/reference/Makefile
  docs/reference/version.xml
])

AC_OUTPUT
