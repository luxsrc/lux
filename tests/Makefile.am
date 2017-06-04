AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined

check_PROGRAMS = ck_libux

TESTS = $(check_PROGRAMS) ../api/sh/lux

ck_libux_SOURCES = ck_libux.c
ck_libux_LDADD   = ../api/c/libux.la
