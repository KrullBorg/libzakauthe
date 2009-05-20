#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([libaute], [0.0.1], [azagli@libero.it])
AC_CONFIG_SRCDIR([src/aute.c])

AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

AC_CANONICAL_SYSTEM

AC_LIBTOOL_WIN32_DLL

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

GTK_DOC_CHECK

# Checks for libraries.
PKG_CHECK_MODULES(LIBAUTE, [glib-2.0 >= 2.10.0
                            gmodule-2.0 >= 2.10.0
                            libconfi >= 0.0.2])

AC_SUBST(LIBAUTE_CFLAGS)
AC_SUBST(LIBAUTE_LIBS)

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
