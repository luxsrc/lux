AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]|^strexception'

lib_LTLIBRARIES = libux.la

libux_la_SOURCES = libux.c
libux_la_LIBADD  = ../../sys/libsys.la
