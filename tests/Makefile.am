AM_CPPFLAGS = -I$(top_builddir)
SUBDIRS =

check_PROGRAMS = ck_cookie ck_dope ck_lazybuf ck_mpool ck_mview ck_parray ck_pvector ck_queue ck_ring ck_stack ck_typecode ck_ticks_timer

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

ck_pvector_SOURCES = ck_pvector.c ../mod/pvector.h
ck_pvector_LDADD   = ../api/c/libux.la

ck_queue_SOURCES = ck_queue.c ../mod/queue.h
ck_queue_LDADD   = ../api/c/libux.la

ck_ring_SOURCES = ck_ring.c ../mod/ring.h
ck_ring_LDADD   = ../api/c/libux.la

ck_stack_SOURCES = ck_stack.c ../mod/stack.h
ck_stack_LDADD   = ../api/c/libux.la

ck_typecode_SOURCES = ck_typecode.c
ck_typecode_LDADD   = ../api/c/libux.la

ck_ticks_timer_SOURCES = ck_ticks_timer.c ../mod/ticks.h ../mod/timer.h
ck_ticks_timer_LDADD   = ../api/c/libux.la

TESTS = $(check_PROGRAMS)

if USING_PTHREAD
SUBDIRS += pthread
TESTS   += pthread/ck_atomic_parafor pthread/ck_tpool
endif

if USING_OPENMP
SUBDIRS += openmp
TESTS   += openmp/ck_atomic_parafor
endif

if USING_OPENCL
SUBDIRS += opencl
TESTS   += opencl/ck_new opencl/ck_copy
endif

if USING_MPI
SUBDIRS += mpi
TESTS   += mpi/ck_mpi
endif

if USING_HDF5
SUBDIRS += hdf5
TESTS   += hdf5/ck_hdf5 hdf5/ck_type hdf5/ck_io
endif
