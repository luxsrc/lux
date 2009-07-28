AM_LDFLAGS = -export-dynamic -module

pkginclude_HEADERS = hash.h htab.h lazybuf.h task.h
pkglib_LTLIBRARIES = task.la

task_la_CPPFLAGS = -DLUX_MOD_NAME=task
task_la_SOURCES  = task.h task.c
task_la_LIBADD   = ../api/libux.la
