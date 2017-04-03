AM_CFLAGS = @GLIB_CFLAGS@ @PURPLE_CFLAGS@
AM_CFLAGS += -DPACKAGE=\"pidgin-ircstatus\" -DLOCALEDIR=NULL
lib_LTLIBRARIES = libircstatus.la
libircstatus_la_SOURCES = ircstatus.c
libircstatus_la_LDFLAGS = -avoid-version @GLIB_LIBS@ @PURPLE_LIBS@
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure config-h.in stamp-h.in $(AUX_DIST)

