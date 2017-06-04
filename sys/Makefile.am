AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined

noinst_LTLIBRARIES = libsys.la

libsys_la_SOURCES =

# Remember to add the symbols defined below to the export list
# `-export-symbols-regex` in ../api/c/Makefile.am
libsys_la_SOURCES += ../mod/exception.h strexception.c
