AM_CPPFLAGS = -I$(top_builddir)
SUBDIRS =

check_PROGRAMS = ck_atomic_parafor ck_cookie ck_dope ck_lazybuf ck_mpool ck_mview ck_parray ck_queue ck_ring ck_stack ck_ticks_timer

ck_atomic_parafor_SOURCES = ck_atomic_parafor.c ../mod/atomic.h ../mod/parafor.h
ck_atomic_parafor_LDADD   = ../api/c/libux.la

ck_cookie_SOURCES = ck_cookie.c ../mod/cookie.h
ck_cookie_LDADD   = ../api/c/libux.la

ck_dope_SOURCES = ck_dope.c ../mod/dope.h
ck_dope_LDADD   = ../api/c/libux.la

ck_lazybuf_SOURCES = ck_lazybuf.c ../mod/lazybuf.h
ck_lazybuf_LDADD   = ../api/c/libux.la

ck_mpool_SOURCES = ck_mpool.c ../mod/mpool.h
ck_mpool_LDADD   = ../api/c/libux.la

ck_mview_SOURCES = ck_mview.c ../mod/mview.h
ck_mview_LDADD   = ../api/c/libux.la

ck_parray_SOURCES = ck_parray.c ../mod/parray.h
ck_parray_LDADD   = ../api/c/libux.la

ck_queue_SOURCES = ck_queue.c ../mod/queue.h
ck_queue_LDADD   = ../api/c/libux.la

ck_stack_SOURCES = ck_stack.c ../mod/stack.h
ck_stack_LDADD   = ../api/c/libux.la

ck_ring_SOURCES = ck_ring.c ../mod/ring.h
ck_ring_LDADD   = ../api/c/libux.la

ck_ticks_timer_SOURCES = ck_ticks_timer.c ../mod/ticks.h ../mod/timer.h
ck_ticks_timer_LDADD   = ../api/c/libux.la

if USING_MPI
SUBDIRS += mpi
endif

if USING_PTHREAD
SUBDIRS += pthread
endif

TESTS = $(check_PROGRAMS)
