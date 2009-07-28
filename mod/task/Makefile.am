AM_LDFLAGS = -export-dynamic -module

taskdir = $(pkglibdir)/task
task_LTLIBRARIES   = advect.la

advect_la_CPPFLAGS = -DLUX_MOD_NAME=advect
advect_la_SOURCES  = ../task.h advect.c
advect_la_LIBADD   = ../../api/libux.la
