AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkglib_LTLIBRARIES = planner.la

planner_la_SOURCES  = ../planner.h mod.c
planner_la_SOURCES += ../mangle.h
planner_la_LIBADD   = ../../api/c/libux.la
planner_la_CPPFLAGS = $(AM_CPPFLAGS) -DLUX_MOD_NAME=planner
