AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_queue ck_ring ck_stack ck_tpool
TESTS = $(check_PROGRAMS)

ck_queue_SOURCES = ck_queue.c ../mod/queue.h
ck_queue_LDADD   = ../api/libux.la

ck_stack_SOURCES = ck_stack.c ../mod/stack.h
ck_stack_LDADD   = ../api/libux.la

ck_ring_SOURCES = ck_ring.c ../mod/ring.h
ck_ring_LDADD   = ../api/libux.la

ck_tpool_SOURCES = ck_tpool.c ../mod/tpool.h
ck_tpool_LDADD   = ../api/libux.la
