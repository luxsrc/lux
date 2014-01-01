AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined

bin_PROGRAMS = lux

lux_SOURCES  = src.h main.c unknown.c usage.c version.c
lux_SOURCES += ../../mod/failed.h ../../mod/job.h ../../mod/strutils.h ../../mod/switch.h
lux_LDADD    = ../c/libux.la
