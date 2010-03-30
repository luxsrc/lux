AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_mpool ck_mview ck_tpool
TESTS = $(check_PROGRAMS)

ck_mpool_SOURCES = ck_mpool.c ../mod/mpool.h
ck_mpool_LDADD   = ../api/libux.la

ck_mview_SOURCES = ck_mview.c ../mod/mview.h
ck_mview_LDADD   = ../api/libux.la

ck_tpool_SOURCES = ck_tpool.c ../mod/tpool.h
ck_tpool_LDADD   = ../api/libux.la
