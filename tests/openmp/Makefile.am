AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_atomic_parafor

ck_atomic_parafor_SOURCES  = ck_atomic_parafor.c ../../mod/atomic.h ../../mod/parafor.h
ck_atomic_parafor_LDADD    = ../../api/c/libux.la
ck_atomic_parafor_CFLAGS   = $(OPENMP_CFLAGS)
ck_atomic_parafor_CPPFLAGS = $(AM_CPPFLAGS) -DENFORCE_OPENMP=1
ck_atomic_parafor_LDFLAGS  =

TESTS = $(check_PROGRAMS)
