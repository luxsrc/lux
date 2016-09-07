AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_atomic_parafor ck_tpool

CC=@PTHREAD_CC@

ck_atomic_parafor_SOURCES  = ck_atomic_parafor.c ../../mod/atomic.h ../../mod/parafor.h
ck_atomic_parafor_LDADD    = $(PTHREAD_LIBS) ../../api/c/libux.la
ck_atomic_parafor_CFLAGS   = $(PTHREAD_CFLAGS)
ck_atomic_parafor_CPPFLAGS = $(AM_CPPFLAGS)
ck_atomic_parafor_LDFLAGS  =

ck_tpool_SOURCES  = ck_tpool.c ../../mod/tpool.h
ck_tpool_LDADD    = $(PTHREAD_LIBS) ../../api/c/libux.la
ck_tpool_CFLAGS   = $(PTHREAD_CFLAGS)
ck_tpool_CPPFLAGS = $(AM_CPPFLAGS)
ck_tpool_LDFLAGS  =

TESTS = $(check_PROGRAMS)
