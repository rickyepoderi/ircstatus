#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ircstatus.c], [0.1], [https://github.com/rickyepoderi/ircstatus])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

# define plugin directory
AC_SUBST([plugindir], ['${libdir}/gedit/plugins'])

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_INSTALL
LT_PREREQ([2.2.6])
LT_INIT([disable-static])


# Use the C language and compiler for the following checks
AC_LANG([C])

# Checks for libraries.
PKG_CHECK_MODULES([PURPLE], [purple])
PKG_CHECK_MODULES([GLIB], [glib-2.0])

AC_PROG_INTLTOOL
AM_NLS
GETTEXT_PACKAGE="pidgin-ircstatus"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, ["$GETTEXT_PACKAGE"], [Define the gettext package to be used])

ALL_LINGUAS="es"
AM_GLIB_GNU_GETTEXT

# change libdir
AC_PREFIX_DEFAULT(${HOME}/.purple/plugins)
libdir='${prefix}'
AC_SUBST(lispdir)

AC_OUTPUT
