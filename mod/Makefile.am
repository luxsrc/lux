AM_LDFLAGS = -export-dynamic -module

pkginclude_HEADERS = hash.h htab.h lazybuf.h task.h
pkglib_LTLIBRARIES = advect.la

advect_la_CPPFLAGS = -DLUX_MOD_NAME=advect
advect_la_SOURCES  = task.h advect.c
advect_la_LIBADD   = ../api/libux.la
