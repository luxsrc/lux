AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined

bin_PROGRAMS = lux

lux_SOURCES = ../mod/failed.h ../mod/job.h ../mod/strutils.h ../mod/switch.h src.h main.c unknown.c usage.c version.c
lux_LDADD   = ../api/c/libux.la
