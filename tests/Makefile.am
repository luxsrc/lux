AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined

check_PROGRAMS = ck_strexception ck_libux

TESTS = $(check_PROGRAMS) ../api/sh/lux

ck_strexception_SOURCES = ck_strexception.c
ck_strexception_LDADD   = ../api/c/libux.la

ck_libux_SOURCES = ck_libux.c
ck_libux_LDADD   = ../api/c/libux.la
