AM_LDFLAGS = -export-dynamic -module

# Static modules
pkginclude_HEADERS  = basename.h hash.h htab.h lazybuf.h shorthand.h switch.h

# Interfaces to dynamic modules
pkginclude_HEADERS += task.h
pkglib_LTLIBRARIES  = task.la
SUBDIRS             = task

task_la_CPPFLAGS = -DLUX_MOD_NAME=task
task_la_SOURCES  = task.h task.c
task_la_LIBADD   = ../api/libux.la