AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_tpool

CC=@PTHREAD_CC@

ck_tpool_SOURCES  = ck_tpool.c ../../mod/tpool.h
ck_tpool_LDADD    = $(PTHREAD_LIBS) ../../api/c/libux.la
ck_tpool_CFLAGS   = $(PTHREAD_CFLAGS)
ck_tpool_CPPFLAGS = $(AM_CPPFLAGS)
ck_tpool_LDFLAGS  =

TESTS = $(check_PROGRAMS)
