AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_atomic ck_cookie ck_dope ck_lazybuf ck_mpool ck_mview ck_parray ck_parafor ck_queue ck_ring ck_stack ck_tpool

ck_atomic_SOURCES = ck_atomic.c ../mod/atomic.h
ck_atomic_LDADD   = ../api/c/libux.la

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

ck_parafor_SOURCES = ck_parafor.c ../mod/parafor.h
ck_parafor_LDADD   = ../api/c/libux.la

ck_queue_SOURCES = ck_queue.c ../mod/queue.h
ck_queue_LDADD   = ../api/c/libux.la

ck_stack_SOURCES = ck_stack.c ../mod/stack.h
ck_stack_LDADD   = ../api/c/libux.la

ck_ring_SOURCES = ck_ring.c ../mod/ring.h
ck_ring_LDADD   = ../api/c/libux.la

ck_tpool_SOURCES = ck_tpool.c ../mod/tpool.h
ck_tpool_LDADD   = ../api/c/libux.la

if USING_MPI
check_PROGRAMS +=ck_mpi
ck_mpi_SOURCES  = ck_mpi.c ../mod/mpi.h
ck_mpi_LDADD    = ../api/c/libux.la
endif

TESTS = $(check_PROGRAMS)
